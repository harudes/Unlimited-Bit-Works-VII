#define GLUT_DISABLE_ATEXIT_HACK
#include <windows.h>
#include <iostream>
#include <math.h>
#include<GL/glut.h>
#include <KDTree.h>
#include <stdlib.h>
#include <time.h>
#include <fstream>
#define KEY_ESC 27

using namespace std;


int mx = 0;
int my = 0;

float ax=0.0;
float ay=0.0;
float speed = 0.1;

vector<Point> points;
vector<Line> lines;

KDTree* kt;

//dibuja un simple gizmo

void pintar_puntos(){
    //glPointSize(5.0f);
    //glBegin(GL_POINTS);
    //cout<<points.size()<<endl;
    glColor3d(0.0f, 0.0f, 1.0f);
	for(unsigned int i=0;i<points.size();++i){
        //glVertex3d(points[i].val[0], points[i].val[1], points[i].val[2]);
        glPushMatrix();
        glTranslatef(points[i].val[0], points[i].val[1], points[i].val[2]);
        glutSolidSphere(0.3f,10,10);
        glPopMatrix();
	}
	//glEnd();

}

void pintar_lineas(){
    glBegin(GL_LINES);
	//cout<<lines.size()/4<<endl;
	for(unsigned int i=0;i<lines.size();++i){
        glColor3f(1.0, 1.0, 1.0);
        glVertex3d(lines[i].u.val[0], lines[i].u.val[1], lines[i].u.val[2]);
        glVertex3d(lines[i].v.val[0], lines[i].v.val[1], lines[i].v.val[2]);
	}
	glEnd();
	glLineWidth(2.0f);
}

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
	pintar_puntos();
	//pintar_lineas();

	glPolygonMode(GL_FRONT_AND_BACK,GL_LINE);
    //glutSolidTeapot(7);

    //glTranslatef(20,20,0);
    //glutSolidCube(10);
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
	auto start = high_resolution_clock::now();
    auto stop = high_resolution_clock::now();
    auto duration = duration_cast<milliseconds>(stop - start);
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

	kt = new KDTree(3,37500*15/100,0);


	for(int i=0;i<37500;++i){
        vector<coordenada> p={rand()%61-30,rand()%61-30,rand()%61-30};
        //start = high_resolution_clock::now();
        kt->insertPoint(p,kt);
        //stop = high_resolution_clock::now();
        //duration = duration_cast<milliseconds>(stop - start);
        //cout<< duration.count()<<endl;
	}


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
            vector<coordenada> p={x,y,z};
            temp.push_back(p);
        }
        figures[i]=temp;
    }

	KDTree* testKDTree;
    ofstream result("C:/Users/Luis/Desktop/results2.txt");
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
                    testKDTree = new KDTree(3,ms[k],0);
                else
                    testKDTree = new KDTree(3,ms[k]*figures[i].size()/100,0);
                start = high_resolution_clock::now();
                for(size_t t=0; t<figures[i].size()*percentaje[j]/100; ++t){
                    testKDTree->insertPoint(figures[i][t],testKDTree);
                }
                stop = high_resolution_clock::now();
                duration = duration_cast<milliseconds>(stop - start);
                result << "Creacion: " << duration.count()<<" ";
                start = high_resolution_clock::now();
                testKDTree->searchPoint(figures[i][200],testKDTree);
                stop = high_resolution_clock::now();
                auto duration2 = duration_cast<microseconds>(stop - start);
                result << "Busqueda: " << duration2.count()<<endl;
                delete(testKDTree);
            }
        }
    }*/

	//qt = new quadTree();
	///glutMainLoop(); //bucle de rendering
	//no escribir nada abajo de mainloop
	return 0;
}
