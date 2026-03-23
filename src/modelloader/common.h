#ifndef COMMON_H
#define COMMON_H

#include <QVector3D>
#include <QVector>
#include <QOpenGLTexture>

struct Vertex
{
	QVector3D position;
	QVector3D normal;
	QVector2D texCoord;

	Vertex() : position(0, 0, 0), normal(0, 0, 0), texCoord(0, 0)
	{
	}
};

// Структура меша
struct Mesh
{
	QVector<Vertex> vertices;
	QVector<unsigned int> indices;
	QString name;
	
	QVector3D m_sizes;
	QVector3D m_middle_point;

	int m_iMaterialIndex = 0;
};


std::pair<QVector3D, QVector3D> CalcMinMaxPointsForMeshes(const QVector<Mesh> &vec_meshes);
std::pair<QVector3D, QVector3D> CalcMinMaxPoints(const Mesh &mesh);

struct Material
{
	QVector3D ambientColor;
	QVector3D diffuseColor;
	QVector3D specularColor;

	float shininess;
	
	std::unique_ptr<QOpenGLTexture> m_texture_ambient;
	std::unique_ptr<QOpenGLTexture> m_texture_diffuse;
	std::unique_ptr<QOpenGLTexture> m_texture_specular;
	std::unique_ptr<QOpenGLTexture> m_texture_bump;

	Material() = default;

	Material(Material &&other)
		: ambientColor{other.ambientColor}
		, diffuseColor{other.diffuseColor}
		, specularColor{other.specularColor}
		, shininess{other.shininess}
		, m_texture_ambient{std::move(other.m_texture_ambient)}
		, m_texture_diffuse{std::move(other.m_texture_diffuse)}
		, m_texture_specular{std::move(other.m_texture_specular)}
		, m_texture_bump{std::move(other.m_texture_bump)}
	{

	}
};

#endif // !COMMON_H



