#ifndef GLOBAL_H
#define GLOBAL_H

#include <filesystem>
#include <QVector>
#include <QString>

class Global
{
	static std::filesystem::path m_root_path;

public:
	static void SetRootDirectory(std::filesystem::path root);
	static std::filesystem::path GetRootDirectory();
	static std::filesystem::path GetShadersDirectory();
	static QVector<QString> GetEnabledFileFormats();
};

#endif // !GLOBAL_H
