/* Assignment 2: 3D Scene
 *
 * Key bindings
 * m/M        Cycle through different sets of objects
 * a          Toggle axes
 * arrows     Change view angle
 * 0          Reset view angle
 * ESC        Exit
 *
 */


#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <math.h>
//  OpenGL with prototypes for glext
#define GL_GLEXT_PROTOTYPES
#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif

int axes=0;       //  Display axes
int mode=0;       //  Projection mode
int th=0;         //  Azimuth of view angle
int ph=0;         //  Elevation of view angle
int fov=55;       //  Field of view (for perspective)
double asp=1;     //  Aspect ratio
double dim=8.0;   //  Size of world
double zh=0;      //  Rotation of airplane
int i;

//  Macro for sin & cos in degrees
#define Cos(th) cos(3.1415927/180*(th))
#define Sin(th) sin(3.1415927/180*(th))

/*
 *  Convenience routine to output raster text
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
 *  Set projection
 */
static void Project()
{
    //  Tell OpenGL we want to manipulate the projection matrix
    glMatrixMode(GL_PROJECTION);
    //  Undo previous transformations
    glLoadIdentity();
    //  Perspective transformation
    if (mode)
        gluPerspective(fov,asp,dim/4,4*dim);
    //  Orthogonal projection
    else
        glOrtho(-asp*dim,+asp*dim, -dim,+dim, -dim,+dim);
    //  Switch to manipulating the model matrix
    glMatrixMode(GL_MODELVIEW);
    //  Undo previous transformations
    glLoadIdentity();
}

/*
 * Draw a house which also contains a door and a window
 */

