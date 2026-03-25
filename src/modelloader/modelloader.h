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

class ModelLoader
{
public:
	//static bool load(const QString &filePath, QVector<Mesh> &meshes);
	//static QVector<Mesh> load(const QString &filePath);

	static ObjectNodeSTL *loadStl(const QString &filePath);
	static ObjectNodeOBJ *loadObj(const QString &filePath);
	static ObjectNodeGLTF *loadGltf(const QString &filePath);

	static std::shared_ptr<ObjectSTL> loadStlByIGL(const QString &filePath);
	//static ObjectNodeFBX *loadFbx(const QString &filePath);
	//static bool loadSingleMesh(const QString &filePath, Mesh &mesh);
	static QString getErrorString();
	static QStringList getSupportedFormats();

protected:
	static std::vector<Material> ProcessMaterials(const aiScene *scen);
	//static void processNode(aiNode *node, const aiScene *scene, std::vector<Mesh> &meshes);
	//static Mesh processMesh(aiMesh *aiMesh, const aiScene *scene);

	static QString m_lastError;
};




#endif