/*
 *  Demonstrates an First Person Shooter Application.
 *
 *  Key bindings:
 *  Left Arrow     Strafe Left
 *  Right Arrow    Strafe Right
 *  Up Arrow       Move Forward
 *  Down Arrow     Move Backward
 *  l/L        Toggles lighting
 *  a/A        Decrease/increase ambient light
 *  d/D        Decrease/increase diffuse light
 *  s/S        Decrease/increase specular light
 *  e/E        Decrease/increase emitted light
 *  n/N        Decrease/increase shininess
 *  F1         Toggle smooth/flat shading
 *  x              Toggle Axes
 *  j          Jump with the gun
 *  </>         Move light
 *  [/]         Light elevation
 *  Space       Fire a bullet
 *  ESC            Exit
 */

#include "CSCIx229.h"
#include "terrain.cpp"

using namespace std;

// angle of rotation for the camera direction
float angle = 0.0f;

// XZ position of the camera
float x=0.0f, z=5.0f;
// actual vector representing the camera's direction
float lx=0.0f,lz=-1.0f;
int model, tree, house;
float ydir = 1.0f;
int sky[2];   //  Sky textures
float deltaMove = 0;
double dim=30.0;  //  Size of world
int xOrigin = -1;
float deltaAngle = 0.0f;
/* vector */
typedef float vec3_t[3];
int axes = 0;
int zh = 90;
int th=0;         //  Azimuth of view angle
int ph=0;         //  Elevation of view angle
int inc =  10;  // Plane increment
float ylight = 15.5f;  // Elevation of light
float jAngle = 0.0f;
float jLength = 1.0f;
float jAngleMod = 0.0f;
int jumping = 0;
float bx, by ,bz = 0.0f;
float speed = 10.0;
int fire = 0;
int smooth    =   1;  // Smooth/Flat shading
int emission  =   0;  // Emission intensity (%)
int ambient   =  50;  // Ambient intensity (%)
int diffuse   = 70;  // Diffuse intensity (%)
int specular  =   0;  // Specular intensity (%)
int shininess =   0;  // Shininess (power of two)
float shinyvec[1] = {1};    // Shininess (value)
int light=1;      //  Lighting
int wid, len;

/* enumerations for the mouse buttons */
enum {
    UP = 1,
    DOWN,
};
/* old position of the mouse */
int oldX = -13;
int oldY = -13;

/* global rotation, for use with the mouse */
vec3_t gRot = {0,0,0};

/* mouse state, UP or DOWN */
int mState = UP;

unsigned int texture[4];

/* Computes the position everytime for gluLookAt*/
void computePos(float deltaMove) {
    x += deltaMove * lx * 0.2f;
    z += deltaMove * lz * 0.2f;
}

/*
 *  Draw a cube
 *     at (x,y,z)
 *     dimentions (dx,dy,dz)
 *     rotated th about the y axis
 */
