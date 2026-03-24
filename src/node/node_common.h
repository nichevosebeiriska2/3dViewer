#ifndef NODE_COMMON_H
#define NODE_COMMON_H

#include <QMatrix4x4>
#include <QVector3D>
#include <QOpenGLBuffer>
#include <QOpenGLVertexArrayObject>
#include <QOpenGLTexture>
#include <QOpenGLShaderProgram>

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>


struct Vertex
{
	QVector3D position{0.0,0.0,0.0};
	QVector3D normal{0.0,0.0,0.0};
	QVector2D texCoord{0.0,0.0};
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

struct Mesh2
{
	std::vector<Vertex> vertices;
	std::vector<unsigned int> indices;
	std::string name;

	QVector3D m_sizes;
	QVector3D m_middle_point;

	int m_iMaterialIndex = 0;
};

//std::pair<QVector3D, QVector3D> CalcMinMaxPointsForMeshes(const QVector<Mesh> &vec_meshes);
//std::pair<QVector3D, QVector3D> CalcMinMaxPoints(const Mesh &mesh);

std::pair<QVector3D, QVector3D> CalcMinMaxPointsForMeshes(const std::vector<Mesh2> &vec_meshes);
std::pair<QVector3D, QVector3D> CalcMinMaxPoints(const Mesh2 &mesh);

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
	Material(Material &&other) = default;
};

class ObjectNode
{
protected:
	QMatrix4x4 m_matTransform;
	QOpenGLBuffer m_vbo;
	QOpenGLVertexArrayObject m_vao;

	QVector3D m_node_middle_point;
	QVector3D m_node_sizes;

public:

	QVector3D GetSizes();
	QVector3D GetMiddlePoint();

	virtual void SetAttributes(QOpenGLShaderProgram *program) = 0;

	QString m_strName{"node"};
};

QVector3D createMinVec3();
QVector3D createMaxVec3();
QVector3D max_vec(const QVector3D &left, const QVector3D &right);
QVector3D min_vec(const QVector3D &left, const QVector3D &right);

#endif