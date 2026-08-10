#include <GL/freeglut.h>
#include <iostream>
#include <cmath>

float angle = 0.0f;

// Snowman color
float red = 1.0f;
float green = 1.0f;
float blue = 1.0f;

//speed
float speed = 0.5f;

// Camera looking direction
float lx = 0.0f;
float lz = -1.0f;

// Camera position
float x = 0.0f;
float z = 0.0f;


void changeSize(int w, int h)
{
    if (h == 0)
        h = 1;

    float ratio = (float)w / h;

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();

    glViewport(0, 0, w, h);

    gluPerspective(45.0, ratio, 1.0, 100.0);

    glMatrixMode(GL_MODELVIEW);
}


void drawSnowMan(){
    glColor3f(red , green , blue);

    glTranslatef(0.0f, 0.75f , 0.0f);
    glutSolidSphere(0.75f , 20 , 20);
}

void renderScene(void) {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glLoadIdentity();
	
	gluLookAt(	x, 1.0f, z,
			x+lx, 1.0f,  z+lz,
			0.0f, 1.0f,  0.0f);

    
	glColor3f(0.9f, 0.9f, 0.9f);
	glBegin(GL_QUADS);
		glVertex3f(-100.0f, 0.0f, -100.0f);
		glVertex3f(-100.0f, 0.0f,  100.0f);
		glVertex3f( 100.0f, 0.0f,  100.0f);
		glVertex3f( 100.0f, 0.0f, -100.0f);
	glEnd();

	for(int i = -3; i < 3; i++)
		for(int j=-3; j < 3; j++) {
			glPushMatrix();
			glTranslatef(i*10.0,0,j * 10.0);
			drawSnowMan();
			glPopMatrix();
		}

	glutSwapBuffers();
}


void processNormalKeys(unsigned char key, int x, int y) {

	

    if (key == 27)
		exit(0);

    switch(key){
        case 'w' :
            std::cout<<"pressed "<<key <<std::endl; 
            z += lz * speed;
            x += lx * speed;
            glutPostRedisplay();
            break;

        case 's' :
            std::cout<<"pressed "<<key <<std::endl;
            z -= lz * speed;
            x -= lx * speed;
            glutPostRedisplay();
            break;
        case 'a':
            angle -= 0.01f;
            lx = sin(angle);
            lz = -cos(angle);
            glutPostRedisplay();
            break;

        case 'd':
            angle += 0.01f;
            lx = sin(angle);
            lz = -cos(angle);
            glutPostRedisplay();
            break;

    }
}


int main(int argc, char** argv)
{
    glutInit(&argc, argv);

       glutInitDisplayMode(
        GLUT_DEPTH |
        GLUT_DOUBLE |
        GLUT_RGBA
    );


    glutInitWindowPosition(100, 100);
    glutInitWindowSize(320, 320);

    glutCreateWindow("My OpenGL Triangle");

    glEnable(GL_DEPTH_TEST);

    glutDisplayFunc(renderScene);
    glutReshapeFunc(changeSize);

    glutKeyboardFunc(processNormalKeys);
  
    glutMainLoop();

    return 0;
}