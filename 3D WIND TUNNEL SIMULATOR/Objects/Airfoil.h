#ifndef AIRFOIL_H
#define AIRFOIL_H

#include "../Object.h"
#include <GL/freeglut.h>
#include <cmath>
#include <cstdio>

#ifndef M_PI
#define M_PI 3.14159265358979f
#endif

class AirfoilObject : public TestObject {
public:
    float chordLength;
    float thickness;
    float cx, cy, cz;
    float angleOfAttack; // Degrees (positive = nose up)

    AirfoilObject(float length, float t, float x, float y, float z)
        : chordLength(length), thickness(t), cx(x), cy(y), cz(z), angleOfAttack(8.0f) {}

    // Helper: NACA 4-digit symmetric thickness at normalized chord position xn in [0,1]
    static float nacaThickness(float xn, float tmax) {
        float s = std::sqrt(xn + 1e-6f);
        return 5.0f * tmax * (0.2969f*s - 0.1260f*xn - 0.3516f*xn*xn
                              + 0.2843f*xn*xn*xn - 0.1015f*xn*xn*xn*xn);
    }

    void draw() override {
        glPushMatrix();
        glTranslatef(cx, cy, cz);
        // Positive AOA: nose pitches UP = counterclockwise rotation around +Z
        glRotatef(+angleOfAttack, 0.0f, 0.0f, 1.0f);

        glColor3f(0.85f, 0.45f, 0.1f);

        float zStart =  1.5f;
        float zEnd   = -1.5f;
        int   N = 40;
        float c = chordLength;
        // NACA 4412: 4% camber, 40% chord camber position, 12% thickness
        float mCamber = 0.04f;
        float pCamber = 0.40f;

        auto camberLine = [&](float xn) -> float {
            if (xn < pCamber)
                return c * (mCamber/(pCamber*pCamber)) * (2.f*pCamber*xn - xn*xn);
            else
                return c * (mCamber/((1.f-pCamber)*(1.f-pCamber))) *
                       (1.f - 2.f*pCamber + 2.f*pCamber*xn - xn*xn);
        };

        // Upper surface
        glBegin(GL_QUAD_STRIP);
        for (int i = 0; i <= N; i++) {
            float xn   = (float)i / N;
            float xPos = (xn - 0.5f) * c;
            float yt   = nacaThickness(xn, thickness);
            float yUp  = camberLine(xn) + yt;
            glNormal3f(0, 1, 0);
            glVertex3f(xPos, yUp, zStart);
            glVertex3f(xPos, yUp, zEnd);
        }
        glEnd();

        // Lower surface
        glBegin(GL_QUAD_STRIP);
        for (int i = 0; i <= N; i++) {
            float xn   = (float)i / N;
            float xPos = (xn - 0.5f) * c;
            float yt   = nacaThickness(xn, thickness);
            float yLo  = camberLine(xn) - yt;
            glNormal3f(0, -1, 0);
            glVertex3f(xPos, yLo, zStart);
            glVertex3f(xPos, yLo, zEnd);
        }
        glEnd();

        glPopMatrix();
    }