static void cube(double x,double y,double z,
                 double dx,double dy,double dz,
                 double th, int texture)
{
    //  Save transformation
    glPushMatrix();
    //  Set specular color to white
    float white[] = {1,1,1,1};
    float Emission[]  = {(float)0.0,(float)0.0,(float)0.01*emission,(float)1.0};
    glMaterialfv(GL_FRONT_AND_BACK,GL_SHININESS,shinyvec);
    glMaterialfv(GL_FRONT_AND_BACK,GL_SPECULAR,white);
    glMaterialfv(GL_FRONT_AND_BACK,GL_EMISSION,Emission);
    
    //  Offset, scale and rotate
    glTranslated(x,y,z);
    glRotated(th,0,1,0);
    glScaled(dx,dy,dz);
    //  Enable textures
    glEnable(GL_TEXTURE_2D);
    glTexEnvi(GL_TEXTURE_ENV,GL_TEXTURE_ENV_MODE,GL_MODULATE);
    glColor3f(1,1,1);
    glBindTexture(GL_TEXTURE_2D,texture);
    //  Front
    glBegin(GL_QUADS);
    glNormal3f( 0, 0, 1);
    glTexCoord2f(0,0); glVertex3f(-1,-1, 1);
    glTexCoord2f(1,0); glVertex3f(+1,-1, 1);
    glTexCoord2f(1,1); glVertex3f(+1,+1, 1);
    glTexCoord2f(0,1); glVertex3f(-1,+1, 1);
    glEnd();
    //  Back
    glBegin(GL_QUADS);
    glNormal3f( 0, 0,-1);
    glTexCoord2f(0,0); glVertex3f(+1,-1,-1);
    glTexCoord2f(1,0); glVertex3f(-1,-1,-1);
    glTexCoord2f(1,1); glVertex3f(-1,+1,-1);
    glTexCoord2f(0,1); glVertex3f(+1,+1,-1);
    glEnd();
    //  Right
    glBegin(GL_QUADS);
    glNormal3f(+1, 0, 0);
    glTexCoord2f(0,0); glVertex3f(+1,-1,+1);
    glTexCoord2f(1,0); glVertex3f(+1,-1,-1);
    glTexCoord2f(1,1); glVertex3f(+1,+1,-1);
    glTexCoord2f(0,1); glVertex3f(+1,+1,+1);
    glEnd();
    //  Left
    glBegin(GL_QUADS);
    glNormal3f(-1, 0, 0);
    glTexCoord2f(0,0); glVertex3f(-1,-1,-1);
    glTexCoord2f(1,0); glVertex3f(-1,-1,+1);
    glTexCoord2f(1,1); glVertex3f(-1,+1,+1);
    glTexCoord2f(0,1); glVertex3f(-1,+1,-1);
    glEnd();
    //  Top
    glBegin(GL_QUADS);
    glNormal3f( 0,+1, 0);
    glTexCoord2f(0,0); glVertex3f(-1,+1,+1);
    glTexCoord2f(1,0); glVertex3f(+1,+1,+1);
    glTexCoord2f(1,1); glVertex3f(+1,+1,-1);
    glTexCoord2f(0,1); glVertex3f(-1,+1,-1);
    glEnd();
    //  Bottom
    glBegin(GL_QUADS);
    glNormal3f( 0,-1, 0);
    glTexCoord2f(0,0); glVertex3f(-1,-1,-1);
    glTexCoord2f(1,0); glVertex3f(+1,-1,-1);
    glTexCoord2f(1,1); glVertex3f(+1,-1,+1);
    glTexCoord2f(0,1); glVertex3f(-1,-1,+1);
    glEnd();
    //  Undo transformations and textures
    glPopMatrix();
    glDisable(GL_TEXTURE_2D);
}


/*
 *  Draw sky box which has the sky as the texture
 */
static void Sky(double D)
{
    glPushMatrix();
    glColor3f(1,1,1);
    glEnable(GL_TEXTURE_2D);
    glTexEnvi(GL_TEXTURE_ENV,GL_TEXTURE_ENV_MODE,GL_MODULATE);
    //  Sides
    glBindTexture(GL_TEXTURE_2D,sky[0]);
    glBegin(GL_QUADS);
    glTexCoord2f(0.00,0.35); glVertex3f(-D,-D,-D);
    glTexCoord2f(0.25,0.35); glVertex3f(+D,-D,-D);
    glTexCoord2f(0.25,0.66); glVertex3f(+D,+D,-D);
    glTexCoord2f(0.00,0.66); glVertex3f(-D,+D,-D);
    
    glTexCoord2f(0.25,0.35); glVertex3f(+D,-D,-D);
    glTexCoord2f(0.50,0.35); glVertex3f(+D,-D,+D);
    glTexCoord2f(0.50,0.66); glVertex3f(+D,+D,+D);
    glTexCoord2f(0.25,0.66); glVertex3f(+D,+D,-D);
    
    glTexCoord2f(0.50,0.35); glVertex3f(+D,-D,+D);
    glTexCoord2f(0.75,0.35); glVertex3f(-D,-D,+D);
    glTexCoord2f(0.75,0.66); glVertex3f(-D,+D,+D);
    glTexCoord2f(0.50,0.66); glVertex3f(+D,+D,+D);
    
    glTexCoord2f(0.75,0.35); glVertex3f(-D,-D,+D);
    glTexCoord2f(1.00,0.35); glVertex3f(-D,-D,-D);
    glTexCoord2f(1.00,0.66); glVertex3f(-D,+D,-D);
    glTexCoord2f(0.75,0.66); glVertex3f(-D,+D,+D);
    glEnd();
    
    //  Top and bottom
    glBegin(GL_QUADS);
    glTexCoord2f(0.25,0.66); glVertex3f(+D,+D,-D);
    glTexCoord2f(0.5,0.66); glVertex3f(+D,+D,+D);
    glTexCoord2f(0.5,1.0); glVertex3f(-D,+D,+D);
    glTexCoord2f(0.25,1.0); glVertex3f(-D,+D,-D);
    glEnd();
    glDisable(GL_TEXTURE_2D);
    glPopMatrix();
}

