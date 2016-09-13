//
// CameraExample.cpp
//
// Example program to show how to use Chris Root's OpenGL Camera Class
// 
// Christopher Root, 2006
// Minor Modifications by Donald House, 2009
// Minor Modifications by Yujie Shu, 2012
//
#include "Camera.h"
#include "FGAFile.h"

#ifdef __APPLE__
#  pragma clang diagnostic ignored "-Wdeprecated-declarations"
#  include <GLUT/glut.h>
#else
  #include <GL/glut.h>
#endif

int WIDTH = 1280;
int HEIGHT = 720;

Camera *camera;
VelocityGrid velocity_grid;
//Solver *solver;

bool showGrid = true;

// draws a simple grid
void makeGrid() {
  glColor3f(0.0f, 0.0f, 0.0f);

  glLineWidth(1.0f);

  for (double i=-12; i<12; i++) {
    for (double j=-12; j<12; j++) {
      glBegin(GL_LINES);
      glVertex3f(i, 0, j);
      glVertex3f(i, 0, j+1);
      glEnd();
      glBegin(GL_LINES);
      glVertex3f(i, 0, j);
      glVertex3f(i+1, 0, j);
      glEnd();

      if (j == 11){
	glBegin(GL_LINES);
	glVertex3f(i, 0, j+1);
	glVertex3f(i+1, 0, j+1);
	glEnd();
      }
      if (i == 11){
	glBegin(GL_LINES);
	glVertex3f(i+1, 0, j);
	glVertex3f(i+1, 0, j+1);
	glEnd();
      }
    }
  }

  glLineWidth(2.0f);
  glBegin(GL_LINES);
  glVertex3f(-12, 0, 0);
  glVertex3f(12, 0, 0);
  glEnd();
  glBegin(GL_LINES);
  glVertex3f(0, 0, -12);
  glVertex3f(0, 0, 12);
  glEnd();
  glLineWidth(1.0f);
}

void makeVelocityGrid() {
  int xres = velocity_grid.x_res;
  int yres = velocity_grid.y_res;
  int zres = velocity_grid.z_res;

  glColor4f(1.0f, 1.0f, 0.0f, 0.5f);
  glLineWidth(1.0f);

  for (int zi = 0; zi < zres; ++zi) {
    for (int yi = 0; yi < yres; ++yi) {
      for (int xi = 0; xi < xres; ++xi) {
        Vector3d voxel_loc = velocity_grid.get_voxel_location(xi, yi, zi);
        Vector3d velocity = velocity_grid.get_velocity(xi, yi, zi);
        glBegin(GL_LINES);
        glVertex3f((GLfloat) voxel_loc.x,
                   (GLfloat) voxel_loc.y,
                   (GLfloat) voxel_loc.z);
        glVertex3f((GLfloat) (voxel_loc.x + velocity.x),
                   (GLfloat) (voxel_loc.y + velocity.y),
                   (GLfloat) (voxel_loc.z + velocity.z));
        glEnd();
      }
    }
  }
}


void initCamera() {
  // set up camera
  // parameters are eye point, aim point, up vector
  camera = new Camera(Vector3d(0, 5, 5), Vector3d(0, 0, 0),
		      Vector3d(0, 1, 0));

  // grey background for window
  glClearColor(0.62, 0.62, 0.62, 0.0f);
  glShadeModel(GL_SMOOTH);
  glDepthRange(0.0f, 1.0f);

//  glEnable(GL_DEPTH_TEST);
//  glDepthFunc(GL_LEQUAL);
  glEnable(GL_NORMALIZE);
  glEnable(GL_BLEND);
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
}

void perspDisplay() {
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  // draw the camera created in perspective
  camera->PerspectiveDisplay(WIDTH, HEIGHT);

  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();

  if (showGrid)
    makeGrid();

  makeVelocityGrid();

  glFlush();
  glutSwapBuffers();
}

void simulateParticles() {
  // TODO Add simulation step
}

void mouseEventHandler(int button, int state, int x, int y) {
  // let the camera handle some specific mouse events (similar to maya)
  camera->HandleMouseEvent(button, state, x, y);
  glutPostRedisplay();
}

void motionEventHandler(int x, int y) {
  // let the camera handle some mouse motions if the camera is to be moved
  camera->HandleMouseMotion(x, y);
  glutPostRedisplay();
}

void keyboardEventHandler(unsigned char key, int x, int y) {
  switch (key) {
  case 'r': case 'R':
    // reset the camera to its initial position
    camera->Reset();
    break;

  case 'f': case 'F':
    camera->SetCenterOfFocus(Vector3d(0, 0, 0));
    break;

  case 'g': case 'G':
    showGrid = !showGrid;
    break;

  case 'q': case 'Q':	// q or esc - quit
  case 27:		// esc
    exit(0);
  }

  glutPostRedisplay();
}

int main(int argc, char *argv[]) {
  if (argc != 2) {
    cerr << "Proper Usage: $> ParticleSystem velocity_grid.fga" << endl;
    exit(-1);
  }

  FGAFile fga_file;
  fga_file.read(argv[1], &velocity_grid);
  velocity_grid.generate_voxel_locations();

  // set up opengl window
  glutInit(&argc, argv);
  glutInitDisplayMode(GLUT_RGBA | GLUT_DEPTH | GLUT_DOUBLE);
  glutInitWindowSize(WIDTH, HEIGHT);
  glutInitWindowPosition(50, 50);
  int persp_win = glutCreateWindow("Simulating Particles In a Velocity Field");

  // initialize the camera and such
  initCamera();

  // set up opengl callback functions
  glutDisplayFunc(perspDisplay);
  glutIdleFunc(simulateParticles);
  glutMouseFunc(mouseEventHandler);
  glutMotionFunc(motionEventHandler);
  glutKeyboardFunc(keyboardEventHandler);


  glutMainLoop();
  return(0);
}