    void applyDeflection(float px, float py, float pz,
                         float& vx, float& vy, float& vz) override
    {
        // Spanwise clip
        if (pz < cz - 1.6f || pz > cz + 1.6f) return;

        // Use incoming free-stream magnitude for all strength calculations
        float U = vx; // vx == windSpeed before any object perturbation

        // ---- Transform position to airfoil-local frame ----
        float radAOA = angleOfAttack * M_PI / 180.0f;
        float cosA   =  std::cos(radAOA);
        float sinA   =  std::sin(radAOA);

        float dx = px - cx;
        float dy = py - cy;

        float lx =  dx * cosA + dy * sinA;   // along chord
        float ly = -dx * sinA + dy * cosA;   // perpendicular to chord

        float r2 = lx*lx + ly*ly;
        if (r2 < 1e-6f) return;

        // ---- Block / redirect particles AT the solid surface ----
        // Check if point is inside the airfoil in local frame.
        // If inside: redirect velocity tangentially along surface and return.
        {
            float xn = lx / chordLength + 0.5f;   // 0..1 along chord
            if (xn >= 0.f && xn <= 1.f) {
                float yt = nacaThickness(xn, thickness);
                float yc = 0.04f * chordLength * 4.f * xn * (1.f - xn);

                float topY = yc + yt;
                float botY = yc - yt;

                if (ly < topY + 0.05f && ly > botY - 0.05f) {
                    // Point is inside or extremely close to the solid.
                    // Redirect velocity tangentially:
                    // Compute surface slope (dY/dX) at this chord position.
                    float dxn = 0.02f;
                    float xn2 = std::min(xn + dxn, 1.0f);
                    float yt2 = nacaThickness(xn2, thickness);
                    float yc2 = 0.04f * chordLength * 4.f * xn2 * (1.f - xn2);

                    bool aboveCamber = (ly >= yc);
                    float surfY  = aboveCamber ? topY  : botY;
                    float surfY2 = aboveCamber ? (yc2 + yt2) : (yc2 - yt2);

                    // Surface tangent in local frame (chord direction)
                    float txLocal = chordLength * dxn;   // step in x
                    float tyLocal = (surfY2 - surfY);    // rise on surface
                    float tLen    = std::sqrt(txLocal*txLocal + tyLocal*tyLocal);
                    if (tLen > 1e-6f) { txLocal /= tLen; tyLocal /= tLen; }

                    // Normal: perpendicular to tangent, pointing outward
                    float nxLocal = aboveCamber ? -tyLocal :  tyLocal;
                    float nyLocal = aboveCamber ?  txLocal : -txLocal;

                    // Project existing velocity onto tangent, add strong outward push
                    float localVxIn =  vx * cosA + vy * sinA;
                    float localVyIn = -vx * sinA + vy * cosA;
                    float proj = localVxIn * txLocal + localVyIn * tyLocal;

                    float newLocalVx = proj * txLocal + nyLocal * std::abs(U) * 1.5f;
                    float newLocalVy = proj * tyLocal + nyLocal * std::abs(U) * 1.5f;

                    // Rotate back to global frame (only perturbation)
                    vx = newLocalVx * cosA - newLocalVy * sinA;
                    vy = newLocalVx * sinA + newLocalVy * cosA;
                    return;
                }
            }
        }

        // ---- Doublet (symmetric body / thickness effect) ----
        // 2D potential-flow doublet for flow around a body in uniform stream:
        //   du = -κ * (x²-y²)/r⁴      (NEGATIVE sign = flow goes AROUND, not into)
        //   dv = -κ * 2xy   /r⁴
        // where κ = R² * U
        float R      = thickness * 0.55f;     // tighter body radius
        float kappa  = R * R * U;
        float inv_r4 = 1.0f / (r2 * r2);

        // Tight Gaussian: fades within ~0.7 chord
        float sigma2 = chordLength * chordLength * 0.22f;
        float env    = std::exp(-r2 / (2.0f * sigma2));

        float du_d = -kappa * (lx*lx - ly*ly) * inv_r4 * env;
        float dv_d = -kappa * (2.0f * lx * ly) * inv_r4 * env;

        // ---- Bound vortex (lift — Kutta-Joukowski) ----
        // Γ = π * U * c * sin(α_eff),  placed at aerodynamic centre (c/4 from leading edge)
        float alphaEff = radAOA + 0.052f;
        float gamma    = M_PI * U * chordLength * std::sin(alphaEff) * 0.45f;

        // Vortex at MIDCHORD (lx=0 in local frame).
        // This prevents the vortex from inducing strong downwash BELOW the wing.
        // At (lx=0, ly<0): dvx=0 → dv_v=0 → no downward push below midchord.
        float vcx = 0.0f;
        float dvx = lx - vcx;
        float dvy = ly;
        float vr2 = dvx*dvx + dvy*dvy;

        float du_v = 0.f, dv_v = 0.f;
        if (vr2 > 0.01f) {
            float coreR2 = (R * 0.3f) * (R * 0.3f);
            float evr2   = std::max(vr2, coreR2);
            float venv   = std::exp(-vr2 / (2.0f * sigma2));
            float factor = (gamma / (2.0f * M_PI)) / evr2 * venv;
            // u = +Γ/(2π) * y/r²,  v = -Γ/(2π) * x/r²
            du_v =  factor * dvy;
            dv_v = -factor * dvx;
        }

        // ---- Total PERTURBATION in local frame ----
        float pertX = du_d + du_v;
        float pertY = dv_d + dv_v;

        // ---- Wake deficit (narrow band behind trailing edge) ----
        float trailingX = chordLength * 0.5f;
        if (lx > trailingX) {
            float wakeHalf = thickness * 2.5f;
            float wakeEnv  = std::exp(-(ly*ly) / (wakeHalf*wakeHalf));
            float xDecay   = std::exp(-(lx - trailingX) * 0.6f);
            // Reduce only the X-perturbation (slow down the wake)
            pertX -= std::abs(U) * 0.3f * wakeEnv * xDecay;
        }

        // ---- CRITICAL: Rotate ONLY the PERTURBATION back to global frame ----
        // This prevents the false U*sinA upwash that was pushing all streamlines up.
        // The uniform flow (vx) is already in the correct global frame — don't touch it.
        vx += pertX * cosA - pertY * sinA;
        vy += pertX * sinA + pertY * cosA;
        // vz unchanged (2D potential flow)
    }

