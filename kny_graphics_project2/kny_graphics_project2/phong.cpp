#include "cg.h"
#include "camera.h"
#include "vboplane.h"
#include "vbomesh.h"
void startup();
void render();
void shutdown();
extern void version();

struct scene {
	int width = 800, height = 600;
	GLuint rendering_program;
	GLuint vertex_array_object;

	// 모델링 변수
	VBOPlane *plane;
	VBOPlane *plane2;
	VBOMesh *car;
	VBOMesh *maincar;
	VBOMesh *stem;
	VBOMesh *leaves;
	// 뷰잉 변수
	mat4 view, proj;
	GLint m_loc, v_loc, proj_loc, color_loc;
	float carpos;
};
scene sc;

void startup()
{
	glEnable(GL_DEPTH_TEST); glDepthFunc(GL_LEQUAL);
	sc.rendering_program = compile_shaders("perfrag.glsl", "perfragf.glsl");
	glUseProgram(sc.rendering_program);

	sc.plane = new VBOPlane(15.0f, 200.0f, 1, 1);
	sc.plane2 = new VBOPlane(0.5f, 200.0f, 1, 1);
	sc.car = new VBOMesh("car.obj", true);
	sc.maincar = new VBOMesh("maincar.obj", true);
	sc.stem = new VBOMesh("stem.obj", true);
	sc.leaves = new VBOMesh("leaves.obj", true);
	sc.carpos = 0.0f;

	sc.view = LookAt(vec3(0.0f, 2.0f, 8.0f), vec3(0.0f, 0.0f, 0.0f), vec3(0.0f, 1.0f, 0.0f));

	float cameraZ = sc.height * 0.5 / tan(radians(0.5*50.0));
	sc.proj = Perspective(50.0f, (float)sc.width / (float)sc.height, (float)0.001*cameraZ, (float)10.0*cameraZ);	

	sc.m_loc = glGetUniformLocation(sc.rendering_program, "m_matrix");
	sc.v_loc = glGetUniformLocation(sc.rendering_program, "v_matrix");
	sc.proj_loc = glGetUniformLocation(sc.rendering_program, "proj_matrix");
	sc.color_loc = glGetUniformLocation(sc.rendering_program, "color");

	glUniformMatrix4fv(sc.m_loc, 1, GL_FALSE, value_ptr(mat4(1.0)));
	glUniformMatrix4fv(sc.v_loc, 1, GL_FALSE, value_ptr(sc.view));
	glUniformMatrix4fv(sc.proj_loc, 1, GL_TRUE, value_ptr(sc.proj));

	glUniform3f(glGetUniformLocation(sc.rendering_program, "Light[0].Intensity"), 0.1, 0.0, 0.0);
	glUniform3f(glGetUniformLocation(sc.rendering_program, "Light[1].Intensity"), 0.3, 0.3, 0.3);
}

void idle() {
	sc.carpos += 0.001f;
	glutPostRedisplay();
}

