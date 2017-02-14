/********************************* 
 Nishesh Shukla
 12/06/16
 **********************************/  
/*
  l          Toggles lighting
  t          Change Textures
  k          Toggle between projections (Default is set to FPV)
  m/M        Toggle Textures
  1/!        Decrease/increase ambient light
  2/@        Decrease/increase diffuse light
  3/#        Decrease/increase specular light
  4/$        Decrease/increase emitted light
  5/%        Decrease/increase shininess
  u          Stop/Start the movement of light and cart
  []         Lower/rise light
  x          Toggle axes
  arrows     Change view angle
  g/h         Zoom out and in
  0          Default view from point A (The starting point for FPV, initial poiunt)
  9          Better view of the moving cart
  8          View point of the Bell tower, FPS. 
  7          Get a good view of the playground and the cart in FPS. 
  ESC        Exit
 */
#include "CSCIx229.h"

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


int ntex=1;       //  Cube Faces
int axes=1;       //  Display axes
int mode=0;       //  Texture Mode
int pmode=1;	  //  Projection Mode
int move=1;       //  Move light
int th=-90;       //  Azimuth of view angle
int ph=0;         //  Elevation of view angle
int fov=55;       //  Field of view (for perspective)
int light=1;      //  Lighting
int fog = 1;      //  Fog
int rep=1;        //  Repitition
double asp=1;     //  Aspect ratio
double dim=3.6;   //  Size of world
int obj;          //  Object display list

const char *pmodes[] = { "Perspective", "First Person"};

// Light values
int one       =   1;  // Unit value
int distance  =   5;  // Light distance
int inc       =  10;  // Ball increment
int smooth    =   1;  // Smooth/Flat shading
int local     =   0;  // Local Viewer Model
int emission  =   0;  // Emission intensity (%)
int ambient   =  10;  // Ambient intensity (%)
int diffuse   = 60;  // Diffuse intensity (%)
int specular  =   0;  // Specular intensity (%)
int shininess =   0;  // Shininess (power of two)
float shinyvec[1];    // Shininess (value)
int zh        =  90;  // Light azimuth
float ylight  =   0;  // Elevation of light
unsigned int texture[40]; //Texture names


// Default Ex, Ey, Ez values for first person
double xOffset = -1.2;
double yOffset = 0.1; 
double zOffset = 0;

//Defualt Ex, Ey, Ez values for first person
double Cx = 0;
double Cy = 0;
double Cz = 0;


/*
 *  Draw a cube
 *     at (x,y,z)
 *     dimentions (dx,dy,dz)
 *     rotated th about the y axis
 */
static void cube(double x,double y,double z,
                 double dx,double dy,double dz,
                 double th, int n)
{
   //  Set specular color to white
   float white[] = {1,1,1,1};
   float Emission[]  = {0.0,0.0,0.01*emission,1.0};
   glMaterialfv(GL_FRONT_AND_BACK,GL_SHININESS,shinyvec);
   glMaterialfv(GL_FRONT_AND_BACK,GL_SPECULAR,white);
   glMaterialfv(GL_FRONT_AND_BACK,GL_EMISSION,Emission);
   //  Save transformation
   glPushMatrix();
   //  Offset, scale and rotate
   glTranslated(x,y,z);
   glRotated(th,0,1,0);
   glScaled(dx,dy,dz);
   //  Enable textures
   glEnable(GL_TEXTURE_2D);
   glTexEnvi(GL_TEXTURE_ENV,GL_TEXTURE_ENV_MODE,mode?GL_REPLACE:GL_MODULATE);
   glColor3f(1,1,1);
   glBindTexture(GL_TEXTURE_2D,texture[0]);
   
   //  Cube
   //  Front
   glColor3f(1,1,1);
   if (ntex) glBindTexture(GL_TEXTURE_2D,texture[n]);
   glBegin(GL_QUADS);
   glNormal3f( 0, 0, 1);
   glTexCoord2f(0,0); glVertex3f(-1,-1, 1);
   glTexCoord2f(1,0); glVertex3f(+1,-1, 1);
   glTexCoord2f(1,1); glVertex3f(+1,+1, 1);
   glTexCoord2f(0,1); glVertex3f(-1,+1, 1);
   //  Back
   glColor3f(1,1,1);
   if (ntex) glBindTexture(GL_TEXTURE_2D,texture[n]);
   glBegin(GL_QUADS);
   glNormal3f( 0, 0,-1);
   glTexCoord2f(0,0); glVertex3f(+1,-1,-1);
   glTexCoord2f(1,0); glVertex3f(-1,-1,-1);
   glTexCoord2f(1,1); glVertex3f(-1,+1,-1);
   glTexCoord2f(0,1); glVertex3f(+1,+1,-1);
   glEnd();
   //  Right
   glColor3f(1,1,1);
   if (ntex) glBindTexture(GL_TEXTURE_2D,texture[n]);
   glBegin(GL_QUADS);
   glNormal3f(+1, 0, 0);
   glTexCoord2f(0,0); glVertex3f(+1,-1,+1);
   glTexCoord2f(1,0); glVertex3f(+1,-1,-1);
   glTexCoord2f(1,1); glVertex3f(+1,+1,-1);
   glTexCoord2f(0,1); glVertex3f(+1,+1,+1);
   glEnd();
   //  Left
   glColor3f(1,1,1);
   if (ntex) glBindTexture(GL_TEXTURE_2D,texture[n]);
   glBegin(GL_QUADS);
   glNormal3f(-1, 0, 0);
   glTexCoord2f(0,0); glVertex3f(-1,-1,-1);
   glTexCoord2f(1,0); glVertex3f(-1,-1,+1);
   glTexCoord2f(1,1); glVertex3f(-1,+1,+1);
   glTexCoord2f(0,1); glVertex3f(-1,+1,-1);
   glEnd();
   //  Top
   glColor3f(1,1,1);
   if (ntex) glBindTexture(GL_TEXTURE_2D,texture[0]);
   glBegin(GL_QUADS);
   glNormal3f( 0,+1, 0);
   glTexCoord2f(0,0); glVertex3f(-1,+1,+1);
   glTexCoord2f(1,0); glVertex3f(+1,+1,+1);
   glTexCoord2f(1,1); glVertex3f(+1,+1,-1);
   glTexCoord2f(0,1); glVertex3f(-1,+1,-1);
   glEnd();
   //  Bottom
   glColor3f(1,1,1);
   if (ntex) glBindTexture(GL_TEXTURE_2D,texture[0]);
   glBegin(GL_QUADS);
   glNormal3f( 0,-1, 0);
   glTexCoord2f(0,0); glVertex3f(-1,-1,-1);
   glTexCoord2f(1,0); glVertex3f(+1,-1,-1);
   glTexCoord2f(1,1); glVertex3f(+1,-1,+1);
   glTexCoord2f(0,1); glVertex3f(-1,-1,+1);
   glEnd();
   //  End
   //  Undo transofrmations
   glPopMatrix();
   glDisable(GL_TEXTURE_2D);

}

  
static void house_1(double x, double y, double z, double dx, double dy, double dz, double th, int n, int r) {
   
   // Starting with the base of the house, using the built in functions
   cube(x,y,z,dx,dy,dz,th,n);

   // Roof and chimney, triangle and rectangle.
   
   float white[]={1,1,1,1};
   float Emission[] = {0.0,0.0,0.01*emission,1.0};
   glMaterialfv(GL_FRONT_AND_BACK, GL_SHININESS,shinyvec);
   glMaterialfv(GL_FRONT_AND_BACK,GL_SPECULAR,white);
   glMaterialfv(GL_FRONT_AND_BACK,GL_EMISSION,Emission);
   // Save transformation
   glPushMatrix();
   // Offset
   glTranslated(x,y,z);
   glRotated(th,0,1,0);
   glScaled(dx,dy,dz);

   //Add roof to the cube

   //Enable Textures
   glEnable(GL_TEXTURE_2D);
   glTexEnvi(GL_TEXTURE_ENV,GL_TEXTURE_ENV_MODE,mode?GL_REPLACE:GL_MODULATE);
   glColor3f(1,1,1);
   glBindTexture(GL_TEXTURE_2D,texture[11]);   

   // 1
   glColor3f(1,1,1);
   if (ntex) glBindTexture(GL_TEXTURE_2D,texture[r]);  
   glBegin(GL_QUADS);
   glNormal3f(0,1,1);
   glTexCoord2f(0,0); glVertex3f(-1,+1,+1);
   glTexCoord2f(1,0); glVertex3f(+1,+1,+1);
   glTexCoord2f(1,1); glVertex3f(+1,+2,0);
   glTexCoord2f(0,1); glVertex3f(-1,+2,0);
   glEnd();

   //2
   glColor3f(1,1,1);
   if (ntex) glBindTexture(GL_TEXTURE_2D,texture[r]);
   glBegin(GL_QUADS);
   glNormal3f(0,1,-1); 
   glTexCoord2f(0,0); glVertex3f(+1,+1,-1);
   glTexCoord2f(1,0); glVertex3f(-1,+1,-1);
   glTexCoord2f(1,1); glVertex3f(-1,+2,0);
   glTexCoord2f(0,1); glVertex3f(+1,+2,0);
   glEnd();

   //Roof ends, triangular bits
   glColor3f(1,1,1);
   if (ntex) glBindTexture(GL_TEXTURE_2D,texture[r]);   
   glBegin(GL_TRIANGLES);
   glNormal3f(1,0,0);
   glTexCoord2f(0,0); glVertex3f(1,2,0);
   glTexCoord2f(1,0); glVertex3f(1,1,-1);
   glTexCoord2f(0,1); glVertex3f(1,1,1);
   glEnd();

   glColor3f(1,1,1);
   if (ntex) glBindTexture(GL_TEXTURE_2D,texture[r]);   
   glBegin(GL_TRIANGLES); 
   glNormal3f(-1,0,0);
   glTexCoord2f(0,0); glVertex3f(-1,2,0);
   glTexCoord2f(1,0); glVertex3f(-1,1,-1);
   glTexCoord2f(0,1); glVertex3f(-1,1,1);
   glEnd();

   //Chimney

   // Starting with the top
   glColor3f(1,1,1);
   if (ntex) glBindTexture(GL_TEXTURE_2D,texture[5]);
   glBegin(GL_QUADS);
   glNormal3f(0,1,0);
   glTexCoord2f(0,0); glVertex3f(.5,3,.5);
   glTexCoord2f(1,0); glVertex3f(0,3,.5);
   glTexCoord2f(1,1); glVertex3f(0,3,0);
   glTexCoord2f(0,1); glVertex3f(.5,3,0);
   glEnd();

   //Side 1 (Front)
   glColor3f(1,1,1);
   if (ntex) glBindTexture(GL_TEXTURE_2D,texture[5]);
   glBegin(GL_QUADS); 
   glNormal3f(0,0,1);
   glTexCoord2f(0,0); glVertex3f(.5,3,.5);
   glTexCoord2f(1,0); glVertex3f(.5,1,.5);
   glTexCoord2f(1,1); glVertex3f(0,1,.5);
   glTexCoord2f(0,1); glVertex3f(0,3,.5);
   glEnd();

   //Side 2 (Right)
   glColor3f(1,1,1);
   if (ntex) glBindTexture(GL_TEXTURE_2D,texture[5]);
   glBegin(GL_QUADS);  
   glNormal3f(1,0,0);
   glTexCoord2f(0,0); glVertex3f(.5,3,.5);
   glTexCoord2f(1,0); glVertex3f(.5,1,.5);
   glTexCoord2f(1,1); glVertex3f(.5,1,0);
   glTexCoord2f(0,1); glVertex3f(.5,3,0);
   glEnd();

   //Side 3 (Left)
   glColor3f(1,1,1);
   if (ntex) glBindTexture(GL_TEXTURE_2D,texture[5]);
   glBegin(GL_QUADS); 
   glNormal3f(-1,0,0);
   glTexCoord2f(0,0); glVertex3f(0,3,0);
   glTexCoord2f(1,0); glVertex3f(0,1,0);
   glTexCoord2f(1,1); glVertex3f(0,1,.5);
   glTexCoord2f(0,1); glVertex3f(0,3,.5);
   glEnd();

   //Side 4 (Back)

   glColor3f(1,1,1);
   if (ntex) glBindTexture(GL_TEXTURE_2D,texture[5]);
   glBegin(GL_QUADS); 
   glNormal3f(0,0,-1);
   glTexCoord2f(0,0); glVertex3f(0,3,0);
   glTexCoord2f(0,1); glVertex3f(0,1,0);
   glTexCoord2f(1,1); glVertex3f(.5,1,0);
   glTexCoord2f(0,1); glVertex3f(.5,3,0);
   glEnd();

   //Undo Transformations
   glPopMatrix();
   glDisable(GL_TEXTURE_2D);  

 }

