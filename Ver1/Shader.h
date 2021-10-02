#define SHADER_H
#include <glad/glad.h>
#include "Light.h"
#include <string>
#include <fstream>
#include <sstream>
#include <iostream>

class Shader
{
public:
	unsigned int ID;

	// Конструктор генерирует шейдер "на лету"
	Shader(const char* vertexPath, const char* fragmentPath, const char* geometryPath = nullptr)
	{
		// 1. Получение исходного кода вершинного/фрагментного шейдера
		std::string vertexCode;
		std::string fragmentCode;
		std::string geometryCode;
		std::ifstream vShaderFile;
		std::ifstream fShaderFile;
		std::ifstream gShaderFile;

		// Убеждаемся, что объекты ifstream могут выбросить исключение:
		vShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
		fShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
		gShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
		try
		{
			// Открываем файлы
			vShaderFile.open(vertexPath);
			fShaderFile.open(fragmentPath);
			std::stringstream vShaderStream, fShaderStream;

			// Читаем содержимое файловых буферов
			vShaderStream << vShaderFile.rdbuf();
			fShaderStream << fShaderFile.rdbuf();

			// Закрываем файлы
			vShaderFile.close();
			fShaderFile.close();

			// Конвертируем в строковую переменную данные из потока
			vertexCode = vShaderStream.str();
			fragmentCode = fShaderStream.str();

			// Если дан путь к геометрическому шейдеру, то загружаем и его
			if (geometryPath != nullptr)
			{
				gShaderFile.open(geometryPath);
				std::stringstream gShaderStream;
				gShaderStream << gShaderFile.rdbuf();
				gShaderFile.close();
				geometryCode = gShaderStream.str();
			}
		}
		catch (std::ifstream::failure& e)
		{
			std::cout << "Shader File-path is dosnt exist!" << std::endl;
		}
		const char* vShaderCode = vertexCode.c_str();
		const char* fShaderCode = fragmentCode.c_str();

		// 2. Компилируем шейдеры
		unsigned int vertex, fragment;

		// Вершинный шейдер
		vertex = glCreateShader(GL_VERTEX_SHADER);
		glShaderSource(vertex, 1, &vShaderCode, NULL);
		glCompileShader(vertex);
		checkCompileErrors(vertex, "VERTEX");

		// Фрагментный шейдер
		fragment = glCreateShader(GL_FRAGMENT_SHADER);
		glShaderSource(fragment, 1, &fShaderCode, NULL);
		glCompileShader(fragment);
		checkCompileErrors(fragment, "FRAGMENT");

		// Если был дан геометрический шейдер, то компилируем его
		unsigned int geometry;
		if (geometryPath != nullptr)
		{
			const char* gShaderCode = geometryCode.c_str();
			geometry = glCreateShader(GL_GEOMETRY_SHADER);
			glShaderSource(geometry, 1, &gShaderCode, NULL);
			glCompileShader(geometry);
			checkCompileErrors(geometry, "GEOMETRY");
		}

		// Шейдерная программа
		ID = glCreateProgram();
		glAttachShader(ID, vertex);
		glAttachShader(ID, fragment);
		if (geometryPath != nullptr)
			glAttachShader(ID, geometry);
		glLinkProgram(ID);
		checkCompileErrors(ID, "PROGRAM");

		// После того, как мы связали шейдеры с нашей программой, удаляем их, т.к. они нам больше не нужны
		glDeleteShader(vertex);
		glDeleteShader(fragment);
		if (geometryPath != nullptr)
			glDeleteShader(geometry);

		use();

		setUniformInt("albedoMap", 0);
		setUniformInt("depthMap", 1);
		setUniformInt("metallicMap", 2);
		setUniformInt("roughnessMap", 3);
		setUniformInt("emissionMap", 4);
		setUniformInt("aoMap", 5);
		setUniformInt("normalMap", 6);
	}

	Shader() {}

	void ApplyLightToShader(int ind, Light light) {
		//std::string coordinatesInGLSL = "light[" + ind;coordinatesInGLSL += "].";
		setUniformVec3("light[0].position", light.position);
		setUniformFloat("light[0].intensity", light.intensity);
		setUniformFloat("light[0].cutOff", light.cutOff);
		setUniformFloat("light[0].outerCutOff", light.outerCutOff);

		setUniformVec3("light[0].diffuse", light.color);
		setUniformVec3("light[0].specular", light.specular);
		setUniformFloat("light[0].constant", light.constant);
		setUniformFloat("light[0].linear", light.linear);
		setUniformFloat("light[0].quadratic",light.quadratic);
	}

