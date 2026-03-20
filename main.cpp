
#include <QApplication>
#include <QSurfaceFormat>
#include <filesystem>
#include <iostream>

#include "src/mainwindow/mainwindow.h"

#include "src/modelloader/modelloader.h"
#include "global.h"

int main(int argc, char *argv[])
{
	std::filesystem::path workDir = std::filesystem::current_path();
	std::cout<<"current = "<< workDir.generic_string() << std::endl;
	// Глобальный формат OpenGL (должен быть до создания QApplication)
	QSurfaceFormat format;
	format.setVersion(4, 5);
	format.setProfile(QSurfaceFormat::CoreProfile);
	format.setDepthBufferSize(24);
	format.setSamples(4);
	QSurfaceFormat::setDefaultFormat(format);

	QApplication app(argc, argv);

	ModelLoader loader;
	QVector<Mesh> vecMesh;
	//std::filesystem::path path_to_model = "";
	QString strPath = R"(C:\work\69-figma_parser\Tools\QtTest\models\Cosplay Warhammer 40k Grenade - 7298751\files\body.stl)";
	//QString strPath = R"(C:\work\69-figma_parser\Tools\QtTest\models\73edlwf6cw74-KSR-29-sniper\KSR-29 sniper\KSR-29 sniper rifle new_stl.stl)";
	//QString strPath = R"(C:\work\69-figma_parser\Tools\QtTest\models\gdoi5oiog8hs-Black-Dragon-NEW-27.03.2017\Black Dragon NEW\Dragon 2.5_stl.stl)";
	//QString strPath = R"(C:\work\69-figma_parser\Tools\QtTest\models\1903_Wright_Flyer_-_3D_model_by_The_Smithsonian_Institution_-_Sketchfab.stl)";
	//QString strPath = R"(C:\work\69-figma_parser\Tools\QtTest\models\dota2-undying-miniature20210524-23886-14rzs2s\SkullX\dota2-undying-miniature\undying-new.stl)";
	//QString strPath = R"(C:\work\69-figma_parser\Tools\QtTest\models\Mount Everest - 721769\files\everest.stl)";

	MainWindow window;
	window.show();
	window.AddObject(new ObjectWrapperSTL(ModelLoader{}.load(strPath).front().vertices));

	return app.exec();
}