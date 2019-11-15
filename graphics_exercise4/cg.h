#pragma once

#include <cmath>
#include <iostream>

#pragma comment (lib, "lib/freeglut.lib")
#pragma comment (lib, "lib/glew32.lib")

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

#define radians(x) x*M_PI/180.0
#define degrees(x) x*180.0/M_PI

#include "GL/glew.h"
#include "GL/freeglut.h"
#include "GL/freeglut_ext.h"
#include "GL/freeglut_std.h"
#include "GL/glut.h"

using namespace std;

void version();
GLuint compile_shaders(const char*  vsource, const char*  fsource);
