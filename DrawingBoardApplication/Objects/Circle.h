#pragma once
#include "../DrawObject.h"

class Circle : public DrawObject
{

public:
	Circle();

	const int numSegments = 36;


	virtual void RefreshPos() override;

	virtual void CreateRenderData() override;

	virtual void OnRender(float InDeltaTime)override;

};
