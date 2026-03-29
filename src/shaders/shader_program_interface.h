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
		STL, STL_MESH, STL_COLORED, STL_NORMAL_FIELD,// STL
		OBJ, OBJ_WITH_TEXTURE, //OBJ
		SCENE_AXIS,
		glass
	};

	const static std::map<std::string, QOpenGLShader::ShaderTypeBit> map_shader_names;
	const static std::map<ObjectType, std::string> map_object_type_names;


	static std::optional<QOpenGLShaderProgram*> LoadByObjectType(ObjectType type, QObject *parent);

};

class ObjectSTL;

class IShaderProgram
{
protected:
	QOpenGLShaderProgram* m_ptrProgram = nullptr;

public:
	virtual void Draw(ObjectSTL* object) = 0;
	void Bind();
	void Release();
	void AcceptModelToSetAttributes(ObjectSTL* object);
	QOpenGLShaderProgram* Get();
};

class ShaderProgramTriangles : public IShaderProgram
{
public:
	ShaderProgramTriangles();
	void Draw(ObjectSTL* object) override;
};


class ShaderProgramNormals : public IShaderProgram
{
public:
	ShaderProgramNormals();
	void Draw(ObjectSTL* object) override;
};


class ShaderProgramWireFrame : public IShaderProgram
{
public:
	ShaderProgramWireFrame();
	void Draw(ObjectSTL* object) override;
};

#endif SHADER_PROGRAMM_INTERFACE_H