#include <Common/Image/Image.h>
#include <Core/Core.h>

namespace Columbus
{

	uint32 GetBPPFromFormat(TextureFormat Format)
	{
		switch (Format)
		{
		case TextureFormat::R8:      return 1;  break;
		case TextureFormat::RG8:     return 2;  break;
		case TextureFormat::RGB8:    return 3;  break;
		case TextureFormat::RGBA8:   return 4;  break;

		case TextureFormat::R16:    
		case TextureFormat::R16F:    return 2;  break;
		case TextureFormat::RG16:    
		case TextureFormat::RG16F:   return 4;  break;
		case TextureFormat::RGB16:
		case TextureFormat::RGB16F:  return 6;  break;
		case TextureFormat::RGBA16:
		case TextureFormat::RGBA16F: return 8;  break;

		case TextureFormat::R32F:    return 4;  break;
		case TextureFormat::RG32F:   return 8;  break;
		case TextureFormat::RGB32F:  return 12; break;
		case TextureFormat::RGBA32F: return 16; break;

		case TextureFormat::Unknown: return 0;  break;

		default:                     return 0;  break;
		}

		return 0;
	}

	ImageFormat ImageGetFormat(std::string FileName)
	{
		if (ImageIsBMP(FileName)) return ImageFormat::BMP;
		if (ImageIsDDS(FileName)) return ImageFormat::DDS;
		if (ImageIsPNG(FileName)) return ImageFormat::PNG;
		if (ImageIsTIF(FileName)) return ImageFormat::TIF;
		if (ImageIsJPG(FileName)) return ImageFormat::JPG;
		if (ImageIsTGA(FileName)) return ImageFormat::TGA;

		return ImageFormat::Unknown;
	}
	//////////////////////////////////////////////////////////////////////////////
	//////////////////////////////////////////////////////////////////////////////
	//////////////////////////////////////////////////////////////////////////////
	bool ImageBGR2RGB(uint8* aData, uint64 aSize)
	{
		if (aData == nullptr) return false;

		uint8 bgr[3];
		for (uint64 i = 0; i < aSize; i += 3)
		{
			bgr[0] = aData[i + 0];
			bgr[1] = aData[i + 1];
			bgr[2] = aData[i + 2];

			aData[i + 0] = bgr[2];
			aData[i + 1] = bgr[1];
			aData[i + 2] = bgr[0];
		}

		return true;
	}
	
	bool ImageBGRA2RGBA(uint8* aData, uint64 aSize)
	{
		if (aData == nullptr) return false;

		uint8 bgr[3];
		for (uint64 i = 0; i < aSize; i += 4)
		{
			bgr[0] = aData[i + 0];
			bgr[1] = aData[i + 1];
			bgr[2] = aData[i + 2];

			aData[i + 0] = bgr[2];
			aData[i + 1] = bgr[1];
			aData[i + 2] = bgr[0];
			aData[i + 3] = aData[i + 3];
		}

		return true;
	}
	
	bool ImageABGR2RGBA(uint8* aData, uint64 aSize)
	{
		if (aData == nullptr) return false;

		uint8 abgr[4];
		for (uint64 i = 0; i < aSize; i += 4)
		{
			abgr[0] = aData[i + 0];
			abgr[1] = aData[i + 1];
			abgr[2] = aData[i + 2];
			abgr[3] = aData[i + 3];

			aData[i + 0] = abgr[3];
			aData[i + 1] = abgr[2];
			aData[i + 2] = abgr[1];
			aData[i + 3] = abgr[0];
		}

		return true;
	}
	
	bool ImageRGB2BGR(uint8* aData, uint64 aSize)
	{
		return ImageBGR2RGB(aData, aSize);
	}
	
	bool ImageRGBA2BGRA(uint8* aData, uint64 aSize)
	{
		return ImageBGRA2RGBA(aData, aSize);
	}
	
	bool ImageFlipX(uint8* Data, uint32 Width, uint32 Height, uint32 BPP)
	{
		if (Data == nullptr) return false;

		const uint32 stride = Width * BPP;
		uint8* row;
		uint8* pixel = new uint8[BPP];

		for (uint32 i = 0; i < Height; i++)
		{
			row = &Data[i * stride];

			for (uint32 j = 0; j < Width / 2; j++)
			{
				Memory::Memcpy(pixel, &row[j * BPP], BPP);
				Memory::Memcpy(&row[j * BPP], &row[(Width - j) * BPP], BPP);
				Memory::Memcpy(&row[(Width - j) * BPP], pixel, BPP);
			}
		}

		return true;
	}
	
