
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

#define WINDOW_WIDTH  1400
#define WINDOW_HEIGHT 800

#define TIMER_PERIOD  25 // Period for the timer.
#define TIMER_ON       1 // 0:disable timer, 1:enable timer

#define D2R 0.0174532

#define NumStars 20     //number of stars

/* Global Variables for Template File */
bool up = false, down = false, right = false, left = false;
int  winWidth, winHeight; // current Window width and height

//conrolers
int mode = 0;
bool animationstop = false;
bool clear = false;

//buton color
int cr, cg, cb,
cr1, cg1, cb1,
cr2, cg2, cb2;;

//stars
int starLocations[NumStars][2];
int starY = 0;
bool starControl = false;

//planet
int LplanetX, LplanetY;
int PlanetR, PlanetG, PlanetB;
bool planetControl = false;

//laser controllers
bool laserControl = false;
int  laserY = 0, laserx = 35, lasery;

//attacker
bool attackercontrol = false;
int Ax, Ay;

//animation
int locationX = 0, locationY = 0;
int direction = 0;
int animationMode = 0;
int timecounter = 39;

int count = 0;

//spawner
int clicklocationX = 0, clicklocationY = 0;



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


void constantbackground() {
    glColor3f(1, 1, 1);
    if (mode == 0 || mode == 1)
        vprint2(-150, 300, 0.25, "SpaceShip Simulation");
}

//menus
void start()
{
    glColor3b(cr, cg, cb);
    glRectf(-250, 100, 250, -100);
    glColor3f(1, 1, 1);
    vprint2(-75, -25, 0.5, "Start");
}
void stopmenu() {
    glColor3f(0.3, 0.3, 0.3);
    glRectf(-250, 100, 250, -100);
    glColor3b(cr2, cg2, cb2);
    glRectf(-200, 270, 200, 350);
    glColor3b(cr, cg, cb);
    glRectf(-250, -120, -10, -220);
    glColor3b(cr1, cg1, cb1);
    glRectf(250, -120, 10, -220);
    glColor3f(1, 1, 1);
    vprint2(-220, -25, 0.4, "Simulation Stoped");
    vprint2(-150, 300, 0.22, "Full Control List");
    vprint2(-220, -180, 0.25, "Resume");
    vprint2(100, -180, 0.25, "Exit");
}
void loosemenu() {
    glColor3f(0.3, 0.3, 0.3);
    glRectf(-250, 100, 250, -100);
    glRectf(-200, 270, 200, 350);
    glColor3b(cr, cg, cb);
    glRectf(-250, -120, -10, -220);
    glColor3b(cr1, cg1, cb1);
    glRectf(250, -120, 10, -220);
    glColor3f(1, 1, 1);
    vprint2(-150, -25, 0.5, "You Lose");
    vprint2(-150, 300, 0.22, "Score : %d", count);
    vprint2(-220, -180, 0.25, "Try Again");
    vprint2(100, -180, 0.25, "Exit");
}
void controlMenu() {
    glColor3b(cr, cg, cb);
    glRectf(-100, -150, 100, -250);
    glColor3f(1, 1, 1);
    vprint2(-200, 200, 0.25, "Control List");
    vprint2(-200, 150, 0.15, "Change Mode : <F1>");
    vprint2(-200, 120, 0.15, "SpaceShip Controls : <wasd> or <arrows>");
    vprint2(-200, 90, 0.15, "Triger Laser : <space>");
    vprint2(-200, 60, 0.15, "Respawn SpaceShip : <click>");
    vprint2(-200, 30, 0.15, "Stop menu : <Esc>");
    vprint2(-200, 0, 0.15, "Spawn Random Planet: <k>");
    vprint2(-200, -30, 0.15, "Recreate Stars : <x>");
    vprint2(-200, -60, 0.15, "StopTime(whileHold) : <p>");
    vprint2(-200, -90, 0.15, "ClearDisplay : <c>");
    vprint2(-200, -120, 0.15, "Open Game Mode : <f>");
    vprint2(-50, -210, 0.25, "Return");


}

