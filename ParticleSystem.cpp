//
// Created by Austin Brennan on 9/12/16
// See readme.md for more details
//
#include "Camera.h"
#include "FGAFile.h"
#include "Solver.h"
#include "ObjWriter.h"

#ifdef __APPLE__
#  pragma clang diagnostic ignored "-Wdeprecated-declarations"
#  include <GLUT/glut.h>
#else
  #include <GL/glut.h>
#endif

int WIDTH = 1280;
int HEIGHT = 720;

Camera *camera;
Solver *solver;
ObjWriter particle_writer;

bool showReferenceGrid = true;
bool showVelocityGrid = true;
bool showReferenceParticles = true;
bool showReferenceColliders = true;
bool showRenderParticles = false;
bool renderAsPoints = true;
bool renderAsLines = false;
bool writeObjSequence = false;
unsigned int FRAME_NUMBER = 0;

// draws a simple grid
void drawReferenceGrid() {
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

void drawVelocityGrid() {
  int xres = solver->velocity_grid.x_res;
  int yres = solver->velocity_grid.y_res;
  int zres = solver->velocity_grid.z_res;

  glColor4f(1.0f, 1.0f, 0.0f, 0.1f);
  glLineWidth(1.0f);

  for (int zi = 0; zi < zres; ++zi) {
    for (int yi = 0; yi < yres; ++yi) {
      for (int xi = 0; xi < xres; ++xi) {
        Vector3d voxel_loc = solver->velocity_grid.get_voxel_location(xi, yi, zi);
        Vector3d velocity = solver->velocity_grid.get_velocity(xi, yi, zi);
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

void drawParticles() {
  glPointSize(2.5f);
  glColor3f(0.12843f, 0.0f, 0.7823f);

  for (auto p = solver->particles.begin(); p != solver->particles.end(); ++p) {
    glBegin(GL_POINTS);
    glVertex3f((GLfloat) p->pos.x,
               (GLfloat) p->pos.y,
               (GLfloat) p->pos.z);
    glEnd();
  }
}


void drawColliders() {
  for(auto c = solver->colliders.begin(); c != solver->colliders.end(); ++c) {
    glColor3f(0.1, 1.0, 0.1);
    GLUquadric *quad;
    quad = gluNewQuadric();
    glTranslatef(c->pos.x,c->pos.y, c->pos.z);
    gluSphere(quad, c->radius, 100, 20);
  }
}


void initCameraDebug() {
  // set up camera
  // parameters are eye point, aim point, up vector
  camera = new Camera(Vector3d(0, 5, 5), Vector3d(0, 0, 0),
		      Vector3d(0, 1, 0));

  // grey background for window
  glClearColor(0.62, 0.62, 0.62, 0.0f);
  glShadeModel(GL_SMOOTH);
  glDepthRange(0.0f, 1.0f);

  glEnable(GL_NORMALIZE);
  glEnable(GL_BLEND);
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
}

void initCameraRender() {
  // set up camera
  // parameters are eye point, aim point, up vector
  camera = new Camera(Vector3d(0, 5, 5), Vector3d(0, 0, 0),
                      Vector3d(0, 1, 0));

  // black background for window
  glClearColor(0.0, 0.0, 0.0, 0.0f);
  glShadeModel(GL_SMOOTH);
  glDepthRange(0.0f, 1.0f);

  glEnable(GL_NORMALIZE);
  glEnable(GL_BLEND);
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
}

void renderParticles() {
  glPointSize(2.5f);
  double percentage_of_life;

  if (renderAsPoints) {
    for (auto p = solver->particles.begin(); p != solver->particles.end(); ++p) {
      percentage_of_life = p->life_left / p->lifetime;
      glColor3f(p->color.x * percentage_of_life, p->color.y * percentage_of_life, p->color.z * percentage_of_life);

      glBegin(GL_POINTS);
      glVertex3f((GLfloat) p->pos.x,
                 (GLfloat) p->pos.y,
                 (GLfloat) p->pos.z);
      glEnd();
    }
  }
  else if (renderAsLines) {
    for (auto p = solver->particles.begin(); p != solver->particles.end(); ++p) {
      percentage_of_life = p->life_left / p->lifetime;

      glBegin(GL_LINES);
      glColor3f(p->color.x * percentage_of_life, p->color.y * percentage_of_life, p->color.z * percentage_of_life);
      glVertex3f((GLfloat) p->pos.x,
                 (GLfloat) p->pos.y,
                 (GLfloat) p->pos.z);

      glVertex3f((GLfloat) p->pos.x + p->vel.x,
                 (GLfloat) p->pos.y + p->vel.y,
                 (GLfloat) p->pos.z + p->vel.z);
      glEnd();
    }
  }
}

void perspDisplay() {
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  // draw the camera created in perspective
  camera->PerspectiveDisplay(WIDTH, HEIGHT);

  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();

  if (showReferenceGrid)
    drawReferenceGrid();
  if (showVelocityGrid)
    drawVelocityGrid();
  if (showReferenceParticles)
    drawParticles();
  if (showReferenceColliders)
    drawColliders();
  if (showRenderParticles)
    renderParticles();

  glFlush();
  glutSwapBuffers();
}

void simulateParticles() {
  solver->update();
  if (writeObjSequence) {
    particle_writer.writeParticles(solver->particles, "/home/awbrenn/Documents/workspace/physanim/2/obj_sequence/",
                                   "particles", FRAME_NUMBER++);
  }

  glutPostRedisplay();
}

void initializeSimulation(char *paramfile_name) {
  std::ifstream paramfile_stream;
  std::string velocity_grid_name;
  double emission_rate, timestep,
         particle_mass_avg, particle_mass_sdv,
         particle_life_avg, particle_life_sdv,
         particle_color_sdv, collider_radius,
         gravity, velocity_grid_scale, air_resistance;
  size_t max_number_of_particles, substeps;
  bool emit_on_surface, even_dist;
  Vector3d emitter_position, emmiter_size, collider_position, particle_color_avg, wind_force;
  std::vector<Emitter> emitters;
  std::vector<Collider> colliders;
  std::string skipline;

  paramfile_stream.open(paramfile_name, std::ios_base::in);

  // read the file
  if (paramfile_stream) {
    // skip the first line
    getline(paramfile_stream, skipline);

    // extract the name of the velocity grid
    paramfile_stream >> velocity_grid_name;

    // skip some lines
    getline(paramfile_stream, skipline);
    getline(paramfile_stream, skipline);
    getline(paramfile_stream, skipline);

    // extract emitter values
    paramfile_stream >> emission_rate >> emmiter_size.x >> emmiter_size.y >> emmiter_size.z >>
                        emitter_position.x >> emitter_position.y >>
                        emitter_position.z >> std::boolalpha >> emit_on_surface >>
                        even_dist;

    // skip some lines
    getline(paramfile_stream, skipline);
    getline(paramfile_stream, skipline);
    getline(paramfile_stream, skipline);

    // extract particle values for current emitter
    paramfile_stream >> particle_mass_avg >> particle_mass_sdv;

    // skip some lines
    getline(paramfile_stream, skipline);
    getline(paramfile_stream, skipline);
    getline(paramfile_stream, skipline);

    // extract particle values for current emitter
    paramfile_stream >> particle_life_avg >> particle_life_sdv;

    // skip some lines
    getline(paramfile_stream, skipline);
    getline(paramfile_stream, skipline);
    getline(paramfile_stream, skipline);

    paramfile_stream >> particle_color_avg.x >> particle_color_avg.y >>
                        particle_color_avg.z >> particle_color_sdv;

    emitters.push_back(Emitter(emission_rate, emmiter_size, emit_on_surface, even_dist, emitter_position, particle_mass_avg,
                               particle_mass_sdv, particle_life_avg, particle_life_sdv, particle_color_avg,
                               particle_color_sdv));

    // skip some lines
    getline(paramfile_stream, skipline);
    getline(paramfile_stream, skipline);
    getline(paramfile_stream, skipline);

    paramfile_stream >> gravity >> wind_force.x >> wind_force.y >> wind_force.z >>
                     velocity_grid_scale >> air_resistance;

    // skip some lines
    getline(paramfile_stream, skipline);
    getline(paramfile_stream, skipline);
    getline(paramfile_stream, skipline);

    // extract solver values
    paramfile_stream >> max_number_of_particles >> timestep >> substeps;
  }
  else {
    std::cerr << "Unable to open file " << paramfile_name << std::endl;
    exit(EXIT_FAILURE);
  }

  // read the velocity grid
  VelocityGrid velocity_grid;
  FGAFile fga_file;
  fga_file.read(velocity_grid_name, &velocity_grid);
  velocity_grid.generate_voxel_locations();

  solver = new Solver(max_number_of_particles, emitters, colliders, velocity_grid, velocity_grid_scale, gravity,
                      wind_force, air_resistance, timestep, substeps);

  cout << endl;
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
    // render the particles
    showReferenceGrid = false;
    showVelocityGrid = false;
    showReferenceColliders = false;
    showReferenceParticles = false;
    showRenderParticles = true;
    initCameraRender();
    ;
    break;

  case 'd': case 'D':
    // render the particles
    showReferenceGrid = true;
    showVelocityGrid = true;
    showReferenceColliders = true;
    showReferenceParticles = true;
    showRenderParticles = false;
    initCameraDebug();
    ;
    break;

  case 'p': case 'P':
    // render the particles as points
    renderAsPoints = true;
    renderAsLines = false;
    ;
    break;

  case 'l': case 'L':
    // render the particles as lines
    renderAsLines = true;
    renderAsPoints = false;
    ;
    break;

  case 'v': case 'V':
    showVelocityGrid = !showVelocityGrid;
    ;
    break;

  case 'g': case 'G':
    showReferenceGrid = !showReferenceGrid;
    break;

  case 'f': case 'F':
    camera->SetCenterOfFocus(Vector3d(0, 0, 0));
    break;

  case 'w': case 'W':
    writeObjSequence = !writeObjSequence;
    break;

  case 'q': case 'Q':	// q or esc - quit
  case 27:		// esc
    exit(0);
  }

  glutPostRedisplay();
}

int main(int argc, char *argv[]) {
  if (argc != 2) {
    cerr << "Proper Usage: $> ParticleSystem parameters_file" << endl;
    exit(-1);
  }

  initializeSimulation(argv[1]);

  // set up opengl window
  glutInit(&argc, argv);
  glutInitDisplayMode(GLUT_RGBA | GLUT_DEPTH | GLUT_DOUBLE);
  glutInitWindowSize(WIDTH, HEIGHT);
  glutInitWindowPosition(50, 50);
  int persp_win = glutCreateWindow("Simulating Particles In a Velocity Field");

  // initialize the camera and such
  initCameraDebug();

  // set up opengl callback functions
  glutDisplayFunc(perspDisplay);
  glutIdleFunc(simulateParticles);
  glutMouseFunc(mouseEventHandler);
  glutMotionFunc(motionEventHandler);
  glutKeyboardFunc(keyboardEventHandler);


  glutMainLoop();
  return(0);
}

