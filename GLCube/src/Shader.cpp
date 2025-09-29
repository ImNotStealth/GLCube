#include "Shader.h"

Shader::Shader(const std::string& _name, std::filesystem::path _vertexShaderPath, std::filesystem::path _fragmentShaderPath)
	: m_Name(_name)
{
	const std::string vertexShaderSource = ReadFromFile(_vertexShaderPath);
	const std::string fragmentShaderSource = ReadFromFile(_fragmentShaderPath);
	Compile(vertexShaderSource, fragmentShaderSource);
}

Shader::~Shader()
{
	if (m_ShaderProgramID != 0)
		glDeleteProgram(m_ShaderProgramID);
}

void Shader::Use()
{
	glUseProgram(m_ShaderProgramID);
}

void Shader::SetBool(const std::string& _name, bool _value)
{
	glUniform1i(glGetUniformLocation(m_ShaderProgramID, _name.c_str()), (int)_value);
}

void Shader::SetInt(const std::string& _name, int _value)
{
	glUniform1i(glGetUniformLocation(m_ShaderProgramID, _name.c_str()), _value);
}

void Shader::SetFloat(const std::string& _name, float _value)
{
	glUniform1f(glGetUniformLocation(m_ShaderProgramID, _name.c_str()), _value);
}

void Shader::SetMat4(const std::string& _name, const glm::mat4& _value)
{
	glUniformMatrix4fv(glGetUniformLocation(m_ShaderProgramID, _name.c_str()), 1, GL_FALSE, glm::value_ptr(_value));
}

const std::string Shader::ReadFromFile(std::filesystem::path _shaderPath)
{
	std::string shaderSource;
	std::ifstream shaderFile;
	shaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);

	try
	{
		shaderFile.open(_shaderPath.string());

		std::stringstream shaderStream;
		shaderStream << shaderFile.rdbuf();
		shaderFile.close();

		shaderSource = shaderStream.str();
	}
	catch (std::ifstream::failure e)
	{
		std::cout << m_Name << ": Failed to read Shader file." << std::endl;
	}

	return shaderSource;
}

void Shader::Compile(const std::string& _vertexShaderSource, const std::string& _fragmentShaderSource)
{
	int success;
	char infoLog[512];

	// Create Vertex Shader
	unsigned int vertShader = glCreateShader(GL_VERTEX_SHADER);
	const char* vertSource = _vertexShaderSource.c_str();
	glShaderSource(vertShader, 1, &vertSource, nullptr);
	glCompileShader(vertShader);

	// Check if Vertex Shader is compiled
	glGetShaderiv(vertShader, GL_COMPILE_STATUS, &success);

	if (!success)
	{
		glGetShaderInfoLog(vertShader, 512, nullptr, infoLog);
		std::cout << m_Name << ": Failed to compile Vertex Shader\n" << infoLog << std::endl;
		return;
	}

	// Create Fragment Shader
	unsigned int fragShader = glCreateShader(GL_FRAGMENT_SHADER);
	const char* fragSource = _fragmentShaderSource.c_str();
	glShaderSource(fragShader, 1, &fragSource, nullptr);
	glCompileShader(fragShader);

	// Check if Fragment Shader is compiled
	glGetShaderiv(fragShader, GL_COMPILE_STATUS, &success);

	if (!success)
	{
		glGetShaderInfoLog(fragShader, 512, nullptr, infoLog);
		std::cout << m_Name << ": Failed to compile Fragment Shader\n" << infoLog << std::endl;
		return;
	}

	// Create the Shader program
	m_ShaderProgramID = glCreateProgram();
	glAttachShader(m_ShaderProgramID, vertShader);
	glAttachShader(m_ShaderProgramID, fragShader);
	glLinkProgram(m_ShaderProgramID);

	glGetProgramiv(m_ShaderProgramID, GL_LINK_STATUS, &success);
	if (!success)
	{
		glGetProgramInfoLog(m_ShaderProgramID, 512, nullptr, infoLog);
		std::cout << m_Name << ": Failed to link Program\n" << infoLog << std::endl;
		return;
	}

	// Delete shaders after linking them
	glDeleteShader(vertShader);
	glDeleteShader(fragShader);
}
