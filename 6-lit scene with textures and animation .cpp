
//use a function called drawBox to render the floor
//demonstrates several calls to drawBox and several translations to move the boxes drawn by DrawBox
//show the rudiments of a camera function - moves the camera using world x,z co-ords and a rotate about
// the y-axis.  
//uses lighting-  not that we change colour of objects by changing material properties before rendering.
//uses textures - this requires reading bitmaps from a file.  Any reader will do. Home grown C based one used
//Note use of windows.h bitmap struct to save re-inventing the wheel
//needs bitmap files of course this uses share.bmp and checker.bmp - not dimensions of bitmaps are power of 2
//cube spin added
//////Defines
#define BITMAP_ID 0x4D42		// the universal bitmap ID

////// Includes
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <GL/glut.h>
#include <windows.h>
////// Texture Information
BITMAPINFOHEADER	bitmapInfoHeader;	// bitmap info header (needs windows.h to define type
unsigned char*		bitmapData;			// the texture data
static GLuint		texnum[2];			// array of 2 texture object names

/////////#include <gl/gl.h>				// standard OpenGL include
/////////#include <gl/glu.h>				// OpenGL utilties - we use glut - it already includes these

////// Global Variables
float globangle=0;

//function prototypes
unsigned char *LoadBitmapFile(char *filename, BITMAPINFOHEADER *bitmapInfoHeader);
int WriteBitmapFile(char *filename, int width, int height, unsigned char *imageData);
void init(void);
void display(void);
void spindDisplay(void);
void drawBox(float w, float h, float l);
void reshape(int w, int h);
void camera(float,float,float);

////// Lighting variables
float ambientLight[] = {1.0f, 1.0f, 1.0f, 1.0f  };	// ambient light
float diffuseLight[] = {1.0f, 1.0f, 1.0f, 1.0f };	// diffuse light
float lightPosition[] = {10.0f, 100.0f, 10.0f, 0.0f };	// the light position

////// Material variables
float matAmbient[] = { 0.3f, 0.5f, 0.8f, 1.0f };
float matDiff[] = { 0.25f, 0.25f, 0.25f, 1.0f };
float matAmbient1[] = { 0.83f, 0.5f, 0.8f, 1.0f };
float matDiff1[] = { 0.525f, 0.25f, 0.25f, 1.0f };
float matAmbient2[] = { 0.83f, 0.0f, 0.0f, 1.0f };
float matDiff2[] = { 0.525f, 0.2f, 0.2f, 1.0f };

void init()
// Initialize
//************************************************************************
// desc: initializes OpenGL
{
	glClearColor(1.0f, 1.0f, 1.0f, 0.0f);		// clears colour buffer to to white
	glEnable(GL_DEPTH_TEST);					// hidden surface removal
	glEnable(GL_CULL_FACE);						// do not calculate inside of poly's
	glFrontFace(GL_CCW);						// counter clock-wise polygons are out

	//***** lighting
    glEnable(GL_SMOOTH);
	glEnable(GL_LIGHTING);						// enable lighting

	// Setup the materials for LIGHT0
	glMaterialfv(GL_FRONT, GL_AMBIENT, matAmbient);
	glMaterialfv(GL_FRONT, GL_DIFFUSE, matDiff);

	// Now setup LIGHT0
	glLightfv(GL_LIGHT0, GL_AMBIENT, ambientLight);		// setup the ambient element
	glLightfv(GL_LIGHT0, GL_DIFFUSE, diffuseLight);		// the diffuse element
	glLightfv(GL_LIGHT0, GL_POSITION, lightPosition);	// place the light in the world

	// Enable the light
	glEnable(GL_LIGHT0);

		glEnable(GL_TEXTURE_2D);					// enable 2D texturing

	// load our bitmap file
	bitmapData = LoadBitmapFile("share.bmp", &bitmapInfoHeader);

	glGenTextures(2, texnum);                  // generate 2 texture objects
	glBindTexture(GL_TEXTURE_2D, texnum[0]);    // enable our first texture object

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

     // generate the texture image
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, bitmapInfoHeader.biWidth,
				 bitmapInfoHeader.biHeight, 0, GL_RGB, GL_UNSIGNED_BYTE, bitmapData);



