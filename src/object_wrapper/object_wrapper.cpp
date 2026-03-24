#include "object_wrapper.h"
#include "node_tree.h"

void checkGLError() {
	GLenum err;
	while ((err = glGetError()) != GL_NO_ERROR) {
		qDebug() << "OpenGL Error at" << ":" << err;
	}
}

unsigned int ObjectWrapper::GetNumberOfVertices()
{
	return m_num_of_vertices;
}

QVector3D ObjectWrapper::GetSizes()
{
	return m_vec_sizes;
}


QVector3D ObjectWrapper::GetMiddlePoint()
{
	return m_vec_middle_point;
}


float ObjectWrapper::GetMinPolySize()
{
	return m_min_poly_size;
}


float ObjectWrapper::GetMaxPolySize()
{
	return m_max_poly_size;
}


void ObjectWrapper::SetSizes(QVector3D sizes)
{
	m_vec_sizes = sizes;
}


void ObjectWrapper::SetMiddlePoint(QVector3D middle_point)
{
	m_vec_middle_point = middle_point;
}


QTreeView *ObjectWrapper::createTreeView()
{
	return nullptr;
}

ObjectWrapperSTL::ObjectWrapperSTL(QVector<Vertex> vertices)
{
	m_num_of_vertices = vertices.size();

	QVector3D min_coords{0,0,0};
	QVector3D max_coords{0,0,0};

	for(const auto &[vertex, normal, texCoord] : vertices)
	{
		min_coords = QVector3D{std::min(min_coords.x(), vertex.x())
												 , std::min(min_coords.y(), vertex.y())
												 , std::min(min_coords.z(), vertex.z())};

		max_coords = QVector3D{std::max(max_coords.x(), vertex.x())
												 , std::max(max_coords.y(), vertex.y())
												 , std::max(max_coords.z(), vertex.z())};
	}
	m_min_poly_size = 1000;
	m_max_poly_size = -1000;
	for(int i = 0; i < vertices.size(); i += 3)
	{
		float S = QVector3D::crossProduct(vertices[i].position - vertices[i+1].position, vertices[i+1].position - vertices[i+2].position).length();
		m_min_poly_size = std::min(S, m_min_poly_size);
		m_max_poly_size = std::max(S, m_max_poly_size);
	}

	m_vec_middle_point	= (max_coords + min_coords) * 0.5;
	m_vec_sizes					= max_coords - min_coords;

	checkGLError();
	m_vao.create();
	m_vao.bind();

	checkGLError();
	// 5. Настройка VBO (Вершины)
	m_vbo.create();
	m_vbo.bind();
	m_vbo.allocate(vertices.data(), vertices.size() * sizeof(Vertex));

	checkGLError();
	m_vbo.release();
	m_vao.release();
	checkGLError();
}


ObjectWrapperSTL::ObjectWrapperSTL(ObjectNodeSTL *pNode)
	: m_stl_root{pNode}
{
	SetSizes(m_stl_root->GetSizes());
	SetMiddlePoint(m_stl_root->GetMiddlePoint());
}

void ObjectWrapperSTL::SetAttributes(QOpenGLShaderProgram *program)
{
	if(program && m_stl_root)
		m_stl_root->SetAttributes(program);
}


void ObjectWrapperSTL::DrawByShader(QOpenGLShaderProgram* program)
{
	if (!program)
		return;

	program->bind();

	for(auto &mesh : m_stl_root->m_vecMeshes)
	{
		mesh.m_ptr_vao->bind();
		glDrawArrays(GL_TRIANGLES, 0, mesh.vertices.size());
		mesh.m_ptr_vao->release();
	}

	for(auto& node :m_stl_root->m_vecChildren)
		for(auto &mesh : node->m_vecMeshes)
		{
			mesh.m_ptr_vao->bind();
			glDrawArrays(GL_TRIANGLES, 0, mesh.vertices.size());
			mesh.m_ptr_vao->release();
		}

	program->release();
}

#include <QStandardItem>
#include <QStandardItemModel>

void addNodeToModel(QStandardItem *parentItem, QStandardItemModel *model)
{
	QStandardItem *item = new QStandardItem(QString("123"));

	// Добавляем информацию о типе (можно расширить)
	QStandardItem *typeItem = new QStandardItem("Node");

	parentItem->appendRow(QList<QStandardItem *>() << item << typeItem);

	// Рекурсивно добавляем детей
	//for(unsigned int i = 0; i < node->mNumChildren; ++i)
	{
		//addNodeToModel(node->mChildren[i], item, model);
	}
}


