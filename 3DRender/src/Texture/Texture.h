#pragma once
class Texture
{
public:
	Texture(const char* filename, bool isAlpha = true);
	void Bind(const unsigned int& offset);

private:
	unsigned int m_texture;
	unsigned char* m_data;
	int m_width;
	int m_height;
	int m_nrChannels;
};