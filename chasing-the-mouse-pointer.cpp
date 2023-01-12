/***
   CTIS164 - Template Source Program
----------
STUDENT : Yakuphan Kucukkesim
STUDENT NUMBER: 21903430
SECTION : 2
HOMEWORK:3
----------
PROBLEMS:
----------
ADDITIONAL FEATURES:


    Press F2 to activate custom path registering, click some points on the screen (You'll see the steps as green circles)
    then press F2 to make the Kite fly over your custom route.


***/

#ifdef _MSC_VER
#define _CRT_SECURE_NO_WARNINGS
#endif
#include <GL/glut.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include <stdarg.h>
#include "vec.h"

#define WINDOW_WIDTH  1200
#define WINDOW_HEIGHT 800

#define TIMER_PERIOD  30 // Period for the timer.
#define TIMER_ON         1 // 0:disable timer, 1:enable timer

#define D2R 0.0174532

/* Global Variables for Template File */
bool up = false, down = false, right = false, left = false;
int  winWidth, winHeight; // current Window width and height

typedef struct kite_t {
    int rudder;
    vec_t bone;
} kite_t;


bool spacebarLever = false;

bool focus = false;

bool autoR = false;

bool autoM = false;


vec_t mouse = { 0, 0 };

kite_t kite = { 0, {0,0} };

vec_t cache = { 0, 0 };

vec_t autoMode[10] = { 0,0 };

int registeredPaths = 0;

//
// to draw circle, center at (x,y)
// radius r
//
void circle(int x, int y, int r)
{
#define PI 3.1415
    float angle;
    glBegin(GL_POLYGON);
    for (int i = 0; i < 100; i++)
    {
        angle = 2 * PI * i / 100;
        glVertex2f(x + r * cos(angle), y + r * sin(angle));
    }
    glEnd();
}

void circle_wire(int x, int y, int r)
{
#define PI 3.1415
    float angle;

    glBegin(GL_LINE_LOOP);
    for (int i = 0; i < 100; i++)
    {
        angle = 2 * PI * i / 100;
        glVertex2f(x + r * cos(angle), y + r * sin(angle));
    }
    glEnd();
}

void print(int x, int y, const char* string, void* font)
{
    int len, i;

    glRasterPos2f(x, y);
    len = (int)strlen(string);
    for (i = 0; i < len; i++)
    {
        glutBitmapCharacter(font, string[i]);
    }
}

// display text with variables.
// vprint(-winWidth / 2 + 10, winHeight / 2 - 20, GLUT_BITMAP_8_BY_13, "ERROR: %d", numClicks);
void vprint(int x, int y, void* font, const char* string, ...)
{
    va_list ap;
    va_start(ap, string);
    char str[1024];
    vsprintf_s(str, string, ap);
    va_end(ap);

    int len, i;
    glRasterPos2f(x, y);
    len = (int)strlen(str);
    for (i = 0; i < len; i++)
    {
        glutBitmapCharacter(font, str[i]);
    }
}

// vprint2(-50, 0, 0.35, "00:%02d", timeCounter);
void vprint2(int x, int y, float size, const char* string, ...) {
    va_list ap;
    va_start(ap, string);
    char str[1024];
    vsprintf_s(str, string, ap);
    va_end(ap);
    glPushMatrix();
    glTranslatef(x, y, 0);
    glScalef(size, size, 1);

    int len, i;
    len = (int)strlen(str);
    for (i = 0; i < len; i++)
    {
        glutStrokeCharacter(GLUT_STROKE_ROMAN, str[i]);
    }
    glPopMatrix();
}


int verX(int x, int y, int rudder) { return x * cos(rudder * D2R) - y * sin(rudder * D2R); }
int verY(int x, int y, int rudder) { return y * cos(rudder * D2R) + x * sin(rudder * D2R); }

