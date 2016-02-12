/*
 *  Lighting
 *
 *  Demonstrates lighting and textures on a 3D Scene.
 *
 *  Key bindings:
 *  l/L        Toggles lighting
 *  a/A        Decrease/increase ambient light
 *  d/D        Decrease/increase diffuse light
 *  s/S        Decrease/increase specular light
 *  e/E        Decrease/increase emitted light
 *  n/N        Decrease/increase shininess
 *  F1         Toggle smooth/flat shading
 *  F3         Toggle two-side lighting
 *  F8         Change plane increment
 *  m          Toggles light movement
 *  []         Lower/rise light
 *  p          Toggles ortogonal/perspective projection
 *  +/-        Change field of view of perspective
 *  x          Toggle axes
 *  arrows     Change view angle
 *  PgDn/PgUp  Zoom in and out
 *  0          Reset view angle
 *  ESC        Exit
 */
#include "CSCIx229.h"

int axes=1;       //  Display axes
int mode=1;       //  Projection mode
int side=0;       //  Two sided mode
int movement=1;       //  Move light
int th=0;         //  Azimuth of view angle
int ph=0;         //  Elevation of view angle
int fov=55;       //  Field of view (for perspective)
int light=1;      //  Lighting
double asp=1;     //  Aspect ratio
double dim=8.0;   //  Size of world
// Light values
int distance  =   8;  // Light distance
int inc       =  10;  // Plane increment
int smooth    =   1;  // Smooth/Flat shading
int emission  =   0;  // Emission intensity (%)
int ambient   =  0;  // Ambient intensity (%)
int diffuse   = 70;  // Diffuse intensity (%)
int specular  =   0;  // Specular intensity (%)
int shininess =   0;  // Shininess (power of two)
float shinyvec[1] = {1};    // Shininess (value)
int zh        =  90;  // Light azimuth
float ylight  =   8.5f;  // Elevation of light
unsigned int texture[9]; // Texture names
const double PI = 3.141592654;
int i;

/* Draw a ground on either side of the road
 *
 */

static void drawGround()
{
    double x=0,z=0;
    //  Save transformation
    glPushMatrix();
    //  Set specular color to white
    float white[] = {1,1,1,1};
    float Emission[]  = {0.0,0.0,0.01*emission,1.0};
    glMaterialfv(GL_FRONT_AND_BACK,GL_SHININESS,shinyvec);
    glMaterialfv(GL_FRONT_AND_BACK,GL_SPECULAR,white);
    glMaterialfv(GL_FRONT_AND_BACK,GL_EMISSION,Emission);
    
    //  Texture Enabling
    glEnable(GL_TEXTURE_2D);
    glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
    
    glBindTexture(GL_TEXTURE_2D, texture[8]);
    
    for(x=8.0; x>-8.0; x= x-0.5)
    {
        for(z=7; z>=0.5; z = z-0.5)
        {
            glBegin(GL_QUADS);
            glNormal3f(0, 1, 0);
            glTexCoord2f(0,0); glVertex3f(x,-0.01, z);
            glTexCoord2f(1,0); glVertex3f(x, -0.01, z-1);
            glTexCoord2f(1,1); glVertex3f(x-1, -0.01, z-1);
            glTexCoord2f(0,1); glVertex3f(x-1, -0.01, z);
            glEnd();
        }
        
    }
    for(x=8.0; x>-8.0; x= x-0.5)
    {
        for(z=-2.5; z>=-9.5; z = z-0.5)
        {
            glBegin(GL_QUADS);
            glNormal3f(0, 1, 0);
            glTexCoord2f(0,0); glVertex3f(x,-0.01, z);
            glTexCoord2f(1,0); glVertex3f(x, -0.01, z-1);
            glTexCoord2f(1,1); glVertex3f(x-1, -0.01, z-1);
            glTexCoord2f(0,1); glVertex3f(x-1, -0.01, z);
            glEnd();
        }
        
    }
    //  Undo transformations
    glPopMatrix();
    glDisable(GL_TEXTURE_2D);
}

/*
 * Draw a house which also contains a door and a window
 */

