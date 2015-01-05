#ifndef HGE_CAMERA_UNIT_HPP
#define HGE_CAMERA_UNIT_HPP
#include "hge-math.hpp"
namespace hge
{
	namespace math
	{
		class CameraUnit
		{
		private:
			Vector3D<> cmrX;
			Vector3D<> cmrY;
			Vector3D<> cmrZ;
			Vector3D<> cmrLoc;
			Matrix4D<> viewM;
			Matrix4D<> rotsclM;
		public:
			CameraUnit();
			~CameraUnit();
			void translate(const Vector3D<> &vec);
			void move(const Vector3D<> &vec);
			void rotateLocalX(const float &rad);
			void rotateLocalY(const float &rad);
			void rotateLocalZ(const float &rad);
			void rotateLocal(const float &rad, const Vector3D<> &vec);
			void rotateGlobalX(const float &rad);
			void rotateGlobalY(const float &rad);
			void rotateGlobalZ(const float &rad);
			void rotateTotal(const float &rad, const Vector3D<> &vec);
			void rotate(const float &rad, const Vector3D<> &vec);
			void scaleX(const float &x);
			void scaleY(const float &y);
			void scaleZ(const float &z);
			void scale(const float &x, const float &y, const float &z);
			const Matrix4D<> &getMatrix();
			const Matrix4D<> &getRotateScaleMatrix();
			const Vector3D<> getLocation();
			void moveForward (const float &spd);
			void moveSideward(const float &spd);
		};
	}
}
#endif
