#include"Imager.h"
#include<cmath>

namespace Imager
{
	Scene::Scene(const Color & _backgroundColor)
	{
		backgroundColor=_backgroundColor;
		ambientRefraction=REFRACTION_VACUUM;
		activeDebugPoint=NULL;
	}

	Scene::~Scene()
	{
		ClearSolidObjectList();
	}

	const double MIN_OPTICAL_INTENSITY = 0.001;

	inline bool IsSignificant(const Color& color)
	{
		return
			(color.red >= MIN_OPTICAL_INTENSITY) ||
			(color.green >= MIN_OPTICAL_INTENSITY) ||
			(color.blue >= MIN_OPTICAL_INTENSITY);
	}

	void Scene::AddLightSource(const LightSource & lightSource)
	{
		lightSourceList.push_back(lightSource);
	}


	SolidObject & Scene::AddSolidObject(SolidObject * solidObject)
	{
		solidObjectList.push_back(solidObject);
		return *solidObject;
	}


	void Scene::SaveImage(const char * outPngFileName, size_t pixelWide, size_t pixelHigh, double zoom, size_t antiAliasFactor) const
	{
		const size_t largePixelWide=antiAliasFactor*pixelWide;
		const size_t largePixelHigh=antiAliasFactor*pixelHigh;
		const size_t smallerDim= ((pixelWide<pixelHigh)?pixelWide:pixelHigh);

		const double largeZoom=antiAliasFactor*zoom*smallerDim;

		ImageBuffer buffer(largePixelWide,largePixelHigh,backgroundColor);


		Vector3 camera(0.0,0.0,0.0);

		Vector3 direction(0.0,0.0,-1.0);

		const Color fullIntensity(1.0,1.0,1.0);

		PixelList ambigiousPixelList;

		for (size_t i = 0; i < largePixelWide; i++)
		{
			direction.x=(i-largePixelWide/2.0)/largeZoom;

			for (size_t j = 0; j < largePixelHigh; j++)
			{
				direction.y=(largePixelHigh/2.0-j)/largeZoom;

				PixelData& pixel = buffer.Pixel(i, j);
				try
				{
					pixel.color = TarceRay(
						camera,
						direction,
						ambientRefraction,
						fullIntensity,
						0
					);
				}
				catch (AmbiguousIntersectionException)
				{
					pixel.isAmbiguous=true;
					ambigiousPixelList.push_back(PixelCoordinates(i,j));
				}
				
			}
		}

		PixelList::const_iterator iter=ambigiousPixelList.begin();
		PixelList::const_iterator end = ambigiousPixelList.end();

		for (;iter!=end; ++iter)
		{
			const PixelCoordinates& p=*iter;
			ResolveAmbiguousPixel(buffer,p.i,p.j);
		}
	}
	void Scene::SetAmbientRefraction(double refraction)
	{}
	void Scene::AddDebugPoint(int iPixel, int jPixel)
	{}

	void Scene::ClearSolidObjectList()
	{}

	int Scene::FindClosestIntersectionPoint(const Vector3 & vantage, const Vector3 & direction, Intersection & intersection) const
	{
		return 0;
	}

	bool Scene::HasClearLineOfSight(const Vector3 & point1, const Vector3 & point2) const
	{
		return false;
	}

	Color Imager::Scene::TarceRay(const Vector3 & vantage, const Vector3 & direction, double refractiveIndex, Color rayIntensity, int recurtionDepth) const
	{
		Intersection intersection;
		const int numClosest = FindClosestIntersectionPoint(
			vantage,
			direction,
			intersection
		);

		switch (numClosest)
		{
		case 0:
			// The ray of light did not hit anything.
			// Therefore we see the background color attenuated
			// by the incoming ray intensity.
			return rayIntensity*backgroundColor;

			// The ray of light struck exactly one closest surface.
			// Determine the lighting using that single intersection.
		case 1:
			return CalculateLighting(
				intersection,
				direction,
				refractiveIndex,
				rayIntensity,
				1+recurtionDepth
			);

		default:
			// There is an ambiguity: more than one intersection
			// has the same minimum distance.  Caller must catch
			// this exception and have a backup plan for handling
			// this ray of light.
			throw AmbiguousIntersectionException();
			break;
		}
		
	}
	Color Scene::CalculateLighting(const Intersection & intersection, const Vector3 & direction, double refractiveIndex, Color rayIntensity, int recursionDepth) const
	{
		Color colorSum(0.0,0.0,0.0);
		if (recursionDepth <= MAX_OPTICAL_RECURSION_DEPTH) {
			if (IsSignificant(rayIntensity))
			{
				if (intersection.solid == NULL) {
					throw ImageException("Undefined solid at intersection.");
				}
				const SolidObject& solid=*intersection.solid;

				const Optics optics = solid.SurfaceOptics(
					intersection.point,
					intersection.context
				);

				const double opacity=optics.GetOpacity();
				const double transparency=1.0-opacity;
				if (opacity > 0.0) {
					const Color matteColor=opacity*optics.GetMatteColor()*rayIntensity*CalculateMatte(intersection);
					colorSum+=matteColor;

					double refractiveReflectionFactor=0.0;
					if (transparency > 0.0) {
						colorSum += CalculateRefraction(
							intersection,
							direction,
							refractiveIndex,
							transparency*rayIntensity,
							recursionDepth,
							refractiveReflectionFactor
						);
					}

					Color reflectionColor(1.0,1.0,1.0);
					reflectionColor *=transparency*refractiveReflectionFactor;

					reflectionColor+=opacity*optics.GetGlossColor();

					reflectionColor*=rayIntensity;

					if (IsSignificant(reflectionColor)) {
						const Color matteColor = CalculateReflection(
							intersection,direction,refractiveIndex,reflectionColor,recursionDepth);
						colorSum+=matteColor;
					}
				}
			}
		}
		return colorSum;
	}
	Color Scene::CalculateMatte(const Intersection & intersection) const
	{
		Color colorSum(0.0,0.0,0.0);

		LightSourceList::const_iterator iter=lightSourceList.begin();
		LightSourceList::const_iterator end = lightSourceList.end();

		for (;iter!=end; ++iter)
		{
			const LightSource& source=*iter;

			if (HasClearLineOfSight(intersection.point,source.location))
			{
				Vector3 direction=source.location-intersection.point;

				const double incidence = DotProduct(
					intersection.surfaceNormal,direction);

				if (incidence > 0.0) {
					const double intensity=incidence/direction.MagnetitudeSquared();
					colorSum+=intensity*source.color;
				}
			}
		}

		return colorSum;
	}
	Color Scene::CalculateReflection(const Intersection & intersection, const Vector3 & incidentDir, double refectiveIndex, Color rayIntensity, int recurtionDepth) const
	{
		const Vector3& normal =intersection.surfaceNormal;
		const double perp=2.0*DotProduct(incidentDir,normal);
		const Vector3 reflectDir=incidentDir-(perp*normal);

		return TarceRay(
			intersection.point,
			reflectDir,
			refectiveIndex,
			rayIntensity,
			recurtionDepth
			);
	}