static void drawHouse(double x,double y,double z,
                      double dx,double dy,double dz,
                      double th)
{
    //  Set specular color to white
    float white[] = {1,1,1,1};
    float Emission[]  = {0.0,0.0,0.01*emission,1.0};
    glMaterialfv(GL_FRONT_AND_BACK,GL_SHININESS,shinyvec);
    glMaterialfv(GL_FRONT_AND_BACK,GL_SPECULAR,white);
    glMaterialfv(GL_FRONT_AND_BACK,GL_EMISSION,Emission);
    
    //  Save transformation
    glPushMatrix();
    //  Offset
    glTranslated(x,y,z);
    glRotated(th,0,1,0);
    glScaled(dx,dy,dz);
    
    //  Texture Enabling
    glEnable(GL_TEXTURE_2D);
    glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
    
    // House drawing begins
    // Drawing front left side of the house
    glBindTexture(GL_TEXTURE_2D, texture[1]);
    glBegin(GL_QUADS);
    glNormal3f(0, 0, 1);
    glTexCoord2f(0, 0); glVertex3f(-1.0f, 0.0f, 1.5f);
    glTexCoord2f(1, 0); glVertex3f(-0.25f, 0.0f, 1.5f);
    glTexCoord2f(1, 1); glVertex3f(-0.25f, 1.0f, 1.5f);
    glTexCoord2f(0, 1); glVertex3f(-1.0f, 1.0f, 1.5f);
    glEnd();
    // Drawing front right side of the house
    glBindTexture(GL_TEXTURE_2D, texture[1]);
    glBegin(GL_QUADS);
    glNormal3f(0, 0, 1);
    glTexCoord2f(0, 0); glVertex3f(0.25f, 0.0f, 1.5f);
    glTexCoord2f(1, 0); glVertex3f(1.0f, 0.0f, 1.5f);
    glTexCoord2f(1, 1); glVertex3f(1.0f, 1.0f, 1.5f);
    glTexCoord2f(0, 1); glVertex3f(0.25f, 1.0f, 1.5f);
    glEnd();
    // Drawing front top side of the house
    glBindTexture(GL_TEXTURE_2D, texture[1]);
    glBegin(GL_QUADS);
    glNormal3f(0, 0, 1);
    glTexCoord2f(0, 0); glVertex3f(-1.0f, 1.0f, 1.5f);
    glTexCoord2f(1, 0); glVertex3f(1.0f, 1.0f, 1.5f);
    glTexCoord2f(1, 1); glVertex3f(1.0f, 1.5f, 1.5f);
    glTexCoord2f(0, 1); glVertex3f(-1.0f, 1.5f, 1.5f);
    glEnd();
    
    // Drawing right side of the house - no windows
    glBindTexture(GL_TEXTURE_2D, texture[1]);
    glBegin(GL_QUADS);
    glNormal3f(1, 0, 0);
    glTexCoord2f(0, 0); glVertex3f(1.0f, 0.0f, 1.5f);
    glTexCoord2f(1, 0); glVertex3f(1.0f, 0.0f, -1.5f);
    glTexCoord2f(1, 1); glVertex3f(1.0f, 1.5f, -1.5f);
    glTexCoord2f(0, 1); glVertex3f(1.0f, 1.5f, 1.5f);
    glEnd();
    
    // Drawing left side of the house with windows - left of the window
    glBindTexture(GL_TEXTURE_2D, texture[1]);
    glBegin(GL_QUADS);
    glNormal3f(-1, 0, 0);
    glTexCoord2f(0, 0); glVertex3f(-1.0f, 0.0f, -1.5f);
    glTexCoord2f(1, 0); glVertex3f(-1.0f, 0.0f, -0.4f);
    glTexCoord2f(1, 1); glVertex3f(-1.0f, 1.5f, -0.4f);
    glTexCoord2f(0, 1); glVertex3f(-1.0f, 1.5f, -1.5f);
    glEnd();
    // Right of the window
    glBindTexture(GL_TEXTURE_2D, texture[1]);
    glBegin(GL_QUADS);
    glNormal3f(-1, 0, 0);
    glTexCoord2f(0, 0); glVertex3f(-1.0f, 0.0f, 0.4f);
    glTexCoord2f(1, 0); glVertex3f(-1.0f, 0.0f, 1.5f);
    glTexCoord2f(1, 1); glVertex3f(-1.0f, 1.5f, 1.5f);
    glTexCoord2f(0, 1); glVertex3f(-1.0f, 1.5f, 0.4f);
    glEnd();
    // Bottom of the window
    glBindTexture(GL_TEXTURE_2D, texture[1]);
    glBegin(GL_QUADS);
    glNormal3f(-1, 0, 0);
    glTexCoord2f(0, 0); glVertex3f(-1.0f, 0.0f, -0.4f);
    glTexCoord2f(1, 0); glVertex3f(-1.0f, 0.0f, 0.4f);
    glTexCoord2f(1, 1); glVertex3f(-1.0f, 0.5f, 0.4f);
    glTexCoord2f(0, 1); glVertex3f(-1.0f, 0.5f, -0.4f);
    glEnd();
    // Top of the window
    glBindTexture(GL_TEXTURE_2D, texture[1]);
    glBegin(GL_QUADS);
    glNormal3f(-1, 0, 0);
    glTexCoord2f(0, 0); glVertex3f(-1.0f, 1.0f, -0.4f);
    glTexCoord2f(1, 0); glVertex3f(-1.0f, 1.0f, 0.4f);
    glTexCoord2f(1, 1); glVertex3f(-1.0f, 1.5f, 0.4f);
    glTexCoord2f(0, 1); glVertex3f(-1.0f, 1.5f, -0.4f);
    glEnd();
    // Back of the house
    glBindTexture(GL_TEXTURE_2D, texture[1]);
    glBegin(GL_QUADS);
    if(side) glNormal3f(0, 0, 1);
    else glNormal3f(0, 0, -1);
    glTexCoord2f(0, 0); glVertex3f(-1.0f, 0.0f, -1.5f);
    glTexCoord2f(1, 0); glVertex3f(1.0f, 0.0f, -1.5f);
    glTexCoord2f(1, 1); glVertex3f(1.0f, 1.5f, -1.5f);
    glTexCoord2f(0, 1); glVertex3f(-1.0f, 1.5f, -1.5f);
    glEnd();
    
    // Left side of the roof
    glBindTexture(GL_TEXTURE_2D, texture[0]);
    glBegin(GL_QUADS);
    glNormal3f(-1, 1, 0);
    glTexCoord2f(0, 0); glVertex3f(-1.0f, 1.5f, -1.5f);
    glTexCoord2f(1, 0); glVertex3f(-1.0f, 1.5f, 1.5f);
    glTexCoord2f(1, 1); glVertex3f(0.0f, 2.2f, 1.5f);
    glTexCoord2f(0, 1); glVertex3f(0.0f, 2.2f, -1.5f);
    glEnd();
    
    // Right side of the roof
    glBindTexture(GL_TEXTURE_2D, texture[0]);
    glBegin(GL_QUADS);
    glNormal3f(1, 1, 0);
    glTexCoord2f(0, 0); glVertex3f(1.0f, 1.5f, 1.5f);
    glTexCoord2f(1, 0); glVertex3f(1.0f, 1.5f, -1.5f);
    glTexCoord2f(1, 1); glVertex3f(0.0f, 2.2f, -1.5f);
    glTexCoord2f(0, 1); glVertex3f(0.0f, 2.2f, 1.5f);
    glEnd();
    
    // Front side roof triangle
    glBindTexture(GL_TEXTURE_2D, texture[0]);
    glBegin(GL_TRIANGLES);
    glNormal3f(0, 0, 1);
    glTexCoord2f(0, 0); glVertex3f(-1.0f, 1.5f, 1.5f);
    glTexCoord2f(1, 0); glVertex3f(1.0f, 1.5f, 1.5f);
    glTexCoord2f(1, 1); glVertex3f(0.0f, 2.2f, 1.5f);
    glEnd();
    
    // Back side roof triangle
    glBindTexture(GL_TEXTURE_2D, texture[0]);
    glBegin(GL_TRIANGLES);
    if(side) glNormal3f(0, 0, 1);
    else glNormal3f(0, 0, -1);
    glTexCoord2f(0, 0); glVertex3f(-1.0f, 1.5f, -1.5f);
    glTexCoord2f(1, 0); glVertex3f(1.0f, 1.5f, -1.5f);
    glTexCoord2f(1, 1); glVertex3f(0.0f, 2.2f, -1.5f);
    glEnd();
    
    
    //  Undo transformations
    glPopMatrix();
    glDisable(GL_TEXTURE_2D);
}

