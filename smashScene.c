/*
 *  3D Objects
 *
 *  Demonstrates how to draw objects in 3D.
 *
 *  Key bindings:
 *  m/M                   Toggle Perspective mode
 *  f                     First person view
 *  w                     Move forward
 *  s                     Move backwards
 *  a                     Look left
 *  d                     Look right
 *  up/dow arrow          +/- Elevation
 *  Right/left arrow      +/- Azimuth
 *  0                     Reset view angle in Perspective/Orthogonal mode
 *  ESC                   Exit
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
#include "CSCIx229.h"

int th=0;         //  Azimuth of view angle
int ph=0;         //  Elevation of view angle
int zh = 0;
int axes=1;       //  Display axes
int mode=1;       //  Projection type
double dim=7;     // Size of scene
double asp=1;     // Aspect ratio
int fov = 55;     // Field of view
int fp = 0;
double rotation = 0.0;

// TODO: Implement first person view
double fpx = 0.0;
double fpz = 0.0;
double epx = 0.0;
double epy = 0.0;
double epz = 5.0;

unsigned int texture[7]; // Texture names


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

static void ProjectView()
{
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();

  if(mode) {
    gluPerspective(fov, asp, dim / 4, 4 * dim);
  }
  else {
  glOrtho(-asp*dim,+asp*dim, -dim,+dim, -dim,+dim);
  }
  //  Switch to manipulating the model matrix
  glMatrixMode(GL_MODELVIEW);
  //  Undo previous transformations
  glLoadIdentity();

}

/*
 *  Draw vertex in polar coordinates with normal
 */
static void Vertex(double th,double ph)
{
   double x = Sin(th)*Cos(ph);
   double y = Cos(th)*Cos(ph);
   double z =         Sin(ph);
   //  For a sphere at the origin, the position
   //  and normal vectors are the same
   glNormal3d(x,y,z);
   glTexCoord2d(th/360.0,ph/180.0+0.5);
   glVertex3d(x,y,z);
}

static void platform(double x, double y, double z)
{

  int th, ph;
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
  for (unsigned int i = 0; i <= 32; ++i)
  {
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

  int th, ph;
  const int d = 5;

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

  /*// From arrow up
  // Starting at index 1
  // Starting at point front left
  // Numbering rotating to the right

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
  for (unsigned int i = 0; i <= numberOfSegments; ++i)
  {
      float c = cos(angleIncrement * i);
      float s = sin(angleIncrement * i);
      glTexCoord2f( textureCoordinateIncrement * i, 0); glVertex3f( c, 0, s);
      glTexCoord2f( textureCoordinateIncrement * i, 1.0f); glVertex3f( c, .5, s);
  }
  glEnd();
  glPopMatrix();


  glPushMatrix();
  glTranslated(x + 1.5, y, z);
  glRotatef(90, 0, 0,1);
  glScaled(0.5,.5,.5);
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

  glPushMatrix();
  glTranslated(x + .5,y, z+ 1);
  glRotatef(90, 0, 0,1);
  glRotatef(-50, 1, 0, 0);
  glScaled(0.5,.5,.5);
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

  glPushMatrix();
  glTranslated(x - 1.5,y, z - .5);
  glRotatef(90, 0, 0,1);
  glRotatef(-50, 1, 0, 0);
  glRotatef(-150, 1, 0, 0);
  glScaled(0.5,.5,.5);
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

void update()
{
   //  Elapsed time in seconds
   double t = glutGet(GLUT_ELAPSED_TIME)/1000.0;
   zh = fmod(90*t,360.0);
   //  Tell GLUT it is necessary to redisplay the scene
   glutPostRedisplay();
}

/*
 *  OpenGL (GLUT) calls this routine to display the scene
 */
void display()
{
   const double len=1.5;  //  Length of axes
   //  Erase the window and the depth buffer
   glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
   //  Enable Z-buffering in OpenGL
   glEnable(GL_DEPTH_TEST);
   //  Undo previous transformations
   glLoadIdentity();
   //  Perspective - set eye position
   if (mode) {
        double Ex = -2*dim*Sin(th)*Cos(ph);
        double Ey = +2*dim        *Sin(ph);
        double Ez = +2*dim*Cos(th)*Cos(ph);
        gluLookAt(Ex,Ey,Ez , 0,0,0 , 0,Cos(ph),0);
   }
   else {
        glRotatef(ph,1,0,0);
        glRotatef(th,0,1,0);
   }

   glPushMatrix();
   glTranslated(-1.7,5, -2);
   glRotatef(90, 0,0,1);
   glRotatef(270, 0,1,0);
   glScaled(0.7, 0.7, 0.7);
   platform(0, 0, 0);
   glPopMatrix();

   glPushMatrix();
   glTranslated(+3.5,5, -2);
   glRotatef(90, 0,0,1);
   glRotatef(270, 0,1,0);
   glScaled(0.7, 0.7, 0.7);
   platform(0, 0, 0);
   glPopMatrix();

   glPushMatrix();
   glTranslated(+.8,7, -2);
   glRotatef(90, 0,0,1);
   glRotatef(270, 0,1,0);
   glScaled(0.7, 0.7, 0.7);
   platform(0, 0, 0);
   glPopMatrix();

   stage(0,0,0);


   glPushMatrix();
   glTranslated(0,3, -4.5);
   glScaled(0.5, .7, .5);
   whispy(0, 0, 0);
   glPopMatrix();




   //  White
   glColor3f(1,1,1);
   //  Draw axes
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
   Print("Angle=%d,%d",th,ph);
   //  Render the scene
   ErrCheck("display");
   glFlush();
   //  Make the rendered scene visible
   glutSwapBuffers();
}

/*
 *  GLUT calls this routine when an arrow key is pressed
 */
void special(int key,int x,int y)
{
  if(!fp) {
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
   ProjectView();
   //  Tell GLUT it is necessary to redisplay the scene
   glutPostRedisplay();
}

/*
 *  GLUT calls this routine when a key is pressed
 */
void keyboard(unsigned char key,int x,int y)
{


   if (key==27) {
     exit(0);
   }
   //  Reset view angle
   if (key == '0')
      th = ph = 0;
   //  Toggle axes

   if(key == 'p') {
     mode = 1 - mode;
   }

   if(key == 'a') {
     axes = 1 - axes;
   }

   if(key == '+') {
     dim -= 1;
   }

   if(key == '-') {
     dim += 1;
   }

   ProjectView();
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
   ProjectView();
}

/*
 *  Start up GLUT and tell it what to do
 */
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
   //glutIdleFunc(update);
   texture[0] = LoadTexBMP("grass_side.bmp");
   texture[1] = LoadTexBMP("dirt_bottom.bmp");
   texture[2] = LoadTexBMP("platform_top.bmp");
   texture[3] = LoadTexBMP("platform_bottom.bmp");
   texture[4] = LoadTexBMP("grass_top.bmp");
   texture[5] = LoadTexBMP("grass_way.bmp");
   texture[6] = LoadTexBMP("whispy.bmp");
   //  Pass control to GLUT so it can interact with the user
   ErrCheck("init");
   glutMainLoop();
   return 0;
}
