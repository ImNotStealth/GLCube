#pragma once

#include "Common.h"

class Shader
{
public:
	Shader(const std::string& _name, std::filesystem::path _vertexShaderPath, std::filesystem::path _fragmentShaderPath);
	~Shader();

	void Use();
	void SetBool(const std::string& _name, bool _value);
	void SetInt(const std::string& _name, int _value);
	void SetFloat(const std::string& _name, float _value);
	void SetMat4(const std::string& _name, const glm::mat4& _value);

	inline const std::string& GetName() { return m_Name; }
	inline const unsigned int GetShaderProgramID() { return m_ShaderProgramID; }

private:
	const std::string ReadFromFile(std::filesystem::path _shaderPath);
	void Compile(const std::string& _vertexShaderSource, const std::string& _fragmentShaderSource);

private:
	std::string m_Name;
	unsigned int m_ShaderProgramID = 0;
};