	bool ImageFlipY(uint8* Data, uint32 Width, uint32 Height, uint32 BPP)
	{
		if (Data == nullptr) return false;

		const uint32 stride = Width * BPP;
		uint8* row = (uint8_t*)malloc(stride);

		for (uint32 i = 0; i < Height / 2; i++)
		{
			Memory::Memcpy(row, &Data[i * stride], stride);
			Memory::Memcpy(&Data[i * stride], &Data[(Height - i - 1) * stride], stride);
			Memory::Memcpy(&Data[(Height - i - 1) * stride], row, stride);
		}
		Memory::Free(row);

		return true;
	}
	
	bool ImageFlipXY(uint8* Data, uint32 Width, uint32 Height, uint32 BPP)
	{
		if (Data == nullptr) return false;

		const uint32 size = Width * Height;
		uint8* pixel = new uint8[BPP];

		for (size_t i = 0; i < size / 2; i++)
		{
			Memory::Memcpy(pixel, &Data[i * BPP], BPP);
			Memory::Memcpy(&Data[i * BPP], &Data[(size - i) * BPP], BPP);
			Memory::Memcpy(&Data[(size - i) * BPP], pixel, BPP);
		}
		Memory::Free(pixel);

		return true;
	}
	//////////////////////////////////////////////////////////////////////////////
	//////////////////////////////////////////////////////////////////////////////
	//////////////////////////////////////////////////////////////////////////////
	uint8* ImageLoad(std::string FileName, uint32& OutWidth, uint32& OutHeight, uint64& OutSize, uint32& OutMipMaps, TextureFormat& OutFormat)
	{
		switch (ImageGetFormat(FileName))
		{
		case ImageFormat::BMP: return ImageLoadBMP(FileName, OutWidth, OutHeight, OutSize, OutFormat); break;
		case ImageFormat::DDS: return ImageLoadDDS(FileName, OutWidth, OutHeight, OutSize, OutMipMaps, OutFormat); break;
		case ImageFormat::PNG: return ImageLoadPNG(FileName, OutWidth, OutHeight, OutSize, OutFormat); break;
		case ImageFormat::TIF: return ImageLoadTIF(FileName, OutWidth, OutHeight, OutSize, OutFormat); break;
		case ImageFormat::JPG: return ImageLoadJPG(FileName, OutWidth, OutHeight, OutSize, OutFormat); break;
		case ImageFormat::TGA: return ImageLoadTGA(FileName, OutWidth, OutHeight, OutSize, OutFormat); break;
		case ImageFormat::Unknown: return nullptr; break;
		default: return nullptr; break;
		}

		return nullptr;
	}
	//////////////////////////////////////////////////////////////////////////////
	bool ImageSave(std::string FileName, uint32 Width, uint32 Height, TextureFormat BPP, uint8* Data, ImageFormat Format, uint32 Quality)
	{
		switch (Format)
		{
		case ImageFormat::BMP:
			return ImageSaveBMP(FileName, Width, Height, BPP, Data);
			break;
		case ImageFormat::DDS:
			return false; //TODO
			break;
		case ImageFormat::TGA:
			return ImageSaveTGA(FileName, Width, Height, BPP, Data);
			break;
		case ImageFormat::PNG:
			return ImageSavePNG(FileName, Width, Height, BPP, Data);
			break;
		case ImageFormat::TIF:
			return ImageSaveTIF(FileName, Width, Height, BPP, Data);
			break;
		case ImageFormat::JPG:
			return ImageSaveJPG(FileName, Width, Height, BPP, Data, Quality);
			break;
		case ImageFormat::Unknown:
			return false;
			break;
		}

		return false;
	}
	/*
	* Image class
	*/
	Image::Image() :
		Width(0),
		Height(0),
		Size(0),
		MipMaps(0),
		Exist(false),
		Data(nullptr)
	{ }
	/*
	* Load image from file
	* @param std::string InFileName: Name of image file to load
	* @param int Flags: Loading flags
	*/
	bool Image::Load(std::string InFileName, ImageLoading Flags)
	{
		FreeData();

		Data = ImageLoad(InFileName, Width, Height, Size, MipMaps, Format);
		if (Data == nullptr) return false;

		else
		{
			FileName = InFileName;
			Exist = true;

			switch (Flags)
			{
			case ImageLoading::FlipX:  FlipX();  break;
			case ImageLoading::FlipY:  FlipY();  break;
			case ImageLoading::FlipXY: FlipXY(); break;
			case ImageLoading::None: break;
			}

			return true;
		}
	}
	/*
	* Save image to file
	* @param std::string InFileName: Name of image file to save
	* @param Format: Image format
	* @param Quality: Compression level
	*/
	bool Image::Save(std::string InFileName, ImageFormat InFormat, size_t Quality) const
	{
		return false;
		if (!IsExist()) return false;
		return ImageSave(InFileName, Width, Height, Format, Data, InFormat, Quality);
	}
	/*
	* Checks if image is exist in memory
	*/
	bool Image::IsExist() const
	{
		return Exist;
	}
	/*
	* Frees image data
	*/
	void Image::FreeData()
	{
		Width = 0;
		Height = 0;
		Size = 0;
		MipMaps = 0;
		Format = TextureFormat::RGBA8;
		Exist = false;
		FileName.clear();

		delete[] Data;
	}
	/*
	* Horizontal image flipping
	*/
	bool Image::FlipX()
	{
		if (IsExist())
		{
			return ImageFlipX(Data, Width, Height, GetBPPFromFormat(Format));
		}

		return false;
	}
	/*
	* Vertical image flipping
	*/
	bool Image::FlipY()
	{
		if (IsExist())
		{
			return ImageFlipY(Data, Width, Height, GetBPPFromFormat(Format));
		}

		return false;
	}
	/*
	* Diagonal image flipping
	*/
	bool Image::FlipXY()
	{
		if (IsExist())
		{
			return ImageFlipXY(Data, Width, Height, GetBPPFromFormat(Format));
		}

		return false;
	}

