#define GLUT_DISABLE_ATEXIT_HACK
#include <windows.h>
#include <iostream>
#include <math.h>
#include <GL/glut.h>
#include <stdlib.h>
#include <time.h>
#include <QuadTree.h>
#include <fstream>
#include <thread>
#define KEY_ESC 27
#define PI 3.14159
#define radio 100


using namespace std;



//Crear quad tree
QuadTree *qt;
//RTree* qt;
vector<Line> lines;
vector<Point> points;
vector<Point> nearby;
bool cercanos=0;
int circleX, circleY,maxp;
int heigth,length;
//dibuja un simple gizmo

void graficar_circulo(int x, int y){
    glBegin(GL_LINES);
     for (int i = 0; i < 100 + 1; i++) {  // +1 para cerrar
         glVertex2f( x + radio * cos(2.0 * PI * i / 100),
                y + radio * sin(2.0 * PI * i / 100) );
    }
    glEnd();
}

void pintar_lineas(){
    glBegin(GL_LINES);
	lines=qt->getLines();
	//cout<<lines.size()/4<<endl;
	for(unsigned int i=0;i<lines.size();++i){
        glColor3f(1.0, 1.0, 1.0);
        glVertex2d(lines[i].u.getX(), lines[i].u.getY());
        glVertex2d(lines[i].v.getX(), lines[i].v.getY());
	}
	glEnd();
	glLineWidth(2.0f);
}

void pintar_puntos(){
    glBegin(GL_POINTS);
    points=qt->getPoints();
    //cout<<points.size()<<endl;
	for(unsigned int i=0;i<points.size();++i){
        //glColor3d(0, 0.5f, 0.5f);
        glColor3d(0,0,255);
        glVertex2d(points[i].getX(), points[i].getY());
	}
	glEnd();
	glPointSize(5.0f);
}

void resaltar_puntos(){
    glBegin(GL_POINTS);
    nearby=qt->cercanos(circleX,circleY,radio);
	for(unsigned int i=0;i<nearby.size();++i){
        glColor3d(255,0,0);
        glVertex2d(nearby[i].getX(), nearby[i].getY());
	}
	glEnd();
	glPointSize(5.0f);
}

void displayGizmo()
{
    if(cercanos)
        graficar_circulo(circleX, circleY);
	pintar_lineas();
	pintar_puntos();
	if(cercanos)
        resaltar_puntos();
}


void OnMouseClick(int button, int state, int x, int y)
{
    x-=length/2;
    y-=heigth/2;
    y*=-1;
  if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN){
    //convertir x,y
    Point p(x,y);
    //qt->insertPoint(p);
    qt->insertPoint(x,y,qt);
    //cout<<points.size()<<endl;
    //points.push_back(p);

    /*cout<<endl;
    qt->PrintTree();
    cout<<endl;*/
    //cout<<"Hay "<<points.size()<<" puntos"<<endl;
	//insertar un nuevo punto en el quadtree
  }
  if (button == GLUT_RIGHT_BUTTON && state == GLUT_DOWN){
    //float rad;
    //cin>>rad;
    cercanos=!cercanos;
  }
}

void OnMouseMotion(int x, int y)
{
    x-=length/2;
    y-=heigth/2;
    y*=-1;
    //nearby=qt->cercanos(x,y,radio);
    circleX=x;
    circleY=y;
     //opcional
	 //hacer algo x,z cuando se mueve el mouse
}



void idle(){ // AGREGAR ESTA FUNCION
	glutPostRedisplay();
}

//funcion llamada a cada imagen
void glPaint(void) {

	//El fondo de la escena al color initial
	glClear(GL_COLOR_BUFFER_BIT); //CAMBIO
	glLoadIdentity();
	glOrtho(-length/2,  length/2,-heigth/2, heigth/2, -1.0f, 1.0f);

	//dibujar quadTree (qt->draw())

	//dibuja el gizmo
	displayGizmo();

	//doble buffer, mantener esta instruccion al fin de la funcion
	glutSwapBuffers();
}

//
//inicializacion de OpenGL
//
void init_GL(void) {
	//Color del fondo de la escena
	//glClearColor(0.180f, 0.193f, 0.227f, 0.0f); //(R, G, B, transparencia) en este caso un fondo negro
    glClearColor(0, 0, 0, 0.0f);
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
	ifstream coordenadas("C:/Users/Luis/Desktop/Coordenadas.txt");
	glutInit(&argc, argv);
	/*string aux;
	getline(coordenadas,aux);
	cout<<aux<<endl;*/
	//cout<<"Ingrese las dimensiones"<<endl<<endl;
	//cout<<"Ingrese el ancho de la ventana: ";
	coordenadas>>length;
	//cout<<"Ingrese el alto de la ventana: ";
	coordenadas>>heigth;
	//cout<<"Ingrese la cantidad maxima de puntos por sector: ";
	coordenadas>>maxp;
	int pointnum;
	coordenadas>>pointnum;
	coordenada a,b;

	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
	glutInitWindowSize(length, heigth); //tamaño de la ventana
	glutInitWindowPosition(0, 0); //posicion de la ventana
	glutCreateWindow("TP2 bis OpenGL : Bresenham"); //titulo de la ventana

	init_GL(); //funcion de inicializacion de OpenGL

	glutDisplayFunc(glPaint);
	glutReshapeFunc(&window_redraw);
	// Callback del teclado
	glutKeyboardFunc(&window_key);
	glutMouseFunc(&OnMouseClick);
	glutPassiveMotionFunc(&OnMouseMotion);
	glutIdleFunc(&idle);
    qt=new QuadTree(-length/2,length/2,-heigth/2,heigth/2,maxp);
    //qt= new RTree(50);
    for(int i=0;i<pointnum;++i){
        coordenadas>>a>>b;
        //cout<<a<<" "<<b<<" "<<i<<endl;
        a*=heigth;
        b*=length;
        //cout<<a<<" "<<b<<endl;
        Point aux(b,a);
        qt->insertPoint(b,a,qt);
        points.push_back(aux);
    }
	glutMainLoop(); //bucle de rendering
	//no escribir nada abajo de mainloop
	return 0;
}
