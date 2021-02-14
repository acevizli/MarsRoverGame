#include <iostream>
#include <vector>
#define GLM_FORCE_LEFT_HANDED
#include "GLM/glm.hpp"
#include "GLM/gtc/constants.hpp"
#include "GLM/gtc/matrix_transform.hpp"
#include "GLM/gtx/rotate_vector.hpp"
#include "GLM/gtx/string_cast.hpp"
#include "GLM/gtc/type_ptr.hpp"
#include "GLAD/glad.h"
#include "GLFW/glfw3.h"
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"


#include "opengl_utilities.h"
#include "extras.h"

/* Keep the global state inside this struct */
static struct
{
	glm::dvec2 mouse_position;
	glm::ivec2 screen_dimensions = glm::ivec2(1080, 1080);
	unsigned char key;
} Globals;

/* GLFW Callback functions */
static void ErrorCallback(int error, const char* description)
{
	std::cerr << "Error: " << description << std::endl;
}

static void CursorPositionCallback(GLFWwindow* window, double x, double y)
{
	Globals.mouse_position.x = x;
	Globals.mouse_position.y = y;
}

void KeyboardCallback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	Globals.key = key;
}

static void WindowSizeCallback(GLFWwindow* window, int width, int height)
{
	Globals.screen_dimensions.x = width;
	Globals.screen_dimensions.y = height;

	glViewport(0, 0, width, height);
}


