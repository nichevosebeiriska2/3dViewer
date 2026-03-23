#ifndef SHADER_PROGRAMM_INTERFACE_H
#define SHADER_PROGRAMM_INTERFACE_H

#include <QOpenGLShader>

#include <map>
#include <optional>

class ShaderProgramInterface
{
public:
	enum ObjectType
	{
		STL, STL_MESH, STL_COLORED, // STL
		OBJ, OBJ_WITH_TEXTURE, //OBJ
		SCENE_AXIS,
		glass
	};

	const static std::map<std::string, QOpenGLShader::ShaderTypeBit> map_shader_names;
	const static std::map<ObjectType, std::string> map_object_type_names;


	static std::optional<QOpenGLShaderProgram*> LoadByObjectType(ObjectType type, QObject *parent);

};

#endif SHADER_PROGRAMM_INTERFACE_H