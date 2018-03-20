#include"Imager.h"

namespace Imager
{
	

	
	SolidObject::SolidObject(const Vector3 & _center, bool _isFullyEnclosed)
		:isFullyEnclosed(_isFullyEnclosed)
	{
		center = _center;
		refractiveIndex = REFRACTION_GLASS;
	}

	int SolidObject::FindClosestIntersection(const Vector3 & vantage, const Vector3 & direction, Intersection & intersection) const
	{
		cachedIntersectionList.clear();
		AppendAllIntersections(vantage,direction,cachedIntersectionList);
		return PickClosestIntersection(cachedIntersectionList,intersection);
	}

	

	Optics SolidObject::SurfaceOptics(const Vector3 & surfacePoint, const void * context) const
	{
		return uniformOptics;
	}

	double SolidObject::GetRefractiveIndex() const
	{
		return refractiveIndex;
	}

	SolidObject & SolidObject::Translate(double dx, double dy, double dz)
	{
		center.x+=dx;
		center.y += dy;
		center.z += dz;
		return *this;
	}

	SolidObject & SolidObject::Move(double cx, double cy, double cz)
	{
	
		return Translate(cx-center.x,cy-center.y,cz-center.z);
	}


	SolidObject & SolidObject::Move(const Vector3 & newCenter)
	{
		return Move(newCenter.x,newCenter.y,newCenter.z);
	}

	const Vector3 & SolidObject::Center() const
	{
		return center;
	}

	void SolidObject::SetUniformOptics(const Optics & optics)
	{
		uniformOptics = optics;
	}

	void SolidObject::SetMatteGlossBalance(double glossFactor, const Color & rawMatteColor, const Color & rawGlossColor)
	{
		uniformOptics.SetMatteGlossBalance(glossFactor,rawMatteColor, rawGlossColor);
	}

	void SolidObject::SetFullMatte(const Color & matteColor)
	{
		uniformOptics.SetMatteGlossBalance(0.0,matteColor,Color(0,0,0));
	}

	void SolidObject::SetOptics(const double opacity)
	{
		uniformOptics.SetOpacity(opacity);
	}

	void SolidObject::SetRefraction(const double refraction)
	{
		ValidateRefraction(refraction);
		refractiveIndex=refraction;
	}

	const Optics & SolidObject::GetUniformOptics() const
	{
		return uniformOptics;
	}
	


}