#ifndef GLWIDGET_H
#define GLWIDGET_H

#include <QOpenGLWidget>
#include <QOpenGLFunctions>
#include <QOpenGLShaderProgram>
#include <QOpenGLBuffer>
#include <QOpenGLVertexArrayObject>

#include <memory>

#include "object_wrapper.h"
#include "shader_program_interface.h"


class GLWidget : public QOpenGLWidget, protected QOpenGLFunctions
{
	Q_OBJECT

public:
	explicit GLWidget(QWidget *parent = nullptr);
	~GLWidget();

	void AddObject(ObjectWrapper *wrapper);
	void AddStlObject(std::shared_ptr<ObjectSTL> ptr_stl);
	void SetLightAngles(float hor, float ver);
	void SetModelColor(QColor color);

	void SetDefaultShader();
	void SetMeshShader();
	void SetMeshSizeShader();

	void SetDefaultOBJShader();
	QTreeView *CreateTreeView();

signals:
	void UpdateTree(QTreeView* pTree);

protected:
	void initializeGL() override;
	void paintGL() override;
	void resizeGL(int w, int h) override;

	void wheelEvent(QWheelEvent *event) override;
	void mousePressEvent(QMouseEvent *event) override;
	void mouseMoveEvent(QMouseEvent *event) override;
	void mouseReleaseEvent(QMouseEvent *event) override;
	
	void dragEnterEvent(QDragEnterEvent *event) override;
	void dropEvent(QDropEvent *event) override;

	QMatrix4x4 GetRotationMatrix();
	QMatrix4x4 GetScaleMatrix();
	QMatrix4x4 GetOffsetMatrix();

private:
	QOpenGLShaderProgram *m_program;
	std::unique_ptr<ObjectWrapper> m_ptr_object;
	std::shared_ptr<ObjectSTL> m_ptr_stl_object{nullptr};

	QString str_next_file;
	float m_distance_to_center{3};
	float m_rotation;
	float m_light_angle_hor{0};
	float m_light_angle_ver{0};

	float m_rotationX{0.0f};      // Вращение вокруг X
	float m_rotationY{0.0f};      // Вращение вокруг Y
	float m_rotationZ{0.0f};      // Вращение вокруг X
	float m_zoom;           // Дистанция камеры
	float m_panX;           // Панорамирование X
	float m_panY;           // Панорамирование Y

	QVector3D m_model_color{0.5, 0.5, 0.5};
	QPoint m_lastPos;
	bool m_mousePressed;
	enum class MouseMode
	{
		None, Rotate, Pan
	};
	MouseMode m_mouseMode;

	//void updateCamera();
};

#endif // GLWIDGET_H