/*********
STUDENT : Yakuphan K���kkesim
SECTION : 2
HOMEWORK: Hitting a Complex Object with a Ball - HW2
----------
PROBLEMS: Game does not end when the time is end. My target does not change color after it hit.
----------
ADDITIONAL FEATURES: Firing object is complex, my target changes color fast
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
#include <stdlib.h>
#include <time.h>

#define WINDOW_WIDTH  1000
#define WINDOW_HEIGHT 600

#define TIMER_PERIOD  16 // Period for the timer.
#define TIMER_ON      1 // 0:disable timer, 1:enable timer

#define D2R 0.0174532

/* Global Variables for Template File */
bool up = false, down = false, right = false, left = false;
int  winWidth, winHeight; // current Window width and height
double count = 60.0;
int hitCnt = 0, r, g, b;
float fireX = 0,  fireY = 0;
float complexTXRand = 100, complexTYRand = 100;
float complexTAngle;
float angleB = 0;
float distance;
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

void info()
{
    glColor3f(0, 0, 0);
    vprint(-470, 270, GLUT_BITMAP_8_BY_13, "Yakuphan Kucukkesim");

    glColor3f(0, 0, 0);
    vprint(-470, 255, GLUT_BITMAP_8_BY_13, "21903430");

    glColor3f(0, 0, 0);
    vprint(-470, 240, GLUT_BITMAP_8_BY_13, "CTIS 164-2");
}

void firingObject() {

    glColor3f(0, 0, 0);
    circle(30, 0, 15);

    glColor3f(0, 0, 0);
    circle(-30, 0, 15);

    glColor3f(0, 0, 0);
    circle(0, 30, 15);

    glColor3f(0, 0, 0);
    circle(0, -30, 15);

    glColor3f(0, 94, 255);
    circle(0, 0, 35);

    glColor3f(213, 255, 0);
    glBegin(GL_TRIANGLES);
    glVertex2f(0, 0);
    glVertex2f(60 * cos(angleB * D2R) - 6 * sin(angleB * D2R), 60 * sin(angleB * D2R) + 6 * cos(angleB * D2R));
    glVertex2f(60 * cos(angleB * D2R) - (-6) * sin(angleB * D2R), 60 * sin(angleB * D2R) + (-6) * cos(angleB * D2R));
    glEnd();
}

void complexTarget()
{
    r = rand() % 255 - 1;
    g = rand() % 255 - 1;
    b = rand() % 255 - 1;

    glColor3ub(r, g, b);
    circle(complexTXRand, complexTYRand, 15);

    glColor3f(0, 0, 0);
    circle(complexTXRand - 6, complexTYRand, 2);
    circle(complexTXRand + 6, complexTYRand, 2);

    glColor3f(0, 0, 0);
    glBegin(GL_LINES);
    glVertex2f(-35 + complexTXRand, -20 + complexTYRand);
    glVertex2f(-15 + complexTXRand, complexTYRand);
    glEnd();
    
    glColor3f(0, 0, 0);
    glBegin(GL_LINES);
    glVertex2f(35 + complexTXRand, -20 + complexTYRand);
    glVertex2f(15 + complexTXRand, complexTYRand);
    glEnd();

    glColor3f(0, 0, 0);
    glBegin(GL_LINES);
    glVertex2f(-10 + complexTXRand, -50 + complexTYRand);
    glVertex2f(-10 + complexTXRand, -13 + complexTYRand);
    glEnd();

    glColor3f(0, 0, 0);
    glBegin(GL_LINES);
    glVertex2f(10 + complexTXRand, -50 + complexTYRand);
    glVertex2f(10 + complexTXRand, -13 + complexTYRand);
    glEnd();

    glColor3f(0, 0, 0);
    glBegin(GL_LINES);
    glVertex2f(complexTXRand, complexTYRand);
    glVertex2f(complexTXRand, complexTYRand);
    glEnd();

    glColor3f(0, 0, 0);
    glBegin(GL_LINES);
    glVertex2f(complexTXRand - 5, complexTYRand - 7);
    glVertex2f(complexTXRand + 5, complexTYRand - 7);
    glEnd();

    glColor3f(0, 0, 0);
    circle(complexTXRand - 10, complexTYRand + 12, 5);

    glColor3f(0, 0, 0);
    circle(complexTXRand + 10, complexTYRand + 12, 5);

    complexTAngle = atan2(complexTYRand, complexTXRand) / D2R;

    if (complexTAngle < 0)
        complexTAngle += 360;
}

void targetRespawn()
{
    complexTXRand = rand() % 1000 - 1000 / 2;
    complexTYRand = rand() % 600 - 600 / 2;
  
    complexTAngle = atan2(complexTYRand, complexTXRand) / D2R;

    if (complexTAngle < 0)
        complexTAngle += 360;
}

void ball()
{
    glColor3f(255, 153, 255);
    circle(fireX * cos(angleB * D2R) - fireY * sin(angleB * D2R), fireX * sin(angleB * D2R) + fireY * cos(angleB * D2R), 8);
}
//
// To display onto window using OpenGL commands
//
void display() {
    //
    // clear window to black
    //
    glClearColor(0.7, 0.3, 1, 0);
    glClear(GL_COLOR_BUFFER_BIT);

    info();
    ball();
    firingObject();
    complexTarget();

    glColor3ub(1, 1, 1);
    vprint(0, 15, GLUT_BITMAP_8_BY_13, "%.f", complexTAngle);
    vprint(0, -15, GLUT_BITMAP_8_BY_13, "%.f", angleB);
    vprint(-100, -280, GLUT_BITMAP_8_BY_13, "Number of targets hit: %d", hitCnt);
    vprint(310, 270, GLUT_BITMAP_8_BY_13, "Remaining time: %0.1f", count);

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
    case GLUT_KEY_UP: up = true; break;
    case GLUT_KEY_DOWN: down = true; break;
    case GLUT_KEY_LEFT: left = true; break;
    case GLUT_KEY_RIGHT: right = true; break;
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

    count -= 0.016;

    glutTimerFunc(TIMER_PERIOD, onTimer, 0);
    // Write your codes here.

    if ((int)angleB != (int)complexTAngle)
    {
        angleB += 1;
        fireX = 0;
        fireY = 0;
    }

    if (complexTXRand >= 0)
    {
        fireX += (cos(angleB * D2R)) * 15;
    }
    else
    {
        fireX -= (cos(angleB * D2R)) * 15;
    }

    if (fireX * cos(angleB * D2R) - fireY * sin(angleB * D2R) > complexTXRand - 15 && fireX * cos(angleB * D2R) - fireY * sin(angleB * D2R) < complexTXRand + 15 && fireX * sin(angleB * D2R) + fireY * cos(angleB * D2R) > complexTYRand - 15 && fireX * sin(angleB * D2R) + fireY * cos(angleB * D2R) < complexTYRand + 15)
    {
        fireY = 0;
        fireX = 0;
        targetRespawn();
        hitCnt++;
    }

    if (angleB > 360)
        angleB = 0;
    // to refresh the window it calls display() function
    glutPostRedisplay(); // display()
}
#endif

void Init() {

    // Smoothing shapes
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    srand(time(0));
}

void main(int argc, char* argv[]) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE);
    glutInitWindowSize(WINDOW_WIDTH, WINDOW_HEIGHT);
    //glutInitWindowPosition(100, 100);
    glutCreateWindow("Yakuphan Kucukkesim's HW2 - 21903430 - Hitting a Complex Object with a Ball");

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