//*2nd texture********************************************************8

	bitmapData = LoadBitmapFile("checker.bmp", &bitmapInfoHeader);


	glBindTexture(GL_TEXTURE_2D, texnum[1]);       // enable our second texture object

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

     // generate the texture image
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, bitmapInfoHeader.biWidth,
				 bitmapInfoHeader.biHeight, 0, GL_RGB, GL_UNSIGNED_BYTE, bitmapData);

//************end of 2nd tex******************************************************8

	

}
	

void drawBox(float w, float h, float l)
//drawBox
//***************************************************************************
//This function first translates a further x,y,z units then draws a box of
//width w, height h, length l
{
	    glPushMatrix();       //save modelview 
		w=w/2.0; h = h/2.0; l = l/2.0; //adjust values so centre is in middle of box
		glBindTexture(GL_TEXTURE_2D, texnum[1]);       // enable our second texture object
		glBegin(GL_POLYGON);
			glNormal3f(0.0f, 1.0f, 0.0f);	// top face
			glTexCoord2f(0.0f, 0.0f); glVertex3f(w, l, h);	
			glTexCoord2f(1.0f, 0.0f); glVertex3f(w, l, -h);
			glTexCoord2f(1.0f, 1.0f); glVertex3f(-w, l, -h);
			glTexCoord2f(0.0f, 1.0f); glVertex3f(-w, l, h);
		glEnd();
		glBegin(GL_POLYGON);
			glNormal3f(0.0f, 0.0f, 1.0f);	// front face
			glTexCoord2f(0.0f, 0.0f);glVertex3f(w, l, h);	
			glTexCoord2f(1.0f, 0.0f);glVertex3f(-w, l, h);
			glTexCoord2f(1.0f, 1.0f);glVertex3f(-w, -l, h);
			glTexCoord2f(0.0f, 1.0f);glVertex3f(w, -l, h);
		glEnd();
		glBegin(GL_POLYGON);
			glNormal3f(1.0f, 0.0f, 0.0f);	// right face
			glTexCoord2f(0.0f, 0.0f);glVertex3f(w, l, h);	
			glTexCoord2f(1.0f, 0.0f);glVertex3f(w, -l, h);
			glTexCoord2f(1.0f, 1.0f);glVertex3f(w, -l, -h);
			glTexCoord2f(0.0f, 1.0f);glVertex3f(w, l, -h);
		glEnd();
	    glBindTexture(GL_TEXTURE_2D, texnum[0]);       // enable our first texture object
		glBegin(GL_POLYGON);
			glNormal3f(-1.0f, 0.0f, 0.0f);	// left face
			glTexCoord2f(0.0f, 0.0f);glVertex3f(-w, l, h);	
			glTexCoord2f(1.0f, 0.0f);glVertex3f(-w, l, -h);
			glTexCoord2f(1.0f, 1.0f);glVertex3f(-w, -l, -h);
			glTexCoord2f(0.0f, 1.0f);glVertex3f(-w, -l, h);
		glEnd();
		glBegin(GL_POLYGON);
			glNormal3f(0.0f, -1.0f, 0.0f);	// bottom face
			glTexCoord2f(0.0f, 0.0f);glVertex3f(-w, -l, h);
			glTexCoord2f(1.0f, 0.0f);glVertex3f(-w, -l, -h);
			glTexCoord2f(1.0f, 1.0f);glVertex3f(w, -l, -h);
			glTexCoord2f(0.0f, 1.0f);glVertex3f(w, -l, h);	
		glEnd();
		glBegin(GL_POLYGON);
			glNormal3f(0.0f, 0.0f, -1.0f);	// back face
			glTexCoord2f(0.0f, 0.0f);glVertex3f(w, -l, -h);
			glTexCoord2f(1.0f, 0.0f);glVertex3f(-w, -l, -h);
			glTexCoord2f(1.0f, 1.0f);glVertex3f(-w, l, -h);
			glTexCoord2f(0.0f, 1.0f);glVertex3f(w, l, -h);	
		glEnd();
		glPopMatrix();   //restore previous modelview matrix so leaving things as you found them 
}


