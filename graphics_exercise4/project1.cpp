#include "cg-2.h"
#include "time.h"
void startup();
void render();
void shutdown();

struct scene {
	int width = 800, height = 600;
	GLuint rendering_program;
	GLuint vertex_array_object;

	vec4 vertices[100];
	vec4 colors[100];
	unsigned int indices[100];
	int num, numIndex;

	mat4 model;
	GLint mv_location;
	int color;
	float theta;
	float dispw, dispb;
};

scene sc;

void make_model()
{
	sc.num = 0;
	//House (drawArray) 
	/* 
	sc.vertices[sc.num] = vec4(0.5, 0.2, 0.0, 1.0); sc.colors[sc.num] = vec4(1.0, 1.0, 0.0, 1.0);  sc.num++;
	sc.vertices[sc.num] = vec4(0.0, 0.5, 0.0, 1.0); sc.colors[sc.num] = vec4(1.0, 1.0, 0.0, 1.0);  sc.num++;
	sc.vertices[sc.num] = vec4(-0.5, 0.2, 0.0, 1.0); sc.colors[sc.num] = vec4(1.0, 1.0, 0.0, 1.0);  sc.num++;
	
	sc.vertices[sc.num] = vec4(0.5, 0.2, 0.0, 1.0); sc.colors[sc.num] = vec4(1.0, 1.0, 0.0, 1.0);  sc.num++;
	sc.vertices[sc.num] = vec4(-0.5, 0.2, 0.0, 1.0); sc.colors[sc.num] = vec4(1.0, 1.0, 0.0, 1.0);  sc.num++;
	sc.vertices[sc.num] = vec4(-0.5, -0.5, 0.0, 1.0); sc.colors[sc.num] = vec4(1.0, 0.0, 0.0, 1.0);  sc.num++;

	sc.vertices[sc.num] = vec4(0.5, 0.2, 0.0, 1.0); sc.colors[sc.num] = vec4(1.0, 1.0, 0.0, 1.0);  sc.num++;
	sc.vertices[sc.num] = vec4(-0.5, -0.5, 0.0, 1.0); sc.colors[sc.num] = vec4(1.0, 0.0, 0.0, 1.0);  sc.num++;
	sc.vertices[sc.num] = vec4(0.5, -0.5, 0.0, 1.0); sc.colors[sc.num] = vec4(1.0, 0.0, 0.0, 1.0);  sc.num++;
	*/
	//House (drawElements)
	/*
	sc.vertices[sc.num] = vec4(0.5, 0.2, 0.0, 1.0); sc.colors[sc.num] = vec4(1.0, 1.0, 0.0, 1.0);  sc.num++;
	sc.vertices[sc.num] = vec4(0.0, 0.5, 0.0, 1.0); sc.colors[sc.num] = vec4(1.0, 1.0, 0.0, 1.0);  sc.num++;
	sc.vertices[sc.num] = vec4(-0.5, 0.2, 0.0, 1.0); sc.colors[sc.num] = vec4(1.0, 1.0, 0.0, 1.0);  sc.num++;
	sc.vertices[sc.num] = vec4(-0.5, -0.5, 0.0, 1.0); sc.colors[sc.num] = vec4(1.0, 0.0, 0.0, 1.0);  sc.num++;
	sc.vertices[sc.num] = vec4(0.5, -0.5, 0.0, 1.0); sc.colors[sc.num] = vec4(1.0, 0.0, 0.0, 1.0);  sc.num++;
	sc.indices[0] = 0; sc.indices[1] = 1; sc.indices[2] = 2;
	sc.indices[3] = 0; sc.indices[4] = 2; sc.indices[5] = 3;
	sc.indices[6] = 0; sc.indices[7] = 3; sc.indices[8] = 4;
	sc.numIndex = 9;
	*/
	//rect (drawElement)
	sc.vertices[sc.num] = vec4(0.5, 0.5, 0.0, 1.0); sc.num++;
	sc.vertices[sc.num] = vec4(-0.5, 0.5, 0.0, 1.0); sc.num++;
	sc.vertices[sc.num] = vec4(-0.5, -0.5, 0.0, 1.0); sc.num++;
	sc.vertices[sc.num] = vec4(0.5, -0.5, 0.0, 1.0); sc.num++;

	sc.indices[0] = 0; 	sc.indices[1] = 1; 	sc.indices[2] = 3;
	sc.indices[3] = 3; 	sc.indices[4] = 1; 	sc.indices[5] = 2;
	sc.numIndex = 6;
}

