#include "modelloader.h"
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include <QFileInfo>
#include <QDebug>
#include <iostream>

QString ModelLoader::m_lastError;

bool ModelLoader::load(const QString &filePath, QVector<Mesh> &meshes)
{
	Assimp::Importer importer;

	// Флаги пост-обработки
	unsigned int flags = aiProcess_Triangulate |
		aiProcess_GenNormals |
		aiProcess_FlipUVs |
		aiProcess_CalcTangentSpace |
		aiProcess_RemoveRedundantMaterials |
		aiProcess_OptimizeMeshes |
		aiProcess_SortByPType;

	const aiScene *scene = importer.ReadFile(filePath.toStdString(), flags);

	if(!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
	{
		m_lastError = QString::fromStdString(importer.GetErrorString());
		qWarning() << "Assimp error:" << m_lastError;
		return false;
	}

	meshes.clear();
	processNode(scene->mRootNode, scene, meshes);

	if(meshes.isEmpty())
	{
		m_lastError = "No meshes found in file";
		return false;
	}

	return true;
}


QVector<Mesh> ModelLoader::load(const QString &filePath)
{
	Assimp::Importer importer;
	QVector<Mesh> meshes;

	// Флаги пост-обработки
	unsigned int flags = aiProcess_Triangulate;
		
		//|aiProcess_GenNormals |
		//aiProcess_FlipUVs |
		//aiProcess_CalcTangentSpace |
		//aiProcess_RemoveRedundantMaterials;
		//aiProcess_OptimizeMeshes |
		//aiProcess_SortByPType;

	const aiScene *scene = importer.ReadFile(filePath.toStdString(), flags);

	if(!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
	{
		m_lastError = QString::fromStdString(importer.GetErrorString());
		qWarning() << "Assimp error:" << m_lastError;
		return {};
	}

	meshes.clear();
	processNode(scene->mRootNode, scene, meshes);

	if(meshes.isEmpty())
	{
		m_lastError = "No meshes found in file";
		return {};
	}

	return meshes;
}

bool ModelLoader::loadSingleMesh(const QString &filePath, Mesh &mesh)
{
	QVector<Mesh> meshes;

	if(!load(filePath, meshes))
	{
		return false;
	}

	// Объединяем все меши в один
	mesh.vertices.clear();
	mesh.indices.clear();

	int indexOffset = 0;
	for(const auto &m : meshes)
	{
		mesh.vertices.append(m.vertices);

		for(unsigned int idx : m.indices)
		{
			mesh.indices.append(idx + indexOffset);
		}

		indexOffset += m.vertices.size();
	}

	return true;
}

void ModelLoader::processNode(aiNode *node, const aiScene *scene, QVector<Mesh> &meshes)
{
	// Обработка мешей текущего узла
	for(unsigned int i = 0; i < node->mNumMeshes; i++)
	{
		aiMesh *aiMesh = scene->mMeshes[node->mMeshes[i]];
		Mesh mesh = processMesh(aiMesh, scene);
		mesh.name = QString::fromStdString(node->mName.data);
		meshes.append(mesh);
	}

	// Рекурсивная обработка дочерних узлов
	for(unsigned int i = 0; i < node->mNumChildren; i++)
	{
		processNode(node->mChildren[i], scene, meshes);
	}
}

Mesh ModelLoader::processMesh(aiMesh *aiMesh, const aiScene *scene)
{
	Mesh mesh;

	// Обработка вершин
	for(unsigned int i = 0; i < aiMesh->mNumVertices; i++)
	{
		Vertex vertex;

		// Позиция
		vertex.position.setX(aiMesh->mVertices[i].x);
		vertex.position.setY(aiMesh->mVertices[i].y);
		vertex.position.setZ(aiMesh->mVertices[i].z);

		// Нормали
		if(aiMesh->mNormals)
		{
			vertex.normal.setX(aiMesh->mNormals[i].x);
			vertex.normal.setY(aiMesh->mNormals[i].y);
			vertex.normal.setZ(aiMesh->mNormals[i].z);
		}

		// Текстуры (если есть)
		if(aiMesh->mTextureCoords[0])
		{
			vertex.texCoord.setX(aiMesh->mTextureCoords[0][i].x);
			vertex.texCoord.setY(aiMesh->mTextureCoords[0][i].y);
		}

		mesh.vertices.append(vertex);
	}

	// Обработка индексов
	for(unsigned int i = 0; i < aiMesh->mNumFaces; i++)
	{
		aiFace face = aiMesh->mFaces[i];
		//std::cout << face.mNumIndices << std::endl;
		//std::cout << face.mIndices[0] << " : " <<face.mIndices[1] << " : " << face.mIndices[2] << std::endl;

		if(face.mNumIndices == 3)
			for (unsigned int j = 0; j < face.mNumIndices; j++)
			{
				mesh.indices.append(face.mIndices[j]);
			}
		else if (face.mNumIndices == 4)
			{
			mesh.indices.append(face.mIndices[0]);
			mesh.indices.append(face.mIndices[1]);
			mesh.indices.append(face.mIndices[2]);

			mesh.indices.append(face.mIndices[0]);
			mesh.indices.append(face.mIndices[2]);
			mesh.indices.append(face.mIndices[3]);
			}
	}

	return mesh;
}

QString ModelLoader::getErrorString()
{
	return m_lastError;
}

QStringList ModelLoader::getSupportedFormats()
{
	return {
			"STL (*.stl *.STL)",
			"OBJ (*.obj *.OBJ)",
			"FBX (*.fbx *.FBX)",
			"glTF (*.gltf *.glb *.GLTF *.GLB)",
			"COLLADA (*.dae *.DAE)",
			"3DS (*.3ds *.3DS)",
			"PLY (*.ply *.PLY)",
			"X3D (*.x3d *.X3D)"
	};
}