// display
//************************************************************************
// desc: handles drawing of scene
void display()
{
	glPushMatrix();
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);		// clear screen and depth buffer
   										                    // reset modelview matrix

    glColor3f(0.8,0.8,1.0);  //set colour not used. Automatically disabled when lighting Enabled
    glTranslatef(0,0,0);  //move by x,y and z units
	glMaterialfv(GL_FRONT, GL_AMBIENT, matAmbient);//set backdrop material properties
	glMaterialfv(GL_FRONT, GL_DIFFUSE, matDiff);
	drawBox(5.0, 10.0, 0.01);  //draw floor
    glColor3f(1.0,0.8,0.8);  //set colour not used. Automatically disabled when lighting Enabled
 
	glPushMatrix(); //save current position/orientation, etc.
	glTranslatef(0,1.505,-5);  //move by x,y and z units
	glMaterialfv(GL_FRONT, GL_AMBIENT, matAmbient1);//set backdrop material properties
	glMaterialfv(GL_FRONT, GL_DIFFUSE, matDiff1);
	drawBox(5,0.01,3);  //draw back drop
 	glPopMatrix(); //restore previous "current" position/orientation etc.

	glColor3f(0.8,1.0,0.8);  //set colour not used. Automatically disabled when lighting Enabled
    glPushMatrix(); //save current position/orientation, etc.
	glTranslatef(1,1,1);  //move by x,y and z units
	glMaterialfv(GL_FRONT, GL_AMBIENT, matAmbient1);//set teapot light material properties
	glMaterialfv(GL_FRONT, GL_DIFFUSE, matDiff1);
	glutSolidTeapot(0.2); //param is size of teapot
 	glPopMatrix(); //restore previous "current" position/orientation etc.

	glColor3f(0.4,1.0,1.0);  //set colour not used. Automatically disabled when lighting Enabled
	glPushMatrix(); //save current position/orientation, etc.
	glTranslatef(0,1,0);  //move by x,y and z units
	glMaterialfv(GL_FRONT, GL_AMBIENT, matAmbient2);//set box light material properties
	glMaterialfv(GL_FRONT, GL_DIFFUSE, matDiff2);
	glRotatef(globangle,1,1,1);
	drawBox(1,1,1);  //draw box
 	glPopMatrix(); //restore previous "current" position/orientation etc.

	//  glTranslatef(0,0,0);  //move by x,y and z units
	//drawBox(5.0, 10.0, 0.20);  //draw floor
	glPopMatrix();
	glFlush();  //force drawing

	glutSwapBuffers();			// bring backbuffer to foreground
//any errors then display error codes 
	GLenum errCode;
const GLubyte *errString;
if((errCode = glGetError()) != GL_NO_ERROR) 
{
	   errString = gluErrorString(errCode);
	   fprintf (stderr,"OpenGL error : %s\n",errString);
}

}

void reshape(int w, int h)
//reshape
//************************************************************************
//OpenGL reshape callback is called when the window is resized
{
	  glViewport(0,0,(GLsizei) w, (GLsizei) h); //respecify size of view port to match new w and h
	  glMatrixMode(GL_PROJECTION);  //go into projection mode
	  glLoadIdentity();   //reset projection matrix
      gluPerspective(60.0,(GLfloat)w/(GLfloat)h,1.0,200.0);  //redefine projection matrix to match changes in w and h
	  glMatrixMode(GL_MODELVIEW);  //switch back to modelview mode
	  glLoadIdentity(); //reset modelview matrix
   	  gluLookAt (0.0, 2.0, 5.0, 0.0, 2.0, -1.0, 0.0, 1.0, 0.0);// note these match the initial values 
																//of the keybard fn vx,vy,vz, angle

}



