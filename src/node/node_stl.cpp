#include "node_stl.h"

//std::pair<QVector3D, QVector3D> CalcMinMaxPoints(const Mesh &mesh)
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


//std::pair<QVector3D, QVector3D> CalcMinMaxPointsForMeshes(const QVector<Mesh> &vec_meshes)
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


std::pair<QVector3D, QVector3D> CalcMinMaxPoints(const ObjectNodeSTL::MeshStl &mesh)
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


std::pair<QVector3D, QVector3D> CalcMinMaxPointsForMeshes(const std::vector<Mesh2> &vec_meshes)
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


std::pair<QVector3D, QVector3D> CalcMinMaxPoints(const Mesh2 &mesh)
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


void ObjectNodeSTL::ProcessNode(aiNode *node, const aiScene *scene)
{
	for(unsigned int i = 0; i < node->mNumMeshes; i++)
	{
		m_vecMeshes.emplace_back(ProcessMesh(scene->mMeshes[node->mMeshes[i]], scene));
		//scene->mName
	}

	for(auto &mesh : m_vecMeshes)
	{
		mesh.m_ptr_vao.reset(new QOpenGLVertexArrayObject());
		mesh.m_ptr_vao->create();
		mesh.m_ptr_vao->bind();
		mesh.m_vbo.create();
		mesh.m_vbo.bind();
		mesh.m_vbo.allocate(mesh.vertices.data(), mesh.vertices.size() * sizeof(Vertex));
		mesh.m_ptr_vao->release();
		mesh.m_vbo.release();
	}

	CalculateMiddlePointByMeshes();
	CalculateSizesByMeshes();
}

void ObjectNodeSTL::SetAttributes(QOpenGLShaderProgram *program)
{
	for(auto &mesh : m_vecMeshes)
	{
		mesh.m_ptr_vao->bind();
		mesh.m_vbo.bind();

		program->setAttributeBuffer(0, GL_FLOAT, 0, 3, sizeof(Vertex));									// vertex
		program->setAttributeBuffer(1, GL_FLOAT, sizeof(QVector3D), 3, sizeof(Vertex));	//normal

		program->enableAttributeArray(0);
		program->enableAttributeArray(1);

		mesh.m_ptr_vao->release();
		mesh.m_vbo.release();
	}

	for(auto &pChild : m_vecChildren)
		pChild->SetAttributes(program);
}


ObjectNodeSTL::MeshStl ObjectNodeSTL::ProcessMesh(aiMesh *aiMesh, const aiScene *scene)
{
	MeshStl mesh;

	// Обработка вершин
	mesh.vertices.reserve(aiMesh->mNumVertices);

	for(unsigned int i = 0; i < aiMesh->mNumVertices; i++)
	{
		Vertex vertex;

		vertex.position = {aiMesh->mVertices[i].x, aiMesh->mVertices[i].y, aiMesh->mVertices[i].z};
		vertex.normal = {aiMesh->mNormals[i].x,	aiMesh->mNormals[i].y, aiMesh->mNormals[i].z};

		mesh.vertices.emplace_back(vertex);
	}

	auto [min, max] = CalcMinMaxPoints(mesh);

	mesh.m_sizes = max - min;
	mesh.m_middle_point = (max + min) * 0.5;
	mesh.m_min_point = min;
	mesh.m_max_point = max;

	// Обработка индексов
	for(unsigned int i = 0; i < aiMesh->mNumFaces; i++)
	{
		const aiFace &face = aiMesh->mFaces[i];

		for(unsigned int j = 0; j < face.mNumIndices; j++)
			mesh.indices.emplace_back(face.mIndices[j]);
	}

	return mesh;
}


void ObjectNodeSTL::CalculateMiddlePointByMeshes()
{
	if(m_vecMeshes.empty())
	{
		m_node_middle_point = QVector3D{0,0,0};
		return;
	}

	QVector3D middle_point;
	for(auto &mesh : m_vecMeshes)
		middle_point += mesh.m_middle_point;

	m_node_middle_point = middle_point / m_vecMeshes.size();
}


void ObjectNodeSTL::CalculateSizesByMeshes()
{
	if(m_vecMeshes.empty())
	{
		m_node_sizes = QVector3D{100,100,100};
		return;
	}

	QVector3D max = createMinVec3();
	QVector3D min = createMaxVec3();

	for(auto &mesh : m_vecMeshes)
	{
		max = max_vec(max, mesh.m_max_point);
		min = min_vec(min, mesh.m_min_point);
	}

	m_node_sizes = max - min;
}


ObjectNodeSTL::ObjectNodeSTL(aiNode *pNode, const aiScene *scene, ObjectNodeSTL *pParent)
{
	if(pParent)
		m_pParent = pParent;

	ProcessNode(pNode, scene);

	for(unsigned int i = 0; i < pNode->mNumChildren; i++)
		m_vecChildren.emplace_back(new ObjectNodeSTL(pNode->mChildren[i], scene, this));
}


void ObjectNodeSTL::SetParent(ObjectNodeSTL *pParent)
{
	m_pParent = pParent;
}
