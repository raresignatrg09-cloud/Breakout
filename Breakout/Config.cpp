#pragma once

namespace WinConfig
{
	constexpr unsigned int WINDOW_WIDTH = 600;
	constexpr unsigned int WINDOW_HEIGHT = 800;
	constexpr char WINDOW_TITLE[] = "Breakout";
}

namespace PaddleConfig
{
	constexpr float PADDLE_WIDTH = 100.f;
	constexpr float PADDLE_HEIGHT = 20.f;
	
	constexpr float PADDLE_SPEED = 400.f;
}

namespace BallConfig
{
	constexpr float radius = 10.f;

	constexpr float speed = 300.f;
}

namespace BrickConfig
{
	constexpr float BRICK_WIDTH = 50.f;
	constexpr float BRICK_HEIGHT = 20.f;
}