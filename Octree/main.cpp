#define GLUT_DISABLE_ATEXIT_HACK
#include <windows.h>
#include <iostream>
#include <math.h>
#include <GL/glut.h>
#include "OcTree.h"
#include <stdlib.h>
#include <time.h>
//#include <quadTree.h>
#define KEY_ESC 27

using namespace std;

int mx = 0;
int my = 0;

float ax=0.0;
float ay=0.0;
float speed = 0.1;

vector<Line> lines;
vector<Point> points;

OcTree* ot;

//dibuja un simple gizmo
void displayGizmo()
{
	glBegin(GL_LINES);
        glColor3d(255,0,0);
        glVertex3d(-30, 0,0);
        glVertex3d(30, 0,0);
        glColor3d(0, 255, 0);
        glVertex3d(0, -30,0);
        glVertex3d(0, 30,0);
        glColor3d(0, 0, 255);
        glVertex3d(0, 0,-30);
        glVertex3d(0, 0,30);
	glEnd();
}

void pintar_lineas(){
    glLineWidth(2.0f);
    glBegin(GL_LINES);
	lines=ot->getLines();
	//glColor3d(255, 255, 255);
	for(unsigned int i=0;i<lines.size();++i){
        if(i%3==0)
            glColor3d(255, 0, 0);
        if(i%3==1)
            glColor3d(0, 255, 0);
        if(i%3==2)
            glColor3d(0, 0, 255);
        glVertex3d(lines[i].u.x, lines[i].u.y, lines[i].u.z);
        glVertex3d(lines[i].v.x, lines[i].v.y, lines[i].v.z);
	}
	glEnd();
}

void pintar_lineas2(){
    glLineWidth(2.0f);
    glBegin(GL_LINES);
	lines=ot->getPointLines();
	glColor3d(0, 0, 255);
	for(unsigned int i=0;i<lines.size();++i){

        glVertex3d(lines[i].u.x, lines[i].u.y, lines[i].u.z);
        glVertex3d(lines[i].v.x, lines[i].v.y, lines[i].v.z);
	}
	glEnd();
}

void pintar_puntos(){
    glPointSize(5.0f);
    glBegin(GL_POINTS);
    points=ot->getPoints();
    glColor3d(0,0,255);
	for(unsigned int i=0;i<points.size();++i){
        glVertex3d(points[i].x, points[i].y, points[i].z);
	}
	glEnd();

}
bool r = false;

void OnMouseClick(int button, int state, int x, int y)
{
  if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN)
  {
    mx=x;
    my=y;
  }else if(button == GLUT_RIGHT_BUTTON && state == GLUT_DOWN){
    r = true;
  }else if(button == GLUT_RIGHT_BUTTON && state == GLUT_UP){
    r = false;
  }
}

void OnMouseMotion(int x, int y)
{
    int dx = mx-x;
    int dy = my-y;

    mx=x;
    my=y;

    ax += dx*speed;
    ay += dy*speed;
}



void idle(){ // AGREGAR ESTA FUNCION
	glutPostRedisplay();
}

//funcion llamada a cada imagen
void glPaint(void) {

	//El fondo de la escena al color initial
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); //CAMBIO
	glLoadIdentity();
	gluPerspective(45.0,1.0,1.0,500.0);

	glTranslatef(0,0,-100.0);
	glRotatef(ax,0,1,0);
	glRotatef(ay,1,0,0);

	displayGizmo();

	pintar_lineas();
	//pintar_lineas2();
	pintar_puntos();

	glPolygonMode(GL_FRONT_AND_BACK,GL_LINE);

    glTranslatef(20,20,0);
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

	default:
		break;
	}

}
//
//el programa principal
//
int main(int argc, char** argv) {

	//Inicializacion de la GLUT
	srand(time(NULL));
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
	glutInitWindowSize(600, 600); //tamaño de la ventana
	glutInitWindowPosition(100, 100); //posicion de la ventana
	glutCreateWindow("QuadTree"); //titulo de la ventana

	init_GL(); //funcion de inicializacion de OpenGL

	glutDisplayFunc(glPaint);
	glutReshapeFunc(&window_redraw);
	// Callback del teclado
	glutKeyboardFunc(&window_key);
	glutMouseFunc(&OnMouseClick);
	glutMotionFunc(&OnMouseMotion);
	glutIdleFunc(&idle);
	ot=new OcTree(-30,30,-30,30,-30,30,2);
    for(int i=0;i<10;++i){
        ot->insertPoint(Point(rand()%61-30,rand()%61-30,rand()%61-30),ot);
    }

	//qt = new quadTree();
	glutMainLoop(); //bucle de rendering
	//no escribir nada abajo de mainloop
	return 0;
}
