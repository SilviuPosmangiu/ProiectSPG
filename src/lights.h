#pragma once
#include "object.h"

class PointLight : Object {
public:
	glm::vec3 ambient;
	glm::vec3 diffuse;
	glm::vec3 specular;

	float constant;
	float linear;
	float quadratic;

	int isOn;

	PointLight(
		Mesh& mesh,
		glm::vec3 position = glm::vec3(0.0),
		glm::vec3 rotation = glm::vec3(0.0),
		glm::vec3 scale = glm::vec3(1.0f)
	) : Object(mesh, position, rotation, scale) {
		
		ambient = glm::vec3(0.2f, 0.2f, 0.2f);
		diffuse = glm::vec3(0.3f, 0.3f, 0.3f);
		specular = glm::vec3(1.0f, 1.0f, 1.0f);

		constant = 1.0f;
		linear = 0.09f;
		quadratic = 0.032f;

		isOn = 1;
	}

	void Draw(Shader& shader) override {
		shader.setVec4("lightColor", glm::vec4(specular, 1.0f));
		Object::Draw(shader);
	}

	void loadLight(Shader& shaderProgram, int index) {
		shaderProgram.setVec3("pointLights[" + to_string(index) + "].position", position);
		shaderProgram.setVec3("pointLights[" + to_string(index) + "].ambient", ambient);
		shaderProgram.setVec3("pointLights[" + to_string(index) + "].diffuse", diffuse);
		shaderProgram.setVec3("pointLights[" + to_string(index) + "].specular", specular);
		shaderProgram.setFloat("pointLights[" + to_string(index) + "].constant", constant);
		shaderProgram.setFloat("pointLights[" + to_string(index) + "].linear", linear);
		shaderProgram.setFloat("pointLights[" + to_string(index) + "].quadratic", quadratic);
		shaderProgram.setInt("pointLights[" + to_string(index) + "].isOn", isOn);
	}
};

class DirLight {
public:
	glm::vec3 direction;

	glm::vec3 ambient;
	glm::vec3 diffuse;
	glm::vec3 specular;

	int isOn;

	DirLight() {
		direction = glm::vec3(0.0, -1.0, 0.0);

		ambient = glm::vec3(0.1f, 0.1f, 0.1f);
		diffuse = glm::vec3(0.4f, 0.4f, 0.4f);
		specular = glm::vec3(0.5f, 0.5f, 0.5f);

		isOn = 1;
	}

	void loadLight(Shader& shaderProgram) {
		shaderProgram.setVec3("dirLight.direction", direction);
		shaderProgram.setVec3("dirLight.ambient", ambient);
		shaderProgram.setVec3("dirLight.diffuse", diffuse);
		shaderProgram.setVec3("dirLight.specular", specular);
		shaderProgram.setInt("dirLight.isOn", isOn);
	}
};

class SpotLight {
public:
	glm::vec3 position;
	glm::vec3 direction;

	float cutOff;
	float outerCutOff;

	glm::vec3 ambient;
	glm::vec3 diffuse;
	glm::vec3 specular;

	float constant;
	float linear;
	float quadratic;

	int isOn;

	SpotLight(glm::vec3 position, glm::vec3 drection) {
		this->position = position;
		this->direction = direction;

		cutOff = glm::cos(glm::radians(12.5f));
		outerCutOff = glm::cos(glm::radians(15.5f));

		ambient = glm::vec3(0.2f, 0.2f, 0.2f);
		diffuse = glm::vec3(0.5f, 0.5f, 0.5f);
		specular = glm::vec3(1.0f, 1.0f, 1.0f);

		constant = 1.0f;
		linear = 0.09f;
		quadratic = 0.032f;

		isOn = 0;
	}

	void update(glm::vec3 pos, glm::vec3 dir) {
		position = pos;
		direction = dir;
	}

	void loadLight(Shader& shaderProgram) {
		shaderProgram.setVec3("spotLight.position", position);
		shaderProgram.setVec3("spotLight.direction", direction);
		shaderProgram.setFloat("spotLight.cutOff", cutOff);
		shaderProgram.setFloat("spotLight.outerCutOff", outerCutOff);
		shaderProgram.setVec3("spotLight.ambient", ambient);
		shaderProgram.setVec3("spotLight.diffuse", diffuse);
		shaderProgram.setVec3("spotLight.specular", specular);
		shaderProgram.setFloat("spotLight.constant", constant);
		shaderProgram.setFloat("spotLight.linear", linear);
		shaderProgram.setFloat("spotLight.quadratic", quadratic);
		shaderProgram.setInt("spotLight.isOn", isOn);
	}
};