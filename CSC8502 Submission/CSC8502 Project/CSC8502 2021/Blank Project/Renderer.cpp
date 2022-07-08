#include "Renderer.h"
#include "Terrain.h"
#include "Tree.h"
#include "Water.h"
#include "../nclgl/Light.h"
#include "../nclgl/Heightmap.h"
#include "../nclgl/Shader.h"
#include "../nclgl/Camera.h"
#include "../nclgl/MeshMaterial.h"
#include "../nclgl/MeshAnimation.h"
#include "../nclgl/Window.h"
#include <algorithm>
const int POST_PASSES = 10;
Renderer::Renderer(Window& parent) : OGLRenderer(parent) {
	quad = Mesh::GenerateQuad();
	postquad = Mesh::GenerateQuad();

	heightMap = new HeightMap(TEXTUREDIR "test.png");

	waterTex = SOIL_load_OGL_texture(
		TEXTUREDIR "water.TGA", SOIL_LOAD_AUTO,
		SOIL_CREATE_NEW_ID, SOIL_FLAG_MIPMAPS);

	sandTex = SOIL_load_OGL_texture(
		TEXTUREDIR "sand.JPG", SOIL_LOAD_AUTO,
		SOIL_CREATE_NEW_ID, SOIL_FLAG_MIPMAPS);

	sandBump = SOIL_load_OGL_texture(
		TEXTUREDIR "sandBump.JPG", SOIL_LOAD_AUTO,
		SOIL_CREATE_NEW_ID, SOIL_FLAG_MIPMAPS);

	grassTex = SOIL_load_OGL_texture(
		TEXTUREDIR "grass.JPG", SOIL_LOAD_AUTO,
		SOIL_CREATE_NEW_ID, SOIL_FLAG_MIPMAPS);

	grassBump = SOIL_load_OGL_texture(
		TEXTUREDIR "grassBump.JPG", SOIL_LOAD_AUTO,
		SOIL_CREATE_NEW_ID, SOIL_FLAG_MIPMAPS);

	rockTex = SOIL_load_OGL_texture(
		TEXTUREDIR "grass2Tex.JPG", SOIL_LOAD_AUTO,
		SOIL_CREATE_NEW_ID, SOIL_FLAG_MIPMAPS);

	rockBump = SOIL_load_OGL_texture(
		TEXTUREDIR "grass2Bump.JPG", SOIL_LOAD_AUTO,
		SOIL_CREATE_NEW_ID, SOIL_FLAG_MIPMAPS);

	cubeMap = SOIL_load_OGL_cubemap(
		TEXTUREDIR "skyrender0001.bmp", TEXTUREDIR "skyrender0004.bmp",
		TEXTUREDIR "skyrender0003.bmp", TEXTUREDIR "skyrender0006.jpg",
		TEXTUREDIR "skyrender0005.bmp", TEXTUREDIR "skyrender0002.bmp",
		SOIL_LOAD_RGB, SOIL_CREATE_NEW_ID, 0);

	if (!sandTex || !sandBump || !grassTex || !grassBump || !rockTex || !rockBump || !cubeMap || !waterTex) {
		return;
	}
	SetTextureRepeating(sandTex, true);
	SetTextureRepeating(sandBump, true);
	SetTextureRepeating(grassTex, true);
	SetTextureRepeating(grassBump, true);
	SetTextureRepeating(rockTex, true);
	SetTextureRepeating(rockBump, true);
	SetTextureRepeating(waterTex, true);

	reflectShader = new Shader(
		"reflectVertex.glsl", "reflectFragment.glsl");
	skyboxShader = new Shader(
		"skyboxVertex.glsl", "skyboxFragment.glsl");
	lightShader = new Shader(
		"TerrainVertex.glsl", "TerrainFragment.glsl");

	animMeshShader = new Shader("SkinningVertex.glsl", "TexturedFragment.glsl");
	meshShader = new Shader("PerPixelVertex.glsl", "PerPixelFragment.glsl");

	sceneShader = new Shader("TexturedVertex.glsl", "TexturedFragment.glsl");
	processShader = new Shader("TexturedVertex.glsl", "processfrag.glsl");

	if (!reflectShader->LoadSuccess() ||
		!skyboxShader->LoadSuccess() ||
		!lightShader->LoadSuccess() ||
		!meshShader->LoadSuccess() ||
		!animMeshShader->LoadSuccess() ||
		!sceneShader->LoadSuccess() ||
		!processShader->LoadSuccess()){
		return;
	}




	meshtemp = Mesh::LoadFromMeshFile("tree-maple-low-poly-Anim.msh");
	anim = new MeshAnimation("tree-maple-low-poly-Anim.anm");
	material = new MeshMaterial("tree-maple-low-poly-Anim.mat");

	velmesh = Mesh::LoadFromMeshFile("Raptor_Animated_FBX_5K.msh");
	velanim = new MeshAnimation("Raptor_Animated_FBX_5K.anm");
	velmaterial = new MeshMaterial("Raptor_Animated_FBX_5K.mat");

	skelmesh = Mesh::LoadFromMeshFile("Skeleton@Skin.msh");
	skelanim = new MeshAnimation("Skeleton@Skin.anm");
	skelmaterial = new MeshMaterial("Skeleton@Skin.mat");

	spimesh = Mesh::LoadFromMeshFile("spider.msh");
	spianim = new MeshAnimation("spider.anm");
	spimaterial = new MeshMaterial("spider.mat");

	/*eagmesh = Mesh::LoadFromMeshFile("Eagle_Elite.msh");
	eaganim = new MeshAnimation("Eagle_Elite.anm");
	eagmaterial = new MeshMaterial("Eagle_Elite.mat");*/

	/*for (int i = 0; i < meshtemp->GetSubMeshCount(); ++i) {
		const MeshMaterialEntry* matEntry =
			material->GetMaterialForLayer(i);

		const string* filename = nullptr;
		matEntry->GetEntry("Diffuse", &filename);
		string path = TEXTUREDIR + *filename;
		GLuint texID = SOIL_load_OGL_texture(path.c_str(), SOIL_LOAD_AUTO,
			SOIL_CREATE_NEW_ID, SOIL_FLAG_MIPMAPS | SOIL_FLAG_INVERT_Y);
		matTextures.emplace_back(texID);
	}

	currentFrame = 0;
	frameTime = 0.0f;

	palm = Mesh::LoadFromMeshFile("palm.msh");*/

	Vector3 heightmapSize = heightMap->GetHeightmapSize();

	camera = new Camera(-20.0f, 0.0f, heightmapSize * Vector3(1.0f, 2.0f, 1.0f));
	camera->heightmapSize = heightmapSize;
	light = new Light(heightmapSize * Vector3(1.0f, 10.0f, 0.0f),
		Vector4(1, 1, 1, 1), heightmapSize.x*5);

	projMatrix = Matrix4::Perspective(1.0f, 25000.0f,
		(float)width / (float)height, 45.0f);

	root = new SceneNode();

	root->AddChild(new Terrain(lightShader, sandTex, heightMap, sandBump, grassTex, grassBump, rockTex, rockBump));
	root->AddChild(new Water(reflectShader, waterTex, quad, cubeMap, heightmapSize));
	root->AddChild(new Tree(meshShader, meshtemp, anim, material, Vector3(2800.0f, 350.0f, 2800.0f), 75.0f, 30.0f, false));
	root->AddChild(new Tree(meshShader, meshtemp, anim, material, Vector3(8000.0f, 600.0f, 8000.0f), 75.0f, 60.0f, false));
	root->AddChild(new Tree(meshShader, meshtemp, anim, material, Vector3(9000.0f, 500.0f, 5000.0f), 75.0f, 90.0f, false));
	root->AddChild(new Tree(meshShader, meshtemp, anim, material, Vector3(8000.0f, 600.0f, 4600.0f), 75.0f, 120.0f, false));
	root->AddChild(new Tree(meshShader, meshtemp, anim, material, Vector3(5500.0f, 600.0f, 12000.0f), 75.0f, 150.0f, false));
	root->AddChild(new Tree(meshShader, meshtemp, anim, material, Vector3(7000.0f, 500.0f, 11500.0f), 75.0f, 180.0f, false));
	root->AddChild(new Tree(meshShader, meshtemp, anim, material, Vector3(13000.0f, 450.0f, 3000.0f), 75.0f, 210.0f, false));
	root->AddChild(new Tree(meshShader, meshtemp, anim, material, Vector3(10500.0f, 450.0f, 6000.0f), 75.0f, 240.0f, false));
	root->AddChild(new Tree(meshShader, meshtemp, anim, material, Vector3(8000.0f, 650.0f, 6600.0f), 75.0f, 270.0f, false));
	root->AddChild(new Tree(meshShader, meshtemp, anim, material, Vector3(8500.0f, 650.0f, 9600.0f), 75.0f, 300.0f, false));
	root->AddChild(new Tree(meshShader, meshtemp, anim, material, Vector3(6800.0f, 750.0f, 6900.0f), 75.0f, 330.0f, false));
	root->AddChild(new Tree(meshShader, meshtemp, anim, material, Vector3(11000.0f, 550.0f, 7600.0f), 75.0f, 360.0f, false));
	root->AddChild(new Tree(meshShader, meshtemp, anim, material, Vector3(9800.0f, 550.0f, 10800.0f), 75.0f, 0.0f, false));

	root->AddChild(new Tree(animMeshShader, velmesh, velanim, velmaterial, Vector3(10000.0f, -1020.0f, 12700.0f), 300.0f, 0.0f, false));
	root->AddChild(new Tree(animMeshShader, skelmesh, skelanim, skelmaterial, Vector3(13890.0f, 380.0f, 12800.0f), 4000.0f, 180.0f, false));

	Tree* spider = new Tree(animMeshShader, spimesh, spianim, spimaterial, Vector3(0.0f, -150.0f, 0.0f), 220.0f, 0.0f, true);
	root->AddChild(spider);
	spider->AddChild(new Tree(animMeshShader, skelmesh, skelanim, skelmaterial, Vector3(160.0f, 875.0f, 1050.0f), 2500.0f, 0.0f, false));





	glGenTextures(1, &bufferDepthTex);
	glBindTexture(GL_TEXTURE_2D, bufferDepthTex);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH24_STENCIL8, width, height,
		0, GL_DEPTH_STENCIL, GL_UNSIGNED_INT_24_8, NULL);

	for (int i = 0; i < 2; ++i) {
		glGenTextures(1, &bufferColourTex[i]);
		glBindTexture(GL_TEXTURE_2D, bufferColourTex[i]);
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, width, height, 0,
			GL_RGBA, GL_UNSIGNED_BYTE, NULL);
	}
	glGenFramebuffers(1, &bufferFBO); // We ’ll render the scene into this
	glGenFramebuffers(1, &processFBO); // And do post processing in this

	glBindFramebuffer(GL_FRAMEBUFFER, bufferFBO);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT,
		GL_TEXTURE_2D, bufferDepthTex, 0);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_STENCIL_ATTACHMENT,
		GL_TEXTURE_2D, bufferDepthTex, 0);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0,
		GL_TEXTURE_2D, bufferColourTex[0], 0);
	// We can check FBO attachment success using this command !
	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) !=
		GL_FRAMEBUFFER_COMPLETE || !bufferDepthTex || !bufferColourTex[0]) {
		return;
	}
	glBindFramebuffer(GL_FRAMEBUFFER, 0);




	//glEnable(GL_CULL_FACE);
	glEnable(GL_BLEND);
	glEnable(GL_DEPTH_TEST);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glEnable(GL_TEXTURE_CUBE_MAP_SEAMLESS);
	waterRotate = 0.0f;
	waterCycle = 0.0f;
	init = true;
	postswitch = false;
}