	Color Scene::CalculateRefraction(
		const Intersection & intersection,
		const Vector3 & direction,
		double sourceRefectiveIndex,
		Color rayIntensity,
		int recurtionDepth,
		 double & outRefrectionFactor) const
	{
		const Vector3 dirUnit=direction.UnitVector();

		double cos_a1=DotProduct(dirUnit,intersection.surfaceNormal);
		double sin_a1;
		if (cos_a1 <= -1.0) {
			if (cos_a1 < -1.0001) {
				throw ImageException("Dot product too small");
			}

			cos_a1=-1.0;
			sin_a1=0.0;
		}
		else if (cos_a1 >= 1.0) {
			if (cos_a1 > 1.0001) {
				throw ImageException("Dot product too small");
			}

			cos_a1=1.0;
			sin_a1=0.0;
		}
		else
		{
			sin_a1=sqrt(1.0-cos_a1*cos_a1);
		}

		const double SMALL_SHIFT=0.001;
		const Vector3 testPoint=intersection.point+SMALL_SHIFT*dirUnit;

		const SolidObject* container=PrimaryContainer(testPoint);

		const double targetRefractiveIndex=(container!=NULL)?container->GetRefractiveIndex():ambientRefraction;

		const double ratio=sourceRefectiveIndex/targetRefractiveIndex;

		const double sin_a2=ratio*sin_a1;

		if (sin_a2 <= -1.0 || sin_a2 >= 1.0) {
			outRefrectionFactor=1.0;
			return Color(0.0,0.0,0.0);
		}

		double k[2];
		const int numSolutions = Algebra::SolveQuadraticEquation(
			1.0,
			2.0 * cos_a1,
			1.0 - 1.0 / (ratio*ratio),
			k);

		double maxAlignment=-0.001;
		Vector3 refractionDir;
		for (int i = 0; i < numSolutions; i++)
		{
			Vector3 refractionAttempt=dirUnit+(k[i]*intersection.surfaceNormal);
			double alignment=DotProduct(dirUnit,refractionAttempt);
			if (alignment > maxAlignment) {
				maxAlignment=alignment;
				refractionDir=refractionAttempt;
			}
		}

		if (maxAlignment <= 0.0) {
			throw ImageException("Refraction faliure");
		}

		double cos_a2=sqrt(1.0-sin_a2*sin_a2);
		if (cos_a1 < 0.0) {
			cos_a2-=cos_a2;
		}


		// We assume uniform polarization of light,
		// and therefore average the contributions of s-polarized
		// and p-polarized light.
		const double Rs = PolarizedReflection(
			sourceRefectiveIndex,
			targetRefractiveIndex,
			cos_a1,
			cos_a2);

		const double Rp = PolarizedReflection(
			sourceRefectiveIndex,
			targetRefractiveIndex,
			cos_a2,
			cos_a1);

		outRefrectionFactor = (Rs + Rp) / 2.0;

		const Color nextRayIntensity=(1.0-outRefrectionFactor)*rayIntensity;

		return TarceRay(
		intersection.point,
			refractionDir,
			targetRefractiveIndex,
			nextRayIntensity,
			recurtionDepth
		)



		
	}
	const SolidObject * Scene::PrimaryContainer(const Vector3 & point) const
	{
		return nullptr;
	}
	double Scene::PolarizedReflection(double n1, double n2, double cos_a1, double cos_a2) const
	{
		return 0.0;
	}
	void Scene::ResolveAmbiguousPixel(ImageBuffer & buffer, size_t i, size_t j) const
	{}
	unsigned char Scene::ConvertPixelValue(double colorComponent, double maxColorValue)
	{
		return 0;
	}
}