/*
 *  Draw a tree
 *     at (x,y,z)
 *     dimentions (dx,dy,dz)
 *     rotated th about the x,y,z axes
 */
static void drawTree(double x,double y,double z,
                 double dx,double dy,double dz,
                 double th)
{
    //Draw a tree
    glPushMatrix();
    //  Set specular color to white
    float white[] = {1,1,1,1};
    float Emission[]  = {(float)0.0,(float)0.0,(float)0.01*emission,(float)1.0};
    glMaterialfv(GL_FRONT_AND_BACK,GL_SHININESS,shinyvec);
    glMaterialfv(GL_FRONT_AND_BACK,GL_SPECULAR,white);
    glMaterialfv(GL_FRONT_AND_BACK,GL_EMISSION,Emission);
    
    glTranslatef(x,y,z);
    glRotatef(th, 1, 1, 1);
    glScaled(dx,dy,dz);
    glCallList(tree);
    glPopMatrix();
    
}

/*
 *  Draw a house
 *     at (x,y,z)
 *     dimentions (dx,dy,dz)
 *     rotated th about the y axis
 */
static void drawHouse(double x,double y,double z,
                     double dx,double dy,double dz,
                     double th)
{
    //Draw a house
    glPushMatrix();
    //  Set specular color to white
    float white[] = {1,1,1,1};
    float Emission[]  = {(float)0.0,(float)0.0,(float)0.01*emission,(float)1.0};
    glMaterialfv(GL_FRONT_AND_BACK,GL_SHININESS,shinyvec);
    glMaterialfv(GL_FRONT_AND_BACK,GL_SPECULAR,white);
    glMaterialfv(GL_FRONT_AND_BACK,GL_EMISSION,Emission);
    
    glTranslatef(x,y,z);
    glRotatef(th, 0, 1, 0);
    glScaled(dx,dy,dz);
    glCallList(house);
    glPopMatrix();
    
}

/*
 *  Draw a gun
 *     at (x,y,z)
 *     dimentions (dx,dy,dz)
 *     rotated about the axes specified
 */
static void drawGun(double x,double y,double z,
                      double dx,double dy,double dz,
                      double th, double rx, double ry, double rz)
{
    //Draw gun
    glTranslatef(x,y,z);
    glRotatef(th, rx, ry, rz);
    glScaled(dx,dy,dz);
    glCallList(model);
}

/*
 * A crosshair which is always aligned with the gun whenever the gun moves
 */
static void drawCrosshair()
{
    glPushMatrix();
    glLineWidth(1);
    glColor3f(0, 1, 0);
    glBegin(GL_LINES);
    glVertex3f(0.01, -0.02, -2.0);
    glVertex3f(0.08, -0.02, -2.0);
    glEnd();
    glColor3f(0, 1, 0);
    glBegin(GL_LINES);
    glVertex3f(0.045, 0.01, -2.0);
    glVertex3f(0.045, -0.06, -2.0);
    glEnd();
    glPopMatrix();
}

/*
 * Shoots a weapon from the gun
 */
void shoot(){
    glColor3f(1.0f, 1.0f, 1.0f);
    bx +=-sin(0.1*PI/180)*speed;
    by +=sin(0.1*PI/180)*speed;
    bz +=cos(0.1*PI/180)*speed;
    
    if(bz>200.0){
        fire = 0;
        bx=0.0;
        by=1.0;
        bz=1.0;
    }
}

/*
 *  OpenGL (GLUT) calls this routine to display the scene
 */

