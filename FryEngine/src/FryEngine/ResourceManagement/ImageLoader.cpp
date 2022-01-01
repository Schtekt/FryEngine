#include "ImageLoader.h"
#include <iostream>
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

ImageLoader::ImageLoader(const std::string& path) : m_pPixels(nullptr), m_width(0), m_height(0)
{
	LoadImage(path);
}

ImageLoader::~ImageLoader()
{
	delete m_pPixels;
	m_pPixels = nullptr;
}

void ImageLoader::LoadImage(const std::string& path)
{
	delete m_pPixels;
	m_pPixels = nullptr;

	m_pPixels = stbi_load(path.c_str(), &m_width, &m_height, &m_channels, 0);

	if (m_pPixels == NULL)
	{
		std::cout << "ERROR: When loading the image, the program found an error, is the path " << path << " correct?" << std::endl;
	}
}

unsigned char* ImageLoader::GetImage()
{
	return m_pPixels;
}

int ImageLoader::GetWidth() const
{
	return m_width;
}

int ImageLoader::GetHeight() const
{
	return m_height;
}