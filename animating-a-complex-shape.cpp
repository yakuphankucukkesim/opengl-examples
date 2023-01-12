/*********
STUDENT : Yakuphan K���kkesim
SECTION : 2
HOMEWORK: Animating a Complex Shape, HW-1
----------
PROBLEMS: The object is not moving in the opposite directionafter reaching to the end of the frame.
----------
ADDITIONAL FEATURES:
*********/

#ifdef _MSC_VER
#define _CRT_SECURE_NO_WARNINGS
#endif
#include <GL/glut.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include <stdarg.h>
#include <time.h>

#define WINDOW_WIDTH 1280
#define WINDOW_HEIGHT 720

#define TIMER_PERIOD  40 // Period for the timer.
#define TIMER_ON         1 // 0:disable timer, 1:enable timer

#define D2R 0.0174532

#define START 0
#define ACTION 1

/* Global Variables for Template File */
bool up = false, down = false, right = false, left = false;
int mode = START, winWidth, winHeight, xD = 0, yD = 0, sx, sy, cloud = 0, mAuto = 0, arrow = 4;

// structure that shows the directions
struct arrow_t {
    int left = 0;
    int right = 1;
    int up = 2;
    int down = 3;
}arrow_t;

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

void startSentences()
{
    // sentences that in the start page
    glColor3f(1, 0, 0);
    vprint(-100, 0, GLUT_BITMAP_8_BY_13, "Click ");
    glColor3ub(0, 0, 0);
    vprint(-55, 0, GLUT_BITMAP_8_BY_13, "to create/relocate the Bee");
}

void mainSentences()
{
    // sentences that in the action page
    glColor3f(0, 0, 0);
    vprint(-620, 335, GLUT_BITMAP_8_BY_13, "Mode:");
    vprint(-620, 320, GLUT_BITMAP_8_BY_13, "Direction:");
    vprint(450, 335, GLUT_BITMAP_8_BY_13, "YAKUPHAN KUCUKKESIM");
    vprint(-220, -350, GLUT_BITMAP_8_BY_13, "Press");
    vprint(-140, -350, GLUT_BITMAP_8_BY_13, "to switch the mode");
    vprint(95, -350, GLUT_BITMAP_8_BY_13, "to control the Bee");

    glColor3f(1, 0, 0);
    vprint(500, 320, GLUT_BITMAP_8_BY_13, "21903430");

    glColor3f(1, 0, 0);
    vprint(-177, -350, GLUT_BITMAP_8_BY_13, "<F1>");
    vprint(10, -350, GLUT_BITMAP_8_BY_13, "Arrow Keys");
}

