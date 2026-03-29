#include "mainwindow.h"
#include <QVBoxLayout>
#include <QWidget>
#include <QPushButton>
#include <QLabel>
#include <QColorDialog>
#include <QToolBar>
#include <QMenu>
#include <QStandardItemModel>

#include <iostream>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
		QToolBar *toolbar = new QToolBar;
		addToolBar(toolbar);
		toolbar->addWidget(new QPushButton("axis"));
		toolbar->addWidget(new QPushButton("bounding box"));

		QAction *viewAction = new QAction("View Mode", this);
		
		

		QWidget *centralWidget = new QWidget(this);
		setCentralWidget(centralWidget);

		m_main_layout = new QHBoxLayout(centralWidget);

		glWidget = new GLWidget(this);
		glWidget->setMinimumSize(1000, 800);
		glWidget->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);

		connect(glWidget, &GLWidget::UpdateTree, this, &MainWindow::UpdateTreeView);

		QVBoxLayout *layout = new QVBoxLayout();
    
		layout->addWidget(new QPushButton("center"));
		layout->addWidget(new QPushButton("Кнопка 2"));
		layout->addWidget(new QPushButton("Кнопка 3"));
		QPushButton *btnColor = new QPushButton("color");
		QLabel *colorPreview = new QLabel();

		connect(btnColor, &QPushButton::clicked, [=]()
		{
			QColor color = QColorDialog::getColor(Qt::white, this, "Выберите цвет");
			if(color.isValid())
			{
				colorPreview->setStyleSheet(
					QString("background-color: %1; border: 1px solid black;").arg(color.name())
				);
				// Использование цвета
				// myObject->setColor(color);
				glWidget->SetModelColor(color);
			}
		});

    layout->addWidget(btnColor);
		
    m_slider_x = new QSlider();
    m_slider_y = new QSlider();
		connect(m_slider_x, &QSlider::valueChanged, this, &MainWindow::SliderXValueChanged);
		connect(m_slider_y, &QSlider::valueChanged, this, &MainWindow::SliderYValueChanged);
		layout->addWidget(m_slider_x);
		layout->addWidget(m_slider_y);
    layout->addStretch();

	m_pDockWidget = new QDockWidget(this);

	m_pDockWidget->setFeatures(
		QDockWidget::DockWidgetFloatable |
		QDockWidget::DockWidgetMovable |
		QDockWidget::DockWidgetClosable
	);

	m_pDockWidget->setWidget(glWidget);
	addDockWidget(Qt::AllDockWidgetAreas, m_pDockWidget);

	m_main_layout->addWidget(m_pDockWidget);
    m_main_layout->addLayout(layout);

		auto* p_elem = new QStandardItemModel(this);

		//p_elem->setHorizontalHeaderLabels(QStringList() << "Node Name" << "Type");
		//auto *p_tree = new QTreeView();
		//p_tree->setModel(p_elem);

		//m_main_layout->addWidget(p_tree);

		viewAction->setMenu(CreateMenuShaders());
		toolbar->addAction(viewAction);

    setWindowTitle("Qt + OpenGL");
    resize(800, 600);
}


void MainWindow::AddObject(ObjectWrapper *ptr)
{
	glWidget->AddObject(ptr);
}


void MainWindow::SliderXValueChanged(int i)
{
  glWidget->SetLightAngles(m_slider_x->value(), m_slider_y->value());
}

void MainWindow::SliderYValueChanged(int i)
{
  glWidget->SetLightAngles(m_slider_x->value(), m_slider_y->value());
}


void MainWindow::UpdateTreeView(QTreeView *pTree)
{
	//m_main_layout->addWidget(pTree);
}


QMenu *MainWindow::CreateMenuShaders()
{
	QMenu *menuShaders = new QMenu(this);

	QAction *action_shader_default = new QAction("default");
	QAction *action_shader_mesh = new QAction("mesh");
	QAction *action_shader_mesh_size = new QAction("mesh size");

	connect(action_shader_default, &QAction::triggered, glWidget, &GLWidget::SetDefaultShader);
	connect(action_shader_mesh, &QAction::triggered, glWidget, &GLWidget::SetMeshShader);
	connect(action_shader_mesh_size, &QAction::triggered, glWidget, &GLWidget::SetMeshSizeShader);

	menuShaders->addAction(action_shader_default);
	menuShaders->addAction(action_shader_mesh);
	menuShaders->addAction(action_shader_mesh_size);
	return menuShaders;
}
