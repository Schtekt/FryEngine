#pragma once
#include <cinttypes>
#include <string>

class ImageLoader
{
public:
	ImageLoader(const std::string& path);
	~ImageLoader();
	void LoadImage(const std::string& path);
	unsigned char* GetImage();
	int GetWidth() const;
	int GetHeight() const;
private:
	int m_width;
	int m_height;
	int m_channels;
	unsigned char* m_pPixels;
};