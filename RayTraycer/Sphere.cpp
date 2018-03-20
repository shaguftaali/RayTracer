#include"Imager.h"

namespace Imager
{

	Sphere::Sphere(const Vector3 & _center, double _radius):SolidObject(_center)
	{
		radius=_radius;
		SetTag("Sphere");
	}
	bool Sphere::Contains(const Vector3 & point) const
	{
		const double r=radius+EPSILON;
		return (point-Center()).MagnetitudeSquared<=(r*r);
	}
	SolidObject & Sphere::RotateX(double angleInDegrees)
	{
		return *this;
	}
	SolidObject & Sphere::RotateY(double angleInDegrees)
	{
		return *this;
	}
	SolidObject & Sphere::RotateZ(double angleInDegrees)
	{
		return *this;
	}
}