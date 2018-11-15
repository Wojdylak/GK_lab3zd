#define _USE_MATH_DEFINES

#include <windows.h>
#include <gl/gl.h>
#include <gl/glut.h>
#include <cmath>
#include <ctime>
#include <iostream>

/***********************
Typedefs
************************/

typedef float point3[3];

struct point {
	float x, y, z;
	float rColor, gColor, bColor;
};

/***********************
Function declarations
************************/

void MyMenu();

void RenderScene();

// Callback function for resizing render window
void ChangeSize(GLsizei horizontal, GLsizei vertical);

// Initializes additional pre-render settings
void MyInit();

void Axes(void);

void myTorus();

void spin();

void keys(unsigned char key, int x, int y);

// Funkcja odświerzająca widok po upływie czasu
void timer(int value);

float getDistanceTwoPoint(float x1, float y1, float x2, float y2);

float getDegrees(float x1, float y1, float x2, float y2);

/***********************
Constants
************************/

int model = 2;

const int liczbaN = 20;
point matrix[liczbaN][liczbaN];
const int scale = 4;
const int moveDown = 5;

static GLfloat theta[] = { 0.0, 0.0, 0.0 };
// Początkowy kąt kamery
float angle = 0;
// Funkcja określająca czas odświeżania widoku
const int refreshMills = 16;

float R=3, r=0.5;
float distanceBetweenCenterTorus = 2 * R - 3 * r;
float sizeWindow = (30 - R - 2 * r);
float scaleSin = sizeWindow / M_PI;

/***********************
Program entry point
************************/
int main()
{
	MyMenu();
	// Initializing renering settings
	MyInit();

	// Starting GLUT main loop
	glutMainLoop();

	// End of program
	return 0;
}


/***********************
Function definitions
************************/


void MyMenu()
{
	;
}

void RenderScene()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glLoadIdentity();
	// Czyszczenie macierzy bieżącej

	//glTranslatef(0, -0.3f, -1.5f);
	//glRotatef(30, 1, 0, 0);
	//glRotatef(60, 0, 1, 0);
	glRotatef(angle += 0.5f, 1, 0, 0);
//##########################################################
	Axes();

	glColor3f(1.0f, 1.0f, 1.0f); // Ustawienie koloru rysowania na biały

	myTorus();

	glFlush();
	// Przekazanie poleceń rysujących do wykonania

	glutSwapBuffers();
	//


}

void ChangeSize(GLsizei horizontal, GLsizei vertical)
{
	// Divide by 0 protection
	if (vertical == 0)
		vertical = 1;

	// Determining window's aspect ratio
	GLfloat AspectRatio = (GLfloat)horizontal / (GLfloat)vertical;

	// Setting Viewport size
	glViewport(0, 0, horizontal, vertical);

	// Reseting projection matrix
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

	// Setting projection matrix's bounaries to
	if (horizontal <= vertical)
		glOrtho(-30, 30, -30 / AspectRatio, 30 / AspectRatio, 30.0, -30.0);
	else
		glOrtho(-30 * AspectRatio, 30 * AspectRatio, -30, 30, 30.0, -30.0);

	// Reseting Model View matrix
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
}

void MyInit()
{
	// Setting up view mode
	// GLUT_SINGLE - single view buffer
	// GLUT_RGBA - RGB color model
	glutInitDisplayMode(GLUT_SINGLE | GLUT_RGBA | GLUT_DEPTH);

	// Creating window with secified title
	glutCreateWindow("Grafika komputerowa - lancuch");

	// Black background
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

	// GLUT rendering callback function
	glutDisplayFunc(RenderScene);

	// GLUT window resizing callback function
	glutReshapeFunc(ChangeSize);


	glEnable(GL_DEPTH_TEST);
	// Włączenie mechanizmu usuwania powierzchni niewidocznych

	//glutKeyboardFunc(keys);

	//glutIdleFunc(spin);

	glutTimerFunc(0, timer, 0);

	// Initializing randomness
	srand(time(NULL));
}

void Axes(void)
{

	point3  x_min = { -5.0, 0.0, 0.0 };
	point3  x_max = { 5.0, 0.0, 0.0 };
	// początek i koniec obrazu osi x

	point3  y_min = { 0.0, -5.0, 0.0 };
	point3  y_max = { 0.0,  5.0, 0.0 };
	// początek i koniec obrazu osi y

	point3  z_min = { 0.0, 0.0, -5.0 };
	point3  z_max = { 0.0, 0.0,  5.0 };
	//  początek i koniec obrazu osi y

	glColor3f(1.0f, 0.0f, 0.0f);  // kolor rysowania osi - czerwony
	glBegin(GL_LINES); // rysowanie osi x

	glVertex3fv(x_min);
	glVertex3fv(x_max);

	glEnd();

	glColor3f(0.0f, 1.0f, 0.0f);  // kolor rysowania - zielony
	glBegin(GL_LINES);  // rysowanie osi y

	glVertex3fv(y_min);
	glVertex3fv(y_max);

	glEnd();

	glColor3f(0.0f, 0.0f, 1.0f);  // kolor rysowania - niebieski
	glBegin(GL_LINES); // rysowanie osi z

	glVertex3fv(z_min);
	glVertex3fv(z_max);

	glEnd();

}



void myTorus()
{
	glColor3f(0.2f, 0.5f, 0.8f);

	float lastX = -sizeWindow, lastY = 0;
	float tmpX, tmpY;
	float tmp;

	glTranslated(-sizeWindow, 0, 0);
	//glBegin(GL_TRIANGLES);
	//glutWireTorus(r, R, 100, 100);
	//glEnd();

	bool flag = true;
	
	for (float i = -M_PI; i <= M_PI; i += 0.05)
	{
		tmpX = scaleSin * i;
		tmpY = sizeWindow * tan(i);

		tmp = getDistanceTwoPoint(lastX, lastY, tmpX, tmpY);
		if ( tmp > distanceBetweenCenterTorus)
		{
			if (flag)
			{
				glTranslated(tmpX - lastX, tmpY - lastY, 0);
				glRotated(90, 1, 0, 0);
				glRotated(getDegrees(lastX, lastY, tmpX, tmpY), 0, 1, 0);
				glBegin(GL_TRIANGLES);
				glutWireTorus(r, R, 100, 100);
				glEnd();
				glRotated(-getDegrees(lastX, lastY, tmpX, tmpY), 0, 1, 0);
			}
			else
			{
				glRotated(-90, 1, 0, 0);
				//glRotated(getDegrees(lastX, lastY, tmpX, tmpY), 0, 1, 0);
				glTranslated(tmpX - lastX, tmpY - lastY, 0);
				glBegin(GL_TRIANGLES);
				glutWireTorus(r, R, 100, 100);
				glEnd();
			}
			
			flag = !flag;
			lastX = tmpX;
			lastY = tmpY;
		}
	}
	

}


void spin()
{

}

void keys(unsigned char key, int x, int y)
{
	
}

void timer(int value) {
	glutPostRedisplay();
	glutTimerFunc(refreshMills, timer, 0);
}


float getDistanceTwoPoint(float x1, float y1, float x2, float y2)
{
	float x = x2 - x1;
	float y = y2 - y1;
	return sqrt(x*x + y*y);
}

float getDegrees(float x1, float y1, float x2, float y2)
{
	float x = x2 - x1;
	float y = y2 - y1;
	return atan(y / x) *180.0 / M_PI;
}