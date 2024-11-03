#include "CameraEx.h"

void	CameraEx::update(GameData *gd)
{
	uintptr_t	cam_ptr = gd->memory->getAddress(gd->baseAddr.get() + 0x016148F0, { 0x18, 0x480, 0x108, 0x18, 0xC0, 0x60, 0x170 });

	camera = gd->memory->read<Camera>(cam_ptr);

	windowWidth = gd->width;
	windowHeight = gd->height;

	//windowWidth = 820;
	//windowHeight = 493; // more like 599 in the engine
}

float dot(vec3 u, vec3 v)
{
	return (u.x * v.x + u.y * v.y + u.z * v.z);
}

vec3 sub(vec3 u, vec3 v)
{
	return { u.x - v.x, u.y - v.y, u.z - v.z };
}

vec3 CameraEx::WorldToScreen(vec3 worldPosition)
{
	float	fovy = camera.fovx / windowWidth * windowHeight;

	vec3	ps = sub(worldPosition, { camera.position.x, camera.position.y, camera.position.z });

	vec3	mx1({ camera.rotation.x, camera.rotation.y, camera.rotation.z });
	vec3	mx2({ camera.dunno.x, camera.dunno.y, camera.dunno.z });
	vec3	mx3({ camera.lookAt.x, camera.lookAt.y, camera.lookAt.z });

	// dot product
	vec3	Transform;
	Transform.x = dot(ps, mx1);
	Transform.y = dot(ps, mx2);
	Transform.z = dot(ps, mx3);

	// get the center of the screen
	vec2 Center;
	Center.x = (float)(windowWidth * 0.5f);
	Center.y = (float)(windowHeight * 0.5f);

	float x = Center.x * (1 - (Transform.x / -camera.fovx / Transform.z));
	float y = Center.y * (1 - (Transform.y / fovy / Transform.z));

	return { x, y, Transform.z };
}
