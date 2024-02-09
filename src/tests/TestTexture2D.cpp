#include "TestTexture2D.h"

#include <Renderer.h>
#include "imgui/imgui.h"

namespace test
{
	TestTexture2D::TestTexture2D()
	{
	}
	TestTexture2D::~TestTexture2D()
	{
	}
	void TestTexture2D::OnUpdate(float deltaTime)
	{
	}
	void TestTexture2D::OnRender()
	{
		GlCall(glClearColor(0.0f, 0.0f, 0.0f, 0.0f));
		
	}
	void TestTexture2D::OnImGuiRender()
	{
		
	}
}