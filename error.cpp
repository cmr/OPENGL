//
//  main.cpp
//  error
//
//  Created by Alexander Macri on 1/13/14.
//  Copyright (c) 2014 Alexander Macri. All rights reserved.
//
//run command g++ error.cpp -lglut -lGL -lGLEW -g

#include "stdio.h"
#include "GL/glew.h"
#define GLFW_DLL
//#include "GLFW/glfw3.h"
//#include "GL/glfw.h"
#include "GL/freeglut.h"
#include "GL/gl.h"
#include "GL/glu.h"

#include <vector>
#include <stdio.h>
#include <iostream>
using namespace std;
int counter=0;



typedef struct{
  GLenum type;
  const char* filename;
} ShaderInfo;

//vertexIDs
GLuint voaID, vboID;
//vertices
GLfloat vertexarray[]={0.5f,-0.5f,0.0f,0.0f,0.5f,0.0f,-0.5f,-0.5f,0.0f};
//indices of triangle
GLubyte indices[3]={0,1,2};

const GLchar* inputShader(const char* filename){

  FILE* fshade = fopen(filename, "rb");
  
  if(!fshade){
    fprintf(stderr,"unable to open file '%s'\n",filename);
    return NULL;
  }
  
  fseek(fshade, 0, SEEK_END);
  long filesize=ftell(fshade);
  fseek(fshade, 0, SEEK_SET);
  
  GLchar* shadingSource= new GLchar[filesize+1];
  
  fread(shadingSource, 1, filesize, fshade);
  
  if(ftell(fshade) == 0){
    fprintf(stderr, "File '%s' is empty.\n",filename);
    return NULL;
  }

  fclose(fshade);
  
  shadingSource[filesize] = 0;
  
  return const_cast<const GLchar*>(shadingSource);
  
}

GLuint createShader(GLenum type, const GLchar* shadeSource){
  
  GLuint shader = glCreateShader(type);
  glShaderSource(shader, 1, &shadeSource, NULL);
  
  glCompileShader(shader);
  
  GLint compileStatus;
  glGetShaderiv(shader, GL_COMPILE_STATUS, &compileStatus);
  
  if(!compileStatus){
    GLint logSize;
    glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &logSize);
    
    GLchar* infoLog = new GLchar[logSize+1];
    glGetShaderInfoLog(shader,logSize,&logSize,infoLog);
    
    const char *shadeInfo= NULL;
    switch(type){
      case GL_VERTEX_SHADER: shadeInfo = "vertex"; break;
      case GL_GEOMETRY_SHADER_EXT: shadeInfo = "geometric"; break;
      case GL_FRAGMENT_SHADER: shadeInfo = "fragment"; break;
    }
    fprintf(stderr,"\nCompile failure in %u shader: %s\n Error message:\n%s\n",type,shadeInfo,infoLog);
    delete[] infoLog;
  }
  
  return shader;
}

GLuint createProgram(const vector<GLuint> shadeList){

  GLuint program = glCreateProgram();
  
  for(GLuint i=0;i<shadeList.size();i++){glAttachShader(program,shadeList[i]);}
  
  glBindAttribLocation(program, 0, "position");
  glLinkProgram(program);
  
  GLint linkStatus;
  glGetProgramiv(program, GL_LINK_STATUS, &linkStatus);
  
  if(!linkStatus){
    GLint logSize;
    glGetProgramiv(program, GL_INFO_LOG_LENGTH, &logSize);
    
    GLchar *infoLog = new GLchar[logSize+1];
    glGetProgramInfoLog(program,logSize,&logSize,infoLog);
    
    fprintf(stderr,"\nShader linking failed: %s\n",infoLog);
    delete[] infoLog;
    
    for(GLuint i=0;i<shadeList.size();i++){glDeleteShader(shadeList[i]);}
    return 0;
  }
  
  return program;
}

void initShaders(ShaderInfo* shaders){
  
  ShaderInfo* shade=shaders;
  vector<GLuint> Shadelist;
  
  while(shade->type != GL_NONE){
    Shadelist.push_back(createShader(shade->type,inputShader(shade->filename)));
    ++shade;
  }
  
  GLuint program=createProgram(Shadelist);
  
  glUseProgram(program);
}

