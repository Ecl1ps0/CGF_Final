#include <GL/glew.h>
#include <GL/freeglut.h>
#include <GLFW/glfw3.h>
#include <vector>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <iostream>
#include "stb_image.h"
#define TINYOBJLOADER_IMPLEMENTATION
#include "tinyobjloader.h"

GLuint vbo, ibo, vbo1, fbo;
GLuint shp, shp1;
GLuint posloc, colorloc, colloc, matloc, rmatloc, normloc, tcloc, posloc1, matloc1, colorloc1;
GLuint tex;
glm::mat4 transform, rtransform;
float a = 0;

struct Vertex
{
	glm::vec3 position;
	glm::vec3 color;
	glm::vec3 normal;
	glm::vec2 texCoord;
};

std::vector<Vertex> vertices;

std::vector<unsigned int> indices;

float rotationSpeed = 0.0005f;
float translationSpeed = 0.0005f;

void handleKeyboardInput(GLFWwindow* window)
{
	//TRANSLATION ANIMATION
	if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS)
	{
		transform = glm::translate(transform, glm::vec3(0.0f, 0.0f, -translationSpeed));
	}
	if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS)
	{
		transform = glm::translate(transform, glm::vec3(0.0f, 0.0f, translationSpeed));
	}
	if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS)
	{
		transform = glm::translate(transform, glm::vec3(-translationSpeed, 0.0f, 0.0f));
	}
	if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS)
	{
		transform = glm::translate(transform, glm::vec3(translationSpeed, 0.0f, 0.0f));
	}
	if (glfwGetKey(window, GLFW_KEY_Z) == GLFW_PRESS)
	{
		transform = glm::translate(transform, glm::vec3(0.0f, translationSpeed, 0.0f));
	}
	if (glfwGetKey(window, GLFW_KEY_X) == GLFW_PRESS)
	{
		transform = glm::translate(transform, glm::vec3(0.0f, -translationSpeed, 0.0f));
	}

	//ROTATION ANIMATION
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
	{
		transform = glm::rotate(transform, rotationSpeed, glm::vec3(0.0f, 1.0f, 0.0f));
	}
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
	{
		transform = glm::rotate(transform, -rotationSpeed, glm::vec3(0.0f, 1.0f, 0.0f));
	}
	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
	{
		transform = glm::rotate(transform, rotationSpeed, glm::vec3(1.0f, 0.0f, 0.0f));
	}
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
	{
		transform = glm::rotate(transform, -rotationSpeed, glm::vec3(1.0f, 0.0f, 0.0f));
	}
}

void mydisplay(GLFWwindow* window)
{
	glBindFramebuffer(GL_FRAMEBUFFER, fbo);

	glClearColor(1, 1, 1, 1);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	handleKeyboardInput(window);
	glUseProgram(shp1);
	glUniformMatrix4fv(matloc1, 1, GL_FALSE, glm::value_ptr<float>(transform));

	glEnableVertexAttribArray(posloc1);
	glEnableVertexAttribArray(colorloc1);

	glBindBuffer(GL_ARRAY_BUFFER, vbo1);
	glDrawArrays(GL_TRIANGLES, 0, 6);

	glDisableVertexAttribArray(posloc1);
	glDisableVertexAttribArray(colorloc1);

	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	glClearColor(0.4, 0.2, 0.3, 1);
	glUseProgram(shp);
	glUniformMatrix4fv(matloc, 1, GL_FALSE, glm::value_ptr<float>(transform));
	glUniformMatrix4fv(rmatloc, 1, GL_FALSE, glm::value_ptr<float>(rtransform));

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glEnableVertexAttribArray(posloc);
	glEnableVertexAttribArray(colorloc);
	glEnableVertexAttribArray(normloc);
	glEnableVertexAttribArray(tcloc);

	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);
	glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, nullptr);

	glDisableVertexAttribArray(posloc);
	glDisableVertexAttribArray(colorloc);
	glDisableVertexAttribArray(normloc);
	glDisableVertexAttribArray(tcloc);
}

