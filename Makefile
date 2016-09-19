#----------------------------------------------
#    Example Makefile for compiling a program that uses
#    the Matrix and Vector classes
#----------------------------------------------
#
#  Programmer: Donald House
#  Date: March 8, 1999
#
#  Copyright (C) - Donald H. House. 2005
#

CC      = g++
C	= cpp
H	= h
CFLAGS 	= -g -std=c++11

ifeq ("$(shell uname)", "Darwin")
  LDFLAGS     = -framework Foundation -framework GLUT -framework OpenGL -lm
else
  ifeq ("$(shell uname)", "Linux")
    LDFLAGS     = -lglut -lGL -lm -L/usr/local/lib -lGLU
  endif
endif

HFILES 	= Matrix.${H} Vector.${H} Utility.${H} Camera.${H} Collider.${H} Emitter.${H} Particle.${H} Solver.${H} FGAFile.${H} VelocityGrid.${H}
OFILES 	= Matrix.o Vector.o Utility.o Camera.o Collider.o Emitter.o Particle.o Solver.o FGAFile.o VelocityGrid.o
PROJECT = ParticleSystem

${PROJECT}:	${PROJECT}.o $(OFILES)
	${CC} $(CFLAGS) -o ${PROJECT} ${PROJECT}.o $(OFILES) $(LDFLAGS)

${PROJECT}.o: ${PROJECT}.${C} $(HFILES)
	${CC} $(CFLAGS) -c ${PROJECT}.${C}

Solver.o: Solver.${C} Matrix.${H} Vector.${H} Utility.${H} Collider.${H} Emitter.${H} Particle.${H} Solver.${H} FGAFile.${H} VelocityGrid.${H}
	${CC} $(CFLAGS) -c Solver.${C}

Collider.o: Collider.${C} Vector.${H} Utility.${H}
	${CC} $(CFLAGS) -c Collider.${C}

Emitter.o: Emitter.${C} Matrix.${H} Vector.${H} Utility.${H} Particle.${H}
	${CC} $(CFLAGS) -c Emitter.${C}

Particle.o: Particle.${C} Matrix.${H} Vector.${H} Utility.${H}
	${CC} $(CFLAGS) -c Particle.${C}

FGAFile.o: FGAFile.${C} Matrix.${H} Vector.${H} Utility.${H} VelocityGrid.${H}
	${CC} $(CFLAGS) -c FGAFile.${C}

VelocityGrid.o: VelocityGrid.${C} Matrix.${H} Vector.${H} Utility.${H}
	${CC} $(CFLAGS) -c VelocityGrid.${C}

Camera.o: Camera.${C} Camera.${H} Matrix.${H} Vector.${H} Utility.${H} 
	${CC} $(CFLAGS) -c Camera.${C}

Matrix.o: Matrix.${C} Matrix.${H} Vector.${H} Utility.${H}
	${CC} $(CFLAGS) -c Matrix.${C}

Vector.o: Vector.${C} Vector.${H} Utility.${H} 
	${CC} $(CFLAGS) -c Vector.${C}

Utility.o: Utility.${C} Utility.${H}
	${CC} $(CFLAGS) -c Utility.${C}

debug:
	make 'DFLAGS = /usr/lib/debug/malloc.o'

clean:
	rm -f *.o *~ ${PROJECT}
