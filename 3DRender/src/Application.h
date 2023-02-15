#pragma once
#include "Device/Device.h"
#include "Window/Window.h"
#include "Shader/Shader.h"
#include "Shader/ShaderProgram.h"

#include <string>

class Application
{
public:
	Application(int width, int height, const char* title);
	virtual ~Application();
	int Run();

protected:
	inline const Device& GetDevice() const { return m_device; }
	inline Device& GetDevice() { return m_device; }
	inline Window& GetMainWindow() { return m_mainWindow; }
	inline const Window& GetMainWindow() const { return m_mainWindow; }
	inline float GetCurrentTime() const { return m_currentTime; }
	inline float GetDeltaTime() const { return m_deltaTime; }
	inline void Close() { Terminate("Close the window"); }

protected:
	virtual void Initialize();
	virtual void InitializeGeometry();
	virtual void InitializeShaders();
	virtual void Update();
	virtual void Render();
	virtual void Clearup();
	bool IsRunning();
	void LoadAndCompileShader(Shader& shader, const char* path);


protected:
	void Terminate(const char* errorMsg);

private:
	Device m_device;
	Window m_mainWindow;


	float m_currentTime;
	float m_deltaTime;

	std::string m_errorMsg;

private:
	void UpdateTime(float newCurrentTime);

};

