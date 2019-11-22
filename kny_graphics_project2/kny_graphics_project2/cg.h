#pragma once

#include <cmath>
#include <iostream>

// WINDOWS. Mac �� ���̺귯�� ��� ���� �ʿ�
#pragma comment (lib, "lib/freeglut.lib")
#pragma comment (lib, "lib/glew32.lib")

#include "GL/glew.h"
#include "GL/freeglut.h"
#include "GL/freeglut_ext.h"
#include "GL/freeglut_std.h"
#include "GL/glut.h"

// Windows �� Mac ��� ����
#include "glm/glm.hpp"
#include "glm/ext/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"

using namespace std;
using namespace glm;

void version();
GLuint compile_shaders(const char*  vsource, const char*  fsource);
