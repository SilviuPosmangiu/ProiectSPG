#pragma once
#include "mesh.h"

class Object {
public:
	Mesh* mesh;
	glm::vec3 position;
	glm::vec3 rotation;
	glm::vec3 scale;

	Object(
		Mesh &mesh,
		glm::vec3 position = glm::vec3(0.0),
		glm::vec3 rotation = glm::vec3(0.0),
		glm::vec3 scale = glm::vec3(1.0f)
	){
		this->mesh = &mesh;
		this->position = position;
		this->rotation = rotation;
		this->scale = scale;
	}

	virtual void Draw(Shader &shader) {
		glm::mat4 model = glm::mat4(1.0f);

		model = glm::translate(model, position);

		model = glm::rotate(model, glm::radians(rotation.x), glm::vec3(1, 0, 0));
		model = glm::rotate(model, glm::radians(rotation.y), glm::vec3(0, 1, 0));
		model = glm::rotate(model, glm::radians(rotation.z), glm::vec3(0, 0, 1));

		model = glm::scale(model, scale);

		shader.setMat4("model", model);

		mesh->Draw(shader);
	}
};