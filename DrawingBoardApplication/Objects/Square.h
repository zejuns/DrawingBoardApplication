#pragma once
#include "../DrawObject.h"


class Square : public DrawObject
{
public:
	Square();


	virtual void RefreshPos() override;

	virtual void CreateRenderData() override;

	virtual void OnRender(float InDeltaTime)override;
};

