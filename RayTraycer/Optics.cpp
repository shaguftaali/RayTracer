#include"Imager.h"

namespace Imager
{
	Optics::Optics()
	{
		matteColor=Color(1.0,1.0,1.0);
		glossColor = Color(1.0, 1.0, 1.0);
		opacity=1.0;
	}

	Optics::Optics(Color _matteColor, Color _glossColor, double _opacity)
	{
		SetMatteColor(_matteColor);
		SetGlossColor(_glossColor);
		SetOpacity(opacity);
	}
	void Optics::SetMatteColor(const Color & _matteColor)
	{
		ValidateReflectionColor(_matteColor);
		matteColor=_matteColor;


	}

	void Optics::SetGlossColor(const Color & _glossColor)
	{
		ValidateReflectionColor(_glossColor);
		glossColor = _glossColor;
	}

	void Optics::SetOpacity(double _opacity)
	{
		if (_opacity < 0.0 || _opacity > 1.0)
		{
			throw ImageException("Invalid opacity.");
		}
		opacity = _opacity;
	}

	void Optics::SetMatteGlossBalance(double glossFactor, const Color & rawMatteColor, Color & rawGlossColor)
	{
		//TODO

		// Make sure the raw colors have realistic values.
		// Otherwise very high component values can defeat
		// the purpose of trying to balance reflected light
		// levels to realistic ranges.
		ValidateReflectionColor(rawMatteColor);
		ValidateReflectionColor(rawGlossColor);
	}

	const Color & Optics::GetMatteColor() const
	{
		return matteColor;
	}

	const Color & Optics::GetGlossColor() const
	{
		return glossColor;
	}

	const double Optics::GetOpacity() const
	{
		return opacity;
	}

	void Optics::ValidateReflectionColor(const Color & color) const
	{
		// A color is valid for reflection if all its
		// components are in the range 0..1.
		// Otherwise, it is possible for multiple reflections
		// to keep "amplifying" light beyond any limit.
		if (color.red < 0.0 || color.red > 1.0)
		{
			throw ImageException("Invalid red color component.");
		}

		if (color.green < 0.0 || color.green > 1.0)
		{
			throw ImageException("Invalid green color component.");
		}

		if (color.blue < 0.0 || color.blue > 1.0)
		{
			throw ImageException("Invalid blue color component.");
		}

	}



}