Renderer::~Renderer(void) {
	delete root;
	delete camera;
	delete heightMap;
	delete quad;
	delete reflectShader;
	delete skyboxShader;
	delete lightShader;
	delete light;
	delete meshtemp;
	delete anim;
	delete material;
	delete meshShader;
	
	delete sceneShader;
	delete processShader;
	delete postquad;

	glDeleteTextures(2, bufferColourTex);
	glDeleteTextures(1, &bufferDepthTex);
	glDeleteFramebuffers(1, &bufferFBO);
	glDeleteFramebuffers(1, &processFBO);
}

void Renderer::UpdateScene(float dt) {
	camera->UpdateCamera(dt);
	viewMatrix = camera->BuildViewMatrix();
	frameFrustum.FromMatrix(projMatrix * viewMatrix);
	root->Update(dt);

	if (Window::GetKeyboard()->KeyDown(KEYBOARD_P)) {
		postswitch = true;
	}

	if (Window::GetKeyboard()->KeyDown(KEYBOARD_O)) {
		postswitch = false;
	}

	/*frameTime -= dt;
	while (frameTime < 0.0f) {
		currentFrame = (currentFrame + 1) % anim->GetFrameCount();
		frameTime += 1.0f / anim->GetFrameRate();
	}*/
}

void Renderer::BuildNodeLists(SceneNode* from) {
	if (frameFrustum.InsideFrustum(*from)||from->GetMesh()) {
		Vector3 dir = from->GetWorldTransform().GetPositionVector() -
			camera->GetPosition();
		from->SetCameraDistance(Vector3::Dot(dir, dir));

		if (from->GetColour().w < 1.0f) {
			transparentNodeList.push_back(from);
		}
		else {
			nodeList.push_back(from);
		}
	}
	for (vector <SceneNode*>::const_iterator i =
		from->GetChildIteratorStart();
		i != from->GetChildIteratorEnd(); ++i) {
		BuildNodeLists((*i));
	}
}

