#ifndef NODE_OBJ_H
#define NODE_OBJ_H

#include "node_common.h"

class ObjectNodeOBJ : public ObjectNode
{
protected:
	struct MeshObj
	{
		QOpenGLBuffer m_vbo{QOpenGLBuffer::VertexBuffer};
		QOpenGLBuffer m_ebo{QOpenGLBuffer::IndexBuffer};
		std::unique_ptr<QOpenGLVertexArrayObject> m_ptr_vao;

		std::vector<Vertex> vertices;
		std::vector<unsigned int> indices;
		std::string name;

		QVector3D m_max_point;
		QVector3D m_min_point;
		QVector3D m_sizes;
		QVector3D m_middle_point;
	};

protected:
	QOpenGLBuffer m_vbo{QOpenGLBuffer::VertexBuffer};
	QOpenGLBuffer m_ebo{QOpenGLBuffer::IndexBuffer};
	QOpenGLVertexArrayObject m_vao;
	int m_iMaterialIndex{0};

	ObjectNodeOBJ *m_pParent{nullptr};
	std::vector<ObjectNodeOBJ *> m_vecChildren;
	std::vector<MeshObj> m_vecMeshes;

public:

	ObjectNodeOBJ(aiNode *pNode, const aiScene *scene, ObjectNodeOBJ *pParent = nullptr);
	void SetAttributes(QOpenGLShaderProgram *program) override;
};

#endif