#include "Shader.h"




Shader::Shader(const char* InVertexPath, const char* InFragmentPath, const char* InGeometryPath /*= nullptr*/)
{
	// 打开文件
	unsigned int vs = 0, fs = 0, gs = 0;
	if (std::string vCode; ReadShaderFile(InVertexPath, vCode))vs = CreateShader(GL_VERTEX_SHADER, vCode.c_str());
	if (std::string fCode; ReadShaderFile(InFragmentPath, fCode))fs = CreateShader(GL_FRAGMENT_SHADER, fCode.c_str());
	if (InGeometryPath != nullptr)if (std::string gCode; ReadShaderFile(InGeometryPath, gCode))gs = CreateShader(GL_GEOMETRY_SHADER, gCode.c_str());
	
	// 创建着色器程序
	m_Id = CreateShaderProgram(vs, fs, gs);
}

Shader::~Shader()
{
	glDeleteProgram(m_Id);
}

bool Shader::ReadShaderFile(const char* InFilePath, std::string& OutCode)
{
	std::ifstream file;
	file.open(InFilePath);
	if (!file.is_open())
	{
		std::cout << "Failed to open file: " << InFilePath << std::endl;
		return false;
	}
	std::string line;
	while (std::getline(file, line))
	{
		OutCode += line + "\n";
	}
	file.close();
	return true;
}

unsigned int Shader::CreateShader(unsigned int InShaderType, const char* InShaderCode)
{
	unsigned int shaderId = glCreateShader(InShaderType);
	glShaderSource(shaderId, 1, &InShaderCode, nullptr);
	glCompileShader(shaderId);

	int success = 0;
	glGetShaderiv(shaderId, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		char infoLog[1024];
		glGetShaderInfoLog(shaderId, 1024, NULL, infoLog);
		std::cout << "compile shader failed:\n" << std::endl;
		std::cout << InShaderCode << std::endl;
		std::cout << infoLog << std::endl;
		std::cout << "----------------------" << std::endl;
	}

	return shaderId;
}

unsigned int Shader::CreateShaderProgram(unsigned int InVertexShader, unsigned int InFragmentShader, unsigned int InGeometryShader)
{
	unsigned int shaderProgram = glCreateProgram();
	glAttachShader(shaderProgram, InVertexShader);
	glAttachShader(shaderProgram, InFragmentShader);
	if (InGeometryShader != 0)glAttachShader(shaderProgram, InGeometryShader);

	glLinkProgram(shaderProgram);
	glDeleteShader(InVertexShader);
	glDeleteShader(InFragmentShader);
	if (InGeometryShader != 0)glDeleteShader(InGeometryShader);

	int success = 0;
	glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
	if (!success)
	{
		char infoLog[1024];
		glGetProgramInfoLog(shaderProgram, 1024, NULL, infoLog);
		std::cout << "link shader program failed:\n" << infoLog << std::endl;
	}


	return shaderProgram;
}

void Shader::Activity()
{
	glUseProgram(m_Id);
}

void Shader::SetBool(const std::string& InName, bool InValue) const
{
	glUniform1i(glGetUniformLocation(m_Id, InName.c_str()), InValue);
}

void Shader::SetInt(const std::string& InName, int InValue) const
{
	glUniform1i(glGetUniformLocation(m_Id, InName.c_str()), InValue);
}

void Shader::SetFloat(const std::string& InName, float InValue) const
{
	glUniform1f(glGetUniformLocation(m_Id, InName.c_str()), InValue);
}

void Shader::SetVec2(const std::string& InName, const glm::vec2& InValue) const
{
	glUniform2fv(glGetUniformLocation(m_Id, InName.c_str()), 1, &InValue[0]);
}

void Shader::SetVec3(const std::string& InName, const glm::vec3& InValue) const
{
	glUniform3fv(glGetUniformLocation(m_Id, InName.c_str()), 1, &InValue[0]);
}

void Shader::SetVec4(const std::string& InName, const glm::vec4& InValue) const
{
	glUniform4fv(glGetUniformLocation(m_Id, InName.c_str()), 1, &InValue[0]);
}

void Shader::SetMat4(const std::string& InName, const glm::mat4& InValue) const
{
	glUniformMatrix4fv(glGetUniformLocation(m_Id, InName.c_str()), 1, GL_FALSE, &InValue[0][0]);
}