static void house_2(double x,double y,double z,
                 double dx,double dy,double dz,
                 double th, int n)
{
   //  Set specular color to white
   float white[] = {1,1,1,1};
   float Emission[]  = {0.0,0.0,0.01*emission,1.0};
   glMaterialfv(GL_FRONT_AND_BACK,GL_SHININESS,shinyvec);
   glMaterialfv(GL_FRONT_AND_BACK,GL_SPECULAR,white);
   glMaterialfv(GL_FRONT_AND_BACK,GL_EMISSION,Emission);
   //  Save transformation
   glPushMatrix();
   //  Offset, scale and rotate
   glTranslated(x,y,z);
   glRotated(th,0,1,0);
   glScaled(dx,dy,dz);
   //  Enable textures
   glEnable(GL_TEXTURE_2D);
   glTexEnvi(GL_TEXTURE_ENV,GL_TEXTURE_ENV_MODE,mode?GL_REPLACE:GL_MODULATE);
   glColor3f(1,1,1);
   glBindTexture(GL_TEXTURE_2D,texture[2]);
   
   //  Cube
   //  Front
   glColor3f(1,1,1);
   if (ntex) glBindTexture(GL_TEXTURE_2D,texture[n]);
   glBegin(GL_QUADS);
   glNormal3f( 0, 0, 1);
   glTexCoord2f(0,0); glVertex3f(-1,-1, 1);
   glTexCoord2f(1,0); glVertex3f(+1,-1, 1);
   glTexCoord2f(1,1); glVertex3f(+1,+1, 1);
   glTexCoord2f(0,1); glVertex3f(-1,+1, 1);
   //  Back
   glColor3f(1,1,1);
   if (ntex) glBindTexture(GL_TEXTURE_2D,texture[n]);
   glBegin(GL_QUADS);
   glNormal3f( 0, 0,-1);
   glTexCoord2f(0,0); glVertex3f(+1,-1,-1);
   glTexCoord2f(1,0); glVertex3f(-1,-1,-1);
   glTexCoord2f(1,1); glVertex3f(-1,+1,-1);
   glTexCoord2f(0,1); glVertex3f(+1,+1,-1);
   glEnd();
   //  Right
   glColor3f(1,1,1);
   if (ntex) glBindTexture(GL_TEXTURE_2D,texture[n]);
   glBegin(GL_QUADS);
   glNormal3f(+1, 0, 0);
   glTexCoord2f(0,0); glVertex3f(+1,-1,+1);
   glTexCoord2f(1,0); glVertex3f(+1,-1,-1);
   glTexCoord2f(1,1); glVertex3f(+1,+1,-1);
   glTexCoord2f(0,1); glVertex3f(+1,+1,+1);
   glEnd();
   //  Left
   glColor3f(1,1,1);
   if (ntex) glBindTexture(GL_TEXTURE_2D,texture[n]);
   glBegin(GL_QUADS);
   glNormal3f(-1, 0, 0);
   glTexCoord2f(0,0); glVertex3f(-1,-1,-1);
   glTexCoord2f(1,0); glVertex3f(-1,-1,+1);
   glTexCoord2f(1,1); glVertex3f(-1,+1,+1);
   glTexCoord2f(0,1); glVertex3f(-1,+1,-1);
   glEnd();
   //  Top
   glColor3f(1,1,1);
   if (ntex) glBindTexture(GL_TEXTURE_2D,texture[10]);
   glBegin(GL_QUADS);
   glNormal3f( 0,+1, 0);
   glTexCoord2f(0,0); glVertex3f(-1,+1,+1);
   glTexCoord2f(1,0); glVertex3f(+1,+1,+1);
   glTexCoord2f(1,1); glVertex3f(+1,+1,-1);
   glTexCoord2f(0,1); glVertex3f(-1,+1,-1);
   glEnd();
   //  Bottom
   glColor3f(1,1,1);
   if (ntex) glBindTexture(GL_TEXTURE_2D,texture[0]);
   glBegin(GL_QUADS);
   glNormal3f( 0,-1, 0);
   glTexCoord2f(0,0); glVertex3f(-1,-1,-1);
   glTexCoord2f(1,0); glVertex3f(+1,-1,-1);
   glTexCoord2f(1,1); glVertex3f(+1,-1,+1);
   glTexCoord2f(0,1); glVertex3f(-1,-1,+1);
   glEnd();
   //  End
   //  Undo transofrmations
   glPopMatrix();
   glDisable(GL_TEXTURE_2D);

}