void dKite(vec_t bone, int rudder) {
    glColor3ub(0, 98, 98);
    glBegin(GL_POLYGON);
    glVertex2f(verX(-40, 20, rudder) + bone.x, verY(-40, 20, rudder) + bone.y);
    glVertex2f(verX(-20, 0, rudder) + bone.x, verY(-20, 0, rudder) + bone.y);
    glVertex2f(verX(20, 0, rudder) + bone.x, verY(20, 0, rudder) + bone.y);
    glVertex2f(verX(40, 20, rudder) + bone.x, verY(40, 20, rudder) + bone.y);
    glEnd();


    glBegin(GL_POLYGON);
    glVertex2f(verX(-40, -20, rudder) + bone.x, verY(-40, -20, rudder) + bone.y);
    glVertex2f(verX(-20, 0, rudder) + bone.x, verY(-20, 0, rudder) + bone.y);
    glVertex2f(verX(20, 0, rudder) + bone.x, verY(20, 0, rudder) + bone.y);
    glVertex2f(verX(40, -20, rudder) + bone.x, verY(40, -20, rudder) + bone.y);
    glEnd();

    glColor3ub(0, 63, 63);
    glBegin(GL_TRIANGLES);
    glVertex2f(verX(40, 20, rudder) + bone.x, verY(40, 20, rudder) + bone.y);
    glVertex2f(verX(60, 0, rudder) + bone.x, verY(60, 0, rudder) + bone.y);
    glVertex2f(verX(40, -20, rudder) + bone.x, verY(40, -20, rudder) + bone.y);
    glEnd();

    glColor3ub(200, 255, 255);
    glBegin(GL_TRIANGLES);
    glVertex2f(verX(60, 20, rudder) + bone.x, verY(60, 20, rudder) + bone.y);
    glVertex2f(verX(80, 0, rudder) + bone.x, verY(80, 0, rudder) + bone.y);
    glVertex2f(verX(60, -20, rudder) + bone.x, verY(60, -20, rudder) + bone.y);
    glEnd();

    glColor3ub(0, 255, 255);
    circle(verX(40, 0, rudder) + bone.x, verY(40, 0, rudder) + bone.y, 4);


    circle(verX(80, 0, rudder) + bone.x, verY(80, 0, rudder) + bone.y, 4);


    circle(verX(60, 0, rudder) + bone.x, verY(60, 0, rudder) + bone.y, 4);

    glColor3ub(255, 0, 0);
    glBegin(GL_LINES);
    glVertex2f(verX(80, 0, rudder) + bone.x, verY(80, 0, rudder) + bone.y);
    glVertex2f(verX(-40, 0, rudder) + bone.x, verY(-40, 0, rudder) + bone.y);
    glEnd();


    glBegin(GL_LINES);
    glVertex2f(verX(-40, 20, rudder) + bone.x, verY(-40, 20, rudder) + bone.y);
    glVertex2f(verX(-40, -20, rudder) + bone.x, verY(-40, -20, rudder) + bone.y);
    glEnd();

    glColor3ub(255, 138, 138);
    circle(verX(100, 0, rudder) + bone.x, verY(100, 0, rudder) + bone.y, 4);


    circle(verX(80, 20, rudder) + bone.x, verY(80, 20, rudder) + bone.y, 4);


    circle(verX(80, -20, rudder) + bone.x, verY(80, -20, rudder) + bone.y, 4);

}

void dBackground() {
    glColor3f(0.4, 0.4, 0.8);
    glRectf(600, -400, -600, 400);
    glColor3f(0.4, 0.8, 0.8);
    glRectf(400, -300, -400, 300);
    glColor3f(1, 1, 1);
    vprint2(-600, 300, 0.5, "Yakuphan Kucukkesim");
    vprint2(-600, -350, 0.2, "Spacebar to %s, Current Angle %d, Mouse is %s", spacebarLever ? "STOP" : "CONTINUE", kite.rudder, focus ? "ON" : "DISPATCHED");
    glColor4f(0, 0, 0.7, 0.4);
    if (focus)
        circle(mouse.x, mouse.y, 4);

}

//
// To display onto window using OpenGL commands
//
void display() {
    //
    // clear window to black
    //
    glClearColor(0, 0, 0, 0);
    glClear(GL_COLOR_BUFFER_BIT);

    dBackground();
    dKite(kite.bone, kite.rudder);

    glColor3f(0, 1, 0);
    for (int i = 0; i < registeredPaths; i++) {
        circle(autoMode[i].x, autoMode[i].y, 10);
    }

    glutSwapBuffers();
}

//
// key function for ASCII charachters like ESC, a,b,c..,A,B,..Z
//
void onKeyDown(unsigned char key, int x, int y)
{
    // exit when ESC is pressed.
    if (key == 27)
        exit(0);

    if (key == ' ') spacebarLever = !spacebarLever;

    // to refresh the window it calls display() function
    glutPostRedisplay();
}

void onKeyUp(unsigned char key, int x, int y)
{
    // exit when ESC is pressed.
    if (key == 27)
        exit(0);

    // to refresh the window it calls display() function
    glutPostRedisplay();
}

//
// Special Key like GLUT_KEY_F1, F2, F3,...
// Arrow Keys, GLUT_KEY_UP, GLUT_KEY_DOWN, GLUT_KEY_RIGHT, GLUT_KEY_RIGHT
//
void onSpecialKeyDown(int key, int x, int y)
{
    // Write your codes here.
    switch (key) {
    case GLUT_KEY_UP: up = true; break;
    case GLUT_KEY_DOWN: down = true; break;
    case GLUT_KEY_LEFT: left = true; break;
    case GLUT_KEY_RIGHT: right = true; break;
    case GLUT_KEY_F2: autoR = !autoR; if (registeredPaths > 0) autoM = true; break;
    }

    // to refresh the window it calls display() function
    glutPostRedisplay();
}

