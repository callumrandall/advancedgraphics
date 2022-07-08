#pragma once
#include "../Blank Project/ShadedSceneNode.h"

class Terrain :
	public ShadedSceneNode
{

public:
	Terrain(Shader* shader, GLuint tex, Mesh* mesh, GLuint sandBump, GLuint grassTex, GLuint grassBump, GLuint rockTex, GLuint rockBump);
	void Draw(const OGLRenderer& r);
protected:
	GLuint sandBump;
	GLuint grassTex;
	GLuint grassBump;
	GLuint rockTex;
	GLuint rockBump;
};