void display(){
    
    //  Erase the window and the depth buffer
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    //  Enable Z-buffering in OpenGL
    glEnable(GL_DEPTH_TEST);
    glMatrixMode(GL_MODELVIEW);
    //  Undo previoustransformations
	glLoadIdentity();
    
    if (deltaMove){
        computePos(deltaMove);
    }
    
    gluLookAt(x, jLength, z, x+lx, jLength,  z+lz, 0.0f, 1.0f,  0.0f);
    
    //  Flat or smooth shading
    glShadeModel(smooth ? GL_SMOOTH : GL_FLAT);
    

    //Lighting
    //  Translate intensity to color vectors
    float Ambient[]   = {(float)0.01*ambient ,(float)0.01*ambient ,(float)0.01*ambient ,(float)1.0};
    float Diffuse[]   = {(float)0.01*diffuse ,(float)0.01*diffuse ,(float)0.01*diffuse ,(float)1.0};
    float Specular[]  = {(float)0.01*specular,(float)0.01*specular,(float)0.01*specular,(float)1.0};
    float Position[] = {(float)(15.0*Cos(zh)),(float)ylight, (float)(15.0*Sin(zh)),(float)1.0};
    
    if(light){
        //  OpenGL should normalize normal vectors
        glEnable(GL_NORMALIZE);
        //  Enable lighting
        glEnable(GL_LIGHTING);
        //  Two sided mode
        glLightModeli(GL_LIGHT_MODEL_TWO_SIDE,1);
        //  glColor sets ambient and diffuse color materials
        glColorMaterial(GL_FRONT_AND_BACK,GL_AMBIENT_AND_DIFFUSE);
        //  glColor Material Enabled
        glEnable(GL_COLOR_MATERIAL);
        //  Set specular colors
        glMaterialfv(GL_FRONT,GL_SHININESS,shinyvec);
        //  Enable light 0
        glEnable(GL_LIGHT0);
        //  Set ambient, diffuse, specular components and position of light 0
        glLightfv(GL_LIGHT0,GL_AMBIENT ,Ambient);
        glLightfv(GL_LIGHT0,GL_DIFFUSE ,Diffuse);
        glLightfv(GL_LIGHT0,GL_SPECULAR,Specular);
        glLightfv(GL_LIGHT0,GL_POSITION,Position);
    }else{
        glDisable(GL_LIGHTING);
    }
    
    glPushMatrix();
    glTranslatef(x+lx, jLength-0.4f, z+lz);
    glRotatef(-angle* 57.2957795, 0.0f, 1.0f, 0.0f);
    glColor3f(0.3f, 0.3f, 0.3f);
    drawCrosshair();
    glColor3f(0.3f, 0.3f, 0.3f);
    drawGun(0.0, 0.0, 0.0, 0.02, 0.02, 0.02, 180, 0,1,0);
    if(fire){
        glPushMatrix();
        glColor3f(0.0f, 0.0f, 0.0f);
        glTranslatef(-2.2, -0.8, 43.0);
        glTranslatef(bx, by, bz);
        glutSolidSphere(0.6,16,16);
        shoot();
        glPopMatrix();
    }
    glPopMatrix();
    
    /* apply the rotation from the mouse */
    glRotatef(gRot[0], 1.0, 0.0, 0.0);
    glRotatef(gRot[1], 0.0, 1.0, 0.0);
    
    //Cube similar to counter-strike
    cube(-5,-0.3,0 , 0.3,0.3,0.3 , 0, texture[1]);
    cube(-5,-0.1,-0.8 , 0.5,0.5,0.5 , 0, texture[1]);
    cube(5,-0.4,-0.8 , 0.5,0.5,0.5 , 0, texture[1]);
    cube(5,-0.6,0 , 0.3,0.3,0.3 , 0, texture[1]);
    
    
    drawTree(-5, -0.5, 0.7, 0.03, 0.03, 0.03, -130);
    drawTree(-10, -0.2, 0.3, 0.02, 0.02, 0.02, -130);
    //Other side of scene draw trees
    drawTree(5, -0.8, 0.7, 0.03, 0.03, 0.03, -130);
    drawTree(10, -0.6, 0.4, 0.02, 0.02, 0.02, -130);
    drawTree(12, -0.1, 7.0, 0.02, 0.02, 0.02, -130);
    
    //draw house
    drawHouse(-9, -0.7, -6.5, 0.12, 0.12, 0.12, 270);
    drawHouse(14, -0.23, 11.5, 0.16, 0.16, 0.16, 90);
    
    //Draw guns on top of box
    cube(-11, -0.3, -3, 0.3,0.3,0.5 , 0, texture[1]);
    glPushMatrix();
    glTranslatef(1, 1, 5);
    glColor3f(0.3f, 0.3f, 0.3f);
    drawGun(-12, -0.9, -7.5, 0.02, 0.02, 0.02, 180, 1, 1, 0);
    glPopMatrix();
    
    //Draw the terrain
    glPushMatrix();
	glTranslatef(0.0f, 0.0f, 0.0f);
    glRotated(180.0, 0.0, 1.0, 0.0);
	float scale = 60.0f / max(_terrain->width() - 1, _terrain->length() - 1);
	glScalef(scale, scale, scale);
	glTranslatef(-(float)(_terrain->width() - 1) / 2, 0.0f, -(float)(_terrain->length() - 1) / 2);
    //  Set specular color to white
    float white[] = {1,1,1,1};
    float Emission[]  = {(float)0.0,(float)0.0,(float)0.01*emission,(float)1.0};
    glMaterialfv(GL_FRONT_AND_BACK,GL_SHININESS,shinyvec);
    glMaterialfv(GL_FRONT_AND_BACK,GL_SPECULAR,white);
    glMaterialfv(GL_FRONT_AND_BACK,GL_EMISSION,Emission);
    
	glEnable(GL_TEXTURE_2D);
    glColor3f(1.0f, 1.0f, 1.0f);
	for(len = 0; len < _terrain->length() - 1; len++) {
		//Makes OpenGL draw a quad for the terrain to have a texture on it.
        glBindTexture(GL_TEXTURE_2D,texture[0]);
        glBegin(GL_TRIANGLE_STRIP);
		for(wid = 0; wid < _terrain->width(); wid++) {
			Vec3f normal = _terrain->getNormal(wid, len);
			glNormal3f(normal[0], normal[1], normal[2]);
            glTexCoord2f(wid/300.0,len/300.0);
            glVertex3f(wid, _terrain->getHeight(wid, len), len);
			normal = _terrain->getNormal(wid, len + 1);
			glNormal3f(normal[0], normal[1], normal[2]);
            glTexCoord2d(wid/300.0, (len + 1)/300.0);
            glVertex3f(wid, _terrain->getHeight(wid, len + 1), len + 1);
		}
		glEnd();
	}
    glPopMatrix();
    glDisable(GL_TEXTURE_2D);
    glDisable(GL_LIGHTING);
    
    //Draw sky
    Sky(dim);
    
    const double len=2.0;
    glLineWidth(2);
    glColor3f(1,1,1);
    if(axes)
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
    glWindowPos2i(5,5);
    Print("Position=%0.1f, %0.1f, %0.1f Ambient=%d Diffuse=%d Specular=%d Emission=%d Shininess=%.0f", x, jLength, z, ambient,diffuse,specular,emission,shinyvec[0]);
    //  Render the scene and make it visible
    ErrCheck("display");
    glFlush();
    glutSwapBuffers();
}