void keyboard (unsigned char key, int x, int y)
//This function alters the camera angle and x,y,z position
// how would you alter the 'w' and 's' key equations to move 
//the camera in the direction you're looking? Hint -> needs a little bit of trig
// and both  a vx and a vz formula will be needed in each 'w' and 's' case.
{
	static float vx=0,vy=2,vz=5,angle=0;
    switch (key) {
     
	  case 'a'|'A':
        vx= vx - 0.1;
		glMatrixMode(GL_MODELVIEW);
        break;
      case 'd'|'D':
        vx=vx+0.1;  
		glMatrixMode(GL_MODELVIEW);
       break;
	  case 'w'|'w':
        vz= vz - 0.1;
		glMatrixMode(GL_MODELVIEW);
        break;
      case 's'|'S':
		glMatrixMode(GL_MODELVIEW);
        vz= vz + 0.1;
 
        break;
  	  case 'Z'|'z':
        angle= angle - 10;
	
		glMatrixMode(GL_MODELVIEW);
       break;
      case 'X'|'x':
		glMatrixMode(GL_MODELVIEW);
        angle=angle+10;  
	
        break;
      case 27:
         exit(0);
         break;
      default:
         break;

   }
		glLoadIdentity();
		camera(vx,vz,angle);  //use glulookat() if you want
        glutPostRedisplay();
}
void camera(float x,float z, float angle)
{
	glRotatef(angle,0,1,0); //not the inverse transforms and their sequence
	glTranslatef(-x,-2,-z);  //what happens if you swap the transform order
								//visualize it first then try it.
}

void spinDisplay(void)
{
		  globangle = globangle + 0.1f;					// increase our rotation angle counter
	  if (globangle >= 360.0f)					
	  	  globangle = 0.0f;
      glutPostRedisplay();
}
int main(int argc, char** argv)
{
	  glutInit(&argc,argv);
	  glutInitDisplayMode(GLUT_DOUBLE |GLUT_RGB | GLUT_DEPTH);
	  glutInitWindowSize(500,500);
	  glutInitWindowPosition(100,100);
	  glutCreateWindow(argv[0]);
	  init();
	  glutDisplayFunc(display);
	  glutReshapeFunc(reshape);
	  glutKeyboardFunc(keyboard);
	  glutIdleFunc(spinDisplay);
	  glutMainLoop();
	  return 0;
}

//********************************************************************