static void drawHouse(double x,double y,double z,
                      double dx,double dy,double dz,
                      double th)
{
    //  Save transformation
    glPushMatrix();
    //  Offset
    glTranslated(x,y,z);
    glRotated(th,0,1,0);
    glScaled(dx,dy,dz);
    //House
    glBegin(GL_TRIANGLE_STRIP); // draw in triangle strips
    
    glColor3f(0.0, 1.0, 0.0);
    glVertex3f(0.0f, 0.75f, 0.0f); // top of the roof
    glVertex3f(-0.5f, 0.25f, 0.0f); // left corner of the roof
    glVertex3f(0.5f, 0.25f, 0.0f); // right corner of the roof
    glEnd();
        
    glBegin(GL_QUADS);
    glVertex3f(0.5f, 0.25f, 0.0f); // right corner of the roof
    glVertex3f(0.5f, -0.5f, 0.0f); //bottom right corner of the house
        
    //Door
    glVertex3f(0.1f, -0.5f, 0.0f);
    glVertex3f(0.1f, 0.25f, 0.0f);
    glEnd();
        
    glBegin(GL_QUADS);
    glVertex3f(0.1f, 0.25f, 0.0f);
    glVertex3f(0.1f, -0.1f, 0.0f);
    glVertex3f(-0.1f, -0.1f, 0.0f);
    glVertex3f(-0.1f, 0.25f, 0.0f);
    glEnd();
        
    glBegin(GL_QUADS);
    glVertex3f(-0.1f, 0.25f, 0.0f);
    glVertex3f(-0.5f, 0.25f, 0.0f); // left corner of the roof
    glVertex3f(-0.5f, -0.5f, 0.0f); // bottom left corner of the house
    glVertex3f(-0.1f, -0.5f, 0.0f);
    glEnd();
        
    glBegin(GL_QUADS);
    //Side
    glColor3f(0.0, 0.6, 1.0);
    glVertex3f(0.5f, -0.5f, 0.0f); //bottom right corner of the house
    //Window
    glVertex3f(0.5f, -0.5f, 0.5f);
    glVertex3f(0.5f, 0.25f, 0.5f);
    glVertex3f(0.5f, 0.25f, 0.0f); // right corner of the roof
    glEnd();
    
    glBegin(GL_QUADS);
    glVertex3f(0.5f, 0.25f, 0.5f);
    glVertex3f(0.5f, 0.0f, 0.5f);
    glVertex3f(0.5f, 0.0f, 1.0f);
    glVertex3f(0.5f, 0.25f, 1.0f);
    glEnd();
    
    glBegin(GL_QUADS);
    glVertex3f(0.5f, 0.25f, 1.0f);
    glVertex3f(0.5f, 0.25f, 1.5f); // middle right corner of the side
    glVertex3f(0.5f, -0.5f, 1.5f); // bottom right corner of the side
    glVertex3f(0.5f, -0.5f, 1.0f);
    glEnd();
    
    glBegin(GL_QUADS);
    glVertex3f(0.5f, -0.5f, 1.0f);
    glVertex3f(0.5f, -0.25f, 1.0f);
    glVertex3f(0.5f, -0.25f, 0.5f);
    glVertex3f(0.5f, -0.5f, 0.5f);
    glVertex3f(0.5f, -0.5f, 0.0f); //bottom right corner of the house
    glEnd();
    
    
    glBegin(GL_TRIANGLE_STRIP);
    //Side Roof
    glColor3f(0.687f, 0.765f, 0.597f);
    glVertex3f(0.5f, 0.25f, 0.0f); // right corner of the roof
    glVertex3f(0.5f, 0.25f, 1.5f); // middle right corner of the side
    glVertex3f(0.0f, 0.75f, 1.5f); // top right corner of the side
    glVertex3f(0.0f, 0.75f, 0.0f); // top of the roof
    glVertex3f(0.5f, 0.25f, 0.0f); // right corner of the roof
    
    //Other Side of Roof
    glColor3f(0.687f, 0.765f, 0.597f);
    glVertex3f(0.0f, 0.75f, 0.0f); // top of the roof
    glVertex3f(0.0f, 0.75f, 1.5f); // top right corner of the side
    glVertex3f(-0.5f, 0.25f, 1.5f); // middle right corner of the other side
    glVertex3f(-0.5f, 0.25f, 0.0f); // left corner of the roof
    glVertex3f(0.0f, 0.75f, 0.0f); // top of the roof
    glEnd();
    
    glBegin(GL_QUADS);
    // Other Side
    glColor3f(0.5, 0.5, 0.0);
    glVertex3f(-0.5f, 0.25f, 0.0f); // left corner of the roof
    glVertex3f(-0.5f, 0.25f, 1.5f); // middle right corner of the other side
    glVertex3f(-0.5f, -0.5f, 1.5f); // bottom right corner of the other side
    glVertex3f(-0.5f, -0.5f, 0.0f); // bottom left corner of the house
    glEnd();
    
    
    glBegin(GL_TRIANGLE_STRIP);
    //Back side of House
    glColor3f(0.0, 1.0, 0.0);
    glVertex3f(-0.5f, 0.25f, 0.0f); // left corner of the roof
    glVertex3f(-0.5f, 0.25f, 1.5f); // middle right corner of the other side
    
    glVertex3f(0.0f, 0.75f, 1.5f); // top right corner of the side
    glVertex3f(0.5f, 0.25f, 1.5f); // middle right corner of the side
    glVertex3f(-0.5f, 0.25f, 1.5f); // middle right corner of the other side
    glVertex3f(0.5f, -0.5f, 1.5f); // bottom right corner of the side
    glVertex3f(-0.5f, -0.5f, 1.5f); // bottom left corner of the other side house
    glVertex3f(-0.5f, 0.25f, 1.5f); // middle right corner of the other side
    glEnd();
    
     
    glBegin(GL_QUADS);
    //Under neath the house
    glColor3f(0.8, 0.8, 0.8);
    glVertex3f(0.5f, -0.5f, 0.0f);
    glVertex3f(-0.5f, -0.5f, 0.0f);
    glVertex3f(-0.5f, -0.5f, 1.5f);
    glVertex3f(0.5f, -0.5f, 1.5f);
    glEnd();
    
    //  Undo transformations
    glPopMatrix();
}

/*
 * Draw a road for the airplane to land
 */

static void drawRoad()
{
    
    // road line (dashed).
    glLineStipple(1, 0x00FF);
    glLineWidth(2);
    // Use line stipple for the dashed coloring on the road
    glEnable(GL_LINE_STIPPLE);
    glBegin(GL_LINES);
    glColor3f(1, 1, 0);
    glVertex3f(-10.0f, 0.0f, -1.5f);
    glVertex3f(10.0f, 0.0f, -1.5f);
    glEnd();
    glDisable(GL_LINE_STIPPLE);
    
    // Draw the road.
    glBegin(GL_QUADS);
    glColor3f(0.3f, 0.3f, 0.3f);
    glVertex3f(-10.0, 0.0f, -1.0);
    glVertex3f(-10.0, 0.0f, -2.0);
    glVertex3f(10.0, 0.0f, -2.0);
    glVertex3f(10.0, 0.0f, -1.0);
    glEnd();
    
}


