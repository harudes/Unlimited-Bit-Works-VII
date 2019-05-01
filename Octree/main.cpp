#define GLUT_DISABLE_ATEXIT_HACK
#include <windows.h>
#include <iostream>
#include <math.h>
#include <GL/glut.h>
#include "OcTree.h"
#include <stdlib.h>
#include <time.h>
#include <chrono>
#include <fstream>
//#include <quadTree.h>
#define KEY_ESC 27

using namespace std;
using namespace std::chrono;

int mx = 0;
int my = 0;

float ax=0.0;
float ay=0.0;
float speed = 0.1;

float sphereX=0.0,sphereY=0.0,sphereZ=0.0,sphereRadius=15.0;
float moveX,moveY,moveZ;

vector<Line> lines;
vector<Line> lines2;
vector<Point> points;
vector<Point> points2;

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
	glColor3d(1.0f, 1.0f, 1.0f);
	for(unsigned int i=0;i<lines2.size();++i){
        glVertex3d(lines2[i].u.x, lines2[i].u.y, lines2[i].u.z);
        glVertex3d(lines2[i].v.x, lines2[i].v.y, lines2[i].v.z);
	}
	glEnd();
}

void pintar_puntos(){
    /*glPointSize(5.0f);
    glBegin(GL_POINTS);
        glColor3d(0.0,0.0,1.0);
        for(unsigned int i=0;i<points.size();++i){
            glVertex3d(points[i].x, points[i].y, points[i].z);
        }
	glEnd();*/
	glColor3d(0.0,0.0,1.0);
	for(size_t i=0; i<points.size();++i){
        glPushMatrix();
            glTranslatef(points[i].x, points[i].y, points[i].z);
            glutSolidCube(0.5);
        glPopMatrix();
	}
}

void resaltar_puntos(){
    points2=ot->cercanos(Point(sphereX,sphereY,sphereZ),sphereRadius);
    /*glPointSize(5.0f);
    glBegin(GL_POINTS);
        glColor3d(1.0,0.0,0.0);
        for(unsigned int i=0;i<points2.size();++i){
            glVertex3d(points2[i].x, points2[i].y, points2[i].z);
        }
	glEnd();*/
	glColor3d(1.0,0.0,0.0);
	for(size_t i=0; i<points2.size();++i){
        glPushMatrix();
            glTranslatef(points2[i].x, points2[i].y, points2[i].z);
            glutSolidCube(0.5);
        glPopMatrix();
	}
}

void pintar_esfera(){
    if(sphereX+sphereRadius>30||sphereX-sphereRadius<-30){
        moveX*=-1;
    }
    if(sphereY+sphereRadius>30||sphereY-sphereRadius<-30){
        moveY*=-1;
    }
    if(sphereZ+sphereRadius>30||sphereZ-sphereRadius<-30){
        moveZ*=-1;
    }
    /*sphereX+=moveX;
	sphereY+=moveY;
	sphereZ+=moveZ;*/
    glPushMatrix();
        glColor3d(1.0,0.0,0.0);
        glTranslatef(sphereX,sphereY,sphereZ);
        glutSolidSphere(sphereRadius,10,10);
    glPopMatrix();
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

	//displayGizmo();

	//pintar_lineas();
	pintar_lineas2();
	//resaltar_puntos();
	pintar_puntos();
	//pintar_esfera();


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
	auto start = high_resolution_clock::now();
    auto stop = high_resolution_clock::now();
    auto duration = duration_cast<milliseconds>(stop - start);

	moveX=rand()/double(RAND_MAX)/1;
	moveY=rand()/double(RAND_MAX)/1;
	moveZ=rand()/double(RAND_MAX)/1;

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


    ot=new OcTree(-30,30,-30,30,-30,30,37500*15/100);
    for(int i=0;i<37500;++i){
        //ot->insertPoint(Point(rand()%61-30,rand()%61-30,rand()%61-30),ot);
        start = high_resolution_clock::now();
        ot->insertPoint(Point(((rand()/double(RAND_MAX))-0.5)*60,((rand()/double(RAND_MAX))-0.5)*60,((rand()/double(RAND_MAX))-0.5)*60),ot);
        stop = high_resolution_clock::now();
        duration = duration_cast<milliseconds>(stop - start);
        cout<< duration.count()<<endl;
    }

    lines2=ot->getBorderLines();
    points=ot->getPoints();
    lines=ot->getLines();
    points2=ot->cercanos(Point(sphereX,sphereY,sphereZ),sphereRadius);



    ///Insertando obj's
    /*vector<vector<Point> > figures(5);
    vector<string> names={"Skyscrapper","Cessna","Airboat","Trumpet","Bunny"};
    vector<int> percentaje={25,50,75,100};
    vector<int> ms={1,10,20,5,10,15};
    int tam;
	coordenada x,y,z;
	vector<Point> temp;
    for(int i=0; i<5; ++i){
        temp.erase(temp.begin(),temp.end());
        cin>>tam;
        for(int i=0; i<tam; ++i){
            cin>>x>>y>>z;
            temp.push_back(Point(x,y,z));
        }
        figures[i]=temp;
    }


    OcTree* testOctree;
    ofstream result("C:/Users/Luis/Desktop/results.txt");
    for(size_t i=0; i<figures.size(); ++i){
        result<<names[i]<<endl;
        for(size_t j=0; j<percentaje.size(); ++j){
            result<<percentaje[j]<<"% de los puntos, "<<endl;
            for(size_t k=0; k<ms.size(); ++k){
                result<<"m= "<<ms[k];
                if(k>=3)
                    result<<"%";
                result<<", ";
                if(k<3)
                    testOctree = new OcTree(0,1,0,1,0,1,ms[k]);
                else
                    testOctree = new OcTree(0,1,0,1,0,1,ms[k]*figures[i].size()/100);
                start = high_resolution_clock::now();
                for(size_t t=0; t<figures[i].size()*percentaje[j]/100; ++t){
                    testOctree->insertPoint(figures[i][t],testOctree);
                }
                stop = high_resolution_clock::now();
                duration = duration_cast<milliseconds>(stop - start);
                result << "Creacion: " << duration.count()<<" ";
                start = high_resolution_clock::now();
                testOctree->searchPoint(figures[i][200],testOctree);
                stop = high_resolution_clock::now();
                auto duration2 = duration_cast<microseconds>(stop - start);
                result << "Busqueda: " << duration2.count()<<endl;
                delete(testOctree);
            }
        }
    }*/

    /*ot=new OcTree(-30,30,-30,30,-30,30,10);
    int tam;
    coordenada a,b,c,d,e;
    cin>>tam;
    for(int i=0; i<tam; ++i){
        cin>>a>>b>>c>>d>>e;
        ot->insertPoint(Point(a*150,b*150,c*150),ot);
    }
    points=ot->getPoints();
    lines2=ot->getBorderLines();*/

	//qt = new quadTree();
	//glutMainLoop(); //bucle de rendering
	//no escribir nada abajo de mainloop
	return 0;
}
