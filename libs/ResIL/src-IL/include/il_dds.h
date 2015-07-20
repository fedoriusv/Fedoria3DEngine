//-----------------------------------------------------------------------------
//
// ImageLib Sources
// Copyright (C) 2000-2008 by Denton Woods
// Last modified: 12/27/2008
//
// Filename: src-IL/include/il_dds.h
//
// Description: Reads from a DirectDraw Surface (.dds) file.
//
//-----------------------------------------------------------------------------


#ifndef DDS_H
#define DDS_H

#include "il_internal.h"


#ifdef _WIN32
	#pragma pack(push, dds_struct, 1)
#endif
typedef struct DDSHEAD
{
	ILbyte	Signature[4];

	ILuint	Size1;				// size of the structure (minus MagicNum)
	ILuint	Flags1; 			// determines what fields are valid
	ILuint	Height; 			// height of surface to be created
	ILuint	Width;				// width of input surface
	ILuint	LinearSize; 		// Formless late-allocated optimized surface size
	ILuint	Depth;				// Depth if a volume texture
	ILuint	MipMapCount;		// number of mip-map levels requested
	ILuint	AlphaBitDepth;		// depth of alpha buffer requested

	ILuint	NotUsed[10];

	ILuint	Size2;				// size of structure
	ILuint	Flags2;				// pixel format flags
	ILuint	FourCC;				// (FOURCC code)
	ILuint	RGBBitCount;		// how many bits per pixel
	ILuint	RBitMask;			// mask for red bit
	ILuint	GBitMask;			// mask for green bits
	ILuint	BBitMask;			// mask for blue bits
	ILuint	RGBAlphaBitMask;	// mask for alpha channel

	ILuint	ddsCaps1, ddsCaps2, ddsCaps3, ddsCaps4; // direct draw surface capabilities
	ILuint	TextureStage;
} IL_PACKSTRUCT DDSHEAD;
#ifdef _WIN32
	#pragma pack(pop, dds_struct)
#endif



// use cast to struct instead of RGBA_MAKE as struct is
//  much
typedef struct Color8888
{
	ILubyte r;		// change the order of names to change the 
	ILubyte g;		//  order of the output ARGB or BGRA, etc...
	ILubyte b;		//  Last one is MSB, 1st is LSB.
	ILubyte a;
} Color8888;


typedef struct Color888
{
	ILubyte r;		// change the order of names to change the 
	ILubyte g;		//  order of the output ARGB or BGRA, etc...
	ILubyte b;		//  Last one is MSB, 1st is LSB.
} Color888;


typedef struct Color565
{
	unsigned nBlue  : 5;		// order of names changes
	unsigned nGreen : 6;		//  byte order of output to 32 bit
	unsigned nRed	: 5;
} Color565;


typedef struct DXTColBlock
{
	ILshort col0;
	ILshort col1;

	// no bit fields - use bytes
	ILbyte row[4];
} DXTColBlock;

typedef struct DXTAlphaBlockExplicit
{
	ILshort row[4];
} DXTAlphaBlockExplicit;

typedef struct DXTAlphaBlock3BitLinear
{
	ILbyte alpha0;
	ILbyte alpha1;

	ILbyte stuff[6];
} DXTAlphaBlock3BitLinear;


// Defines

//Those 4 were added on 20040516 to make
//the written dds files more standard compliant
#define DDS_CAPS				0x00000001L
#define DDS_HEIGHT				0x00000002L
#define DDS_WIDTH				0x00000004L

#define DDS_RGB					0x00000040L
#define DDS_PIXELFORMAT			0x00001000L

#define DDS_LUMINANCE			0x00020000L

#define DDS_ALPHAPIXELS			0x00000001L
#define DDS_ALPHA				0x00000002L
#define DDS_FOURCC				0x00000004L
#define DDS_PITCH				0x00000008L
#define DDS_COMPLEX				0x00000008L
#define DDS_TEXTURE				0x00001000L
#define DDS_MIPMAPCOUNT			0x00020000L
#define DDS_LINEARSIZE			0x00080000L
#define DDS_VOLUME				0x00200000L
#define DDS_MIPMAP				0x00400000L
#define DDS_DEPTH				0x00800000L

#define DDS_CUBEMAP				0x00000200L
#define DDS_CUBEMAP_POSITIVEX	0x00000400L
#define DDS_CUBEMAP_NEGATIVEX	0x00000800L
#define DDS_CUBEMAP_POSITIVEY	0x00001000L
#define DDS_CUBEMAP_NEGATIVEY	0x00002000L
#define DDS_CUBEMAP_POSITIVEZ	0x00004000L
#define DDS_CUBEMAP_NEGATIVEZ	0x00008000L


#define IL_MAKEFOURCC(ch0, ch1, ch2, ch3) \
			((ILint)(ILbyte)(ch0) | ((ILint)(ILbyte)(ch1) << 8) |	\
			((ILint)(ILbyte)(ch2) << 16) | ((ILint)(ILbyte)(ch3) << 24 ))

enum PixFormat
{
	PF_ARGB,
	PF_RGB,
	PF_DXT1,
	PF_DXT2,
	PF_DXT3,
	PF_DXT4,
	PF_DXT5,
	PF_3DC,
	PF_ATI1N,
	PF_LUMINANCE,
	PF_LUMINANCE_ALPHA,
	PF_RXGB, //Doom3 normal maps
	PF_A16B16G16R16,
	PF_R16F,
	PF_G16R16F,
	PF_A16B16G16R16F,
	PF_R32F,
	PF_G32R32F,
	PF_A32B32G32R32F,
	PF_UNKNOWN = 0xFF
};

#define CUBEMAP_SIDES 6

// Internal functions used by il_blp, il_dds-save, il_iwi, il_vtf, il_utx
ILboolean	ILAPI DecompressDXT1(ILimage *image, ILubyte *lCompData);
ILboolean	DecompressDXT3(ILimage *image, ILubyte *& CompData);
ILboolean	DecompressDXT5(ILimage *image, ILubyte *& CompData);
ILboolean	iConvFloat16ToFloat32(ILuint* dest, ILushort* src, ILuint size);
ILboolean	WriteHeader(ILimage *Image, ILenum DXTCFormat, ILuint CubeFlags);
void		CorrectEndDXT1(ILushort *ex0, ILushort *ex1, ILboolean HasAlpha);
ILAPI ILuint ILAPIENTRY ilGetDXTCData(void *Buffer, ILuint BufferSize, ILenum DXTCFormat);

//extern ILuint CubemapDirections[CUBEMAP_SIDES];

void DxtcReadColor(ILushort Data, Color8888* Out);

#endif//DDS_H