/*
 *  Draw solid airplane
 *    at (x,y,z)
 *    nose towards (dx,dy,dz)
 *    up towards (ux,uy,uz)
 */
static void SolidPlane(double x,double y,double z,
                       double dx,double dy,double dz,
                       double ux,double uy, double uz)
{
    // Dimensions used to size airplane
    const double wid=0.05;
    const double nose=+0.50;
    const double cone= 0.20;
    const double wing= 0.00;
    const double strk=-0.20;
    const double tail=-0.50;
    //  Unit vector in direction of flght
    double D0 = sqrt(dx*dx+dy*dy+dz*dz);
    double X0 = dx/D0;
    double Y0 = dy/D0;
    double Z0 = dz/D0;
    //  Unit vector in "up" direction
    double D1 = sqrt(ux*ux+uy*uy+uz*uz);
    double X1 = ux/D1;
    double Y1 = uy/D1;
    double Z1 = uz/D1;
    //  Cross product gives the third vector
    double X2 = Y0*Z1-Y1*Z0;
    double Y2 = Z0*X1-Z1*X0;
    double Z2 = X0*Y1-X1*Y0;
    //  Rotation matrix
    double mat[16];
    mat[0] = X0;   mat[4] = X1;   mat[ 8] = X2;   mat[12] = 0;
    mat[1] = Y0;   mat[5] = Y1;   mat[ 9] = Y2;   mat[13] = 0;
    mat[2] = Z0;   mat[6] = Z1;   mat[10] = Z2;   mat[14] = 0;
    mat[3] =  0;   mat[7] =  0;   mat[11] =  0;   mat[15] = 1;
    
    //  Save current transforms
    glPushMatrix();
    //  Offset, scale and rotate
    glTranslated(x,y,z);
    glScaled(1.5f, 1.5f, 1.5f);
    glMultMatrixd(mat);
    //  Nose (4 sided)
    glColor3f(0,0,1);
    glBegin(GL_TRIANGLES);
    glVertex3d(nose, 0.0, 0.0);
    glVertex3d(cone, wid, wid);
    glVertex3d(cone,-wid, wid);
    
    glVertex3d(nose, 0.0, 0.0);
    glVertex3d(cone, wid,-wid);
    glVertex3d(cone,-wid,-wid);
    
    glVertex3d(nose, 0.0, 0.0);
    glVertex3d(cone, wid, wid);
    glVertex3d(cone, wid,-wid);
    
    glVertex3d(nose, 0.0, 0.0);
    glVertex3d(cone,-wid, wid);
    glVertex3d(cone,-wid,-wid);
    glEnd();
    //  Fuselage (square tube)
    glBegin(GL_QUADS);
    glVertex3d(cone, wid, wid);
    glVertex3d(cone,-wid, wid);
    glVertex3d(tail,-wid, wid);
    glVertex3d(tail, wid, wid);
    
    glVertex3d(cone, wid,-wid);
    glVertex3d(cone,-wid,-wid);
    glVertex3d(tail,-wid,-wid);
    glVertex3d(tail, wid,-wid);
    
    glVertex3d(cone, wid, wid);
    glVertex3d(cone, wid,-wid);
    glVertex3d(tail, wid,-wid);
    glVertex3d(tail, wid, wid);
    
    glVertex3d(cone,-wid, wid);
    glVertex3d(cone,-wid,-wid);
    glVertex3d(tail,-wid,-wid);
    glVertex3d(tail,-wid, wid);
    
    glVertex3d(tail,-wid, wid);
    glVertex3d(tail, wid, wid);
    glVertex3d(tail, wid,-wid);
    glVertex3d(tail,-wid,-wid);
    glEnd();
    //  Wings (plane triangles)
    glColor3f(1,1,0);
    glBegin(GL_TRIANGLES);
    glVertex3d(wing, 0.0, wid);
    glVertex3d(tail, 0.0, wid);
    glVertex3d(tail, 0.0, 0.5);
    
    glVertex3d(wing, 0.0,-wid);
    glVertex3d(tail, 0.0,-wid);
    glVertex3d(tail, 0.0,-0.5);
    glEnd();
    //  Vertical tail (plane triangle)
    glColor3f(1,0,0);
    glBegin(GL_POLYGON);
    glVertex3d(strk, 0.0, 0.0);
    glVertex3d(tail, 0.3, 0.0);
    glVertex3d(tail, 0.0, 0.0);
    glEnd();
    //  Undo transformations
    glPopMatrix();
}