static void house_3(double x,double y,double z,
                 double dx,double dy,double dz,
                 double th, int n)
{
   //  Set specular color to white
   float white[] = {1,1,1,1};
   float Emission[]  = {0.0,0.0,0.01*emission,1.0};
   glMaterialfv(GL_FRONT_AND_BACK,GL_SHININESS,shinyvec);
   glMaterialfv(GL_FRONT_AND_BACK,GL_SPECULAR,white);
   glMaterialfv(GL_FRONT_AND_BACK,GL_EMISSION,Emission);
   //  Save transformation
   glPushMatrix();
   //  Offset, scale and rotate
   glTranslated(x,y,z);
   glRotated(th,0,1,0);
   glScaled(dx,dy,dz);
   //  Enable textures
   glEnable(GL_TEXTURE_2D);
   glTexEnvi(GL_TEXTURE_ENV,GL_TEXTURE_ENV_MODE,mode?GL_REPLACE:GL_MODULATE);
   glColor3f(1,1,1);
   glBindTexture(GL_TEXTURE_2D,texture[3]);
   
   //  Cube
   //  Front
   glColor3f(1,1,1);
   if (ntex) glBindTexture(GL_TEXTURE_2D,texture[n]);
   glBegin(GL_QUADS);
   glNormal3f( 0, 0, 1);
   glTexCoord2f(0,0); glVertex3f(-1,-1, 1);
   glTexCoord2f(1,0); glVertex3f(+1,-1, 1);
   glTexCoord2f(1,1); glVertex3f(+1,+1, 1);
   glTexCoord2f(0,1); glVertex3f(-1,+1, 1);
   glEnd();
   
   glColor3f(1,1,1);
   if (ntex) glBindTexture(GL_TEXTURE_2D,texture[n]);
   glBegin(GL_QUADS);
   glNormal3f( 0, 0, 1);
   glTexCoord2f(0,0); glVertex3f(+1,-1, 1);
   glTexCoord2f(1,0); glVertex3f(+3,-1, 1);
   glTexCoord2f(1,1); glVertex3f(+3,+1, 1);
   glTexCoord2f(0,1); glVertex3f(+1,+1, 1);
   glEnd();

   //  Back
   glColor3f(1,1,1);
   if (ntex) glBindTexture(GL_TEXTURE_2D,texture[n]);
   glBegin(GL_QUADS);
   glNormal3f( 0, 0, -1);
   glTexCoord2f(0,0); glVertex3f(-1,-1, -1);
   glTexCoord2f(1,0); glVertex3f(+1,-1, -1);
   glTexCoord2f(1,1); glVertex3f(+1,+1, -1);
   glTexCoord2f(0,1); glVertex3f(-1,+1, -1);
   glEnd();
   
   glColor3f(1,1,1);
   if (ntex) glBindTexture(GL_TEXTURE_2D,texture[n]);
   glBegin(GL_QUADS);
   glNormal3f( 0, 0, -1);
   glTexCoord2f(0,0); glVertex3f(+1,-1, -1);
   glTexCoord2f(1,0); glVertex3f(+3,-1, -1);
   glTexCoord2f(1,1); glVertex3f(+3,+1, -1);
   glTexCoord2f(0,1); glVertex3f(+1,+1, -1);
   glEnd();  

    //  Right
   glColor3f(1,1,1);
   if (ntex) glBindTexture(GL_TEXTURE_2D,texture[n]);
   glBegin(GL_QUADS);
   glNormal3f(+1, 0, 0);
   glTexCoord2f(0,0); glVertex3f(+3,-1,+1);
   glTexCoord2f(1,0); glVertex3f(+3,-1,-1);
   glTexCoord2f(1,1); glVertex3f(+3,+1,-1);
   glTexCoord2f(0,1); glVertex3f(+3,+1,+1);
   glEnd();
   //  Left
   glColor3f(1,1,1);
   if (ntex) glBindTexture(GL_TEXTURE_2D,texture[n]);
   glBegin(GL_QUADS);
   glNormal3f(-1, 0, 0);
   glTexCoord2f(0,0); glVertex3f(-1,-1,-1);
   glTexCoord2f(1,0); glVertex3f(-1,-1,+1);
   glTexCoord2f(1,1); glVertex3f(-1,+1,+1);
   glTexCoord2f(0,1); glVertex3f(-1,+1,-1);
   glEnd();

   //  Top
   glColor3f(1,1,1);
   if (ntex) glBindTexture(GL_TEXTURE_2D,texture[11]);
   glBegin(GL_QUADS);
   glNormal3f( 0,+1, 0);
   glTexCoord2f(0,0); glVertex3f(-1,+1,+1);
   glTexCoord2f(1,0); glVertex3f(+3,+1,+1);
   glTexCoord2f(1,1); glVertex3f(+3,+1,-1);
   glTexCoord2f(0,1); glVertex3f(-1,+1,-1);
   glEnd();
   //  Bottom
   glColor3f(1,1,1);
   if (ntex) glBindTexture(GL_TEXTURE_2D,texture[0]);
   glBegin(GL_QUADS);
   glNormal3f( 0,-1, 0);
   glTexCoord2f(0,0); glVertex3f(-1,-1,-1);
   glTexCoord2f(1,0); glVertex3f(+3,-1,-1);
   glTexCoord2f(1,1); glVertex3f(+3,-1,+1);
   glTexCoord2f(0,1); glVertex3f(-1,-1,+1);
   glEnd();
    // End
    // Undo transofrmations
   glPopMatrix();
   glDisable(GL_TEXTURE_2D);

}
static void bell(double x, double y, double z) {

   glPushMatrix();
   glTranslated(x,y,z);
   glScaled(0.075,0.08,0.075);
   glColor3f(0.4,0.2,0.2);
   glCallList(obj);
   glPopMatrix();

}

static void bell_tower(double x,double y,double z,
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
   //  Offset, scale and rotate
   glTranslated(x,y,z);
   glRotated(th,0,1,0);
   glScaled(dx,dy,dz);
   //  Enable textures
   glEnable(GL_TEXTURE_2D);
   glTexEnvi(GL_TEXTURE_ENV,GL_TEXTURE_ENV_MODE,mode?GL_REPLACE:GL_MODULATE);
   glColor3f(1,1,1);
   glBindTexture(GL_TEXTURE_2D,texture[17]);
   
   //  Cube
   //  Front
   glColor3f(1,1,1);
   if (ntex) glBindTexture(GL_TEXTURE_2D,texture[17]);
   glBegin(GL_QUADS);
   glNormal3f( 0, 0, 1);
   glTexCoord2f(0,0); glVertex3f(-1,-1, 1);
   glTexCoord2f(1,0); glVertex3f(+1,-1, 1);
   glTexCoord2f(1,1); glVertex3f(+1,+1, 1);
   glTexCoord2f(0,1); glVertex3f(-1,+1, 1);
   glEnd();
   glColor3f(1,1,1);
   if (ntex) glBindTexture(GL_TEXTURE_2D,texture[17]);
   glBegin(GL_QUADS);
   glNormal3f( 0, 0, 1);
   glTexCoord2f(0,0); glVertex3f(-1,+1, 1);
   glTexCoord2f(1,0); glVertex3f(+1,+1, 1);
   glTexCoord2f(1,1); glVertex3f(+1,+3, 1);
   glTexCoord2f(0,1); glVertex3f(-1,+3, 1);
   glEnd();

   //  Back
   glColor3f(1,1,1);
   if (ntex) glBindTexture(GL_TEXTURE_2D,texture[17]);
   glBegin(GL_QUADS);
   glNormal3f( 0, 0,-1);
   glTexCoord2f(0,0); glVertex3f(+1,-1,-1);
   glTexCoord2f(1,0); glVertex3f(-1,-1,-1);
   glTexCoord2f(1,1); glVertex3f(-1,+1,-1);
   glTexCoord2f(0,1); glVertex3f(+1,+1,-1);
   glEnd();
   glColor3f(1,1,1);
   if (ntex) glBindTexture(GL_TEXTURE_2D,texture[17]);
   glBegin(GL_QUADS);
   glNormal3f( 0, 0,-1);
   glTexCoord2f(0,0); glVertex3f(+1,+1,-1);
   glTexCoord2f(1,0); glVertex3f(-1,+1,-1);
   glTexCoord2f(1,1); glVertex3f(-1,+3,-1);
   glTexCoord2f(0,1); glVertex3f(+1,+3,-1);
   glEnd();
   //  Right
   glColor3f(1,1,1);
   if (ntex) glBindTexture(GL_TEXTURE_2D,texture[17]);
   glBegin(GL_QUADS);
   glNormal3f(+1, 0, 0);
   glTexCoord2f(0,0); glVertex3f(+1,-1,+1);
   glTexCoord2f(1,0); glVertex3f(+1,-1,-1);
   glTexCoord2f(1,1); glVertex3f(+1,+1,-1);
   glTexCoord2f(0,1); glVertex3f(+1,+1,+1);
   glEnd();
   glColor3f(1,1,1);
   if (ntex) glBindTexture(GL_TEXTURE_2D,texture[17]);
   glBegin(GL_QUADS);
   glNormal3f(+1, 0, 0);
   glTexCoord2f(0,0); glVertex3f(+1,+1,+1);
   glTexCoord2f(1,0); glVertex3f(+1,+1,-1);
   glTexCoord2f(1,1); glVertex3f(+1,+3,-1);
   glTexCoord2f(0,1); glVertex3f(+1,+3,+1);
   glEnd();
   //  Left
   glColor3f(1,1,1);
   if (ntex) glBindTexture(GL_TEXTURE_2D,texture[17]);
   glBegin(GL_QUADS);
   glNormal3f(-1, 0, 0);
   glTexCoord2f(0,0); glVertex3f(-1,-1,-1);
   glTexCoord2f(1,0); glVertex3f(-1,-1,+1);
   glTexCoord2f(1,1); glVertex3f(-1,+1,+1);
   glTexCoord2f(0,1); glVertex3f(-1,+1,-1);
   glEnd();
   glColor3f(1,1,1);
   if (ntex) glBindTexture(GL_TEXTURE_2D,texture[17]);
   glBegin(GL_QUADS);
   glNormal3f(-1, 0, 0);
   glTexCoord2f(0,0); glVertex3f(-1,+1,-1);
   glTexCoord2f(1,0); glVertex3f(-1,+1,+1);
   glTexCoord2f(1,1); glVertex3f(-1,+3,+1);
   glTexCoord2f(0,1); glVertex3f(-1,+3,-1);
   glEnd();
   //  Top
   glColor3f(1,1,1);
   if (ntex) glBindTexture(GL_TEXTURE_2D,texture[10]);
   glBegin(GL_QUADS);
   glNormal3f( 0,+1, 0);
   glTexCoord2f(0,0); glVertex3f(-1,+3,+1);
   glTexCoord2f(1,0); glVertex3f(+1,+3,+1);
   glTexCoord2f(1,1); glVertex3f(+1,+3,-1);
   glTexCoord2f(0,1); glVertex3f(-1,+3,-1);
   glEnd();
   //  Bottom
   glColor3f(1,1,1);
   if (ntex) glBindTexture(GL_TEXTURE_2D,texture[0]);
   glBegin(GL_QUADS);
   glNormal3f( 0,-1, 0);
   glTexCoord2f(0,0); glVertex3f(-1,-1,-1);
   glTexCoord2f(1,0); glVertex3f(+1,-1,-1);
   glTexCoord2f(1,1); glVertex3f(+1,-1,+1);
   glTexCoord2f(0,1); glVertex3f(-1,-1,+1);
   glEnd();
   //  End
   //  Undo transofrmations
   glPopMatrix();
   glDisable(GL_TEXTURE_2D);

}


