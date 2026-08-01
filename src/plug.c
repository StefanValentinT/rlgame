#ifndef PLUG_H
#define PLUG_H

#include "rl/raylib.h"
#include <stdbool.h>
#include <stddef.h>

typedef struct
{
	size_t version;
	Vector3 pos;
	Camera camera;
	bool locked;
} Plug;

typedef struct
{
	bool debug;
} Args;

typedef void (*plug_init_fun)(Plug *plug, Args *args);
typedef void (*plug_update_fun)(Plug *plug);
typedef bool (*plug_should_exit_fun)(Plug *plug);
typedef bool (*plug_should_reload_fun)(Plug *plug);
typedef void (*plug_exit_fun)(Plug *plug);

#endif
#if __INCLUDE_LEVEL__ == 0

#include "rl/raylib.h"
#include <stdio.h>

Color sky_color = (Color){60, 150, 235, 255};

void plug_init(Plug *plug, Args *args)
{
	plug->pos = (Vector3){0.0f, 0.0f, 0.0f};

	const int screenWidth = 800;
	const int screenHeight = 450;

	InitWindow(screenWidth, screenHeight,
	           "raylib [models] example - loading gltf");

	Camera camera = {0};
	camera.position = (Vector3){6.0f, 6.0f, 6.0f};
	camera.target = (Vector3){0.0f, 2.0f, 0.0f};
	camera.up = (Vector3){0.0f, 1.0f, 0.0f};
	camera.fovy = 45.0f;
	camera.projection = CAMERA_PERSPECTIVE;

	plug->camera = camera;

	Model model = LoadModel("res/goated.gltf");
	Vector3 position = {0.0f, 0.0f, 0.0f};

	int animCount = 0;
	ModelAnimation *anims = LoadModelAnimations("res/goated.gltf", &animCount);

	if (args)
		printf("%i \n", animCount);

	unsigned int animIndex = 0;
	unsigned int animCurrentFrame = 0;

	SetTargetFPS(60);

	DisableCursor();
	plug->locked = true;

	SetExitKey(0);
}

void lock_cursor(Plug *plug)
{
	plug->locked = true;
	DisableCursor();
}

void unlock_cursor(Plug *plug)
{
	plug->locked = false;
	EnableCursor();
}

void plug_update(Plug *plug)
{
	if (plug->locked)
	{
		UpdateCamera(&plug->camera, CAMERA_FIRST_PERSON);
	}

	if (IsKeyPressed(KEY_ESCAPE) && plug->locked == true)
		unlock_cursor(plug);
	if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON) && plug->locked == false)
		lock_cursor(plug);

	BeginDrawing();

	ClearBackground(sky_color);

	BeginMode3D(plug->camera);

	DrawCube(plug->pos, 2.0f, 2.0f, 2.0f, RED);
	DrawGrid(10, 1.0f);

	EndMode3D();

	EndDrawing();
}

bool plug_should_exit(Plug *plug) { return WindowShouldClose(); }

bool plug_should_reload(Plug *plug) { return IsKeyPressed(KEY_H); }

void plug_exit(Plug *plug) { CloseWindow(); }

#endif
