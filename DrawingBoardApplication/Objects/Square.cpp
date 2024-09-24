#include "Square.h"
#include "../Shader.h"

Square::Square()
{
	static int count = 0;
	objName = "Square_" + std::to_string(count);
	count++;
	shader = new Shader("Shader/DrawObj.vert", "Shader/DrawObj.frag");
}

void Square::RefreshPos()
{
	float w = endPos.x - startPos.x;
	float h = endPos.y - startPos.y;
	//vertices.clear();

	//vertices.push_back(glm::vec3(0.0, 0.0, 0));
	//vertices.push_back(glm::vec3(0.0, h, 0));
	//vertices.push_back(glm::vec3(w,  0.0, 0));
	//vertices.push_back(glm::vec3(w, h, 0  ));

	vertices[0] = (startPos);
	vertices[1] = (glm::vec3(startPos.x, endPos.y, 0.0));
	vertices[2] = (glm::vec3(endPos.x, startPos.y, 0.0));
	vertices[3] = (endPos);
	
}

void Square::CreateRenderData()
{
	indices.push_back(0);
	indices.push_back(1);
	indices.push_back(2);

	indices.push_back(1);
	indices.push_back(3);
	indices.push_back(2);

	float w = 528;
	float h = 528;
	// 顶点
	vertices.push_back(glm::vec3(0.0, 0.0, 0));
	vertices.push_back(glm::vec3(0.0, h, 0));
	vertices.push_back(glm::vec3(w, 0.0, 0));
	vertices.push_back(glm::vec3(w, h, 0));

	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &EBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec3) * vertices.size(), vertices.data(), GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int) * indices.size(), indices.data(), GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(glm::vec3), (void*)0);
	glEnableVertexAttribArray(0);
}

void Square::OnRender(float InDeltaTime)
{
	if (!shader)return;
	glLineWidth(lineWidth);
	RefreshPos();

	glPolygonMode(GL_FRONT_AND_BACK, bFillRender ? GL_FILL : GL_LINE);

	shader->Activity();
	glm::mat4 model = glm::mat4(1);
	shader->SetMat4("model", model);

	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec3) * vertices.size(), vertices.data(), GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int) * indices.size(), indices.data(), GL_STATIC_DRAW);

	shader->SetVec4("color", color);

	glDrawElements(GL_TRIANGLES, static_cast<unsigned int>(indices.size()), GL_UNSIGNED_INT, 0);

	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
}