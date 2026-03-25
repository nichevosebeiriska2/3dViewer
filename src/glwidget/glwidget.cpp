#include "glwidget.h"
#include <QMouseEvent>
#include <QtMath>
#include <QVector3D>
#include <QOpenGLFunctions>
#include <QMimeData>
#include <QFileInfo>

#include <iostream>
#include <algorithm>

#include "shader_program_interface.h"
#include "modelloader.h"
//#include "model_loader_obj.h"
//#include "model_loader_gltf.h"
#include "global.h"
#include "node_tree.h"

GLWidget::GLWidget(QWidget *parent)
	: QOpenGLWidget(parent)
	, m_program(nullptr)
	, m_rotation(0.0f)
{
	// Формат OpenGL (версия 3.3 Core)
	QSurfaceFormat format;
	format.setVersion(3, 3);
	format.setProfile(QSurfaceFormat::CoreProfile);
	format.setDepthBufferSize(24);
	format.setSamples(4);
	setFormat(format);
	setAcceptDrops(true);
}


void GLWidget::AddObject(ObjectWrapper *wrapper)
{
	m_ptr_object.reset(wrapper);
	m_ptr_object->SetAttributes(m_program);
	//UpdateTree(CreateTreeView());
}


void GLWidget::AddStlObject(std::shared_ptr< ObjectSTL> ptr_stl)
{
	m_ptr_stl_object = std::move(ptr_stl);
	m_ptr_stl_object->SetAttributes(m_program);
}


void GLWidget::SetLightAngles(float hor, float ver)
{
	m_light_angle_hor = 3.6 * hor;
	m_light_angle_ver = 3.6 * ver;
}

void GLWidget::SetModelColor(QColor color)
{
	m_model_color = QVector3D(color.red(), color.green(), color.blue()) / 255 ;
}


void GLWidget::SetDefaultOBJShader()
{
	m_program = *ShaderProgramInterface::LoadByObjectType(ShaderProgramInterface::ObjectType::OBJ, this);
	m_ptr_object->SetAttributes(m_program);
}


QTreeView *GLWidget::CreateTreeView()
{
	return m_ptr_object->createTreeView();
}


void GLWidget::SetDefaultShader()
{
	m_program = *ShaderProgramInterface::LoadByObjectType(ShaderProgramInterface::ObjectType::STL, this);
	m_ptr_object->SetAttributes(m_program);
}


void GLWidget::SetMeshShader()
{
	m_program = *ShaderProgramInterface::LoadByObjectType(ShaderProgramInterface::ObjectType::STL_MESH, this);
	m_ptr_object->SetAttributes(m_program);
}


void GLWidget::SetMeshSizeShader()
{
	m_program = *ShaderProgramInterface::LoadByObjectType(ShaderProgramInterface::ObjectType::STL_COLORED, this);
	m_ptr_object->SetAttributes(m_program);
}


GLWidget::~GLWidget()
{
	makeCurrent();
	delete m_program;
	doneCurrent();
}

void GLWidget::initializeGL()
{
	// Инициализация OpenGL функций
	initializeOpenGLFunctions();

	// Включение тестов
	glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
	glEnable(GL_DEPTH_TEST);

	auto shader_program_ptr = ShaderProgramInterface::LoadByObjectType(ShaderProgramInterface::ObjectType::glass, this);
	//auto shader_program_ptr = ShaderProgramInterface::LoadByObjectType(ShaderProgramInterface::ObjectType::STL_MESH, this);
	//auto shader_program_ptr = ShaderProgramInterface::LoadByObjectType(ShaderProgramInterface::ObjectType::STL_COLORED, this);
	auto shader_axis = ShaderProgramInterface::LoadByObjectType(ShaderProgramInterface::ObjectType::SCENE_AXIS, this);
	if(!shader_program_ptr)
	{
		m_program = nullptr;
		std::cout<<"Error loading shader";
		return;
	}

	m_program = *shader_program_ptr;
}

QVector3D createVectorFromAngles(float azimuth, float elevation, float length = 1.0f)
{
	// Перевод в радианы
	float azRad = qDegreesToRadians(azimuth);
	float elRad = qDegreesToRadians(elevation);

	// Сферические → Декартовы координаты
	float x = length * qCos(elRad) * qSin(azRad);
	float y = length * qSin(elRad);
	float z = length * qCos(elRad) * qCos(azRad);

	return QVector3D(x, y, z);
}

