#include "WrapperSTL.h"


ObjectSTL::ObjectSTL(Eigen::MatrixXd&& Vertices, Eigen::MatrixXi&& Faces)
	: m_MatrixVertices(std::move(Vertices))
	, m_MatrixFaces(std::move(Faces))
{
	CreateBuffers();
}

void ObjectSTL::SetAttributes(QOpenGLShaderProgram* program)
{
	m_vao.bind();
	m_ebo.bind();
	m_vbo.bind();

	program->setAttributeBuffer(0, GL_FLOAT, 0, 3, sizeof(float) * 3); // vertex

	program->enableAttributeArray(0);

	m_vao.release();
	m_vbo.release();
	m_ebo.release();
}


void ObjectSTL::CreateBuffers()
{
	m_vao.create();
	m_vao.bind();
	m_vbo.create();
	m_vbo.bind();
	m_ebo.create();
	m_ebo.bind();


	std::vector<float> vecVertices;
	std::vector<unsigned int> indices;
	vecVertices.reserve(m_MatrixVertices.rows() * 3);


	for (int i = 0; i < m_MatrixFaces.rows(); i++)
	{
		int i1, i2, i3;

		i1 = m_MatrixFaces(i, 0);
		i2 = m_MatrixFaces(i, 1);
		i3 = m_MatrixFaces(i, 2);

		indices.emplace_back(i1);
		indices.emplace_back(i2);
		indices.emplace_back(i3);

		vecVertices.push_back(m_MatrixVertices(i1, 0));
		vecVertices.push_back(m_MatrixVertices(i1, 1));
		vecVertices.push_back(m_MatrixVertices(i1, 2));

		vecVertices.push_back(m_MatrixVertices(i2, 0));
		vecVertices.push_back(m_MatrixVertices(i2, 1));
		vecVertices.push_back(m_MatrixVertices(i2, 2));

		vecVertices.push_back(m_MatrixVertices(i3, 0));
		vecVertices.push_back(m_MatrixVertices(i3, 1));
		vecVertices.push_back(m_MatrixVertices(i3, 2));
	}

	m_vbo.allocate(vecVertices.data(), vecVertices.size() * sizeof(float));
	m_ebo.allocate(indices.data(), indices.size() * sizeof(unsigned int));

	m_iNumOfIndices = indices.size();

	m_vao.release();
	m_vbo.release();
	m_ebo.release();
}

#include <igl/decimate_callback_types.h>
#include <igl/decimate.h>
#include <igl/collapse_least_cost_edge.h>
#include <igl/edge_flaps.h>


#include <igl/remove_duplicate_vertices.h>
#include <igl/remove_unreferenced.h>


std::shared_ptr<ObjectSTL> ObjectSTL::Decimate()
{

	//	Eigen::MatrixXd V = m_MatrixVertices;
	//	Eigen::MatrixXi F = m_MatrixFaces;
	//
	//
	//	Eigen::MatrixXd U;
	//	Eigen::MatrixXi G;
	//	Eigen::VectorXi I;
	//	Eigen::MatrixX J;
	//
	//	decltype(I)::RowsAtCompileTime;
	//	decltype(J)::RowsAtCompileTime;
	//	decltype(I)::ColsAtCompileTime;
	//	decltype(J)::ColsAtCompileTime;
	//	Eigen::MatrixXd V_clean;
	//	Eigen::MatrixXi F_clean;
	//
	//	igl::remove_unreferenced(V, F, V_clean, F_clean, I);
	//
	//	bool is_manifold = igl::is_edge_manifold(m_MatrixFaces);
	//
	//	bool status = igl::decimate(m_MatrixVertices, m_MatrixFaces, m_iNumOfIndices / 3 * 0.7, false, U, G, J, I);
	//
	//	auto pNewMesh = std::make_unique<ObjectSTL>();
	//	pNewMesh->m_MatrixVertices = U;
	//	pNewMesh->m_MatrixFaces = G;
	//	pNewMesh->CreateBuffers();
	//
	//	return pNewMesh;
	//}

	return nullptr;
}