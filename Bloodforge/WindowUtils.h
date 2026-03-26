#pragma once
#include <string>
namespace Bloodforge
{
	class CustomCursor;
}

namespace Bloodforge::WindowUtils
{
	void SetWindowSize(int sizeX, int sizeY);
	void SetWindowAlwaysOnTop(bool alwaysOnTop);
	void SetWindowBordered(bool borderless);
	void SetWindowFullScreen(bool fullscreen);
	void SetCustomCursor(CustomCursor* customCursor);
	void SetWindowIcon(const std::string& path);
}