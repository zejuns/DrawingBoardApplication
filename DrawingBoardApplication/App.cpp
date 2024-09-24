#include "App.h"
#include "DrawObject.h"
#include "Objects/Line.h"
#include "Shader.h"
#include "Objects/Square.h"
#include "Objects/Circle.h"
#include "Texture.h"

#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image_write.h"
#include "ObjModel.h"

void MouseButtonCB(GLFWwindow* window, int button, int action, int mods)
{
	app->MouseButtonCallback(window, button, action, mods);
}
// 鼠标移动回调函数
void CursorPosCB(GLFWwindow* window, double xpos, double ypos)
{
	app->CursorPosCallback(window, xpos, ypos);
}

App::App()
{
}

void App::Init()
{
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);            // Required on Mac

	// Create window with graphics context
	window = glfwCreateWindow(viewPortWidth, viewPortHeight, "Render App", nullptr, nullptr);
	if (window == nullptr)
	{
		std::cout << "failed to init glfw window" << std::endl;
		exit(-1);
	}
	glfwMakeContextCurrent(window);
	glfwSwapInterval(1); // Enable vsync

	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialize GLAD" << std::endl;
		exit(-1);
	}

	// 注册鼠标按键回调函数
	glfwSetMouseButtonCallback(window, MouseButtonCB);
	// 注册鼠标移动回调函数
	glfwSetCursorPosCallback(window, CursorPosCB);

	const char* glsl_version = "#version 150";
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO(); (void)io;
	io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
	io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;
	ImGui::StyleColorsDark();
	ImGui_ImplGlfw_InitForOpenGL(window, true);
	ImGui_ImplOpenGL3_Init(glsl_version);

	targetTexture = new Texture();

	objModels.emplace_back(new ObjModel("obj1", "Assets/1.obj"));
	objModels.emplace_back(new ObjModel("obj2", "Assets/2.obj"));
	objModels.emplace_back(new ObjModel("obj3", "Assets/3.obj"));
	objModels.emplace_back(new ObjModel("obj4", "Assets/4.obj"));
	objModels.emplace_back(new ObjModel("obj5", "Assets/5.obj"));
	objModels.emplace_back(new ObjModel("obj6", "Assets/6.obj"));
	objModels.emplace_back(new ObjModel("obj7", "Assets/7.obj"));
	objModels.emplace_back(new ObjModel("obj8", "Assets/8.obj"));
	objModels.emplace_back(new ObjModel("obj9", "Assets/9.obj"));
	objIndex = 0;

	/*Line* line = new Line();
	line->startPos = glm::vec3(drawViewPos.x, drawViewPos.y, 0.0);
	line->endPos = glm::vec3(drawViewPos.x + 500.0, drawViewPos.y + 500.0, 0.0);

	line->color = glm::vec4(0.0, 1.0, 0.0, 1.0);
	line->lineWidth = 5.0;

	objects.emplace_back(line);*/
}

int App::Run()
{
	std::chrono::duration<double> target_frame_duration(1.0 / targeFPS);

	while (!glfwWindowShouldClose(window))
	{
		auto frame_start_time = std::chrono::steady_clock::now();

		ProcessInput();

		OnRender((float)target_frame_duration.count());

		glfwSwapBuffers(window);
		glfwPollEvents();

		auto frame_end_time = std::chrono::steady_clock::now();
		auto frame_duration = frame_end_time - frame_start_time;
		auto sleep_duration = target_frame_duration - frame_duration;

		if (sleep_duration > std::chrono::duration<double>(0)) {
			std::this_thread::sleep_for(sleep_duration);
		}
	};

	glfwTerminate();
	return 0;
}

void App::ProcessInput()
{
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
	{
		glfwSetWindowShouldClose(window, true);
	}
}

void App::OnRender(float InDeltaTime)
{
	deltaTime = InDeltaTime;

	RenderScene(InDeltaTime);

	RenderGui(InDeltaTime);
}

