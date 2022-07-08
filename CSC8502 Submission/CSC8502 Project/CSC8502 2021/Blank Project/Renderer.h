#pragma once
#include "../nclgl/OGLRenderer.h"
#include "../nclgl/SceneNode.h"
#include "../nclgl/Frustum.h"
class Camera;
class Shader;
class HeightMap;
class MeshMaterial;
class MeshAnimation;
class SceneNode;
class Renderer : public OGLRenderer {
public:
	Renderer(Window& parent);
	~Renderer(void);
	void RenderScene() override;
	void UpdateScene(float dt) override;

protected:
	void DrawHeightmap();
	void DrawWater();
	void DrawSkybox();
	//void DrawSoldier();

	Shader* lightShader;
	Shader* reflectShader;
	Shader* skyboxShader;

	HeightMap* heightMap;
	Mesh* quad;
	Mesh* palm;

	Light* light;
	Camera* camera;

	void BuildNodeLists(SceneNode* from);
	void SortNodeLists();
	void ClearNodeLists();
	void DrawNodes();
	void DrawNode(SceneNode* n);
	SceneNode* root;
	Frustum frameFrustum;
	vector<SceneNode*> transparentNodeList;
	vector<SceneNode*> nodeList;


	GLuint cubeMap;
	GLuint waterTex;
	GLuint sandTex;
	GLuint sandBump;
	GLuint grassTex;
	GLuint grassBump;
	GLuint rockTex;
	GLuint rockBump;
	float waterRotate;
	float waterCycle;

	Mesh* meshtemp;
	Shader* meshShader;
	Shader* animMeshShader;
	MeshAnimation* anim;
	MeshMaterial* material;
	vector<GLuint> matTextures;

	Mesh* velmesh;
	MeshAnimation* velanim;
	MeshMaterial* velmaterial;

	Mesh* skelmesh;
	MeshAnimation* skelanim;
	MeshMaterial* skelmaterial;

	Mesh* spimesh;
	MeshAnimation* spianim;
	MeshMaterial* spimaterial;

	int currentFrame;
	float frameTime;


	void PresentScene();
	void DrawPostProcess();
	void DrawScene();

	Shader* processShader;
	Shader* sceneShader;
	Mesh* postquad;
	GLuint bufferFBO;
	GLuint processFBO;
	GLuint bufferColourTex[2];
	GLuint bufferDepthTex;

	bool postswitch;
};
