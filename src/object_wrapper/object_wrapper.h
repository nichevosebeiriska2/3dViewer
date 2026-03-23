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
	virtual void SetAttributes(QOpenGLShaderProgram *program) = 0;
	unsigned int GetNumberOfVertices();
	QVector3D GetSizes();
	QVector3D GetMiddlePoint();
	float GetMinPolySize();
	float GetMaxPolySize();

	void SetSizes(QVector3D sizes);
	void SetMiddlePoint(QVector3D middle_point);
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
	void DrawByShader(QOpenGLShaderProgram* program) override;
};

struct MyMesh
{
	QOpenGLBuffer m_vbo{QOpenGLBuffer::VertexBuffer};
	QOpenGLBuffer m_ebo{QOpenGLBuffer::IndexBuffer};
	std::unique_ptr<QOpenGLVertexArrayObject> m_ptr_vao;
	int m_number_of_indices = 0;
};

class ObjectWrapperOBJ : public ObjectWrapper
{
private:
	
	std::vector<MyMesh> m_vec_meshes;
	std::vector<Material> m_vec_materials;

	QOpenGLBuffer m_vbo{QOpenGLBuffer::VertexBuffer};
	QOpenGLBuffer m_ebo{QOpenGLBuffer::IndexBuffer};
	QOpenGLVertexArrayObject m_vao;

public:
	ObjectWrapperOBJ(QVector<Mesh> meshes, QObject *paren);
	ObjectWrapperOBJ(QVector<Mesh> meshes, std::vector<Material> &&vec_materials);

	void SetAttributes(QOpenGLShaderProgram* program) override;
	void DrawByShader(QOpenGLShaderProgram* program) override;

private:
	void SetMaterial(QOpenGLShaderProgram *program, int i);
};

#endif