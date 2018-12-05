#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <math.h>
#define GL_GLEXT_PROTOTYPES
#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif
#include "CSCIx229.h"

int th=0;         //  Azimuth of view angle
int ph=25;         //  Elevation of view angle
int zh = 90;
int axes=0;       //  Display axes
double dim=20;     // Size of scene
double asp=1;     // Aspect ratio
int fov = 55;     // Field of view
int fp = 0;
int mouse = 0;
int kirbyObj = 0;
int pikaObj = 0;
int yoshiObj = 0;
int distance = 20;
double rotation = 0.0;
float kirby_y  =   0;  // Elevation of light

// First person view
double fpx = 0.0;
double fpz = 0.0;
double fpy = 0.0;
double epx = 0.0;
double epy = 0.0;
double epz = 5.0;

// Textures
unsigned int texture[8]; // Texture names
int sky[1];   //  Sky textures

// Mouse
// Camera position
float camera_x = 0.0, camera_z = 30; // initially 5 units south of origin

// Camera direction
float lx = 0, lz = -1; // camera points initially along y-axis
float angle = 0.0; // angle of rotation for the camera direction
float deltaAngle = 0.0; // additional angle change when dragging

// Mouse drag control
int isDragging = 0; // true when dragging
int xDragStart = 0; // records the x-coordinate when dragging starts

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

static void Vertex(double th,double ph)
{

   double x = Sin(th)*Cos(ph);
   double y = Cos(th)*Cos(ph);
   double z =         Sin(ph);
   glNormal3d(x,y,z);
   glTexCoord2d(th/360.0,ph/180.0+0.5);
   glVertex3d(x,y,z);

}

static void platform(double x, double y, double z)
{

  int th;
  const int d = 5;
  float pi = 3.141592654f;
  float numberOfSegments = 64;
  float angleIncrement = (2.0f * pi) / numberOfSegments;
  float textureCoordinateIncrement = 1.0 / 32;

  // Bottom
  glPushMatrix();
  glTranslated(x, y, z);
  glScaled(1, 3, .5);
  glEnable(GL_TEXTURE_2D);
  glBindTexture(GL_TEXTURE_2D,texture[3]);

  glBegin(GL_QUAD_STRIP);
  for (unsigned int i = 0; i <= 32; ++i)
  {
      float c = cos(angleIncrement * i);
      float s = sin(angleIncrement * i);
      glTexCoord2f( textureCoordinateIncrement * i, 0); glVertex3f( c, 0, s);
      glTexCoord2f( textureCoordinateIncrement * i, 1.0f); glVertex3f( c, .5, s);
  }
  glEnd();
  glPopMatrix();

  glPushMatrix();
  glTranslated(x, y - 1.5, z);
  glScaled(1, 3, .5);
  glBegin(GL_QUAD_STRIP);
  for (unsigned int i = 0; i <= 32; ++i) {
    float c = cos(angleIncrement * i);
    float s = sin(angleIncrement * i);
    glTexCoord2f( textureCoordinateIncrement * i, 1.0f); glVertex3f( c, 0, s);
    glTexCoord2f( textureCoordinateIncrement * i, 0); glVertex3f( c, .5, s);
  }
  glEnd();
  glPopMatrix();

  // Top
  glPushMatrix();
  glTranslated(x , y - 1.5, z - .5);
  glScaled(1, 3, .5);
  glBindTexture(GL_TEXTURE_2D,texture[2]);
  glBegin(GL_QUADS);
  glTexCoord2d(1, 0); glVertex3f(-1,0, 1);
  glTexCoord2d(1, 1); glVertex3f(+1,0, 1);
  glTexCoord2d(0.5,1); glVertex3f(+1,+1, 1);
  glTexCoord2d(0.5,0); glVertex3f(-1,+1, 1);
  glEnd();
  glPopMatrix();

  // Sides
  glPushMatrix();
  glTranslated(x, y + 1.5, z);
  glScaled(1,1,.5);
  glBegin(GL_TRIANGLE_FAN);
  glVertex3f(0,0,0);
  for(th = 0; th <= 180; th+= d) {
    glTexCoord2d(0.5, 1); glVertex3f(Cos(th), 0, Sin(th));
  }
  glEnd();
  glPopMatrix();

  // For some reason I need to render this twice for the sides to work
  glPushMatrix();
  glTranslated(x , y - 1.5, z - .5);
  glScaled(1, 3, .5);
  glBindTexture(GL_TEXTURE_2D,texture[2]);
  glBegin(GL_QUADS);
  glTexCoord2d(1, 0); glVertex3f(-1,0, 1);
  glTexCoord2d(1, 1); glVertex3f(+1,0, 1);
  glTexCoord2d(0.5,1); glVertex3f(+1,+1, 1);
  glTexCoord2d(0.5,0); glVertex3f(-1,+1, 1);
  glEnd();
  glPopMatrix();

  // Sides
  glPushMatrix();
  glTranslated(x, y - 1.5, z);
  glScaled(1,1,.5);
  glBegin(GL_TRIANGLE_FAN);
  glVertex3f(0,0,0);
  for(th = 0; th <= 180; th+= d) {
    glTexCoord2d(0.5, 1); glVertex3f(Cos(th), 0, Sin(th));
  }
  glEnd();
  glPopMatrix();
  glDisable(GL_TEXTURE_2D);

}