static void ground(double x, double y, double z)
{

   float white[]={1,1,1,1};
   float Emission[] = {0.0,0.0,0.01*emission,1.0};
   glMaterialfv(GL_FRONT_AND_BACK, GL_SHININESS,shinyvec);
   glMaterialfv(GL_FRONT_AND_BACK,GL_SPECULAR,white);
   glMaterialfv(GL_FRONT_AND_BACK,GL_EMISSION,Emission);	

   glEnable(GL_TEXTURE_2D);
   glTexEnvi(GL_TEXTURE_ENV,GL_TEXTURE_ENV_MODE,mode?GL_REPLACE:GL_MODULATE);
   glColor3f(1,1,1);
   glBindTexture(GL_TEXTURE_2D,texture[0]); 

      // Save transformation
   glPushMatrix();
   // Offset
   glTranslated(x,y,z);

   //Ground Floor

   glColor3f(1,1,1);
   if (ntex) glBindTexture(GL_TEXTURE_2D,texture[13]);
   glBegin(GL_QUADS);
   glNormal3f(0,0,1);
   glTexCoord2f(0,0); glVertex3f(0,-.1,0);
   glTexCoord2f(1,0); glVertex3f(0,-.1,0.2);
   glTexCoord2f(1,1); glVertex3f(.2, -.1,0.2);
   glTexCoord2f(0,1); glVertex3f(.2, -.1,0);
   glEnd();

   glPopMatrix();
   glDisable(GL_TEXTURE_2D);  
   }

static void grass (double x, double y, double z) {
   
   float white[]={1,1,1,1};
   float Emission[] = {0.0,0.0,0.01*emission,1.0};
   glMaterialfv(GL_FRONT_AND_BACK, GL_SHININESS,shinyvec);
   glMaterialfv(GL_FRONT_AND_BACK,GL_SPECULAR,white);
   glMaterialfv(GL_FRONT_AND_BACK,GL_EMISSION,Emission);	

   glEnable(GL_TEXTURE_2D);
   glTexEnvi(GL_TEXTURE_ENV,GL_TEXTURE_ENV_MODE,mode?GL_REPLACE:GL_MODULATE);
   glColor3f(1,1,1);
   glBindTexture(GL_TEXTURE_2D,texture[6]); 

      // Save transformation
   glPushMatrix();
   // Offset
   glTranslated(x,y,z);


   //grass
   glColor3f(1,1,1);
   if (ntex) glBindTexture(GL_TEXTURE_2D,texture[6]); 
   glBegin(GL_QUADS);
   glNormal3f(0,0,1);
   glTexCoord2f(0,0); glVertex3f(0.0,0,-0.1);
   glTexCoord2f(1,0); glVertex3f(0.0,0,0.1);
   glTexCoord2f(1,1); glVertex3f(0.2,0,0.1);
   glTexCoord2f(0,1); glVertex3f(0.2,0,-0.1);
   glEnd();


   glPopMatrix();
   glDisable(GL_TEXTURE_2D);

}

static void road (double x, double y, double z) {
   
   float white[]={1,1,1,1};
   float Emission[] = {0.0,0.0,0.01*emission,1.0};
   glMaterialfv(GL_FRONT_AND_BACK, GL_SHININESS,shinyvec);
   glMaterialfv(GL_FRONT_AND_BACK,GL_SPECULAR,white);
   glMaterialfv(GL_FRONT_AND_BACK,GL_EMISSION,Emission);	

   glEnable(GL_TEXTURE_2D);
   glTexEnvi(GL_TEXTURE_ENV,GL_TEXTURE_ENV_MODE,mode?GL_REPLACE:GL_MODULATE);
   glColor3f(1,1,1);
   glBindTexture(GL_TEXTURE_2D,texture[8]); 

      // Save transformation
   glPushMatrix();
   // Offset
   glTranslated(x,y,z);


   //road
   glColor3f(1,1,1);
   if (ntex) glBindTexture(GL_TEXTURE_2D,texture[8]); 
   glBegin(GL_QUADS);
   glNormal3f(0,0,1);
   glTexCoord2f(0,0); glVertex3f(0.0,0,-0.2);
   glTexCoord2f(1,0); glVertex3f(0.0,0,0.2);
   glTexCoord2f(1,1); glVertex3f(0.4,0,0.2);
   glTexCoord2f(0,1); glVertex3f(0.4,0,-0.2);
   glEnd();


   glPopMatrix();
   glDisable(GL_TEXTURE_2D);

}

static void door (double x, double y, double z, double th) {
   
   float white[] = {1,1,1,1};
   float Emission[]  = {0.0,0.0,0.01*emission,1.0};
   glMaterialfv(GL_FRONT_AND_BACK,GL_SHININESS,shinyvec);
   glMaterialfv(GL_FRONT_AND_BACK,GL_SPECULAR,white);
   glMaterialfv(GL_FRONT_AND_BACK,GL_EMISSION,Emission);
   //  Save transformation
   glPushMatrix();
   //  Offset, scale and rotate
   glTranslated(x,y,z);
   glRotated(th,0,1,0);
   // glScaled(dx,dy,dz);
   //  Enable textures
   glEnable(GL_TEXTURE_2D);
   glTexEnvi(GL_TEXTURE_ENV,GL_TEXTURE_ENV_MODE,mode?GL_REPLACE:GL_MODULATE);
   glColor3f(1,1,1);
   glBindTexture(GL_TEXTURE_2D,texture[15]);


   //door

   //front
   glColor3f(1,1,1);
   if (ntex) glBindTexture(GL_TEXTURE_2D,texture[15]);
   glBegin(GL_QUADS);
   glNormal3f( 0, 0, 1);
   glTexCoord2f(0,0); glVertex3f(0,0,0);
   glTexCoord2f(1,0); glVertex3f(+.15,0,0);
   glTexCoord2f(1,1); glVertex3f(+.15,+.30,0);
   glTexCoord2f(0,1); glVertex3f(0,+.30,0);
   glEnd();

   //back
   glColor3f(1,1,1);
   if (ntex) glBindTexture(GL_TEXTURE_2D,texture[15]);
   glBegin(GL_QUADS);
   glNormal3f( 0, 0, -1);
   glTexCoord2f(0,0); glVertex3f(0,0,-0.02);
   glTexCoord2f(1,0); glVertex3f(+.15,0,-0.02);
   glTexCoord2f(1,1); glVertex3f(+.15,+0.30,-0.02);
   glTexCoord2f(0,1); glVertex3f(0,+0.30,-0.02);
   glEnd();

   //right
   glColor3f(1,1,1);
   if (ntex) glBindTexture(GL_TEXTURE_2D,texture[16]);
   glBegin(GL_QUADS);
   glNormal3f( 0, 0, 0);
   glTexCoord2f(0,0); glVertex3f(0.15,0,0.0);
   glTexCoord2f(1,0); glVertex3f(0.15,0,-0.02);
   glTexCoord2f(1,1); glVertex3f(0.15,+0.30,-0.02);
   glTexCoord2f(0,1); glVertex3f(0.15,+0.30,0.0);
   glEnd();

   //left
   glColor3f(1,1,1);
   if (ntex) glBindTexture(GL_TEXTURE_2D,texture[16]);
   glBegin(GL_QUADS);
   glNormal3f( 0, 0, 0);
   glTexCoord2f(0,0); glVertex3f(0.0,0,0.0);
   glTexCoord2f(1,0); glVertex3f(0.0,0,-0.02);
   glTexCoord2f(1,1); glVertex3f(0.0,+0.30,-0.02);
   glTexCoord2f(0,1); glVertex3f(0.0,+0.30,0.0);
   glEnd();

   //top
   glColor3f(1,1,1);
   if (ntex) glBindTexture(GL_TEXTURE_2D,texture[16]);
   glBegin(GL_QUADS);
   glNormal3f( 0, 0, 0);
   glTexCoord2f(0,0); glVertex3f(0,0.30,0);
   glTexCoord2f(1,0); glVertex3f(+.15,0.30,0.0);
   glTexCoord2f(1,1); glVertex3f(+.15,0.30,-0.02);
   glTexCoord2f(0,1); glVertex3f(0,0.30,-0.02);
   glEnd();


   //bottom
   glColor3f(1,1,1);
   if (ntex) glBindTexture(GL_TEXTURE_2D,texture[16]);
   glBegin(GL_QUADS);
   glNormal3f( 0, 0, 0);
   glTexCoord2f(0,0); glVertex3f(0,0,0);
   glTexCoord2f(1,0); glVertex3f(+.15,0,0.0);
   glTexCoord2f(1,1); glVertex3f(+.15,0,-0.02);
   glTexCoord2f(0,1); glVertex3f(0,0,-0.02);
   glEnd();


   glPopMatrix();
   glDisable(GL_TEXTURE_2D);

}