void GLWidget::paintGL()
{
	if (!str_next_file.isEmpty())
	{
		auto ext = std::filesystem::path(str_next_file.toStdString()).extension().string();
		if(std::filesystem::path(str_next_file.toStdString()).extension().string() == ".stl")
		{
			AddObject(new ObjectWrapperSTL(ModelLoader{}.loadStl(str_next_file)));
			AddStlObject(ModelLoader{}.loadStlByIGL(str_next_file));
			//auto res = ModelLoader{}.loadStlByIGL(str_next_file);
		}
		else if(std::filesystem::path(str_next_file.toStdString()).extension().string() == ".obj")
		{
			AddObject(new ObjectWrapperOBJ(ModelLoader{}.loadObj(str_next_file)));
			SetDefaultOBJShader();
		}
		else if(std::filesystem::path(str_next_file.toStdString()).extension().string() == ".gltf")
		{
			AddObject(new ObjectWrapperGLTF(ModelLoader{}.loadGltf(str_next_file)));
			SetDefaultOBJShader();
		}
		str_next_file.clear();
	}

	if(!m_ptr_object || !m_program)
		return;

	glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	//glEnable(GL_DEPTH_TEST);

	

	m_program->bind();

	QMatrix4x4 mvp;

	mvp.perspective(45.0f, float(width()) / height(), 0.1f, 1000.0f);
	mvp.translate(0.0f, 0.0f, - m_distance_to_center);

	QMatrix4x4 model = GetRotationMatrix() * GetScaleMatrix() * GetOffsetMatrix();
	
	m_program->setUniformValue("mvp", mvp);
	m_program->setUniformValue("model", model);

	auto my_pos = model.inverted() * QVector3D(0, 0, m_distance_to_center);
	m_program->setUniformValue("light_pos", createVectorFromAngles(m_light_angle_ver, m_light_angle_hor, 1000));
	m_program->setUniformValue("my_position", my_pos);
	m_program->setUniformValue("model_color", m_model_color);
	m_program->setUniformValue("min_poly_size", m_ptr_object->GetMinPolySize());
	m_program->setUniformValue("max_poly_size", m_ptr_object->GetMaxPolySize());

	m_ptr_stl_object->m_vao.bind();
	glDrawArrays(GL_TRIANGLES, 0, 120000);
	m_ptr_stl_object->m_vao.release();
	//m_ptr_object->DrawByShader(m_program);

	// Анимация
	m_rotation += 1.0f;
	update();  // Запрос следующего кадра


}

void GLWidget::resizeGL(int w, int h)
{
	glViewport(0, 0, w, h);
}


void GLWidget::wheelEvent(QWheelEvent *event)
{
	m_distance_to_center += event->angleDelta().y() * 0.001;
}


void GLWidget::mousePressEvent(QMouseEvent *event)
{
	m_lastPos = event->pos();
	m_mousePressed = true;

	// Левая кнопка - вращение, Правая - панорамирование
	if(event->button() == Qt::LeftButton)
		m_mouseMode = MouseMode::Rotate;
	
	else if(event->button() == Qt::RightButton)
		m_mouseMode = MouseMode::Pan;

	update();
}

void GLWidget::mouseMoveEvent(QMouseEvent *event)
{
	if(!m_mousePressed)
		return;

	QPoint delta = event->pos() - m_lastPos;
	m_lastPos = event->pos();

	switch(m_mouseMode)
	{
		case MouseMode::Rotate:
			// Вращение: движение по X вращает вокруг Y, по Y вращает вокруг X
			m_rotationY += delta.x() * 0.5f;
			m_rotationX += delta.y() * 0.5f;
			// Ограничение вращения по X (чтобы не переворачивалось)
			m_rotationX = qBound(-90.0f, m_rotationX, 90.0f);
			break;

		case MouseMode::Pan:
			// Панорамирование
			m_rotationZ += (delta.x() - delta.y()) * 0.5f;
			break;

		case MouseMode::None:
			break;
	}

	update();
}

void GLWidget::mouseReleaseEvent(QMouseEvent *event)
{
	if(event->button() == Qt::LeftButton || event->button() == Qt::RightButton)
	{
		m_mousePressed = false;
		m_mouseMode = MouseMode::None;
	}
}

void GLWidget::dragEnterEvent(QDragEnterEvent *event)
{
	if(event->mimeData()->hasUrls())
	{
		// Принимаем событие (курсор изменится на "можно бросить")
		event->acceptProposedAction();
	}
	else
	{
		// Иначе игнорируем
		event->ignore();
	}
}


void GLWidget::dropEvent(QDropEvent *event)
{
	const QMimeData *mimeData = event->mimeData();
	if(mimeData->hasUrls())
	{
		for(const QUrl &url : mimeData->urls())
		{
			// Конвертируем URL в локальный путь к файлу
			QString filePath = url.toLocalFile();

			// 4. Проверяем расширение файла (опционально, но рекомендуется)
			QFileInfo fileInfo(filePath);
			QString suffix = fileInfo.suffix().toLower();

			const auto vec_enabled_types = Global::GetEnabledFileFormats();

			if(std::ranges::find(vec_enabled_types, suffix) != std::end(vec_enabled_types))
				str_next_file = filePath;

			else
			{
				qDebug() << "Unsupported file format:" << suffix;
			}
		}
		event->acceptProposedAction();
	}
	else
	{
		event->ignore();
	}
}

QMatrix4x4 GLWidget::GetRotationMatrix()
{
	QMatrix4x4 mat;
	mat.rotate(m_rotationX, 1.0f, 0.0f, 0.0f);
	mat.rotate(m_rotationY, 0.0f, 1.0f, 0.0f);
	mat.rotate(m_rotationZ, 0.0f, 0.0f, 1.0f);
	
	return mat;
}


QMatrix4x4 GLWidget::GetScaleMatrix()
{
	QMatrix4x4 mat;
	mat.scale(1.0f / std::max(m_ptr_object->GetSizes().x(), std::max(m_ptr_object->GetSizes().y(), m_ptr_object->GetSizes().z())));
	return mat;

}


QMatrix4x4 GLWidget::GetOffsetMatrix()
{
	QMatrix4x4 mat;
	mat.translate(-m_ptr_object->GetMiddlePoint());
	return mat;
}