int main(void)
{
	/* Set GLFW error callback */
	glfwSetErrorCallback(ErrorCallback);

	/* Initialize the library */
	if (!glfwInit())
	{
		std::cout << "Failed to initialize GLFW" << std::endl;
		return -1;
	}

	/* Create a windowed mode window and its OpenGL context */
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GLFW_TRUE);
	//glfwWindowHint(GLFW_TRANSPARENT_FRAMEBUFFER, GLFW_TRUE);
	GLFWwindow* window = glfwCreateWindow(
		Globals.screen_dimensions.x, Globals.screen_dimensions.y,
		"Abdullah Cevizli", NULL, NULL
	);
	if (!window)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return -1;
	}
	/* Make the window's context current */
	glfwMakeContextCurrent(window);
	glfwSwapInterval(1);
	/* Load OpenGL extensions with GLAD */
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialize GLAD" << std::endl;
		glfwTerminate();
		return -1;
	}

	//glfwSwapInterval(2);






	/* Set GLFW Callbacks */
	glfwSetCursorPosCallback(window, CursorPositionCallback);
	glfwSetWindowSizeCallback(window, WindowSizeCallback);
	glfwSetKeyCallback(window, KeyboardCallback);

	/* Configure OpenGL */
	glClearColor(0.3, 0.1, 0.1, 0);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	/* Creating OpenGL objects */
	std::vector<glm::vec3> positions;
	std::vector<glm::vec3> normals;
	std::vector<glm::vec2> uvs;
	std::vector<GLuint> indices;
	GenerateParametricShapeFrom2D(positions, normals, uvs, indices, ParametricCircle, 64, 32);
	VAO torusVAO(positions, normals, uvs, indices);

	std::vector<glm::vec3> positions1;
	std::vector<glm::vec3> normals1;
	std::vector<glm::vec2> uvs1;
	std::vector<GLuint> indices1;
	GenerateParametricShapeFrom2D(positions1, normals1, uvs1, indices1, ParametricHalfCircle, 512 , 256);
	VAO sphereVAO(positions1, normals1, uvs1, indices1);

	/*VAO quadVAO(
		{
			{-1, -1, 0},
			{+1, -1, 0},
			{+1, +1, 0},
			{-1, +1, 0}
		},
		{
			{0, 0, -1},
			{0, 0, -1},
			{0, 0, -1},
			{0, 0, -1}
		},
		{
			{0, 0},
			{1, 0},
			{1, 1},
			{0, 1}
		},
		{
			0, 1, 2,
			0, 2, 3
		}
	);

	VAO cubeVAO(
		{
			{-0.5f, -0.5f, -0.5f},
			{0.5f, -0.5f, -0.5f},
			{0.5f, 0.5f, -0.5f},
			{0.5f, 0.5f, -0.5f},
			{-0.5f, 0.5f, -0.5f},
			{-0.5f, -0.5f, -0.5f},
			{-0.5f, -0.5f, 0.5f},
			{0.5f, -0.5f, 0.5f},
			{0.5f, 0.5f, 0.5f},
			{0.5f, 0.5f, 0.5f},
			{-0.5f, 0.5f, 0.5f},
			{-0.5f, -0.5f, 0.5f},
			{-0.5f, 0.5f, 0.5f},
			{-0.5f, 0.5f, -0.5f},
			{-0.5f, -0.5f, -0.5f},
			{-0.5f, -0.5f, -0.5f},
			{-0.5f, -0.5f, 0.5f},
			{-0.5f, 0.5f, 0.5f},
			{0.5f, 0.5f, 0.5f},
			{0.5f, 0.5f, -0.5f},
			{0.5f, -0.5f, -0.5f},
			{0.5f, -0.5f, -0.5f},
			{0.5f, -0.5f, 0.5f},
			{0.5f, 0.5f, 0.5f},
			{-0.5f, -0.5f, -0.5f},
			{0.5f, -0.5f, -0.5f},
			{0.5f, -0.5f, 0.5f},
			{0.5f, -0.5f, 0.5f},
			{-0.5f, -0.5f, 0.5f},
			{-0.5f, -0.5f, -0.5f},
			{-0.5f, 0.5f, -0.5f},
			{0.5f, 0.5f, -0.5f},
			{0.5f, 0.5f, 0.5f},
			{0.5f, 0.5f, 0.5f},
			{-0.5f, 0.5f, 0.5f},
			{-0.5f, 0.5f, -0.5f}
		},
		{
			{0.0f, 0.0f, -1.0f},
			{0.0f, 0.0f, -1.0f},
			{0.0f, 0.0f, -1.0f},
			{0.0f, 0.0f, -1.0f},
			{0.0f, 0.0f, -1.0f},
			{0.0f, 0.0f, -1.0f},
			{0.0f, 0.0f, 1.0f},
			{0.0f, 0.0f, 1.0f},
			{0.0f, 0.0f, 1.0f},
			{0.0f, 0.0f, 1.0f},
			{0.0f, 0.0f, 1.0f},
			{0.0f, 0.0f, 1.0f},
			{-1.0f, 0.0f, 0.0f},
			{-1.0f, 0.0f, 0.0f},
			{-1.0f, 0.0f, 0.0f},
			{-1.0f, 0.0f, 0.0f},
			{-1.0f, 0.0f, 0.0f},
			{-1.0f, 0.0f, 0.0f},
			{1.0f, 0.0f, 0.0f},
			{1.0f, 0.0f, 0.0f},
			{1.0f, 0.0f, 0.0f},
			{1.0f, 0.0f, 0.0f},
			{1.0f, 0.0f, 0.0f},
			{1.0f, 0.0f, 0.0f},
			{0.0f, -1.0f, 0.0f},
			{0.0f, -1.0f, 0.0f},
			{0.0f, -1.0f, 0.0f},
			{0.0f, -1.0f, 0.0f},
			{0.0f, -1.0f, 0.0f},
			{0.0f, -1.0f, 0.0f},
			{0.0f, 1.0f, 0.0f},
			{0.0f, 1.0f, 0.0f},
			{0.0f, 1.0f, 0.0f},
			{0.0f, 1.0f, 0.0f},
			{0.0f, 1.0f, 0.0f},
			{0.0f, 1.0f, 0.0f}
		},
		{},
		{
			0, 1, 2,
			3, 4, 5,
			6, 7, 8,
			9, 10, 11,
			12, 13, 14,
			15, 16, 17,
			18, 19, 20,
			21, 22, 23,
			24, 25, 26,
			27, 28, 29,
			30, 31, 32,
			33, 34, 35
		}
	);*/

	stbi_set_flip_vertically_on_load(true);

	auto filename = "Assets/mars_1k_color.jpg";
	int x, y, n;
	unsigned char* texture_data = stbi_load(filename, &x, &y, &n, 0);
	if (texture_data == NULL)
	{
		std::cout << "Texture " << filename << " failed to load." << std::endl;
		std::cout << "Error: " << stbi_failure_reason() << std::endl;
	}
	else
	{
		std::cout << "Texture " << filename << " is loaded, X:" << x << " Y:" << y << " N:" << n << std::endl;
	}

	GLuint texture;
	glGenTextures(1, &texture);

	if (x * n % 4 != 0)
		glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

	glBindTexture(GL_TEXTURE_2D, texture);
	glTexImage2D(
		GL_TEXTURE_2D,
		0,
		GL_RGBA,
		x, y, 0, n == 3 ? GL_RGB : GL_RGBA, GL_UNSIGNED_BYTE, texture_data
	);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR_MIPMAP_LINEAR);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_LINEAR_MIPMAP_LINEAR);

	glGenerateMipmap(GL_TEXTURE_2D);

	if (x * n % 4 != 0)
		glPixelStorei(GL_UNPACK_ALIGNMENT, 4);

	stbi_image_free(texture_data);

	

	GLuint program = CreateProgramFromSources(
		R"VERTEX(
#version 330 core

layout(location = 0) in vec3 a_position;
layout(location = 1) in vec3 a_normal;
layout(location = 2) in vec2 a_uv;

uniform mat4 u_model;
uniform mat4 u_projection_view;

out vec4 world_space_position;
out vec3 world_space_normal;
out vec2 vertex_uv;

void main()
{
	world_space_position = u_model * vec4(a_position, 1);
	world_space_normal = vec3(u_model * vec4(a_normal, 0));
	vertex_uv = a_uv;

	gl_Position = u_projection_view * world_space_position;
}
		)VERTEX",

		R"FRAGMENT(
