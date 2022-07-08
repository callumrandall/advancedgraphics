#include "Tree.h"
#include "../nclgl/Camera.h"
#include "../nclgl/MeshAnimation.h"
#include "../nclgl/MeshMaterial.h"

Tree::Tree(Shader* shader, Mesh* imesh, MeshAnimation* anim, MeshMaterial* mat, Vector3 pos, float scale, float yRot, bool move) : ShadedSceneNode(shader, imesh){
	this->anim = anim;
	this->mat = mat;
	this->pos = pos;
	this->scale = scale;
	this->yRot = yRot;
	this->move = move;
	for (int i = 0; i < mesh->GetSubMeshCount(); ++i) {
		const MeshMaterialEntry* matEntry =	mat->GetMaterialForLayer(i);

		const string* filename = nullptr;
		matEntry->GetEntry("Diffuse", &filename);
		string path = TEXTUREDIR + *filename;
		GLuint texID = SOIL_load_OGL_texture(path.c_str(), SOIL_LOAD_AUTO,
			SOIL_CREATE_NEW_ID, SOIL_FLAG_MIPMAPS | SOIL_FLAG_INVERT_Y);
		matTextures.emplace_back(texID);
	}
	currentFrame = 0;
	frameTime = 0.0f;
}

void Tree::Update(float dt) {
	frameTime -= dt;
	while (frameTime < 0.0f) {
		currentFrame = (currentFrame + 1) % anim->GetFrameCount();
		frameTime += 1.0f / anim->GetFrameRate();
	}

	if (move) {
		pos.z += 5;
	}
}

void Tree::Draw(const OGLRenderer& r) {

	UpdateShaderMatrices();
	const Matrix4* invBindPose = mesh->GetInverseBindPose();
	const Matrix4* frameData = anim->GetJointData(currentFrame);
	transform.ToIdentity();
	transform = transform * Matrix4::Translation(parent->GetTransform().GetPositionVector() + pos);
	transform = transform * Matrix4::Scale(Vector3(scale, scale, scale));
	transform = transform * Matrix4::Rotation(yRot, Vector3(0, 1, 0));

	vector<Matrix4> frameMatrices;

	for (unsigned int i = 0; i < mesh->GetJointCount(); ++i) {
		frameMatrices.emplace_back(frameData[i] * invBindPose[i]);
	}
	int j = glGetUniformLocation(shader->GetProgram(), "joints");
	glUniformMatrix4fv(j, frameMatrices.size(), false,
		(float*)frameMatrices.data());
	for (int i = 0; i < mesh->GetSubMeshCount(); ++i) {
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, matTextures[i]);
		mesh->DrawSubMesh(i);
	}
}
