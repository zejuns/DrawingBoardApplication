#include "ObjModel.h"
#include "Shader.h"

ObjModel::ObjModel(const char* filename)
{
	parse_obj(filename);

	CreateVAOVBO();

	shader = new Shader("Shader/DrawObj.vert", "Shader/DrawObj.frag");
	shader->Activity();
	shader->SetInt("texture1", 0);
	shader->SetInt("bUseTexture", 1);
}

ObjModel::ObjModel(std::string InModelName, const char* InFilename)
{
	this->objName = InModelName;
	parse_obj(InFilename);
	CreateVAOVBO();
	shader = new Shader("Shader/DrawObj.vert", "Shader/DrawObj.frag");
	shader->Activity();
	shader->SetInt("texture1", 0);
	shader->SetInt("bUseTexture", 1);
}

void ObjModel::CreateVAOVBO()
{
	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &EBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex) * vertices.size(), vertices.data(), GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int) * indices.size(), indices.data(), GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, position));
	glEnableVertexAttribArray(0);

	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, normal));
	glEnableVertexAttribArray(1);

	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, texcoord));
	glEnableVertexAttribArray(2);
}

void ObjModel::OnRender(float InDeltaTime)
{
	if (!shader)return;

	shader->Activity();
	glm::mat4 model = glm::mat4(1);
	model = glm::translate(model, glm::vec3(0.0));
	model = glm::scale(model, objSize);
	model = glm::rotate(model, glm::radians(objRotation.x), glm::vec3(1.0, 0.0, 0.0));
	model = glm::rotate(model, glm::radians(objRotation.y), glm::vec3(0.0, 1.0, 0.0));
	model = glm::rotate(model, glm::radians(objRotation.z), glm::vec3(0.0, 0.0, 1.0));
	shader->SetMat4("model", model);
	glBindVertexArray(VAO);
	glDrawElements(GL_TRIANGLES, static_cast<unsigned int>(indices.size()), GL_UNSIGNED_INT, 0);

}

void ObjModel::parse_obj(const char* filename)
{
	std::ifstream objFile(filename);
	if (!objFile.is_open()) {
		std::cerr << "Failed to open " << filename << std::endl;
		return;
	}
	std::vector<glm::vec3> normals;
	std::vector<glm::vec2> uvs;

	std::string line;

	while (std::getline(objFile, line)) {
		std::istringstream ss(line);
		std::string lineHeader;
		ss >> lineHeader;
		if (lineHeader == "v") {
			glm::vec3 pos;
			ss >> pos.x >> pos.y >> pos.z;
			vertices.push_back({ pos, {0,0,0}, {0,0} });
		}
		else if (lineHeader == "vn") {
			glm::vec3 normal;
			ss >> normal.x >> normal.y >> normal.z;
			normals.push_back(normal);
		}
		else if (lineHeader == "vt") {
			glm::vec2 uv;
			ss >> uv.x >> uv.y;
			uvs.push_back(uv);
		}
		else if (lineHeader == "f") {
			std::string v1, v2, v3;
			unsigned int vIndex[3], uvIndex[3], nIndex[3];
			ss >> v1 >> v2 >> v3;

			get_indecies(v1, vIndex[0], uvIndex[0], nIndex[0]);
			get_indecies(v2, vIndex[1], uvIndex[1], nIndex[1]);
			get_indecies(v3, vIndex[2], uvIndex[2], nIndex[2]);

			indices.push_back(vIndex[0] - 1);
			indices.push_back(vIndex[1] - 1);
			indices.push_back(vIndex[2] - 1);

			if (normals.size() != 0 && nIndex[0] - 1 >= 0 && nIndex[1] - 1 >= 0 && nIndex[2] - 1 >= 0)
			{
				vertices[vIndex[0] - 1].normal = normals[nIndex[0] - 1];
				vertices[vIndex[1] - 1].normal = normals[nIndex[1] - 1];
				vertices[vIndex[2] - 1].normal = normals[nIndex[2] - 1];
			}

			if (uvs.size() > 0 && (uvIndex[0] - 1) >= 0 && uvIndex[1] - 1 >= 0 && uvIndex[2] - 1 >= 0)
			{
				vertices[vIndex[0] - 1].texcoord = uvs[uvIndex[0] - 1];
				vertices[vIndex[1] - 1].texcoord = uvs[uvIndex[1] - 1];
				vertices[vIndex[2] - 1].texcoord = uvs[uvIndex[2] - 1];
			}
		}
	}

	if (normals.size() == 0)
	{
		calculationNormal(vertices, indices);
	}
	if (uvs.size() == 0)
	{
		calculationUVS(vertices, indices);
	}

	objFile.close();
}

void ObjModel::calculationNormal(std::vector<Vertex>& InVertexArray, std::vector<unsigned int>InIndices)
{
	for (int i = 0; i < InIndices.size(); i += 3)
	{
		glm::vec3 v1 = InVertexArray[InIndices[i + 1]].position - InVertexArray[InIndices[i]].position;
		glm::vec3 v2 = InVertexArray[InIndices[i + 2]].position - InVertexArray[InIndices[i]].position;

		glm::vec3 normal;
		normal.x = v1.y * v2.z - v1.z * v2.y;
		normal.y = v1.z * v2.x - v1.x * v2.z;
		normal.z = v1.x * v2.y - v1.y * v2.x;

		normal = normal / sqrt(normal.x * normal.x + normal.y * normal.y + normal.z * normal.z);

		InVertexArray[InIndices[i]].normal = normal;
		InVertexArray[InIndices[i + 1]].normal = normal;
		InVertexArray[InIndices[i + 2]].normal = normal;
	}
}

void ObjModel::calculationUVS(std::vector<Vertex>& InVertexArray, std::vector<unsigned int>InIndices)
{
}

void ObjModel::get_indecies(std::string indexStr, unsigned int& posIdex, unsigned int& uvIndex, unsigned int& nIndex)
{
	posIdex = uvIndex = nIndex = 0;
	std::string index;
	std::istringstream ss(indexStr);
	std::getline(ss, index, '/');
	posIdex = std::stoi(index);
	std::getline(ss, index, '/');
	uvIndex = index.empty() ? -1 : std::stoi(index);
	std::getline(ss, index, '/');
	nIndex = index.empty() ? -1 : std::stoi(index);
}