/*
 * Draw a road for the airplane to land
 */

static void drawRoad()
{
    int i;
    
    //  Set specular color to white
    float white[] = {1,1,1,1};
    float Emission[]  = {0.0,0.0,0.01*emission,1.0};
    glMaterialfv(GL_FRONT_AND_BACK,GL_SHININESS,shinyvec);
    glMaterialfv(GL_FRONT_AND_BACK,GL_SPECULAR,white);
    glMaterialfv(GL_FRONT_AND_BACK,GL_EMISSION,Emission);
    
    //  Texture Enabling
    glEnable(GL_TEXTURE_2D);
    glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
    
    // Draw the road.
    for(i=-8.0;i<8.0;i++){
        glBindTexture(GL_TEXTURE_2D, texture[3]);
        glBegin(GL_QUADS);
        glNormal3f(0, 1, 0);
        glTexCoord2f(0, 0);
        glVertex3f(i, 0.0f, -1.0);
        glTexCoord2f(1, 0);
        glVertex3f(i, 0.0f, -2.0);
        glTexCoord2f(1, 1);
        glVertex3f(i+1, 0.0f, -2.0);
        glTexCoord2f(0, 1);
        glVertex3f(i+1, 0.0f, -1.0);
        glEnd();
    }
    glDisable(GL_TEXTURE_2D);
    
}

