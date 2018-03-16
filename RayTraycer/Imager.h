#pragma once
#include<cmath>

namespace Imager
{
	class Vector3
	{
	public:
		double x;
		double y;
		double z;

		Vector3()
		{
			x=0.0f;
			y=0.0f;
			z=0.0f;
		}

		Vector3(double _x, double _y, double _z)
		{
			x=_x;
			y=_y;
			z=_z;
		}

		const double MagnetitudeSquared() const
		{
			return(x*x+y*y+z*z);
		}

		const double Magnitude() const
		{
			return sqrt(MagnetitudeSquared());
		}

		const Vector3 UnitVector() const
		{
			double mag=Magnitude();
			return Vector3(x/mag,y/mag,z/mag);
		}

		Vector3& operator +=(const Vector3& other)
		{
			x+=other.x;
			y+=other.y;
			z+=other.z;
			return *this;
		}

		Vector3& operator *=(const Vector3& other)
		{
			x *= other.x;
			y *= other.y;
			z *= other.z;
			return *this;
		}

		

		Vector3 operator + (const Vector3 &a, const Vector3 &b)
		{
			return Vector3(a.x + b.x, a.y + b.y, a.z + b.z);
		}

		Vector3 operator - (const Vector3 &a, const Vector3 &b)
		{
			return Vector3(a.x - b.x, a.y - b.y, a.z - b.z);
		}

		double DotProduct(const Vector3 &a, const Vector3 &b)
		{
			return Vector3(
				(a.y*b.z)-(a.z-b.y),
				(a.x*b.z)-(a.z*b.z),
				(a.x*b.y)-(a.y*b.x),
			)
		}
	};
}
