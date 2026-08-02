#ifndef CAMERA_H
#define CAMERA_H

#include "rl/raylib.h"

void update_camera(Camera *cam, bool locked, Vector2 mouse_delta);

#endif
#if __INCLUDE_LEVEL__ == 0

#include "rl/raylib.h"
#include "rl/rcamera.h"

float camera_move_speed = 0.1f;
float camera_mouse_move_sensitivity = 0.003f;

void update_camera(Camera *camera, bool locked, Vector2 mouse_delta)
{
	bool moveInWorldPlane = true;
	if (IsKeyDown(KEY_W))
		CameraMoveForward(camera, camera_move_speed, moveInWorldPlane);
	if (IsKeyDown(KEY_A))
		CameraMoveRight(camera, -camera_move_speed, moveInWorldPlane);
	if (IsKeyDown(KEY_S))
		CameraMoveForward(camera, -camera_move_speed, moveInWorldPlane);
	if (IsKeyDown(KEY_D))
		CameraMoveRight(camera, camera_move_speed, moveInWorldPlane);
	if (IsKeyDown(KEY_LEFT_SHIFT))
		CameraMoveUp(camera, -camera_move_speed);
	if (IsKeyDown(KEY_SPACE))
		CameraMoveUp(camera, camera_move_speed);

	if (locked)
	{
		CameraYaw(
		    camera, -mouse_delta.x * camera_mouse_move_sensitivity, false
		);
		CameraPitch(
		    camera, -mouse_delta.y * camera_mouse_move_sensitivity, false,
		    false, false
		);
	}
}

#endif
