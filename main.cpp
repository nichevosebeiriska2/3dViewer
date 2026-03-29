
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

#include <igl/decimate_callback_types.h>
//#include <igl/quadric_decimation.h>


int main(int argc, char *argv[])
{
	//igl::dec
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