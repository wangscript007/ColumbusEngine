#include <Core/Platform/Platform.h>
#include <Core/Platform/PlatformFilesystem.h>
#include <cstdio>

#if defined(PLATFORM_WINDOWS)
	#include <Core/Windows/PlatformWindowsFilesystem.h>
#elif defined(PLATFORM_LINUX)
	#include <Core/Linux/PlatformLinuxFilesystem.h>
#endif

namespace Columbus
{

	const char* Filesystem::GetCurrent()
	{
		#if defined(PLATFORM_WINDOWS)
			return FilesystemWindows::GetCurrent();
		#elif defined(PLATFORM_LINUX)
			return FilesystemLinux::GetCurrent();
		#endif

		return "";
	}

	const char* Filesystem::AbsolutePath(const char* Path)
	{
		#if defined(PLATFORM_WINDOWS)
			return FilesystemWindows::AbsolutePath(Path);
		#elif defined(PLATFORM_LINUX)
			return FilesystemLinux::AbsolutePath(Path);
		#endif

		return "";
	}

	bool Filesystem::FileCreate(const char* Path)
	{
		FILE* File = fopen(Path, "w");
		bool Result = File != nullptr;
		if (File != nullptr) fclose(File);
		return Result;
	}

	bool Filesystem::DirCreate(const char* Path)
	{
		#if defined(PLATFORM_WINDOWS)
			return FilesystemWindows::DirCreate(Path);
		#elif defined(PLATFORM_LINUX)
			return FilesystemLinux::DirCreate(Path);
		#endif

		return false;
	}

	bool Filesystem::Rename(const char* Old, const char* New)
	{
		return rename(Old, New) == 0;
	}

	bool Filesystem::FileRemove(const char* Path)
	{
		return remove(Path) == 0;
	}

	bool Filesystem::DirRemove(const char* Path)
	{
		#if defined(PLATFORM_WINDOWS)
			return FilesystemWindows::DirRemove(Path);
		#elif defined(PLATFORM_LINUX)
			return FilesystemLinux::DirRemove(Path);
		#endif

		return false;
	}

	std::vector<FileInfo> Filesystem::Read(const std::string& Path)
	{
		#if defined(PLATFORM_WINDOWS)
			return FilesystemWindows::Read(Path);
		#elif defined(PLATFORM_LINUX)
			return FilesystemLinux::Read(Path);
		#endif
		
		return {};
	}

}


