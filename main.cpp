
#include <QApplication>
#include <QSurfaceFormat>
#include <filesystem>
#include <iostream>

#include "src/mainwindow/mainwindow.h"

#include "src/modelloader/modelloader.h"
#include "global.h"

int main(int argc, char *argv[])
{
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