void characterModelBee()
{
    // body and head
    glColor3ub(0, 0, 0);
    circle(0 + xD + sx, 0 + yD + sy, 60);
    circle(80 + xD + sx, 0 + yD + sy, 40);

    glColor3ub(255, 255, 51);
    circle(0 + xD + sx, 0 + yD + sy, 58);
    circle(80 + xD + sx, 0 + yD + sy, 38);

    // bee-sting
    glColor3f(0, 0, 0);
    glBegin(GL_TRIANGLES);
    glVertex2f(-100 + xD + sx, 0 + yD + sy);
    glVertex2f(-59 + xD + sx, 10 + yD + sy);
    glVertex2f(-59 + xD + sx, -10 + yD + sy);
    glEnd();

    // eyes
    glColor3ub(0, 0, 0);
    circle(110 + xD + sx, 0 + yD + sy, 12);
    circle(90 + xD + sx, 0 + yD + sy, 12);

    glColor3ub(255, 255, 255);
    circle(110 + xD + sx, 0 + yD + sy, 10);
    circle(90 + xD + sx, 0 + yD + sy, 10);

    glColor3ub(0, 0, 0);
    circle(110 + xD + sx, 0 + yD + sy, 5);
    circle(90 + xD + sx, 0 + yD + sy, 5);

    // mouth
    glColor3f(0, 0, 0);
    glBegin(GL_TRIANGLES);
    glVertex2f(85 + xD + sx, -15 + yD + sy);
    glVertex2f(115 + xD + sx, -15 + yD + sy);
    glVertex2f(100 + xD + sx, -23 + yD + sy);
    glEnd();

    // bee antenna
    glBegin(GL_LINES);
    glVertex2f(90 + xD + sx, 70 + yD + sy);
    glVertex2f(100 + xD + sx, 32 + yD + sy);
    glVertex2f(70 + xD + sx, 70 + yD + sy);
    glVertex2f(90 + xD + sx, 23 + yD + sy);
    glEnd();

    glColor3ub(0, 0, 0);
    circle(90 + xD + sx, 70 + yD + sy, 5);
    circle(70 + xD + sx, 70 + yD + sy, 5);

    // wing
    glColor3ub(0, 0, 0);
    glBegin(GL_TRIANGLES);
    glVertex2f(0 + xD + sx, 0 + yD + sy);
    glVertex2f(-100 + xD + sx, 100 + yD + sy);
    glVertex2f(-50 + xD + sx, 110 + yD + sy);
    glEnd();

    glColor3ub(255, 255, 255);
    glBegin(GL_LINES);
    glVertex2f(0 + xD + sx, 0 + yD + sy);
    glVertex2f(-70 + xD + sx, 90 + yD + sy);
    glEnd();

    // lines on body
    glColor3ub(0, 0, 0);
    glBegin(GL_LINES);
    glVertex2f(30 + xD + sx, 50 + yD + sy);
    glVertex2f(30 + xD + sx, -50 + yD + sy);

    glVertex2f(15 + xD + sx, 58 + yD + sy);
    glVertex2f(15 + xD + sx, -58 + yD + sy);

    glVertex2f(0 + xD + sx, 60 + yD + sy);
    glVertex2f(0 + xD + sx, -60 + yD + sy);

    glVertex2f(-15 + xD + sx, 58 + yD + sy);
    glVertex2f(-15 + xD + sx, -58 + yD + sy);

    glVertex2f(-30 + xD + sx, 50 + yD + sy);
    glVertex2f(-30 + xD + sx, -50 + yD + sy);

    glVertex2f(-45 + xD + sx, 40 + yD + sy);
    glVertex2f(-45 + xD + sx, -40 + yD + sy);

    glEnd();

    // legs
    glColor3ub(0, 0, 0);
    glBegin(GL_LINES);
    glVertex2f(30 + xD + sx, -50 + yD + sy);
    glVertex2f(16 + xD + sx, -70 + yD + sy);

    glVertex2f(20 + xD + sx, -55 + yD + sy);
    glVertex2f(10 + xD + sx, -70 + yD + sy);

    glVertex2f(-15 + xD + sx, -55 + yD + sy);
    glVertex2f(-25 + xD + sx, -70 + yD + sy);

    glVertex2f(-20 + xD + sx, -54 + yD + sy);
    glVertex2f(-32 + xD + sx, -70 + yD + sy);

    glEnd();
}

void cloudModels() {
    // cloud 1
    glColor3f(1, 1, 1);
    circle(-125 + cloud, 250, 30);
    circle(-180 + cloud, 250, 30);
    circle(-150 + cloud, 255, 33);
    circle(-150 + cloud, 245, 33);
    // cloud 2
    circle(110 + cloud, 210, 30);
    circle(55 + cloud, 210, 30);
    circle(85 + cloud, 215, 33);
    circle(85 + cloud, 205, 33);
    // cloud 3
    circle(330 + cloud, 300, 30);
    circle(275 + cloud, 300, 30);
    circle(305 + cloud, 305, 33);
    circle(305 + cloud, 295, 33);
}

