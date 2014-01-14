//
//  main.cpp
//  error
//
//  Created by Alexander Macri on 1/13/14.
//  Copyright (c) 2014 Alexander Macri. All rights reserved.
//
#include "GL/glew.h"
#define GLFW_DLL
#include "GLFW/glfw3.h"
#include "GL/freeglut.h"
#include "GL/gl.h"
#include "GL/glu.h"
//#include <OpenGL/gl.h>
//#include <OpenGL/glu.h>

#include <iostream>
using namespace std;
int counter=0;

//Opengl 4
//vertexIDs
GLuint voaID, vboID;
//vertices
GLfloat vertexarray[]={1.f,-1.f,0.0f,0.0f,1.f,0.0f,-1.f,-1.0f,0.0f};
GLfloat colorarray[]={1.0f,0.0f,1.0f};
//indices of triangle
GLubyte indices[3]={0,1,2};

void triangle1(){
  cout << endl << "triangle1" << endl;
  glClear(GL_COLOR_BUFFER_BIT);
  
  glGenVertexArraysAPPLE(1, &voaID);
	glBindVertexArrayAPPLE(voaID);
  
  glGenBuffers(1, &vboID);

	glBindBuffer(GL_ARRAY_BUFFER, vboID);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertexarray), vertexarray, GL_STATIC_DRAW);
  
  glEnableVertexAttribArray(0);
  glVertexAttribPointer(0,3,GL_FLOAT,GL_FALSE,0,(void*)0);
  
  glDrawArrays(GL_TRIANGLES, 0, 3);
  cout << endl << "triangle1 should have drawn" << endl;
  glFlush();
}

void triangle2(){
  cout << endl <<"triangle2" << endl;
  glClear(GL_COLOR_BUFFER_BIT);
  
  glGenVertexArraysAPPLE(1, &voaID);
	glBindVertexArrayAPPLE(voaID);
  
  glGenBuffers(1, &vboID);
	glBindBuffer(GL_ARRAY_BUFFER, vboID);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertexarray), vertexarray, GL_STATIC_DRAW);
  
  glEnableVertexAttribArray(0);
  glVertexAttribPointer(0,3,GL_FLOAT,GL_FALSE,0,(void*)0);
	glDrawElements(GL_TRIANGLES, 3, GL_UNSIGNED_BYTE, indices);
  cout << endl << "triangle2 should have drawn" << endl;
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

void idle(void){
  glutPostRedisplay();
}

int main(int argc, char **argv){
  glutInit(&argc, argv);
  glutCreateWindow("Shapes");
  
  if(!glfwInit()){
    fprintf (stderr, "ERROR: could not start GLFW3\n");
    exit(EXIT_FAILURE);
  }
  if(glewInit()){
    fprintf(stderr, "Unable to initalize GLEW");
    exit(EXIT_FAILURE);
  }
  glutInitContextVersion(4, 2);
  glutInitContextProfile(GLUT_CORE_PROFILE);
  //glutIdleFunc(idle);
  glutDisplayFunc(drawscene);
  glutMouseFunc(mousepress);
 	glutMainLoop();
	return 0;
}