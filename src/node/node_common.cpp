#include "node_common.h"


QVector3D ObjectNode::GetSizes()
{
	return m_node_sizes;
}


QVector3D ObjectNode::GetMiddlePoint()
{
	return m_node_middle_point;
}

QVector3D createMinVec3()
{
	return QVector3D{std::numeric_limits<float>::lowest(), std::numeric_limits<float>::lowest(), std::numeric_limits<float>::lowest()};
}

QVector3D createMaxVec3()
{
	return QVector3D{std::numeric_limits<float>::max(), std::numeric_limits<float>::max(), std::numeric_limits<float>::max()};
}

QVector3D max_vec(const QVector3D &left, const QVector3D &right)
{
	return QVector3D{std::max(left.x(), right.x()), std::max(left.y(), right.y()), std::max(left.z(), right.z())};
}

QVector3D min_vec(const QVector3D &left, const QVector3D &right)
{
	return QVector3D{std::min(left.x(), right.x()), std::min(left.y(), right.y()), std::min(left.z(), right.z())};
}