/*
 * Draw a tree with the trunk being a cylinder and the leaves represented as a cone
 */

void drawTree(double x, double y, double z, double dx, double dy, double dz, double th, double height)
{
    // Save transformation
    glPushMatrix();
    // Offset
    glTranslated(x,y,z);
    glRotated(th,0,1,0);
    glScaled(dx,dy,dz);
    
    for(i = 0; i < 360; i++)
    {
        // Save transformation
        glPushMatrix();
        // Offset
        glTranslated(x + dx*cos(i), y, z + dz*sin(i));
        glLineWidth(2);
        glBegin(GL_LINES);
        glColor3f(0.4, 0.3, 0.1);
        glVertex3f(0,0,0);
        glVertex3f(0.0,height,0.0);
        glEnd();
        
        // Undo transformations
        glPopMatrix();
    }
    
    for(i = 0; i < 360; i++)
    {
        // Save transformation
        glPushMatrix();
        // Offset
        glTranslated(x,y,z);
        glLineWidth(2);
        glBegin(GL_LINES);
        glColor3f(0.0, 0.9, 0.0);
        glVertex3f(0.0,height*2,0.0);
        glVertex3f(2*dx*cos(i),height,2*dz*sin(i));
        glEnd();
        
        // Undo transformations
        glPopMatrix();
    }
    
    // Undo transformations
    glPopMatrix();
}


/*
 *  OpenGL (GLUT) calls this routine to display the scene
 */
void display()
{
    const double len=5.0;  //  Length of axes
    glClearColor(0.95f, 0.95f, 0.95f, 1.0f);
    //  Erase the window and the depth buffer
    glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
    //  Enable Z-buffering in OpenGL
    glEnable(GL_DEPTH_TEST);
    //  Undo previous transformations
    glLoadIdentity();
    //  Perspective - set eye position
    if (mode)
    {
        double Ex = -2*dim*Sin(th)*Cos(ph);
        double Ey = +2*dim        *Sin(ph);
        double Ez = +2*dim*Cos(th)*Cos(ph);
        gluLookAt(Ex,Ey,Ez , 0,0,0 , 0,Cos(ph),0);
    }
    //  Orthogonal - set world orientation
    else
    {
        glRotatef(ph,1,0,0);
        glRotatef(th,0,1,0);
    }
    
    glColor3f(1.0, 0.0, 0.0);
    //Draw House
    drawHouse(-2.5,1.0,0.0,  2.0, 2.0, 2.0,    0);
    drawHouse(0,0.75,0,        1.5, 1.5, 1.5,    0);
    drawHouse(2.5,0.5,0.0,      1.0, 1.0, 1.0,    0);
    
    //Draw the road
    drawRoad();
    
    //Draw Houses on other side of the road
    drawHouse(-2.5,1.0,-3.5,  2.0, 2.0, 2.0,    180);
    drawHouse(0,0.75,-3.5,        1.5, 1.5, 1.5,    180);
    drawHouse(2.5,0.5,-3.5,      1.0, 1.0, 1.0,    180);
    
    //Draw airplane
    
    //  Solid Airplane
    SolidPlane(6*Cos(zh),5.5f, 6*Sin(zh) ,-Sin(zh),0,Cos(zh) , 0,10,0);
    //  Airplane landed
    SolidPlane( -1.0, 0.1, -1.5 , 1,0,0 , 0, 1,0);
    
    //Draw a tree
    drawTree(3.5, 0.0, 2.0, 0.5, 1.0, 0.5, 0, 0.5);
    drawTree(-3.5, 0.0, 2.0, 0.5, 1.0, 0.5, 0, 0.5);
    drawTree(3.5, 0.0, -2.0, 0.5, 1.0, 0.5, 0, 0.5);
    drawTree(-3.5, 0.0, -2.0, 0.5, 1.0, 0.5, 0, 0.5);
    
    glLineWidth(1);
    //  Draw axes
    glColor3f(0,0,0);
    if (axes)
    {
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
    }
    //  Display parameters
    glWindowPos2i(5,5);
    Print("Angle=%d,%d  Dim=%.1f FOV=%d Projection=%s",th,ph,dim,fov,mode?"Perpective":"Orthogonal");
    //  Render the scene and make it visible
    glFlush();
    glutSwapBuffers();
}