static void stage(double x, double y, double z)
{
  // Top
  glPushMatrix();
  glTranslated(x, y + 3, z);
  glScaled(2,2,2);
  glEnable(GL_TEXTURE_2D);
  glBindTexture(GL_TEXTURE_2D,texture[5]);
  glBegin(GL_QUADS);
  glTexCoord2d(0, 1); glVertex3f(3, 0,  -.5);
  glTexCoord2d(0, 0); glVertex3f(3, 0, -2);
  glTexCoord2d(1,0); glVertex3f(-2, 0, -2.2);
  glTexCoord2d(1,1); glVertex3f(-2, 0, -.5);
  glEnd();
  glPopMatrix();

  /* From arrow up
  Starting at index 1
  Starting at point front left
  Numbering rotating to the right

  glBegin(GL_POLYGON);
  glVertex3f(-1,  0, .5); // 1
  glVertex3f(2, 0,  .5); // 2
  glVertex3f(3, 0,  -.5); // 3
  glVertex3f(3, 0, -2);  // 4
  glVertex3f(2, 0, -3);  // 5
  glVertex3f(-1, 0, -3);  // 6
  glVertex3f(-2, 0, -2.2); // 7
  glVertex3f(-2, 0, -.5); // 8
  glEnd();
  glPopMatrix(); */

  // Front Right Trap
  glPushMatrix();
  glTranslated(x, y + 3, z);
  glScaled(2,2,2);
  glBindTexture(GL_TEXTURE_2D,texture[4]);
  glBegin(GL_TRIANGLES);
  glTexCoord2f(0, 0); glVertex3f(2, 0,  .5); // 2
  glTexCoord2f(1, 0); glVertex3f(3, 0,  -.5); // 3
  glTexCoord2f(0.5, 1); glVertex3f(2, 0,  -.5);
  glEnd();
  glPopMatrix();

  // Front Left Trap
  glPushMatrix();
  glTranslated(x, y + 3, z);
  glScaled(2,2,2);
  glBegin(GL_TRIANGLES);
  glTexCoord2f(0, 0); glVertex3f(-1,  0, .5); // 1
  glTexCoord2f(1, 0); glVertex3f(-2, 0, -.5); // 8
  glTexCoord2f(0.5, 1); glVertex3f(-1, 0,  -.5);
  glEnd();
  glPopMatrix();

  // Front Rec
  glPushMatrix();
  glTranslated(x, y + 3, z);
  glScaled(2,2,2);
  glBegin(GL_POLYGON);
  glTexCoord2f(0,0); glVertex3f(-1,  0, .5); // 1
  glTexCoord2f(1,0); glVertex3f(2, 0,  .5); // 2
  glTexCoord2f(1,1); glVertex3f(2, 0,  -.5);
  glTexCoord2f(0,1); glVertex3f(-1, 0,  -.5);
  glEnd();
  glPopMatrix();

  // Back Right Trap
  glPushMatrix();
  glTranslated(x,y + 3,z);
  glScaled(2,2,2);
  glBegin(GL_TRIANGLES);
  glTexCoord2f(0,0); glVertex3f(3, 0, -2);  // 4
  glTexCoord2f(1,0); glVertex3f(2, 0, -3);  // 5
  glTexCoord2f(0.5,1); glVertex3f(2, 0, -2);
  glEnd();
  glPopMatrix();

  // Back Left Trap
  glPushMatrix();
  glTranslated(x,y + 3,z);
  glScaled(2,2,2);
  glBegin(GL_TRIANGLES);
  glTexCoord2f(0,0); glVertex3f(-2, 0, -2.2); // 7
  glTexCoord2f(1,0); glVertex3f(-1, 0, -3);  // 5
  glTexCoord2f(0.5,1); glVertex3f(-1, 0, -2);
  glEnd();
  glPopMatrix();

  // Back
  glPushMatrix();
  glTranslated(x, y + 3, z);
  glScaled(2,2,2);
  glBegin(GL_POLYGON);
  glTexCoord2f(0,0); glVertex3f(-1, 0, -3);  // 6
  glTexCoord2f(1,0); glVertex3f(2, 0, -3); //  5
  glTexCoord2f(1,1); glVertex3f(2, 0,  -2);
  glTexCoord2f(0,1); glVertex3f(-1, 0,  -2);
  glEnd();
  glPopMatrix();

  // Grass side Rectangles
  glPushMatrix();
  glTranslated(x, y + 3, z);
  glScaled(2, 2, 2);
  glBindTexture(GL_TEXTURE_2D,texture[0]);
  glBegin(GL_QUADS);

  // Front
  glTexCoord2d(0,1); glVertex3f(-1, 0, .5);
  glTexCoord2d(1,1); glVertex3f(0.5, 0, .5);
  glTexCoord2d(1,0); glVertex3f(0.5, -.5, .5);
  glTexCoord2d(0,0); glVertex3f(-1, -.5, .5);

  glTexCoord2d(0,1); glVertex3f(0.5, 0, .5);
  glTexCoord2d(1,1); glVertex3f(2, 0, .5);
  glTexCoord2d(1,0); glVertex3f(2, -.5, .5);
  glTexCoord2d(0,0); glVertex3f(0.5, -.5, .5);

  // Back
  glTexCoord2d(0,1); glVertex3f(-1, 0, -3);
  glTexCoord2d(1,1); glVertex3f(0.5, 0, -3);
  glTexCoord2d(1,0); glVertex3f(0.5, -.5, -3);
  glTexCoord2d(0,0); glVertex3f(-1, -.5, -3);

  glTexCoord2d(0,1); glVertex3f(0.5, 0, -3);
  glTexCoord2d(1,1); glVertex3f(2, 0, -3);
  glTexCoord2d(1,0); glVertex3f(2, -.5, -3);
  glTexCoord2d(0,0); glVertex3f(0.5, -.5, -3);

  // Front sides
  glTexCoord2d(0,1); glVertex3f(2, 0, .5);
  glTexCoord2d(1,1); glVertex3f(3, 0, -.5);
  glTexCoord2d(1,0); glVertex3f(3, -.5, -.5);
  glTexCoord2d(0,0); glVertex3f(2, -.5, .5);

  glTexCoord2d(0,1); glVertex3f(-1, 0, .5);
  glTexCoord2d(1,1); glVertex3f(-2, 0, -.5);
  glTexCoord2d(1,0); glVertex3f(-2, -.5, -.5);
  glTexCoord2d(0,0); glVertex3f(-1, -.5, .5);

  // Ledges
  glTexCoord2d(0,1); glVertex3f(3, 0, -.5);
  glTexCoord2d(1,1); glVertex3f(3, 0, -2);
  glTexCoord2d(1,0); glVertex3f(3, -.5, -2);
  glTexCoord2d(0,0); glVertex3f(3, -.5, -.5);

  glTexCoord2d(0,1); glVertex3f(-2, 0, -.5);
  glTexCoord2d(1,1); glVertex3f(-2, 0, -2.2);
  glTexCoord2d(1,0); glVertex3f(-2, -.5, -2.2);
  glTexCoord2d(0,0); glVertex3f(-2, -.5, -.5);

  // Back sides
  glTexCoord2d(0,1); glVertex3f(3, 0, -2);
  glTexCoord2d(1,1); glVertex3f(2, 0, -3);
  glTexCoord2d(1,0); glVertex3f(2, -.5, -3);
  glTexCoord2d(0,0); glVertex3f(3, -.5, -2);

  glTexCoord2d(0,1); glVertex3f(-2, 0, -2.2);
  glTexCoord2d(1,1); glVertex3f(-1, 0, -3);
  glTexCoord2d(1,0); glVertex3f(-1, -.5, -3);
  glTexCoord2d(0,0); glVertex3f(-2, -.5, -2.2);
  glEnd();
  glPopMatrix();

  // Bottom of Stage
  glPushMatrix();
  glTranslated(x, y, z);
  glScaled(2, 2, 2);
  glBindTexture(GL_TEXTURE_2D,texture[1]);
  glBegin(GL_QUAD_STRIP);
  glColor3f(0.96,.81,.6); // rbg divided by 255

  // Front
  glTexCoord2f(0,0.2); glVertex3f(0, 0, 0);
  glTexCoord2f(0,1); glVertex3f(0, 1, .5);
  glTexCoord2f(1, 0.2); glVertex3f(1, 0, 0);
  glTexCoord2f(1,1); glVertex3f(1, 1, 0.5);

  glTexCoord2f(0, 0.2); glVertex3f(2, 0, 0);
  glTexCoord2f(0,1); glVertex3f(2, 1, 0.5);

  // Right front
  glTexCoord2f(1, 0.2); glVertex3f(2.5, 0, -0.5);
  glTexCoord2f(1,1); glVertex3f(2.5, 1, 0);

  glTexCoord2f(0, 0.2); glVertex3f(3, 0, -1);
  glTexCoord2f(0,1); glVertex3f(3, 1, -0.5);

  // Right Ledge
  glTexCoord2f(1, 0.2); glVertex3f(3, 0, -1.5);
  glTexCoord2f(1, 1); glVertex3f(3, 1, -1.5);

  glTexCoord2f(0, 0.2); glVertex3f(3, 0, -2);
  glTexCoord2f(0,1); glVertex3f(3, 1, -2);

  // Right back
  glTexCoord2f(1, 0.2); glVertex3f(2, 0, -2.8);
  glTexCoord2f(1, 1); glVertex3f(2, 1, -3);

  // Back
  glTexCoord2f(0, 0.2); glVertex3f(1, 0, -2.8);
  glTexCoord2f(0, 1); glVertex3f(1, 1, -3);

  glTexCoord2f(1, 0.2); glVertex3f(0, 0, -2.8);
  glTexCoord2f(1, 1); glVertex3f(0, 1, -3);

  glTexCoord2f(0, 0.2); glVertex3f(-1, 0, -2.8);
  glTexCoord2f(0, 1); glVertex3f(-1, 1, -3);

  // Left Back
  glTexCoord2f(1, 0.2); glVertex3f(-2, 0, -2);
  glTexCoord2f(1, 1); glVertex3f(-2, 1, -2.2);

  // Left Ledge
  glTexCoord2f(0, 0.2); glVertex3f(-1.9, 0, -.5);
  glTexCoord2f(0, 1); glVertex3f(-2, 1, -.5);

  // Left Front
  glTexCoord2f(1, 0.2); glVertex3f(-1, 0, 0);
  glTexCoord2f(1, 1); glVertex3f(-1, 1, .5);

  // Completion
  glTexCoord2f(0,0.2); glVertex3f(0, 0, 0);
  glTexCoord2f(0,1); glVertex3f(0, 1, .5);
  glEnd();
  glPopMatrix();

  // Spike at bottom: Front
  glPushMatrix();
  glTranslated(x, y, z);
  glScaled(2,2,2);
  glBegin(GL_TRIANGLES);
  glTexCoord2f(0.1, 0.2); glVertex3f(0,0,0);
  glTexCoord2f(0.9, 0.2); glVertex3f(1,0, 0);
  glTexCoord2f(0.5, 0.1); glVertex3f(0.5,-.5, -0.3);
  glEnd();
  glPopMatrix();

  glPushMatrix();
  glTranslated(x + 2, y, z);
  glScaled(2,2,2);
  glBegin(GL_TRIANGLES);
  glTexCoord2f(0.1, 0.2); glVertex3f(0,0,0);
  glTexCoord2f(0.9, 0.2); glVertex3f(1,0, 0);
  glTexCoord2f(0.5, 0.1); glVertex3f(0.5,-.5, -0.3);
  glEnd();
  glPopMatrix();

  glPushMatrix();
  glTranslated(x - 2, y, z);
  glScaled(2,2,2);
  glBegin(GL_TRIANGLES);
  glTexCoord2f(0.1, 0.2); glVertex3f(0,0,0);
  glTexCoord2f(0.9, 0.2); glVertex3f(1,0, 0);
  glTexCoord2f(0.5, 0.1); glVertex3f(0.5,-.5, -0.3);
  glEnd();
  glPopMatrix();

  // Right Front
  glPushMatrix();
  glTranslated(x, y, z);
  glScaled(2,2,2);
  glBegin(GL_TRIANGLES);
  glTexCoord2f(0.1, 0.2); glVertex3f(2,0,0);
  glTexCoord2f(0.9, 0.2); glVertex3f(2.5,0, -.5);
  glTexCoord2f(0.5, 0.1); glVertex3f(2,-.5, -.5);
  glEnd();
  glPopMatrix();

  glPushMatrix();
  glTranslated(x, y, z);
  glScaled(2,2,2);
  glBegin(GL_TRIANGLES);
  glTexCoord2f(0.1, 0.2); glVertex3f(2.5,0, -.5);
  glTexCoord2f(0.9, 0.2); glVertex3f(3,0, -1);
  glTexCoord2f(0.5, 0.1); glVertex3f(2.5,-.5, -1);
  glEnd();
  glPopMatrix();

  // Right Ledge
  glPushMatrix();
  glTranslated(x, y, z);
  glScaled(2,2,2);
  glBegin(GL_TRIANGLES);
  glTexCoord2f(0.1, 0.2); glVertex3f(3,0, -1);
  glTexCoord2f(0.9, 0.2); glVertex3f(3,0, -1.5);
  glTexCoord2f(0.5, 0.1); glVertex3f(2.7,-.5, -1.3);
  glEnd();
  glPopMatrix();

  glPushMatrix();
  glTranslated(x, y, z);
  glScaled(2,2,2);
  glBegin(GL_TRIANGLES);
  glTexCoord2f(0.1, 0.2); glVertex3f(3,0, -1.5);
  glTexCoord2f(0.9, 0.2); glVertex3f(3,0, -2);
  glTexCoord2f(0.5, 0.1); glVertex3f(2.7,-.5, -1.7);
  glEnd();
  glPopMatrix();

  // Right Back
  glPushMatrix();
  glTranslated(x, y, z);
  glScaled(2,2,2);
  glBegin(GL_TRIANGLES);
  glTexCoord2f(0.1, 0.2); glVertex3f(3,0, -2);
  glTexCoord2f(0.9, 0.2); glVertex3f(2,0, -2.8);
  glTexCoord2f(0.5, 0.1); glVertex3f(2.3,-.5, -2.2);
  glEnd();
  glPopMatrix();

  // Back
  glPushMatrix();
  glTranslated(x, y, z);
  glScaled(2,2,2);
  glBegin(GL_TRIANGLES);
  glTexCoord2f(0.1, 0.2); glVertex3f(2,0, -2.8);
  glTexCoord2f(0.9, 0.2); glVertex3f(1,0, -2.8);
  glTexCoord2f(0.5, 0.1); glVertex3f(1.5, -.5, -2.5);
  glEnd();
  glPopMatrix();

  glPushMatrix();
  glTranslated(x, y, z);
  glScaled(2,2,2);
  glBegin(GL_TRIANGLES);
  glTexCoord2f(0.1, 0.2); glVertex3f(1,0, -2.8);
  glTexCoord2f(0.9, 0.2); glVertex3f(0,0, -2.8);
  glTexCoord2f(0.5, 0.1); glVertex3f(0.5, -.5, -2.5);
  glEnd();
  glPopMatrix();

  glPushMatrix();
  glTranslated(x, y, z);
  glScaled(2,2,2);
  glBegin(GL_TRIANGLES);
  glTexCoord2f(0.1, 0.2); glVertex3f(0,0, -2.8);
  glTexCoord2f(0.9, 0.2); glVertex3f(-1,0, -2.8);
  glTexCoord2f(0.5, 0.1); glVertex3f(-.5, -.5, -2.5);
  glEnd();
  glPopMatrix();

  // Left Back
  glPushMatrix();
  glTranslated(x, y, z);
  glScaled(2,2,2);
  glBegin(GL_TRIANGLES);
  glTexCoord2f(0.1, 0.2); glVertex3f(-1,0, -2.8);
  glTexCoord2f(0.9, 0.2); glVertex3f(-2,0, -2);
  glTexCoord2f(0.5, 0.1); glVertex3f(-1.3, -.5, -2.3);
  glEnd();
  glPopMatrix();

  // Left Ledge
  glPushMatrix();
  glTranslated(x, y, z);
  glScaled(2,2,2);
  glBegin(GL_TRIANGLES);
  glTexCoord2f(0.1, 0.2); glVertex3f(-2,0, -2);
  glTexCoord2f(0.9, 0.2); glVertex3f(-1.9,0, -.5);
  glTexCoord2f(0.5, 0.1); glVertex3f(-1.7, -.5, -1.2);
  glEnd();
  glPopMatrix();

  //Left front
  glPushMatrix();
  glTranslated(x, y, z);
  glScaled(2,2,2);
  glBegin(GL_TRIANGLES);
  glTexCoord2f(0.1, 0.2); glVertex3f(-1.9,0, -.5);
  glTexCoord2f(0.9, 0.2); glVertex3f(-1,0, 0);
  glTexCoord2f(0.5, 0.1); glVertex3f(-1.4, -.5, -.5);
  glEnd();
  glPopMatrix();
  glDisable(GL_TEXTURE_2D);

}

