#include "mesh.h"

Mesh::Mesh(vector<Vertex> vertices, vector<unsigned int> indices, vector<Texture> textures, bool calcTBN) {
	this->vertices = vertices;
	this->indices = indices;
	this->textures = textures;

    if (calcTBN) {
        setupTBN();
    }
	setupMesh();
}

void Mesh::setupMesh() {
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &EBO);

	glBindVertexArray(VAO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), &vertices[0], GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), &indices[0], GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0);
	glEnableVertexAttribArray(0);

	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)(offsetof(Vertex, Normal)));
	glEnableVertexAttribArray(1);

	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)(offsetof(Vertex, TextCoord)));
	glEnableVertexAttribArray(2);

    glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)(offsetof(Vertex, Tangent)));
    glEnableVertexAttribArray(3);

    glVertexAttribPointer(4, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)(offsetof(Vertex, Bitangent)));
    glEnableVertexAttribArray(4);
	
	glBindVertexArray(0);
}

void Mesh::setupTBN() {
    for (int i = 0; i < indices.size(); i += 3) {
        Vertex v1 = vertices[indices[i]];
        Vertex v2 = vertices[indices[i + 1]];
        Vertex v3 = vertices[indices[i + 2]];

        glm::vec3 edge1 = v2.Position - v1.Position;
        glm::vec3 edge2 = v3.Position - v1.Position;
        glm::vec2 deltaUV1 = v2.TextCoord - v1.TextCoord;
        glm::vec2 deltaUV2 = v3.TextCoord - v1.TextCoord;

        float f = 1.0f / (deltaUV1.x * deltaUV2.y - deltaUV2.x * deltaUV1.y);
        v1.Tangent.x = f * (deltaUV2.y * edge1.x - deltaUV1.y * edge2.x);
        v1.Tangent.y = f * (deltaUV2.y * edge1.y - deltaUV1.y * edge2.y);
        v1.Tangent.z = f * (deltaUV2.y * edge1.z - deltaUV1.y * edge2.z);

        v1.Bitangent.x = f * (-deltaUV2.x * edge1.x + deltaUV1.x * edge2.x);
        v1.Bitangent.y = f * (-deltaUV2.x * edge1.y + deltaUV1.x * edge2.y);
        v1.Bitangent.z = f * (-deltaUV2.x * edge1.z + deltaUV1.x * edge2.z);

        vertices[indices[i]].Tangent = v1.Tangent;
        vertices[indices[i + 1]].Tangent = v1.Tangent;
        vertices[indices[i + 2]].Tangent = v1.Tangent;

        vertices[indices[i]].Bitangent = v1.Bitangent;
        vertices[indices[i + 1]].Bitangent = v1.Bitangent;
        vertices[indices[i + 2]].Bitangent = v1.Bitangent;
    }
}

void Mesh::Draw(Shader& shader) {
    unsigned int diffuseNr = 1;
    unsigned int specularNr = 1;
    unsigned int normalNr = 1;
    unsigned int displacementNr = 1;
    
    shader.setInt("useDisplacement", 0);
    shader.setInt("useNormal", 0);
	shader.setInt("useDiffuse", 0);
	shader.setInt("useSpecular", 0);

	shader.setVec3("objColor", glm::vec3(0.0f, 0.4f, 0.0f));

    for (unsigned int i = 0; i < textures.size(); i++)
    {
        glActiveTexture(GL_TEXTURE0 + i); 

        string number;
        string name = textures[i].type;
        if (name == "texture_diffuse") {
            number = std::to_string(diffuseNr++);
            shader.setInt("useDiffuse", 1);
        }
        else if (name == "texture_specular") {
            number = std::to_string(specularNr++);
			shader.setInt("useSpecular", 1);
        }
        else if (name == "texture_normal") {
            number = std::to_string(normalNr++);
			shader.setInt("useNormal", 1); // Enable normal mapping in shader
        }
        else if (name == "texture_displacement") {
            number = std::to_string(displacementNr++);
			shader.setInt("useDisplacement", 1); // Enable displacement mapping in shader
        }

        //glUniform1i(glGetUniformLocation(shader.ID, ("material." + name + number).c_str()), i);
		shader.setInt(("material." + name + number).c_str(), i);

        glBindTexture(GL_TEXTURE_2D, textures[i].id);
    }

    
    glBindVertexArray(VAO);
    glDrawElements(GL_TRIANGLES, static_cast<unsigned int>(indices.size()), GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);

    glActiveTexture(GL_TEXTURE0);
}