
#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif

#include <iostream>
using namespace std;

int counter=0;

//OpenGL 3
void triangle1(){
  glClear(GL_COLOR_BUFFER_BIT);
  glBegin(GL_TRIANGLES);
  glColor3f(1.0, 0.0, 0.0);
	glVertex3f(0.5f, -0.5f, 0.0f);
	glVertex3f(0.0f, 0.5f, 0.0f);
	glVertex3f(-0.5f, -0.5f, 0.0f);
  glEnd();
  glFlush();
}

//Opengl 4
//vertexIDs
GLuint voaID, vbaID;
//vertices
GLfloat vertexarray[]={0.5f,-0.5f,0.0f,0.0f,0.5f,0.0f,-0.5f,-0.5f,0.0f};

void triangle2(){
  glClear(GL_COLOR_BUFFER_BIT);
  glColor3f(1.0, 0.0, 1.0);
  glGenVertexArraysAPPLE(1, &voaID);
	glBindVertexArrayAPPLE(voaID);
  
  glGenBuffers(1, &vbaID);
	glBindBuffer(GL_ARRAY_BUFFER, vbaID);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertexarray), vertexarray, GL_STATIC_DRAW);
  glEnableVertexAttribArray(0);
  glBindBuffer(GL_ARRAY_BUFFER, vbaID);
  glVertexAttribPointer(0,3,GL_FLOAT,GL_FALSE,0,(void*)0);
  glDrawArrays(GL_TRIANGLES, 0, 3);
  glFlush();
}

void drawscene(){
  switch(counter%2){
      case 0:
      glutDisplayFunc(triangle1);
      glutPostRedisplay();
      break;
      case 1:
      glutDisplayFunc(triangle2);
      glutPostRedisplay();
      break;
    	case 2:
      //glutDisplayFunc(triangle3);
      glutPostRedisplay();
      break;
  }
}

void mousepress(int button, int state, int x, int y){
  if(button==GLUT_RIGHT_BUTTON && state==GLUT_DOWN)
    exit(0);
  else if(button==GLUT_LEFT_BUTTON && state==GLUT_DOWN){
    counter++;
    drawscene();
  }
}

int main(int argc, char **argv)
{
  glutInit(&argc, argv);
	glutCreateWindow("SIMPLE");
  glutDisplayFunc(drawscene);
  glutMouseFunc(mousepress);
	glutMainLoop();
	return 0;
}

