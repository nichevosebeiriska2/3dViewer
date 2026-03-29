#ifndef MODELLOADER_H
#define MODELLOADER_H

#include <memory>

#include <QString>
#include <QVector>
#include <QVector3D>

#include <assimp/Importer.hpp>
#include <assimp/scene.h>      // ← aiNode объявлен здесь!
#include <assimp/postprocess.h>

#include "common.h"

#include "WrapperSTL.h"

class ModelLoader
{
public:
	static ObjectNodeSTL *loadStl(const QString &filePath);
	static ObjectNodeOBJ *loadObj(const QString &filePath);
	static ObjectNodeGLTF *loadGltf(const QString &filePath);

	static std::shared_ptr<ObjectSTL> loadStlByIGL(const QString &filePath);

	static QString getErrorString();
	static QStringList getSupportedFormats();

protected:
	static std::vector<Material> ProcessMaterials(const aiScene *scen);

	static QString m_lastError;
};




#endif