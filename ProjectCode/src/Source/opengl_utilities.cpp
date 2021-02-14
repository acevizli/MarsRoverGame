#include "opengl_utilities.h"
/* OpenGL Utility Structs */

	Model::Model(std::string path)
	{
		loadModel(path);
	}
	std::vector<VAO> meshes;
	/*void Draw(GLuint& shaderProgram)
	{
		for (int i = 0; i < meshes.size(); i++)
		{
			glBindVertexArray(meshes[i].id);

		}
	}*/
	void Model::loadModel(std::string path)
	{
		Assimp::Importer import;
		const aiScene* scene = import.ReadFile(path, aiProcess_Triangulate | aiProcess_FlipUVs);

		if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
		{
			std::cout << "ERROR::ASSIMP::" << import.GetErrorString() << std::endl;
			return;
		}
		directory = path.substr(0, path.find_last_of('/'));

		processNode(scene->mRootNode, scene);
	}

	void Model::processNode(aiNode* node, const aiScene* scene)
	{
		// process all the node's meshes (if any)
		for (unsigned int i = 0; i < node->mNumMeshes; i++)
		{
			aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
			meshes.push_back(processMesh(mesh, scene));
		}
		// then do the same for each of its children
		for (unsigned int i = 0; i < node->mNumChildren; i++)
		{
			processNode(node->mChildren[i], scene);
		}
	}
	VAO Model::processMesh(aiMesh* mesh,const aiScene* scene)
	{
		std::vector<glm::vec3> positions;
		std::vector<glm::vec3> normals;
		std::vector<glm::vec2> uvs;
		std::vector<GLuint> indices;
		for (int i = 0; i < mesh->mNumVertices; i++)
		{
			glm::vec3 position(mesh->mVertices[i].x, mesh->mVertices[i].y, mesh->mVertices[i].z);
			positions.push_back(position);
			glm::vec3 normal(mesh->mNormals[i].x, mesh->mNormals[i].y, mesh->mNormals[i].z);
			normals.push_back(normal);
			if (mesh->mTextureCoords[0])
			{
				glm::vec2 uv(mesh->mTextureCoords[0][i].x, mesh->mTextureCoords[0][i].y);
				uvs.push_back(uv);
			}
			else
				uvs.push_back(glm::vec2(0.0f, 0.0f));
		}
		for (unsigned int i = 0; i < mesh->mNumFaces; i++)
		{
			aiFace face = mesh->mFaces[i];
			for (unsigned int j = 0; j < face.mNumIndices; j++)
				indices.push_back(face.mIndices[j]);
		}
		return VAO(positions, normals, uvs, indices);
	}

VAO::VAO(
	const std::vector<glm::vec3>& positions,
	const std::vector<glm::vec3>& normals,
	const std::vector<glm::vec2>& uvs,
	const std::vector<GLuint>& indices
)
{
	glGenVertexArrays(1, &id);
	glBindVertexArray(id);

	vertex_count = GLsizei(positions.size());

	glGenBuffers(1, &position_buffer);
	glBindBuffer(GL_ARRAY_BUFFER, position_buffer);
	glBufferData(GL_ARRAY_BUFFER, positions.size() * sizeof(glm::vec3), positions.data(), GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, static_cast<void *>(0));
	glEnableVertexAttribArray(0);


	glGenBuffers(1, &normals_buffer);
	glBindBuffer(GL_ARRAY_BUFFER, normals_buffer);
	glBufferData(GL_ARRAY_BUFFER, normals.size() * sizeof(glm::vec3), normals.data(), GL_STATIC_DRAW);

	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, static_cast<void *>(0));
	glEnableVertexAttribArray(1);


	glGenBuffers(1, &uv_buffer);
	glBindBuffer(GL_ARRAY_BUFFER, uv_buffer);
	glBufferData(GL_ARRAY_BUFFER, uvs.size() * sizeof(glm::vec2), uvs.data(), GL_STATIC_DRAW);

	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 0, static_cast<void *>(0));
	glEnableVertexAttribArray(2);


	element_array_count = GLsizei(indices.size());

	glGenBuffers(1, &element_array_buffer);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, element_array_buffer);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(GLuint), indices.data(), GL_STATIC_DRAW);
};

/* OpenGL Utility Functions */
GLuint CreateShaderFromSource(const GLenum& shader_type, const GLchar * source)
{
	GLuint shader = glCreateShader(shader_type);
	glShaderSource(shader, 1, &source, NULL);
	glCompileShader(shader);

	int success;
	char info_log[512];
	glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		std::cout << "Error: Shader Compilation failed" << std::endl;
		glGetShaderInfoLog(shader, 512, NULL, info_log);
		std::cout << info_log << std::endl;

		glDeleteShader(shader);
		return NULL;
	}

	return shader;
}

GLuint CreateProgramFromSources(const GLchar * vertex_shader_source, const GLchar * fragment_shader_source)
{
	GLuint program = glCreateProgram();

	GLuint vertex_shader = CreateShaderFromSource(GL_VERTEX_SHADER, vertex_shader_source);
	GLuint fragment_shader = CreateShaderFromSource(GL_FRAGMENT_SHADER, fragment_shader_source);

	glAttachShader(program, vertex_shader);
	glAttachShader(program, fragment_shader);
	glLinkProgram(program);

	int success;
	char info_log[512];
	glGetProgramiv(program, GL_LINK_STATUS, &success);
	if (!success)
	{
		std::cout << "Error: Program Linking failed" << std::endl;
		glGetProgramInfoLog(program, 512, NULL, info_log);
		std::cout << info_log << std::endl;

		glDeleteProgram(program);
		return NULL;
	}

	return program;
}