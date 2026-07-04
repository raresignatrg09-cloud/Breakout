#pragma once

namespace WinConfig
{
	constexpr unsigned int WINDOW_WIDTH = 960;
	constexpr unsigned int WINDOW_HEIGHT = 720;
	constexpr char WINDOW_TITLE[] = "Breakout";
	constexpr float FIXED_DT = 1.f / 240.f;
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
	constexpr float BRICK_WIDTH = 60.f;
	constexpr float BRICK_HEIGHT = 30.f;

	constexpr float BRICK_SPACING = 4.f;
}

namespace PowerUpConfig
{
	constexpr float fallSpeed = 150.f;
	constexpr float duration = 10.f; // Duration of power-up effects in seconds

	constexpr int expandChance = 20; // Percentage chance for ExpandPaddle power-up
	constexpr int shrinkChance = 20; // Percentage chance for ShrinkPaddle power-up
	constexpr int extraLifeChance = 20; // Percentage chance for ExtraLife power-up
	constexpr int multiBallChance = 20; // Percentage chance for MultiBall power-up
	constexpr int stickyPaddleChance = 20; // Percentage chance for StickyPaddle power-up
}