static void whispy(double x, double y, double z)
{

  int th, ph;
  const int d = 5;
  float pi = 3.141592654f;
  float numberOfSegments = 64;
  float angleIncrement = (2.0f * pi) / numberOfSegments;
  float textureCoordinateIncrement = 1.0 / numberOfSegments;

  // Trunk
  glPushMatrix();
  glTranslated(x, y, z);
  glScaled(2, 10, 2);
  glEnable(GL_TEXTURE_2D);
  glBindTexture(GL_TEXTURE_2D,texture[6]);
  glBegin(GL_QUAD_STRIP);
  for (unsigned int i = 0; i <= numberOfSegments; ++i) {
      float c = cos(angleIncrement * i);
      float s = sin(angleIncrement * i);
      glTexCoord2f( textureCoordinateIncrement * i, 0); glVertex3f( c, 0, s);
      glTexCoord2f( textureCoordinateIncrement * i, 1.0f); glVertex3f( c, .5, s);
  }
  glEnd();
  glPopMatrix();

  // Roots
  glPushMatrix();
  glTranslated(x + 1.9, y, z);
  glRotatef(90, 0, 0,1);
  glRotatef(90, 0, 1,0);
  glScaled(0.5,.5,.5);
  glBegin(GL_TRIANGLES);
  for (th = 0; th <= 180; th += 2 * d) {
    glTexCoord2d((th-d)/360.0, 0);
    glVertex3d(Cos(th-d), 0, Sin(th-d));
    glTexCoord2d(th/360.0, 1);
    glVertex3d(0, -5, 0);
    glTexCoord2d((th+d)/360.0, 0);
    glVertex3d(Cos(th+d), 0, Sin(th+d));
  }
  glEnd();
  glPopMatrix();

  glPushMatrix();
  glTranslated(x + .5,y, z+ 1);
  glRotatef(90, 0, 0,1);
  glRotatef(-50, 1, 0, 0);
  glScaled(0.5,.5,.5);
  glBegin(GL_TRIANGLES);
  for (th = 0; th <= 360; th += 2 * d) {
    glTexCoord2d((th-d)/360.0, 0);
    glVertex3d(Cos(th-d), 0, Sin(th-d));
    glTexCoord2d(th/360.0, 1);
    glVertex3d(0, -5, 0);
    glTexCoord2d((th+d)/360.0, 0);
    glVertex3d(Cos(th+d), 0, Sin(th+d));
  }
  glEnd();
  glPopMatrix();

  glPushMatrix();
  glTranslated(x - 1.5,y, z - .5);
  glRotatef(90, 0, 0,1);
  glRotatef(-50, 1, 0, 0);
  glRotatef(-150, 1, 0, 0);
  glScaled(0.5,.5,.5);
  glBegin(GL_TRIANGLES);
  for (th = 0; th <= 360; th += 2 * d) {
    glTexCoord2d((th-d)/360.0, 0);
    glVertex3d(Cos(th-d), 0, Sin(th-d));
    glNormal3d(Cos(th), 1, Sin(th));
    glTexCoord2d(th/360.0, 1);
    glVertex3d(0, -5, 0);
    glNormal3d(Cos(th+d), 1, Sin(th+d));
    glTexCoord2d((th+d)/360.0, 0);
    glVertex3d(Cos(th+d), 0, Sin(th+d));
  }
  glEnd();
  glPopMatrix();

  // Nose
  glPushMatrix();
  glTranslated(x + .8,y + 2.5, z + 1.1);
  glRotatef(90, 0, 0,1);
  glRotatef(-50, 1, 0, 0);
  glScaled(0.5, .3 , .5);
  glBegin(GL_TRIANGLES);
  for (th = 0; th <= 360; th += 2 * d) {
    glNormal3d(Cos(th-d), 1, Sin(th-d));
    glTexCoord2d((th-d)/360.0, 0);
    glVertex3d(Cos(th-d), 0, Sin(th-d));
    glNormal3d(Cos(th), 1, Sin(th));
    glTexCoord2d(th/360.0, 1);
    glVertex3d(0, -5, 0);
    glNormal3d(Cos(th+d), 1, Sin(th+d));
    glTexCoord2d((th+d)/360.0, 0);
    glVertex3d(Cos(th+d), 0, Sin(th+d));
  }
  glEnd();
  glPopMatrix();

  // Sphere leaves
  glPushMatrix();
  glTranslated(x + 1.5, y + 5.5, z);
  glScaled(1.7,1,1.7);
  glBindTexture(GL_TEXTURE_2D,texture[7]);
  for (ph=-90;ph<90;ph+=5) {
    glBegin(GL_QUAD_STRIP);
    for (th=0;th<=360;th+=5) {
      Vertex(th,ph);
      Vertex(th,ph+5);
    }
    glEnd();
  }
  glPopMatrix();

  glPushMatrix();
  glTranslated(x + 1, y + 5.5, z - 1);
  glScaled(1.7,1,1.7);
  glBindTexture(GL_TEXTURE_2D,texture[7]);
  for (ph=-90;ph<90;ph+=5) {
    glBegin(GL_QUAD_STRIP);
    for (th=0;th<=360;th+=5) {
      Vertex(th,ph);
      Vertex(th,ph+5);
    }
    glEnd();
  }
  glPopMatrix();

  glPushMatrix();
  glTranslated(x - 1, y + 5.5, z - .5);
  glScaled(1.7,1,1.7);
  glBindTexture(GL_TEXTURE_2D,texture[7]);
  for (ph=-90;ph<90;ph+=5) {
    glBegin(GL_QUAD_STRIP);
    for (th=0;th<=360;th+=5) {
      Vertex(th,ph);
      Vertex(th,ph+5);
    }
    glEnd();
  }
  glPopMatrix();

  glPushMatrix();
  glTranslated(x - .5, y + 5.5, z + .7);
  glScaled(1.7,1,1.7);
  glBindTexture(GL_TEXTURE_2D,texture[7]);
  for (ph=-90;ph<90;ph+=5) {
    glBegin(GL_QUAD_STRIP);
    for (th=0;th<=360;th+=5) {
      Vertex(th,ph);
      Vertex(th,ph+5);
    }
    glEnd();
  }
  glPopMatrix();

  glPushMatrix();
  glTranslated(x - 2, y + 5.5, z + .7);
  glScaled(1.7,1,1.7);
  glBindTexture(GL_TEXTURE_2D,texture[7]);
  for (ph=-90;ph<90;ph+=5) {
    glBegin(GL_QUAD_STRIP);
    for (th=0;th<=360;th+=5) {
      Vertex(th,ph);
      Vertex(th,ph+5);
    }
    glEnd();
  }
  glPopMatrix();

  glPushMatrix();
  glTranslated(x + 2, y + 5.5, z + .7);
  glScaled(1.7,1,1.7);
  glBindTexture(GL_TEXTURE_2D,texture[7]);
  for (ph=-90;ph<90;ph+=5) {
    glBegin(GL_QUAD_STRIP);
    for (th=0;th<=360;th+=5) {
      Vertex(th,ph);
      Vertex(th,ph+5);
    }
    glEnd();
  }
  glPopMatrix();

  glPushMatrix();
  glTranslated(x, y + 5.5, z - 1.5);
  glScaled(1.7,1,1.7);
  glBindTexture(GL_TEXTURE_2D,texture[7]);
  for (ph=-90;ph<90;ph+=5) {
    glBegin(GL_QUAD_STRIP);
    for (th=0;th<=360;th+=5) {
      Vertex(th,ph);
      Vertex(th,ph+5);
    }
    glEnd();
  }
  glPopMatrix();

  glPushMatrix();
  glTranslated(x - 1.5, y + 6.5, z);
  glScaled(1.7,1,1.7);
  glBindTexture(GL_TEXTURE_2D,texture[7]);
  for (ph=-90;ph<90;ph+=5) {
    glBegin(GL_QUAD_STRIP);
    for (th=0;th<=360;th+=5) {
      Vertex(th,ph);
      Vertex(th,ph+5);
    }
    glEnd();
  }
  glPopMatrix();

  glPushMatrix();
  glTranslated(x + 2, y + 6.5, z);
  glScaled(1.7,1,1.7);
  glBindTexture(GL_TEXTURE_2D,texture[7]);
  for (ph=-90;ph<90;ph+=5) {
    glBegin(GL_QUAD_STRIP);
    for (th=0;th<=360;th+=5) {
      Vertex(th,ph);
      Vertex(th,ph+5);
    }
    glEnd();
  }
  glPopMatrix();

  glPushMatrix();
  glTranslated(x, y + 6.5, z);
  glScaled(1.7,1,1.7);
  glBindTexture(GL_TEXTURE_2D,texture[7]);
  for (ph=-90;ph<90;ph+=5) {
    glBegin(GL_QUAD_STRIP);
    for (th=0;th<=360;th+=5) {
      Vertex(th,ph);
      Vertex(th,ph+5);
    }
    glEnd();
  }
  glPopMatrix();

  glPushMatrix();
  glTranslated(x, y + 7, z);
  glScaled(1.7,1,1.7);
  glBindTexture(GL_TEXTURE_2D,texture[7]);
  for (ph=-90;ph<90;ph+=5) {
    glBegin(GL_QUAD_STRIP);
    for (th=0;th<=360;th+=5) {
      Vertex(th,ph);
      Vertex(th,ph+5);
    }
    glEnd();
  }
  glPopMatrix();
  glDisable(GL_TEXTURE_2D);

  // Eye
  glPushMatrix();
  glTranslated(x + .5,y + 3.5,z + 2);
  glRotatef(90, 1,0,0);
  glRotatef(-10, 0,0,1);
  glScaled(0.5, 2, 2);
  glColor3f(0,0,0);
  glVertex3f(0,0,0);
  glBegin(GL_TRIANGLE_FAN);
  for(th = 0; th <= 360; th+= d) {
    glTexCoord2d(0.5, 1); glVertex3f(Cos(th), 0, Sin(th)*0.4);
  }
  glEnd();
  glPopMatrix();

  glPushMatrix();
  glTranslated(x + 1.8,y + 3.5,z + 1);
  glRotatef(90, 1,0,0);
  glRotatef(-50, 0,0,1);
  glScaled(0.5, 2, 2);
  glColor3f(0,0,0);
  glVertex3f(0,0,0);
  glBegin(GL_TRIANGLE_FAN);
  for(th = 0; th <= 360; th+= d) {
    glTexCoord2d(0.5, 1); glVertex3f(Cos(th), 0, Sin(th)*0.4);
  }
  glEnd();
  glPopMatrix();

  glPushMatrix();
  glTranslated(x + 1.3,y + 1.5, z + 1.6);
  glRotatef(90, 1,0,0);
  glRotatef(-50, 0,0,1);
  glScaled(0.3, 2, 2);
  glColor3f(0,0,0);
  glVertex3f(0,0,0);
  glBegin(GL_TRIANGLE_FAN);
  for(th = 0; th <= 360; th+= d) {
    glTexCoord2d(0.5, 1); glVertex3f(Cos(th), 0, Sin(th)*0.2);
  }
  glEnd();
  glPopMatrix();

}


