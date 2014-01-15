//
//  Created by Alexander Macri on 1/9/14.
//  Copyright (c) 2014 Alexander Macri. All rights reserved.
//
#include "GL/glew.h"
#include "freeglut.h"
#include <OpenGL/OpenGL.h>
#include <iostream>
#include <vector>
#include <string>
using namespace std;

typedef struct{
  GLenum type;
  const char* filename;
} ShaderInfo;

GLuint voaID, vbaID;
GLfloat vertexarray[]={0.5f,-0.5f,0.0f,0.0f,0.5f,0.0f,-0.5f,-0.5f,0.0f};

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


void init(){
  glGenVertexArraysAPPLE(1, &voaID);
	glBindVertexArrayAPPLE(voaID);
  
  glGenBuffers(1, &vbaID);
	glBindBuffer(GL_ARRAY_BUFFER, vbaID);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertexarray), vertexarray, GL_STATIC_DRAW);
  
  ShaderInfo shaders[]={
  { GL_VERTEX_SHADER , "vertexshader.glsl"} ,
  { GL_FRAGMENT_SHADER , "fragmentshader.glsl"},
  { GL_NONE , NULL} 
  };
  
  initShaders(shaders);
  
  glEnableVertexAttribArray(0);
  glVertexAttribPointer(0,3,GL_FLOAT,GL_FALSE,0,(void*)0);
  
}

void display(){
  glClear(GL_COLOR_BUFFER_BIT);
  glDrawArrays(GL_TRIANGLES, 0, 3);
  glFlush();
}

void mousepress(int button, int state, int x, int y){
  if(button==GLUT_RIGHT_BUTTON && state==GLUT_DOWN) exit(0);
}

void idle(void){
  glutPostRedisplay();
}

int main(int argc, char **argv){
   glutInit(&argc, argv);
   /*glutInitContextVersion(4, 1);
  glutInitContextProfile(GLUT_CORE_PROFILE | GLUT_COMPATIBILITY_PROFILE);
 
  const GLubyte* version1 = glGetString(GL_SHADING_LANGUAGE_VERSION);
  fprintf (stderr, "GLSL Version: %s\n", version1);
  
  version1 = glGetString(GL_VERSION);
  fprintf (stderr, "openGL Version: %s\n", version1);
  
  version1 = glGetString(GL_EXTENSIONS);
  fprintf (stderr, "openGL Extenstions: %s\n", version1);
  */
  CGLContextObj ctx;
  CGLPixelFormatObj pix;
  GLint npix;
  
  int attribs[] = {
    (int)kCGLPFAOpenGLProfile,  (int)kCGLOGLPVersion_GL4_Core, 0
  };
  
  CGLChoosePixelFormat((CGLPixelFormatAttribute *)attribs, &pix, &npix);
  CGLCreateContext(pix, NULL, &ctx);
  CGLSetCurrentContext(ctx);
  
  const GLubyte* version2 = glGetString(GL_SHADING_LANGUAGE_VERSION);
  fprintf (stderr, "GLSL Version: %s\n", version2);
  
  version2 = glGetString(GL_VERSION);
  fprintf (stderr, "openGL Version: %s\n", version2);
  
  version2 = glGetString(GL_EXTENSIONS);
  fprintf (stderr, "openGL Extenstions: %s\n", version2);
  glutInitContextProfile(GLUT_CORE_PROFILE | GLUT_COMPATIBILITY_PROFILE);
  
  glutCreateWindow("Shaders");
  glutMouseFunc(mousepress);
  
  if(glewInit()){
    fprintf(stderr, "Unable to initalize GLEW");
    exit(EXIT_FAILURE);
  }
  
  init();
  
  glutDisplayFunc(display);
	glutMainLoop();
	return 0;

}

