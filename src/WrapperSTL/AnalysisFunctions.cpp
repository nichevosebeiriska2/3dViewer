#include "AnalysisFunctions.h"

#include <igl/is_edge_manifold.h>
#include <igl/bounding_box.h>

#include <iostream>

bool CheckIsManifold(ObjectSTL* object)
{
	return igl::is_edge_manifold(object->m_MatrixFaces);
}

std::shared_ptr<ObjectSTL> GetBoundingBox(ObjectSTL* object)
{
	Eigen::MatrixXd bbox_vertices;
	Eigen::MatrixXi bbox_faces;;
	
	igl::bounding_box(object->m_MatrixVertices, bbox_vertices, bbox_faces);

	//bot
	bbox_faces(0, 0) = 0;
	bbox_faces(0, 1) = 1;
	bbox_faces(0, 2) = 2;

	bbox_faces(1, 0) = 0;
	bbox_faces(1, 1) = 2;
	bbox_faces(1, 2) = 3;

	//top 
	bbox_faces(2, 0) = 4;
	bbox_faces(2, 1) = 5;
	bbox_faces(2, 2) = 6;

	bbox_faces(3, 0) = 4;
	bbox_faces(3, 1) = 6;
	bbox_faces(3, 2) = 7;

	//side 0
	bbox_faces(4, 0) = 0;
	bbox_faces(4, 1) = 1;
	bbox_faces(4, 2) = 4;

	bbox_faces(5, 0) = 1;
	bbox_faces(5, 1) = 3;
	bbox_faces(5, 2) = 4;

	//side 1
	bbox_faces(6, 0) = 1;
	bbox_faces(6, 1) = 5;
	bbox_faces(6, 2) = 2;

	bbox_faces(7, 0) = 2;
	bbox_faces(7, 1) = 5;
	bbox_faces(7, 2) = 6;

	//side 0
	bbox_faces(8, 0) = 2;
	bbox_faces(8, 1) = 6;
	bbox_faces(8, 2) = 7;

	bbox_faces(9, 0) = 2;
	bbox_faces(9, 1) = 7;
	bbox_faces(9, 2) = 3;

	//side 2
	bbox_faces(10, 0) = 0;
	bbox_faces(10, 1) = 7;
	bbox_faces(10, 2) = 3;

	//side 3
	bbox_faces(11, 0) = 0;
	bbox_faces(11, 1) = 4;
	bbox_faces(11, 2) = 7;
	//for (auto i = 0; i < bbox_vertices.rows(); i++)
	//{
	//	std::cout << bbox_vertices(i, 0) << " " << bbox_vertices(i, 1) << " " << bbox_vertices(i, 2) << " " << std::endl;
	//}
	//std::cout << "--------------------" << std::endl;

	//for (auto i = 0; i < bbox_faces.rows(); i++)
	//{
	//	std::cout << bbox_faces(i, 0) << " " << bbox_faces(i, 1) << " " << bbox_faces(i, 2) << " " << std::endl;
	//}

	return std::make_shared<ObjectSTL>(std::move(bbox_vertices), std::move(bbox_faces));
}

QVector3D GetMiddlePoint(ObjectSTL* object)
{
	return { 0,0,0 };
}