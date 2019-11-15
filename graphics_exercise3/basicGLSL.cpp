#include "cg.h"
#include "time.h"
void startup();
void render();
void shutdown();
const int WIDTH = 800; const int HEIGHT = 600;

struct scene {
	int width = 800, height = 600;
	GLuint rendering_program;
	GLuint vertex_array_object;
};

scene sc;

void render()
{
	const GLfloat color[] = { 0.8f, 0.8f, 0.8f, 1.0f };
	glClearBufferfv(GL_COLOR, 0, color);

	glUseProgram(sc.rendering_program);

	clock_t ct1 = clock()*0.001;
	GLfloat attrib[] = { (float)sin((float)ct1)*0.5f,  (float)cos((float)ct1)*0.6f, 0.0f, 0.0f };
	glVertexAttrib4fv(0, attrib);

	float val = (float)sin((float)ct1) / 2.0 + 0.5;
	float go[4] = { val, val, 0.2, 1.0f };
	glVertexAttrib4fv(1, go);

	glDrawArrays(GL_TRIANGLES, 0, 3);

	glutSwapBuffers();
}

void idle() { glutPostRedisplay(); }

void startup()
{
	/**** Shaders as variables */
	//0. Point
	sc.rendering_program = compile_shaders("v0.glsl", "f0.glsl");

	/* Vertex Array Object */
	glGenVertexArrays(1, &sc.vertex_array_object);
	glBindVertexArray(sc.vertex_array_object);
}

void shutdown()
{
	glDeleteVertexArrays(1, &sc.vertex_array_object);
	glDeleteProgram(sc.rendering_program);
}

int main(int argc, char **argv)
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH);
	glutInitWindowSize(WIDTH, HEIGHT);
	glutCreateWindow("Point");
	glewInit();

	version();
	startup();
	glutDisplayFunc(render);
	glutIdleFunc(idle);

	glutMainLoop();

	system("pause");

	shutdown();
	return 0;
}