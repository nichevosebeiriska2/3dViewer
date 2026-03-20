#ifndef OBJECT_WRAPPER_H
#define OBJECT_WRAPPER_H


#include <QOpenGLBuffer>
#include <QOpenGLVertexArrayObject>
#include <QOpenGLShaderProgram>


struct Vertex
{
	QVector3D position;
	QVector3D normal;
	QVector2D texCoord;

	Vertex() : position(0, 0, 0), normal(0, 0, 0), texCoord(0, 0) {}
};

// Структура меша
struct Mesh
{
	QVector<Vertex> vertices;
	QVector<unsigned int> indices;
	QString name;
};


class ObjectWrapper
{
protected:
	unsigned int m_num_of_vertices{0};
	QVector3D m_vec_middle_point{0,0,0};
	QVector3D m_vec_sizes{0,0,0};

	float m_min_poly_size = 0.0f;
	float m_max_poly_size = 0.0f;

public:
	virtual void Bind() = 0;
	virtual void Release() = 0;
	virtual void SetAttributes(QOpenGLShaderProgram *program) = 0;
	unsigned int GetNumberOfVertices();
	QVector3D GetSizes();
	QVector3D GetMiddlePoint();
	float GetMinPolySize();
	float GetMaxPolySize();
};


class ObjectWrapperSTL : public ObjectWrapper
{
private:
	QOpenGLBuffer m_vbo;
	QOpenGLVertexArrayObject m_vao;

public:
	ObjectWrapperSTL(QVector<Vertex> vertices);

	void SetAttributes(QOpenGLShaderProgram *program) override;
	void Bind() override;
	void Release() override;
};


#endif