#pragma once

#include "Mesh.h"
#include <assimp\Importer.hpp>
#include <assimp\scene.h>
#include <assimp\postprocess.h>
#include "stb_image.h"

class Model
{
public:
	Model(const char* path, bool gamma = false);
	~Model();

	glm::vec3 initialNode; 
	void Draw(Shader shader);
	std::vector<Vertex> allVertices;

private:
	std::vector<Mesh> meshes;
	std::string directory;
	std::vector<Texture> texturesLoaded;
	bool gammaCorrection;
		
	void LoadModel(std::string path);
	void ProcessNode(aiNode* node, const aiScene* scene);
	Mesh ProcessMesh(aiMesh* mesh, const aiScene* scene);
	vector<Texture> LoadMaterialTextures(aiMaterial *mat, aiTextureType type, string typeName);
	unsigned int TextureFromFile(const char* path, const string &directory, bool gamma);
};

