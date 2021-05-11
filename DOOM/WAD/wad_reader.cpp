/*
* Name: wad_reader.cpp
* Date: 03/03/2021 (DD/MM/YYYY)
* Author: Víctor Martínez Lascorz (738845)
* Author: Pedro José Pérez garcía (756642)
* Coms: Cargador de archivos .WAD para nuestro DOOM.
*	Basado en https://github.com/amroibrahim/DIYDoom/tree/master/DIYDOOM/Notes001/notes
*/

#include "wad_reader.h"


void WADReader::ReadHeader(const uint8_t* WAD_data, int offset, WAD_Header& header)
{
	//4 primeros bytes [0x00 - 0x03]: tipo de wad
	for (int i = 0; i < 4; i++)
	{
		header.WAD_type[i] = WAD_data[offset + i];
	}
	header.WAD_type[4] = '\0';
	
	//Siguientes 4 Bytes: número de directorios del wad
	header.dir_count = Read4Bytes(WAD_data, offset + 4);

	//Últimos 4 bytes del header: offset en Bytes a los directorios
	header.listing_offset = Read4Bytes(WAD_data, offset + 8);
}

void WADReader::ReadDirectory(const uint8_t* WAD_data, int offset, WAD_Directory& dir)
{
	//Primero el offset
	dir.lump_offset = Read4Bytes(WAD_data, offset);
	//Luego el tamaño
	dir.lump_size = Read4Bytes(WAD_data, offset + 4);
	//Por último el nombre + '\0'
	for (int i = 0; i < 8; i++)
	{
		dir.lump_name[i] = WAD_data[offset + 8 + i];
	}
	dir.lump_name[8] = '\0';

	std::cout << dir.lump_offset << std::endl;
	std::cout << dir.lump_size << std::endl;
	std::cout << dir.lump_name << std::endl << std::endl;
}

uint16_t WADReader::Read2Bytes(const uint8_t* WAD_data, int offset)
{
	uint16_t read_value;
	std::memcpy(&read_value, WAD_data + offset, sizeof(uint16_t));
	return read_value;
}

uint32_t WADReader::Read4Bytes(const uint8_t* WAD_data, int offset)
{
	uint32_t read_value;
	std::memcpy(&read_value, WAD_data + offset, sizeof(uint32_t));
	return read_value;
}

void WADReader::ReadVertexData(const uint8_t* WAD_data, int offset, Vertex& vertex)
{
	vertex.x = Read2Bytes(WAD_data, offset);
	vertex.y = Read2Bytes(WAD_data, offset + 2);
}

void WADReader::ReadLinedefData(const uint8_t* WAD_data, int offset, WADLinedef& line)
{
	line.vert1 = Read2Bytes(WAD_data, offset);
	line.vert2 = Read2Bytes(WAD_data, offset + 2);
	line.flags = Read2Bytes(WAD_data, offset + 4);
	line.line_type = Read2Bytes(WAD_data, offset + 6);
	line.sector_tag = Read2Bytes(WAD_data, offset + 8);
	line.sidedef_r = Read2Bytes(WAD_data, offset + 10);
	line.sidedef_l = Read2Bytes(WAD_data, offset + 12);
}

void WADReader::ReadThingData(const uint8_t* WAD_data, int offset, Thing& thing)
{
	thing.XPos = Read2Bytes(WAD_data, offset);
	thing.YPos = Read2Bytes(WAD_data, offset + 2);
	thing.Angle = Read2Bytes(WAD_data, offset + 4);
	thing.Type = Read2Bytes(WAD_data, offset + 6);
	thing.Flags = Read2Bytes(WAD_data, offset + 8);
}