static void Sky(double D)
{

   glColor3f(1,1,1);
   glEnable(GL_TEXTURE_2D);

   //  Sides
   glBindTexture(GL_TEXTURE_2D,sky[0]);
   glBegin(GL_QUADS);
   glTexCoord2f(0,0); glVertex3f(-D,-D,-D);
   glTexCoord2f(1,0); glVertex3f(+D,-D,-D);
   glTexCoord2f(1,1); glVertex3f(+D,+D,-D);
   glTexCoord2f(0,1); glVertex3f(-D,+D,-D);

   glTexCoord2f(0,.7); glVertex3f(+D,-D,-D);
   glTexCoord2f(0.3,.7); glVertex3f(+D,-D,+D);
   glTexCoord2f(0.3,1); glVertex3f(+D,+D,+D);
   glTexCoord2f(0,1); glVertex3f(+D,+D,-D);

   glTexCoord2f(0,0.7); glVertex3f(+D,-D,+D);
   glTexCoord2f(0.3,0.7); glVertex3f(-D,-D,+D);
   glTexCoord2f(0.3,1); glVertex3f(-D,+D,+D);
   glTexCoord2f(0,1); glVertex3f(+D,+D,+D);

   glTexCoord2f(0,0.7); glVertex3f(-D,-D,+D);
   glTexCoord2f(0.3,0.7); glVertex3f(-D,-D,-D);
   glTexCoord2f(0.3,1); glVertex3f(-D,+D,-D);
   glTexCoord2f(0,1); glVertex3f(-D,+D,+D);
   glEnd();

   //  Top and bottom
   glBegin(GL_QUADS);
   glTexCoord2f(0,0.7); glVertex3f(+D,+D,-D);
   glTexCoord2f(0.3,0.7); glVertex3f(+D,+D,+D);
   glTexCoord2f(0.3,1); glVertex3f(-D,+D,+D);
   glTexCoord2f(0,1); glVertex3f(-D,+D,-D);

   glTexCoord2f(0,0.7); glVertex3f(-D,-D,+D);
   glTexCoord2f(0.3,0.7); glVertex3f(+D,-D,+D);
   glTexCoord2f(0.3,1); glVertex3f(+D,-D,-D);
   glTexCoord2f(0,1); glVertex3f(-D,-D,-D);
   glEnd();
   glDisable(GL_TEXTURE_2D);

}

