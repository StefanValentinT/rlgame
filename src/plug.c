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
	Texture2D texture;
	Model model;
	bool locked;
} Plug;

typedef void (*plug_init_fun)(Plug *plug, bool release);
typedef void (*plug_update_fun)(Plug *plug);
typedef bool (*plug_should_exit_fun)(Plug *plug);
typedef bool (*plug_should_reload_fun)(Plug *plug);
typedef void (*plug_on_hot_reload_fun)(Plug *plug);
typedef void (*plug_exit_fun)(Plug *plug);

#endif
#if __INCLUDE_LEVEL__ == 0

#include "rl/raylib.h"
#include "rl/rcamera.h"
#include <stdio.h>

#include "camera.c"
#include "resources.c"

Color sky_color = (Color){60, 150, 235, 255};

void plug_init(Plug *plug, bool release)
{
	plug->pos = (Vector3){0.0f, 0.0f, 0.0f};

	const int screenWidth = 800;
	const int screenHeight = 450;

	InitWindow(
	    screenWidth, screenHeight, "raylib [models] example - loading gltf"
	);

	Camera camera = {0};
	camera.position = (Vector3){6.0f, 6.0f, 6.0f};
	camera.target = (Vector3){0.0f, 2.0f, 0.0f};
	camera.up = (Vector3){0.0f, 1.0f, 0.0f};
	camera.fovy = 45.0f;
	camera.projection = CAMERA_PERSPECTIVE;

	plug->camera = camera;

	plug->texture = load_texture();

	Model model = LoadModelFromMesh(GenMeshCube(2.0f, 2.0f, 2.0f));
	model.materials[0].maps[MATERIAL_MAP_DIFFUSE].texture = plug->texture;

	plug->model = model;

	SetTargetFPS(60);

	DisableCursor();
	plug->locked = true;

	SetExitKey(0);
}

void plug_on_hot_reload(Plug *plug)
{
	plug->texture = load_texture();

	Model model = LoadModelFromMesh(GenMeshCube(2.0f, 2.0f, 2.0f));
	model.materials[0].maps[MATERIAL_MAP_DIFFUSE].texture = plug->texture;

	plug->model = model;
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
	bool just_locked = false;
	if (IsKeyPressed(KEY_ESCAPE) && plug->locked == true)
	{
		unlock_cursor(plug);
	}
	if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON) && plug->locked == false)
	{
		lock_cursor(plug);
		just_locked = true;
	}

	Vector2 mouse_position_delta;
	if (plug->locked && !just_locked)
	{
		mouse_position_delta = GetMouseDelta();
	}
	else
	{
		mouse_position_delta = (Vector2){0.0f, 0.0f};
	}
	update_camera(&plug->camera, plug->locked, mouse_position_delta);
}

void plug_update(Plug *plug)
{
	update_controls(plug);

	BeginDrawing();

	ClearBackground(sky_color);

	BeginMode3D(plug->camera);

	// DrawCube(plug->pos, 2.0f, 2.0f, 2.0f, RED);
	DrawCubeWires(plug->pos, 2.01f, 2.01f, 2.01f, BLACK);
	DrawModel(plug->model, (Vector3){0.0f, 0.0f, 0.0f}, 1.0, WHITE);

	// DrawTexture(plug->texture, 3.0f, 3.0f, WHITE);

	DrawGrid(10, 1.0f);

	EndMode3D();

	DrawRectangle(0, 0, 90, 35, RAYWHITE);
	DrawTexture(plug->texture, 50.0f, 50.0f, WHITE);
	DrawFPS(10, 10);

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
