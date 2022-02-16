#include "CameraEx.h"

void	CameraEx::update(GameData *gd)
{
	uintptr_t	cam_ptr = 0x7FF4E327A100;		// TODO : use static pointers
	uintptr_t	win_ptr = 0x7FF4DA574970;

	camera = gd->memory->read<Camera>(cam_ptr);

	//std::cout << "lookAt : "   << camera.lookAt.x << " " << camera.lookAt.y << " " << camera.lookAt.z << std::endl;
	//std::cout << "position : " << camera.position.x << " " << camera.position.y << " " << camera.position.z << std::endl;
	//std::cout << "fovx : "     << camera.fovx << std::endl;

	windowWidth = 800;
	windowHeight = 600; // more like 599 in the engine
}

vec3 CameraEx::WorldToScreen(vec3 worldPosition)
{
	float fovy = camera.fovx / windowWidth * windowHeight;
	D3DXVECTOR3 camToObj(worldPosition.x - camera.position.x, worldPosition.y - camera.position.y, worldPosition.z - camera.position.z);
	float distToObj = sqrt(camToObj.x * camToObj.x + camToObj.y * camToObj.y + camToObj.z * camToObj.z);
	D3DXVec3Normalize(&camToObj, &camToObj);

	float camYaw = atan2f(camera.lookAt.y, camera.lookAt.x);
	float objYaw = atan2f(camToObj.y, camToObj.x);

	float relYaw = camYaw - objYaw;
	if (relYaw > D3DX_PI)
		relYaw -= 2 * D3DX_PI;
	else if (relYaw < -D3DX_PI)
		relYaw += 2 * D3DX_PI;

	float objPitch = asin(camToObj.z);
	float camPitch = asin(camera.lookAt.z);

	float relPitch = camPitch - objPitch;

	float x = relYaw * 2 / camera.fovx; // radian angle measurement cancels here.
	float y = relPitch * 2 / fovy; // and that's the (relative pitch) / (fov / 2)

	x = (x + 1) / 2; // Lastly, change from range (-1,1) to (0,1)  Also, it CAN be outside of that range - if it's outside of the FOV.
	y = (y + 1) / 2;

	return { x * windowWidth, y * windowHeight, distToObj };
}