/* Normal for the light source
 *
 */
static void Vertex(double th,double ph)
{
    double x = Sin(th)*Cos(ph);
    double y = Cos(th)*Cos(ph);
    double z =         Sin(ph);
    //  For a plane at the origin, the position
    //  and normal vectors are the same
    glNormal3d(x,y,z);
    glVertex3d(x,y,z);
}


/*
 *  Draw solid airplane
 *    at (x,y,z)
 *    nose towards (dx,dy,dz)
 *    up towards (ux,uy,uz)
 */
static void SolidPlane(double x,double y,double z,
                       double dx,double dy,double dz,
                       double ux,double uy, double uz, int isLit)
{
    
    //  Set specular color to white
    float white[] = {1,1,1,1};
    float Emission[]  = {0.0,0.0,0.01*emission,1.0};
    glMaterialfv(GL_FRONT_AND_BACK,GL_SHININESS,shinyvec);
    glMaterialfv(GL_FRONT_AND_BACK,GL_SPECULAR,white);
    glMaterialfv(GL_FRONT_AND_BACK,GL_EMISSION,Emission);
    
    //  Texture Enabling
    glEnable(GL_TEXTURE_2D);
    glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
    

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
    glScaled(2.0f, 2.0f, 2.0f);
    glMultMatrixd(mat);
    
    //  Nose (4 sided)
    glBindTexture(GL_TEXTURE_2D, texture[6]);
    glBegin(GL_TRIANGLES);
    glNormal3f(0, 1, 0);
    glTexCoord2f(0, 0);
    glVertex3d(nose, 0.0, 0.0);
    glTexCoord2f(0, 1);
    glVertex3d(cone, wid, wid);
    glTexCoord2f(1, 0);
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
    glBindTexture(GL_TEXTURE_2D, texture[7]);
    glBegin(GL_QUADS);
    glTexCoord2f(0, 0);
    glVertex3d(cone, wid, wid);
    glTexCoord2f(0, 1);
    glVertex3d(cone,-wid, wid);
    glTexCoord2f(1, 1);
    glVertex3d(tail,-wid, wid);
    glTexCoord2f(1, 0);
    glVertex3d(tail, wid, wid);
    
    glTexCoord2f(0, 0);
    glVertex3d(cone, wid,-wid);
    glTexCoord2f(0, 1);
    glVertex3d(cone,-wid,-wid);
    glTexCoord2f(1, 1);
    glVertex3d(tail,-wid,-wid);
    glTexCoord2f(1, 0);
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
    glBindTexture(GL_TEXTURE_2D, texture[6]);
    glBegin(GL_TRIANGLES);
    glNormal3f(0, 1, 0);
    glTexCoord2f(0, 0);
    glVertex3d(wing, -0.03, wid);
    glTexCoord2f(0, 1);
    glVertex3d(tail, -0.03, wid);
    glTexCoord2f(1, 0);
    glVertex3d(tail, -0.03, 0.5);
    glEnd();
    
    glBindTexture(GL_TEXTURE_2D, texture[6]);
    glBegin(GL_TRIANGLES);
    glTexCoord2f(0, 0);
    glVertex3d(wing, -0.03,-wid);
    glTexCoord2f(0, 1);
    glVertex3d(tail, -0.03,-wid);
    glTexCoord2f(1, 0);
    glVertex3d(tail, -0.03,-0.5);
    glEnd();
    //  Vertical tail (plane triangle)
    glBindTexture(GL_TEXTURE_2D, texture[6]);
    glBegin(GL_POLYGON);
    glNormal3f(0, 0, 1);
    glTexCoord2f(0, 0);
    glVertex3d(strk, 0.0, 0.0);
    glTexCoord2f(0, 1);
    glVertex3d(tail, 0.3, 0.0);
    glTexCoord2f(1, 0);
    glVertex3d(tail, 0.0, 0.0);
    glEnd();
    
    if(isLit){
        //  Bands of latitude
        for (ph=-90;ph<90;ph+=inc)
        {
            for (th=0;th<=360;th+=2*inc)
            {
                Vertex(th,ph);
                Vertex(th,ph+inc);
            }
        }
    }
    //  Undo transformations
    glPopMatrix();
    glDisable(GL_TEXTURE_2D);
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
    
    //  Set specular color to white
    float white[] = {1,1,1,1};
    float Emission[]  = {0.0,0.0,0.01*emission,1.0};
    glMaterialfv(GL_FRONT_AND_BACK,GL_SHININESS,shinyvec);
    glMaterialfv(GL_FRONT_AND_BACK,GL_SPECULAR,white);
    glMaterialfv(GL_FRONT_AND_BACK,GL_EMISSION,Emission);
    
    //  Texture Enabling
    glEnable(GL_TEXTURE_2D);
    glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
    
    for(i = 0; i <=360; i++)
    {
        const float tc = (i/(float)(2*PI));
        // Save transformation
        glPushMatrix();
        // Offset
        glTranslated(x + dx*cos(i), y, z + dz*sin(i));
        glLineWidth(2);
        glBindTexture(GL_TEXTURE_2D, texture[4]);
        glBegin(GL_LINES);
        glNormal3f(2*dx*cos(i),1,2*dz*sin(i));
        glTexCoord2f(tc, 0.0);
        glVertex3f(0,0,0);
        glTexCoord2f(tc, 1.0);
        glVertex3f(0.0,height,0.0);
        glEnd();
        
        // Undo transformations
        glPopMatrix();
    }
    
    for(i = 0; i <= 360; i++)
    {
        const float tc = (i/(float)(2*PI));
        // Save transformation
        glPushMatrix();
        // Offset
        glTranslated(x,y,z);
        glLineWidth(2);
        glBindTexture(GL_TEXTURE_2D, texture[5]);
        glBegin(GL_LINES);
        glNormal3f(2*dx*cos(i),1,2*dz*sin(i));
        glTexCoord2f(tc, 0.0);
        glVertex3f(0.0,height*2,0.0);
        glTexCoord2f(tc, 1.0);
        glVertex3f(2*dx*cos(i),height,2*dz*sin(i));
        glEnd();
        
        // Undo transformations
        glPopMatrix();
    }
    
    // Undo transformations
    glPopMatrix();
    glDisable(GL_TEXTURE_2D);
}

