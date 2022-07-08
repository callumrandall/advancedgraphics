#pragma once
#include "../Blank Project/ShadedSceneNode.h"
class Mesh;
class MeshAnimation;
class MeshMaterial;

class Tree :
	public ShadedSceneNode
{
public:
	Tree(Shader* shader, Mesh* mesh, MeshAnimation* anim, MeshMaterial* mat, Vector3 pos, float scale, float yRot, bool move);
protected:
	void Draw(const OGLRenderer& r);
	void Update(float dt);
	//Shader* shader;
	//Mesh* mesh;
	MeshAnimation* anim;
	MeshMaterial* mat;
	Vector3 pos;
	float scale;
	float yRot;
	bool move;
	vector<GLuint> matTextures;
	int currentFrame;
	float frameTime;
};