/*
 *  GLUT calls this routine when an arrow key is pressed
 */
void special(int key,int x,int y)
{
    //  Right arrow key - increase angle by 5 degrees
    if (key == GLUT_KEY_RIGHT)
        th += 5;
    //  Left arrow key - decrease angle by 5 degrees
    else if (key == GLUT_KEY_LEFT)
        th -= 5;
    //  Up arrow key - increase elevation by 5 degrees
    else if (key == GLUT_KEY_UP)
        ph += 5;
    //  Down arrow key - decrease elevation by 5 degrees
    else if (key == GLUT_KEY_DOWN)
        ph -= 5;
    //  PageUp key - increase dim
    else if (key == GLUT_KEY_PAGE_UP)
        dim += 0.1;
    //  PageDown key - decrease dim
    else if (key == GLUT_KEY_PAGE_DOWN && dim>1)
        dim -= 0.1;
    //  Keep angles to +/-360 degrees
    th %= 360;
    ph %= 360;
    //  Update projection
    Project();
    //  Tell GLUT it is necessary to redisplay the scene
    glutPostRedisplay();
}

/*
 *  GLUT calls this routine when a key is pressed
 */
void key(unsigned char ch,int x,int y)
{
    //  Exit on ESC
    if (ch == 27)
        exit(0);
    //  Reset view angle
    else if (ch == '0')
        th = ph = 0;
    //  Toggle axes
    else if (ch == 'a' || ch == 'A')
        axes = 1-axes;
    //  Switch display mode
    else if (ch == 'm' || ch == 'M')
        mode = 1-mode;
    //  Change field of view angle
    else if (ch == '-' && ch>1)
        fov--;
    else if (ch == '+' && ch<179)
        fov++;
    //  Reproject
    Project();
    //  Tell GLUT it is necessary to redisplay the scene
    glutPostRedisplay();
}

/*
 *  GLUT calls this routine when the window is resized
 */
void reshape(int width,int height)
{
    //  Ratio of the width to the height of the window
    asp = (height>0) ? (double)width/height : 1;
    //  Set the viewport to the entire window
    glViewport(0,0, width,height);
    //  Set projection
    Project();
}

/*
 *  GLUT calls this toutine when there is nothing else to do
 */
void idle()
{
    double t = glutGet(GLUT_ELAPSED_TIME)/1000.0;
    zh = fmod(120*t,360);
    glutPostRedisplay();
}

/*
 *  Start up GLUT and tell it what to do
 */
int main(int argc,char* argv[])
{
    //  Initialize GLUT
    glutInit(&argc,argv);
    //  Request double buffered, true color window with Z buffering at 600x600
    glutInitDisplayMode(GLUT_RGB | GLUT_DEPTH | GLUT_DOUBLE);
    glutInitWindowSize(600,600);
    //  Create the window
    glutCreateWindow("Assignment 2: Krishna Chaitanya Sripada");
    //  Tell GLUT to call "idle" when there is nothing else to do
    glutIdleFunc(idle);
    //  Tell GLUT to call "display" when the scene should be drawn
    glutDisplayFunc(display);
    //  Tell GLUT to call "reshape" when the window is resized
    glutReshapeFunc(reshape);
    //  Tell GLUT to call "special" when an arrow key is pressed
    glutSpecialFunc(special);
    //  Tell GLUT to call "key" when a key is pressed
    glutKeyboardFunc(key);
    //  Pass control to GLUT so it can interact with the user
    glutMainLoop();
    return 0;
}
