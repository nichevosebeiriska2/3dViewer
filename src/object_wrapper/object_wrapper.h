#ifndef OBJECT_WRAPPER_H
#define OBJECT_WRAPPER_H


#include <QOpenGLBuffer>
#include <QOpenGLVertexArrayObject>
#include <QOpenGLShaderProgram>

#include <memory>

#include "modelloader.h"




class ObjectWrapper
{
protected:
	unsigned int m_num_of_vertices{0};
	QVector3D m_vec_middle_point{0.1,0.1,0.1 };
	QVector3D m_vec_sizes{1,1,1};

	float m_min_poly_size = 0.0f;
	float m_max_poly_size = 0.0f;
	
	std::vector<QOpenGLBuffer> m_vec_vbos;
	std::vector<QOpenGLBuffer> m_vec_ebos;
	std::vector<std::unique_ptr<QOpenGLVertexArrayObject>> m_vec_vaos;

public:
	virtual void Bind() = 0;
	virtual void Release() = 0;
	virtual void SetAttributes(QOpenGLShaderProgram *program) = 0;
	unsigned int GetNumberOfVertices();
	QVector3D GetSizes();
	QVector3D GetMiddlePoint();
	float GetMinPolySize();
	float GetMaxPolySize();
	virtual void DrawByShader(QOpenGLShaderProgram* program) = 0;
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
	void DrawByShader(QOpenGLShaderProgram* program) override;
};


class ObjectWrapperOBJ : public ObjectWrapper
{
private:
	struct MyMesh
	{
		QOpenGLBuffer m_vbo{ QOpenGLBuffer::VertexBuffer };
		QOpenGLBuffer m_ebo{ QOpenGLBuffer::IndexBuffer };
		std::unique_ptr<QOpenGLVertexArrayObject> m_vao;
		int m_numbder_of_indices = 0;
	};
	std::vector<MyMesh> m_vec_heshes;

public:
	ObjectWrapperOBJ(QVector<Mesh> meshes, QObject* paren);

	void SetAttributes(QOpenGLShaderProgram* program) override;
	void Bind() override;
	void Release() override;
	void DrawByShader(QOpenGLShaderProgram* program) override;
};


#endif