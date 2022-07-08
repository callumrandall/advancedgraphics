#include "Terrain.h"
#include "../nclgl/Camera.h"

Terrain::Terrain(Shader* shader, GLuint tex, Mesh* mesh, GLuint sandBump, GLuint grassTex, GLuint grassBump, GLuint rockTex, GLuint rockBump) : ShadedSceneNode(shader, mesh, tex) {
	this->sandBump = sandBump;
	this->grassTex = grassTex;
	this->grassBump = grassBump;
	this->rockTex = rockTex;
	this->rockBump = rockBump;

	transform.ToIdentity();

}

void Terrain::Draw(const OGLRenderer& r) {
	LoadTexture();

	glUniform1i(glGetUniformLocation(
		shader->GetProgram(), "bumpTex"), 1);
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, sandBump);

	glUniform1i(glGetUniformLocation(
		shader->GetProgram(), "diffuseTex2"), 2);
	glActiveTexture(GL_TEXTURE2);
	glBindTexture(GL_TEXTURE_2D, grassTex);

	glUniform1i(glGetUniformLocation(
		shader->GetProgram(), "bumpTex2"), 3);
	glActiveTexture(GL_TEXTURE3);
	glBindTexture(GL_TEXTURE_2D, grassBump);

	glUniform1i(glGetUniformLocation(
		shader->GetProgram(), "diffuseTex3"), 4);
	glActiveTexture(GL_TEXTURE4);
	glBindTexture(GL_TEXTURE_2D, rockTex);

	glUniform1i(glGetUniformLocation(
		shader->GetProgram(), "bumpTex3"), 5);
	glActiveTexture(GL_TEXTURE5);
	glBindTexture(GL_TEXTURE_2D, rockBump);

	UpdateShaderMatrices();
	mesh->Draw();
}