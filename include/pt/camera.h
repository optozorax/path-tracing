#ifndef PT_CAMERA_H
#define PT_CAMERA_H

namespace pt
{

	class Camera
	{
	public:
		Camera(Vector pos) : pos(pos) {}

		virtual ~Camera() {}
		virtual Ray getRay(float x, float y) = 0;

		Vector pos;
	};

};

#endif