#include "cg.h"
#include "camera.h"
#include "vboplane.h"
#include "vbocube.h"
#include "vbomesh.h"
#include "bmpreader.h"
#include "vbotorus.h"
void startup();
void render();
void shutdown();
void initBuffers();
extern void version();

struct scene {
	int width = 800, height = 600;
	GLuint rendering_program;
	GLuint vertex_array_object;


	VBOPlane *plane;
	VBOCube *cube;
	VBOTorus *torus;
	VBOMesh *car;
	VBOMesh *stem;
	VBOMesh *leaves;

	mat4 view, proj;
	GLint m_loc, v_loc, proj_loc, color_loc;
	float carpos, angle;
	GLuint tex_object[2];
	vector <mat4> mvs;
};
scene sc;

void generate_texture(GLubyte * data, int width, int height)
{
	int x, y;
	for (y = 0; y < height; y++)
	{
		for (x = 0; x < width; x++)
		{
			GLubyte c = (((x & 0x8) == 0) ^ ((y & 0x8) == 0))*255.0;
			data[(y * width + x) * 4 + 0] = c;
			data[(y * width + x) * 4 + 1] = c;
			data[(y * width + x) * 4 + 2] = c;
			data[(y * width + x) * 4 + 3] = 1.0f;
		}
	}
}

float randFloat() {
	return ((float)rand() / RAND_MAX);
}

void startup()
{
	glEnable(GL_DEPTH_TEST); glDepthFunc(GL_LEQUAL); glEnable(GL_SMOOTH);
	sc.rendering_program = compile_shaders("perfrag.glsl", "perfragf.glsl");
	glUseProgram(sc.rendering_program);

	sc.plane = new VBOPlane(18.0f, 15.0f, 1, 1);
	sc.cube = new VBOCube();
	sc.torus = new VBOTorus(1, 1, 50, 50);
	sc.car = new VBOMesh("car.obj", true);
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

	glUniform3f(glGetUniformLocation(sc.rendering_program, "Light.Intensity"), 0.1, 0.1, 0.1);
	glUniform3f(glGetUniformLocation(sc.rendering_program, "Spot[0].Intensity"), 0.4, 0.4, 0.3);
	glUniform1f(glGetUniformLocation(sc.rendering_program, "Spot[0].exponent"), 10.0f);
	glUniform1f(glGetUniformLocation(sc.rendering_program, "Spot[0].cutoff"), 20.0f);
	glUniform3f(glGetUniformLocation(sc.rendering_program, "Spot[1].Intensity"), 0.4, 0.4, 0.3);
	glUniform1f(glGetUniformLocation(sc.rendering_program, "Spot[1].exponent"), 10.0f);
	glUniform1f(glGetUniformLocation(sc.rendering_program, "Spot[1].cutoff"), 20.0f);

	//도로 텍스쳐
	glActiveTexture(GL_TEXTURE0);
	sc.tex_object[0] = BMPReader::loadTex("road.bmp");
	//트럭 텍스쳐
	glActiveTexture(GL_TEXTURE1);
	sc.tex_object[1] = BMPReader::loadTex("truck.bmp");
}

#define M_PI 3.141592
void idle() {
	sc.carpos += 0.03f;
	glutPostRedisplay();
}