void WADReader::ReadNodesData(const uint8_t* WAD_data, int offset, BSP_Node& node)
{
	node.XPartition = Read2Bytes(WAD_data, offset);
	node.YPartition = Read2Bytes(WAD_data, offset + 2);
	node.XPartDir = Read2Bytes(WAD_data, offset + 4);
	node.YPartDir = Read2Bytes(WAD_data, offset + 6);

	node.RightBoxTop = Read2Bytes(WAD_data, offset + 8);
	node.RightBoxBottom = Read2Bytes(WAD_data, offset + 10);
	node.RightBoxLeft = Read2Bytes(WAD_data, offset + 12);
	node.RightBoxRight = Read2Bytes(WAD_data, offset + 14);

	node.LeftBoxTop = Read2Bytes(WAD_data, offset + 16);
	node.LeftBoxBottom = Read2Bytes(WAD_data, offset + 18);
	node.LeftBoxLeft = Read2Bytes(WAD_data, offset + 20);
	node.LeftBoxRight = Read2Bytes(WAD_data, offset + 22);

	node.RightChild = Read2Bytes(WAD_data, offset + 24);
	node.LeftChild = Read2Bytes(WAD_data, offset + 26);
}

void WADReader::ReadSegsData(const uint8_t* WAD_data, int offset, WADSeg& seg)
{
	seg.vert1 = Read2Bytes(WAD_data, offset);
	seg.vert2 = Read2Bytes(WAD_data, offset + 2);
	seg.angle = Read2Bytes(WAD_data, offset + 4);
	seg.linedef_index = Read2Bytes(WAD_data, offset + 6);
	seg.dir = Read2Bytes(WAD_data, offset + 8);
	seg.offset = Read2Bytes(WAD_data, offset + 10);
}

void WADReader::ReadSSecsData(const uint8_t* WAD_data, int offset, Subsector& ssec)
{
	ssec.seg_count = Read2Bytes(WAD_data, offset);
	ssec.first_segID = Read2Bytes(WAD_data, offset + 2);
}

void WADReader::ReadSecsData(const uint8_t* WAD_data, int offset, WADSector& sec)
{
	sec.FloorHeight = Read2Bytes(WAD_data, offset);
	sec.CeilingHeight = Read2Bytes(WAD_data, offset + 2);
	//FloorTexture
	for (int i = 0; i < 8; i++)
	{
		sec.FloorTexture[i] = WAD_data[offset + 4 + i];
	}
	//CeilingTexture
	for (int i = 0; i < 8; i++)
	{
		sec.CeilingTexture[i] = WAD_data[offset + 12 + i];
	}
	sec.Lightlevel = Read2Bytes(WAD_data, offset + 20);
	sec.Type = Read2Bytes(WAD_data, offset + 22);
	sec.Tag = Read2Bytes(WAD_data, offset + 24);
}

void WADReader::ReadSidedefsData(const uint8_t* WAD_data, int offset, WADSidedef& sidedef)
{
	sidedef.XOffset = Read2Bytes(WAD_data, offset);
	sidedef.YOffset = Read2Bytes(WAD_data, offset + 2);
	//UpperTexture
	for (int i = 0; i < 8; i++)
	{
		sidedef.UpperTexture[i] = WAD_data[offset + 4 + i];
	}
	//LowerTexture
	for (int i = 0; i < 8; i++)
	{
		sidedef.LowerTexture[i] = WAD_data[offset + 12 + i];
	}
	//MiddleTexture
	for (int i = 0; i < 8; i++)
	{
		sidedef.MiddleTexture[i] = WAD_data[offset + 20 + i];
	}
	sidedef.SectorID = Read2Bytes(WAD_data, offset + 28);
}

void WADReader::ReadPalette(const uint8_t* WAD_data, int offset, WADPalette& palette)
{
	for (int i = 0; i < 256; ++i)
	{
		palette.Colors[i].r = WAD_data[offset++];
		palette.Colors[i].g = WAD_data[offset++];
		palette.Colors[i].b = WAD_data[offset++];
		palette.Colors[i].a = 255;
	}
}

void WADReader::ReadPatchHeader(const uint8_t* WAD_data, int offset, WADPatchHeader& header)
{
	header.Width = Read2Bytes(WAD_data, offset);
	header.Height = Read2Bytes(WAD_data, offset + 2);
	header.LeftOffset = Read2Bytes(WAD_data, offset + 4);
	header.TopOffset = Read2Bytes(WAD_data, offset + 6);

	header.ColumnOffset = new uint32_t[header.Width];
	offset += 8;
	for (int i = 0; i < header.Width; ++i)
	{
		header.ColumnOffset[i] = Read4Bytes(WAD_data, offset);
		offset += 4;
	}
}

