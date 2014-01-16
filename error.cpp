//
//
//run command g++ error.cpp -lglut -lGL -lGLEW -g

#include "stdio.h"
#include "GL/glew.h"
//#define GLFW_DLL
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


//function prototypes
GLuint createShader(GLenum type, const GLchar* shadeSource);
const GLchar* inputShader(const char* filename);
GLuint createProgram(const vector<GLuint> shadeList);


//the info for loading and linking shaders
typedef struct{
  GLenum type;// GL_VERTEX_SHADER or GL_FRAGMENT_SHADER
  const char* filename;//name of file to input
} ShaderInfo;

//vertexIDs
GLuint vaoID, vboID;// the buffers that are going to be linked too
//vertices
GLfloat vertexarray[]={0.5f,-0.5f,0.0f,0.0f,0.5f,0.0f,-0.5f,-0.5f,0.0f};// vertices that are drawn x,y,z ...
//indices of triangle
GLubyte indices[3]={0,1,2};

//create the shaders for your program
void initShaders(ShaderInfo* shaders){
  
  ShaderInfo* shade=shaders;
  vector<GLuint> Shadelist;//initialize list of shaders
  
  while(shade->type != GL_NONE){//loop through all the shaders in the list
    Shadelist.push_back(createShader(shade->type,inputShader(shade->filename)));//adding shaders into the list
    ++shade;//incrementation
  }
  
  GLuint program=createProgram(Shadelist);//creates the program linking to all the shaders
  
  glUseProgram(program);//installs a program object as part of current rendering state
}

//this funtion loads the shader from the vertex, fragments shaders 
const GLchar* inputShader(const char* filename){

  FILE* fshade = fopen(filename, "rb");//opens file
  
  if(!fshade){//check to see if file is opened
    fprintf(stderr,"unable to open file '%s'\n",filename);
    return NULL;
  }
  
  //neat way to get the length of the file
  fseek(fshade, 0, SEEK_END);
  long filesize=ftell(fshade);
  fseek(fshade, 0, SEEK_SET);
  
  
  //allocates memory for the file and read in the file 
  GLchar* shadingSource= new GLchar[filesize+1];//
  fread(shadingSource, 1, filesize, fshade);
  
  
  if(ftell(fshade) == 0){//checks to see if the file is empty
    fprintf(stderr, "File '%s' is empty.\n",filename);
    return NULL;
  }

  fclose(fshade);//closes file
  
  shadingSource[filesize] = 0;//neat way to set a '\0' at end of file
  
  return const_cast<const GLchar*>(shadingSource);//overloads the const so the value with change per file  
  
  //NOTE: if the file is unable to open or is empty this function will segmentation fault your program
}

//this function create your shader
GLuint createShader(GLenum type, const GLchar* shadeSource){
  
  GLuint shader = glCreateShader(type);//create shader based on type GL_VERTEX_SHADER or GL_FRAGMENT_SHADER
  glShaderSource(shader, 1, &shadeSource, NULL);//loads the source code of the file into the shader
  
  glCompileShader(shader);//compiles a shader object
  
  GLint compileStatus;//status of the compilation variable
  glGetShaderiv(shader, GL_COMPILE_STATUS, &compileStatus);//returns the comiple status into the variable
  
  if(!compileStatus){//checks to see if the shader compiled
    GLint logSize;//variable for size of the debug info
    glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &logSize);//returns the size of the the source file into the variable
    
    GLchar* infoLog = new GLchar[logSize+1];//allocating memory for the debug info
    glGetShaderInfoLog(shader,logSize,&logSize,infoLog);//returns the error messages into the variable infoLog
    
    const char *shadeInfo= NULL;//char array for what shader that is having an error
    switch(type){//way to get what shader has the error
      case GL_VERTEX_SHADER: shadeInfo = "vertex"; break;
      case GL_GEOMETRY_SHADER_EXT: shadeInfo = "geometric"; break;
      case GL_FRAGMENT_SHADER: shadeInfo = "fragment"; break;
    }
    fprintf(stderr,"\nCompile failure in %u shader: %s\n Error message:\n%s\n",type,shadeInfo,infoLog);//prints information need to debug shaders
    delete[] infoLog;//memory management
  }
  return shader;//self explanatory
}

//this function creates the shading program we are going to link the shader too
GLuint createProgram(const vector<GLuint> shadeList){

  GLuint program = glCreateProgram();//creates your program
  
  for(GLuint i=0;i<shadeList.size();i++){glAttachShader(program,shadeList[i]);}//attaches shaders to program
  
  glBindAttribLocation(program, 0, "position");//binds the location an attribute to a program
  glLinkProgram(program);//links program to your program //weird
  
  GLint linkStatus;//status for linking variable
  glGetProgramiv(program, GL_LINK_STATUS, &linkStatus);//returns the status of linking the program into the variable
  
  if(!linkStatus){//checks to see if your program linked to the program
    GLint logSize;//variable for size of the debug info
    glGetProgramiv(program, GL_INFO_LOG_LENGTH, &logSize);//returns the linking status into the variable
    
    GLchar *infoLog = new GLchar[logSize+1];//allocating memory for the debug info
    glGetProgramInfoLog(program,logSize,&logSize,infoLog);//returns the error messages into the variable infoLog
    
    fprintf(stderr,"\nShader linking failed: %s\n",infoLog);//prints your linking failed
    delete[] infoLog;//memory management
    
    for(GLuint i=0;i<shadeList.size();i++){glDeleteShader(shadeList[i]);}//memory management
  }
  return program;//self explanatory
}

