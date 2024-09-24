#pragma once

#include "Core.h"

class Shader;


struct Vertex
{
	glm::vec3 position;// 顶点位置
	glm::vec3 normal;// 顶点法线
	glm::vec2 texcoord;// 顶点UV
};

class ObjModel
{
public:

	unsigned int VAO, VBO, EBO;
	Shader* shader;

	glm::vec3 objSize = glm::vec3(1.0);
	glm::vec3 objRotation = glm::vec3(0.0);

	std::string objName;
	std::vector<Vertex> vertices;
	std::vector<unsigned int> indices;


	ObjModel(const char* filename);

	ObjModel(std::string InModelName, const char* InFilename);


	void CreateVAOVBO();

	void OnRender(float InDeltaTime);

	void parse_obj(const char* filename);

	void calculationNormal(std::vector<Vertex>& InVertexArray, std::vector<unsigned int>InIndices);
	void calculationUVS(std::vector<Vertex>& InVertexArray, std::vector<unsigned int>InIndices);



	void get_indecies(std::string indexStr, unsigned int& posIdex, unsigned int& uvIndex, unsigned int& nIndex);
};
