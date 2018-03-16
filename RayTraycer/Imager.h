#pragma once
#include<math.h>
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


	};
}