/*
//opengl antique
void triangle1(){
	glClear(GL_COLOR_BUFFER_BIT);//clears the screen
	glBegin(GL_TRIANGLES);//draws a triangle
	glColor3f(1.0,1.0,0.0);//specifies the color r,g,b
	glVertex3f(0.5,-0.5,0.0);
	glVertex3f(0.0,0.5,0.0);
	glVertex3f(-0.5,-0.5,0.0);
	glEnd();
	glFlush();
}
*/
//opengl 4
void triangle1(){
  glClear(GL_COLOR_BUFFER_BIT);//clears the screen
  
  glGenVertexArrays(1, &vaoID);//generates object name for Vertex Array Objects
  glBindVertexArray(vaoID);//bind the object to the array

  glGenBuffers(1, &vboID);//generates object name for the Vertex Buffer Object
  glBindBuffer(GL_ARRAY_BUFFER, vboID);//bind the object to the array
  glBufferData(GL_ARRAY_BUFFER, sizeof(vertexarray), vertexarray, GL_STATIC_DRAW);//allocates the memory of teh vertices

 ShaderInfo shaders[]={//create the shader specified by my initshaders 
  { GL_VERTEX_SHADER , "vertexshader1.glsl"} ,
  { GL_FRAGMENT_SHADER , "fragmentshader1.glsl"},
  { GL_NONE , NULL} 
  };

  initShaders(shaders);//creates shaders
  
  glEnableVertexAttribArray(0);//enables the vertex attribute index 
  glVertexAttribPointer(0,3,GL_FLOAT,GL_FALSE,0,(void*)0);//specified the start the vertice array used to the draw

  glDrawArrays(GL_TRIANGLES, 0, 3);//draws array
  glFlush();//makes sure the prcesses finish
}

void triangle2(){
  glClear(GL_COLOR_BUFFER_BIT);//clear screen

  glGenVertexArrays(1, &vaoID);//generates object name for Vertex Array Objects
  glBindVertexArray(vaoID);//bind the object to the array

  glGenBuffers(1, &vboID);//generates object name for the Vertex Buffer Object
  glBindBuffer(GL_ARRAY_BUFFER, vboID);//bind the object to the array
  glBufferData(GL_ARRAY_BUFFER, sizeof(vertexarray), vertexarray, GL_STATIC_DRAW);//allocates the memory of teh vertices

 ShaderInfo shaders[]={//create the shader specified by my initshaders input
  { GL_VERTEX_SHADER , "vertexshader2.glsl"} ,
  { GL_FRAGMENT_SHADER , "fragmentshader2.glsl"},
  { GL_NONE , NULL} 
  };

  initShaders(shaders);//creates shaders
  	  	
  glEnableVertexAttribArray(0);//enables the vertex attribute index 
  glVertexAttribPointer(0,3,GL_FLOAT,GL_FALSE,0,(void*)0);//specified the start the vertice array used to the draw
  
  glDrawElements(GL_TRIANGLES, 3, GL_UNSIGNED_BYTE, indices);//draws object based on indices of the polygon
  glDisableVertexAttribArray(0);
  glFlush();//make sure the processes finish
}

void drawscene(){
  switch(counter%2){//easy way to switch throw functions
    case 0:
      glutDisplayFunc(triangle1);
      glutPostRedisplay();//sets flags for opengl to redraw the display
      break;
    case 1:
      glutDisplayFunc(triangle2);
      glutPostRedisplay();
      break;
  }
}
//this function create the interaction with the mouse
void mousepress(int button, int state, int x, int y){
  if(button==GLUT_RIGHT_BUTTON && state==GLUT_DOWN)//right click closes the screen
    exit(0);
  else if(button==GLUT_LEFT_BUTTON && state==GLUT_DOWN){//left click changes the shape color
    counter++;
    drawscene();
  }
}

void idle(void){
  glutPostRedisplay();
}

int main(int argc, char **argv){

  glutInit(&argc, argv);
  glutCreateWindow("Shapes");//creates the window with the specified name
  
  //initializes glew
  glewExperimental=GL_TRUE;
  if(glewInit()){
    fprintf(stderr, "Unable to initalize GLEW");
    exit(EXIT_FAILURE);
  }
  
  glutInitContextVersion(4, 3);//specifies the version of opengl
  glutInitContextProfile(GLUT_CORE_PROFILE|GLUT_COMPATIBILITY_PROFILE);//specifies what profile your using


  //retruns what version of opengl and glsl your computer can use
  const GLubyte* version=glGetString(GL_SHADING_LANGUAGE_VERSION);
  fprintf(stderr,"Opengl glsl version %s\n", version);

  version=glGetString(GL_VERSION);
  fprintf(stderr,"Opengl version %s\n", version);

  glutDisplayFunc(drawscene);//displays callback draws the shapes
  glutMouseFunc(mousepress);//control callback specifies the mouse controls
  glutMainLoop();//sets opengl state in a neverending loop
  return 0;
}
