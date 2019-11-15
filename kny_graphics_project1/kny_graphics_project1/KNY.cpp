#include "cg.h"
#include "time.h"

void startup();
void render();
void shutdown();

struct scene {
	int width = 800, height = 800;
	GLuint rendering_program;
	GLuint vertex_array_object;

	vec4 vertices[600];
	unsigned int indices[100];
	int num, numIndex;

	mat4 model;
	GLint mv_location;
	int color;
	float down, move, theta; //떨어지는 속도, 움직이는 폭, 회전속도
};

scene sc;
mat4 snow[300]; //눈송이 300개
float xPos[300]; //눈송이 x좌표
float snowSize[300]; //눈송이 크기
int moveDir = -1; //흔들리는 방향

void make_model()
{
	sc.num = 0;
	sc.vertices[sc.num] = vec4(0.0546875, 0.8359375, 0.0, 1.0); sc.num++; //0
	sc.vertices[sc.num] = vec4(-0.0625, 0.8359375, 0.0, 1.0); sc.num++; //1 
	sc.vertices[sc.num] = vec4(-0.0625, -0.8359375, 0.0, 1.0); sc.num++; //2
	sc.vertices[sc.num] = vec4(0.0546875, -0.8359375, 0.0, 1.0); sc.num++; //3
	sc.vertices[sc.num] = vec4(0.6796875, 0.4609375, 0.0, 1.0); sc.num++; //4
	sc.vertices[sc.num] = vec4(0.7421875, 0.359375, 0.0, 1.0); sc.num++; //5
	sc.vertices[sc.num] = vec4(-0.75, -0.3671875, 0.0, 1.0); sc.num++; //6
	sc.vertices[sc.num] = vec4(-0.6953125, -0.46484375, 0.0, 1.0); sc.num++; //7
	sc.vertices[sc.num] = vec4(0.6796875, -0.46484375, 0.0, 1.0); sc.num++; //8
	sc.vertices[sc.num] = vec4(0.7421875, -0.3671875, 0.0, 1.0); sc.num++; //9
	sc.vertices[sc.num] = vec4(-0.6953125, 0.4609375, 0.0, 1.0); sc.num++; //10
	sc.vertices[sc.num] = vec4(-0.75, 0.359375, 0.0, 1.0); sc.num++; //11
	sc.vertices[sc.num] = vec4(-0.19140625, 0.70703125, 0.0, 1.0); sc.num++; //12
	sc.vertices[sc.num] = vec4(-0.27734375, 0.62109375, 0.0, 1.0); sc.num++; //13
	sc.vertices[sc.num] = vec4(0.0, 0.53515625, 0.0, 1.0); sc.num++; //14
	sc.vertices[sc.num] = vec4(0.0, 0.3515625, 0.0, 1.0); sc.num++; //15
	sc.vertices[sc.num] = vec4(0.19140625, 0.70703125, 0.0, 1.0); sc.num++; //16
	sc.vertices[sc.num] = vec4(0.27734375, 0.62109375, 0.0, 1.0); sc.num++; //17
	sc.vertices[sc.num] = vec4(-0.19140625, -0.70703125, 0.0, 1.0); sc.num++; //18
	sc.vertices[sc.num] = vec4(-0.27734375, -0.62109375, 0.0, 1.0); sc.num++; //19
	sc.vertices[sc.num] = vec4(0.0, -0.53515625, 0.0, 1.0); sc.num++; //20
	sc.vertices[sc.num] = vec4(0.0, -0.3515625, 0.0, 1.0); sc.num++; //21
	sc.vertices[sc.num] = vec4(0.19140625, -0.70703125, 0.0, 1.0); sc.num++; //22
	sc.vertices[sc.num] = vec4(0.27734375, -0.62109375, 0.0, 1.0); sc.num++; //23
	sc.vertices[sc.num] = vec4(0.39453125, 0.55078125, 0.0, 1.0); sc.num++; //24
	sc.vertices[sc.num] = vec4(0.4921875, 0.5078125, 0.0, 1.0); sc.num++; //25
	sc.vertices[sc.num] = vec4(0.45703125, 0.27734375, 0.0, 1.0); sc.num++; //26
	sc.vertices[sc.num] = vec4(0.296875, 0.17578125, 0.0, 1.0); sc.num++; //27
	sc.vertices[sc.num] = vec4(0.69921875, 0.19140625, 0.0, 1.0); sc.num++; //28
	sc.vertices[sc.num] = vec4(0.671875, 0.078125, 0.0, 1.0); sc.num++; //29
	sc.vertices[sc.num] = vec4(0.39453125, -0.55078125, 0.0, 1.0); sc.num++; //30
	sc.vertices[sc.num] = vec4(0.515625, -0.515625, 0.0, 1.0); sc.num++; //31
	sc.vertices[sc.num] = vec4(0.45703125, -0.25390625, 0.0, 1.0); sc.num++; //32
	sc.vertices[sc.num] = vec4(0.3046875, -0.17578125, 0.0, 1.0); sc.num++; //33
	sc.vertices[sc.num] = vec4(0.69921875, -0.19140625, 0.0, 1.0); sc.num++; //34
	sc.vertices[sc.num] = vec4(0.671875, -0.078125, 0.0, 1.0); sc.num++; //35
	sc.vertices[sc.num] = vec4(-0.51953125, -0.55078125, 0.0, 1.0); sc.num++; //36
	sc.vertices[sc.num] = vec4(-0.40625, -0.515625, 0.0, 1.0); sc.num++; //37
	sc.vertices[sc.num] = vec4(-0.45703125, -0.25390625, 0.0, 1.0); sc.num++; //38
	sc.vertices[sc.num] = vec4(-0.3046875, -0.17578125, 0.0, 1.0); sc.num++; //39
	sc.vertices[sc.num] = vec4(-0.71484375, -0.19140625, 0.0, 1.0); sc.num++; //40
	sc.vertices[sc.num] = vec4(-0.671875, -0.078125, 0.0, 1.0); sc.num++; //41
	sc.vertices[sc.num] = vec4(-0.51953125, 0.5078125, 0.0, 1.0); sc.num++; //42
	sc.vertices[sc.num] = vec4(-0.40625, 0.55078125, 0.0, 1.0); sc.num++; //43
	sc.vertices[sc.num] = vec4(-0.45703125, 0.27734375, 0.0, 1.0); sc.num++; //44
	sc.vertices[sc.num] = vec4(-0.3125, 0.17578125, 0.0, 1.0); sc.num++; //45
	sc.vertices[sc.num] = vec4(-0.71484375, 0.19140625, 0.0, 1.0); sc.num++; //46
	sc.vertices[sc.num] = vec4(-0.671875, 0.078125, 0.0, 1.0); sc.num++; //47

	sc.indices[0] = 0; 	sc.indices[1] = 1; 	sc.indices[2] = 3; //1
	sc.indices[3] = 3; 	sc.indices[4] = 1; 	sc.indices[5] = 2; //2
	sc.indices[6] = 4; 	sc.indices[7] = 5; 	sc.indices[8] = 6; //3
	sc.indices[9] = 6; 	sc.indices[10] = 5; 	sc.indices[11] = 7; //4
	sc.indices[12] = 8; sc.indices[13] = 9; 	sc.indices[14] = 10; //5
	sc.indices[15] = 10; sc.indices[16] = 11; 	sc.indices[17] = 8; //6
	sc.indices[18] = 12; sc.indices[19] = 13; 	sc.indices[20] = 14; //7
	sc.indices[21] = 15; sc.indices[22] = 13; 	sc.indices[23] = 14; //8
	sc.indices[24] = 14; sc.indices[25] = 15; 	sc.indices[26] = 16; //9
	sc.indices[27] = 15; sc.indices[28] = 17; 	sc.indices[29] = 16; //10
	sc.indices[30] = 18; sc.indices[31] = 19; 	sc.indices[32] = 20; //11
	sc.indices[33] = 19; sc.indices[34] = 21; 	sc.indices[35] = 20; //12
	sc.indices[36] = 20; sc.indices[37] = 21; 	sc.indices[38] = 22; //13
	sc.indices[39] = 21; sc.indices[40] = 22; 	sc.indices[41] = 23; //14
	sc.indices[42] = 24; sc.indices[43] = 25; 	sc.indices[44] = 26; //15
	sc.indices[45] = 24; sc.indices[46] = 26; 	sc.indices[47] = 27; //16
	sc.indices[48] = 26; sc.indices[49] = 27; 	sc.indices[50] = 28; //17
	sc.indices[51] = 27; sc.indices[52] = 28; 	sc.indices[53] = 29; //18
	sc.indices[54] = 30; sc.indices[55] = 31; 	sc.indices[56] = 32; //19
	sc.indices[57] = 30; sc.indices[58] = 32; 	sc.indices[59] = 33; //20
	sc.indices[60] = 32; sc.indices[61] = 33; 	sc.indices[62] = 34; //21
	sc.indices[63] = 34; sc.indices[64] = 35; 	sc.indices[65] = 33; //22
	sc.indices[66] = 36; sc.indices[67] = 37; 	sc.indices[68] = 38; //23
	sc.indices[69] = 38; sc.indices[70] = 37; 	sc.indices[71] = 39; //24
	sc.indices[72] = 38; sc.indices[73] = 39; 	sc.indices[74] = 40; //25
	sc.indices[75] = 41; sc.indices[76] = 40; 	sc.indices[77] = 39; //26
	sc.indices[78] = 42; sc.indices[79] = 43; 	sc.indices[80] = 44; //27
	sc.indices[81] = 43; sc.indices[82] = 44; 	sc.indices[83] = 45; //28
	sc.indices[84] = 44; sc.indices[85] = 45; 	sc.indices[86] = 46; //29
	sc.indices[87] = 45; sc.indices[88] = 46; 	sc.indices[89] = 47; //30

	sc.numIndex = 90;
}