QTreeView* ObjectWrapperSTL::createTreeView()
{
	//auto pTree = new QTreeView();

	//QStandardItem *typeItem = new QStandardItem("Node");
	//QStandardItemModel *model = new QStandardItemModel();

	//pTree->setModel(model);
	////auto *pRoot = model->invisibleRootItem();
	////pRoot->appendRow(QList<QStandardItem *>() << new QStandardItem("Node"));
	////pRoot->appendRow(QList<QStandardItem *>() << new QStandardItem("Node"));
	////pRoot->appendRow(QList<QStandardItem *>() << new QStandardItem("Node"));
	////pRoot->appendRow(QList<QStandardItem *>() << new QStandardItem("Node"));
	//model->appendRow(CreateTreeFromObjectNode(m_stl_root));
	////CreateTreeFromObjectNode(m_stl_root);

	auto pTree = new QTreeView();
	QStandardItemModel *model = new QStandardItemModel();
	pTree->setModel(model);
	auto* pItem = TreeNodeVisitor{}.FromSTL(m_stl_root);
	auto *pRoot = model->invisibleRootItem();
	pRoot->appendRow(pItem);
	return pTree;
};


ObjectWrapperOBJ::ObjectWrapperOBJ(QVector<Mesh> meshes, QObject* parent)
{
	for (const auto& mesh_ : meshes)
	{
		auto& mesh = m_vec_meshes.emplace_back();

		mesh.m_number_of_indices = mesh_.indices.size();
		mesh.m_ptr_vao.reset(new QOpenGLVertexArrayObject());
		mesh.m_ptr_vao->create();
		mesh.m_ptr_vao->bind();

		mesh.m_vbo.create();
		mesh.m_vbo.bind();
		mesh.m_vbo.allocate(mesh_.vertices.data(), mesh_.vertices.size()* sizeof(Vertex));

		mesh.m_ebo.create();
		mesh.m_ebo.bind();
		mesh.m_ebo.allocate(mesh_.indices.data(), mesh_.indices.size() * sizeof(unsigned int));
	}

	m_vec_middle_point;
}


ObjectWrapperOBJ::ObjectWrapperOBJ(QVector<Mesh> meshes, std::vector<Material>&& vec_materials)
{
	m_vec_materials = std::move(vec_materials);

	for(const auto &mesh_ : meshes)
	{
		auto &mesh = m_vec_meshes.emplace_back();

		mesh.m_number_of_indices = mesh_.indices.size();
		mesh.m_ptr_vao.reset(new QOpenGLVertexArrayObject());
		mesh.m_ptr_vao->create();
		mesh.m_ptr_vao->bind();

		mesh.m_vbo.create();
		mesh.m_vbo.bind();
		mesh.m_vbo.allocate(mesh_.vertices.data(), mesh_.vertices.size()* sizeof(Vertex));

		mesh.m_ebo.create();
		mesh.m_ebo.bind();
		mesh.m_ebo.allocate(mesh_.indices.data(), mesh_.indices.size() * sizeof(unsigned int));
	}
}


ObjectWrapperOBJ::ObjectWrapperOBJ(ObjectNodeOBJ *pNode)
	: m_pRootNode{pNode}
{
}

void ObjectWrapperOBJ::SetAttributes(QOpenGLShaderProgram* program)
{
	if (!program)
		return;

	checkGLError();
	program->bind();

	for (auto& mesh : m_vec_meshes)
	{
		mesh.m_ptr_vao->bind();
		mesh.m_vbo.bind();
		mesh.m_ebo.bind();	
		
		program->setAttributeBuffer(0, GL_FLOAT, 0,											3, sizeof(Vertex));											// vertex
		program->setAttributeBuffer(1, GL_FLOAT, sizeof(QVector3D),			3, sizeof(Vertex));			//normal
		program->setAttributeBuffer(2, GL_FLOAT, 2 * sizeof(QVector3D), 2, sizeof(Vertex));	//texture_coord

		program->enableAttributeArray(0);
		program->enableAttributeArray(1);
		program->enableAttributeArray(2);

		mesh.m_ptr_vao->release();
		mesh.m_vbo.release();
		mesh.m_ebo.release();
	}
	
	program->release();
	checkGLError();
}


void ObjectWrapperOBJ::SetMaterial(QOpenGLShaderProgram *program, int i)
{
	if(m_vec_materials.size() <= i || !program)
		return;

	auto &material = m_vec_materials[i];

	if(material.m_texture_ambient)
	{
		material.m_texture_ambient->bind(0);
		program->setUniformValue("u_diffuseMap", 0);
		program->setUniformValue("hasAmbientMap", true);
	}
	else
	{
		program->setUniformValue("hasAmbientMap", false);
		program->setUniformValue("Ka", material.ambientColor);
	}

	if(material.m_texture_diffuse)
	{
		material.m_texture_diffuse->bind(1);
		program->setUniformValue("u_diffuseMap", 1);
		program->setUniformValue("hasDiffuseMap", true);
	}
	else
	{
		program->setUniformValue("hasDiffuseMap", false);
		program->setUniformValue("Kd", material.diffuseColor);
	}

	if(material.m_texture_specular)
	{
		material.m_texture_specular->bind(2);
		program->setUniformValue("u_specularMap", 2);
		program->setUniformValue("hasSpecularMap", true);
	}
	else
	{
		program->setUniformValue("hasSpecularMap", false);
		program->setUniformValue("Ks", material.specularColor);
	}
	program->setUniformValue("Kd", material.diffuseColor);
	program->setUniformValue("Ks", material.specularColor);
	program->setUniformValue("Ns", material.shininess);
}