double direct, colorInt;
void render()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glClearColor(0.05f, 0.05f, 0.2f, 1.0f);

	glUseProgram(sc.rendering_program);

	//조명
	glUniform4fv(glGetUniformLocation(sc.rendering_program, "Light[0].Position"),
		1, value_ptr(sc.view*vec4(sin(2.5 + sc.carpos*3.1415926*0.5) * 3, 0.9, -2 - sc.carpos * 5, 1.0f)));
	glUniform4fv(glGetUniformLocation(sc.rendering_program, "Light[1].Position"),
		1, value_ptr(sc.view*vec4(20.0f * cos(sc.carpos * 5), 5.0f, -100*sin(sc.carpos*5)-90, 1.0f)));

	//카메라 이동
	sc.view = LookAt(vec3(sin(2.5 + sc.carpos*3.1415926*0.5), 3.0f, 8.0f-sc.carpos*5),
					 vec3(-1+sin(2.5 + sc.carpos*3.1415926*0.5), 0.0f, -sc.carpos*5),
					 vec3(0.0f, 1.0f, 0.0f));

	glUniformMatrix4fv(sc.v_loc, 1, GL_FALSE, value_ptr(sc.view));


	//도로
	mat4 m1;
	glUniform3f(glGetUniformLocation(sc.rendering_program, "Material.Ka"), 0.1f, 0.1f, 0.1f);
	glUniform3f(glGetUniformLocation(sc.rendering_program, "Material.Kd"), 0.1f, 0.1f, 0.1f);
	glUniform3f(glGetUniformLocation(sc.rendering_program, "Material.Ks"), 0.9f, 0.9f, 0.9f);
	glUniform1f(glGetUniformLocation(sc.rendering_program, "Material.Shininess"), 10.0f);
	m1 = translate(mat4(1.0), vec3(0.0, 0.0, -90.0));
	glUniformMatrix4fv(sc.m_loc, 1, GL_FALSE, value_ptr(m1));
	sc.plane->render();

	//인도
	glUniform3f(glGetUniformLocation(sc.rendering_program, "Material.Ka"), 0.5f, 0.5f, 0.5f);
	glUniform3f(glGetUniformLocation(sc.rendering_program, "Material.Kd"), 0.5f, 0.5f, 0.5f);
	glUniform3f(glGetUniformLocation(sc.rendering_program, "Material.Ks"), 0.1f, 0.1f, 0.1f);
	glUniform1f(glGetUniformLocation(sc.rendering_program, "Material.Shininess"), 1.0f);
	m1 = translate(mat4(1.0), vec3(0.0, -0.01, -90.0));
	m1 = scale(m1, vec3(1.2, 1.2, 1.2));
	glUniformMatrix4fv(sc.m_loc, 1, GL_FALSE, value_ptr(m1));
	sc.plane->render();

	//중앙선
	glUniform3f(glGetUniformLocation(sc.rendering_program, "Material.Ka"), 0.8f, 0.8f, 0.1f);
	glUniform3f(glGetUniformLocation(sc.rendering_program, "Material.Kd"), 0.5f, 0.5f, 0.5f);
	glUniform3f(glGetUniformLocation(sc.rendering_program, "Material.Ks"), 0.1f, 0.1f, 0.1f);
	glUniform1f(glGetUniformLocation(sc.rendering_program, "Material.Shininess"), 1.0f);
	m1 = translate(mat4(1.0), vec3(0.0, 0.01, -90.0));
	glUniformMatrix4fv(sc.m_loc, 1, GL_FALSE, value_ptr(m1));
	sc.plane2->render();

	//나무
	mat4 m2, m22;
	for (int i = 0; i < 100; i++) {
		direct = i % 2;
		colorInt = i % 3;
		glUniform3f(glGetUniformLocation(sc.rendering_program, "Material.Ka"), 0.9 - colorInt * 0.3, 0.1+colorInt*0.3, 0.0f);
		glUniform3f(glGetUniformLocation(sc.rendering_program, "Material.Kd"), 0.1f, 0.1f, 0.1f);
		glUniform3f(glGetUniformLocation(sc.rendering_program, "Material.Ks"), 0.1f, 0.1f, 0.1f);
		glUniform1f(glGetUniformLocation(sc.rendering_program, "Material.Shininess"), 1.0f);
		m2 = translate(mat4(1.0), vec3(8.0 - 16.0 * direct, 2.0, -i*2.0));
		m2 = rotate(m2, (float)radians(-90.0), vec3(1.0, 0.0, 0.0));
		m2 = scale(m2, vec3(0.05, 0.05, 0.05));
		glUniformMatrix4fv(sc.m_loc, 1, GL_FALSE, value_ptr(m2));
		if(-i*2.0 < -2 - sc.carpos * 5 + 10 && -i*2.0 > -2-sc.carpos*5-50)
			sc.leaves->render();
		
		glUniform3f(glGetUniformLocation(sc.rendering_program, "Material.Ka"), 0.3f, 0.1f, 0.1f);
		glUniform3f(glGetUniformLocation(sc.rendering_program, "Material.Kd"), 0.1f, 0.1f, 0.1f);
		glUniform3f(glGetUniformLocation(sc.rendering_program, "Material.Ks"), 0.1f, 0.1f, 0.1f);
		glUniform1f(glGetUniformLocation(sc.rendering_program, "Material.Shininess"), 1.0f);
		m22 = translate(mat4(1.0), vec3(8.0 - 16.0 * direct, 0.5, -i*2.0));
		m22 = rotate(m22, (float)radians(-90.0), vec3(1.0, 0.0, 0.0));
		m22 = scale(m22, vec3(0.05, 0.05, 0.05));
		glUniformMatrix4fv(sc.m_loc, 1, GL_FALSE, value_ptr(m22));
		if (-i*2.0 < -2 - sc.carpos * 5 + 10 && -i*2.0 > -2 - sc.carpos * 5 - 50)
			sc.stem->render();
	}

	//도주 차량
	mat4 m3;
	glUniform3f(glGetUniformLocation(sc.rendering_program, "Material.Ka"), 0.8f, 0.0f, 0.0f);
	glUniform3f(glGetUniformLocation(sc.rendering_program, "Material.Kd"), 0.1f, 0.1f, 0.1f);
	glUniform3f(glGetUniformLocation(sc.rendering_program, "Material.Ks"), 0.9f, 0.9f, 0.9f);
	glUniform1f(glGetUniformLocation(sc.rendering_program, "Material.Shininess"), 10.0f);
	m3 = translate(mat4(1.0), vec3(sin(2.5+sc.carpos*3.1415926*0.5)*3, 1.1, -2-sc.carpos*5));
	m3 = rotate(m3, (float)radians(-90.0), vec3(1.0, 0.0, 0.0));
	m3 = scale(m3, vec3(0.1, 0.1, 0.1));
	glUniformMatrix4fv(sc.m_loc, 1, GL_FALSE, value_ptr(m3));
	sc.maincar->render();

	//방해 차량
	mat4 m4;
	for (int i = 0; i < 20; i++) {
		direct = i % 2;
		glUniform3f(glGetUniformLocation(sc.rendering_program, "Material.Ka"), direct*(double)i/40 + 0.5, 1 - (double)i/20, (double)i/20);
		glUniform3f(glGetUniformLocation(sc.rendering_program, "Material.Kd"), 1.0f, 1.0f, 1.0f);
		glUniform3f(glGetUniformLocation(sc.rendering_program, "Material.Ks"), 0.9f, 0.9f, 0.9f);
		glUniform1f(glGetUniformLocation(sc.rendering_program, "Material.Shininess"), 10.0f);
		m4 = translate(mat4(1.0), vec3(3.0 - 6.0 * direct, 0.9, -10.0 -i*10.0));
		m4 = rotate(m4, (float)radians(-90.0), vec3(1.0, 0.0, 0.0));
		m4 = scale(m4, vec3(0.1, 0.1, 0.1));
		glUniformMatrix4fv(sc.m_loc, 1, GL_FALSE, value_ptr(m4));
		if (-10.0 - i*10.0 < -2 - sc.carpos * 5 + 10 && -10.0 - i*10.0 > -2 - sc.carpos * 5 - 50)
			sc.car->render();
	}

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
	glutCreateWindow("KNY_2nd_homework");
	glewInit();

	version();
	startup();
	glutDisplayFunc(render);
	glutIdleFunc(idle);

	glutMainLoop();
	shutdown();
	return 0;
}
