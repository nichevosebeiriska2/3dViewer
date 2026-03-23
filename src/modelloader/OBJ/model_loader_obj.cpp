#include "model_loader_obj.h"
#include "common.h"
#include <filesystem>

QString ModelLoaderOBJ::m_lastError = "";

ObjectWrapperOBJ* ModelLoaderOBJ::load(const QString &filePath)
{
	Assimp::Importer importer;

	unsigned int flags = aiProcess_Triangulate
		| aiProcess_GenNormals
		| aiProcess_FlipUVs
		| aiProcess_CalcTangentSpace
		| aiProcess_RemoveRedundantMaterials
		| aiProcess_OptimizeMeshes
		| aiProcess_SortByPType;

	const aiScene *scene = importer.ReadFile(filePath.toStdString(), flags);
	std::vector<Material> vec_materials = ProcessMaterials(filePath, scene);

	QVector<Mesh> vec_meshes;

	if(!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
	{
		m_lastError = QString::fromStdString(importer.GetErrorString());
		qWarning() << "Assimp error:" << m_lastError;
	}

	vec_meshes.clear();
	processNode(scene->mRootNode, scene, vec_meshes);

	auto *p_obj = new ObjectWrapperOBJ(vec_meshes, std::move(vec_materials));

	QVector3D vec_mid_point{0,0,0};
	QVector3D vec_sizes{0,0,0};

	auto [min, max] = CalcMinMaxPointsForMeshes(vec_meshes);
	p_obj->SetSizes((max - min));
	p_obj->SetMiddlePoint((max + min) * 0.5);

	return p_obj;
}


std::unique_ptr<QOpenGLTexture> ModelLoaderOBJ::CreateTexture()
{
	return nullptr;
}


std::vector<Material> ModelLoaderOBJ::ProcessMaterials(const QString &filePath, const aiScene *scene)
{
	std::vector<Material> materials;

	for(unsigned int i = 0; i < scene->mNumMaterials; ++i)
	{
		aiMaterial *mat = scene->mMaterials[i];
		Material matData;

		std::pair<aiTextureType, QString> textureTypes[] = {
				{aiTextureType_DIFFUSE, "diffuse"},
				{aiTextureType_SPECULAR, "specular"},
				{aiTextureType_NORMALS, "normal"},
				{aiTextureType_HEIGHT, "bump"},
				{aiTextureType_AMBIENT, "ambient"},
				{aiTextureType_EMISSIVE, "emissive"},
				{aiTextureType_LIGHTMAP, "lightmap"},
				{aiTextureType_OPACITY, "opacity"},
		};
		aiString strTexture;

		for(auto [type, str_type] : textureTypes)
		{
			if(AI_SUCCESS == mat->GetTexture(type, 0, &strTexture))
			{
				if(!strTexture.Empty())
				{
					int a = 1;
					std::filesystem::path path_to_image(strTexture.C_Str());
					std::filesystem::path path_to_dir = filePath.toStdString();
					path_to_dir = path_to_dir.parent_path();
					path_to_image = path_to_dir / path_to_image;

					QImage image(path_to_image.generic_string().c_str());
					auto ptr_texture = new QOpenGLTexture(image);
					
					switch(type)
					{
						case(aiTextureType_AMBIENT):
						{
							matData.m_texture_ambient.reset(ptr_texture);
							break;
						}

						case(aiTextureType_DIFFUSE):
						{
							matData.m_texture_diffuse.reset(ptr_texture);
							break;
						}

						case(aiTextureType_SPECULAR):
						{
							matData.m_texture_specular.reset(ptr_texture);
							break;
						}

						default:
							break;
					}
					
				}
			}
		}

		aiColor4D color_coef(0.f, 0.f, 0.f, 0.f);

		if(AI_SUCCESS == aiGetMaterialColor(mat, AI_MATKEY_COLOR_AMBIENT, &color_coef))
			matData.ambientColor = QVector3D(color_coef.r, color_coef.g, color_coef.b);
		else
			matData.ambientColor = QVector3D(1.0, 1.0, 1.0);

		if(AI_SUCCESS == aiGetMaterialColor(mat, AI_MATKEY_COLOR_DIFFUSE, &color_coef))
			matData.diffuseColor = QVector3D(color_coef.r, color_coef.g, color_coef.b);
		else
			matData.diffuseColor = QVector3D(1.0, 1.0, 1.0); // По умолчанию белый

		if(AI_SUCCESS == aiGetMaterialColor(mat, AI_MATKEY_COLOR_SPECULAR, &color_coef))
			matData.specularColor = QVector3D(color_coef.r, color_coef.g, color_coef.b);
		else
			matData.specularColor = QVector3D(1.0, 1.0, 1.0);


		float shininess = 0.f;
		if(AI_SUCCESS == aiGetMaterialFloat(mat, AI_MATKEY_SHININESS, &shininess))
			matData.shininess = shininess;
		else
			matData.shininess = 32.0f;

		materials.emplace_back(std::move(matData));
	}

	return materials;
}


void ModelLoaderOBJ::processNode(aiNode *node, const aiScene *scene, QVector<Mesh> &meshes)
{
	// Обработка мешей текущего узла
	for(unsigned int i = 0; i < node->mNumMeshes; i++)
	{
		aiMesh *aiMesh = scene->mMeshes[node->mMeshes[i]];
		Mesh mesh = processMesh(aiMesh, scene);
		mesh.name = QString::fromStdString(node->mName.data);
		mesh.m_iMaterialIndex = aiMesh->mMaterialIndex;

		meshes.append(mesh);
	}

	for(unsigned int i = 0; i < node->mNumChildren; i++)
		processNode(node->mChildren[i], scene, meshes);

}


Mesh ModelLoaderOBJ::processMesh(aiMesh *aiMesh, const aiScene *scene)
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

	auto [min, max] = CalcMinMaxPoints(mesh);

	mesh.m_sizes = max - min;
	mesh.m_middle_point = (max + min) * 0.5;

	// Обработка индексов
	for(unsigned int i = 0; i < aiMesh->mNumFaces; i++)
	{
		aiFace face = aiMesh->mFaces[i];

		if(face.mNumIndices == 3)
			for(unsigned int j = 0; j < face.mNumIndices; j++)
				mesh.indices.append(face.mIndices[j]);
	}

	return mesh;
}