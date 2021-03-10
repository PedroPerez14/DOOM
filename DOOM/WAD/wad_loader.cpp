/*
* Name: wad_loader.cpp
* Date: 03/03/2021 (DD/MM/YYYY)
* Author: Víctor Martínez Lascorz (738845)
* Author: Pedro José Pérez garcía (756642)
* Coms: Cargador de archivos .WAD para nuestro DOOM.
*	Basado en https://github.com/amroibrahim/DIYDoom/tree/master/DIYDOOM/Notes001/notes
*/

#include "wad_loader.h"
#include "wad_reader.h"
#include "DataTypes.h"

WADLoader::WADLoader(std::string filePath) : WAD_data(NULL), WAD_path(filePath)
{
}

WADLoader::~WADLoader()
{

}

bool WADLoader::OpenAndLoad()
{
	WAD_stream.open(WAD_path, std::ifstream::binary);
	if (!WAD_stream.is_open())
	{
		std::cerr << "Error. Could not open .WAD file " << WAD_path << std::endl;
		return false;
	}
	//Si se ha abierto correctamente, toca alojar el espacio necesario en memoria
	WAD_stream.seekg(0, WAD_stream.end);
	size_t length = WAD_stream.tellg();
	WAD_data = new uint8_t[length];
	WAD_stream.seekg(std::ifstream::beg);		//Hay que volver a llevar el lector al principio del fichero
	WAD_stream.read((char*)WAD_data, length);	//Leer todo el .WAD a memoria
	
	ReadDirectories();							//A continuación se leen los directorios

	//Por último, se cierra el WAD
	WAD_stream.close();
	return true;
}

bool WADLoader::LoadWAD()
{
	return OpenAndLoad();
}

void WADLoader::ReadDirectories()
{
	WAD_Header header;
	WAD_Directory directory;
	
	reader.ReadHeader(WAD_data, 0, header);

	for (unsigned int i = 0; i < header.dir_count; i++)
	{
		reader.ReadDirectory(WAD_data, header.listing_offset + i * 16, directory);

		WAD_dirs.push_back(directory);
	}
	std::cout << WAD_path << " loaded succesfully." << std::endl;
}

int WADLoader::FindMapIndex(Map& map)
{
	for (int i = 0; i < WAD_dirs.size(); i++)
	{
		if (WAD_dirs[i].lump_name == map.GetName())
		{
			return i;
		}
	}
	return -1;
}

bool WADLoader::ReadMapVertex(Map& map)
{
	int index = FindMapIndex(map);
	int n_verts;
	Vertex v;
	if(index < 0)
	{
		std::cerr << "Could not load vertexes from map " << map.GetName() << "." << std::endl;
		return false;
	}
	std::cout << "INDEX: " << index << std::endl;
	index += LUMPINDEX::eVERTEXES;
	//El lump del mapa va seguido por los lumps con vértices, linedefs, etc.
	//Siempre siguen el mismo orden, definido en map_types.h
	if(strcmp(WAD_dirs[index].lump_name, "VERTEXES") != 0)
	{
		//Si no encontramos los vértices del mapa, abortamos
		std::cerr << "Could not find vertexes from map " << map.GetName() << "." << std::endl;
		return false;
	}
	n_verts = WAD_dirs[index].lump_size / sizeof(Vertex);
	for (int i = 0; i < n_verts; i++)
	{
		reader.ReadVertexData(WAD_data, WAD_dirs[index].lump_offset + i * sizeof(Vertex), v);
		map.addVertex(v);
		std::cout << "Cargando vertex" << std::endl;
		std::cout << v.x << std::endl;
		std::cout << v.y << std::endl;
		std::cout << std::endl;
	}
	return true;
}

bool WADLoader::ReadMapLinedef(Map& map)
{
	int index = FindMapIndex(map);
	int n_linedefs;
	Linedef ld;
	if (index < 0)
	{
		std::cerr << "Could not find map " << map.GetName() << " to load linedefs from" << std::endl;
		return false;
	}
	index += LUMPINDEX::eLINEDEFS;
	if (strcmp(WAD_dirs[index].lump_name, "LINEDEFS") != 0)
	{
		std::cerr << "Could not find linedefs to load from map " << map.GetName() << std::endl;
		return false;
	}
	n_linedefs = WAD_dirs[index].lump_size / sizeof(Linedef);
	for (int i = 0; i < n_linedefs; i++)
	{
		reader.ReadLinedefData(WAD_data, WAD_dirs[index].lump_offset + i * sizeof(Linedef), ld);
		map.addLinedef(ld);
		std::cout << "Cargando linedef" << std::endl;
		std::cout << ld.vert1 << std::endl;
		std::cout << ld.vert2 << std::endl;
		std::cout << ld.flags << std::endl;
		std::cout << ld.line_type << std::endl;
		std::cout << ld.sector_tag << std::endl;
		std::cout << ld.sidedef_r << std::endl;
		std::cout << ld.sidedef_l << std::endl;
		std::cout << std::endl;
	}
	return true;
}

bool WADLoader::LoadMapData(Map& map)
{
	//Si alguna falla se dedican a informar del fallo las funciones
	return (ReadMapVertex(map) && ReadMapLinedef(map));
}