void ObjectWrapperOBJ::DrawByShader(QOpenGLShaderProgram* program)
{
	if (!program)
		return;

	program->bind();

	for (auto& mesh : m_vec_meshes)
	{
		mesh.m_ptr_vao->bind();

		SetMaterial(program, 0);
		glDrawElements(GL_TRIANGLES, mesh.m_number_of_indices, GL_UNSIGNED_INT, 0);
		mesh.m_ptr_vao->release();
	}
	program->release();
}


ObjectWrapperGLTF::ObjectWrapperGLTF(ObjectNodeGLTF *pNode)
	: m_pRootNode{pNode}
{
}

ObjectWrapperGLTF::ObjectWrapperGLTF(QVector<Mesh> meshes, std::vector<Material> &&vec_materials)
{
	m_vec_materials = std::move(vec_materials);

	for(const auto &mesh_ : meshes)
	{
		auto &mesh = m_vec_meshes.emplace_back();

		mesh.m_number_of_indices = mesh_.indices.size();
		mesh.m_ptr_vao.reset(new QOpenGLVertexArrayObject());
		mesh.m_ptr_vao->create();
		mesh.m_ptr_vao->bind();

		mesh.m_vbo.create();
		mesh.m_vbo.bind();
		mesh.m_vbo.allocate(mesh_.vertices.data(), mesh_.vertices.size()* sizeof(Vertex));

		mesh.m_ebo.create();
		mesh.m_ebo.bind();
		mesh.m_ebo.allocate(mesh_.indices.data(), mesh_.indices.size() * sizeof(unsigned int));
	}
}

void ObjectWrapperGLTF::DrawByShader(QOpenGLShaderProgram *program)
{
	if(!program)
		return;

	program->bind();

	for(auto &mesh : m_vec_meshes)
	{
		mesh.m_ptr_vao->bind();

		SetMaterial(program, 0);
		glDrawElements(GL_TRIANGLES, mesh.m_number_of_indices, GL_UNSIGNED_INT, 0);
		mesh.m_ptr_vao->release();
	}
	program->release();
}

void ObjectWrapperGLTF::SetAttributes(QOpenGLShaderProgram *program)
{
	if(!program)
		return;

	checkGLError();
	program->bind();

	for(auto &mesh : m_vec_meshes)
	{
		mesh.m_ptr_vao->bind();
		mesh.m_vbo.bind();
		mesh.m_ebo.bind();

		program->setAttributeBuffer(0, GL_FLOAT, 0, 3, sizeof(Vertex));											// vertex
		program->setAttributeBuffer(1, GL_FLOAT, sizeof(QVector3D), 3, sizeof(Vertex));			//normal
		program->setAttributeBuffer(2, GL_FLOAT, 2 * sizeof(QVector3D), 2, sizeof(Vertex));	//texture_coord

		program->enableAttributeArray(0);
		program->enableAttributeArray(1);
		program->enableAttributeArray(2);

		mesh.m_ptr_vao->release();
		mesh.m_vbo.release();
		mesh.m_ebo.release();
	}

	program->release();
	checkGLError();
}

void ObjectWrapperGLTF::SetMaterial(QOpenGLShaderProgram *program, int i)
{
	if(m_vec_materials.size() <= i || !program)
		return;

	auto &material = m_vec_materials[i];

	if(material.m_texture_ambient)
	{
		material.m_texture_ambient->bind(0);
		program->setUniformValue("u_diffuseMap", 0);
		program->setUniformValue("hasAmbientMap", true);
	}
	else
	{
		program->setUniformValue("hasAmbientMap", false);
		//program->setUniformValue("Ka", material.ambientColor);
		program->setUniformValue("Ka", QVector3D(0.0, 0.0, 0.0));
	}

	if(material.m_texture_diffuse)
	{
		material.m_texture_diffuse->bind(1);
		program->setUniformValue("u_diffuseMap", 1);
		program->setUniformValue("hasDiffuseMap", true);
	}
	else
	{
		program->setUniformValue("hasDiffuseMap", false);
		program->setUniformValue("Kd", material.diffuseColor);
	}

	if(material.m_texture_specular)
	{
		material.m_texture_specular->bind(2);
		program->setUniformValue("u_specularMap", 2);
		program->setUniformValue("hasSpecularMap", true);
	}
	else
	{
		program->setUniformValue("hasSpecularMap", false);
		program->setUniformValue("Ks", material.specularColor);
	}
	program->setUniformValue("Kd", material.diffuseColor);
	program->setUniformValue("Ks", material.specularColor);
	program->setUniformValue("Ns", material.shininess);
}