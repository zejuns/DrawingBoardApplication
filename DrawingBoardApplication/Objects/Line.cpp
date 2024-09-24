#include "Line.h"
#include "../Shader.h"

Line::Line()
{
	static int count = 0;
	objName = "Line_" + std::to_string(count);
	count++;
	shader = new Shader("Shader/drawObj.vert", "Shader/drawobj.frag");
}

void Line::CreateRenderData()
{
	this->vertices.push_back(glm::vec3(0.0));
	this->vertices.push_back(glm::vec3(700.0));
	DrawObject::CreateRenderData();
}

void Line::OnRender(float InDeltaTime)
{
	if (!shader)return;
	shader->Activity();
	glm::mat4 model = glm::mat4(1);
	shader->SetMat4("model", model);
	glLineWidth(lineWidth);
	this->vertices[0] = startPos;
	this->vertices[1] = endPos;
	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec3) * vertices.size(), vertices.data(), GL_STATIC_DRAW);

	shader->SetVec4("color", color);
	glDrawArrays(GL_LINES, 0, 2);
}