/*
 *  GLUT calls this routine when an arrow key is pressed
 */
void special(int key, int xx, int yy) {
    
    switch (key) {
        case GLUT_KEY_UP :
            deltaMove = 0.5f;
            break;
        case GLUT_KEY_DOWN :
            deltaMove = -0.5f;
            break;
        case GLUT_KEY_LEFT:
            angle -= 0.1f;
            lx = sin(angle);
            lz = -cos(angle);
            break;
        case GLUT_KEY_RIGHT:
            angle += 0.1f;
            lx = sin(angle);
            lz = -cos(angle);
            break;
        case GLUT_KEY_F1:
            smooth = 1-smooth;
            break;
        
    }
    glutPostRedisplay();
}

/*
 *  GLUT calls this routine when an arrow key is let go
 */

void specialUp(int key, int x, int y) {
    
    switch (key) {
        case GLUT_KEY_UP :
            deltaMove = 0;
            break;
        case GLUT_KEY_DOWN :
            deltaMove = 0;
            break;
    }
    glutPostRedisplay();
}
/*
 *  Make use mouse rotations are within 360 degrees
 *
 */

void rotate(vec3_t v)
{
    int i;
    for (i = 0; i < 3; i ++)
        if (v[i] > 360 || v[i] < -360)
            v[i] = 0.0f;
}

/*
 * GLUT cals this routine to get a motion while mouse is clicked
 *
 */
void motion(int x, int y) {
    
    if (mState == DOWN)
    {
        gRot[0] -= ((oldY - y) * 180.0f) / 360.0f;
        gRot[1] -= ((oldX - x) * 180.0f) / 360.0f;
        rotate(gRot);
        glutPostRedisplay();
    }
    oldX = x;
    oldY = y;
    glutPostRedisplay();
}

/*
 * GLUT cals this routine when the left button of the mouse is clicked
 *
 */

