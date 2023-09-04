#pragma once
#include <glm.hpp>
#include "SceneObject.h"
#include "Mesh.h"
#include "VAOLoader.h"

/*
* Desc: Model acts as a container within the scene graph for a mesh
*/
class Model: public SceneObject
{
private:
	// Model does not own the mesh. Instead, delegate 
	Mesh m_mesh;

public:
	Model() :m_mesh(Mesh()) {};
	Model(Mesh& m) : SceneObject(glm::mat4(1.0f)), m_mesh(m) {};
	void Init(VAOLoader& loader);
	void Render(ShaderProgram& loader);

};

