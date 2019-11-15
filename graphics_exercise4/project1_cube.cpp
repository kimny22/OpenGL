#include "cg-2.h"
#include "time.h"
void startup();
void render();
void shutdown();


struct scene{
	int width = 800, height = 600;
	GLuint rendering_program;
	GLuint vertex_array_object;

	vec4 vertices[100];
	vec4 colors[100];
	unsigned int indices[100];
	int num, numIndex;

	mat4 model;
	GLint mv_location;
	float theta;
};

scene sc;
vec4 vertices[8] = {
	vec4(-0.5, -0.5,  0.5, 1.0),
	vec4(-0.5,  0.5,  0.5, 1.0),
	vec4(0.5,  0.5,  0.5, 1.0),
	vec4(0.5, -0.5,  0.5, 1.0),
	vec4(-0.5, -0.5, -0.5, 1.0),
	vec4(-0.5,  0.5, -0.5, 1.0),
	vec4(0.5,  0.5, -0.5, 1.0),
	vec4(0.5, -0.5, -0.5, 1.0)
};

vec4 vertex_colors[8] = {
	vec4(0.0, 0.0, 0.0, 1.0),  // black
	vec4(1.0, 0.0, 0.0, 1.0),  // red
	vec4(1.0, 1.0, 0.0, 1.0),  // yellow
	vec4(0.0, 1.0, 0.0, 1.0),  // green
	vec4(0.0, 0.0, 1.0, 1.0),  // blue
	vec4(1.0, 0.0, 1.0, 1.0),  // magenta
	vec4(1.0, 1.0, 1.0, 1.0),  // white
	vec4(0.0, 1.0, 1.0, 1.0)   // cyan
};

void quad(int a, int b, int c, int d)
{
	sc.colors[sc.num] = vertex_colors[a]; sc.vertices[sc.num] = vertices[a]; sc.num++;
	sc.colors[sc.num] = vertex_colors[b]; sc.vertices[sc.num] = vertices[b]; sc.num++;
	sc.colors[sc.num] = vertex_colors[c]; sc.vertices[sc.num] = vertices[c]; sc.num++;
	sc.colors[sc.num] = vertex_colors[a]; sc.vertices[sc.num] = vertices[a]; sc.num++;
	sc.colors[sc.num] = vertex_colors[c]; sc.vertices[sc.num] = vertices[c]; sc.num++;
	sc.colors[sc.num] = vertex_colors[d]; sc.vertices[sc.num] = vertices[d]; sc.num++;
}

void colorcube()
{
	sc.num = 0; 
	quad(1, 0, 3, 2);
	quad(2, 3, 7, 6);
	quad(4, 0, 3, 7);
	quad(5, 1, 2, 6);
	quad(6, 7, 4, 5);
	quad(5, 4, 0, 1);
}
/*
    1     2        5     6 

    0     3        4      7
*/


// 맨 처음 한번 실행
void startup()
{
	//------------------------------------------------------------------------
	sc.rendering_program = compile_shaders("vproject1.glsl", "fproject1.glsl");

	glGenVertexArrays(1, &sc.vertex_array_object);
	glBindVertexArray(sc.vertex_array_object);

	colorcube();

	GLuint buffer[2];
	glGenBuffers(2, buffer);
	glBindBuffer(GL_ARRAY_BUFFER, buffer[0]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(sc.vertices), sc.vertices, GL_STATIC_DRAW);
	GLuint vPosition = glGetAttribLocation(sc.rendering_program, "vPosition");
	glVertexAttribPointer(vPosition, 4, GL_FLOAT, GL_FALSE, 0, NULL);
	glEnableVertexAttribArray(vPosition);

	glBindBuffer(GL_ARRAY_BUFFER, buffer[1]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(sc.colors), sc.colors, GL_STATIC_DRAW);
	GLuint vColor = glGetAttribLocation(sc.rendering_program, "vColor");
	glVertexAttribPointer(vColor, 4, GL_FLOAT, GL_FALSE, 0, NULL);
	glEnableVertexAttribArray(vColor);
	
	glUseProgram(sc.rendering_program);
	sc.model = rotate(mat4(1.0), radians(25.0f), vec3(1.0, 1.0, 1.0));
	sc.mv_location = glGetUniformLocation(sc.rendering_program, "mv_matrix");
	glUniformMatrix4fv(sc.mv_location, 1, GL_FALSE, value_ptr(sc.model));

	sc.theta = 0.0;
}

//  계속해서 실행됨
void idle()
{
	sc.theta += 0.0001;
	glutPostRedisplay();
}

void render()
{
	glUseProgram(sc.rendering_program);

	const GLfloat color[] = { 0.8f, 0.8f, 0.8f, 0.8f };
	glClearBufferfv(GL_COLOR, 0, color);
	glShadeModel(GL_SMOOTH);

	sc.model = rotate(sc.model, sc.theta, vec3(0.0, 1.0, 0.0));
	glUniformMatrix4fv(sc.mv_location, 1, GL_FALSE, value_ptr(sc.model));
	glDrawArrays(GL_TRIANGLES, 0, sc.num);
//	glDrawElements(GL_TRIANGLES, sc.numIndex, GL_UNSIGNED_INT, sc.indices);  
	
	glutSwapBuffers();
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
	// MAC
	//glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH | GLUT_3_2_CORE_PROFILE);

	glutInitWindowSize(800, 600);
	glutCreateWindow("2D Drawing");
	glewInit();
	version();
	/*
	glutInitContextVersion(3, 3);
	glutInitContextProfile(GLUT_CORE_PROFILE);
	glutInitContextFlags(GLUT_DEBUG);*/
	//GLUT_FORWARD_COMPATIBLE

	startup();

	glutDisplayFunc(render);
	glutIdleFunc(idle);

	glutMainLoop();
	shutdown();
	return 0;
}