void background()
{
    // grass
    glColor3ub(102, 204, 0);
    glRectf(700, -230, -700, -400);

    // trees
    glColor3ub(90, 51, 12);
    glRectf(-420, -350, -400, -150);
    glEnd();

    glColor3ub(20, 210, 20);
    circle(-370, -120, 50);
    circle(-410, -150, 50);
    circle(-450, -120, 50);
    circle(-400, -70, 50);
    circle(-450, -70, 50);

    glColor3ub(20, 210, 20);
    circle(-370, -120, 50);
    circle(-410, -150, 50);
    circle(-450, -120, 50);
    circle(-400, -70, 50);
    circle(-450, -70, 50);

    // apples
    glColor3ub(255, 0, 0);
    circle(-380, -130, 12);
    circle(-390, -100, 12);
    circle(-420, -140, 12);
    circle(-460, -90, 12);
}
//
// To display onto window using OpenGL commands
//

void display() {
    glClearColor(0, 0.8, 1, 0);
    glClear(GL_COLOR_BUFFER_BIT);

    switch (mode) {
    case START:
        mainSentences();
        startSentences();
        cloudModels();
        background();
        break;
    default:
        cloudModels();
        background();
        mainSentences();
        characterModelBee();
    }
    // auto and the manual mode
    if (mAuto == 0) {
        glColor3f(1, 0, 0);
        vprint(-575, 335, GLUT_BITMAP_8_BY_13, "MANUAL");
    }
    else
    {
        glColor3f(1, 0, 0);
        vprint(-575, 335, GLUT_BITMAP_8_BY_13, "AUTO");
    }
    // arrow keys
    switch (arrow)
    {
    case 0:  vprint(-535, 320, GLUT_BITMAP_8_BY_13, "LEFT");
        break;
    case 1: vprint(-535, 320, GLUT_BITMAP_8_BY_13, "RIGHT");
        break;
    case 2:  vprint(-535, 320, GLUT_BITMAP_8_BY_13, "UP");
        break;
    case 3:  vprint(-535, 320, GLUT_BITMAP_8_BY_13, "DOWN");
        break;
    case 4:  vprint(-535, 320, GLUT_BITMAP_8_BY_13, "DO NOT SET");
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
    case GLUT_KEY_UP: up = true;
        yD += 7;
        break;
    case GLUT_KEY_DOWN: down = true;
        yD -= 7;
        break;
    case GLUT_KEY_LEFT: left = true;
        xD -= 7;
        break;
    case GLUT_KEY_RIGHT: right = true;
        xD += 7;
        break;
    }

    if (key == GLUT_KEY_F1) {
        mAuto++;
        if (mAuto != 1)
            mAuto = 0;
    }

    switch (key) {
    case GLUT_KEY_LEFT:
        arrow = arrow_t.left;
        break;
    case GLUT_KEY_RIGHT:
        arrow = arrow_t.right;
        break;
    case GLUT_KEY_UP:
        arrow = arrow_t.up;
        break;
    case GLUT_KEY_DOWN:
        arrow = arrow_t.down;
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

    if (button == GLUT_LEFT_BUTTON && stat == GLUT_DOWN)
    {
        mode++;

        sx = (x - winWidth / 2) - xD;
        sy = (winHeight / 2 - y) + yD;
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



    // to refresh the window it calls display() function
    glutPostRedisplay();
}

#if TIMER_ON == 1
void onTimer(int v) {

    glutTimerFunc(TIMER_PERIOD, onTimer, 0);
    // Write your codes here.

    cloud--;

    if (mAuto != 0)
    {
        switch (arrow)
        {
        case 0:
            xD -= 7;
            break;
        case 1:
            xD += 7;
            break;
        case 2:
            yD += 7;
            break;
        case 3:
            yD -= 7;
            break;
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
    glutCreateWindow("HW-1: Animating a Complex Shape, A Bee is Flying in the Sky");

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

    srand(time(0));

    glutMainLoop();
}