static void grassplane(){

	double x = -1.2;
	double y = 0.0001;
	double z = 1.7;
	for (int j = 0; j < 3; j++){
		for (int i = 0; i <3; i++){
			grass(x,y,z);
			x+=.2;
		}
		x= -1.2;
		z +=.2;
	}
	x = 1;
	z = 0.7;
	for (int j = 0; j < 8; j++){
		grass(x,y,z);
		z +=.2;
	}
	x = .8;
	z = 0.7;
	for (int j = 0; j < 3; j++){
		grass(x,y,z);
		z +=.2;
	}
	x=-2;
	z=.5;
	for (int j=0; j < 9; j++){
		for (int i = 0; i < 2; i++){
			grass(x,y,z);
			x+=.2;
		}
		x=-2;
		z+=.2;
	}
	x=-1.8;
	z=.5;
	for (int j = 0; j < 3; j++){
		for (int i = 0; i < 3; i++){
			grass(x,y,z);
			x+=.2;
		}
		x=-1.8;
		z+=.2;
	}

	x=0.5;
	z=-1.9;
	for (int j = 0; j < 4; j++){
		for (int i = 0; i < 3; i++){
			grass(x,y,z);
			x+=.2;
		}
	x=0.5;
	z+=.2;
	}

	x=-2;
	z=-1.2;
	for (int j = 0; j < 4; j++){
		for (int i = 0; i < 4; i++){
			grass(x,y,z);
			x+=.2;
		}
	  x=-2;
	  z+=.2;
	}
   x = -0.2;
   z =0.2;
   for (int j = 0; j < 6; j++){
      for (int i = 0; i < 4; i++){
         grass(x,y,z);
         x+=.2;
      }
      x = -0.2;
      z+=0.2;
   }


}

static void groundplane(){
	double x = -2;
	double y = 0.09;
	double z = 2;
	for (int j = 0; j < 21; j++){
		for (int i = 0; i < 20; i++){
			ground(x,y,z);
			x +=.2;
		}
		x = -2;
		z -=.2;
	}

}

static void roadplane(){
	double x = -2.0;
	double y = 0.001;
	double z = 0.0;
	for (int i = 0; i < 16; i++){
		road(x,y,z);
		x +=.2;
	}
	x = -0.6;
	z = -1.8;
	for (int j = 0; j < 20; j++){
		road(x,y,z);
		z +=.2;
	}
	x= -1.8;
	z= -1.8;
	for (int p = 0; p < 7; p++){
		road(x,y,z);
		x +=.2;
	}
	x=-1.2;
	z=1.4;
	for (int q = 0; q < 10; q++){
		road(x,y,z);
		x +=.2;
	}
	x=0.6;
	z=1.4;
	for (int m = 0; m < 4; m++){
		road(x,y,z);
		z +=.2;
	}
	x = 1.2;
	z = -1.0;
	for (int r = 0; r < 16; r++){
		road(x,y,z);
		z +=.2;
	}
	x= 1.2;
	z=-1.0;
	for (int b = 0; b < 3; b++){
		road(x,y,z);
		x+=0.2;
	}
	x=1.6;
	z=-1.0;
	for (int n = 0; n < 5; n++){
		road(x,y,z);
		z-=0.2;
	}
}

/*
 *  Draw a ball
 *     at (x,y,z)
 *     radius (r)
 */
static void ball(double x,double y,double z,double r, int red, int green, int blue)
{

   //  Save transformation
   glPushMatrix();
   //  Offset, scale and rotate
   glTranslated(x,y,z);
   glScaled(r,r,r);
   //  White ball
   glColor3f(red,green,blue);
   glutSolidSphere(1.0,16,16);
   //  Undo transofrmations
   glPopMatrix();
   
}

static void vertical_cylinder(double x, double y, double z, double s, double rotate, int boolean , double r, double g, double b, int text){

	// Draw barrel
   	glPushMatrix();
   	glTranslated(x,y,z);
   	glRotated(rotate, 0, 1, 0);
   	glScaled(s,s,s);
   	glColor3f(r,g,b);
   	if (ntex) glBindTexture(GL_TEXTURE_2D,texture[text]);   
   	glBegin(GL_QUAD_STRIP);
   	double angle = 0.0;
   	double n = 16;
   	for(int k =0; k<n+1; k++)
   	{
           	x = 0.0625 * Cos(angle);
           	z = 0.0625 * Sin(angle);           
           	glTexCoord2f(0,0); glNormal3f( x, 0, z); glVertex3f(x, 0 ,z) ;
           	glTexCoord2f(1,1); glNormal3f( x, 0, z); glVertex3f(x, 1.5 , z);           
           	angle = angle + 360/n;
   	}

   	glEnd();
      if (boolean == 1){
         glColor3f(1,0,0);
         ball(x,y+1.5,z,0.125,255,165,0);
      }
   	glPopMatrix();
}


static void horizontal_cylinder(double x, double y, double z, double s, double rotate, double r, double g, double b, int text){
      glPushMatrix();
      glTranslated(x,y,z);
      glRotated(rotate, 0, 1, 0);
      glScaled(s,s,s);
      glColor3f(r,g,b);
      if (ntex) glBindTexture(GL_TEXTURE_2D,texture[text]);   
      glBegin(GL_QUAD_STRIP);
      double angle = 0.0;
      double n = 16;
      for(int k =0; k<n+1; k++)
      {
            y = 0.0625 * Cos(angle);
            z = 0.0625 * Sin(angle);           
            glTexCoord2f(0,0); glNormal3f( 0, y, z); glVertex3f(0, y ,z) ;
            glTexCoord2f(1,1); glNormal3f( 0, y, z); glVertex3f(1.5, y , z);           
            angle = angle + 360/n;
      }

      glEnd();
      glPopMatrix();
}

static void monkey_bar(double x, double y, double z, double s, double rotate){
   
   float white[]={1,1,1,1};
   float Emission[] = {0.0,0.0,0.01*emission,1.0};
   glMaterialfv(GL_FRONT_AND_BACK, GL_SHININESS,shinyvec);
   glMaterialfv(GL_FRONT_AND_BACK,GL_SPECULAR,white);
   glMaterialfv(GL_FRONT_AND_BACK,GL_EMISSION,Emission);
   // Save transformation
   glPushMatrix();
   // Offset
   glTranslated(x,y,z);
   glRotated(rotate, 0, 1, 0);


   //Enable Textures
   glEnable(GL_TEXTURE_2D);
   glTexEnvi(GL_TEXTURE_ENV,GL_TEXTURE_ENV_MODE,mode?GL_REPLACE:GL_MODULATE);
   glColor3f(1,1,1);
   glBindTexture(GL_TEXTURE_2D,texture[32]); 

   vertical_cylinder(0,0,0,0.175,0,0,1,1,1,33);
   vertical_cylinder(0.1,0,0,0.175,0,0,1,1,1,33);

   horizontal_cylinder(0,0.2525,0,0.175,90,1,1,1,33);
   horizontal_cylinder(0.1,0.2525,0,0.175,90,1,1,1,33);

   vertical_cylinder(0,0,-0.265,0.175,0,0,1,1,1,33);
   vertical_cylinder(0.1,0,-0.265,0.175,0,0,1,1,1,33);

   //little ones
   horizontal_cylinder(0,0.2525,0,0.06,0,1,1,1,32);
   horizontal_cylinder(0,0.2525,-0.06625,0.06,0,1,1,1,32);
   horizontal_cylinder(0,0.2525,-0.1325,0.06,0,1,1,1,32);
   horizontal_cylinder(0,0.2525,-0.19875,0.06,0,1,1,1,32);
   horizontal_cylinder(0,0.2525,-0.06625,0.06,0,1,1,1,32);
   horizontal_cylinder(0,0.2525,-0.265,0.06,0,1,1,1,32);
   horizontal_cylinder(0,0.06625,0,0.06,0,1,1,1,32);
   horizontal_cylinder(0,0.06625,-0.265,0.06,0,1,1,1,32);

   glPopMatrix();
   glDisable(GL_TEXTURE_2D);  

}

