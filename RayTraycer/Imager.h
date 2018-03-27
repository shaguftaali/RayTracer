#pragma once
#include<cmath>
#include<string>
#include<vector>

namespace Imager
{

	const double EPSILON = 1.0e-6;
	// Forward declarations
	class SolidObject;
	class ImageBuffer;

	const int MAX_OPTICAL_RECURSION_DEPTH = 20;

	class ImageException
	{
	public:
		explicit ImageException(const char* _message)
			: message(_message)
		{

		}
		const char *GetMessage() const { return message; }

	private:
		const char* const message;
	};

	class AmbiguousIntersectionException
	{};

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

		

		
	};


	inline Vector3 operator+ (const Vector3 &a, const Vector3 &b)
	{
		return Vector3(a.x + b.x, a.y + b.y, a.z + b.z);
	}

	inline Vector3 operator - (const Vector3 &a, const Vector3 &b)
	{
		return Vector3(a.x - b.x, a.y - b.y, a.z - b.z);
	}

	inline Vector3 operator - (const Vector3& a)
	{
		return Vector3(-a.x, -a.y, -a.z);
	}

	inline double DotProduct(const Vector3 &a, const Vector3 &b)
	{
		return (a.x*b.x) + (a.y*b.y) + (a.z*b.z);
	}

	inline Vector3 CrossProduct(const Vector3 &a, const Vector3 &b)
	{
		return Vector3(
			(a.y*b.z) - (a.z - b.y),
			(a.x*b.z) - (a.z*b.z),
			(a.x*b.y) - (a.y*b.x)
		);
	}

	inline Vector3 operator * (double s, const Vector3& v)
	{
		return Vector3(s*v.x, s*v.y, s*v.z);
	}

	inline Vector3 operator / (const Vector3& v, double s)
	{
		return Vector3(v.x / s, v.y / s, v.z / s);
	}


	struct  Color
	{
		double red;
		double green;
		double blue;

		Color(double _red, double _green, double _blue,double _luminosity=1.0)
		{
			red=_red*_luminosity;
			green=_green*_luminosity;
			blue=_blue*_luminosity;
		}

		Color()
		{
			red=0.0;
			green=0.0;
			blue=0.0;
		}

		Color& operator+=(const Color& other)
		{
			red += other.red;
			green += other.green;
			blue += other.blue;
		}

		Color& operator*=(const Color& other)
		{
			red*=other.red;
			green*=other.green;
			blue*=other.blue;
		}

		Color& operator *= (double factor)
		{
			red *= factor;
			green *= factor;
			blue *= factor;
			return *this;
		}

		Color& operator/=(const Color& other)
		{
			red /= other.red;
			green /= other.green;
			blue /= other.blue;
		}

		void Validate() const
		{
			if ((red <0.0)||(green<0.0)||(blue<0.0))
			{

				throw new ImageException("Negative color not allowed");
			}
		}
			 

	};

	inline Color operator*(const Color& aColor, const Color& bColor)
	{
		return Color(
			aColor.red*bColor.red,
			aColor.green*bColor.green,
			aColor.blue*bColor.blue

		);
	}

	inline Color operator*(double scalar, const Color& bColor)
	{
		return Color(
			scalar*bColor.red,
			scalar*bColor.green,
			scalar*bColor.blue

		);
	}

	inline Color operator + (const Color& a, const Color& b)
	{
		return Color(
			a.red + b.red,
			a.green + b.green,
			a.blue + b.blue);
	}


	//------------------------------------------------------------------------
	//  Refractive indexes of common substances/media...

	const double REFRACTION_VACUUM = 1.0000;
	const double REFRACTION_AIR = 1.0003;
	const double REFRACTION_ICE = 1.3100;
	const double REFRACTION_WATER = 1.3330;
	const double REFRACTION_GASOLINE = 1.3980;
	const double REFRACTION_GLASS = 1.5500;
	const double REFRACTION_SAPPHIRE = 1.7700;
	const double REFRACTION_DIAMOND = 2.4190;

	// Range of allowed refraction values...
	const double REFRACTION_MINIMUM = 1.0000;
	const double REFRACTION_MAXIMUM = 9.0000;

	inline void ValidateRefraction(double refraction)
	{
		if (refraction < REFRACTION_MINIMUM ||
			refraction > REFRACTION_MAXIMUM)
		{
			throw ImageException("Invalid refractive index.");
		}
	}


	

	class Taggable
	{
	public:
		Taggable(std::string _tag="")
		{
			tag=_tag;
		}

		void SetTag(std::string _tag)
		{
			tag=_tag;
		}

		std::string GetTag() const
		{
			return tag;
		}

		

	private:
		std::string tag;
	};

	struct LightSource: public Taggable
	{
		Vector3 location;
		Color color;
		LightSource(const Vector3& _location, const Color& _color, std::string _tag="")
			:Taggable(_tag)
		{
			location=_location;
			color=_color;
		}
		
	};


	class Optics
	{
	public:
		Optics();


		explicit Optics(Color _matteColor, Color _glossColor = Color(0.0, 0.0, 0.0), double _opacity=1.0);

		void SetMatteColor(const Color& _matteColor);
		void SetGlossColor(const Color& _glossColor);
		void SetOpacity(double _opacity);

		void SetMatteGlossBalance(double glossFactor,const Color& rawMatteColor, const Color& rawGlossColor);

		const Color& GetMatteColor() const;

		const Color& GetGlossColor() const;
		const double GetOpacity() const;

	protected:
		void ValidateReflectionColor(const Color& color) const;

	private:
		Color matteColor;
		Color glossColor;
		double opacity;
	};


	struct  Intersection
	{
		double distanceSquared;

		Vector3 point;

		Vector3 surfaceNormal;

		// A pointer to the solid object that the ray
		// intersected with.
		const SolidObject* solid;

		const void* context;

		Intersection()
		{
			distanceSquared=1.0e+20;
			
		}
			 
	};


	typedef std::vector<Intersection> IntersectionList;

	int PickClosestIntersection(
		const IntersectionList& list,
		Intersection& intersection);
	
	class SolidObject:public Taggable
	{
	public:

		SolidObject(const Vector3& _center=Vector3(),bool _isFullyEnclosed=true);
		
		virtual void AppendAllIntersections(const Vector3& vantage, const Vector3& direction, IntersectionList& intersectionList)const=0;

		int FindClosestIntersection(const Vector3& vantage, const Vector3& direction, Intersection& intersection)const;

		virtual bool Contains(const Vector3& point)const;

		virtual Optics SurfaceOptics(const Vector3& surfacePoint, const void *context)const;

		double GetRefractiveIndex() const;

		virtual SolidObject& RotateX(double angleInDegrees)=0;

		virtual SolidObject& RotateY(double angleInDegrees) = 0;

		virtual SolidObject& RotateZ(double angleInDegrees) = 0;

		virtual SolidObject& Translate(double dx,double dy, double dz);

		SolidObject& Move(double cx, double cy,double cz);

		SolidObject& Move(const Vector3& newCenter);

		const Vector3& Center() const;

		void SetUniformOptics(const Optics& optics);

		void SetMatteGlossBalance(double glossFactor, const Color& rawMatte, const Color& rawGlossColor);

		void SetFullMatte(const Color& matteColor);

		void SetOptics(const double opatics);

		void SetRefraction(const double refraction);
	
	protected:
		const Optics& GetUniformOptics() const;

	private:
		Vector3 center;

		Optics uniformOptics;

		double refractiveIndex;

		const bool isFullyEnclosed;

		mutable IntersectionList cachedIntersectionList;
		mutable IntersectionList enclosedList;
	};


	class Sphere :public SolidObject
	{
	public:
		Sphere(const Vector3& _center,double radius);

		virtual void AppendAllIntersections(const Vector3& vantage, const Vector3& direction, IntersectionList& intersectionList)const = 0;

		virtual bool Contains(const Vector3& point) const;

		virtual SolidObject& RotateX(double angleInDegrees);
		virtual SolidObject& RotateY(double angleInDegrees);
		virtual SolidObject& RotateZ(double angleInDegrees);

	private:
		double radius;
	};

	
	class Scene
	{
	public:
		Scene(const Color& _backgroundColor=Color());

		virtual ~Scene();


		void AddLightSource(const LightSource &lightSource);

		SolidObject& AddSolidObject(SolidObject* solidObject);

		void SaveImage(const char* outPngFileName, size_t pixelWide,size_t pixelHigh,double zoom, size_t antiAliasFactor)const;

		void SetAmbientRefraction(double refraction);

		void AddDebugPoint(int iPixel,int jPixel);


		
	private:
		void ClearSolidObjectList();
		
		int FindClosestIntersectionPoint(const Vector3& vantage,const Vector3& direction, Intersection& intersection) const;

		bool HasClearLineOfSight(const Vector3& point1, const Vector3& point2) const;

		Color TarceRay(const Vector3& vantage, const Vector3& direction, double refractiveIndex, Color rayIntensity, int recurtionDepth) const;

		Color CalculateLighting(const Intersection& intersection, const Vector3& direction, double refractiveIndex, Color rayIntensity, int recursionDepth)const;

		Color CalculateMatte(const Intersection& intersection) const;


		Color CalculateReflection(
			const Intersection& intersection,
			const Vector3& incidentDir,
			double refectiveIndex,
			Color rayIntensity,
			int recurtionDepth)const;

		Color CalculateRefraction(
			const Intersection& intersection,
			const Vector3& direction,
			double sourceRefectiveIndex,
			Color rayIntensity,
			int recurtionDepth,
			double& outRefrectionFactor)const;

		const SolidObject* PrimaryContainer(const Vector3& point) const;

		double PolarizedReflection(
			double n1,                           // source material's index of refraction
			double n2,                          // target material's index of refraction
			double cos_a1,                     // incident or outgoing ray angle cosine
			double cos_a2)const;              // outgoing or incident ray angle cosine

		void ResolveAmbiguousPixel(ImageBuffer& buffer, size_t i, size_t j) const;

		// Convert a floating point color component value, 
		// based on the maximum component value,
		// to a byte RGB value in the range 0x00 to 0xff.
		static unsigned char ConvertPixelValue(
			double colorComponent,
			double maxColorValue);


		Color backgroundColor;

		typedef std::vector<SolidObject*> SolidObjectList;
		typedef std::vector<LightSource> LightSourceList;

		struct PixelCoordinates
		{
			size_t i;
			size_t j;

			PixelCoordinates(size_t _i, size_t _j)
			{
				i=_i;
				j=_j;
			}

		};

		typedef std::vector<PixelCoordinates> PixelList;

		SolidObjectList solidObjectList;

		LightSourceList lightSourceList;


		double ambientRefraction;

		mutable IntersectionList cachedIntersectionList;

		struct DebugPoint
		{
			int     iPixel;
			int     jPixel;

			DebugPoint(int _iPixel, int _jPixel)
				: iPixel(_iPixel)
				, jPixel(_jPixel)
			{}
		};
		typedef std::vector<DebugPoint> DebugPointList;
		DebugPointList debugPointList;
		mutable const DebugPoint* activeDebugPoint;

	};


	struct PixelData
	{
		Color color;
		bool isAmbiguous;

		PixelData()
			:color(),
			isAmbiguous(false)
		{
		}

	};
	
	class ImageBuffer
	{
	public:
		ImageBuffer(
			size_t _pixelWide,
			size_t _pixelHigh,
			const Color &backgroundColor);

		virtual ~ImageBuffer();

		PixelData& Pixel(size_t i,size_t j) const;

		size_t GetPixelsWide() const;

		size_t GetPixelHigh() const;

		double MaxColorValue() const;

	private:
		size_t  pixelsWide;     // the width of the image in pixels (columns).
		size_t  pixelsHigh;     // the height of the image in pixels (rows).
		size_t  numPixels;      // the total number of pixels.
		PixelData*  array;      // flattened array [pixelsWide * pixelsHigh].
	};

	
	
}
