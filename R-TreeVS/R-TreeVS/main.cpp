#define GLUT_DISABLE_ATEXIT_HACK
#include <windows.h>
#include <iostream>
#include <math.h>
#include <GL/glut.h>
#include <time.h>
#include "RTree.h"
#include <chrono>
#define KEY_ESC 27

using namespace std;

using namespace chrono;

int mx = 0;
int my = 0;

float ax = 0.0;
float ay = 0.0;
float speed = 0.1;

float sphereX = 0.0, sphereY = 0.0, sphereZ = 0.0, sphereRadius = 15.0;
float moveX, moveY, moveZ;

bool showSphere = true;
bool autoMove = false;
bool method = true;
bool showLines = true;

RTree* rt;

vector<Line> lines;
vector<Line> lines2;
vector<Point> points;///todos los puntos
vector<Point> points2;///puntos resaltados por la busqueda

vector<Point> cercanos(Point p, float radius) {
	vector<Point> result;
	for (size_t i = 0, top = points.size(); i<top; ++i) {
		if (euclidean(p, points[i]) < radius)
			result.push_back(points[i]);
	}
	return result;
}

void drawPoints() {
	glPointSize(5.0f);
	glBegin(GL_POINTS);
	glColor3d(0.0, 0.0, 1.0);
	for (unsigned int i = 0; i < points.size(); ++i) {
		glVertex3d(points[i].val[0], points[i].val[1], points[i].val[2]);
	}
	glEnd();

	/*glColor3d(0.0, 0.0, 1.0);
	for (size_t i = 0; i < points.size(); ++i) {
		glPushMatrix();
		glTranslatef(points[i].val[0], points[i].val[1], points[i].val[2]);
		glutSolidCube(0.5);
		glPopMatrix();
	}*/
}

void drawPoints2() {
	glPointSize(5.0f);
	glBegin(GL_POINTS);
	glColor3d(1.0, 0.0, 0.0);
	for (unsigned int i = 0; i < points2.size(); ++i) {
		glVertex3d(points2[i].val[0], points2[i].val[1], points2[i].val[2]);
	}
	glEnd();

	/*glColor3d(1.0, 0.0, 0.0);
	for (size_t i = 0; i < points2.size(); ++i) {
		glPushMatrix();
		glTranslatef(points2[i].val[0], points2[i].val[1], points2[i].val[2]);
		glutSolidCube(0.5);
		glPopMatrix();
	}*/
}

void drawLines() {
	glLineWidth(2.0f);
	glBegin(GL_LINES);
	glColor3d(1.0f, 1.0f, 1.0f);
	for (unsigned int i = 0; i < lines.size(); ++i) {
		glVertex3d(lines[i].u.val[0], lines[i].u.val[1], lines[i].u.val[2]);
		glVertex3d(lines[i].v.val[0], lines[i].v.val[1], lines[i].v.val[2]);
	}
	glEnd();
}

void drawLines2() {
	glLineWidth(2.0f);
	glBegin(GL_LINES);
	glColor3d(0.0f, 1.0f, 0.0f);
	for (unsigned int i = 0; i < lines2.size(); ++i) {
		glVertex3d(lines2[i].u.val[0], lines2[i].u.val[1], lines2[i].u.val[2]);
		glVertex3d(lines2[i].v.val[0], lines2[i].v.val[1], lines2[i].v.val[2]);
	}
	glEnd();
}

void drawSphere() {

	if (sphereX + sphereRadius > 30 || sphereX - sphereRadius < -30) {
		moveX *= -1;
	}
	if (sphereY + sphereRadius > 30 || sphereY - sphereRadius < -30) {
		moveY *= -1;
	}
	if (sphereZ + sphereRadius > 30 || sphereZ - sphereRadius < -30) {
		moveZ *= -1;
	}

	if (autoMove) {
		sphereX += moveX;
		sphereY += moveY;
		sphereZ += moveZ;
	}

	glPushMatrix();
	glTranslatef(sphereX,sphereY,sphereZ);
	glColor3f(1.0f,0.0f,0.0f);
	glutSolidSphere(sphereRadius,10,10);
	glPopMatrix();
}

//dibuja un simple gizmo
void displayGizmo()
{
	glBegin(GL_LINES);
	glColor3d(255, 0, 0);
	glVertex3d(-30, 0, 0);
	glVertex3d(30, 0, 0);
	glColor3d(0, 255, 0);
	glVertex3d(0, -30, 0);
	glVertex3d(0, 30, 0);
	glColor3d(0, 0, 255);
	glVertex3d(0, 0, -30);
	glVertex3d(0, 0, 30);
	glEnd();
}
bool r = false;

void OnMouseClick(int button, int state, int x, int y)
{
	if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN)
	{
		mx = x;
		my = y;
	}
	else if (button == GLUT_RIGHT_BUTTON && state == GLUT_DOWN) {
		r = true;
	}
	else if (button == GLUT_RIGHT_BUTTON && state == GLUT_UP) {
		r = false;
	}
}

void OnMouseMotion(int x, int y)
{
	int dx = mx - x;
	int dy = my - y;

	mx = x;
	my = y;

	ax += dx * speed;
	ay += dy * speed;
}