void triangle1(){
cout << endl << "triangle1" << endl;
	glClear(GL_COLOR_BUFFER_BIT);
	glBegin(GL_TRIANGLES);
	glColor3f(1.0,0.0,0.0);
	glVertex3f(0.5,-0.5,0.0);
	glVertex3f(0.0,0.5,0.0);
	glVertex3f(-0.5,-0.5,0.0);
	glEnd();
	glFlush();
cout << endl << "triangle1 should have drawn" << endl;
}

void triangle2(){
  cout << endl << "triangle2" << endl;
  glClear(GL_COLOR_BUFFER_BIT);
  glColor3f(0.0,1.0,0.0);
  
  glGenVertexArrays(1, &voaID);
  glBindVertexArray(voaID);

  glGenBuffers(1, &vboID);
  glBindBuffer(GL_ARRAY_BUFFER, vboID);
  glBufferData(GL_ARRAY_BUFFER, sizeof(vertexarray), vertexarray, GL_STATIC_DRAW);

 ShaderInfo shaders[]={
  { GL_VERTEX_SHADER , "vertexshader.glsl"} ,
  { GL_FRAGMENT_SHADER , "fragmentshader.glsl"},
  { GL_NONE , NULL} 
  };

  initShaders(shaders);
  
  
  glEnableVertexAttribArray(0);
  glVertexAttribPointer(0,3,GL_FLOAT,GL_FALSE,0,(void*)0);

  glDrawArrays(GL_TRIANGLES, 0, 3);
  cout << endl << "triangle2 should have drawn" << endl;
  glFlush();
}

void triangle3(){
  cout << endl <<"triangle3" << endl;
  glClear(GL_COLOR_BUFFER_BIT);

  glGenVertexArrays(1, &voaID);
  glBindVertexArray(voaID);

  glGenBuffers(1, &vboID);
  glBindBuffer(GL_ARRAY_BUFFER, vboID);
  glBufferData(GL_ARRAY_BUFFER, sizeof(vertexarray), vertexarray, GL_STATIC_DRAW);

 ShaderInfo shaders[]={
  { GL_VERTEX_SHADER , "vertexshader.glsl"} ,
  { GL_FRAGMENT_SHADER , "fragmentshader.glsl"},
  { GL_NONE , NULL} 
  };

  initShaders(shaders);

  	  	
  glEnableVertexAttribArray(0);
  glVertexAttribPointer(0,3,GL_FLOAT,GL_FALSE,0,(void*)0);
  
  //glEnableClientState(GL_VERTEX_ARRAY);
  //glVertexPointer(3, GL_FLOAT, 0, (void *)0);
  
  glDrawElements(GL_TRIANGLES, 3, GL_UNSIGNED_BYTE, indices);
  cout << endl << "triangle3 should have drawn" << endl;
  glFlush();
}

void drawscene(){
  switch(counter%3){
    case 0:
      glutDisplayFunc(triangle1);
      glutPostRedisplay();
      break;
    case 1:
      glutDisplayFunc(triangle2);
      glutPostRedisplay();
      break;
    case 2:
    	glutDisplayFunc(triangle3);
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
  
  glewExperimental=GL_TRUE;
  if(glewInit()){
    fprintf(stderr, "Unable to initalize GLEW");
    exit(EXIT_FAILURE);
  }
  
  glutInitContextVersion(4, 3);
  glutInitContextProfile(GLUT_CORE_PROFILE|GLUT_COMPATIBILITY_PROFILE);

  const GLubyte* version=glGetString(GL_SHADING_LANGUAGE_VERSION);
  fprintf(stderr,"Opengl glsl version %s\n", version);

  version =glGetString(GL_VERSION);
  fprintf(stderr,"Opengl version %s\n", version);

  glutDisplayFunc(drawscene);
  glutMouseFunc(mousepress);
  glutMainLoop();
  return 0;
}