#version 330 core

uniform vec2 u_mouse_position;
uniform sampler2D u_texture;
uniform vec3 u_surface_color;

in vec4 world_space_position;
in vec3 world_space_normal;
in vec2 vertex_uv;

out vec4 out_color;

void main()
{
	vec3 color = vec3(0);

	vec3 surface_position = world_space_position.xyz;
	vec3 surface_normal = normalize(world_space_normal);
	vec2 surface_uv = vertex_uv;
	vec3 surface_color;
	if(u_surface_color == vec3(0,0,0)) surface_color = texture(u_texture,surface_uv).rgb;
	else surface_color = u_surface_color;
	vec3 ambient_color = vec3(0.3);
	color += ambient_color * surface_color;

	vec3 light_direction = normalize(vec3(-1, -1, 1));
	vec3 to_light = -light_direction;

	vec3 light_color = vec3(1.3);

	float diffuse_intensity = max(0, dot(to_light, surface_normal));
	color += diffuse_intensity * light_color * surface_color;

	out_color = vec4(color, 1);
}
		)FRAGMENT");
	if (program == NULL)
	{
		glfwTerminate();
		return -1;
	}
	glUseProgram(program);

	auto texture_location = glGetUniformLocation(program, "u_texture");
	glUniform1i(texture_location, 0);

	glActiveTexture(GL_TEXTURE0); // activate the texture unit first before binding texture
	glBindTexture(GL_TEXTURE_2D, texture);

	auto mouse_location = glGetUniformLocation(program, "u_mouse_position");
	auto model_location = glGetUniformLocation(program, "u_model");
	auto projection_view_location = glGetUniformLocation(program, "u_projection_view");
	auto surface_color_location = glGetUniformLocation(program, "u_surface_color");

	/* Loop until the user closes the window */

	double cameraZ = -1;
	double cameraX = 0;
	double cameraY = 3.1;
	bool freeCamera = true;
	glm::mat4 reward(1);
	int rewardPos = 0;
	int rewardCount = 0;
	int MaxReward = 0;
	double rewardX = 0;
	glm::mat4 roverTransform(1);
	glm::mat4 transform2 = glm::scale(glm::vec3(5.));
	auto chasing_pos = glm::dvec2(0.35, -0.9);
	auto chasing_pos2 = glm::dvec2(-0.35, -0.9);
	int direction = 0;
	glm::mat4 roverWay(1);
	bool collision = true;
	auto path = "Assets/MSL_clean/MSL_clean.obj";
	Model model(path);
	while (!glfwWindowShouldClose(window))
	{
		/* Render here */
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		// Calculate mouse position
		auto mouse_position = Globals.mouse_position;
		mouse_position /= glm::dvec2(Globals.screen_dimensions);
		mouse_position.y = 1. - mouse_position.y;
		mouse_position = mouse_position * 2. - 1.;

		glUniform2fv(mouse_location, 1, glm::value_ptr(glm::vec2(mouse_position)));
			// Draw
			switch (Globals.key)
			{
			case 'W':
			{
				if (freeCamera)
				{
					cameraZ += 0.05f;
				}
				else if(collision)
				{
					roverTransform = glm::rotate(-0.01f, glm::vec3(1,0,0));
					chasing_pos = glm::mix(glm::dvec2(0.3, -0.8), chasing_pos, 0.9);
					chasing_pos2 = glm::mix(glm::dvec2(-0.3, -0.8), chasing_pos2, 0.9);
					reward = glm::translate(reward,glm::vec3(0,0.003,-0.02));
					rewardPos++;
				}
				else cameraZ += 0.05f;
				Globals.key = ' ';
				break;
			}
			case 'S':
			{
				if (freeCamera)
				{
					cameraZ -= 0.05f;
				}
				else if (collision)
				{
					roverTransform = glm::rotate(0.01f, glm::vec3(1, 0, 0));
				}
				else cameraZ -= 0.05f;
				Globals.key = ' ';
				break;
			}
			case 'A':
			{
				if (freeCamera)
				{
					cameraX -= 0.05f;
				}
				else if (collision)
				{
					if (direction != 5)
					{
						roverTransform = glm::rotate(0.01f, glm::vec3(0, 0, -1));
						reward = glm::translate(reward, glm::vec3(0.04, 0, 0));
						direction++;
					}
				}
				else cameraX += 0.044;
				Globals.key = ' ';
				break;
			}
			case 'D': 
			{
				if (freeCamera)
				{
					cameraX += 0.05f;
				}
				else if (collision)
				{
					if (direction != -5)
					{
						roverTransform = glm::rotate(0.01f, glm::vec3(0, 0, 1));
						reward = glm::translate(reward, glm::vec3(-0.04, 0, 0));
						direction--;
					}
				}
				else cameraX -= 0.044;
				Globals.key = ' ';
				break;
			}
			case 'C':
			{
				freeCamera = true;
				cameraX = 0;
				cameraZ = -1;
				cameraY = 3.1;
				Globals.key = ' ';
				break;
			}
			case 'R':
			{
				cameraX = 0;
				cameraZ = -1;
				freeCamera = false;
				Globals.key = ' ';
				collision = true;
				chasing_pos = glm::dvec2(0.35, -0.9);
				chasing_pos2 = glm::dvec2(-0.35, -0.9);
				rewardCount = 0;

				break;
			}
			case 'Q':
			{
				if (freeCamera)
				{
					cameraY += 0.05;
					Globals.key = ' ';
				}
				break;
			}
			case 'E':
			{
				if (freeCamera)
				{
					cameraY -= 0.05;
					Globals.key = ' ';
				}
				break;
			}
			default:
				roverTransform = glm::mat4(1);
				break;
			}
			if (freeCamera)
			{
				glBindTexture(GL_TEXTURE_2D, texture);
				glBindVertexArray(sphereVAO.id);
				auto view = glm::lookAt(
					glm::vec3(cameraX, cameraY, cameraZ),
					glm::vec3(mouse_position.x * 4 + cameraX, mouse_position.y * 4+cameraY-2., -cameraZ + 1),
					glm::vec3(0, 1, 0)
				);

				auto projection = glm::perspective(glm::radians(45.f), 1.f, 0.1f, 10.f);

				glUniformMatrix4fv(projection_view_location, 1, GL_FALSE, glm::value_ptr(projection * view));
				glm::mat4 transform(1);
				transform = glm::scale(transform,glm::vec3(3.f));
				transform *= glm::rotate(10.f, glm::vec3(1, 0, 0));
				glUniformMatrix4fv(model_location, 1, GL_FALSE, glm::value_ptr(transform));
				glUniform3fv(surface_color_location, 1, glm::value_ptr(glm::vec3(0, 0, 0)));
				glDrawElements(GL_TRIANGLES, sphereVAO.element_array_count, GL_UNSIGNED_INT, NULL);


			}
			else
			{
				auto view = glm::lookAt(
					glm::vec3(cameraX, 5.5, cameraZ),
					glm::vec3(0, 4.9, 0),
					glm::vec3(0, 1, 0)
				);
				auto projection = glm::perspective(glm::radians(45.f), 1.f, 0.1f, 10.f);

				glBindVertexArray(sphereVAO.id);
				glUniformMatrix4fv(projection_view_location, 1, GL_FALSE, glm::value_ptr(projection * view));
				transform2 = roverTransform*transform2;
				glUniformMatrix4fv(model_location, 1, GL_FALSE, glm::value_ptr(transform2));
				glUniform3fv(surface_color_location, 1, glm::value_ptr(glm::vec3(0, 0, 0)));
				glDrawElements(GL_TRIANGLES, sphereVAO.element_array_count, GL_UNSIGNED_INT, NULL);
				glm::highp_mat4 transform;
				for (size_t i = 0; i < model.meshes.size(); i++)
				{
					glBindVertexArray(model.meshes[i].id);
					transform =  glm::translate(glm::vec3(0, 5.0, 0))* roverWay;
					auto scale = glm::scale(transform, glm::vec3(0.1, 0.1, 0.1));
					glUniformMatrix4fv(model_location, 1, GL_FALSE, glm::value_ptr(scale));
					glUniform3fv(surface_color_location, 1, glm::value_ptr(glm::vec3(i*1.0/model.meshes.size(), i * 1.0 / model.meshes.size(), i * 1.0 / model.meshes.size())));
					glDrawElements(GL_TRIANGLES, model.meshes[i].element_array_count, GL_UNSIGNED_INT, NULL);
				}
				if (chasing_pos.y > -0.33)
				{
					collision = false;
					if (MaxReward < rewardCount) MaxReward = rewardCount;
				}
				else chasing_pos = glm::mix(glm::dvec2(0, 0), chasing_pos, 0.99);
				for (int i = 0; i < model.meshes.size(); i++)
				{
					glBindVertexArray(model.meshes[i].id);
					auto Ftransform = transform * glm::translate(glm::vec3(chasing_pos.x, -0.018, chasing_pos.y));
					auto Fscale = glm::scale(Ftransform, glm::vec3(0.1, 0.1, 0.1));
					glUniformMatrix4fv(model_location, 1, GL_FALSE, glm::value_ptr(Fscale));
					glUniform3fv(surface_color_location, 1, glm::value_ptr(glm::vec3(i * 1.0 / model.meshes.size(), i * 1.0 / model.meshes.size(), i * 1.0 / model.meshes.size())));
					glDrawElements(GL_TRIANGLES, model.meshes[i].element_array_count, GL_UNSIGNED_INT, NULL);
				}
				if (collision) chasing_pos2 = glm::mix(glm::dvec2(0, 0), chasing_pos2, 0.99);
				for (int i = 0; i < model.meshes.size(); i++)
				{
					glBindVertexArray(model.meshes[i].id);
					auto Ftransform = transform * glm::translate(glm::vec3(chasing_pos2.x, -0.018, chasing_pos2.y));
					auto Fscale = glm::scale(Ftransform, glm::vec3(0.1, 0.1, 0.1));
					glUniformMatrix4fv(model_location, 1, GL_FALSE, glm::value_ptr(Fscale));
					glUniform3fv(surface_color_location, 1, glm::value_ptr(glm::vec3(i * 1.0 / model.meshes.size(), i * 1.0 / model.meshes.size(), i * 1.0 / model.meshes.size())));
					glDrawElements(GL_TRIANGLES, model.meshes[i].element_array_count, GL_UNSIGNED_INT, NULL);
				}
				if (rewardPos != 40)
				{
					glBindVertexArray(torusVAO.id);
					auto rewardTransform = glm::translate(glm::vec3(rewardX, 4.972, 0.8)) * reward;
					auto rewardScale = rewardTransform * glm::scale(glm::vec3(0.05, 0.05, 0.05));
					auto rewardRotate = glm::rotate(glm::radians(90.f), glm::vec3(1, 0, 0));
					glUniformMatrix4fv(model_location, 1, GL_FALSE, glm::value_ptr(rewardScale * rewardRotate));
					glUniform3fv(surface_color_location, 1, glm::value_ptr(glm::vec3(0.5, 0.5, 0)));
					glDrawElements(GL_TRIANGLES, torusVAO.element_array_count, GL_UNSIGNED_INT, NULL);
				}
				else
				{
					if ((rewardX == 0.2 && direction < -3) || (rewardX == -0.2 && direction > 3) || (rewardX == 0 && (direction<2 || direction > -2)))
						rewardCount++;
					rewardPos = 0;
					reward = glm::mat4(1);
					rewardX = rand() % 3 - 1;
					rewardX /= 5.;
					direction = 0;
				}

			}
		/* Swap front and back buffers */
		glfwSwapBuffers(window);

		/* Poll for and process events */
		glfwPollEvents();
	}
	if (MaxReward < rewardCount) MaxReward = rewardCount;
	std::cout << "Max point: " << MaxReward << std::endl;

	glfwTerminate();
	return 0;
}