//information 
void point(int Py) {
    vprint(-(winWidth / 2) + 20, (winHeight / 2) + Py, GLUT_BITMAP_8_BY_13, "Score        : %d", count);
}
void Dmode(int My) {
    switch (animationMode) {
    case 0:
        vprint(-(winWidth / 2) + 20, (winHeight / 2) + My, GLUT_BITMAP_8_BY_13, "Mode         : Simulation");
        break;
    case 1:
        vprint(-(winWidth / 2) + 20, (winHeight / 2) + My, GLUT_BITMAP_8_BY_13, "Mode         : Game");
        break;
    case 2:
        vprint(-(winWidth / 2) + 20, (winHeight / 2) + My, GLUT_BITMAP_8_BY_13, "Mode         : Auto");
        break;
    }
}
void directionS(int Dy) {
    switch (direction)
    {
    case 0:
        vprint(-(winWidth / 2) + 20, (winHeight / 2) + Dy, GLUT_BITMAP_8_BY_13, "Direction    : None");
        break;
    case 1:
        vprint(-(winWidth / 2) + 20, (winHeight / 2) + Dy, GLUT_BITMAP_8_BY_13, "Direction    : Up");
        break;
    case 2:
        vprint(-(winWidth / 2) + 20, (winHeight / 2) + Dy, GLUT_BITMAP_8_BY_13, "Direction    : Down");
        break;
    case 3:
        vprint(-(winWidth / 2) + 20, (winHeight / 2) + Dy, GLUT_BITMAP_8_BY_13, "Direction    : Left");
        break;
    case 4:
        vprint(-(winWidth / 2) + 20, (winHeight / 2) + Dy, GLUT_BITMAP_8_BY_13, "Direction    : Right");
        break;
    }
}
void ingamecontrol() {
    glColor3f(1, 1, 1);
    vprint(-(winWidth / 2) + 20, -(winHeight / 2) + 120, GLUT_BITMAP_8_BY_13, "Change Mode        : <F1>");
    vprint(-(winWidth / 2) + 20, -(winHeight / 2) + 95, GLUT_BITMAP_8_BY_13, "SpaceShip Controls : <wasd> or <arrows>");
    vprint(-(winWidth / 2) + 20, -(winHeight / 2) + 70, GLUT_BITMAP_8_BY_13, "Triger Laser       : <space>");
    vprint(-(winWidth / 2) + 20, -(winHeight / 2) + 45, GLUT_BITMAP_8_BY_13, "Respawn SpaceShip  : <click>");
    vprint(-(winWidth / 2) + 20, -(winHeight / 2) + 20, GLUT_BITMAP_8_BY_13, "Stop menu          : <Esc>");
}


//background
void printplanet(int x, int y, int r, int g, int b)
{
    if (planetControl)
    {
        glColor3b(r - 20, g - 20, b - 20);
        circle_wire(x, y, 100);
        glColor3b(r, g, b);
        circle(x, y, 100);
        glColor3b(r + 20, g + 20, b + 20);
        circle(x + 30, y + 30, 30);
        circle(x - 40, y + 50, 20);
        circle(x - 10, y - 60, 25);
        glColor3b(r - 20, g - 20, b - 20);
        circle_wire(x + 30, y + 30, 30);
        circle_wire(x - 40, y + 50, 20);
        circle_wire(x - 10, y - 60, 25);
    }
}
void planetRandom() {
    LplanetX = rand() % (winWidth / 2 - 100) + 100;
    LplanetY = rand() % (winHeight / 2 - 100) + 100;
    if (rand() % 2)
        LplanetX *= -1;

    PlanetR = rand() % 100 + 40;
    PlanetG = rand() % 100 + 40;
    PlanetB = rand() % 100 + 40;
    planetControl = true;

}
void Stars()
{
    if (starControl)
    {
        glColor3f(1, 1, 1);
        glPointSize(5);
        for (int i = 0; i < NumStars; i++)
        {
            glBegin(GL_POINTS);
            glVertex2f(starLocations[i][0], starLocations[i][1]);
            glEnd();
        }
    }
}
void StarRandom() {
    for (int i = 0; i < NumStars; i++) {
        starLocations[i][0] = rand() % (winWidth / 2 - 25) + 10;
        starLocations[i][1] = rand() % (winHeight / 2 - 25) + 10;
        if (rand() % 2)
            starLocations[i][0] *= -1;
        if (rand() % 2)
            starLocations[i][1] *= -1;
    }
    starControl = true;
}

