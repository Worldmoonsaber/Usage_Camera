#pragma once
#include "ICamera.h"


class FakeCamera :public ICamera
{

public:
	FakeCamera();
	~FakeCamera();

	void Initialize();
};