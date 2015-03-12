
#include <stdio.h>
#include <stdlib.h>
#include <Math.h>
#include <time.h>
#include <GL/glut.h>
#include "Terrain.h"
#define MAX 200
#define MAXBIT 100
#define NumTree 10
#define R 34
#define G 139
#define B 34
//3D
int side_window;
bool switch3D = false;
int sidewidth, sideheight;
int sidex, sidey;
float pts[MAX][MAX];
char texData[R][G][B];
GLuint texture;           
scrPt numTree[NumTree];


double eyeX = -40;
double eyeY = 20;
double eyeZ = -40;
double lookX =100;
double lookY = 0;
double lookZ = 100;
bool first = true;
/*******/
Terrain terrain[MAX][MAX];





/***/
void iniT();
void createT();
void drawT(int x, int z);
void drawTerrain(int x, int z);
float nosie(int x, int y);
void noise_1();
void noise_2();
float Cosine_Interpolate(float a, float b, float x);
float PerlinNoise_2D(float x, float y);
float InterpolatedNoise2D(float x, float y);
float SmoothNosie_1(int x, int z);
void drawTree(float r, float h, int iter);
void createBranches(GLUquadricObj *optr, double a, double r, double h, int iter);
void LoadGLTextures( );
float RandomBetween();

void redraw_func(int id) {

    glutSetWindow(side_window);  
	glutPostRedisplay();
}
void iniT(){

	int x ,z;
	if(first){
	for(z =0 ; z< MAX;z++)
		for( x = 0; x < MAX; x++){
		    terrain[x][z].x =x;
         // terrain[x][z].y =rand()%3;
			terrain[x][z].y =100.0f*PerlinNoise_2D((x+10000)/10.0f, (z+10000)/10.0f);
            terrain[x][z].z =z;

            terrain[x][z].s =x/15.0f;
            terrain[x][z].t =z/15.0f;
		}

		for(int random = 0; random <NumTree;++random){
			numTree[random].x = rand()%MAX;
			numTree[random].y=  rand()%MAX;
			numTree[random].size=  rand()%5+1;
	}

			LoadGLTextures( );
	}

	

float PcX,PcZ,PcY;

	for(z =0 ; z< MAX;z++)
		for( x = 0; x < MAX; x++){
			if(x>0 && z>0 && x<249 && z<249){
                PcX = terrain[x+1][z].y-terrain[x-1][z].y;
				PcZ = terrain[x][z+1].y-terrain[x][z-1].y;
				PcY = PcX*PcZ;

				
                terrain[x][z].norx =PcX;
                terrain[x][z].nory =PcY;
                terrain[x][z].norz =PcZ;
         
            }
            else
            {
                terrain[x][z].norx =0.0f;
                terrain[x][z].nory =1.0f;
                terrain[x][z].norz =0.0f;
            }







			
		}
}
float PerlinNoise_2D(float x, float z){
		float total = 0.0f;
		float p = 0.25;
		double n =4;

		for (int i = 0; i <= n; ++i){
			float frequency = pow((float)2,i);
			float amplitude = pow(p,i);
			total = total + InterpolatedNoise2D(x * frequency, z * frequency) * amplitude;
		}
		return total;
	}
float InterpolatedNoise2D(float x, float z)
{

      int integer_X    = int(x);
      float fractional_X = x - integer_X;

      int integer_Z    = int(z);
      float fractional_Z = z - integer_Z;

      float v1 = SmoothNosie_1(integer_X,     integer_Z);
      float v2 = SmoothNosie_1(integer_X + 1, integer_Z);
      float v3 = SmoothNosie_1(integer_X,     integer_Z + 1);
      float v4 = SmoothNosie_1(integer_X + 1, integer_Z + 1);

      float i1 = Cosine_Interpolate(v1 , v2 , fractional_X);
      float i2 = Cosine_Interpolate(v3 , v4 , fractional_X);

      return Cosine_Interpolate(i1 , i2 , fractional_Z);
}

float Cosine_Interpolate(float a, float b, float x)
{
    double ft = x * 3.1415927;
    double f = (1 - cos(ft)) * 0.5f;

    return  a*(1-f) + b*f;

}
float SmoothNosie_1(int x, int z)
{
    float corners = ( nosie(x-1, z-1)+nosie(x+1, z-1)+nosie(x-1, z+1)+nosie(x+1, z+1) ) / 16.0f;
    float sides   = ( nosie(x-1, z)  +nosie(x+1, z)  +nosie(x, z-1)  +nosie(x, z+1) ) /  8.0f;
    float center  =  nosie(x, z) / 4.0f;
    return corners + sides + center;
}