// 맨 처음 한번 실행
void startup()
{
	//------------------------------------------------------------------------
	sc.rendering_program = compile_shaders("vproject1.glsl", "fproject1.glsl");

	glGenVertexArrays(1, &sc.vertex_array_object);
	glBindVertexArray(sc.vertex_array_object);

	GLuint buffer[2];

	make_model();

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

	sc.model = mat4(1.0); sc.dispb = -1.0; sc.dispw = -1.0;
	sc.mv_location = glGetUniformLocation(sc.rendering_program, "mv_matrix");
	glUniformMatrix4fv(sc.mv_location, 1, GL_FALSE, value_ptr( sc.model)); 
	sc.color = glGetUniformLocation(sc.rendering_program, "color");
	glUniform4f(sc.color, 1.0, 1.0, 1.0, 1.0);
}

//  계속해서 실행됨
void idle()
{
	//화면 중심으로 회전
	/*
	sc.model = rotate(sc.model, radians(0.01f), vec3(0.0, 0.0, 1.0));
	glUniformMatrix4fv(sc.mv_location, 1, GL_FALSE, value_ptr(sc.model));
	*/
	//화면 중심을 기준으로 하는 원 위를 따라서 움직이는 애니메이션
	/*
	sc.theta += 0.01f;
	sc.model = translate(mat4(1.0), vec3(0.5*cos(radians(sc.theta)),0.5*sin(radians(sc.theta)), 0.0));
	sc.model = scale(sc.model, vec3(0.3, 0.3, 0.0));
	glUniformMatrix4fv(sc.mv_location, 1, GL_FALSE, value_ptr(sc.model));
	*/
	sc.dispw += 0.0001; if (sc.dispw > 1.0) sc.dispw = -1.0;
	sc.dispb += 0.0002; if (sc.dispb > 1.0) sc.dispb = -1.0;

	glutPostRedisplay();

}

void render()
{
	glUseProgram(sc.rendering_program);

	const GLfloat color[] = { 0.8f, 0.8f, 0.8f, 0.8f };
	glClearBufferfv(GL_COLOR, 0, color);
	
	mat4 white, black;
	white = translate(mat4(1.0), vec3(sc.dispw, 0.1, 0.0));
	white = scale(white, vec3(0.2, 0.2, 0.0));
	glUniformMatrix4fv(sc.mv_location, 1, GL_FALSE, value_ptr(white));
	glUniform4f(sc.color, 1.0, 1.0, 1.0, 1.0);
	//glDrawArrays(GL_TRIANGLES, 0, sc.num);
	glDrawElements(GL_TRIANGLES, sc.numIndex, GL_UNSIGNED_INT, sc.indices);  

	black = translate(mat4(1.0), vec3(sc.dispb, -0.1, 0.0));
	black = scale(black, vec3(0.2, 0.2, 0.0));
	glUniformMatrix4fv(sc.mv_location, 1, GL_FALSE, value_ptr(black));
	glUniform4f(sc.color, 0.0, 0.0, 0.0, 1.0);
	//glDrawArrays(GL_TRIANGLES, 0, sc.num);
	glDrawElements(GL_TRIANGLES, sc.numIndex, GL_UNSIGNED_INT, sc.indices);

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

	glutInitContextVersion(3, 3);
	glutInitContextProfile(GLUT_CORE_PROFILE);
	glutInitContextFlags(GLUT_DEBUG);

	glutInitWindowSize(800, 600);
	glutCreateWindow("2D Drawing");
	glewInit();
	version();

	startup();
	glutDisplayFunc(render);
	glutIdleFunc(idle);

	glutMainLoop();
	shutdown();
	return 0;
}