void Renderer::SortNodeLists() {
	std::sort(transparentNodeList.rbegin(), // note the r!
		transparentNodeList.rend(), // note the r!
		SceneNode::CompareByCameraDistance);
	std::sort(nodeList.begin(),
		nodeList.end(),
		SceneNode::CompareByCameraDistance);
}

void Renderer::DrawNodes() {
	for (const auto& i : nodeList) {
		DrawNode(i);
	}
	for (const auto& i : transparentNodeList) {
		DrawNode(i);
	}
}

void Renderer::DrawNode(SceneNode* n) {
	if (n->GetMesh()) {
		Shader* shader = n->getShader();
		if (shader) {
			BindShader(shader);
			glUniform3fv(glGetUniformLocation(shader->GetProgram(), "cameraPos"), 1, (float*)& camera->GetPosition());
			UpdateShaderMatrices();
			SetShaderLight(*light);
			n->Draw(*this);
		}
	}
}

void Renderer::ClearNodeLists() {
	transparentNodeList.clear();
	nodeList.clear();
}

void Renderer::RenderScene() {
	if (postswitch) {
		projMatrix = Matrix4::Perspective(1.0f, 25000.0f, (float)width / (float)height, 45.0f);
		glBindFramebuffer(GL_FRAMEBUFFER, bufferFBO);
		glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
		DrawSkybox();
		BuildNodeLists(root);
		SortNodeLists();
		DrawNodes();
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
		DrawPostProcess();
		PresentScene();
		ClearNodeLists();
	}
	else {
		projMatrix = Matrix4::Perspective(1.0f, 25000.0f, (float)width / (float)height, 45.0f);
		viewMatrix = camera->BuildViewMatrix();
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
		glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);
		DrawSkybox();
		BuildNodeLists(root);
		SortNodeLists();
		DrawNodes();
		ClearNodeLists();
	}
}

