
#ifndef WRAPPER_STL_H
#define WRAPPER_STL_H

#include <Eigen/Core>
#include <QVector3D>
#include <QOpenGLBuffer>
#include <QOpenGLVertexArrayObject>
#include <QOpenGLShaderProgram>

struct ObjectSTL
{
	QVector3D m_max_point;
	QVector3D m_min_point;
	QVector3D m_sizes;
	QVector3D m_middle_point;

	std::shared_ptr<ObjectSTL> m_ptrBoundingBox = nullptr;

	Eigen::MatrixXd m_MatrixVertices;
	Eigen::MatrixXi m_MatrixFaces;

	unsigned int m_iNumOfIndices = 0;

	QOpenGLBuffer m_vbo{ QOpenGLBuffer::VertexBuffer };
	QOpenGLBuffer m_ebo{ QOpenGLBuffer::IndexBuffer };
	QOpenGLVertexArrayObject m_vao;

	void SetAttributes(QOpenGLShaderProgram* program);
	void CreateBuffers();

	std::shared_ptr<ObjectSTL> Decimate();

	ObjectSTL() = default;
	ObjectSTL(Eigen::MatrixXd&& Vertices, Eigen::MatrixXi&& Faces);
};

#endif WRAPPER_STL_H