    bool isInsideSolid(float px, float py, float pz) override {
        if (pz < cz - 1.6f || pz > cz + 1.6f) return false;
        float radAOA = angleOfAttack * M_PI / 180.0f;
        float cosA =  std::cos(radAOA), sinA = std::sin(radAOA);
        float dx = px - cx, dy = py - cy;
        float lx =  dx*cosA + dy*sinA;
        float ly = -dx*sinA + dy*cosA;
        float xn = lx / chordLength + 0.5f;
        if (xn < 0.f || xn > 1.f) return false;
        float yt = nacaThickness(xn, thickness);
        float yc = 0.04f * chordLength * 4.f * xn * (1.f - xn);
        return (ly < yc + yt && ly > yc - yt);
    }

    void pushOut(float px, float py, float pz, float& outX, float& outY, float& outZ) override {
        outX = px; outY = py; outZ = pz;
        if (pz < cz - 1.6f || pz > cz + 1.6f) return;
        float radAOA = angleOfAttack * M_PI / 180.0f;
        float cosA =  std::cos(radAOA), sinA = std::sin(radAOA);
        float dx = px - cx, dy = py - cy;
        float lx =  dx*cosA + dy*sinA;
        float ly = -dx*sinA + dy*cosA;
        float xn = std::max(0.01f, std::min(0.99f, lx / chordLength + 0.5f));
        float yt = nacaThickness(xn, thickness);
        float yc = 0.04f * chordLength * 4.f * xn * (1.f - xn);
        float newLy = (ly >= yc) ? (yc + yt + 0.06f) : (yc - yt - 0.06f);
        outX = cx + ( lx*cosA - newLy*sinA);
        outY = cy + ( lx*sinA + newLy*cosA);
        outZ = pz;
    }

    void printDebug(float windSpd) {

        printf("\n=== AIRFOIL DEBUG ===\n");
        printf("Chord=%.2f  Thickness=%.2f  AOA=%.1f deg\n", chordLength, thickness, angleOfAttack);
        printf("Centre=(%.2f, %.2f, %.2f)  U=%.2f\n", cx, cy, cz, windSpd);
        struct Pt { float x, y; const char* lbl; };
        Pt pts[] = {
            { cx - 3.f*chordLength, cy,             "Far upstream    " },
            { cx - chordLength*0.6f,cy,             "Leading edge    " },
            { cx,                   cy+thickness*1.5f,"Above wing    " },
            { cx,                   cy-thickness*1.5f,"Below wing    " },
            { cx + chordLength*0.6f,cy,             "Trailing edge   " },
            { cx + 3.f*chordLength, cy,             "Far downstream  " },
        };
        printf("%-18s  vx      vy    speed  ratio\n","Location");
        for (auto& p : pts) {
            float pvx=windSpd, pvy=0, pvz=0;
            applyDeflection(p.x, p.y, cz, pvx, pvy, pvz);
            float spd=std::sqrt(pvx*pvx+pvy*pvy+pvz*pvz);
            printf("%-18s %6.3f %6.3f  %5.3f  %.3f\n",
                   p.lbl, pvx, pvy, spd, spd/windSpd);
        }
        printf("====================\n\n");
    }
};

#endif
