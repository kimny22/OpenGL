#include "cg.h"
#include "camera.h"
#include "vboplane.h"
#include "vboteapot.h"

void startup();
void render();
void shutdown();
extern void version();

struct scene {
	int width = 800, height = 600;
	GLuint rendering_program;
	GLuint vertex_array_object;

	// ¸ðµ¨¸µ º¯¼ö
	VBOPlane *plane;
	VBOTeapot *teapot;

	// ºäÀ× º¯¼ö
	mat4 model, view, proj;
	GLint m_loc, v_loc, proj_loc, color_loc;
	float angle;
};
scene sc;

void startup()
{
	/******* OpenGL Initialization */
	glEnable(GL_DEPTH_TEST); glDepthFunc(GL_LEQUAL); //Passes if the incoming depth value is less than or
	/**** Shaders as variables */
	sc.rendering_program = compile_shaders("perfrag.glsl", "perfragf.glsl");
	glUseProgram(sc.rendering_program);

	// modeling
	sc.plane = new VBOPlane(50.0f, 50.0f, 1, 1);
	sc.teapot = new VBOTeapot(14, mat4(1.0f));

	// viewing
	sc.angle = 0.957283f;
	sc.model = mat4(1.0);
	
	sc.view = LookAt(vec3(0.0f, 2.0f, 8.0f), vec3(0.0f, 0.0f, 0.0f), vec3(0.0f, 1.0f, 0.0f));

	float cameraZ = sc.height * 0.5 / tan(radians(0.5*50.0));
	sc.proj = Perspective(50.0f, (float)sc.width / (float)sc.height, (float)0.001*cameraZ, (float)10.0*cameraZ);	

		//ortho
	sc.m_loc = glGetUniformLocation(sc.rendering_program, "m_matrix");
	sc.v_loc = glGetUniformLocation(sc.rendering_program, "v_matrix");
	sc.proj_loc = glGetUniformLocation(sc.rendering_program, "proj_matrix");
	sc.color_loc = glGetUniformLocation(sc.rendering_program, "color");

	glUniformMatrix4fv(sc.m_loc, 1, GL_FALSE, value_ptr(mat4(1.0)));
	glUniformMatrix4fv(sc.v_loc, 1, GL_FALSE, value_ptr(sc.view));
	glUniformMatrix4fv(sc.proj_loc, 1, GL_TRUE, value_ptr(sc.proj));

	// Lighting
	glUniform3f(glGetUniformLocation(sc.rendering_program, "Light[0].Intensity"), 0.2, 0.2, 0.2);
	glUniform3f(glGetUniformLocation(sc.rendering_program, "Light[1].Intensity"), 0.6, 0.6, 0.6);

}

