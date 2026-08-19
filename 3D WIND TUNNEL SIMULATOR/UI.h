#ifndef UI_H
#define UI_H

#include <string>

class UI {
public:
    static void renderText(float x, float y, const std::string& text, int windowWidth, int windowHeight);
    static void drawHUD(float windSpeed, int currentObjectType, int windowWidth, int windowHeight,
                         bool useHeatmap = false, bool showPressure = false);
};

#endif
