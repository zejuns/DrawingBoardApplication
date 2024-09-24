#pragma once

#include "Core.h"

class Shader
{
	unsigned int m_Id;
public:

	Shader(const char* InVertexPath, const char* InFragmentPath, const char* InGeometryPath = nullptr);
	~Shader();

public:
	static bool ReadShaderFile(const char* InFilePath, std::string& OutCode);
	static unsigned int CreateShader(unsigned int InShaderType, const char* InShaderCode);
	static unsigned int CreateShaderProgram(unsigned int InVertexShader, unsigned int InFragmentShader, unsigned int InGeometryShader = 0);

	void Activity();
	unsigned int GetId() const { return m_Id; }
	void SetBool(const std::string& InName, bool InValue) const;
	void SetInt(const std::string& InName, int InValue) const;
	void SetFloat(const std::string& InName, float InValue) const;
	void SetVec2(const std::string& InName, const glm::vec2& InValue) const;
	void SetVec3(const std::string& InName, const glm::vec3& InValue) const;
	void SetVec4(const std::string& InName, const glm::vec4& InValue) const;
	void SetMat4(const std::string& InName, const glm::mat4& InValue) const;
};

