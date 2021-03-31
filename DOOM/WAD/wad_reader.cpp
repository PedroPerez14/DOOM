/*
* Name: wad_reader.cpp
* Date: 03/03/2021 (DD/MM/YYYY)
* Author: V�ctor Mart�nez Lascorz (738845)
* Author: Pedro Jos� P�rez garc�a (756642)
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
	
	//Siguientes 4 Bytes: n�mero de directorios del wad
	header.dir_count = Read4Bytes(WAD_data, offset + 4);

	//�ltimos 4 bytes del header: offset en Bytes a los directorios
	header.listing_offset = Read4Bytes(WAD_data, offset + 8);
}

void WADReader::ReadDirectory(const uint8_t* WAD_data, int offset, WAD_Directory& dir)
{
	//Primero el offset
	dir.lump_offset = Read4Bytes(WAD_data, offset);
	//Luego el tama�o
	dir.lump_size = Read4Bytes(WAD_data, offset + 4);
	//Por �ltimo el nombre + '\0'
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

void WADReader::ReadLinedefData(const uint8_t* WAD_data, int offset, Linedef& line)
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

void WADReader::ReadSegsData(const uint8_t* WAD_data, int offset, Seg& seg)
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
