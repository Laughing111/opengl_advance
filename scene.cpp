#include "scene.h"
#include "ggl.h"
#include "utils.h"
GLuint vbo,ebo;		//vertex buff object
GLuint program;   //声明一个shader程序
GLint positionLocation, modelMatrixLocation, viewMatrixLocation, projectionMatrixLocation;
glm::mat4 modelMatrix, viewMatrix, projectionMatrix;
float data[] = {
		-0.2f,-0.2f,0,1.0f,
		0.2f,-0.2f,0,1.0f,
		0.0f,0.2f,0,1.0f
};

unsigned short indexes[] = { 0,1,2 };
void Init()
{
	//glGenBuffers(1, &vbo);   //生成一个Vbo对象
	//glBindBuffer(GL_ARRAY_BUFFER, vbo);    //绑定vbo
	//glBufferData(GL_ARRAY_BUFFER, sizeof(data), data, GL_STATIC_DRAW);   //传递CPU上的内存 至vbo
	//glBindBuffer(GL_ARRAY_BUFFER, 0);  //清除CPU上的内存

	//glGenBuffers(1, &ebo);
	//glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);    //绑定ebo
	//glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned short) * 3, indexes, GL_STATIC_DRAW);   //传递CPU上的内存 至ebo
	//glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);    //清除CPU上的内存

	int fileSize = 0;
	unsigned char* shaderCode = LoadFileContent("Res/test.vs", fileSize);
	GLuint vsShader = CompileShader(GL_VERTEX_SHADER, (char*)shaderCode);
	delete shaderCode;
	shaderCode = LoadFileContent("Res/test.fs", fileSize);
	GLuint fsShader = CompileShader(GL_FRAGMENT_SHADER, (char*)shaderCode);
	delete shaderCode;

	program = CreateProgram(vsShader, fsShader);
	glDeleteShader(vsShader);
	glDeleteShader(fsShader);

	positionLocation = glGetAttribLocation(program, "position");
	modelMatrixLocation = glGetUniformLocation(program, "ModelMatrix");
	viewMatrixLocation = glGetUniformLocation(program, "ViewMatrix");
	projectionMatrixLocation = glGetUniformLocation(program, "ProjectionMatrix");
	modelMatrix = glm::translate(0.f, 0.f, -0.6f);


}

void SetViewPortSize(float width, float height)
{
	projectionMatrix = glm::perspective(60.f, width / height, 0.1f, 1000.f);
}

void Draw()
{
	float frameTime = GetFrameTime();
	glClearColor(0.f, 0.f, 0.f, 1.f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glUseProgram(program);
	glUniformMatrix4fv(modelMatrixLocation, 1, GL_FALSE, glm::value_ptr(modelMatrix));
	glUniformMatrix4fv(viewMatrixLocation, 1, GL_FALSE, glm::value_ptr(viewMatrix));
	glUniformMatrix4fv(projectionMatrixLocation, 1, GL_FALSE, glm::value_ptr(projectionMatrix));
	
	glGenBuffers(1, &vbo);   //生成一个Vbo对象
	glBindBuffer(GL_ARRAY_BUFFER, vbo);    //绑定vbo
	glBufferData(GL_ARRAY_BUFFER, sizeof(data), data, GL_STATIC_DRAW);   //传递CPU上的内存 至vbo
	glEnableVertexAttribArray(positionLocation);
	glVertexAttribPointer(positionLocation, 4, GL_FLOAT, GL_FALSE, sizeof(float) * 4,0);
	//glDrawArrays(GL_TRIANGLES, 0, 3);


	glGenBuffers(1, &ebo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);    //绑定ebo
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned short) * 3, indexes, GL_STATIC_DRAW);   //传递CPU上的内存 至ebo
	glDrawElements(GL_TRIANGLES, 3, GL_UNSIGNED_SHORT, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glUseProgram(0);

}