void mouse(int button, int state, int x, int y) {
    
    // only start motion if the left button is pressed
    if(state == GLUT_DOWN)
    {
        switch(button)
        {
            case GLUT_LEFT_BUTTON:
            case GLUT_RIGHT_BUTTON:
                mState = DOWN;
                oldX = x;
                oldY = y;
                break;
        }
    }
    else if (state == GLUT_UP)
        mState = UP;
    glutPostRedisplay();
}

/*
 *  GLUT calls this routine when a key is pressed
 */

void key(unsigned char ch, int x, int y) {
    
    //  Exit on ESC
    if(ch == 27)
        exit(0);
    //  Toggle axes
    else if (ch == 'x' || ch == 'X')
        axes = 1-axes;
    //  Toggle lighting
    else if (ch == 'l' || ch == 'L')
        light = 1-light;
    //  Move light
    else if (ch == '<')
        zh += 1;
    else if (ch == '>')
        zh -= 1;
    //  Light elevation
    else if (ch=='[')
        ylight -= 0.1;
    else if (ch==']')
        ylight += 0.1;
    //  Ambient level
    else if (ch=='a' && ambient>0)
        ambient -= 5;
    else if (ch=='A' && ambient<100)
        ambient += 5;
    //  Diffuse level
    else if (ch=='d' && diffuse>0)
        diffuse -= 5;
    else if (ch=='D' && diffuse<100)
        diffuse += 5;
    //  Specular level
    else if (ch=='s' && specular>0)
        specular -= 5;
    else if (ch=='S' && specular<100)
        specular += 5;
    //  Emission level
    else if (ch=='e' && emission>0)
        emission -= 5;
    else if (ch=='E' && emission<100)
        emission += 5;
    //  Shininess level
    else if (ch=='n' && shininess>-1)
        shininess -= 1;
    else if (ch=='N' && shininess<7)
        shininess += 1;
    else if (ch=='f')
        fire = 1;
    else if (ch==' ')
        jumping = 1 - jumping;
    //  Translate shininess power to value (-1 => 0)
    shinyvec[0] = shininess<0 ? 0 : pow(2.0,shininess);
    
    glutPostRedisplay();
}
/*
 *  GLUT calls this routine when the window is resized
 */
void reshape(int w, int h) {
    glViewport(0, 0, w, h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(45.0, (double)w / (double)h, 1.0, 200.0);
}

/*
 *  GLUT calls this routine when the time function has finished waiting
 *
 */
void idle(){
    _angle += 1.0f;
    if (_angle > 360) {
        _angle -= 360;
    }
    if(jumping){
        if(jAngle<=180){
            jAngleMod = sin(jAngle *(6.284/360.0));
            jLength = ydir + jAngleMod;
            jAngle+=10.0f;
        }else{
            jAngle = 0.0f;
            jumping = 0;
        }
    }
    //  Elapsed time in seconds
    double t = glutGet(GLUT_ELAPSED_TIME)/1000.0;
    zh = fmod(40*t,360.0);
    glutPostRedisplay();
}

/*
 *  Start up GLUT and tell it what to do
 */

int main(int argc, char** argv) {
    
    //  Initialize GLUT
	glutInit(&argc, argv);
    //  Request double buffered, true color window with Z buffering at 600x600
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
	glutInitWindowSize(1400, 1400);
	
	glutCreateWindow("Project: Krishna Chaitanya Sripada");
    //  Load textures and objects
	_terrain = loadTerrain("h.bmp", 30);
	texture[0] = LoadTexBMP("sand.bmp");
    texture[1] = LoadTexBMP("crate.bmp");
    texture[2] = LoadTexBMP("danger.bmp");
    sky[0] = LoadTexBMP("sky.bmp");
    tree = LoadOBJ("Palm_Tree.obj");
    model = LoadOBJ("m4a1.obj");
    house = LoadOBJ("house.obj");
    //  Set callbacks
    glutDisplayFunc(display);
	glutReshapeFunc(reshape);
    glutKeyboardFunc(key);
    glutIdleFunc(idle);
    glutSpecialFunc(special);
    glutSpecialUpFunc(specialUp);
    // here are the mouse functions
    glutMouseFunc(mouse);
    glutMotionFunc(motion);
    //  Pass control to GLUT so it can interact with the user
	ErrCheck("init");
	glutMainLoop();
	return 0;
}