void idle() { // AGREGAR ESTA FUNCION
	glutPostRedisplay();
}

//funcion llamada a cada imagen
void glPaint(void) {

	//El fondo de la escena al color initial
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); //CAMBIO
	glLoadIdentity();
	gluPerspective(45.0, 1.0, 1.0, 500.0);

	glTranslatef(0, 0, -100.0);
	glRotatef(ax, 0, 1, 0);
	glRotatef(ay, 1, 0, 0);

	displayGizmo();

	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	if (showLines) {
		drawLines();
		drawLines2();
	}
	
	if (showSphere) {
		vector<coordenada> p = { sphereX,sphereY,sphereZ };
		if (method) {
			points2 = rt->cercanos(Point(p), sphereRadius);
		}
		else {
			points2 = cercanos(Point(p), sphereRadius);
		}
		drawPoints2();
	}
	drawPoints();
	
	if(showSphere)
		drawSphere();
	//dibujar quadTree (qt->draw())

	/*glBegin(GL_POINTS);
	for()
	glVertex2f(v.at(i)[0],v.at(i)[1])
	glEnd();*/
	//dibuja el gizmo

	//doble buffer, mantener esta instruccion al fin de la funcion
	glutSwapBuffers();
}

//
//inicializacion de OpenGL
//
void init_GL(void) {
	//Color del fondo de la escena
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f); //(R, G, B, transparencia) en este caso un fondo negro
	glEnable(GL_DEPTH_TEST);
	//modo projeccion
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
}

//en el caso que la ventana cambie de tamaño
GLvoid window_redraw(GLsizei width, GLsizei height) {
	glViewport(0, 0, width, height);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();


}

GLvoid window_key(unsigned char key, int x, int y) {
	
	switch (key) {
	case KEY_ESC:
		exit(0);
		break;
	case 'p': {
		vector<coordenada> p;
		for (int i = 0; i < 3; ++i) {
			p.push_back(rand() / double(RAND_MAX) * 60 - 30);
		}
		rt->insertPoint(p, rt);
		break;
	}
	case 'w': {
		sphereY += moveY;
		break;
	}
	case 's': {
		sphereY -= moveY;
		break;
	}
	case 'a': {
		sphereX += moveX;
		break;
	}
	case 'd': {
		sphereX -= moveX;
		break;
	}
	case '+': {
		sphereZ += moveZ;
		break;
	}
	case '-': {
		sphereZ -= moveZ;
		break;
	}
	case 'c': {
		moveX = rand() / double(RAND_MAX) / 1;
		moveY = rand() / double(RAND_MAX) / 1;
		moveZ = rand() / double(RAND_MAX) / 1;
		autoMove = !autoMove;
		break;
	}
	case 'o': {
		showSphere = !showSphere;
		break;
	}
	case 'm': {
		method = !method;
		break;
	}
	case 'l': {
		showLines = !showLines;
		break;
	}
	default:
		break;
	}
	points = rt->getPoints();
	vector<coordenada> p = { sphereX,sphereY,sphereZ };
	if (method) {
		points2 = rt->cercanos(Point(p), sphereRadius);
	}
	else {
		points2 = cercanos(Point(p), sphereRadius);
	}
	lines = rt->getLines();
	lines2 = rt->getLines2();
}
int main(int argc, char** argv) {

	//Inicializacion de la GLUT
	srand(time(NULL));
	auto start = high_resolution_clock::now();
	auto stop = high_resolution_clock::now();
	auto duration = duration_cast<microseconds>(stop - start);
	moveX = rand() / double(RAND_MAX)/1;
	moveY = rand() / double(RAND_MAX)/1;
	moveZ = rand() / double(RAND_MAX)/1;
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
	glutInitWindowSize(600, 600); //tamaño de la ventana
	glutInitWindowPosition(100, 100); //posicion de la ventana
	glutCreateWindow("TP2 bis OpenGL : Bresenham"); //titulo de la ventana

	init_GL(); //funcion de inicializacion de OpenGL

	glutDisplayFunc(glPaint);
	glutReshapeFunc(&window_redraw);
	// Callback del teclado
	glutKeyboardFunc(&window_key);
	glutMouseFunc(&OnMouseClick);
	glutMotionFunc(&OnMouseMotion);
	glutIdleFunc(&idle);

	int maxP, initialP;
	cout << "Ingrese la cantidad de puntos por region: ";
	cin >> maxP;
	cout << "Ingrese la cantidad de puntos iniciales: ";
	cin >> initialP;

	rt = new RTree(3,50);
	for (int i = 0; i < 1000; ++i) {
		vector<coordenada> val;
		for (int i = 0; i < 3; ++i) {
			val.push_back(rand() / double(RAND_MAX) * 60 - 30);
		}
		//start = high_resolution_clock::now();
		rt->insertPoint(Point(val),rt);
		//stop = high_resolution_clock::now();
		//duration = duration_cast<microseconds>(stop - start);
		//cout << duration.count() << endl;
	}
	points = rt->getPoints();
	lines = rt->getLines();
	lines2 = rt->getLines2();
	glutMainLoop(); //bucle de rendering
	//no escribir nada abajo de mainloop
	return 0;
}
