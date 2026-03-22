#include "object_wrapper.h"

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

	m_vao.create();
	m_vao.bind();

	// 5. Настройка VBO (Вершины)
	m_vbo.create();
	m_vbo.bind();
	m_vbo.allocate(vertices.data(), vertices.size() * sizeof(Vertex));

	m_vbo.release();
	m_vao.release();
}

void ObjectWrapperSTL::SetAttributes(QOpenGLShaderProgram *program)
{
	if(!program)
		return;


	program->bind();
	Bind();

	program->setAttributeBuffer(0, GL_FLOAT, 0, 3, sizeof(Vertex));
	program->setAttributeBuffer(1, GL_FLOAT, sizeof(QVector3D), 3, sizeof(Vertex));
	program->enableAttributeArray(0);
	program->enableAttributeArray(1);

	Release();
	program->release();

}


void ObjectWrapperSTL::Bind()
{
	m_vao.bind();
	m_vbo.bind();
}

void ObjectWrapperSTL::Release()
{
	m_vao.release();
	m_vbo.release();
}

void ObjectWrapperSTL::DrawByShader(QOpenGLShaderProgram* program)
{
	if (!program)
		return;

	program->bind();
	m_vao.bind();


	glDrawArrays(GL_TRIANGLES, 0, GetNumberOfVertices());


	m_vao.release();
}


ObjectWrapperOBJ::ObjectWrapperOBJ(QVector<Mesh> meshes, QObject* parent)
{
	for (const auto& mesh_ : meshes)
	{
		auto& mesh = m_vec_heshes.emplace_back();
		checkGLError();
		auto& vbo = m_vec_vbos.emplace_back(QOpenGLBuffer::VertexBuffer);
		auto& ebo = m_vec_ebos.emplace_back(QOpenGLBuffer::IndexBuffer);
		auto& ptr_vao = m_vec_vaos.emplace_back(new QOpenGLVertexArrayObject());
		
		mesh.m_numbder_of_indices = mesh_.indices.size();
		mesh.m_vao.reset(new QOpenGLVertexArrayObject());
		mesh.m_vao->create();
		mesh.m_vao->bind();

		mesh.m_vbo.create();
		mesh.m_vbo.bind();
		mesh.m_vbo.allocate(mesh_.vertices.data(), mesh_.vertices.size());

		mesh.m_ebo.create();
		mesh.m_ebo.bind();
		mesh.m_ebo.allocate(mesh_.indices.data(), mesh_.indices.size());

		//mesh.m_vao->release();
		//mesh.m_vbo.release();
		//mesh.m_ebo.release();
		checkGLError();
	}

	m_vec_middle_point;
}


void ObjectWrapperOBJ::SetAttributes(QOpenGLShaderProgram* program)
{
	if (!program)
		return;

	checkGLError();
	program->bind();

	for (auto& mesh : m_vec_heshes)
	{
		mesh.m_vao->bind();
		mesh.m_vbo.bind();
		program->setAttributeBuffer(0, GL_FLOAT, 0, 3, sizeof(Vertex));
		//program->setAttributeBuffer(1, GL_FLOAT, sizeof(QVector3D), 3, sizeof(Vertex));
		program->enableAttributeArray(0);
		//program->enableAttributeArray(1);
		//mesh.m_vao->release();
		//mesh.m_vbo.release();
	}
	//program->setAttributeBuffer(0, GL_FLOAT, 0, 3, sizeof(Vertex));
	//program->setAttributeBuffer(1, GL_FLOAT, sizeof(QVector3D), 3, sizeof(Vertex));
	//program->enableAttributeArray(0);
	//program->enableAttributeArray(1);
	
	program->release();
	checkGLError();
}


void ObjectWrapperOBJ::Bind() 
{
	//m_vao.bind();
}


void ObjectWrapperOBJ::Release()
{
	//m_vao.release();
}


void ObjectWrapperOBJ::DrawByShader(QOpenGLShaderProgram* program)
{
	if (!program)
		return;

	program->bind();

	for (auto& mesh : m_vec_heshes)
	{
		mesh.m_vao->bind();
		//glDrawElements(GL_POINTS, mesh.m_numbder_of_indices, GL_UNSIGNED_INT, 0);
		//glDrawElements(GL_TRIANGLES, mesh.m_numbder_of_indices, GL_UNSIGNED_INT, 0);
		glDrawArrays(GL_TRIANGLES, 0, mesh.m_numbder_of_indices);
		//glDrawArrays(GL_POINTS, 0, mesh.m_numbder_of_indices);
		glDrawArrays(GL_TRIANGLES, 0, mesh.m_numbder_of_indices);
		checkGLError();
		mesh.m_vao->release();
	}
	program->release();
}