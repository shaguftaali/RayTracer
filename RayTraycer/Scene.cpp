#include"Imager.h"

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

			for (size_t j = 0; j < largePixelHigh; j++)
			{

			}
		}

	}
	void Scene::SetAmbientRefraction(double refraction)
	{}
	void Scene::AddDebugPoint(int iPixel, int jPixel)
	{}
}