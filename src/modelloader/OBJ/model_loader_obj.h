#ifndef MODEL_LOADER_OBJ_H
#define MODEL_LOADER_OBJ_H

#include <QOpenGLTexture>
#include <QVector3D>

#include "common.h"
#include "object_wrapper.h"

class ModelLoaderOBJ
{
public:
	static ObjectWrapperOBJ* load(const QString &filePath);

private:
	static std::vector<Material> ProcessMaterials(const QString &filePath, const aiScene *scen);
	static std::unique_ptr<QOpenGLTexture> CreateTexture();

	static void processNode(aiNode *node, const aiScene *scene, QVector<Mesh> &meshes);
	static Mesh processMesh(aiMesh *aiMesh, const aiScene *scene);
	static bool loadSingleMesh(const QString &filePath, Mesh &mesh);
	static QString getErrorString();

private:
	static QString m_lastError;
	QVector3D m_middle_point;
	QVector3D m_sizes;

};

#endif // !MODEL_LOADER_OBJ_H