static void streetlamp_plane(){
	vertical_cylinder(-0.6125,0,0.225,0.25,45,1,0,0,0,28);
 	vertical_cylinder(-1.125,0,0.225,0.25,90,1,0,0,0,28);
 	vertical_cylinder(-0.6125,0,-0.225,0.25,-45,1,0,0,0,28);
 	vertical_cylinder(-1.125,0,-0.225,0.25,-90,1,0,0,0,28);

 	vertical_cylinder(-0.185,0,0.225,0.25,135,1,0,0,0,28);
 	// vertical_cylinder(0.3,0,0.225,0.25,90,0,0,0,28);
 	vertical_cylinder(0.585,0,0.225,0.25,90,1,0,0,0,28);
 	vertical_cylinder(1.185,0,0.225,0.25,45,1,0,0,0,28);

 	vertical_cylinder(-0.185,0,-0.225,0.25,-135,1,0,0,0,28);
 	vertical_cylinder(1.185,0,-0.225,0.25,-45,1,0,0,0,28);
 	vertical_cylinder(1.185,0,-1.2,0.25,-45,1,0,0,0,28);

 	vertical_cylinder(1.625,0,-.785,0.25,135,1,0,0,0,28);
 	vertical_cylinder(1.625,0,-0.225,0.25,180,1,0,0,0,28);
 	vertical_cylinder(1.625,0,0.225,0.25,180,1,0,0,0,28);
 	vertical_cylinder(1.625,0,1,0.25,180,1,0,0,0,28);
 	vertical_cylinder(1.625,0,1.8,0.25,180,1,0,0,0,28);

 	vertical_cylinder(-0.6125,0,-.725,0.25,0,1,0,0,0,28);
 	vertical_cylinder(-0.6125,0,-1.59,0.25,45,1,0,0,0,28);
 	vertical_cylinder(-1.125,0,-1.59,0.25,90,1,0,0,0,28);

 	vertical_cylinder(-1.125,0,1.175,0.25,-90,1,0,0,0,28);
 	vertical_cylinder(-0.6125,0,1.175,0.25,-45,1,0,0,0,28);
 	vertical_cylinder(-0.185,0,1.175,0.25,-135,1,0,0,0,28);
 	vertical_cylinder(-0.185,0,1.625,0.25,135,1,0,0,0,28);
 	vertical_cylinder(0.5,0,1.625,0.25,45,1,0,0,0,28);


 	vertical_cylinder(-0.185,0,-1.25,0.25,180,1,0,0,0,28);
}


static void cartwheel(double x, double y, double z, double s){
	glPushMatrix();
   	glTranslated(x,y,z);
   	glScaled(s,s,s);
   	glColor3f(1,1,1);
   	if (ntex) glBindTexture(GL_TEXTURE_2D,texture[31]);   
   	glBegin(GL_QUAD_STRIP);
   	double angle = 0.0;
   	double n = 16;
   	for(int k =0; k<n+1; k++)
   	{
           	x = 0.0625 * Cos(angle);
           	y = 0.0625 * Sin(angle);           
           	glTexCoord2f(0,0); glNormal3f( x, y, 0); glVertex3f(x, y , 0) ;
           	glTexCoord2f(1,1); glNormal3f( x, y, 0); glVertex3f(x, y , 0.025);           
           	angle = angle + 360/n;
   	}
   	glEnd();
   	glPopMatrix();
}

static void cart(double x,double y,double z, double th)
{
   const double wid=0.1;
   const double nose=+0.20;
   const double side_1= 0.20;
   const double tail=-0.2;

   float white[] = {1,1,1,1};
   float Emission[]  = {0.0,0.0,0.01*emission,1.0};
   glMaterialfv(GL_FRONT_AND_BACK,GL_SHININESS,shinyvec);
   glMaterialfv(GL_FRONT_AND_BACK,GL_SPECULAR,white);
   glMaterialfv(GL_FRONT_AND_BACK,GL_EMISSION,Emission);
   //  Save transformation
   glPushMatrix();
   //  Offset, scale and rotate
   glTranslated(x,y,z);
   glRotated(th,0,1,0);

   glEnable(GL_TEXTURE_2D);
   glTexEnvi(GL_TEXTURE_ENV,GL_TEXTURE_ENV_MODE,mode?GL_REPLACE:GL_MODULATE);
   glColor3f(1,1,1);
   glBindTexture(GL_TEXTURE_2D,texture[0]);

   cartwheel(nose*.6,-0.085,wid,1);	
   cartwheel(tail*.6,-0.085,wid,1);
   cartwheel(nose*.6,-0.085,-wid-0.025,1);	
   cartwheel(tail*.6,-0.085,-wid-0.025,1);		
   
   // left
   glColor3f(1,1,1);
   if (ntex) glBindTexture(GL_TEXTURE_2D,texture[30]);
   glBegin(GL_QUADS);
   glNormal3f( -1, 0, 0);
   glTexCoord2d(0,0);glVertex3d(side_1, wid, wid);
   glTexCoord2d(1,0);glVertex3d(side_1,-wid, wid);
   glTexCoord2d(1,1);glVertex3d(tail,-wid, wid);
   glTexCoord2d(0,1);glVertex3d(tail, wid, wid);
   glEnd();

   // right
   glColor3f(1,1,1);
   if (ntex) glBindTexture(GL_TEXTURE_2D,texture[30]);
   glBegin(GL_QUADS);
   glNormal3f( +1, 0, 0);
   glTexCoord2d(0,0);glVertex3d(side_1, wid,-wid);
   glTexCoord2d(1,0);glVertex3d(side_1,-wid,-wid);
   glTexCoord2d(1,1);glVertex3d(tail,-wid,-wid);
   glTexCoord2d(0,1);glVertex3d(tail, wid,-wid);
   glEnd();

   //bottom
   glColor3f(1,1,1);
   if (ntex) glBindTexture(GL_TEXTURE_2D,texture[29]);
   glBegin(GL_QUADS);
   glNormal3f( 0, -1, 0);
   glTexCoord2d(0,0);glVertex3d(side_1,-wid, wid);
   glTexCoord2d(1,0);glVertex3d(side_1,-wid,-wid);
   glTexCoord2d(1,1);glVertex3d(tail,-wid,-wid);
   glTexCoord2d(0,1);glVertex3d(tail,-wid, wid);
   glEnd();

   // back
   glColor3f(1,1,1);
   if (ntex) glBindTexture(GL_TEXTURE_2D,texture[30]);
   glBegin(GL_QUADS);
   glNormal3f( 0, 0, -1);
   glTexCoord2d(0,0);glVertex3d(tail,-wid, wid);
   glTexCoord2d(1,0);glVertex3d(tail, wid, wid);
   glTexCoord2d(1,1);glVertex3d(tail, wid,-wid);
   glTexCoord2d(0,1);glVertex3d(tail,-wid,-wid);
   glEnd();

   //front 

   glColor3f(1,1,1);
   if (ntex) glBindTexture(GL_TEXTURE_2D,texture[30]);
   glBegin(GL_QUADS);
   glNormal3f( 0, 0, 1);
   glTexCoord2d(0,0);glVertex3d(nose,-wid, wid);
   glTexCoord2d(1,0);glVertex3d(nose, wid, wid);
   glTexCoord2d(1,1);glVertex3d(nose, wid,-wid);
   glTexCoord2d(0,1);glVertex3d(nose,-wid,-wid);
   glEnd();

   //  Undo transformations
   glPopMatrix();
   glDisable(GL_TEXTURE_2D);  
}


/* 
 *  Draw sky box From Vlakkies Schreuder's ex25
 */