int WADReader::ReadPatchColumn(const uint8_t* WAD_data, int offset, WADPatchColumn& col)
{
	col.TopDelta = WAD_data[offset++];
	int iDataIndex = 0;
	if (col.TopDelta != 0xFF)
	{
		col.Length = WAD_data[offset++];
		col.PaddingPre = WAD_data[offset++];

		col.pColumnData = new uint8_t[col.Length];
		for (int i = 0; i < col.Length; ++i)
		{
			col.pColumnData[i] = WAD_data[offset++];
		}
		col.PaddingPost = WAD_data[offset++];
	}
	return offset;
}

void WADReader::ReadTextureHeader(const uint8_t* WAD_data, int offset, WADTextureHeader& th)
{
	th.TexturesCount = Read4Bytes(WAD_data, offset);
	th.TexturesOffset = Read4Bytes(WAD_data, offset + 4);
	th.pTexturesDataOffset = new uint32_t[th.TexturesCount];
	offset = offset + 4;
	for (int i = 0; i < th.TexturesCount; ++i)
	{
		th.pTexturesDataOffset[i] = Read4Bytes(WAD_data, offset);
		offset = offset + 4;
	}
}

void WADReader::ReadPName(const uint8_t* WAD_data, int offset, WADPNames& PNames)
{
	PNames.PNameCount = Read4Bytes(WAD_data, offset);
	PNames.PNameOffset += 4;
}

void WADReader::ReadTextureData(const uint8_t* WAD_data, int offset, WADTextureData& texture)
{
	texture.TextureName[0] = WAD_data[offset];
	texture.TextureName[1] = WAD_data[offset + 1];
	texture.TextureName[2] = WAD_data[offset + 2];
	texture.TextureName[3] = WAD_data[offset + 3];
	texture.TextureName[4] = WAD_data[offset + 4];
	texture.TextureName[5] = WAD_data[offset + 5];
	texture.TextureName[6] = WAD_data[offset + 6];
	texture.TextureName[7] = WAD_data[offset + 7];
	texture.TextureName[8] = '\0';

	texture.Flags = Read4Bytes(WAD_data, offset + 8);
	texture.Width = Read2Bytes(WAD_data, offset + 12);
	texture.Height = Read2Bytes(WAD_data, offset + 14);
	texture.ColumnDirectory = Read4Bytes(WAD_data, offset + 16);
	texture.PatchCount = Read2Bytes(WAD_data, offset + 20);
	texture.pTexturePatch = new WADTexturePatch[texture.PatchCount];

	offset += 22;
	for (int i = 0; i < texture.PatchCount; ++i)
	{
		ReadTexturePatch(WAD_data, offset, texture.pTexturePatch[i]);
		offset += 10;
	}
}

void WADReader::ReadTexturePatch(const uint8_t* WAD_data, int offset, WADTexturePatch& texturepatch)
{
	texturepatch.XOffset = Read2Bytes(WAD_data, offset);
	texturepatch.YOffset = Read2Bytes(WAD_data, offset + 2);
	texturepatch.PNameIndex = Read2Bytes(WAD_data, offset + 4);
	texturepatch.StepDir = Read2Bytes(WAD_data, offset + 6);
	texturepatch.ColorMap = Read2Bytes(WAD_data, offset + 8);
}

void WADReader::Read8Characters(const uint8_t* WAD_data, int offset, char* pName)
{
	pName[0] = WAD_data[offset++];
	pName[1] = WAD_data[offset++];
	pName[2] = WAD_data[offset++];
	pName[3] = WAD_data[offset++];
	pName[4] = WAD_data[offset++];
	pName[5] = WAD_data[offset++];
	pName[6] = WAD_data[offset++];
	pName[7] = WAD_data[offset];
}