void idle() {
	sc.angle += 0.001f;
	if (sc.angle > 2.0*3.141592) sc.angle -= 2.0*3.141592;
	glutPostRedisplay();
}
void render()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glClearColor(0.6f, 0.6f, 0.6f, 1.0f);

	glUseProgram(sc.rendering_program);

	// Lighting 
	glUniform4fv(glGetUniformLocation(sc.rendering_program, "Light[0].Position"),
		1, value_ptr(sc.view*vec4(10.0f*cos(sc.angle), 3.0f, 10.0f*sin(sc.angle), 1.0f)));
	glUniform4fv(glGetUniformLocation(sc.rendering_program, "Light[1].Position"),
		1, value_ptr(sc.view*vec4(0.0f, 1.0f, 0.0f, 1.0f)));
	
	/******** Mesh  */
	glUniform3f(glGetUniformLocation(sc.rendering_program, "Material.Ka"), 0.9f, 0.5f, 0.3f);
	glUniform3f(glGetUniformLocation(sc.rendering_program, "Material.Kd"), 0.9f, 0.96f, 0.95f);
	glUniform3f(glGetUniformLocation(sc.rendering_program, "Material.Ks"), 0.1f, 0.1f, 0.1f);
	glUniform1f(glGetUniformLocation(sc.rendering_program, "Material.Shininess"), 100.0f);
	// Models

	mat4 m2;
	m2 = translate(mat4(1.0), vec3(2.0, 0.0, -4.0));
	m2 = rotate(m2, (float)radians(-90.0), vec3(1.0, 0.0, 0.0));
	m2 = scale(m2, vec3(0.3, 0.3, 0.3));
	glUniformMatrix4fv(sc.m_loc, 1, GL_FALSE, value_ptr(m2));
	glUniform4f(sc.color_loc, 0.2, 0.7, 0.7, 1.0);
	sc.teapot->render();

	glUniform3f(glGetUniformLocation(sc.rendering_program, "Material.Ka"), 0.2f, 0.8f, 0.2f);
	glUniform3f(glGetUniformLocation(sc.rendering_program, "Material.Kd"), 0.5f, 0.5f, 0.5f);
	glUniform3f(glGetUniformLocation(sc.rendering_program, "Material.Ks"), 0.1f, 0.1f, 0.1f);
	glUniform1f(glGetUniformLocation(sc.rendering_program, "Material.Shininess"), 100.0f);
	m2 = translate(mat4(1.0), vec3(-2.0, 0.0, -4.0));
	m2 = rotate(m2, (float)radians(-90.0), vec3(1.0, 0.0, 0.0));
	m2 = scale(m2, vec3(0.3, 0.3, 0.3));
	glUniformMatrix4fv(sc.m_loc, 1, GL_FALSE, value_ptr(m2));
	glUniform4f(sc.color_loc, 0.2, 0.0, 0.7, 1.0);
	sc.teapot->render();

	glUniform3f(glGetUniformLocation(sc.rendering_program, "Material.Ka"), 0.2f, 0.3f, 0.6f);
	glUniform3f(glGetUniformLocation(sc.rendering_program, "Material.Kd"), 0.8f, 0.8f, 0.8f);
	glUniform3f(glGetUniformLocation(sc.rendering_program, "Material.Ks"), 0.5f, 0.5f, 0.5f);
	glUniform1f(glGetUniformLocation(sc.rendering_program, "Material.Shininess"), 20.0f);
	m2 = translate(mat4(1.0), vec3(0.0, 0.0, -4.0));
	m2 = rotate(m2, (float)radians(-90.0), vec3(1.0, 0.0, 0.0));
	m2 = scale(m2, vec3(0.3, 0.3, 0.3));
	glUniformMatrix4fv(sc.m_loc, 1, GL_FALSE, value_ptr(m2));
	glUniform4f(sc.color_loc, 0.7, 0.3, 0.3, 1.0);
	sc.teapot->render();

	glUniform3f(glGetUniformLocation(sc.rendering_program, "Material.Ka"), 0.9f, 0.9f, 0.9f);
	glUniform3f(glGetUniformLocation(sc.rendering_program, "Material.Kd"), 0.7f, 0.7f, 0.7f);
	glUniform3f(glGetUniformLocation(sc.rendering_program, "Material.Ks"), 0.1f, 0.1f, 0.1f);
	glUniform1f(glGetUniformLocation(sc.rendering_program, "Material.Shininess"), 180.0f);
	// floor
	glUniformMatrix4fv(sc.m_loc, 1, GL_FALSE, value_ptr(mat4(1.0)));
	glUniform4f(sc.color_loc, 0.3, 0.3, 0.3, 1.0);
	sc.plane->render();
	
	glutSwapBuffers();
}

void shutdown()
{
	glDeleteProgram(sc.rendering_program);
}

int main(int argc, char **argv)
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH);
	glutInitWindowSize(sc.width, sc.height);
	glutCreateWindow("Phong Shading");
	glewInit();

	version();
	startup();
	glutDisplayFunc(render);
	glutIdleFunc(idle);

	glutMainLoop();
	shutdown();
	return 0;
}