// LoadBitmapFile
// desc: Returns a pointer to the bitmap image of the bitmap specified
//       by filename. Also returns the bitmap header information.
//		 No support for 8-bit bitmaps.
unsigned char *LoadBitmapFile(char *filename, BITMAPINFOHEADER *bitmapInfoHeader)
{
	FILE *filePtr;							// the file pointer
	BITMAPFILEHEADER	bitmapFileHeader;		// bitmap file header
	unsigned char		*bitmapImage;			// bitmap image data
	int					imageIdx = 0;		// image index counter
	unsigned char		tempRGB;				// swap variable

	// open filename in "read binary" mode
	filePtr = fopen(filename, "rb");
	if (filePtr == NULL)
		return NULL;

	// read the bitmap file header
	fread(&bitmapFileHeader, sizeof(BITMAPFILEHEADER), 1, filePtr);
	
	// verify that this is a bitmap by checking for the universal bitmap id
	if (bitmapFileHeader.bfType != BITMAP_ID)
	{
		fclose(filePtr);
		return NULL;
	}

	// read the bitmap information header
	fread(bitmapInfoHeader, sizeof(BITMAPINFOHEADER), 1, filePtr);

	// move file pointer to beginning of bitmap data
	fseek(filePtr, bitmapFileHeader.bfOffBits, SEEK_SET);

	// allocate enough memory for the bitmap image data
	bitmapImage = (unsigned char*)malloc(bitmapInfoHeader->biSizeImage);

	// verify memory allocation
	if (!bitmapImage)
	{
		free(bitmapImage);
		fclose(filePtr);
		return NULL;
	}

	// read in the bitmap image data
	fread(bitmapImage, 1, bitmapInfoHeader->biSizeImage, filePtr);

	// make sure bitmap image data was read
	if (bitmapImage == NULL)
	{
		fclose(filePtr);
		return NULL;
	}

	// swap the R and B values to get RGB since the bitmap color format is in BGR
	for (imageIdx = 0; imageIdx < bitmapInfoHeader->biSizeImage; imageIdx+=3)
	{
		tempRGB = bitmapImage[imageIdx];
		bitmapImage[imageIdx] = bitmapImage[imageIdx + 2];
		bitmapImage[imageIdx + 2] = tempRGB;
	}

	// close the file and return the bitmap image data
	fclose(filePtr);
	return bitmapImage;
}

// WriteBitmapFile()
// desc: takes image data and saves it into a 24-bit RGB .BMP file
//       with width X height dimensions
int WriteBitmapFile(char *filename, int width, int height, unsigned char *imageData)
{
	FILE			 *filePtr;		// file pointer
	BITMAPFILEHEADER bitmapFileHeader;	// bitmap file header
	BITMAPINFOHEADER bitmapInfoHeader;	// bitmap info header
	int				 imageIdx;	// used for swapping RGB->BGR
	unsigned char	 tempRGB;			// used for swapping

	// open file for writing binary mode
	filePtr = fopen(filename, "wb");
	if (!filePtr)
		return 0;

	// define the bitmap file header
	bitmapFileHeader.bfSize = sizeof(BITMAPFILEHEADER);
	bitmapFileHeader.bfType = 0x4D42;
	bitmapFileHeader.bfReserved1 = 0;
	bitmapFileHeader.bfReserved2 = 0;
	bitmapFileHeader.bfOffBits = sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER);
	
	// define the bitmap information header
	bitmapInfoHeader.biSize = sizeof(BITMAPINFOHEADER);
	bitmapInfoHeader.biPlanes = 1;
	bitmapInfoHeader.biBitCount = 24;						// 24-bit
	bitmapInfoHeader.biCompression = BI_RGB;				// no compression
	bitmapInfoHeader.biSizeImage = width * abs(height) * 3;	// width * height * (RGB bytes)
	bitmapInfoHeader.biXPelsPerMeter = 0;
	bitmapInfoHeader.biYPelsPerMeter = 0;
	bitmapInfoHeader.biClrUsed = 0;
	bitmapInfoHeader.biClrImportant = 0;
	bitmapInfoHeader.biWidth = width;						// bitmap width
	bitmapInfoHeader.biHeight = height;					// bitmap height

	// switch the image data from RGB to BGR
	for (imageIdx = 0; imageIdx < bitmapInfoHeader.biSizeImage; imageIdx+=3)
	{
		tempRGB = imageData[imageIdx];
		imageData[imageIdx] = imageData[imageIdx + 2];
		imageData[imageIdx + 2] = tempRGB;
	}

	// write the bitmap file header
	fwrite(&bitmapFileHeader, 1, sizeof(BITMAPFILEHEADER), filePtr);

	// write the bitmap info header
	fwrite(&bitmapInfoHeader, 1, sizeof(BITMAPINFOHEADER), filePtr);

	// write the image data
	fwrite(imageData, 1, bitmapInfoHeader.biSizeImage, filePtr);

	// close our file
	fclose(filePtr);

	return 1;
}
