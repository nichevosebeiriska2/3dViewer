#ifndef NODE_STL_H
#define NODE_STL_H

#include "node_common.h"

struct ObjectNodeSTL : public ObjectNode
{
	struct MeshStl
	{
		QOpenGLBuffer m_vbo{QOpenGLBuffer::VertexBuffer};
		std::unique_ptr<QOpenGLVertexArrayObject> m_ptr_vao;

		std::vector<Vertex> vertices;
		std::vector<unsigned int> indices;
		std::string name;

		QVector3D m_max_point;
		QVector3D m_min_point;
		QVector3D m_sizes;
		QVector3D m_middle_point;
	};

	QOpenGLBuffer m_vbo;
	QOpenGLVertexArrayObject m_vao;

	ObjectNodeSTL *m_pParent{nullptr};
	std::vector<ObjectNodeSTL *> m_vecChildren;
	std::vector<MeshStl> m_vecMeshes;


	ObjectNodeSTL(aiNode *pNode, const aiScene *scene, ObjectNodeSTL *pParent = nullptr);

	void SetParent(ObjectNodeSTL *pParent);
	void SetAttributes(QOpenGLShaderProgram *program);

protected:
	void ProcessNode(aiNode *node, const aiScene *scene);
	MeshStl ProcessMesh(aiMesh *aiMesh, const aiScene *scene);
	void CalculateMiddlePointByMeshes();
	void CalculateSizesByMeshes();
};

#endif