static void Sky(double D)
{
   glColor3f(1,1,1);
   glEnable(GL_TEXTURE_2D);

   //  Sides
   glBindTexture(GL_TEXTURE_2D,texture[26]);
   glBegin(GL_QUADS);
   glTexCoord2f(0.00,0); glVertex3f(-D,-D,-D);
   glTexCoord2f(0.25,0); glVertex3f(+D,-D,-D);
   glTexCoord2f(0.25,1); glVertex3f(+D,+D,-D);
   glTexCoord2f(0.00,1); glVertex3f(-D,+D,-D);

   glTexCoord2f(0.25,0); glVertex3f(+D,-D,-D);
   glTexCoord2f(0.50,0); glVertex3f(+D,-D,+D);
   glTexCoord2f(0.50,1); glVertex3f(+D,+D,+D);
   glTexCoord2f(0.25,1); glVertex3f(+D,+D,-D);

   glTexCoord2f(0.50,0); glVertex3f(+D,-D,+D);
   glTexCoord2f(0.75,0); glVertex3f(-D,-D,+D);
   glTexCoord2f(0.75,1); glVertex3f(-D,+D,+D);
   glTexCoord2f(0.50,1); glVertex3f(+D,+D,+D);

   glTexCoord2f(0.75,0); glVertex3f(-D,-D,+D);
   glTexCoord2f(1.00,0); glVertex3f(-D,-D,-D);
   glTexCoord2f(1.00,1); glVertex3f(-D,+D,-D);
   glTexCoord2f(0.75,1); glVertex3f(-D,+D,+D);
   glEnd();

   //  Top and bottom
   glBindTexture(GL_TEXTURE_2D,texture[27]);
   glBegin(GL_QUADS);
   glTexCoord2f(0.0,0); glVertex3f(+D,+D,-D);
   glTexCoord2f(0.5,0); glVertex3f(+D,+D,+D);
   glTexCoord2f(0.5,1); glVertex3f(-D,+D,+D);
   glTexCoord2f(0.0,1); glVertex3f(-D,+D,-D);

   glTexCoord2f(1.0,1); glVertex3f(-D,-D,+D);
   glTexCoord2f(0.5,1); glVertex3f(+D,-D,+D);
   glTexCoord2f(0.5,0); glVertex3f(+D,-D,-D);
   glTexCoord2f(1.0,0); glVertex3f(-D,-D,-D);
   glEnd();

   glDisable(GL_TEXTURE_2D);
}

static void town(){
	//house #1 Starting house
 	house_1(-1.7,.2,0,.3,.2,.4,0,20,4);
 	door(-1.39,0,0.075,90); // main door
 	door(-1.8,0,.415,0); //side door

 	// house #2 - top left
 	house_2(-1.6,.2,-1.7,.3,.2,.3,0,2); 
 	door(-1.285,0,-1.7,90); //main door
 	
 	//house #3 - pub 
 	house_3(.2,.2,-.45,.3,.2,.2,0,3);
 	door(0.4,0,-.245,0); // main door
 	door(1.1175,0,-0.375,90); // side door
 	door(-0.090,0,-0.375,90); //side door

 	//house #4 - between #1 and #2. 3 houses there
 	house_1(-0.815,.2,-1.4,.15,.2,.2,90, 20, 4);

 	house_1(-0.785,.2,-.95,.15,.2,.15,0, 19, 23);

 	house_1(-0.85,.2,-.45,.2,.2,.2,-90, 21, 25);


 	//house #5 - next to the grass, bottom left.
 	house_3(-1.4,.2,1.6,.2,.2,.2,90, 20);

 	//house #6 bottom left infront of the grass
 	house_2(-1.4,.3,2,.2,.3,.185,0, 21);


 	//house #7 - corner house, bottom left adjacent to the grass
 	house_1(-0.830,.2,.985,.2,.2,.2,0, 22, 24);

 	house_2(-0.83,.2,.45,.2,.2,.2,0, 18);


 	//Bell Tower

 	bell_tower(0.2,.3,1.925,.25,.3,.25,0);

 	// house #8 infront of bell tower

 	// house_2(0.05,0.2,0.95,.2,.2,.2,0, 20);
 	// house_1(0.05,0.2,0.45,.2,.2,.2,180, 2, 4);
 	// house_1(0.55,0.2,0.45,.2,.2,.2,90, 3, 23);
 	house_2(1,0.3,0.42,.18,.3,.18,0, 19);
 	// house_1(0.55,0.2,0.95,.2,.2,.2,-90, 21, 24);

 	//house #9s the strip
 	house_1(1.83,.2,2,.17,.2,.17,180, 22, 25);
 	house_1(1.83,.2,1.585,.17,.2,.17,180, 18, 24);
 	house_2(1.83,.2,1.19,.17,.2,.17,180, 2);
 	house_3(1.83,.2,.8,.17,.2,.17,90, 3);
 	house_1(1.83,.2,.05,.17,.2,.17,180, 22, 23);
 	house_2(1.83,.2,-.5,.17,.2,.26,180, 2);

 	//house #10s 
 	house_1(0.1,.2,-.95,.2,.2,.2,180, 21, 4);
 	house_1(0.05,.2,-1.375,.15,.2,.15,180, 22,25);
 	house_1(0.1,.2,-1.8,.2,.2,.2,180, 21,24);
 	house_1(.75,.2,-.95,.35,.2,.2,0,20,23);

 	house_3(1.375,.2,-1.425,.185,.2,.17,90, 19);
}

/*
 *  OpenGL (GLUT) calls this routine to display the scene
 */