void Renderer::DrawPostProcess() {
	glBindFramebuffer(GL_FRAMEBUFFER, processFBO);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0,
		GL_TEXTURE_2D, bufferColourTex[1], 0);
	glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);

	BindShader(processShader);
	modelMatrix.ToIdentity();
	viewMatrix.ToIdentity();
	projMatrix.ToIdentity();
	UpdateShaderMatrices();

	glDisable(GL_DEPTH_TEST);

	glActiveTexture(GL_TEXTURE0);
	glUniform1i(glGetUniformLocation(
		processShader->GetProgram(), "sceneTex"), 0);
	for (int i = 0; i < POST_PASSES; ++i) {
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0,
			GL_TEXTURE_2D, bufferColourTex[1], 0);
		glUniform1i(glGetUniformLocation(processShader->GetProgram(),
			"isVertical"), 0);

		glBindTexture(GL_TEXTURE_2D, bufferColourTex[0]);
		postquad->Draw();
		// Now to swap the colour buffers , and do the second blur pass
		glUniform1i(glGetUniformLocation(processShader->GetProgram(),
			"isVertical"), 1);
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0,
			GL_TEXTURE_2D, bufferColourTex[0], 0);
		glBindTexture(GL_TEXTURE_2D, bufferColourTex[1]);
		postquad->Draw();
	}
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glEnable(GL_DEPTH_TEST);
}

void Renderer::PresentScene() {
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);
	BindShader(sceneShader);
	modelMatrix.ToIdentity();
	viewMatrix.ToIdentity();
	projMatrix.ToIdentity();
	UpdateShaderMatrices();
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, bufferColourTex[0]);
	glUniform1i(glGetUniformLocation(
		sceneShader->GetProgram(), "diffuseTex"), 0);
	postquad->Draw();
}