float nosie(int x, int z){
	//pseudo-random number generator
  x = x % 25;
  z = z % 25;
  int n = x + z * 57;
  n = (n<<13) ^ n;
  return ( 1.0f - ( (n * (n * n * 15731 + 789221) + 1376312589) & 0x7fffffff) / 1073741824.0f); 
}

float RandomBetween()
{
    float diff = 1- 0;
    return (((float) rand() / RAND_MAX) * diff) + 0;
}

void noise_1(){
int x, z;

float i = RandomBetween();
for(z =0 ; z< MAXBIT;z++)
		for( x = 0; x < MAXBIT; x++){
			texData[x][z][0]= RandomBetween();

		}



}
void noise_2(){
int  x,z;
for(z =0 ; z< MAXBIT;z++){
		for( x = 0; x < MAXBIT; x++)
			if(x%2==0 && z%2 == 0)
				texData[x][z][1] =RandomBetween();
				}

for(z =0 ; z< MAXBIT;z++)
		for( x = 0; x < MAXBIT; x++)
			if(x%2!=0 && z%2 == 0)
				texData[x][z][1] =(texData[x-1][z][1]+texData[x+1][z][1])/2;

for(z =0 ; z< MAXBIT;z++)
		for( x = 0; x < MAXBIT; x++)
		if(x%2==0 && z%2 != 0)
			texData[x][z][1] =(texData[x][z-1][1]+texData[x][z+1][1])/2;

for(z =0 ; z< MAXBIT;z++)
		for( x = 0; x < MAXBIT; x++)
		if(x%2!=0 && z%2 != 0)
			texData[x][z][1] =(texData[x-1][z][1]+texData[x+1][z][1])/2;
			

}
void noise_3(){
	int  x,z;

	for(z =0 ; z< MAXBIT;z++){
		for( x = 0; x < MAXBIT; x++)
			if(x%4==0 && z%4 == 0)
				texData[x][z][2] =RandomBetween();				
				}

	for(z =0 ; z< MAXBIT;z++){
	for( x = 0; x < MAXBIT; x++)
		printf("%0.01f |",texData[x][z][2] );
		printf("\n");
}
}
void drawTerrain(int x, int z){
	//cross product
	


	glBegin(GL_TRIANGLE_STRIP );
	glNormal3f(terrain[x][z].norx,terrain[x][z].nory,terrain[x][z].norz);
    glTexCoord2f(0.0f, 0.0f);
	glVertex3f(x*1,terrain[x][z].y,z*1);
    glTexCoord2f(1.0f, 0.0f);
	glVertex3f((x+1)*1,terrain[x+1][z].y,z*1);
	glTexCoord2f(0.0f, 1.0f);
	glVertex3f(x*1,terrain[x][z+1].y,(z+1)*1);
	glEnd();

	glBegin(GL_TRIANGLE_STRIP );
		glNormal3f(terrain[x][z].norx,terrain[x][z].nory,terrain[x][z].norz);

	glTexCoord2f(1.0f, 1.0f);

	glVertex3f((x+1)*1,terrain[x+1][z+1].y,(z+1)*1);
	glTexCoord2f(1.0f, 0.0f);
	glVertex3f((x+1)*1,terrain[x+1][z].y,z*1);
	glTexCoord2f(0.0f, 1.0f);
	glVertex3f(x*1,terrain[x][z+1].y,(z+1)*1);
	glEnd();

}

void LoadGLTextures( ){
	int i;
	int j;
	float perl;
	if(first){
	for(i=0;i!=R;i++)
    {
        for(j=0;j!=G;j++)
        {  
            perl = PerlinNoise_2D(i,j)*10000;
            perl = perl / 4;
 
            texData[i][j][0] = (char)perl/64;
            texData[i][j][1] = (char)perl;
            texData[i][j][2] = (char)perl/64;
        }
    first = false;

	}
	}
	
}






