	bool Image::IsRawFormat() const
	{
		return (Format == TextureFormat::R8 ||
		        Format == TextureFormat::RG8 ||
		        Format == TextureFormat::RGB8 ||
		        Format == TextureFormat::RGBA8);
	}

	bool Image::IsUnsignedShortFormat() const
	{
		return (Format == TextureFormat::R16 ||
		        Format == TextureFormat::RG16 ||
		        Format == TextureFormat::RGB16 ||
		        Format == TextureFormat::RGBA16);
	}

	bool Image::IsHalfFormat() const
	{
		return (Format == TextureFormat::R16F ||
		        Format == TextureFormat::RG16F ||
		        Format == TextureFormat::RGB16F ||
		        Format == TextureFormat::RGBA16F);
	}

	bool Image::IsFloatFormat() const
	{
		return (Format == TextureFormat::R32F ||
		        Format == TextureFormat::RG32F ||
		        Format == TextureFormat::RGB32F ||
		        Format == TextureFormat::RGBA32F);
	}

	bool Image::IsCompressedFormat() const
	{
		return (Format == TextureFormat::DXT1 ||
		        Format == TextureFormat::DXT3 ||
		        Format == TextureFormat::DXT5);
	}
	
	uint32 Image::GetWidth() const
	{
		return Width;
	}
	
	uint32 Image::GetHeight() const
	{
		return Height;
	}

	uint32 Image::GetDepth() const
	{
		return Depth;
	}

	uint32 Image::GetMipmapsCount() const
	{
		return MipMaps;
	}

	uint64 Image::GetOffset(uint32 Level) const
	{
		return 0; //TODO
	}

	uint64 Image::GetSize() const
	{
		return Size;
	}

	uint8* Image::Get2DData(uint32 Level) const
	{
		if (Data != nullptr)
		{
			return Data + GetOffset(Level);
		}

		return nullptr;
	}

	TextureFormat Image::GetFormat() const
	{
		return Format;
	}
	
	uint8* Image::GetData() const
	{
		return Data;
	}
	
	std::string Image::GetFileName() const
	{
		return FileName;
	}
	
	Image::~Image()
	{
		FreeData();
	}
}