//void Renderer::DrawSoldier() {
//	//glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);
//
//	BindShader(soldierShader);
//	glUniform1i(glGetUniformLocation(soldierShader->GetProgram(),
//		"diffuseTex"), 0);
//
//	modelMatrix.ToIdentity();
//
//	modelMatrix = Matrix4::Translation(Vector3(2000.0f, 0.0f, 2000.0f)) * Matrix4::Scale(Vector3(1000.0f, 1000.0f, 1000.0f));
//
//	UpdateShaderMatrices();
//
//	vector<Matrix4> frameMatrices;
//
//	const Matrix4* invBindPose = soldier->GetInverseBindPose();
//	const Matrix4* frameData = anim->GetJointData(currentFrame);
//
//	for (unsigned int i = 0; i < soldier->GetJointCount(); ++i) {
//		frameMatrices.emplace_back(frameData[i] * invBindPose[i]);
//	}
//	int j = glGetUniformLocation(soldierShader->GetProgram(), "joints");
//	glUniformMatrix4fv(j, frameMatrices.size(), false,
//		(float*)frameMatrices.data());
//	for (int i = 0; i < soldier->GetSubMeshCount(); ++i) {
//		glActiveTexture(GL_TEXTURE0);
//		glBindTexture(GL_TEXTURE_2D, matTextures[i]);
//		soldier->DrawSubMesh(i);
//	}
//}

void Renderer::DrawSkybox() {
	glDepthMask(GL_FALSE);

	BindShader(skyboxShader);
	UpdateShaderMatrices();

	quad->Draw();

	glDepthMask(GL_TRUE);
}

void Renderer::DrawHeightmap() {
	BindShader(lightShader);
	SetShaderLight(*light);
	glUniform3fv(glGetUniformLocation(lightShader->GetProgram(),
		"cameraPos"), 1, (float*)& camera->GetPosition());

	glUniform1i(glGetUniformLocation(
		lightShader->GetProgram(), "diffuseTex"), 0);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, sandTex);

	glUniform1i(glGetUniformLocation(
		lightShader->GetProgram(), "bumpTex"), 1);
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, sandBump);

	glUniform1i(glGetUniformLocation(
		lightShader->GetProgram(), "diffuseTex2"), 2);
	glActiveTexture(GL_TEXTURE2);
	glBindTexture(GL_TEXTURE_2D, grassTex);

	glUniform1i(glGetUniformLocation(
		lightShader->GetProgram(), "bumpTex2"), 3);
	glActiveTexture(GL_TEXTURE3);
	glBindTexture(GL_TEXTURE_2D, grassBump);

	glUniform1i(glGetUniformLocation(
		lightShader->GetProgram(), "diffuseTex3"), 4);
	glActiveTexture(GL_TEXTURE4);
	glBindTexture(GL_TEXTURE_2D, rockTex);

	glUniform1i(glGetUniformLocation(
		lightShader->GetProgram(), "bumpTex3"), 5);
	glActiveTexture(GL_TEXTURE5);
	glBindTexture(GL_TEXTURE_2D, rockBump);

	modelMatrix.ToIdentity(); // New !
	textureMatrix.ToIdentity(); // New !

	UpdateShaderMatrices();

	heightMap->Draw();
}

void Renderer::DrawWater() {
	BindShader(reflectShader);

	glUniform3fv(glGetUniformLocation(reflectShader->GetProgram(),
		"cameraPos"), 1, (float*)& camera->GetPosition());

	glUniform1i(glGetUniformLocation(
		reflectShader->GetProgram(), "diffuseTex"), 0);
	glUniform1i(glGetUniformLocation(
		reflectShader->GetProgram(), "cubeTex"), 2);

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, waterTex);

	glActiveTexture(GL_TEXTURE2);
	glBindTexture(GL_TEXTURE_CUBE_MAP, cubeMap);

	Vector3 hSize = heightMap->GetHeightmapSize();
	hSize.y *= 0.37;
	//hSize.x *= 2.5;

	modelMatrix =
		Matrix4::Translation(hSize * 0.5f) *
		Matrix4::Scale(hSize * 2.0f) *
		Matrix4::Rotation(90, Vector3(100, 0, 0));

	textureMatrix =
		Matrix4::Translation(Vector3(waterCycle, 0.0f, waterCycle)) *
		Matrix4::Scale(Vector3(150, 150, 150)) *
		Matrix4::Rotation(waterRotate, Vector3(0, 0, 1));

	UpdateShaderMatrices();
	//SetShaderLight(* light ); // No lighting in this shader !
	quad->Draw();
}