int main(int argc, char** argv)
{
	GLFWwindow* window;

	if (!glfwInit())
		exit(EXIT_FAILURE);

	//Version of glfw = 2.0
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 2);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);

	window = glfwCreateWindow(512, 512, argv[0], NULL, NULL);

	if (!window)
	{
		glfwTerminate();
		exit(EXIT_FAILURE);
	}

	glfwMakeContextCurrent(window);

	glewInit();
	glEnable(GL_DEPTH_TEST);

	size_t n = 20;
	float r = 0.5;
	float R = 1.5;
	float pi = 3.14159265;
	float h = 0.3;
	int currentindex = 0;

	std::string inputfile = "C:\\D.K\\3rd course\\Graphic\\Axe\\TV.obj";
	tinyobj::attrib_t attrib;
	std::vector<tinyobj::shape_t> shapes;
	std::vector<tinyobj::material_t> materials;

	std::string warn;
	std::string err;

	bool ret = tinyobj::LoadObj(&attrib, &shapes, &materials, &warn, &err, inputfile.c_str());

	if (!warn.empty())
	{
		std::cout << warn << std::endl;
	}

	if (!err.empty())
	{
		std::cout << err << std::endl;
		return 0;
	}

	if (!ret)
	{
		exit(1);
	}

	for (size_t s = 0; s < shapes.size(); s++) {
		size_t index_offset = 0;
		for (size_t f = 0; f < shapes[s].mesh.num_face_vertices.size(); f++) {
			size_t fv = size_t(shapes[s].mesh.num_face_vertices[f]);

			for (size_t v = 0; v < fv; v++) {
				Vertex v1;

				tinyobj::index_t idx = shapes[s].mesh.indices[index_offset + v];

				tinyobj::real_t vx = attrib.vertices[3 * size_t(idx.vertex_index) + 0];
				tinyobj::real_t vy = attrib.vertices[3 * size_t(idx.vertex_index) + 1];
				tinyobj::real_t vz = attrib.vertices[3 * size_t(idx.vertex_index) + 2];
				v1.position = glm::vec3(vx, vy, vz);

				if (idx.normal_index >= 0) {
					tinyobj::real_t nx = attrib.normals[3 * size_t(idx.normal_index) + 0];
					tinyobj::real_t ny = attrib.normals[3 * size_t(idx.normal_index) + 1];
					tinyobj::real_t nz = attrib.normals[3 * size_t(idx.normal_index) + 2];
					v1.normal = glm::vec3(nx, ny, nz);
				}

				if (idx.texcoord_index >= 0) {
					tinyobj::real_t tx = attrib.texcoords[2 * size_t(idx.texcoord_index) + 0];
					tinyobj::real_t ty = attrib.texcoords[2 * size_t(idx.texcoord_index) + 1];
					v1.texCoord = glm::vec2(tx, 1.0f - ty);
				}
				else {
					v1.texCoord = glm::vec2(0.0f, 0.0f);
				}

				vertices.push_back(v1);
				indices.push_back(index_offset + v);
			}
			index_offset += fv;
		}
	}

	glGenBuffers(1, &vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(vertices[0]),
		vertices.data(), GL_STATIC_DRAW);

	glGenBuffers(1, &ibo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(indices[0]),
		indices.data(), GL_STATIC_DRAW);

	shp = glCreateProgram();
	GLuint vsh = glCreateShader(GL_VERTEX_SHADER);
	const GLchar* vshcode = ""
		"#version 450 \n"
		"layout(location = 0) in vec3 pos;"
		"layout(location = 1) in vec3 color;"
		"layout(location = 2) in vec3 normal;"
		"layout(location = 3) in vec2 tcoord;"
		"out vec3 vcolor;"
		"out vec3 vnormal;"
		"out vec3 vpos;"
		"out vec2 vtcoord;"
		"layout(location = 4) uniform mat4 transform;"
		"layout(location = 5) uniform mat4 rtransform;"
		"layout(location = 6) uniform mat4 projection;"
		"void main() {"
		"vcolor = color;"
		"vtcoord = tcoord;"
		"vnormal = (rtransform*vec4(normal,1)).xyz;"
		"vpos = (transform*vec4(pos,1)).xyz;"
		"gl_Position = projection*transform*vec4(pos, 0.3);}";

	GLint vshlen = strlen(vshcode);
	glShaderSource(vsh, 1, &vshcode, &vshlen);
	glCompileShader(vsh);
	glAttachShader(shp, vsh);

	GLuint fsh = glCreateShader(GL_FRAGMENT_SHADER);
	const GLchar* fshcode = ""
		"#version 450 \n"
		"in vec3 vcolor;"
		"in vec3 vnormal;"
		"in vec3 vpos;"
		"in vec2 vtcoord;"
		"layout(location = 7) uniform sampler2D tex1;"
		"out vec4 FragColor;"
		"void main() {"
		"vec3 lightpos = vec3(1, 2, 2);"
		"vec3 campos = vec3(1.2, 2, 4);"
		"vec3 camdir = normalize(campos - vpos);"
		"float lighting = max(dot(normalize(vnormal), normalize(lightpos - vpos)), 0);"
		"vec3 r = reflect(-normalize(lightpos - vpos), normalize(vnormal));"
		"float specularity = max(pow(dot(camdir, r), 15), 0);"
		"vec3 difcolor = texture(tex1, vtcoord).xyz;"
		"FragColor = vec4(lighting*difcolor + specularity*vec3(1), 1);}";
	GLint fshlen = strlen(fshcode);
	glShaderSource(fsh, 1, &fshcode, &fshlen);
	glCompileShader(fsh);

	GLint isCompiled = 0;
	glGetShaderiv(fsh, GL_COMPILE_STATUS, &isCompiled);
	if (isCompiled == GL_FALSE)
	{
		GLint maxLength = 0;
		glGetShaderiv(fsh, GL_INFO_LOG_LENGTH, &maxLength);

		std::vector<GLchar> errorLog(maxLength);
		glGetShaderInfoLog(fsh, maxLength, &maxLength, &errorLog[0]);

		printf("%s", errorLog.data());

		glDeleteShader(fsh);
		return 0;
	}

	glAttachShader(shp, fsh);

	glLinkProgram(shp);
	glValidateProgram(shp);
	glUseProgram(shp);

	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	posloc = 0;
	glVertexAttribPointer(posloc, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), 0);

	colorloc = 1;
	glVertexAttribPointer(colorloc, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)(sizeof(glm::vec3)));

	normloc = 2;
	glVertexAttribPointer(normloc, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)(2 * sizeof(glm::vec3)));

	tcloc = 3;
	glVertexAttribPointer(tcloc, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)(3 * sizeof(glm::vec3)));

	transform = glm::mat4(1);
	matloc = 4;
	rtransform = glm::mat4(1);
	rmatloc = 5;

	glm::mat4 projection = glm::mat4(1);
	projection = glm::perspective(45.0f, 1.f, 0.01f, 100.0f);
	glm::mat4 lookat = glm::lookAt(glm::vec3(-3), glm::vec3(0), glm::vec3(0, 1, 0));

	projection = projection * lookat;
	GLuint projloc = 6;
	glUniformMatrix4fv(matloc, 1, GL_FALSE, glm::value_ptr<float>(transform));
	glUniformMatrix4fv(rmatloc, 1, GL_FALSE, glm::value_ptr<float>(rtransform));
	glUniformMatrix4fv(projloc, 1, GL_FALSE, glm::value_ptr<float>(projection));

	int width = 512;
	int height = 512;
	int comp;

	glGenFramebuffers(1, &fbo);
	glBindFramebuffer(GL_FRAMEBUFFER, fbo);

	glGenTextures(1, &tex);
	glBindTexture(GL_TEXTURE_2D, tex);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, tex, 0);

	GLuint rbo;
	glGenRenderbuffers(1, &rbo);
	glBindRenderbuffer(GL_RENDERBUFFER, rbo);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, width, height);

	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, rbo);

	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, tex);

	GLuint texloc = 7;
	glUniform1i(texloc, 0);

	std::vector<float> vertices1 =
	{
		1,0,1,    0,0,0.8,
		1,1,1,    0,0.6,1,
		0,0,0,    0,1,0,

		1,20,3,  0.5,0.3,1,
		0,1,10,   0,0,1,
		0,0,0.1,  0,0.3,1
	};
	glGenBuffers(1, &vbo1);
	glBindBuffer(GL_ARRAY_BUFFER, vbo1);
	glBufferData(GL_ARRAY_BUFFER, vertices1.size() * sizeof(vertices1[0]), vertices1.data(), GL_STATIC_DRAW);

	shp1 = glCreateProgram();
	GLuint vsh1 = glCreateShader(GL_VERTEX_SHADER);
	const GLchar* vsh1code =
		"#version 450\n"
		"layout(location = 10) in vec3 pos;"
		"layout(location = 12) in vec3 color;"
		"layout(location = 11) uniform mat4 transform;"
		"out vec3 vcolor;"
		"void main(){"
		"vcolor = color;"
		"gl_Position = transform*vec4(pos, 2);}";
	GLint vsh1len = strlen(vsh1code);
	glShaderSource(vsh1, 1, &vsh1code, &vsh1len);
	glCompileShader(vsh1);
	glAttachShader(shp1, vsh1);

	isCompiled = 0;
	glGetShaderiv(vsh1, GL_COMPILE_STATUS, &isCompiled);
	if (isCompiled == GL_FALSE)
	{
		GLint maxLength = 0;
		glGetShaderiv(vsh1, GL_INFO_LOG_LENGTH, &maxLength);

		std::vector<GLchar> errorLog(maxLength);
		glGetShaderInfoLog(vsh1, maxLength, &maxLength, &errorLog[0]);

		printf("%s", errorLog.data());

		glDeleteShader(vsh1);
		return 0;
	}

	GLuint fsh1 = glCreateShader(GL_FRAGMENT_SHADER);
	const GLchar* fsh1code =
		"#version 450\n"
		"in vec3 vcolor;"
		"out vec4 FragColor;"
		"void main(){"
		"FragColor = vec4(vcolor, 1);}";
	GLint fsh1len = strlen(fsh1code);
	glShaderSource(fsh1, 1, &fsh1code, &fsh1len);
	glCompileShader(fsh1);
	glAttachShader(shp1, fsh1);

	isCompiled = 0;
	glGetShaderiv(fsh1, GL_COMPILE_STATUS, &isCompiled);
	if (isCompiled == GL_FALSE)
	{
		GLint maxLength = 0;
		glGetShaderiv(fsh1, GL_INFO_LOG_LENGTH, &maxLength);

		std::vector<GLchar> errorLog(maxLength);
		glGetShaderInfoLog(fsh1, maxLength, &maxLength, &errorLog[0]);

		printf("%s", errorLog.data());

		glDeleteShader(fsh1);
		return 0;
	}

	glValidateProgram(shp1);
	glLinkProgram(shp1);
	glUseProgram(shp1);

	posloc1 = 10;
	colorloc1 = 12;
	glBindBuffer(GL_ARRAY_BUFFER, vbo1);
	glVertexAttribPointer(posloc1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), 0);
	glVertexAttribPointer(colorloc1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
	matloc1 = 11;
	glUniformMatrix4fv(matloc1, 1, GL_FALSE, glm::value_ptr<float>(transform));

	// Main loop
	while (!glfwWindowShouldClose(window))
	{
		// Render and handle other events
		mydisplay(window);

		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	glfwDestroyWindow(window);

	glfwTerminate();
	return 0;
}