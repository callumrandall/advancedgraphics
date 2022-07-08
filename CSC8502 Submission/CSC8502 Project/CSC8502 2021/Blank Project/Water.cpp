#include "Water.h"
#include "../nclgl/Camera.h"

Water::Water(Shader* shader, GLuint tex, Mesh* mesh, GLuint cubemap, Vector3 size) : ShadedSceneNode(shader, mesh, tex) {
	this->cubemap = cubemap;
	this->size = size;
	size.y *= 0.37;

	transform =
		Matrix4::Translation(size * 0.5f) *
		Matrix4::Scale(size * 2.0f) *
		Matrix4::Rotation(90, Vector3(100, 0, 0));
}

void Water::Update(float dt) {
	waterRotate += dt * 0.05f; //2 degrees a second
	waterCycle += dt * 0.25f; // 10 units a second
}

void Water::Draw(const OGLRenderer& r) {
	LoadTexture();
	glUniform1i(glGetUniformLocation(
		shader->GetProgram(), "cubeTex"), 2);
	glActiveTexture(GL_TEXTURE2);
	glBindTexture(GL_TEXTURE_CUBE_MAP, cubemap);

	textureMatrix =
		Matrix4::Translation(Vector3(waterCycle, 0.0f, waterCycle)) *
		Matrix4::Scale(Vector3(150, 150, 150)) *
		Matrix4::Rotation(waterRotate, Vector3(0, 0, 1));
	glUniformMatrix4fv(glGetUniformLocation(shader->GetProgram(), "textureMatrix"), 1, false, textureMatrix.values);
	UpdateShaderMatrices();
	mesh->Draw();
}
