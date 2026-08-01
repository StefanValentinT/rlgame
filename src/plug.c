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

typedef void (*plug_init_fun)(Plug *plug, bool release);
typedef void (*plug_update_fun)(Plug *plug);
typedef bool (*plug_should_exit_fun)(Plug *plug);
typedef bool (*plug_should_reload_fun)(Plug *plug);
typedef void (*plug_exit_fun)(Plug *plug);

#endif
#if __INCLUDE_LEVEL__ == 0

#include "rl/raylib.h"
#include "rl/rcamera.h"
#include <stdio.h>

#include "camera.c"

Color sky_color = (Color){60, 150, 235, 255};

void plug_init(Plug *plug, bool release)
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
	ModelAnimation *anims = LoadModelAnimations("res/dummy e.gltf", &animCount);

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

void update_controls(Plug *plug)
{
	Vector2 mousePositionDelta = GetMouseDelta();

	if (IsKeyPressed(KEY_ESCAPE) && plug->locked == true)
		unlock_cursor(plug);
	if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON) && plug->locked == false)
		lock_cursor(plug);

	update_camera(&plug->camera, plug->locked);
}

void plug_update(Plug *plug)
{
	update_controls(plug);

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

#ifdef RELEASE

Plug plug = {0};

int main(void)
{
	plug_init(&plug, true);

	while (!plug_should_exit(&plug))
	{
		plug_update(&plug);
	}

	plug_exit(&plug);
}

#endif

#endif