/*
 *  OpenGL (GLUT) calls this routine to display the scene
 */
void display()
{
    const double len=4.0;  //  Length of axes
    //glClearColor(0.95f, 0.95f, 0.95f, 1.0f);
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
    
    //  Flat or smooth shading
    glShadeModel(smooth ? GL_SMOOTH : GL_FLAT);
    
    glColor3f(1.0f, 1.0f, 1.0f);
    //  Light switch
    if (light)
    {
        //  Translate intensity to color vectors
        float Ambient[]   = {0.01*ambient ,0.01*ambient ,0.01*ambient ,1.0};
        float Diffuse[]   = {0.01*diffuse ,0.01*diffuse ,0.01*diffuse ,1.0};
        float Specular[]  = {0.01*specular,0.01*specular,0.01*specular,1.0};
        //  Light position
        float Position[]  = {8.0*Cos(zh),ylight, 8.0*Sin(zh),1.0};
        //  Solid Airplane - Light source
        SolidPlane(Position[0],Position[1],Position[2],-Sin(zh),0,Cos(zh) , 0,10,0, 0);
        //  OpenGL should normalize normal vectors
        glEnable(GL_NORMALIZE);
        //  Enable lighting
        glEnable(GL_LIGHTING);
        //  Two sided mode
        glLightModeli(GL_LIGHT_MODEL_TWO_SIDE,side);
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
        
    }
    else
        glDisable(GL_LIGHTING);
    
    //Draw the scene
    drawGround();
    
    //Draw House
    drawHouse(-3.0,0.0,2.0,  1.0, 1.0, 1.0,    180);
    drawHouse(0,0.0,2.0,      0.75, 0.75, 0.75,    180);
    drawHouse(2.0,0.0,2.0,   0.5, 0.5, 0.5,    180);
    
    //Draw the road
    drawRoad();
    
    //Draw Houses on other side of the road
    drawHouse(-3.0,0.0,-5.0,  1.0, 1.0, 1.0,    0);
    drawHouse(0,0.0,-5.0,    0.75, 0.75, 0.75,    0);
    drawHouse(2.0,0.0,-5.0,   0.5, 0.5, 0.5,    0);
    
    //Draw a tree
    drawTree(3.5, 0.0, 2.0, 0.5, 1.0, 0.5, 0, 0.5);
    drawTree(-4.0, 0.0, 2.0, 0.5, 1.0, 0.5, 0, 0.5);
    drawTree(3.5, 0.0, -4.0, 0.5, 1.0, 0.5, 0, 0.5);
    drawTree(-4.0, 0.0, -4.0, 0.5, 1.0, 0.5, 0, 0.5);
    
    //  Draw axes - no lighting from here on
    glDisable(GL_LIGHTING);
    glLineWidth(2);
    glColor3f(1.0f,1.0f,1.0f);
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
    Print("Angle=%d,%d  Dim=%.1f FOV=%d Projection=%s Light=%s",
          th,ph,dim,fov,mode?"Perpective":"Orthogonal",light?"On":"Off");
    if (light)
    {
        glWindowPos2i(5,45);
        Print("Model=%s Two-sided=%s Distance=%d Elevation=%.1f",smooth?"Smooth":"Flat",side?"Off":"On",distance,ylight);
        glWindowPos2i(5,25);
        Print("Ambient=%d  Diffuse=%d Specular=%d Emission=%d Shininess=%.0f",ambient,diffuse,specular,emission,shinyvec[0]);
    }
    
    //  Render the scene and make it visible
    ErrCheck("display");
    glFlush();
    glutSwapBuffers();
}