void display()
{
   const double len=2.0;  //  Length of axes
  
   //  Erase the window and the depth buffer
   glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
   //  Enable Z-buffering in OpenGL
   glEnable(GL_DEPTH_TEST);

   //  Undo previous transformations
   glLoadIdentity();
     //  Perspective - set eye position
   if (pmode == 0){
      double Ex = -2 * dim * Sin(th) * Cos(ph);
      double Ey = +2 * dim * Sin(ph);
      double Ez = +2 * dim * Cos(th) * Cos(ph);
      gluLookAt(Ex,Ey,Ez,0,0,0,0,Cos(ph),0);
   }

   //First Person Projection
   else if (pmode == 1) {
      Cx = -2 * dim * Sin(th) * Cos(ph) + xOffset;
      Cy = +2 * dim * Sin(ph) + yOffset;
      Cz = +2 * dim * Cos(th) * Cos(ph) + zOffset;
      gluLookAt(xOffset,yOffset,zOffset,Cx,Cy,Cz,0,Cos(ph),0);
    }

    // Light switch
   if (light)
   {
        //  Translate intensity to color vectors
        float Ambient[]   = {0.01*ambient ,0.01*ambient ,0.01*ambient ,1.0};
        float Diffuse[]   = {0.01*diffuse ,0.01*diffuse ,0.01*diffuse ,1.0};
        float Specular[]  = {0.01*specular,0.01*specular,0.01*specular,1.0};
        //  Light position
        float Position[]  = {7.5*Cos(zh),ylight,7.5*Sin(zh),1.0};
        //  Draw light position as ball (still no lighting here)
        glColor3f(1,1,1);
        ball(Position[0],Position[1],Position[2] , 0.1,1,1,1);
        //  OpenGL should normalize normal vectors
        glEnable(GL_NORMALIZE);
        //  Enable lighting
        glEnable(GL_LIGHTING);
        //  Location of viewer for specular calculations
        // glLightModeli(GL_LIGHT_MODEL_LOCAL_VIEWER,local);
        //  glColor sets ambient and diffuse color materials
        glColorMaterial(GL_FRONT_AND_BACK,GL_AMBIENT_AND_DIFFUSE);
        glEnable(GL_COLOR_MATERIAL);
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

   if (fog){
      glEnable(GL_FOG); {
         GLfloat fogColor[4] = {0.25,0.25,0.25,1};
         // fogMode = GL_EXP;
         glFogi(GL_FOG_MODE, GL_EXP);
         glFogfv(GL_FOG_COLOR, fogColor);
         glFogf(GL_FOG_DENSITY,0.7);
         glHint(GL_FOG_HINT, GL_DONT_CARE);
         glFogf(GL_FOG_START,1);
         glFogf(GL_FOG_END, 5);
      }

      glClearColor(0.5,0.5,0.5,1.0);
   }
   else glDisable(GL_FOG);

 	town();
	cart(-0.425,0.15, 1.75*Cos(zh), 90); 
   cart(-0.9,0.15, -1.8, 0); 
   cart(0.1,0.15, 0.8, -45); 

   monkey_bar(0.3,0,0.55,.5,0);
   monkey_bar(1,0,-1.4,.5,90);
	Sky(6*dim);
	groundplane();
	roadplane();
	grassplane();
	streetlamp_plane();
   bell(0.2,1.25,1.9);
   
   //Pillars
   glEnable(GL_TEXTURE_2D);
   glTexEnvi(GL_TEXTURE_ENV,GL_TEXTURE_ENV_MODE,mode?GL_REPLACE:GL_MODULATE);
   glColor3f(1,1,1);
   glBindTexture(GL_TEXTURE_2D,texture[15]); 
   vertical_cylinder(0.43,1.2,1.89,0.175,0,0,1,1,1,15);
   vertical_cylinder(-.03,1.2,1.89,0.175,0,0,1,1,1,15);
   vertical_cylinder(0.2,1.2,2.1395,0.175,0,0,1,1,1,15);
   glPopMatrix();
   glDisable(GL_TEXTURE_2D);  

   //  Draw axes - no lighting from here on
   glDisable(GL_LIGHTING);
   glColor3f(1,1,1);
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
   Print("Angle=%d,%d  Dim=%.1f FOV=%d  Light=%s Texture=%s  Projection=%s",th,ph,dim,fov,light?"On":"Off", mode?"Replace":"Modulate", pmode[pmodes]);
   if (light)
   {
      glWindowPos2i(5,25);
      Print("Ambient=%d  Diffuse=%d Specular=%d Emission=%d Shininess=%.0f",ambient,diffuse,specular,emission,shinyvec[0]);
   }
   //  Render the scene and make it visible

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
   zh = fmod(90*t,360.0);
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
   //  Smooth color model
   else if (key == GLUT_KEY_F1)
      smooth = 1-smooth;
   //  Local Viewer
   else if (key == GLUT_KEY_F2)
      local = 1-local;
   else if (key == GLUT_KEY_F3)
      distance = (distance==1) ? 5 : 1;
   //  Toggle ball increment
   else if (key == GLUT_KEY_F8)
      inc = (inc==10)?3:10;
   //  Flip sign
   else if (key == GLUT_KEY_F9)
      one = -one;

  	   //  First person view angle is limited by 45 degrees
   else if (key == GLUT_KEY_UP){
      if (pmode == 1){
         if (ph < 45) ph += 5;
         else ph = 45;
      }
      else ph += 5;  
   }
   //  Down arrow key - decrease elevation by 5 degrees
   else if (key == GLUT_KEY_DOWN){
      if (pmode == 1){
         if (ph > -45) ph -= 5;
         else ph = -45;
      }
      else ph -= 5;
   }

   //  Keep angles to +/-360 degrees
   th %= 360;
   ph %= 360;
   //  Update projection
   Project(45,asp,dim);
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


   else if (ch == '9'){
      pmode = 1;
   	th = ph = 0;
  		xOffset = -0.4;
      yOffset = 0.1;
      zOffset = -3;
   }
   else if (ch == '0'){
      pmode = 1;
   	th = -90;
   	ph = 0;
  		xOffset = -1.6;
      yOffset = 0.1;
      zOffset = 0;
   }
   else if (ch == '8'){
      pmode = 1;
      th = 0;
      ph = 0;
      xOffset = 0.2;
      yOffset = 1.3;
      zOffset = 0.8;
   }
   else if (ch =='7'){
      pmode = 1;
      th = -45;
      ph = 0;
      xOffset = -0.3;
      yOffset = 0.1;
      zOffset =0.1;
   }

   //  Toggle axes
   else if (ch == 'x' || ch == 'X')
      axes = 1-axes;
   //  Toggle lighting
   else if (ch == 'l' || ch == 'L')
      light = 1-light;
   else if (ch == 'f' || ch =='F')
      fog = 1- fog;
   //  Toggle light movement
   else if (ch == 'u' || ch == 'U')
      move = 1-move;
   //  Toggle texture mode
   else if (ch == 'm' || ch == 'M')
      mode = 1-mode;
   // Toggle textures 
   else if (ch=='t')
      ntex = 1-ntex;
   //  Move light
   else if (ch == '>')
      zh += 1;
   else if (ch == '<')
      zh -= 1;

     //  Switch display mode
   else if (ch == 'k' || ch == 'K') {
      //Perspective View
      if (pmode == 1){
         pmode = 0;
         xOffset =-2;
         yOffset =3;
         zOffset = -2;
         ph = 45;
         th = -45;
         dim = 2.6;
      }

      //First Person View
      else if (pmode == 0){
         pmode = 1;
         fov =55;
         ph = 0;
         th=-90;
         xOffset = -1.2;
         yOffset = 0.1;
         zOffset = 0;
         dim = 3.6;
      }
   }

   //Chage Field Of View
   else if (ch =='-' && ch > 1){
      if (pmode == 1) fov = 55;
      else fov--;
   }
   else if (ch == '+' && ch<179){
      if (pmode ==1) fov =55;
      else fov++;
   }   
      //Forward and Sideways movement - wasd
   else if (ch == 'w' && pmode == 1)
   {
      xOffset += 0.01 * (Cx - xOffset);
      zOffset += 0.01 * (Cz - zOffset);
   }
   else if (ch == 's' && pmode == 1)
   {
      xOffset -= 0.01 * (Cx - xOffset);
      zOffset -= 0.01 * (Cz - zOffset);
   }
   else if (ch == 'a' && pmode == 1)
   {
      xOffset += 0.015 * ((yOffset * Cz) - (zOffset * Cy));
      zOffset += 0.015 * ((xOffset * Cy) - (yOffset * Cx));
   }
   else if (ch == 'd' && pmode == 1)
   {
      xOffset -= 0.015 * ((yOffset * Cz) - (zOffset * Cy));
      zOffset -= 0.015 * ((xOffset * Cy) - (yOffset * Cx));
   }
   //  Light elevation
   else if (ch=='[')
      ylight -= 0.1;
   else if (ch==']')
      ylight += 0.1;
   //  Ambient level
   else if (ch=='1' && ambient>0)
      ambient -= 5;
   else if (ch=='!' && ambient<100)
      ambient += 5;
   //  Diffuse level
   else if (ch=='2' && diffuse>0)
      diffuse -= 5;
   else if (ch=='@' && diffuse<100)
      diffuse += 5;
   //  Specular level
   else if (ch=='3' && specular>0)
      specular -= 5;
   else if (ch=='#' && specular<100)
      specular += 5;
   //  Emission level
   else if (ch=='4' && emission>0)
      emission -= 5;
   else if (ch=='$' && emission<100)
      emission += 5;
   //  Shininess level
   else if (ch=='5' && shininess>-1)
      shininess -= 1;
   else if (ch=='%' && shininess<7)
      shininess += 1;
      //  PageUp key - increase dim
   else if (ch == 'g')
      dim += 0.1;
   //  PageDown key - decrease dim
   else if (ch == 'h' && dim>1)
      dim -= 0.1;
   //  Translate shininess power to value (-1 => 0)
   shinyvec[0] = shininess<0 ? 0 : pow(2.0,shininess);
   //  Reproject
   Project(45,asp,dim);
   // //  Animate if requested
   glutIdleFunc(move?idle:NULL);
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
   Project(45,asp,dim);
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
   glutInitWindowSize(1300,800);
   glutCreateWindow("Nishesh Shukla Final Project");
   //  Set callbacks
   glutDisplayFunc(display);
   glutReshapeFunc(reshape);
   glutSpecialFunc(special);
   glutKeyboardFunc(key);
   glutIdleFunc(idle);
   texture[0] = LoadTexBMP("backwall.bmp");
   texture[1] = LoadTexBMP("building1.bmp");
   texture[2] = LoadTexBMP("building2.bmp");
   texture[3] = LoadTexBMP("building3.bmp");
   texture[4] = LoadTexBMP("roof.bmp");
   texture[5] = LoadTexBMP("chimney.bmp");
   texture[6] = LoadTexBMP("grass.bmp");
   texture[7] = LoadTexBMP("buildingside.bmp");
   texture[8] = LoadTexBMP("road.bmp");
   texture[9] = LoadTexBMP("extrawall.bmp");
   texture[10] = LoadTexBMP("flatroof.bmp");
   texture[11] = LoadTexBMP("flatroof2.bmp");
   texture[12] = LoadTexBMP("extrawall.bmp");
   texture[13] = LoadTexBMP("de_dust_ground_alternative.bmp");
   texture[14] = LoadTexBMP("ground.bmp");
   texture[15] = LoadTexBMP("door.bmp");
   texture[16] = LoadTexBMP("sidedoor.bmp");
   texture[17] = LoadTexBMP("belltowerwall.bmp");
   texture[18] = LoadTexBMP("wall1.bmp");
   texture[19] = LoadTexBMP("wall2.bmp");
   texture[20] = LoadTexBMP("wall3.bmp");
   texture[21] = LoadTexBMP("wall4.bmp");
   texture[22] = LoadTexBMP("wall5.bmp");
   texture[23] = LoadTexBMP("roof1.bmp");
   texture[24] = LoadTexBMP("roof2.bmp");
   texture[25] = LoadTexBMP("roof3.bmp");
   texture[26] = LoadTexBMP("sky0.bmp");
   texture[27] = LoadTexBMP("sky1.bmp");
   texture[28] = LoadTexBMP("lamppost.bmp");
   texture[29] = LoadTexBMP("cartbottom.bmp");
   texture[30] = LoadTexBMP("cartfrontandback.bmp");
   texture[31] = LoadTexBMP("cartsides.bmp");
   texture[31] = LoadTexBMP("cartwheel.bmp");
   texture[32] = LoadTexBMP("greyscale.bmp");
   texture[33] = LoadTexBMP("brownscale.bmp");
   //Load Object
   obj = LoadOBJ("bellobject.obj");
   //  Pass control to GLUT so it can interact with the user
   ErrCheck("init");
   glutMainLoop();
   return 0;
}
