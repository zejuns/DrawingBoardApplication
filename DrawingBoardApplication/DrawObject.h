#pragma once

#include "Core.h"

class Shader;

class DrawObject
{
public:
	std::string objName;
	// 是否填充绘制
	bool bFillRender = true;
	// 起始点位置
	glm::vec3 startPos = glm::vec3(0.0);
	// 终止点位置
	glm::vec3 endPos = glm::vec3(0.0);
	// 大小
	glm::vec3 size = glm::vec3(1.0);
	// 线条宽度
	float lineWidth = 5;
	// 绘制的颜色
	glm::vec4 color = glm::vec4(1.0);
	// 绘制用的shader
	Shader* shader;

	unsigned int VAO, VBO, EBO;
	
	std::vector<unsigned int>indices;

	std::vector<glm::vec3>vertices;// 绘制的顶点


	virtual void RefreshPos() {};

	virtual void CreateRenderData();

	glm::vec3 GetMiddlePos()const { return (startPos + endPos) / 2.0f; }

	virtual void OnRender(float InDeltaTime);
};
