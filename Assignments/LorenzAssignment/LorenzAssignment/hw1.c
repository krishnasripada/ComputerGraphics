/*
 * Lorenz Attractor
 * CSCI 4229/5229 Assignment 1
 *
 * Key bindings
 *
 * q: Rotate X axis +
 * a: Rotate X axis -
 * w: Rotate Y axis +
 * s: Rotate Y axis -
 * e: Rotate Z axis +
 * d: Rotate Z axis -
 * r: Restart animation
 * f: Finish animation
 * t: Animation speed +
 * g: Animation speed -
 * UP: Rotate X axis +
 * DOWN: Rotate X axis -
 * LEFT: Rotate Y axis +
 * RIGHT: Rotate Y axis -
 * p: Lorenz Parameter b +
 * l: Lorenz Parameter b -
 * o: Lorenz Parameter s +
 * k: Lorenz Parameter s -
 * i: Lorenz Parameter r +
 * j: Lorenz Parameter r -
 */
//  OpenGL with prototypes for glext
#define GL_GLEXT_PROTOTYPES
#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif
#include <stdlib.h>
#include <time.h>
#include <stdio.h>
#include <string.h>
#include <stdarg.h>
#include <math.h>
int numPts = 50000;
double th=0;  //  Rotation angle
float pts[50000][3];
int view_rotx = 0, view_roty = 0, view_rotz = 0;
/*  Lorenz Parameters  */
double s  = 10;
double b  = 2.6666;
double r  = 28;

int iter = 0;
int iterInc = 10;
double zh=0;      //  Rotation
double dim=2;   // Dimension of orthogonal box


/*
 *  Convenience routine to output raster textd
 *  Use VARARGS to make this more flexible
 */
#define LEN 8192  //  Maximum length of text string
void Print(const char* format , ...)
{
    char    buf[LEN];
    char*   ch=buf;
    va_list args;
    //  Turn the parameters into a character string
    va_start(args,format);
    vsnprintf(buf,LEN,format,args);
    va_end(args);
    //  Display the characters one at a time at the current raster position
    while (*ch)
        glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18,*ch++);
}

/*
 *  Display the scene
 */
void display() {
    
    //  Clear the image
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    //  Push Matrix
    glPushMatrix();
    
    const double len=1;  //  Length of axes
    //  Set view angle
    glRotatef(view_rotx, 1.0, 0.0, 0.0);
    glRotatef(view_roty, 0.0, 1.0, 0.0);
    glRotatef(view_rotz, 0.0, 0.0, 1.0);
    
    glBegin(GL_LINE_STRIP);
    int i = 0;
    
    //Draw the points
    while( i < iter && i < numPts ) {
        glColor3fv(pts[i]);
        glVertex3fv(pts[i++]);
    }
    glEnd();
    
    if( iter < numPts ) {
        if( iter + iterInc > numPts ) iter = numPts;
        else iter+=iterInc;
    }
    
    //  Draw axes
    glColor3f(1,1,1);
    glBegin(GL_LINES);
    glVertex3d(0.0,0.0,0.0);
    glVertex3d(len,0.0,0.0);
    glVertex3d(0.0,0.0,0.0);
    glVertex3d(0.0,len,0.0);
    glVertex3d(0.0,0.0,0.0);
    glVertex3d(0.0,0.0,len);
    glEnd();
    
    //  Label axes
    glRasterPos3d(len,0.0,0.0);
    Print("X");
    glRasterPos3d(0.0,len,0.0);
    Print("Y");
    glRasterPos3d(0.0,0.0,len);
    Print("Z");
    //  Display parameters
    glWindowPos2i(5,5);
    Print("Lorenz Parameters: s=%.1lf, b=%.3lf, r=%.1lf",s,b,r);
    //  Flush and swap
    glFlush();
    glutSwapBuffers();
    //  Pop matrix
    glPopMatrix();
    
}

/*
 * Provides the points to plot on the screen
 */
void lorenzInputGenerator(){
    
    int i;
    /*  Coordinates  */
    double x = 1;
    double y = 1;
    double z = 1;
    /*  Time step  */
    double dt = 0.001;
    /*
     *  Integrate 50,000 steps (50 time units with dt = 0.001)
     *  Explicit Euler integration
     */
    for (i=0;i<numPts;i++)
    {
        double dx = s*(y-x);
        double dy = x*(r-z)-y;
        double dz = x*y - b*z;
        x += dt*dx;
        y += dt*dy;
        z += dt*dz;
        
        pts[i+1][0] = x/32;
        pts[i+1][1] = y/32;
        pts[i+1][2] = z/32;
        printf("%5f %5f %5f", pts[i+1][0], pts[i+1][1], pts[i+1][2]);
    }
}

/*
 *  GLUT calls this routine when the window is resized
 */

