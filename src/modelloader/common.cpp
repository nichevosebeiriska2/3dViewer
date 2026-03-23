#include "common.h"
#include <limits>

QVector3D max_vec(const QVector3D &left, const QVector3D &right)
{
	return QVector3D{std::max(left.x(), right.x()), std::max(left.y(), right.y()), std::max(left.z(), right.z())};
}

QVector3D min_vec(const QVector3D &left, const QVector3D &right)
{
	return QVector3D{std::min(left.x(), right.x()), std::min(left.y(), right.y()), std::min(left.z(), right.z())};
}

std::pair<QVector3D, QVector3D> CalcMinMaxPoints(const Mesh &mesh)
{
	QVector3D min{std::numeric_limits<float>::max(),std::numeric_limits<float>::max(),std::numeric_limits<float>::max()}
					, max{std::numeric_limits<float>::lowest(),std::numeric_limits<float>::lowest(),std::numeric_limits<float>::lowest()};

	for(auto &vertex : mesh.vertices)
	{
		min = min_vec(min, vertex.position);
		max = max_vec(max, vertex.position);
	}

	return std::make_pair(min, max);
}


std::pair<QVector3D, QVector3D> CalcMinMaxPointsForMeshes(const QVector<Mesh> &vec_meshes)
{
	QVector3D min{std::numeric_limits<float>::max(),std::numeric_limits<float>::max(),std::numeric_limits<float>::max()}
					, max{std::numeric_limits<float>::lowest(),std::numeric_limits<float>::lowest(),std::numeric_limits<float>::lowest()};

	for(const auto &mesh : vec_meshes)
	{
		auto [mesh_min, mesh_max] = CalcMinMaxPoints(mesh);
		min = min_vec(min, mesh_min);
		max = max_vec(max, mesh_max);
	}

	return std::make_pair(min, max);
}