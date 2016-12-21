#define FREEGLUT_STATIC
#define GLUT_DISABLE_ATEXIT_HACK
#include <gl/GLUT.H>
#include <gl/GLU.H>
#include <gl/GL.H>
#include <GL/freeglut.h>
#include <GL/gl.h>
#include <math.h>

static float Theta = 0.05;
const int block_size = 5;
const int map_size = 7;
int map[49] = { \
1, 1, 1, 1, 1, 1, 1, \
1, 0, 0, 0, 0, 0, 1, \
1, 0, 0, 0, 0, 0, 1, \
1, 0, 0, 1, 0, 0, 1, \
1, 0, 0, 1, 0, 0, 0, \
1, 0, 0, 1, 0, 0, 0, \
1, 0, 0, 1, 1, 1, 1  \
};

GLfloat camera_x = 8, camera_y = 3, camera_z = 40;
GLfloat at_x = 5, at_y = 3, at_z = 0;
GLfloat upx = 0, upy = 1, upz = 0;
GLfloat vector_x = at_x - camera_x;
GLfloat vector_y = at_y - camera_y;
GLfloat vector_z = at_z - camera_z;
GLdouble vectorLength = sqrt(vector_x*vector_x + vector_y*vector_y + vector_z*vector_z);
GLfloat walkSpeed = 0.2;
GLfloat displacement = 0.5;
GLdouble cosTheta = cos(Theta);
GLdouble sinTheta = sin(Theta);

void keyboard(unsigned char key, int x, int y);
void display();
void reshape(int w, int h);
void init();
void mouse(int x, int y);
void idle(void);

int main(int argc, char** argv)
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
	glutInitWindowSize(500, 500);
	glutInitWindowPosition(100, 100);
	glutCreateWindow("Maze Runner");
	glutReshapeFunc(&reshape);
	glutIdleFunc(&idle);
	glutKeyboardFunc(&keyboard);
	glutPassiveMotionFunc(mouse);
	init();
	glutMainLoop();
}

void init()
{
	glClearColor(1.0, 1.0, 1.0, 1.0);
	glColor3f(0.0, 0.0, 0.0);
}

void reshape(int w, int h)
{
	glViewport(0, 0, w, h);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(-4.0, 4.0, -4.0, 4.0, -4.0, 4.0);
}

void idle(void)
{
	display();
}

void display()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(60.0f, 1.0f, 0.01f, 50.0f);
	glMatrixMode(GL_MODELVIEW);

	glEnable(GL_CULL_FACE);
	glEnable(GL_DEPTH_TEST);
	glCullFace(GL_BACK);
	glLoadIdentity();
	gluLookAt(camera_x, camera_y, camera_z, at_x, at_y, at_z, upx, upy, upz);
	glEnable(GL_LIGHT0);
	glEnable(GL_LIGHTING);

	glPolygonMode(GL_FRONT, GL_FILL);
	glPolygonMode(GL_BACK, GL_LINE);     
	glFrontFace(GL_CCW);

	//drawMaze
	for (int x = 0; x < map_size; x++){
		glPushMatrix();
		for (int z = 0; z < map_size; z++)	{
			if (map[map_size*x + z] == 1){
				glutSolidCube(block_size);
				glPushMatrix();
				glTranslatef(0, block_size, 0);
				glutSolidCube(block_size);
				glTranslatef(0, block_size, 0);
				glutSolidCube(block_size);
				glPopMatrix();
			}
			glTranslatef(block_size, 0, 0);
		}
		glPopMatrix();
		glTranslatef(0, 0, block_size);
	}
	glFlush();
}

void mouse(int x, int y)
{
	if (x > 400){
	vector_x = vector_x*cosTheta - vector_z*sinTheta;
	vector_z = vector_z*cosTheta + vector_x*sinTheta;
	}
	if (x < 100){
	vector_x = vector_x*cosTheta + vector_z*sinTheta;
	vector_z = vector_z*cosTheta - vector_x*sinTheta;
	}
	at_x = vector_x + camera_x;
	at_y = vector_y + camera_y;
	at_z = vector_z + camera_z;
	vectorLength = sqrt(vector_x*vector_x + vector_y*vector_y + vector_z*vector_z);
}

void keyboard(unsigned char key, int x, int y) {
	GLfloat tmpat_x = at_x, tmpat_y = at_y, tmpat_z = at_z;
	GLfloat tmpcamera_x = camera_x, tmpcamera_y = camera_y, tmpcamera_z = camera_z;
	switch (key){
	case 'i': {
				 at_x += vector_x*walkSpeed / vectorLength;
				 at_z += vector_z*walkSpeed / vectorLength;
				 camera_x = at_x - vector_x;
				 camera_z = at_z - vector_z;
				 if ((camera_x>-1 && camera_x < map_size*block_size - 1) && (camera_z>-1 && camera_z<map_size * block_size - 1) && map[(map_size*int((1 + camera_z) / block_size) + int((1 + camera_x) / block_size))] == 1){
					 //roll back!
					 at_x = tmpat_x; at_z = tmpat_z;
					 camera_z = tmpcamera_z; camera_x = tmpcamera_x;
				 }
				 break;
	}
	case 'k': {
				  at_x -= vector_x*walkSpeed / vectorLength;
				  at_z -= vector_z*walkSpeed / vectorLength;
				  camera_x = at_x - vector_x;
				  camera_z = at_z - vector_z;
				  if ((camera_x>-1 && camera_x < map_size*block_size - 1) && (camera_z>-1 && camera_z<map_size * block_size - 1) && map[(map_size*int((1 + camera_z) / block_size) + int((1 + camera_x) / block_size))] == 1){
					  //roll back!
					  at_x = tmpat_x; at_z = tmpat_z;
					  camera_z = tmpcamera_z; camera_x = tmpcamera_x;
				  }
				  break;
	}
	case 'l': {
				  GLfloat tx, tz;
				  tx = -vector_z; tz = vector_x;
				  at_x += tx*walkSpeed / vectorLength;
				  at_z += tz* walkSpeed / vectorLength;
				  camera_x = at_x - vector_x;
				  camera_z = at_z - vector_z;
				  if ((camera_x>-1 && camera_x < map_size*block_size - 1) && (camera_z>-1 && camera_z<map_size * block_size - 1) && map[(map_size*int((1 + camera_z) / block_size) + int((1 + camera_x) / block_size))] == 1){
					  //roll back!
					  at_x = tmpat_x; at_z = tmpat_z;
					  camera_z = tmpcamera_z; camera_x = tmpcamera_x;
				  }
				  break;
	}
	case 'j':{
				 GLfloat tx, tz;
				 tx = vector_z; tz = -vector_x;
				 at_x += tx*walkSpeed / vectorLength;
				 at_z += tz* walkSpeed / vectorLength;
				 camera_x = at_x - vector_x;
				 camera_z = at_z - vector_z;
				 if ((camera_x>-1 && camera_x < map_size*block_size - 1) && (camera_z>-1 && camera_z<map_size * block_size - 1) && map[(map_size*int((1 + camera_z) / block_size) + int((1 + camera_x) / block_size))] == 1){
					 //roll back!
					 at_x = tmpat_x; at_z = tmpat_z;
					 camera_z = tmpcamera_z; camera_x = tmpcamera_x;
				 }
				 break;
	}
	case 'r':
		camera_x = 3, camera_y = 2, camera_z = 28;
		at_x = 3, at_y = 2, at_z = 0;
		upx = 0; upy = 1; upz = 0;
		break;
	case 27: exit(0); break;
	default: break;
	}
}