	void ApplyLightToShader1(int ind, Light light) {
		//std::string coordinatesInGLSL = "light[" + ind;coordinatesInGLSL += "].";
		setUniformVec3("light[1].position", light.position);
		setUniformFloat("light[1].intensity", light.intensity);
		setUniformFloat("light[1].cutOff", light.cutOff);
		setUniformFloat("light[1].outerCutOff", light.outerCutOff);

		setUniformVec3("light[1].diffuse", light.color);
		setUniformVec3("light[1].specular", light.specular);
		setUniformFloat("light[1].constant", light.constant);
		setUniformFloat("light[1].linear", light.linear);
		setUniformFloat("light[1].quadratic", light.quadratic);
	}



	void use() {
		glUseProgram(ID);
	}

	void setUniformBool(const std::string& name, bool value) const
	{
		glUniform1i(glGetUniformLocation(ID, name.c_str()), value);
	}

	void setUniformInt(const std::string& name, int value) const
	{
		glUniform1i(glGetUniformLocation(ID, name.c_str()), value);
	}

	void setUniformFloat(const std::string& name, float value) const
	{
		glUniform1f(glGetUniformLocation(ID, name.c_str()), value);
	}

	void setUniformVec2(const std::string& name, const glm::vec2& value) const
	{
		glUniform2fv(glGetUniformLocation(ID, name.c_str()), 1, &value[0]);
	}
	void setUniformVec2(const std::string& name, float x, float y) const
	{
		glUniform2f(glGetUniformLocation(ID, name.c_str()), x, y);
	}
	// ------------------------------------------------------------------------
	void setUniformVec3(const std::string& name, const glm::vec3& value) const
	{
		glUniform3fv(glGetUniformLocation(ID, name.c_str()), 1, &value[0]);
	}
	void setUniformVec3(const std::string& name, float x, float y, float z) const
	{
		glUniform3f(glGetUniformLocation(ID, name.c_str()), x, y, z);
	}
	// ------------------------------------------------------------------------
	void setUniformVec4(const std::string& name, const glm::vec4& value) const
	{
		glUniform4fv(glGetUniformLocation(ID, name.c_str()), 1, &value[0]);
	}
	void setUniformVec4(const std::string& name, float x, float y, float z, float w) const
	{
		glUniform4f(glGetUniformLocation(ID, name.c_str()), x, y, z, w);
	}
	// ------------------------------------------------------------------------
	void setUniformMat2(const std::string& name, const glm::mat2& mat) const
	{
		glUniformMatrix2fv(glGetUniformLocation(ID, name.c_str()), 1, GL_FALSE, &mat[0][0]);
	}
	// ------------------------------------------------------------------------
	void setUniformMat3(const std::string& name, const glm::mat3& mat) const
	{
		glUniformMatrix3fv(glGetUniformLocation(ID, name.c_str()), 1, GL_FALSE, &mat[0][0]);
	}
	// ------------------------------------------------------------------------
	void setUniformMat4(const std::string& name, const glm::mat4& mat) const
	{
		glUniformMatrix4fv(glGetUniformLocation(ID, name.c_str()), 1, GL_FALSE, &mat[0][0]);
	}

private:
	void checkCompileErrors(unsigned int shader, std::string type)
	{
		int success;
		char infoLog[1024];
		if (type != "PROGRAM")
		{
			glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
			if (!success)
			{
				glGetShaderInfoLog(shader, 1024, NULL, infoLog);
				std::cout << "Error in " << type << " file compilation:\n" << infoLog << "\n |-- --------------------------------------------------- --| " << "\n";
			}
		}
		else
		{
			glGetProgramiv(shader, GL_LINK_STATUS, &success);
			if (!success)
			{
				glGetProgramInfoLog(shader, 1024, NULL, infoLog);
				std::cout << "Error in " << type << " compilation:\n" << infoLog << "\n |-- --------------------------------------------------- --| " << "\n";
			}
		}
	}
};