/*
 *  GLUT calls this routine when the window is resized
 */
void idle()
{
    //  Elapsed time in seconds
    double t = glutGet(GLUT_ELAPSED_TIME)/1000.0;
    zh = fmod(40*t,360.0);
    //  Tell GLUT it is necessary to redisplay the scene
    glutPostRedisplay();
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
    else if (key == GLUT_KEY_PAGE_DOWN)
        dim += 0.1;
    //  PageDown key - decrease dim
    else if (key == GLUT_KEY_PAGE_UP && dim>1)
        dim -= 0.1;
    //  Smooth color model
    else if (key == GLUT_KEY_F1)
        smooth = 1-smooth;
    //  Two sided mode
    else if (key == GLUT_KEY_F3)
        side = 1-side;
    //  Toggle plane increment
    else if (key == GLUT_KEY_F8)
        inc = (inc==10)?3:10;
    //  Update projection
    Project(mode?fov:0,asp,dim);
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
    else if (ch == 'x' || ch == 'X')
        axes = 1-axes;
    //  Toggle lighting
    else if (ch == 'l' || ch == 'L')
        light = 1-light;
    //  Switch projection mode
    else if (ch == 'p' || ch == 'P')
        mode = 1-mode;
    //  Toggle light movement
    else if (ch == 'm' || ch == 'M')
        movement = 1-movement;
    //  Move light
    else if (ch == '<')
        zh += 1;
    else if (ch == '>')
        zh -= 1;
    //  Change field of view angle
    else if (ch == '-' && ch>1)
        fov--;
    else if (ch == '+' && ch<179)
        fov++;
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
    //  Translate shininess power to value (-1 => 0)
    shinyvec[0] = shininess<0 ? 0 : pow(2.0,shininess);
    //  Reproject
    Project(mode?fov:0,asp,dim);
    //  Animate if requested
    glutIdleFunc(movement?idle:NULL);
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
    Project(mode?fov:0,asp,dim);
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
    glutCreateWindow("Assignment 3: Krishna Chaitanya Sripada");
    //  Set callbacks
    glutDisplayFunc(display);
    glutReshapeFunc(reshape);
    glutSpecialFunc(special);
    glutKeyboardFunc(key);
    glutIdleFunc(idle);
    //  Load textures
    texture[0] = LoadTexBMP("brick.bmp");
    texture[1] = LoadTexBMP("brick1.bmp");
    texture[2] = LoadTexBMP("ground.bmp");
    texture[3] = LoadTexBMP("road.bmp");
    texture[4] = LoadTexBMP("cortex.bmp");
    texture[5] = LoadTexBMP("leaves.bmp");
    texture[6] = LoadTexBMP("wings.bmp");
    texture[7] = LoadTexBMP("window.bmp");
    texture[8] = LoadTexBMP("grass.bmp");
    //  Pass control to GLUT so it can interact with the user
    ErrCheck("init");
    glutMainLoop();
    return 0;
}