void App::RenderScene(float InDeltaTime)
{
	glm::mat4 projection = glm::mat4(1.0);
	glm::mat4 view = glm::mat4(1.0);
	if (bShowModel)
	{
		glClearColor(.30f, .30f, .30f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glEnable(GL_DEPTH_TEST);
		projection = glm::perspective(glm::radians(45.0f), static_cast<float>(viewPortWidth) / static_cast<float>(viewPortHeight), 0.001f, 1000.0f);
		view = glm::lookAt(glm::vec3(0.0, 0.0, -50.0), glm::vec3(0.0), glm::vec3(0.0, 1.0, 0.0));

		objModels[objIndex]->objSize = objSize;
		objModels[objIndex]->objRotation = objRotation;
		objModels[objIndex]->shader->Activity();
		objModels[objIndex]->shader->Activity();
		objModels[objIndex]->shader->SetMat4("projection", projection);
		objModels[objIndex]->shader->SetMat4("view", view);
		objModels[objIndex]->shader->SetVec2("uvOffset", uvOffset);

		targetTexture->Bind();
		objModels[objIndex]->OnRender(InDeltaTime);
		targetTexture->UnBind();

		glDisable(GL_DEPTH_TEST);
	}
	else
	{
		glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);
		glDisable(GL_DEPTH_TEST);
		projection = glm::ortho(0.0f, static_cast<float>(viewPortWidth), static_cast<float>(viewPortHeight), 0.0f, -1.0f, 1.0f);

		for (int i = drawObjects.size() - 1; i >= 0; --i)
		{
			auto& obj = drawObjects[i];
			if (!obj)continue;
			if (obj->shader)
			{
				obj->shader->Activity();
				obj->shader->SetMat4("projection", projection);
				obj->shader->SetMat4("view", view);
			}
			obj->OnRender(InDeltaTime);
		}
	}
}

void App::RenderGui(float InDeltaTime)
{
	ImGui_ImplOpenGL3_NewFrame();
	ImGui_ImplGlfw_NewFrame();
	ImGui::NewFrame();
	SetCustomImgui(InDeltaTime);
	ImGui::Render();
	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}

void App::SetCustomImgui(float InDeltaTime)
{
	static bool no_titlebar = false;
	static bool no_scrollbar = false;
	static bool no_menu = true;
	static bool no_move = true;
	static bool no_resize = true;
	static bool no_collapse = true;
	static bool no_close = false;
	static bool no_nav = true;
	static bool no_background = false;
	static bool no_bring_to_front = false;
	static bool unsaved_document = false;

	ImGuiWindowFlags window_flags = 0;
	if (no_titlebar)        window_flags |= ImGuiWindowFlags_NoTitleBar;
	if (no_scrollbar)       window_flags |= ImGuiWindowFlags_NoScrollbar;
	if (!no_menu)           window_flags |= ImGuiWindowFlags_MenuBar;
	if (no_move)            window_flags |= ImGuiWindowFlags_NoMove;
	if (no_resize)          window_flags |= ImGuiWindowFlags_NoResize;
	if (no_collapse)        window_flags |= ImGuiWindowFlags_NoCollapse;
	if (no_nav)             window_flags |= ImGuiWindowFlags_NoNav;
	if (no_background)      window_flags |= ImGuiWindowFlags_NoBackground;
	if (no_bring_to_front)  window_flags |= ImGuiWindowFlags_NoBringToFrontOnFocus;
	if (unsaved_document)   window_flags |= ImGuiWindowFlags_UnsavedDocument;

	auto SetSizePos = [](glm::vec2 InPos, glm::vec2 InSize)
		{
			ImGui::SetWindowPos(ImVec2(InPos.x, InPos.y));
			ImGui::SetWindowSize(ImVec2(InSize.x, InSize.y));
		};

	if (showDrawOptions)
	{
		ImGui::Begin("Draw Options", nullptr, window_flags);
		SetSizePos(drawOptPos, drawOptSize);
		ImGui::Text("FrameTime:%f", InDeltaTime);
		ImGui::Text("FPS	  :%d", targeFPS);

		ImGui::ColorEdit4("Color", glm::value_ptr(drawColor));

		const char** strShapeArray = vectorToStringArray(drawitems);
		ImGui::Combo("Shape", &drawType, strShapeArray, static_cast<int>(drawitems.size()));

		ImGui::Checkbox("Fill", &bFill);
		if (ImGui::Button("Switch Draw", ImVec2(drawOptSize.x, 25)))
		{
			ReadDrawBoard();
			// 切换显示
			bShowModel = !bShowModel;
		}

		if (ImGui::Button("Save Texture", ImVec2(drawOptSize.x, 25)))
		{
			SaveTextureFile();
		}

		std::vector<std::string>objStr;

		for (auto& obj : objModels)
			objStr.emplace_back(obj->objName);

		const char** strModelArray = vectorToStringArray(objStr);

		ImGui::Combo("Render Model", &objIndex, strModelArray, static_cast<int>(objStr.size()));

		ImGui::SliderFloat2("UV Offset", glm::value_ptr(uvOffset),0.0,1.0);

		ImGui::SliderFloat3("Obj Size", glm::value_ptr(objSize),1.0,100.0);
		ImGui::SliderFloat3("Obj Rotation", glm::value_ptr(objRotation),0.0,360);

		ImGui::End();
	}

	if (bShowModel)
	{
		// 显示绘制模型
		for (int i = drawObjects.size() - 1; i >= 0; i--)
		{
			delete drawObjects[i];
		}
		this->drawObjects.clear();
	}
	else
	{
		{
			ImGui::Begin("Objs Options", nullptr, window_flags);
			SetSizePos(objOptPos, objOptSize);

			int id = 100;
			ImGui::GetWindowDrawList();
			for (auto& i : drawObjects)
			{
				if (i)
				{
					ImGui::PushID(id);
					if (ImGui::TreeNode(i->objName.c_str()))
					{
						ImGui::SliderFloat("Line Width", &i->lineWidth, 1, 10);
						ImGui::ColorEdit4("Obj Color", glm::value_ptr(i->color));

						ImGui::InputFloat3("StartPosition", glm::value_ptr(i->startPos), "%.2f");
						ImGui::InputFloat3("EndPosition", glm::value_ptr(i->endPos), "%.2f");

						ImGui::Checkbox("Fill", &i->bFillRender);

						ImGui::TreePop();
					};
					ImGui::PopID();
					id++;
				}
			}

			ImGui::End();
		}
	}
}

