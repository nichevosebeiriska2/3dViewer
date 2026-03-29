#include <iostream>
#include <fstream>

#include "shader_program_interface.h"
#include "global.h"
#include "WrapperSTL.h"

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
	{STL_NORMAL_FIELD, "STL_NORMAL_VECTORS"},
	{OBJ,								"OBJ"},
	{OBJ_WITH_TEXTURE,	"OBJ_WITH_TEXTURE"},
	{SCENE_AXIS , "Scene/Axis"},
	{glass,				"glass"}
};



std::optional<QOpenGLShaderProgram *> ShaderProgramInterface::LoadByObjectType(ObjectType type, QObject* parent)
{
	using namespace std::filesystem;

	if(!map_object_type_names.contains(type))
		return std::nullopt;

	const path path_to_shaders = Global::GetShadersDirectory() / map_object_type_names.at(type);

	QOpenGLShaderProgram* program = new QOpenGLShaderProgram();

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

void IShaderProgram::Bind()
{
	m_ptrProgram->bind();
}


void IShaderProgram::Release()
{
	m_ptrProgram->release();
}


void IShaderProgram::AcceptModelToSetAttributes(ObjectSTL* object)
{
	object->SetAttributes(m_ptrProgram);
}

QOpenGLShaderProgram* IShaderProgram::Get()
{
	return m_ptrProgram;
}


ShaderProgramTriangles::ShaderProgramTriangles()
{
	m_ptrProgram = *ShaderProgramInterface::LoadByObjectType(ShaderProgramInterface::ObjectType::STL, nullptr);
}


void ShaderProgramTriangles::Draw(ObjectSTL* object)
{
	object->m_vao.bind();
	glDrawElements(GL_TRIANGLES, object->m_iNumOfIndices, GL_UNSIGNED_INT, 0);
	object->m_vao.release();
}

ShaderProgramNormals::ShaderProgramNormals()
{
	m_ptrProgram = *ShaderProgramInterface::LoadByObjectType(ShaderProgramInterface::ObjectType::STL_NORMAL_FIELD, nullptr);
}


void ShaderProgramNormals::Draw(ObjectSTL* object)
{
	object->m_vao.bind();
	glDrawElements(GL_TRIANGLES, object->m_iNumOfIndices, GL_UNSIGNED_INT, 0);
	object->m_vao.release();
};


ShaderProgramWireFrame::ShaderProgramWireFrame()
{
	m_ptrProgram = *ShaderProgramInterface::LoadByObjectType(ShaderProgramInterface::ObjectType::STL_MESH, nullptr);
}


void ShaderProgramWireFrame::Draw(ObjectSTL* object)
{
	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	object->m_vao.bind();
	glDrawElements(GL_TRIANGLES, object->m_iNumOfIndices, GL_UNSIGNED_INT, 0);
	object->m_vao.release();
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
}