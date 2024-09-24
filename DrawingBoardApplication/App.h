#pragma once

#include "Core.h"

class DrawObject;

extern class App* app;
class Texture;
class ObjModel;

class App
{
public:
	static const unsigned int viewPortWidth = 1920;
	static const unsigned int viewPortHeight = 1080;
	static const unsigned int targeFPS = 60;

	GLFWwindow* window = nullptr;
	float deltaTime;

	Texture* targetTexture;

	std::vector<ObjModel* >objModels;
	int objIndex = 0;

	glm::vec2 uvOffset = glm::vec2(0.0);// UV 偏移
	glm::vec3 objSize = glm::vec3(1.0);
	glm::vec3 objRotation = glm::vec3(0.0);// 欧拉角

	// UI大小配置
	glm::vec2 drawOptSize = glm::vec2(300.0, 450.0);
	glm::vec2 drawOptPos = glm::vec2(0.0);
	glm::vec2 objOptSize = glm::vec2(drawOptSize.x, viewPortHeight - drawOptSize.y);
	glm::vec2 objOptPos = glm::vec2(0.0, drawOptSize.y);
	glm::vec2 drawViewSize = glm::vec2(viewPortWidth - drawOptSize.x, viewPortHeight);
	glm::vec2 drawViewPos = glm::vec2(drawOptSize.x, 0.0);
	// 将相对位置转换到实际的窗口位置
	ImVec2 GetReleasePos(glm::vec2 InPos)
	{
		glm::vec2 pos = drawViewPos + InPos;
		return ImVec2(pos.x, pos.y);
	};
	// 鼠标在绘制视口的位置
	glm::vec2 GetMouseInViewPortPos() { return (mousePos - drawViewPos); }
	ImVec2 GetMouseInImViewPortPos() { return ImVec2(GetMouseInViewPortPos().x, GetMouseInViewPortPos().y); }

	bool showDrawOptions = true;

	bool bShowModel = false;

	// 绘制配置
	std::vector<std::string>drawitems{ "Line","Square","Circle" };
	int drawType = 0;// 绘制哪种类型的图形
	bool bFill = true;// 是否填充图像
	glm::vec4 drawColor = glm::vec4(1.0, 0.0, 0.0, 1.0);// 绘制颜色
	float drawlineWidth = 5;// 绘制线条宽度

	glm::vec2 mousePos = glm::vec2(0.0);// 鼠标位置

	DrawObject* currentObject = nullptr;

	std::vector<DrawObject*>drawObjects;

	App();
	void Init();

	int Run();

	void ProcessInput();

	void OnRender(float InDeltaTime);
	void RenderScene(float InDeltaTime);
	void RenderGui(float InDeltaTime);
	void SetCustomImgui(float InDeltaTime);

	static const char** vectorToStringArray(const std::vector<std::string>& vec) {
		const char** strArray = new const char* [vec.size()];
		for (size_t i = 0; i < vec.size(); ++i) {
			strArray[i] = vec[i].c_str();
		}
		return strArray;
	}

	void MouseButtonCallback(GLFWwindow* window, int button, int action, int mods);

	void CursorPosCallback(GLFWwindow* window, double xpos, double ypos);

	void ReadDrawBoard();

	void SaveTextureFile();

	void FlipTexture(int width, int height, unsigned char* pixels)
	{
		unsigned char* temp = new unsigned char[width * 3];
		for (int y = 0; y < height / 2; ++y) {
			int topOffset = y * width * 3;
			int bottomOffset = (height - y - 1) * width * 3;
			// 交换行数据
			memcpy(temp, pixels + topOffset, width * 3);
			memcpy(pixels + topOffset, pixels + bottomOffset, width * 3);
			memcpy(pixels + bottomOffset, temp, width * 3);
		}
		delete[] temp;
	}
};
