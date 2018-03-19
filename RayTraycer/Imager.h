#pragma once
#include<cmath>
#include<string>

namespace Imager
{
	// Forward declarations
	class SolidObject;

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

		Color& operator*=(const Color& other)
		{
			red*=other.red;
			green*=other.green;
			blue*=other.blue;
		}

		Color& operator+=(const Color& other)
		{
			red += other.red;
			green += other.green;
			blue += other.blue;
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

	

	

	class Taggable
	{
	public:
		Taggable(std::string _tag)
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

		void SetMatteGlossBalance(double glossFactor,const Color& rawMatteColor, Color& rawGlossColor);

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

	

	
	
	
}