//
// Special Key like GLUT_KEY_F1, F2, F3,...
// Arrow Keys, GLUT_KEY_UP, GLUT_KEY_DOWN, GLUT_KEY_RIGHT, GLUT_KEY_RIGHT
//
void onSpecialKeyUp(int key, int x, int y)
{
    // Write your codes here.
    switch (key) {
    case GLUT_KEY_UP: up = false; break;
    case GLUT_KEY_DOWN: down = false; break;
    case GLUT_KEY_LEFT: left = false; break;
    case GLUT_KEY_RIGHT: right = false; break;
    }

    // to refresh the window it calls display() function
    glutPostRedisplay();
}

//
// When a click occurs in the window,
// It provides which button
// buttons : GLUT_LEFT_BUTTON , GLUT_RIGHT_BUTTON
// states  : GLUT_UP , GLUT_DOWN
// x, y is the coordinate of the point that mouse clicked.
//
void onClick(int button, int stat, int x, int y)
{
    // Write your codes here.


    if (button == GLUT_LEFT_BUTTON && stat == GLUT_DOWN && autoR) {


        if (registeredPaths < 10) {


            autoMode[registeredPaths] = { mouse.x, mouse.y };
            registeredPaths++;

        }


    }


    // to refresh the window it calls display() function
    glutPostRedisplay();
}

//
// This function is called when the window size changes.
// w : is the new width of the window in pixels.
// h : is the new height of the window in pixels.
//
void onResize(int w, int h)
{
    winWidth = w;
    winHeight = h;
    glViewport(0, 0, w, h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(-w / 2, w / 2, -h / 2, h / 2, -1, 1);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    display(); // refresh window.
}

void onMoveDown(int x, int y) {
    // Write your codes here.



    // to refresh the window it calls display() function   
    glutPostRedisplay();
}

// GLUT to OpenGL coordinate conversion:
//   x2 = x1 - winWidth / 2
//   y2 = winHeight / 2 - y1
void onMove(int x, int y) {
    // Write your codes here.

    int translatedx = x - winWidth / 2;
    int translatedy = winHeight / 2 - y;

    mouse.x = translatedx;
    mouse.y = translatedy;
    focus = fabs(mouse.x) <= 400 && fabs(mouse.y) <= 300;



    // to refresh the window it calls display() function
    glutPostRedisplay();
}

void shiftArray(vec_t* autoModeArr) {
    int length = registeredPaths;
    for (int i = 0; i < registeredPaths; i++) {
        autoModeArr[i] = autoModeArr[i + 1];
    }
    registeredPaths--;
}


void executeAutoMode() {


    if (registeredPaths > 0) {

        spacebarLever = false;

        vec_t pointer = autoMode[0];
        vec_t subv = subV(pointer, kite.bone);

        if (magV(subv) < 50) {
            shiftArray(autoMode);
            return;
        }


        vec_t unitv = unitV(subv);
        kite.bone = addV(kite.bone, unitv);
        polar_t subrudder = rec2pol(unitv);
        kite.rudder = subrudder.angle;

    }
    else {
        autoM = false; autoR = false;
    }

}


#if TIMER_ON == 1
void onTimer(int v) {

    glutTimerFunc(TIMER_PERIOD, onTimer, 0);
    // Write your codes here.


    if (autoM) {
        executeAutoMode();
    }
    else {

        if (spacebarLever) {

            if (focus) {

                vec_t subv = subV(mouse, kite.bone); vec_t unitv = unitV(subv); cache = unitv;

                kite.bone = addV(kite.bone, unitv);

                polar_t subrudder = rec2pol(unitv);

                kite.rudder = subrudder.angle;
            }

            else {
                kite.bone = addV(kite.bone, cache);
                if (fabs(kite.bone.x) > 320) {
                    cache.x *= -1;

                    kite.rudder = (180 - kite.rudder) % 360;
                }
                if (fabs(kite.bone.y) > 220) {
                    cache.y *= -1;

                    kite.rudder = (360 - kite.rudder) % 360;
                }
            }

        }
    }



    // to refresh the window it calls display() function
    glutPostRedisplay(); // display()

}
#endif

void Init() {

    // Smoothing shapes
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

}

void main(int argc, char* argv[]) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE);
    glutInitWindowSize(WINDOW_WIDTH, WINDOW_HEIGHT);
    //glutInitWindowPosition(100, 100);
    glutCreateWindow("Yakuphan Kucukkesim - Autonom Kite");

    glutDisplayFunc(display);
    glutReshapeFunc(onResize);

    //
    // keyboard registration
    //
    glutKeyboardFunc(onKeyDown);
    glutSpecialFunc(onSpecialKeyDown);

    glutKeyboardUpFunc(onKeyUp);
    glutSpecialUpFunc(onSpecialKeyUp);

    //
    // mouse registration
    //
    glutMouseFunc(onClick);
    glutMotionFunc(onMoveDown);
    glutPassiveMotionFunc(onMove);

#if  TIMER_ON == 1
    // timer event
    glutTimerFunc(TIMER_PERIOD, onTimer, 0);
#endif

    Init();

    glutMainLoop();
}
