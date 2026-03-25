
#include <QApplication>
#include <QSurfaceFormat>
#include <filesystem>
#include <iostream>

#include "src/mainwindow/mainwindow.h"

#include "src/modelloader/modelloader.h"
#include "global.h"

#include <igl/cotmatrix.h>
#include <igl/read_triangle_mesh.h>
#include <igl/is_vertex_manifold.h>
#include <Eigen/Dense>

int main(int argc, char *argv[])
{
	std::string str_to_stl = R"(C:\work\69-figma_parser\Tools\QtTest\models\stl\skeleton-octopus.stl)";
	Eigen::MatrixXd V;
	Eigen::MatrixXi F;

	//if(!igl::read_triangle_mesh(str_to_stl, V, F))
	//{
	//	std::cerr << "Ошибка загрузки модели" << std::endl;
	//	return 1;
	//}

	QSurfaceFormat format;
	format.setVersion(4, 5);
	format.setProfile(QSurfaceFormat::CompatibilityProfile);
	format.setDepthBufferSize(24);
	format.setSamples(4);
	QSurfaceFormat::setDefaultFormat(format);

	QApplication app(argc, argv);


	MainWindow window;
	window.show();

	return app.exec();
}