void update()
{
   //  Elapsed time in seconds
   double t = glutGet(GLUT_ELAPSED_TIME)/1000.0;
   zh = fmod(90*t,360.0);
   glutPostRedisplay();

}


static void kirby(double x, double y, double z, double r)
{

  glPushMatrix();
  glTranslated(x + 2, y + 6, z - 2);
  glRotatef(-zh, 0, 1, 0);
  glScaled(7,7,7);
  glCallList(kirbyObj);
  glPopMatrix();

}

static void pika(double x, double y, double z)
{

  glPushMatrix();
  glTranslated(x - 3, y + 10, z -4);
  glRotatef(90, 0 ,1, 0);
  glScaled(7,7,7);
  glCallList(pikaObj);
  glPopMatrix();

}

static void yoshi(double x, double y, double z)
{

  glPushMatrix();
  glTranslated(x + 7, y + 10, z -4);
  glRotatef(-90, 0 ,1, 0);
  glScaled(6,6,6);
  glCallList(yoshiObj);
  glPopMatrix();


}

void display()
{
   const double len=1.5;  //  Length of axes
   //  Erase the window and the depth buffer
   glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
   //  Enable Z-buffering in OpenGL
   glEnable(GL_DEPTH_TEST);
   //glEnable(GL_CULL_FACE);
   //  Undo previous transformations
   glLoadIdentity();

   if(mouse) {
     gluLookAt(
  			camera_x,      8,      camera_z,
  			camera_x + lx, 8, camera_z + lz,
  			0.0,    1,    0);
   }

   if(fp) {
     fpx = 2 * dim * Sin(rotation);
     fpz = 2 * dim * -Cos(rotation);
     gluLookAt(epx, epy + fpy, epz, fpx + epx, epy, fpz + epz, 0, 1, 0);
   }
   if(fp == 0 && mouse ==0) {
     double Ex = -2*dim*Sin(th)*Cos(ph);
     double Ey = +2*dim        *Sin(ph);
     double Ez = +2*dim*Cos(th)*Cos(ph);
     gluLookAt(Ex,Ey,Ez , 0,0,0 , 0,Cos(ph),0);
   }

   Sky(2*dim);
   float Position[]  = {distance*Cos(zh),kirby_y,distance*Sin(zh),1.0};
   kirby(Position[0],Position[1],Position[2] , 0.1);
   pika(0,0,0);
   yoshi(0,0,0);

   glPushMatrix();
   glTranslated(-3.4,10, -4);
   glRotatef(90, 0,0,1);
   glRotatef(270, 0,1,0);
   glScaled(1.5, 1.5, 1.5);
   platform(0, 0, 0);
   glPopMatrix();

   glPushMatrix();
   glTranslated(+7,10, -4);
   glRotatef(90, 0,0,1);
   glRotatef(270, 0,1,0);
   glScaled(1.5, 1.5, 1.5);
   platform(0, 0, 0);
   glPopMatrix();

   glPushMatrix();
   glTranslated(1.6, 14, -4);
   glRotatef(90, 0,0,1);
   glRotatef(270, 0,1,0);
   glScaled(1.5, 1.5, 1.5);
   platform(0, 0, 0);
   glPopMatrix();

   glPushMatrix();
   glScaled(2,2,2);
   stage(0,0,0);
   glPopMatrix();

   glPushMatrix();
   glTranslated(0, 6, -9);
   glScaled(1, 1.4, 1);
   whispy(0, 0, 0);
   glPopMatrix();

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
   //  Five pixels from the lower left corner of the window
   glWindowPos2i(5,25);
   //  Print the text string
   if(fp) {
     Print("Camera: First Person");
   }
   else if(mouse) {
     Print("Camera: Mouse");
   }
   else {
     Print("Camera: Perpective");
   }
   //  Render the scene
   ErrCheck("display");
   glFlush();
   //  Make the rendered scene visible
   glutSwapBuffers();

}

