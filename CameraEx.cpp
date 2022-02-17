#include "CameraEx.h"

void	CameraEx::update(GameData *gd)
{
	uintptr_t	cam_ptr = gd->memory->getAddress(gd->baseAddr.get() + 0x0160B8D0, { 0x18, 0x480, 0x108, 0x18, 0xC0, 0x60, 0x170 });
	uintptr_t	win_ptr = 0x7FF4DA574970;

	camera = gd->memory->read<Camera>(cam_ptr);

	//std::cout << "lookAt : "   << camera.lookAt.x << " " << camera.lookAt.y << " " << camera.lookAt.z << std::endl;
	//std::cout << "position : " << camera.position.x << " " << camera.position.y << " " << camera.position.z << std::endl;
	//std::cout << "fovx : "     << camera.fovx << std::endl;

	windowWidth = 820;
	windowHeight = 493; // more like 599 in the engine

	//820
	//493

	//camera.fovx = 1.75;
}

/*
vec3 CameraEx::WorldToScreen(vec3 worldPosition)
{
	float fovy = camera.fovx / windowWidth * windowHeight;

	//float fovy = camera.fovx;
	//float _fovx = 2 * atan(tan(fovy * 0.5) * (windowWidth / windowHeight));//;fovy * windowWidth / windowHeight;

	// profondeur
	D3DXVECTOR3 camToObj(worldPosition.x - camera.position.x, worldPosition.y - camera.position.y, worldPosition.z - camera.position.z);
	float distToObj = sqrt(camToObj.x * camToObj.x + camToObj.y * camToObj.y + camToObj.z * camToObj.z);

	D3DXVec3Normalize(&camToObj, &camToObj);

	float camYaw = atan2f(camera.lookAt.x, camera.lookAt.z);
	float objYaw = atan2f(camToObj.x, camToObj.z);

	float relYaw = camYaw - objYaw;
	if (relYaw > D3DX_PI)
		relYaw -= 2 * D3DX_PI;
	else if (relYaw < -D3DX_PI)
		relYaw += 2 * D3DX_PI;

	float camPitch = asin(camera.lookAt.y);
	float objPitch = asin(camToObj.y);

	float relPitch = camPitch - objPitch;

	//float x = relYaw * 2 / _fovx;			// radian angle measurement cancels here.
	float x = relYaw * 2 / camera.fovx;		// radian angle measurement cancels here.
	float y = relPitch * 2 / fovy;			// and that's the (relative pitch) / (fov / 2)

	x = (x + 1) / 2; // Lastly, change from range (-1,1) to (0,1)  Also, it CAN be outside of that range - if it's outside of the FOV.
	y = (y + 1) / 2;

	//std::cout << "camera.position.x : " << camera.position.x << std::endl;
	//std::cout << "camera.position.y : " << camera.position.y << std::endl;
	//std::cout << "worldPosition.x : " << worldPosition.x << std::endl;
	//std::cout << "worldPosition.y : " << camera.position.y << std::endl;

	//std::cout << "x : " << x << std::endl;
	//std::cout << "y : " << y << std::endl;

	return { x * windowWidth , y * windowHeight , distToObj };
}
*/

float dot(vec3 u, vec3 v)
{
	return (u.x * v.x + u.y * v.y + u.z * v.z);
}

vec3 sub(vec3 u, vec3 v)
{
	return { u.x - v.x, u.y - v.y, u.z - v.z };
}

// https://guidedhacking.com/threads/world2screen-direct3d-and-opengl-worldtoscreen-functions.8044/

/*
vec3 CameraEx::WorldToScreen(vec3 worldPosition)
{
	//Matrix-vector Product, multiplying world(eye) coordinates by projection matrix = clipCoords
	vec4 clipCoords;
	clipCoords.x = worldPosition.x * camera.__rotation.x + worldPosition.y * camera.__rotation.y + worldPosition.z * camera.__rotation.z + camera.__rotation.t;
	clipCoords.y = worldPosition.x * camera.__dunnovec.x + worldPosition.y * camera.__dunnovec.y + worldPosition.z * camera.__dunnovec.z + camera.__dunnovec.t;
	clipCoords.z = worldPosition.x * camera.lookAt.x + worldPosition.y * camera.lookAt.y + worldPosition.z * camera.lookAt.z + camera.lookAt.t;
	clipCoords.t = worldPosition.x * camera.position.x + worldPosition.y * camera.position.y + worldPosition.z * camera.position.z + camera.position.t;

	if (clipCoords.t < 0.1f)
		return { 0, 0, 0 };

	//perspective division, dividing by clip.W = Normalized Device Coordinates
	vec3 NDC;
	NDC.x = clipCoords.x / clipCoords.t;
	NDC.y = clipCoords.y / clipCoords.t;
	NDC.z = clipCoords.z / clipCoords.t;

	float x = (windowWidth / 2 * NDC.x) + (NDC.x + windowWidth / 2);
	float y = -(windowHeight / 2 * NDC.y) + (NDC.y + windowHeight / 2);

	return { x , windowHeight / 2, 10 };
}
*/



vec3 CameraEx::WorldToScreen(vec3 worldPosition)
{
	// refdef c'est la cam
	//refdef_t* refdef = CODEngine::GetRefDef();

	float fovy = camera.fovx / windowWidth * windowHeight;

	vec3	Transform;

	// get our dot products from viewAxis

	vec3	ps = sub(worldPosition, { camera.position.x, camera.position.y, camera.position.z });

	vec3	jcp1({ camera.__rotation.x, camera.__rotation.y, camera.__rotation.z });
	vec3	jcp2({ camera.__dunnovec.x, camera.__dunnovec.y, camera.__dunnovec.z });
	vec3	jcp3({ camera.lookAt.x, camera.lookAt.y, camera.lookAt.z });

	Transform.x = dot(ps, jcp1);
	Transform.y = dot(ps, jcp2);
	Transform.z = dot(ps, jcp3);

	// make sure it is in front of us
	if (Transform.z < 0.1f)
		return {0, 0, 0};

	// get the center of the screen
	vec2 Center;
	Center.x = (float)(windowWidth * 0.5f);
	Center.y = (float)(windowHeight * 0.5f);

	float x = Center.x * (1 - (Transform.x / camera.fovx / Transform.z));
	float y = Center.y * (1 - (Transform.y / fovy / Transform.z));

	return { x, y, 10 };
}