/**************************************** main() ********************/
void myGlutDisplay( void )
{

	
  float light_ambient[4] = { 0.2, 0.2, 0.2, 1.0 }; // r, g, b, a
  float light_diffuse[4] = { 1.0, 1.0, 1.0, 1.0 };// r, g, b, a
  float light_specular[4] = { 1.0, 1.0, 1.0, 1.0 }; // r, g, b, a
  float light_position[4] = { -1.0, 10.0, 0.0 , 0.0 }; // x, y, z, w

  float ad_col[4] = { 1.0, 0.5, 0.5, 1.0 }; // r, g, b, a
  float ad_col2[4] = { 1.0, 1.0, 1.0, 1.0 }; // r, g, b, a
  float spec_col[4] = { 1.0, 1.0, 1.0, 1.0 }; // r, g, b, a

  	
  glClearColor(0,0,0,0);
  glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();


  gluPerspective(65,(float)400/(float)400,10,1000);
                          // theta, aspect, dnear, dfar
  glViewport(0,0,sidewidth,sideheight); // startx, starty, xsize, ysize 
  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();	

  glEnable(GL_LIGHTING);
  glEnable(GL_DEPTH_TEST);
  glEnable(GL_NORMALIZE);
  glLightfv(GL_LIGHT0, GL_AMBIENT, light_ambient);
  glLightfv(GL_LIGHT0, GL_DIFFUSE, light_diffuse);
  glLightfv(GL_LIGHT0, GL_SPECULAR, light_specular);
  glLightfv(GL_LIGHT0, GL_POSITION, light_position);
  glEnable(GL_LIGHT0);
  glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, ad_col);
  glMaterialfv(GL_FRONT, GL_SPECULAR, spec_col);
 // noise_3();
	
   //enable texture
	glGenTextures(1, &texture);
	glBindTexture(GL_TEXTURE_2D, texture);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, MAXBIT, MAXBIT, 0, GL_RGB, GL_UNSIGNED_BYTE, texData);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_NEAREST);
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, texture); // use texture d
// viewer is at (-10,10,-10) looking towards the center of the terrain
	gluLookAt(eyeX,eyeY,eyeZ,lookX,lookY,lookZ,0,1,0);

	
	
	

	iniT();	
	LoadGLTextures( );


	
	int x,z;
	for(z =0 ; z< MAX;z++)
		for( x = 0; x < MAX; x++)
			drawTerrain(x,z);
	glDisable(GL_TEXTURE_2D);
		
	for(int random = 0; random <NumTree;++random){
		int randX = numTree[random].x;
		int randY = numTree[random].y;
		int num = numTree[random].size;
		float height = terrain[randX][randY].y;
		glPushMatrix();
		glTranslatef(randX,height,randY);
		drawTree(1, 10 ,num);
		glPopMatrix();
	}
	
	glutSwapBuffers(); 
	
}

void drawTree(float r, float h, int iter){
	  GLUquadricObj *optr;
  // *************** Draw the vertical cylinder ************************************
  optr = gluNewQuadric();
  gluQuadricDrawStyle(optr,GLU_FILL);
  glPushMatrix();
  glRotatef(-90.0,1.0,0.0,0.0);
  
  gluCylinder(optr,r,r,h,10,2); // ptr, rbase, rtop, height, nLongitude, nLatitudes
  glPopMatrix();
    // *************** If more iterations, then recursively draw branches ********
  if (iter>0) {
	  for (int num=0 ;num <3; ++num)
		createBranches(optr,  120*num ,  r,  h,  iter);
  }
}
void createBranches(GLUquadricObj *optr, double a, double r, double h, int iter){
	glPushMatrix();
    glTranslatef(0.0,h,0.0);
    glRotatef(a,0.0,1.0,0.0);
    glRotatef(30.0,1.0,0.0,0.0);
    drawTree(0.8*r,0.8*h,iter-1);
    glPopMatrix();	
}


void myGlutKeyboard(unsigned char Key, int x, int y)
{

switch(Key)
  {
  case 'w':
	  eyeX+=4;
	  eyeZ+=4;
	  lookX+=4;
	  lookZ+=4;
	  
	  break;
  case 's':
	  eyeX-=4;
	  eyeZ-=4;
	  lookX-=4;
	  lookZ-=4;
	  break;
  case 'a':	
	

	  lookZ+=2 *-2;
	  lookZ+=2 *-2;
	  break;
  case 'd':
	

	  lookZ+=2 *2;
	  lookZ+=2 *2;
	  break;
  };
  
  glutPostRedisplay();
}




int main(int argc, char* argv[])
{


	sidewidth = 600;
	sideheight = 400;
	srand(time(NULL));
	glutInitDisplayMode( GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH );
	side_window = glutCreateWindow( "HW1" );
	glutPositionWindow( 10, 30 );
	glutReshapeWindow( 600 , 400 );

	glutDisplayFunc( myGlutDisplay );
	//glutReshapeFunc( myGlutReshape ); 
	glutKeyboardFunc( myGlutKeyboard );
	//glutMotionFunc( myGlutMotion ); 
	//glutMouseFunc( myGlutMouse );
	glEnable(GL_DEPTH_TEST);
	glutMainLoop();

	return 0;

}