void startup()
{
	//------------------------------------------------------------------------
	sc.rendering_program = compile_shaders("vproject1.glsl", "fproject1.glsl");

	glGenVertexArrays(1, &sc.vertex_array_object);
	glBindVertexArray(sc.vertex_array_object);

	GLuint buffer[1];

	make_model();

	for (int i = 0; i < 300; i++) {
		xPos[i] = rand() % 200 * 0.01 - 1; //x좌표 (-1 ~ 1)
		snowSize[i] = ( rand() % 200 - 100 ) * 0.001; //스케일 (-0.1 ~ 0.1)
	}

	glGenBuffers(1, buffer);
	glBindBuffer(GL_ARRAY_BUFFER, buffer[0]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(sc.vertices), sc.vertices, GL_STATIC_DRAW);
	GLuint vPosition = glGetAttribLocation(sc.rendering_program, "vPosition");
	glVertexAttribPointer(vPosition, 4, GL_FLOAT, GL_FALSE, 0, NULL);
	glEnableVertexAttribArray(vPosition);

	sc.model = mat4(1.0); sc.down = 0.0; sc.move = 0; sc.theta = 0;
	sc.mv_location = glGetUniformLocation(sc.rendering_program, "mv_matrix");
	glUniformMatrix4fv(sc.mv_location, 1, GL_FALSE, value_ptr(sc.model));
	sc.color = glGetUniformLocation(sc.rendering_program, "vColor");
}