void App::MouseButtonCallback(GLFWwindow* window, int button, int action, int mods)
{
	if (button == GLFW_MOUSE_BUTTON_LEFT)
	{
		if (action == GLFW_PRESS)
		{
			currentObject = nullptr;

			if (ImGui::GetIO().WantCaptureMouse || bShowModel)
			{
				// 如果ImGui正在捕获鼠标输入 或者处于显示模型，则不处理
				return;
			}

			std::cout << "Left mouse button pressed" << std::endl;
			// 添加obj

			std::cout << "Mouse position: (" << mousePos.x << ", " << mousePos.y << ")" << std::endl;
			switch (drawType)
			{
			case 0:
			{
				currentObject = new Line();
			}
			break;
			case 1:
			{
				currentObject = new Square();
			}
			break;
			case 2:
			{
				currentObject = new Circle();
			}
			break;
			default:
				break;
			}
			if (currentObject)
			{
				currentObject->startPos = glm::vec3(mousePos.x, mousePos.y, 0.0);
				currentObject->endPos = currentObject->startPos;
				currentObject->color = drawColor;
				currentObject->lineWidth = drawlineWidth;
				currentObject->bFillRender = bFill;

				currentObject->CreateRenderData();
				drawObjects.emplace_back(currentObject);
			}
		}
		else if (action == GLFW_RELEASE)
		{
			glm::vec3 endPos = glm::vec3(mousePos.x, mousePos.y, 0.0);
			endPos.x = glm::clamp(endPos.x, drawViewPos.x, drawViewPos.x + drawViewSize.x);
			endPos.y = glm::clamp(endPos.y, drawViewPos.y, drawViewPos.y + drawViewSize.y);

			std::cout << "Left mouse button released" << std::endl;
			std::cout << "Mouse position: (" << mousePos.x << ", " << mousePos.y << ")" << std::endl;

			if (currentObject)
			{
				currentObject->endPos = endPos;
			}

			currentObject = nullptr;
		}
	}
}

void App::CursorPosCallback(GLFWwindow* window, double xpos, double ypos)
{
	mousePos = glm::vec2(xpos, ypos);

	if (currentObject)
	{
		currentObject->endPos = glm::vec3(mousePos.x, mousePos.y, 0.0);
	}
}

void App::ReadDrawBoard()
{
	glm::vec2 startPos = drawViewPos;
	glm::vec2 endPos = glm::vec2(viewPortWidth, viewPortHeight);

	int w = drawViewSize.x;
	int h = drawViewSize.y;
	// 图片宽高 rgb 三通道
	unsigned char* pixels = new unsigned char[w * h * 3];
	glReadPixels(startPos.x, startPos.y, w, h, GL_RGB, GL_UNSIGNED_BYTE, pixels);
	FlipTexture(w, h, pixels);
	targetTexture->SetTexture(pixels, w, h);
	//delete[]pixels;
}

void App::SaveTextureFile()
{
	// 读取sju

	glm::vec2 startPos = drawViewPos;
	glm::vec2 endPos = glm::vec2(viewPortWidth, viewPortHeight);

	int w = drawViewSize.x;
	int h = drawViewSize.y;

	// 图片宽高 rgb 三通道
	unsigned char* pixels = new unsigned char[w * h * 3];

	glReadPixels(startPos.x, startPos.y, w, h, GL_RGB, GL_UNSIGNED_BYTE, pixels);

	FlipTexture(w, h, pixels);

	stbi_write_png("Test.png", w, h, 3, pixels, w * 3);

	delete[]pixels;
}
