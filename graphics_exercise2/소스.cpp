#pragma comment (lib, "lib/freeglut.lib")
#pragma comment (lib, "lib/glew32.lib")
#include "GL/glew.h"
#include "GL/freeglut.h"
#include "GL/freeglut_ext.h"
#include "GL/freeglut_std.h"
#include "GL/glut.h"
#include "math.h"
void mydisplay(){
	float vertices[] = { 1.0, 0.0, 0.0, 0.0, 1.0, 0.0, 0.0, 0.0, 1.0 };
	unsigned int indices[] = { 0, 1, 2 };
	glClear(GL_COLOR_BUFFER_BIT);
	glClearColor(0.7, 0.7, 0.7, 1.0);
	glEnableClientState(GL_VERTEX_ARRAY);
	glVertexPointer(3, GL_FLOAT, 0, vertices);
	glDrawElements(GL_TRIANGLES, 3, GL_UNSIGNED_INT, indices);
	glDisableClientState(GL_VERTEX_ARRAY);
	glutSwapBuffers();
}
void mydisplay2() {
	float vertices[12];
	float x = -0.8;
	float y = 0.4;
	for (int i = 0; i < 6; i++) {
		vertices[2 * i] = x; x += 0.3;
		vertices[2 * i + 1] = y; y *= -1.0; 
	}
	glEnableClientState(GL_VERTEX_ARRAY);
	glVertexPointer(2, GL_FLOAT, 0, vertices);
	//glDrawArrays(GL_LINES, 0, 6);
	unsigned int indices[] = { 0,2,1,3,2,5,3,4};
	glDrawElements(GL_LINES, 8, GL_UNSIGNED_INT, indices);
	glDisableClientState(GL_VERTEX_ARRAY);
	glutSwapBuffers();
}
void mydisplay3() {
	float vertices[26];
	float x = 0.0;
	for (int i = 0; i < 13; i++) {
		vertices[2 * i] = 0.5*cos(x);
		vertices[2 * i + 1] = 0.5*sin(x);
		x += 30 * 3.141592654 /180;
	}
	glEnableClientState(GL_VERTEX_ARRAY);
	glVertexPointer(2, GL_FLOAT, 0, vertices);
	unsigned int indices[] = {0,2,1, 0,3,2, 0,4,3, 0,5,4, 0,6,5, 0,7,6, 0,8,7, 0,9,8, 0,10,9, 0,11,10, 0,12,11, 0,1,12};
	glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, indices);
	glDisableClientState(GL_VERTEX_ARRAY);
	glutSwapBuffers();
}
int main(int argc, char** argv) {
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA);
	glutInitWindowSize(512, 512);
	glutCreateWindow("simple");
	glutDisplayFunc(mydisplay3);
	glutMainLoop();
}