void special(int key,int x,int y)
{

  if(!fp && !mouse) {
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
   //  Keep angles to +/-360 degrees
   th %= 360;
   ph %= 360;

  }
   Project(fov, asp, dim);
   glutPostRedisplay();

}

void keyboard(unsigned char key,int x,int y)
{

   if (key==27) {
     exit(0);
   }
   //  Reset view angle
   if (key == '0')
      th = ph = 0;
   //  Toggle axes
   if(key == 'A') {
     axes = 1 - axes;
   }

   if(key == 'f') {
     fp = 1 - fp;
     if(fp) {
       mouse = 0;
     }
   }

   if(key == 'm') {
     mouse = 1 - mouse;
     if(mouse) {
       fp = 0;
     }
   }

   if(fp == 0 && mouse == 0) {
     if(key == '+') {
       dim -= 1;
     }

     if(key == '-') {
       dim += 1;
     }
   }

   if(mouse) {
     double tm = 1;

     if (key=='w') {
       camera_x += lx * tm;
       camera_z += lz * tm;
     }

     if (key=='s') {
       camera_x -= lx * tm;
       camera_z -= lz * tm;
     }
   }

   if(fp) {
     double tt = .1;

     if (key=='w') {
       epx += fpx * tt;
       epz += fpz * tt;
     }

     if (key=='s') {
       epx -= fpx * tt;
       epz -= fpz * tt;
     }

     if(key == 'i') {
       fpy += 1;
     }

     if(key == 'k') {
       fpy -= 1;
     }

     if (key=='d') {
       rotation += 1.5;
     }

     if (key=='a') {
       rotation -= 1.5;
     }
   }

   Project(fov, asp, dim);
   glutPostRedisplay();

}

