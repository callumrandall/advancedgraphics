#pragma once
#include "../nclgl/SceneNode.h"

class Camera;

class ShadedSceneNode :
	public SceneNode
{

public:
	ShadedSceneNode(Shader*, Mesh*, GLuint = 0);
	//ShadedSceneNode(Shader*, Mesh*, GLuint = 0);

	virtual void Draw(const OGLRenderer& r);

	Shader* getShader() {
		return shader;
	}

protected:

	void UpdateShaderMatrices();
	void LoadTexture();

};