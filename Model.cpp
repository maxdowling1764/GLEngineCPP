#include "Model.h"
void Model::LoadModel(std::string path)
{
	Assimp::Importer importer;
	const aiScene* scene = importer.ReadFile(path, aiProcess_Triangulate | aiProcess_FlipUVs);

	if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
	{
		std::cout << "ERROR::ASIMP::" << importer.GetErrorString() << std::endl;
		return;
	}
	directory = path.substr(0, path.find_last_of('/'));
	processNode(scene->mRootNode, scene);
}

void Model::processNode(aiNode* node, const aiScene* scene)
{
	for (unsigned int i = 0; i < node->mNumMeshes; i++)
	{
		aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
		m_meshes.push_back(processMesh(mesh, scene));
	}

	for (unsigned int i = 0; i < node->mNumChildren; i++)
	{
		processNode(node->mChildren[i], scene);
	}
}


Mesh Model::processMesh(aiMesh* mesh, const aiScene* scene)
{
	Mesh m = Mesh();
	bool useTexture = mesh->mTextureCoords[0];
	for (unsigned int i = 0; i < mesh->mNumVertices; i++)
	{
		Vertex v;
		v.Position = vec3_ai2glm(mesh->mVertices[i]);
		v.Normal = vec3_ai2glm(mesh->mNormals[i]);
		if(useTexture)
			v.Texture = vec2_ai2glm(mesh->mTextureCoords[0][i]);
		m.m_vertices.push_back(v);
		//std::cout << v.Position.x << std::endl;
	}

	for (unsigned int i = 0; i < mesh->mNumFaces; i++)
	{
		aiFace face = mesh->mFaces[i];
		for (unsigned int j = 0; j < face.mNumIndices; j++)
		{
			m.m_indices.push_back(face.mIndices[j]);
		}
	}

	
	return m;
}

void Model::Init(VAOLoader& loader)
{
	for (int i = 0; i < m_meshes.size(); i++)
	{
		loader.LoadMesh(m_meshes[i]);
	}
}

void Model::Render(ShaderProgram& shader)
{
	shader.u_SetMat4("model", transform);
	for (int i = 0; i < m_meshes.size(); i++)
	{
		m_meshes[i].Render(shader);
	}
}
