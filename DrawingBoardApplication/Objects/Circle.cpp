#include "Circle.h"
#include "../Shader.h"

#define PI 3.14159265358979323846
float axis[3] = { 0.0f, 0.0f, 1.0f };

#define DEG2RAD(deg) (deg * PI / 180)

Circle::Circle()
{
	static int count = 0;
	objName = "Circle_" + std::to_string(count);
	count++;
	shader = new Shader("Shader/drawObj.vert", "Shader/drawobj.frag");
}

void Circle::RefreshPos()
{
	float radius = glm::length(startPos - endPos);
	vertices.clear();
	indices.clear();

	for (int i = 0; i <= numSegments; ++i)
	{
		glm::vec3 pos(0.0);

		const float angle = 2.0 * glm::pi<float>() * float(i) / float(numSegments);

		pos.x = cosf(angle) * radius;
		pos.y = sinf(angle) * radius;

		vertices.push_back(pos);
	}
	vertices.push_back(glm::vec3(0.0));

	for (auto i = 0; i <= numSegments; i++)
	{
		indices.push_back(i);
		indices.push_back(i + 1);
		indices.push_back(numSegments + 1);
	}
}

void Circle::CreateRenderData()
{
	RefreshPos();

	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &EBO);

	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec3) * vertices.size(), vertices.data(), GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int) * indices.size(), indices.data(), GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(glm::vec3), (void*)0);
	glEnableVertexAttribArray(0);
}

void Circle::OnRender(float InDeltaTime)
{
	if (!shader)return;
	glLineWidth(lineWidth);

	glPolygonMode(GL_FRONT_AND_BACK, bFillRender ? GL_FILL : GL_LINE);

	shader->Activity();
	glm::mat4 model = glm::mat4(1);
	model = glm::translate(model, startPos);
	shader->SetMat4("model", model);

	RefreshPos();

	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec3) * vertices.size(), vertices.data(), GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int) * indices.size(), indices.data(), GL_STATIC_DRAW);

	shader->SetVec4("color", color);

	glDrawElements(GL_TRIANGLES, (indices.size()), GL_UNSIGNED_INT, 0);

	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
}