void reshape(int width, int height) {
    //  Ratio of the width to the height of the window
    double h = (height>0)? (double)width/height: 1;
    //  Set the viewport to the entire window
    glViewport(0, 0, width, height);
    //  Tell OpenGL we want to manipulate the projection matrix
    glMatrixMode(GL_PROJECTION);
    //  Undo previous transformations
    glLoadIdentity();
    //  Orthogonal projection box adjusted for the
    //  aspect ratio of the window
    glOrtho(-dim*h,+dim*h, -dim,+dim, -dim,+dim);
    //  Switch to manipulating the model matrix
    glMatrixMode(GL_MODELVIEW);
    //  Undo previous transformations
    glLoadIdentity();
}

/*
 *  GLUT calls this routine when an arrow key is pressed
 */
void special(int key, int x, int y) {
    switch (key) {
        //  Up arrow key - increase rotation view x by 5 degrees
        case GLUT_KEY_UP:
            view_rotx += 5;
            break;
        //  Down arrow key - decrease rotation view x by 5 degrees
        case GLUT_KEY_DOWN:
            view_rotx -= 5;
            break;
        //  Left arrow key - decrease rotation view y by 5 degrees
        case GLUT_KEY_LEFT:
            view_roty -= 5;
            break;
        //  Right arrow key - increase rotation view y by 5 degrees
        case GLUT_KEY_RIGHT:
            view_roty += 5;
            break;
        default:
            return;
    }
    //  Keep angles to +/-360 degrees
    view_rotx %= 360;
    view_roty %= 360;
    //  Tell GLUT it is necessary to redisplay the scene
    glutPostRedisplay();
}

/*
 * Key bindings that can be used as shortcuts to rotate the image/axes
 */
void key(unsigned char key, int x, int y) {
    switch (key) {
        case 'q':
            view_rotx += 5;
            break;
        case 'a':
            view_rotx -= 5;
            break;
        case 'w':
            view_roty += 5;
            break;
        case 's':
            view_roty -= 5;
            break;
        case 'e':
            view_rotz += 5;
            break;
        case 'd':
            view_rotz -= 5;
            break;
        case 'r':
            iter = 0;
            lorenzInputGenerator();
            break;
        case 'f':
            iter = numPts;
            break;
        case 't':
            iterInc += 5;
            break;
        case 'g':
            if( iterInc - 5 > 0 ) iterInc -= 5;
            break;
        case 'p':
            b +=0.333;
            break;
        case 'l':
            b -=0.333;
            break;
        case 'o':
            s +=1;
            break;
        case 'k':
            s -=1;
            break;
        case 'i':
            r +=1;
            break;
        case 'j':
            r -=1;
            break;
        default:
            return;
    }
    glutPostRedisplay();
}

/*
 * The Idle function is called when all the current events are processed
 */
void idle(void) {
    double t = glutGet(GLUT_ELAPSED_TIME)/1000.0;
    zh = fmod(500*t,360);
    glutPostRedisplay();
}

/*
 *  Start up GLUT and tell it what to do
 */
int main(int argc,char* argv[]) {
    
    printf("Key bindings\n");
    printf("==================================\n");
    printf("q: Rotate X axis +\n");
    printf("a: Rotate X axis -\n");
    printf("w: Rotate Y axis +\n");
    printf("s: Rotate Y axis -\n");
    printf("e: Rotate Z axis +\n");
    printf("d: Rotate Z axis -\n");
    printf("r: Restart animation\n");
    printf("f: Finish animation\n");
    printf("t: Animation speed +\n");
    printf("g: Animation speed -\n");
    printf("UP: Rotate X axis +\n");
    printf("DOWN: Rotate X axis -\n");
    printf("LEFT: Rotate Y axis +\n");
    printf("RIGHT: Rotate Y axis -\n");
    printf("p: Lorenz Parameter b +\n");
    printf("l: Lorenz Parameter b -\n");
    printf("o: Lorenz Parameter s +\n");
    printf("k: Lorenz Parameter s -\n");
    printf("i: Lorenz Parameter r +\n");
    printf("j: Lorenz Parameter r -\n");
    printf("==================================\n");

    
    //LorenzInputGenerator gives the bunch of input points that need to be plotted
    lorenzInputGenerator();
    //  Initialize GLUT and process user parameters
    glutInit(&argc,argv);
    //  Request double buffered, true color window
    glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE);
    //  Request 500 x 500 pixel window
    glutInitWindowSize(600, 600);
    //  Create the window
    glutCreateWindow("Assignment 1: Krishna Chaitanya Sripada");
    //  Tell GLUT to call "display" when the scene should be drawn
    glutDisplayFunc(display);
    //  Tell GLUT to call "reshape" when the window is resized
    glutReshapeFunc(reshape);
    //  Tell GLUT to call "special" when an arrow key is pressed
    glutSpecialFunc(special);
    //  Tell GLUT to call "key" when a key is pressed
    glutKeyboardFunc(key);
    //  Tell GLUT to call "idle" when processing all the current events
    glutIdleFunc(idle);
    //  Pass control to GLUT so it can interact with the user
    glutMainLoop();
    //  Return code
    return 0;
}
