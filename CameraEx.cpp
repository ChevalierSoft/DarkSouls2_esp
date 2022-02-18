#include "CameraEx.h"

void	CameraEx::update(GameData *gd)
{
	uintptr_t	cam_ptr = gd->memory->getAddress(gd->baseAddr.get() + 0x0160B8D0, { 0x18, 0x480, 0x108, 0x18, 0xC0, 0x60, 0x170 });
	//uintptr_t	win_ptr = 0x7FF4DA574970;

	camera = gd->memory->read<Camera>(cam_ptr);

	//std::cout << "lookAt : "   << camera.lookAt.x << " " << camera.lookAt.y << " " << camera.lookAt.z << std::endl;
	//std::cout << "position : " << camera.position.x << " " << camera.position.y << " " << camera.position.z << std::endl;
	//std::cout << "fovx : "     << camera.fovx << std::endl;

	windowWidth = 820;
	windowHeight = 493; // more like 599 in the engine
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

	vec3	jcp1({ camera.__rotation.x, camera.__rotation.y, camera.__rotation.z });
	vec3	jcp2({ camera.__dunnovec.x, camera.__dunnovec.y, camera.__dunnovec.z });
	vec3	jcp3({ camera.lookAt.x, camera.lookAt.y, camera.lookAt.z });

	vec3	Transform;
	Transform.x = dot(ps, jcp1);
	Transform.y = dot(ps, jcp2);
	Transform.z = dot(ps, jcp3);

	// make sure worldPosition is in front
	if (Transform.z < 0.1f)
		return { 0, 0, 0 };

	// get the center of the screen
	vec2 Center;
	Center.x = (float)(windowWidth * 0.5f);
	Center.y = (float)(windowHeight * 0.5f);

	float x = Center.x * (1 - (Transform.x / -camera.fovx / Transform.z));
	float y = Center.y * (1 - (Transform.y / fovy / Transform.z));

	return { x, y, 10 };
}
