#include"Imager.h"

namespace Imager
{
	ImageBuffer::ImageBuffer(size_t _pixelWide, size_t _pixelHigh, const Color & backgroundColor)
	{
		pixelsWide=_pixelWide;
		pixelsHigh=_pixelHigh;
		numPixels=_pixelHigh*_pixelWide;
		array=new PixelData[numPixels];
	}

	ImageBuffer::~ImageBuffer()
	{
		delete[] array;
		array=NULL;
		pixelsWide = 0;
		pixelsHigh = 0;
		numPixels = 0;
	}

	PixelData & ImageBuffer::Pixel(size_t i, size_t j) const
	{
		if ((i < pixelsWide) && (j < pixelsHigh)) {
			return array[(j*pixelsWide)+i];
		}
		else
		{
			throw ImageException("Pixel cordinates out of bound");
		}
	}
	size_t ImageBuffer::GetPixelsWide() const
	{
		return pixelsWide;
	}
	size_t ImageBuffer::GetPixelHigh() const
	{
		return pixelsHigh;
	}
	double ImageBuffer::MaxColorValue() const
	{
		double max = 0.0;
		for (size_t i = 0; i < numPixels; ++i)
		{
			array[i].color.Validate();
			if (array[i].color.red > max)
			{
				max = array[i].color.red;
			}
			if (array[i].color.green > max)
			{
				max = array[i].color.green;
			}
			if (array[i].color.blue > max)
			{
				max = array[i].color.blue;
			}
		}
		if (max == 0.0)
		{
			// Safety feature: the image is solid black anyway,
			// so there is no point trying to scale it.
			// If we did, we would end up dividing by zero.
			max = 1.0;
		}
		return max;
	}
}