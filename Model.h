#pragma once
#include <glm/glm.hpp>
#include "SceneObject.h"
#include "Mesh.h"
#include "VAOLoader.h"
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
/*
* Desc: Model acts as a container within the scene graph for a mesh
*/
class Model: public SceneObject
{
private:
	// Model does not own the mesh. Instead, delegate 
	std::vector<Mesh> m_meshes;
	std::string directory;
	
	void processNode(aiNode* node, const aiScene* scene);
	Mesh processMesh(aiMesh* mesh, const aiScene* scene);
	glm::vec3 vec3_ai2glm(aiVector3D a)
	{
		return glm::vec3(a.x, a.y, a.z);
	}

	glm::vec2 vec2_ai2glm(aiVector3D a)
	{
		return glm::vec2(a.x, a.y);
	}

public:
	glm::mat4 transform = glm::mat4(1.0f);
	Model() :m_meshes({}) {};
	Model(Mesh& m) : SceneObject(glm::mat4(1.0f)), m_meshes({m}) {};
	void Init(VAOLoader& loader);
	void Render(ShaderProgram& loader);
	void LoadModel(std::string path);
};

