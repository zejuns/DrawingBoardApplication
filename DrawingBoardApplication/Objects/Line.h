#pragma once
#include "../DrawObject.h"


class Line :public DrawObject
{
public:
	Line();
	virtual void CreateRenderData() override;
	virtual void OnRender(float InDeltaTime) override;
};

