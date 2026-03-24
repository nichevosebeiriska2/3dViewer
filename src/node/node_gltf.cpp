#include "node_gltf.h"

ObjectNodeGLTF::ObjectNodeGLTF(aiNode *pNode, const aiScene *scene, ObjectNodeGLTF *pParent)
{

}


void ObjectNodeGLTF::SetAttributes(QOpenGLShaderProgram *program)
{
	for(auto &mesh : m_vecMeshes)
	{
		mesh.m_ptr_vao->bind();
		mesh.m_vbo.bind();
		mesh.m_ebo.bind();

		program->setAttributeBuffer(0, GL_FLOAT, 0, 3, sizeof(Vertex));									// vertex
		program->setAttributeBuffer(1, GL_FLOAT, sizeof(QVector3D), 3, sizeof(Vertex));	//normal

		program->enableAttributeArray(0);
		program->enableAttributeArray(1);

		mesh.m_ptr_vao->release();
		mesh.m_vbo.release();
		mesh.m_ebo.release();
	}

	for(auto &pChild : m_vecChildren)
		pChild->SetAttributes(program);
}