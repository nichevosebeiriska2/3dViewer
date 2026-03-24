#ifndef NODE_GLTF_H
#define NODE_GLTF_H

#include "node_common.h"

class ObjectNodeGLTF : public ObjectNode
{
	struct MeshGlfw
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

	QOpenGLBuffer m_vbo;
	QOpenGLVertexArrayObject m_vao;

	ObjectNodeGLTF *m_pParent{nullptr};
	std::vector<ObjectNodeGLTF *> m_vecChildren;
	std::vector<MeshGlfw> m_vecMeshes;

public:
	ObjectNodeGLTF(aiNode *pNode, const aiScene *scene, ObjectNodeGLTF *pParent = nullptr);

	void SetParent(ObjectNodeGLTF *pParent);
	void SetAttributes(QOpenGLShaderProgram *program);

protected:
	void ProcessNode(aiNode *node, const aiScene *scene);
	MeshGlfw ProcessMesh(aiMesh *aiMesh, const aiScene *scene);
	void CalculateMiddlePointByMeshes();
	void CalculateSizesByMeshes();
};

#endif