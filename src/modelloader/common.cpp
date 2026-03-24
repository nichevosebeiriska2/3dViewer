#include "common.h"
#include <limits>

//QVector3D createMinVec3()
//{
//	return QVector3D{std::numeric_limits<float>::lowest(), std::numeric_limits<float>::lowest(), std::numeric_limits<float>::lowest()};
//}
//
//QVector3D createMaxVec3()
//{
//	return QVector3D{std::numeric_limits<float>::max(), std::numeric_limits<float>::max(), std::numeric_limits<float>::max()};
//}
//
//QVector3D max_vec(const QVector3D &left, const QVector3D &right)
//{
//	return QVector3D{std::max(left.x(), right.x()), std::max(left.y(), right.y()), std::max(left.z(), right.z())};
//}
//
//QVector3D min_vec(const QVector3D &left, const QVector3D &right)
//{
//	return QVector3D{std::min(left.x(), right.x()), std::min(left.y(), right.y()), std::min(left.z(), right.z())};
//}
//
//std::pair<QVector3D, QVector3D> CalcMinMaxPoints(const Mesh &mesh)
//{
//	QVector3D min{std::numeric_limits<float>::max(),std::numeric_limits<float>::max(),std::numeric_limits<float>::max()}
//					, max{std::numeric_limits<float>::lowest(),std::numeric_limits<float>::lowest(),std::numeric_limits<float>::lowest()};
//
//	for(auto &vertex : mesh.vertices)
//	{
//		min = min_vec(min, vertex.position);
//		max = max_vec(max, vertex.position);
//	}
//
//	return std::make_pair(min, max);
//}
//
//std::pair<QVector3D, QVector3D> CalcMinMaxPoints(const ObjectNodeSTL::MeshStl &mesh)
//{
//	QVector3D min{std::numeric_limits<float>::max(),std::numeric_limits<float>::max(),std::numeric_limits<float>::max()}
//	, max{std::numeric_limits<float>::lowest(),std::numeric_limits<float>::lowest(),std::numeric_limits<float>::lowest()};
//
//	for(auto &vertex : mesh.vertices)
//	{
//		min = min_vec(min, vertex.position);
//		max = max_vec(max, vertex.position);
//	}
//
//	return std::make_pair(min, max);
//}
//
//
//std::pair<QVector3D, QVector3D> CalcMinMaxPointsForMeshes(const QVector<Mesh> &vec_meshes)
//{
//	QVector3D min{std::numeric_limits<float>::max(),std::numeric_limits<float>::max(),std::numeric_limits<float>::max()}
//					, max{std::numeric_limits<float>::lowest(),std::numeric_limits<float>::lowest(),std::numeric_limits<float>::lowest()};
//
//	for(const auto &mesh : vec_meshes)
//	{
//		auto [mesh_min, mesh_max] = CalcMinMaxPoints(mesh);
//		min = min_vec(min, mesh_min);
//		max = max_vec(max, mesh_max);
//	}
//
//	return std::make_pair(min, max);
//}
//
//
//std::pair<QVector3D, QVector3D> CalcMinMaxPointsForMeshes(const QVector<Mesh2> &vec_meshes)
//{
//	QVector3D min{std::numeric_limits<float>::max(),std::numeric_limits<float>::max(),std::numeric_limits<float>::max()}
//	, max{std::numeric_limits<float>::lowest(),std::numeric_limits<float>::lowest(),std::numeric_limits<float>::lowest()};
//
//	for(const auto &mesh : vec_meshes)
//	{
//		auto [mesh_min, mesh_max] = CalcMinMaxPoints(mesh);
//		min = min_vec(min, mesh_min);
//		max = max_vec(max, mesh_max);
//	}
//
//	return std::make_pair(min, max);
//}
//
//
//std::pair<QVector3D, QVector3D> CalcMinMaxPoints(const Mesh2 &mesh)
//{
//	QVector3D min{std::numeric_limits<float>::max(),std::numeric_limits<float>::max(),std::numeric_limits<float>::max()}
//	, max{std::numeric_limits<float>::lowest(),std::numeric_limits<float>::lowest(),std::numeric_limits<float>::lowest()};
//
//	for(auto &vertex : mesh.vertices)
//	{
//		min = min_vec(min, vertex.position);
//		max = max_vec(max, vertex.position);
//	}
//
//	return std::make_pair(min, max);
//}
//
//
//
//Mesh2 processMesh(aiMesh *aiMesh, const aiScene *scene)
//{
//	Mesh2 mesh;
//
//	// Обработка вершин
//	for(unsigned int i = 0; i < aiMesh->mNumVertices; i++)
//	{
//		Vertex vertex;
//
//		vertex.position = {aiMesh->mVertices[i].x,					aiMesh->mVertices[i].y,					aiMesh->mVertices[i].z};
//		vertex.normal		= {aiMesh->mNormals[i].x,						aiMesh->mNormals[i].y,					aiMesh->mNormals[i].z};
//		vertex.texCoord = {aiMesh->mTextureCoords[0][i].x,	aiMesh->mTextureCoords[0][i].y};
//
//
//		mesh.vertices.emplace_back(vertex);
//	}
//
//	auto [min, max] = CalcMinMaxPoints(mesh);
//
//	mesh.m_sizes = max - min;
//	mesh.m_middle_point = (max + min) * 0.5;
//
//	// Обработка индексов
//	for(unsigned int i = 0; i < aiMesh->mNumFaces; i++)
//	{
//		aiFace face = aiMesh->mFaces[i];
//
//		if(face.mNumIndices == 3)
//			for(unsigned int j = 0; j < face.mNumIndices; j++)
//				mesh.indices.emplace_back(face.mIndices[j]);
//	}
//
//	return mesh;
//}
//
//
//void processNode(aiNode *node, const aiScene *scene, std::vector<Mesh2> &meshes)
//{
//	for(unsigned int i = 0; i < node->mNumMeshes; i++)
//	{
//		aiMesh *aiMesh = scene->mMeshes[node->mMeshes[i]];
//		Mesh2 mesh = processMesh(aiMesh, scene);
//		mesh.name = node->mName.data;
//		mesh.m_iMaterialIndex = aiMesh->mMaterialIndex;
//	}
//}