//SpaceShip
void drawspacecraft(int x, int y)
{
    //booster
    glColor3f(0.2, 0.2, 0.9);
    glBegin(GL_TRIANGLES);
    glVertex2f(x + 30, y + 0);
    glVertex2f(x + -30, y + 0);
    glVertex2f(x + 0, y + -50);
    glColor3f(0.3, 0.3, 0.9);
    glBegin(GL_TRIANGLES);
    glVertex2f(x + 20, y + 20);
    glVertex2f(x + -20, y + 20);
    glVertex2f(x + 0, y + -30);
    glEnd();
    //torpedos
    glColor3f(1, 0.2, 0.2);
    glRectf(x + 30, y + 50, x + 40, y + 0);
    glRectf(x + -30, y + 50, x + -40, y + 0);
    glColor3f(1, 1, 1);
    glBegin(GL_TRIANGLES);
    glVertex2f(x + 30, y + 50);
    glVertex2f(x + 40, y + 50);
    glVertex2f(x + 35, y + 60);
    glVertex2f(x + -30, y + 50);
    glVertex2f(x + -40, y + 50);
    glVertex2f(x + -35, y + 60);
    glEnd();
    //main body
    glColor3f(0.3, 0, 0.2);
    glBegin(GL_QUADS);
    glVertex2f(x + 0, y + 100);
    glVertex2f(x + -50, y + -20);
    glVertex2f(x + 0, y + 0);
    glVertex2f(x + 50, y + -20);
    glEnd();

    glColor3f(0.4, 0, 0.2);
    glLineWidth(3);
    glBegin(GL_LINE_LOOP);
    glVertex2f(x + 0, y + 100);
    glVertex2f(x + -50, y + -20);
    glVertex2f(x + 0, y + 0);
    glVertex2f(x + 50, y + -20);
    glEnd();
    glColor3f(0.4, 0, 0.2);
    glLineWidth(3);
    glBegin(GL_LINES);
    glVertex2f(x + 0, y + 100);
    glVertex2f(x + 0, y + 0);
    glEnd();
    //cocpit
    glColor3f(1, 1, 1);
    glBegin(GL_TRIANGLES);
    glVertex2f(x + 0, y + 90);
    glVertex2f(x + 10, y + 60);
    glVertex2f(x + -10, y + 60);
    glEnd();

}

//laser
void laser(int x, int y)
{
    if (laserControl == true) {
        glLineWidth(4);
        glColor3f(1, 0, 0);

        glBegin(GL_LINES);
        glVertex2f(x, y + laserY);
        glVertex2f(x, y + 20 + laserY);
        glEnd();
    }
}
void laserloop()
{
    if (lasery + laserY < winHeight / 2) {
        laser(laserx, lasery);
        laser(laserx - 70, lasery);
    }
    else {
        laserControl = false;
        laserY = 0;
    }
}

//attacker
void attackerrandom() {
    Ax = rand() % (winWidth / 2 - 50) + 10;
    Ay = rand() % (winHeight / 2 - 100) + 80;
    if (rand() % 2)
        Ax *= -1;
}
void attacker() {
    if (attackercontrol == true)
    {
        glColor3f(1, 0, 0);
        circle(Ax, Ay, 30);
        glColor3f(1, 1, 1);
        circle_wire(Ax, Ay, 15);
    }
    if (((laserx > Ax - 30 && laserx < Ax + 30) || (laserx - 70 > Ax - 30 && laserx - 70 < Ax + 30)) && (lasery + 50 > Ay) && (lasery + 50 < Ay + 30))
    {
        attackerrandom();
        count++;
        laserControl = false;
    }
}

