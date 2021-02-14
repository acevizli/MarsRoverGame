#pragma once

#include <iostream>
#include <vector>

#include "GLAD/glad.h"
#include "GLM/glm.hpp"
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
/* OpenGL Utility Structs */

struct VAO
{
	GLuint id;

	GLsizei vertex_count;
	GLuint position_buffer;
	GLuint normals_buffer;
	GLuint uv_buffer;

	GLsizei element_array_count;
	GLuint element_array_buffer;

	VAO(
		const std::vector<glm::vec3>& positions,
		const std::vector<glm::vec3>& normals,
		const std::vector<glm::vec2>& uvs,
		const std::vector<GLuint>& indices
	);
};
class Model
{
public:
	Model(std::string path);
	std::vector<VAO> meshes;
private:
	std::string directory;
	void loadModel(std::string path);
	void processNode(aiNode* node, const aiScene* scene);
	VAO processMesh(aiMesh* mesh, const aiScene* scene);
};


/* OpenGL Utility Functions */

GLuint CreateShaderFromSource(const GLenum& shader_type, const GLchar * source);

GLuint CreateProgramFromSources(const GLchar * vertex_shader_source, const GLchar * fragment_shader_source);

