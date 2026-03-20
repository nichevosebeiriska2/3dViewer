#include "global.h"


std::filesystem::path Global::m_root_path = std::filesystem::current_path();

void Global::SetRootDirectory(std::filesystem::path root)
{
	m_root_path = root;
}


std::filesystem::path Global::GetRootDirectory()
{
	return m_root_path;
}


std::filesystem::path Global::GetShadersDirectory()
{
	return m_root_path / "../../../src/shaders/";
}

QVector<QString> Global::GetEnabledFileFormats()
{
	return {"stl"};
}