void reshape(int width,int height)
{
   //  Ratio of the width to the height of the window
   asp = (height>0) ? (double)width/height : 1;
   glViewport(0,0, width,height);
   Project(fov, asp, dim);
}

void mouseMove(int x, int y)
{

	if (isDragging) { // only when dragging
		// update the change in angle
		deltaAngle = (x - xDragStart) * 0.005;

		// camera's direction is set to angle + deltaAngle
		lx = sin(angle + deltaAngle);
		lz = -cos(angle + deltaAngle);
	}

}

void mouseButton(int button, int state, int x, int y)
{

	if (button == GLUT_LEFT_BUTTON) {
		if (state == GLUT_DOWN) { // left mouse button pressed
			isDragging = 1; // start dragging
			xDragStart = x; // save x where button first pressed
		}
		else  { /* (state = GLUT_UP) */
			angle += deltaAngle; // update camera turning angle
			isDragging = 0; // no longer dragging
		}
	}

}

int main(int argc,char* argv[])
{
   //  Initialize GLUT and process user parameters
   glutInit(&argc,argv);
   //  Request double buffered, true color window with Z buffering at 600x600
   glutInitWindowSize(600,600);
   glutInitDisplayMode(GLUT_RGB | GLUT_DEPTH | GLUT_DOUBLE);
   //  Create the window
   glutCreateWindow("Final_Project_D'Artagnan_Wake");
   //  Tell GLUT to call "display" when the scene should be drawn
   glutDisplayFunc(display);
   //  Tell GLUT to call "reshape" when the window is resized
   glutReshapeFunc(reshape);
   //  Tell GLUT to call "special" when an arrow key is pressed
   glutSpecialFunc(special);
   //  Tell GLUT to call "key" when a key is pressed
   glutKeyboardFunc(keyboard);
   glutMouseFunc(mouseButton); // process mouse button push/release
	 glutMotionFunc(mouseMove); // process mouse dragging motion
   glutIdleFunc(update);
   texture[0] = LoadTexBMP("grass_side.bmp");
   texture[1] = LoadTexBMP("dirt_bottom.bmp");
   texture[2] = LoadTexBMP("platform_top.bmp");
   texture[3] = LoadTexBMP("platform_bottom.bmp");
   texture[4] = LoadTexBMP("grass_top.bmp");
   texture[5] = LoadTexBMP("grass_way.bmp");
   texture[6] = LoadTexBMP("whispy.bmp");
   texture[7] = LoadTexBMP("leaves.bmp");
   sky[0] = LoadTexBMP("background.bmp");

   // Objects
   kirbyObj = LoadOBJ("kirby.obj");
   pikaObj = LoadOBJ("pika.obj");
   yoshiObj = LoadOBJ("yoshi.obj");

   //  Pass control to GLUT so it can interact with the user
   ErrCheck("init");
   glutMainLoop();
   return 0;
}