void idle()
{
	sc.down -= 0.00001; //떨어짐

	if (sc.move < -0.05) moveDir = 1; //왼쪽 끝에서 방향 전환
	else if(sc.move > 0.05) moveDir = -1; //오른쪽 끝에서 방향 전환

	if (moveDir == -1) sc.move -= 0.0001; //왼쪽으로
	else if(moveDir == 1) sc.move += 0.0001; //오른쪽으로
	
	sc.theta += 0.03f; //회전

	glutPostRedisplay();
}

void render()
{
	glUseProgram(sc.rendering_program);

	const GLfloat color[] = { 0.4f, 0.6f, 1.0f, 0.8f };
	glClearBufferfv(GL_COLOR, 0, color);
	for (int i = 0; i < 300; i++) {
		glUniformMatrix4fv(sc.mv_location, 1, GL_FALSE, value_ptr(snow[i]));
		glUniform4f(sc.color, 1.0, 1.0, 1.0, 1.0); //흰색
		glDrawElements(GL_TRIANGLES, sc.numIndex, GL_UNSIGNED_INT, sc.indices);

		//x좌표 랜덤+좌우로 흔들림, 작을수록 빨리 떨어짐
		//스케일이 양수냐 음수냐에 따라 흔들리는 방향이 서로 반대
		snow[i] = translate(mat4(1.0),
			vec3(xPos[i]+sc.move*snowSize[i]*10, i*0.04+sc.down/abs(snowSize[i]), 0.0));

		//스케일 랜덤
		snow[i] = scale(snow[i], vec3(snowSize[i], snowSize[i], 0.0));

		//작을수록 빨리 회전
		snow[i] = rotate(snow[i], radians(sc.theta*0.1f/snowSize[i]), vec3(0.0, 0.0, 1.0));
	}
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


	glutInitContextVersion(3, 3);
	glutInitContextProfile(GLUT_CORE_PROFILE);
	glutInitContextFlags(GLUT_DEBUG);

	glutInitWindowSize(800, 800);
	glutCreateWindow("Snowing");
	glewInit();
	version();

	startup();
	glutDisplayFunc(render);
	glutIdleFunc(idle);

	glutMainLoop();
	shutdown();
	return 0;
}