int roadPos;
void render()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glClearColor(0.08f, 0.04f, 0.18f, 1.0f);

	glUseProgram(sc.rendering_program);

	//조명
	//ambient light
	glUniform4fv(glGetUniformLocation(sc.rendering_program, "Light.Position"),
		1, value_ptr(sc.view*vec4(0.0, 10.0, 0.0, 1.0f)));
	//spotlight1
	vec4 lightPos = vec4(0.0, 15.0f, -sc.carpos * 5, 1.0f);
	vec4 direction = sc.view*vec4(vec4(0.0, 0.0, sin(sc.carpos*M_PI*0.2) * 2 - sc.carpos * 5, 1.0f) -lightPos);
	glUniform4fv(glGetUniformLocation(sc.rendering_program, "Spot[0].Position"), 1, value_ptr(sc.view*lightPos));
	glUniform3f(glGetUniformLocation(sc.rendering_program, "Spot[0].direction"), direction[0], direction[1], direction[2]);
	//spotlight2
	vec4 lightPos2 = vec4(cos(sc.carpos*M_PI*0.1)*4.5, 15.0f, sin(sc.carpos*M_PI*0.1)*10 -sc.carpos * 5, 1.0f);
	vec4 direction2 = sc.view*vec4(vec4(0.0, 0.0, -2 - sc.carpos * 5, 1.0f) - lightPos);
	glUniform4fv(glGetUniformLocation(sc.rendering_program, "Spot[1].Position"), 1, value_ptr(sc.view*lightPos2));
	glUniform3f(glGetUniformLocation(sc.rendering_program, "Spot[1].direction"), direction2[0], direction2[1], direction2[2]);

	//카메라 이동
	sc.view = LookAt(vec3(cos(sc.carpos*M_PI*0.05)*5, 5.0f, 8.0f-sc.carpos*5),
					 vec3(-1, 0.0f, -sc.carpos*5),
					 vec3(0.0f, 1.0f, 0.0f));

	glUniformMatrix4fv(sc.v_loc, 1, GL_FALSE, value_ptr(sc.view));


	roadPos = -(2 + sc.carpos * 5) / 15 +3;
	//도로
	mat4 m1;
	for (int i = 0; i < 10; i++) {
		glUniform3f(glGetUniformLocation(sc.rendering_program, "Material.Ka"), 1.0f, 1.0f, 1.0f);
		glUniform3f(glGetUniformLocation(sc.rendering_program, "Material.Kd"), 0.0f, 0.0f, 0.0f);
		glUniform3f(glGetUniformLocation(sc.rendering_program, "Material.Ks"), 0.0f, 0.0f, 0.0f);
		glUniform1f(glGetUniformLocation(sc.rendering_program, "Material.Shininess"), 1.0f);
		glUniform1i(glGetUniformLocation(sc.rendering_program, "Tex1"), 0);
		m1 = translate(mat4(1.0), vec3(0.0, 0.0, -i*15 + roadPos*15));
		glUniformMatrix4fv(sc.m_loc, 1, GL_FALSE, value_ptr(m1));
		sc.plane->render();
	}
	roadPos = -(2 + sc.carpos * 5) / 5 + 6;
	//나무
	mat4 m2, m22;
	for (int i = 0; i < 30; i++) {
		
		glUniform3f(glGetUniformLocation(sc.rendering_program, "Material.Ka"), 0.2f, 0.8f, 0.0f);
		glUniform3f(glGetUniformLocation(sc.rendering_program, "Material.Kd"), 0.1f, 0.1f, 0.1f);
		glUniform3f(glGetUniformLocation(sc.rendering_program, "Material.Ks"), 0.1f, 0.1f, 0.1f);
		glUniform1f(glGetUniformLocation(sc.rendering_program, "Material.Shininess"), 1.0f);
		m2 = translate(mat4(1.0), vec3(-8.0, 2.0, -i * 5 + roadPos * 5));
		m2 = rotate(m2, (float)radians(-90.0), vec3(1.0, 0.0, 0.0));
		m2 = scale(m2, vec3(0.08, 0.08, 0.08));
		glUniformMatrix4fv(sc.m_loc, 1, GL_FALSE, value_ptr(m2));
		sc.leaves->render();

		glUniform3f(glGetUniformLocation(sc.rendering_program, "Material.Ka"), 0.3f, 0.1f, 0.1f);
		glUniform3f(glGetUniformLocation(sc.rendering_program, "Material.Kd"), 0.1f, 0.1f, 0.1f);
		glUniform3f(glGetUniformLocation(sc.rendering_program, "Material.Ks"), 0.1f, 0.1f, 0.1f);
		glUniform1f(glGetUniformLocation(sc.rendering_program, "Material.Shininess"), 1.0f);
		m22 = translate(mat4(1.0), vec3(-8.0, 0.5, -i * 5 + roadPos * 5));
		m22 = rotate(m22, (float)radians(-90.0), vec3(1.0, 0.0, 0.0));
		m22 = scale(m22, vec3(0.1, 0.1, 0.1));
		glUniformMatrix4fv(sc.m_loc, 1, GL_FALSE, value_ptr(m22));
		sc.stem->render();

		glUniform3f(glGetUniformLocation(sc.rendering_program, "Material.Ka"), 0.2f, 0.8f, 0.0f);
		glUniform3f(glGetUniformLocation(sc.rendering_program, "Material.Kd"), 0.1f, 0.1f, 0.1f);
		glUniform3f(glGetUniformLocation(sc.rendering_program, "Material.Ks"), 0.1f, 0.1f, 0.1f);
		glUniform1f(glGetUniformLocation(sc.rendering_program, "Material.Shininess"), 1.0f);
		m2 = translate(mat4(1.0), vec3(8.0, 2.0, -i * 5 + roadPos * 5));
		m2 = rotate(m2, (float)radians(-90.0), vec3(1.0, 0.0, 0.0));
		m2 = scale(m2, vec3(0.08, 0.08, 0.08));
		glUniformMatrix4fv(sc.m_loc, 1, GL_FALSE, value_ptr(m2));
		sc.leaves->render();

		glUniform3f(glGetUniformLocation(sc.rendering_program, "Material.Ka"), 0.3f, 0.1f, 0.1f);
		glUniform3f(glGetUniformLocation(sc.rendering_program, "Material.Kd"), 0.1f, 0.1f, 0.1f);
		glUniform3f(glGetUniformLocation(sc.rendering_program, "Material.Ks"), 0.1f, 0.1f, 0.1f);
		glUniform1f(glGetUniformLocation(sc.rendering_program, "Material.Shininess"), 1.0f);
		m22 = translate(mat4(1.0), vec3(8.0, 0.5, -i * 5 + roadPos * 5));
		m22 = rotate(m22, (float)radians(-90.0), vec3(1.0, 0.0, 0.0));
		m22 = scale(m22, vec3(0.1, 0.1, 0.1));
		glUniformMatrix4fv(sc.m_loc, 1, GL_FALSE, value_ptr(m22));
		sc.stem->render();
	}

	//경찰차1
	mat4 m4;
	glUniform3f(glGetUniformLocation(sc.rendering_program, "Material.Ka"), 0.0f, 0.0f, 0.3f);
	glUniform3f(glGetUniformLocation(sc.rendering_program, "Material.Kd"), 0.5f, 0.5f, 0.5f);
	glUniform3f(glGetUniformLocation(sc.rendering_program, "Material.Ks"), 0.9f, 0.9f, 0.9f);
	glUniform1f(glGetUniformLocation(sc.rendering_program, "Material.Shininess"), 10.0f);
	m4 = translate(mat4(1.0), vec3(sin(sc.carpos*M_PI*0.5-M_PI*0.1)*0.1-2, 1.1, 3 - sc.carpos * 5));
	sc.mvs.push_back(m4);
	m4 = rotate(m4, (float)radians(-90.0), vec3(1.0, 0.0, 0.0));
	m4 = scale(m4, vec3(0.1, 0.1, 0.1));
	glUniformMatrix4fv(sc.m_loc, 1, GL_FALSE, value_ptr(m4));
	sc.car->render();
	m4 = sc.mvs.back(); sc.mvs.pop_back();

	if(sin(sc.carpos*3)>=0)
		glUniform3f(glGetUniformLocation(sc.rendering_program, "Material.Ka"), 1.0f, 0.0f, 0.0f);
	else
		glUniform3f(glGetUniformLocation(sc.rendering_program, "Material.Ka"), 0.0f, 0.0f, 1.0f);
	glUniform3f(glGetUniformLocation(sc.rendering_program, "Material.Kd"), 0.5f, 0.5f, 0.5f);
	glUniform3f(glGetUniformLocation(sc.rendering_program, "Material.Ks"), 0.9f, 0.9f, 0.9f);
	glUniform1f(glGetUniformLocation(sc.rendering_program, "Material.Shininess"), 10.0f);
	m4 = translate(m4, vec3(0.0f, 1.0f, 0.0f));
	sc.mvs.push_back(m4);
	m4 = scale(m4, vec3(0.3, 0.3, 0.3));
	m4 = rotate(m4, (float)radians(sc.carpos * 1000), vec3(0.0, 1.0, 0.0));
	glUniformMatrix4fv(sc.m_loc, 1, GL_FALSE, value_ptr(m4));
	sc.cube->render();
	m4 = sc.mvs.back(); sc.mvs.pop_back();

	//경찰차2
	glUniform3f(glGetUniformLocation(sc.rendering_program, "Material.Ka"), 0.0f, 0.0f, 0.3f);
	glUniform3f(glGetUniformLocation(sc.rendering_program, "Material.Kd"), 0.5f, 0.5f, 0.5f);
	glUniform3f(glGetUniformLocation(sc.rendering_program, "Material.Ks"), 0.9f, 0.9f, 0.9f);
	glUniform1f(glGetUniformLocation(sc.rendering_program, "Material.Shininess"), 10.0f);
	m4 = translate(mat4(1.0), vec3(cos(sc.carpos*M_PI*0.5 - M_PI*0.1)*0.2 + 2, 1.1, 3 - sc.carpos * 5));
	sc.mvs.push_back(m4);
	m4 = rotate(m4, (float)radians(-90.0), vec3(1.0, 0.0, 0.0));
	m4 = scale(m4, vec3(0.1, 0.1, 0.1));
	glUniformMatrix4fv(sc.m_loc, 1, GL_FALSE, value_ptr(m4));
	sc.car->render();
	m4 = sc.mvs.back(); sc.mvs.pop_back();

	if (sin(sc.carpos * 3 + M_PI/4) >= 0)
		glUniform3f(glGetUniformLocation(sc.rendering_program, "Material.Ka"), 1.0f, 0.0f, 0.0f);
	else
		glUniform3f(glGetUniformLocation(sc.rendering_program, "Material.Ka"), 0.0f, 0.0f, 1.0f);
	glUniform3f(glGetUniformLocation(sc.rendering_program, "Material.Kd"), 0.5f, 0.5f, 0.5f);
	glUniform3f(glGetUniformLocation(sc.rendering_program, "Material.Ks"), 0.9f, 0.9f, 0.9f);
	glUniform1f(glGetUniformLocation(sc.rendering_program, "Material.Shininess"), 10.0f);
	m4 = translate(m4, vec3(0.0f, 1.0f, 0.0f));
	sc.mvs.push_back(m4);
	m4 = scale(m4, vec3(0.3, 0.3, 0.3));
	m4 = rotate(m4, (float)radians(sc.carpos * 1000), vec3(0.0, 1.0, 0.0));
	glUniformMatrix4fv(sc.m_loc, 1, GL_FALSE, value_ptr(m4));
	sc.cube->render();
	m4 = sc.mvs.back(); sc.mvs.pop_back();

	//경찰차3
	glUniform3f(glGetUniformLocation(sc.rendering_program, "Material.Ka"), 0.0f, 0.0f, 0.3f);
	glUniform3f(glGetUniformLocation(sc.rendering_program, "Material.Kd"), 0.5f, 0.5f, 0.5f);
	glUniform3f(glGetUniformLocation(sc.rendering_program, "Material.Ks"), 0.9f, 0.9f, 0.9f);
	glUniform1f(glGetUniformLocation(sc.rendering_program, "Material.Shininess"), 10.0f);
	m4 = translate(mat4(1.0), vec3(cos(sc.carpos*M_PI*0.8 - M_PI*0.05)*0.1, 1.1, 7 - sc.carpos * 5));
	sc.mvs.push_back(m4);
	m4 = rotate(m4, (float)radians(-90.0), vec3(1.0, 0.0, 0.0));
	m4 = scale(m4, vec3(0.1, 0.1, 0.1));
	glUniformMatrix4fv(sc.m_loc, 1, GL_FALSE, value_ptr(m4));
	sc.car->render();
	m4 = sc.mvs.back(); sc.mvs.pop_back();

	if (-sin(sc.carpos * 3) >= 0)
		glUniform3f(glGetUniformLocation(sc.rendering_program, "Material.Ka"), 1.0f, 0.0f, 0.0f);
	else
		glUniform3f(glGetUniformLocation(sc.rendering_program, "Material.Ka"), 0.0f, 0.0f, 1.0f);
	glUniform3f(glGetUniformLocation(sc.rendering_program, "Material.Kd"), 0.5f, 0.5f, 0.5f);
	glUniform3f(glGetUniformLocation(sc.rendering_program, "Material.Ks"), 0.9f, 0.9f, 0.9f);
	glUniform1f(glGetUniformLocation(sc.rendering_program, "Material.Shininess"), 10.0f);
	m4 = translate(m4, vec3(0.0f, 1.0f, 0.0f));
	sc.mvs.push_back(m4);
	m4 = scale(m4, vec3(0.3, 0.3, 0.3));
	m4 = rotate(m4, (float)radians(sc.carpos * 1000), vec3(0.0, 1.0, 0.0));
	glUniformMatrix4fv(sc.m_loc, 1, GL_FALSE, value_ptr(m4));
	sc.cube->render();
	m4 = sc.mvs.back(); sc.mvs.pop_back();
	
	//트럭
	mat4 m5;
	glUniform3f(glGetUniformLocation(sc.rendering_program, "Material.Ka"), 0.8f, 0.6f, 0.0f);
	glUniform3f(glGetUniformLocation(sc.rendering_program, "Material.Kd"), 0.5f, 0.5f, 0.5f);
	glUniform3f(glGetUniformLocation(sc.rendering_program, "Material.Ks"), 0.9f, 0.9f, 0.9f);
	glUniform1f(glGetUniformLocation(sc.rendering_program, "Material.Shininess"), 10.0f);
	glUniform1i(glGetUniformLocation(sc.rendering_program, "Tex1"), 1);
	m5 = translate(mat4(1.0), vec3(sin(sc.carpos*M_PI*0.3), 1.8, -4.1 - sc.carpos * 5));
	m5 = scale(m5, vec3(2.5, 1.5, 2.0));
	glUniformMatrix4fv(sc.m_loc, 1, GL_FALSE, value_ptr(m5));
	sc.cube->render();

	m5 = translate(mat4(1.0), vec3(sin(sc.carpos*M_PI*0.3), 0.7, -3 - sc.carpos * 5));
	m5 = scale(m5, vec3(2.5, 0.8, 4.2));
	glUniformMatrix4fv(sc.m_loc, 1, GL_FALSE, value_ptr(m5));
	sc.cube->render();
	//트럭 바퀴
	glUniform3f(glGetUniformLocation(sc.rendering_program, "Material.Ka"), 0.0f, 0.0f, 0.0f);
	glUniform3f(glGetUniformLocation(sc.rendering_program, "Material.Kd"), 0.5f, 0.5f, 0.5f);
	glUniform3f(glGetUniformLocation(sc.rendering_program, "Material.Ks"), 0.1f, 0.1f, 0.1f);
	glUniform1f(glGetUniformLocation(sc.rendering_program, "Material.Shininess"), 10.0f);
	m5 = translate(mat4(1.0), vec3(sin(sc.carpos*M_PI*0.3) + 1.25, 0.4, -2 - sc.carpos * 5));
	m5 = rotate(m5, (float)radians(90.0), vec3(0.0, 0.1, 0.0));
	m5 = scale(m5, vec3(0.2, 0.2, 0.2));
	glUniformMatrix4fv(sc.m_loc, 1, GL_FALSE, value_ptr(m5));
	sc.torus->render();

	m5 = translate(mat4(1.0), vec3(sin(sc.carpos*M_PI*0.3) + 1.25, 0.4, -4.5 - sc.carpos * 5));
	m5 = rotate(m5, (float)radians(-90.0), vec3(0.0, 0.1, 0.0));
	m5 = scale(m5, vec3(0.2, 0.2, 0.2));
	glUniformMatrix4fv(sc.m_loc, 1, GL_FALSE, value_ptr(m5));
	sc.torus->render();

	m5 = translate(mat4(1.0), vec3(sin(sc.carpos*M_PI*0.3) - 1.25, 0.4, -2 - sc.carpos * 5));
	m5 = rotate(m5, (float)radians(90.0), vec3(0.0, 0.1, 0.0));
	m5 = scale(m5, vec3(0.2, 0.2, 0.2));
	glUniformMatrix4fv(sc.m_loc, 1, GL_FALSE, value_ptr(m5));
	sc.torus->render();

	m5 = translate(mat4(1.0), vec3(sin(sc.carpos*M_PI*0.3) - 1.25, 0.4, -4.5 - sc.carpos * 5));
	m5 = rotate(m5, (float)radians(-90.0), vec3(0.0, 0.1, 0.0));
	m5 = scale(m5, vec3(0.2, 0.2, 0.2));
	glUniformMatrix4fv(sc.m_loc, 1, GL_FALSE, value_ptr(m5));
	sc.torus->render();

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
	glutCreateWindow("KNY_final");
	glewInit();

	version();
	startup();
	glutDisplayFunc(render);
	glutIdleFunc(idle);

	glutMainLoop();
	shutdown();
	return 0;
}