//color
void selected() {
    cr = 200;
    cg = 100;
    cb = 153;
}
void selected1() {
    cr1 = 200;
    cg1 = 100;
    cb1 = 153;
}
void selected2() {
    cr2 = 200;
    cg2 = 100;
    cb2 = 153;
}

//control
void stop() {
    laserControl = false;
    starControl = false;
    planetControl = false;
    attackercontrol = false;
}

//
// To display onto window using OpenGL commands
//
void display() {
    //
    // clear window to black
    //
    glClearColor(0.05, 0.05, 0.1, 0);
    glClear(GL_COLOR_BUFFER_BIT);



    switch (mode)
    {
    case 0:
        start();
        break;
    case 1:
        //Stars
        Stars();

        //Planets
        printplanet(LplanetX, LplanetY, PlanetR, PlanetG, PlanetB);

        //laser
        laserloop();
        //spaceShip
        drawspacecraft(locationX + clicklocationX, locationY + clicklocationY);

        //attacker
        attacker();

        //display information
        ingamecontrol();
        point(-20);
        Dmode(-45);
        directionS(-70);

        break;
    case 2:
        stopmenu();
        break;
    case 3:
        loosemenu();
        break;
    case 4:
        controlMenu();
        break;
    }
    constantbackground();

    glutSwapBuffers();
}

//
// key function for ASCII charachters like ESC, a,b,c..,A,B,..Z
//
void onKeyDown(unsigned char key, int x, int y)
{
    // exit when ESC is pressed.
    if (key == 27)
    {
        mode = 2;
        animationstop = true;
    }
    //laser control
    if (key == ' ')
    {
        laserControl = true;
        laserx = locationX + clicklocationX + 35;
        lasery = locationY + clicklocationY;
    }

    //location contol
    switch (key) {
    case 'w':
        direction = 1;
        locationY += 5;
        break;
    case 's':
        direction = 2;
        locationY -= 5;
        break;
    case 'a':
        direction = 3;
        locationX -= 5;
        break;
    case 'd':
        direction = 4;
        locationX += 5;
        break;
    }

    //planet Randomiser
    if (key == 'k')
        planetRandom();

    //star randomiser
    if (key == 'x')
        StarRandom();

    //attacker openner
    if (key == 'f' && attackercontrol == false)
    {
        animationMode = 1;
        attackerrandom();
        attackercontrol = true;
    }

    if (key == 'c') {
        stop();
        laserY = 0;
    }

    if (key == 'p')
        animationstop = true;


    // to refresh the window it calls display() function
    glutPostRedisplay();
}

