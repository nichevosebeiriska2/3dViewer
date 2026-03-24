#include "node_obj.h"

ObjectNodeOBJ::ObjectNodeOBJ(aiNode *pNode, const aiScene *scene, ObjectNodeOBJ *pParent)
{

}


void ObjectNodeOBJ::SetAttributes(QOpenGLShaderProgram *program)
{
	for(auto &mesh : m_vecMeshes)
	{
		mesh.m_ptr_vao->bind();
		mesh.m_vbo.bind();

		program->setAttributeBuffer(0, GL_FLOAT, 0, 3, sizeof(Vertex));											// vertex
		program->setAttributeBuffer(1, GL_FLOAT, sizeof(QVector3D), 3, sizeof(Vertex));			//normal
		program->setAttributeBuffer(2, GL_FLOAT, sizeof(QVector3D) * 2, 3, sizeof(Vertex));	//texture

		program->enableAttributeArray(0);
		program->enableAttributeArray(1);

		mesh.m_ptr_vao->release();
		mesh.m_vbo.release();
	}

	for(auto &pChild : m_vecChildren)
		pChild->SetAttributes(program);
}
