in vec4 vPosition;

uniform mat4 mv_matrix;

void main()
{
	gl_Position = mv_matrix*vPosition;
}