void onKeyUp(unsigned char key, int x, int y)
{
    // exit when ESC is pressed.

    if (key == 'p')
        animationstop = false;

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
        direction = 1;
        locationY += 5;
        break;
    case GLUT_KEY_DOWN: down = true;
        direction = 2;
        locationY -= 5;
        break;
    case GLUT_KEY_LEFT: left = true;
        direction = 3;
        locationX -= 5;
        break;
    case GLUT_KEY_RIGHT: right = true;
        direction = 4;
        locationX += 5;
        break;
    }

    if (key == GLUT_KEY_F1)
        if (animationMode == 0)
            animationMode = 2;
        else {
            animationMode = 0;
            attackercontrol = false;
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

    //glut gl 
    int ix = x - winWidth / 2, iy = winHeight / 2 - y;

    // start button
    if (mode == 0 && ix > -250 && ix<250 && iy > -100 && iy < 100)
    {
        mode = 1;
        planetRandom();
        StarRandom();
    }

    //return Buttons
    if (mode == 2 && ix > -250 && ix<-10 && iy > -220 && iy < -90 && button == GLUT_LEFT_BUTTON && stat == GLUT_DOWN)
    {
        animationstop = false;
        mode = 1;
    }
    else if (mode == 3 && ix > -250 && ix<-10 && iy > -220 && iy < -90 && button == GLUT_LEFT_BUTTON && stat == GLUT_DOWN)
    {
        mode = 1;
        count = 0;
        stop();
        planetRandom();
        StarRandom();
        animationstop = false;
    }
    else if ((mode == 4) && ix > -100 && ix<100 && iy > -250 && iy < -150 && button == GLUT_LEFT_BUTTON && stat == GLUT_DOWN) {
        animationstop = false;
        mode = 1;
    }

    //exit buttons 
    if (mode == 2 && ix < 250 && ix>10 && iy > -220 && iy < -90 && button == GLUT_LEFT_BUTTON && stat == GLUT_UP)
        mode = 3;
    else if (mode == 3 && ix < 250 && ix>10 && iy > -220 && iy < -90 && button == GLUT_LEFT_BUTTON && stat == GLUT_DOWN)
        exit(0);

    // control button -200, 270, 200, 350
    if (mode == 2 && ix < 200 && ix>-200 && iy > 270 && iy < 350 && button == GLUT_LEFT_BUTTON && stat == GLUT_DOWN)
        mode = 4;

    //take location for spaceship
    if (mode == 1 && button == GLUT_LEFT_BUTTON && stat == GLUT_DOWN)
    {
        locationX = 0;
        locationY = 0;
        clicklocationX = ix;
        clicklocationY = iy;

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
    int ix = x - winWidth / 2, iy = winHeight / 2 - y;

    if (mode == 0 && ix > -250 && ix<250 && iy > -100 && iy < 100)
        selected();
    else  if ((mode == 2 || mode == 3) && ix < 250 && ix>10 && iy > -220 && iy < -90)
        selected1();
    else if ((mode == 2 || mode == 3) && ix > -250 && ix<-10 && iy > -220 && iy < -90)
        selected();
    else if ((mode == 4) && ix > -100 && ix<100 && iy > -250 && iy < -150)
        selected();
    else  if (mode == 2 && ix < 200 && ix>-200 && iy > 270 && iy < 350)
        selected2();
    else {
        cr = 70;
        cg = 70;
        cb = 70;
        cr1 = 70;
        cg1 = 70;
        cb1 = 70;
        cr2 = 70;
        cg2 = 70;
        cb2 = 70;
    }


    // to refresh the window it calls display() function
    glutPostRedisplay();
}

#if TIMER_ON == 1
void onTimer(int v) {

    glutTimerFunc(TIMER_PERIOD, onTimer, 0);
    // Write your codes here.
    if (!animationstop)
    {
        if (laserControl == true)
            lasery += 5;

        if (attackercontrol) {
            Ay--;
            if (Ay < -winHeight / 2 - 30)
                mode = 3;
        }
        if (planetControl)
        {
            LplanetY--;
            if (LplanetY < -winHeight / 2 - 100)
                LplanetY = winHeight / 2 + 100;
        }
        if (starControl)
        {
            for (int i = 0; i < NumStars; i++)
            {
                starLocations[i][1]--;
                if (starLocations[i][1] < -winHeight / 2)
                    starLocations[i][1] = winHeight / 2;
            }
        }

        if (animationMode == 2) {
            timecounter++;
            if (timecounter == 40) {
                direction = rand() % 4 + 1;
                timecounter = 0;
            }
            switch (direction)
            {
            case 1:
                locationY++;
                if (locationY + clicklocationY + 100 > winHeight / 2)
                    direction = 2;
                break;
            case 2:
                locationY--;
                if (locationY + clicklocationY - 20 < -(winHeight / 2))
                    direction = 1;
                break;
            case 3:
                locationX--;
                if (locationX + clicklocationX - 50 < -(winWidth / 2))
                    direction = 4;
                break;
            case 4:
                locationX++;
                if (locationY + clicklocationX + 50 > winWidth / 2)
                    direction = 3;
                break;
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
    glutCreateWindow("SpaceShip");

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

    //random timer
    srand(time(0));

#if  TIMER_ON == 1
    // timer event
    glutTimerFunc(TIMER_PERIOD, onTimer, 0);
#endif

    Init();

    glutMainLoop();
}
