#include <iostream>
#include <fstream>

#include "shader_program_interface.h"
#include "global.h"

std::string ReadFile(std::filesystem::path path_to_file)
{
	std::ifstream file(path_to_file);

	if(file.is_open())
		return std::string{std::istreambuf_iterator<char>{file}, std::istreambuf_iterator<char>{}};

	return "";
}

const std::map<std::string, QOpenGLShader::ShaderTypeBit> ShaderProgramInterface::map_shader_names =
{
	{"vertex",									QOpenGLShader::Vertex},
	{"geometry",								QOpenGLShader::Geometry},
	{"tesselation_control",			QOpenGLShader::TessellationControl},
	{"tesselation_evaluation",	QOpenGLShader::TessellationEvaluation},
	{"fragment",								QOpenGLShader::Fragment},
	{"compute",									QOpenGLShader::Compute},
};

const std::map<ShaderProgramInterface::ObjectType, std::string> ShaderProgramInterface::map_object_type_names =
{
	{STL,					"STL"},
	{STL_MESH,		"STL_MESH"},
	{STL_COLORED, "STL_COLORED" },
	{SCENE_AXIS , "Scene/Axis"},
	{glass,				"glass"}
};



std::optional<QOpenGLShaderProgram *> ShaderProgramInterface::LoadByObjectType(ObjectType type, QObject* parent)
{
	using namespace std::filesystem;

	if(!map_object_type_names.contains(type) || !parent)
		return std::nullopt;

	const path path_to_shaders = Global::GetShadersDirectory() / map_object_type_names.at(type);

	QOpenGLShaderProgram* program = new QOpenGLShaderProgram(parent);

	for(auto it : directory_iterator(path_to_shaders))
	{
		const std::string file_name = it.path().filename().replace_extension().generic_string();

		if(!map_shader_names.contains(file_name))
			continue;

		program->addShaderFromSourceCode(map_shader_names.at(file_name), ReadFile(it).c_str());
	}

	if(!program->link())
	{
		qWarning() << "Shader link failed:" << program->log();
		return std::nullopt;
	}

	return program;
}