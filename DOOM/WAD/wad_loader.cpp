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

int WADLoader::FindMapIndex(Map* map)
{
	int index = map->getMapIndex();
	if (index > -1)
	{
		return index;
	}
	for (int i = 0; i < WAD_dirs.size(); i++)
	{
		if (WAD_dirs[i].lump_name == map->GetName())
		{
			map->setMapIndex(i);
			return i;
		}
	}
	return -1;
}

bool WADLoader::ReadMapVertex(Map* map)
{
	int index = FindMapIndex(map);
	int n_verts;
	Vertex v;
	if(index < 0)
	{
		std::cerr << "Could not load vertexes from map " << map->GetName() << "." << std::endl;
		return false;
	}
	std::cout << "INDEX: " << index << std::endl;
	index += LUMPINDEX::eVERTEXES;
	//El lump del mapa va seguido por los lumps con vértices, linedefs, etc.
	//Siempre siguen el mismo orden, definido en map_types.h
	if(strcmp(WAD_dirs[index].lump_name, "VERTEXES") != 0)
	{
		//Si no encontramos los vértices del mapa, abortamos
		std::cerr << "Could not find vertexes from map " << map->GetName() << "." << std::endl;
		return false;
	}
	n_verts = WAD_dirs[index].lump_size / sizeof(Vertex);
	for (int i = 0; i < n_verts; i++)
	{
		reader.ReadVertexData(WAD_data, WAD_dirs[index].lump_offset + i * sizeof(Vertex), v);
		map->addVertex(v);
		std::cout << "Cargando vertex" << std::endl;
		std::cout << v.x << std::endl;
		std::cout << v.y << std::endl;
		std::cout << std::endl;
	}
	return true;
}

bool WADLoader::ReadMapLinedef(Map* map)
{
	int index = FindMapIndex(map);
	int n_linedefs;
	Linedef ld;
	if (index < 0)
	{
		std::cerr << "Could not find map " << map->GetName() << " to load linedefs from" << std::endl;
		return false;
	}
	index += LUMPINDEX::eLINEDEFS;
	if (strcmp(WAD_dirs[index].lump_name, "LINEDEFS") != 0)
	{
		std::cerr << "Could not find linedefs to load from map " << map->GetName() << std::endl;
		return false;
	}
	n_linedefs = WAD_dirs[index].lump_size / sizeof(Linedef);
	for (int i = 0; i < n_linedefs; i++)
	{
		reader.ReadLinedefData(WAD_data, WAD_dirs[index].lump_offset + i * sizeof(Linedef), ld);
		map->addLinedef(ld);
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

bool WADLoader::ReadMapThing(Map* map)
{
	int index = FindMapIndex(map);
	int n_things;
	Thing th;
	if (index < 0)
	{
		std::cerr << "Could not find map " << map->GetName() << " to load any THING from!" << std::endl;
		return false;
	}
	index += LUMPINDEX::eTHINGS;
	if (strcmp(WAD_dirs[index].lump_name, "THINGS") != 0)
	{
		std::cerr << "Could not find THINGS to load from map " << map->GetName() << std::endl;
		return false;
	}
	n_things = WAD_dirs[index].lump_size / sizeof(Thing);
	for (int i = 0; i < n_things; i++)
	{
		reader.ReadThingData(WAD_data, WAD_dirs[index].lump_offset + i * sizeof(Thing), th);
		map->addThing(th);
		std::cout << "Cargando THING" << std::endl;
		std::cout << th.XPos << std::endl;
		std::cout << th.YPos << std::endl;
		std::cout << th.Angle << std::endl;
		std::cout << th.Type << std::endl;
		std::cout << th.Flags << std::endl;
		std::cout << std::endl;
	}
	return true;
}

bool WADLoader::ReadMapNodes(Map* map)
{
	int index = FindMapIndex(map);
	int n_nodes;
	BSP_Node node;
	if (index < 0)
	{
		std::cerr << "Could not find map " << map->GetName() << " to load any NODE from!" << std::endl;
		return false;
	}
	index += LUMPINDEX::eNODES;
	if (strcmp(WAD_dirs[index].lump_name, "NODES") != 0)
	{
		std::cerr << "Could not find NODES to load from map " << map->GetName() << std::endl;
		return false;
	}
	n_nodes = WAD_dirs[index].lump_size / sizeof(BSP_Node);
	for (int i = 0; i < n_nodes; i++)
	{
		reader.ReadNodesData(WAD_data, WAD_dirs[index].lump_offset + i * sizeof(BSP_Node), node);
		map->addNode(node);
		std::cout << "Cargando NODE" << std::endl;
		std::cout << node.XPartition << std::endl;
		std::cout << node.YPartition << std::endl;
		std::cout << node.XPartDir << std::endl;
		std::cout << node.YPartDir << std::endl;
		std::cout << std::endl;
	}
	return true;
}

bool WADLoader::ReadMapSegs(Map* map)
{
	int index = FindMapIndex(map);
	int n_segs;
	Seg seg;
	if (index < 0)
	{
		std::cerr << "Could not find map " << map->GetName() << " to load any SEGS from!" << std::endl;
		return false;
	}
	index += LUMPINDEX::eSEGS;
	if (strcmp(WAD_dirs[index].lump_name, "SEGS") != 0)
	{
		std::cerr << "Could not find SEGS to load from map " << map->GetName() << std::endl;
		return false;
	}
	n_segs = WAD_dirs[index].lump_size / sizeof(Seg);
	for (int i = 0; i < n_segs; i++)
	{
		reader.ReadSegsData(WAD_data, WAD_dirs[index].lump_offset + i * sizeof(Seg), seg);
		map->addSeg(seg);
		std::cout << "Cargando SEG" << std::endl;
		std::cout << seg.vert1 << std::endl;
		std::cout << seg.vert2 << std::endl;
		std::cout << seg.angle << std::endl;
		std::cout << seg.linedef_index << std::endl;
		std::cout << seg.dir << std::endl;
		std::cout << seg.offset << std::endl;
		std::cout << std::endl;
	}
	return true;
}

bool WADLoader::ReadMapSubs(Map* map)
{
	int index = FindMapIndex(map);
	int n_subs;
	Subsector ssec;
	if (index < 0)
	{
		std::cerr << "Could not find map " << map->GetName() << " to load any SSECTORS from!" << std::endl;
		return false;
	}
	index += LUMPINDEX::eSSECTORS;
	if (strcmp(WAD_dirs[index].lump_name, "SSECTORS") != 0)
	{
		std::cerr << "Could not find SSECTORS to load from map " << map->GetName() << std::endl;
		return false;
	}
	n_subs = WAD_dirs[index].lump_size / sizeof(Subsector);
	for (int i = 0; i < n_subs; i++)
	{
		reader.ReadSSecsData(WAD_data, WAD_dirs[index].lump_offset + i * sizeof(Subsector), ssec);
		map->addSsect(ssec);
		std::cout << "Cargando SSECTOR" << std::endl;
		std::cout << ssec.seg_count << std::endl;
		std::cout << ssec.first_segID << std::endl;
		std::cout << std::endl;
	}
	return true;
}

bool WADLoader::LoadMapData(Map* map)
{
	//Si alguna falla se aborta el proceso, 
	//	las propias funciones informan por pantalla del fallo así que no hace falta poner nada más aquí
	return (ReadMapVertex(map) && ReadMapLinedef(map) && ReadMapThing(map) && ReadMapNodes(map) && ReadMapSegs(map) && ReadMapSubs(map));
}
