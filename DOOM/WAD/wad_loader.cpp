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
#include "../PatchesTextures/AssetsManager.h"

WADLoader::WADLoader()
{}

WADLoader::WADLoader(std::string filePath, DisplayManager* dp) : WAD_data(NULL), WAD_path(filePath)
{
	m_pDisplayManager = dp;
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
	WADLinedef ld;
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
	n_linedefs = WAD_dirs[index].lump_size / sizeof(WADLinedef);
	for (int i = 0; i < n_linedefs; i++)
	{
		reader.ReadLinedefData(WAD_data, WAD_dirs[index].lump_offset + i * sizeof(WADLinedef), ld);
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
	WADSeg seg;
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
	n_segs = WAD_dirs[index].lump_size / sizeof(WADSeg);
	for (int i = 0; i < n_segs; i++)
	{
		reader.ReadSegsData(WAD_data, WAD_dirs[index].lump_offset + i * sizeof(WADSeg), seg);
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

bool WADLoader::ReadMapSectors(Map* map)
{
	int index = FindMapIndex(map);
	int n_secs;
	WADSector sec;
	if (index < 0)
	{
		std::cerr << "Could not find map " << map->GetName() << " to load any SECTORS from!" << std::endl;
		return false;
	}
	index += LUMPINDEX::eSECTORS;
	if (strcmp(WAD_dirs[index].lump_name, "SECTORS") != 0)
	{
		std::cerr << "Could not find SECTORS to load from map " << map->GetName() << std::endl;
		return false;
	}
	n_secs = WAD_dirs[index].lump_size / sizeof(WADSector);
	for (int i = 0; i < n_secs; i++)
	{
		reader.ReadSecsData(WAD_data, WAD_dirs[index].lump_offset + i * sizeof(WADSector), sec);
		map->addSect(sec);
		std::cout << "Cargando SECTOR" << std::endl;
		std::cout << std::endl;
	}
	return true;
}

bool WADLoader::ReadMapSidedefs(Map* map)
{
	int index = FindMapIndex(map);
	int n_sidedefs;
	WADSidedef sidedef;
	if (index < 0)
	{
		std::cerr << "Could not find map " << map->GetName() << " to load any SIDEDEFS from!" << std::endl;
		return false;
	}
	index += LUMPINDEX::eSIDEDDEFS;
	if (strcmp(WAD_dirs[index].lump_name, "SIDEDEFS") != 0)
	{
		std::cerr << "Could not find SIDEDEFS to load from map " << map->GetName() << std::endl;
		return false;
	}
	n_sidedefs = WAD_dirs[index].lump_size / sizeof(WADSidedef);
	for (int i = 0; i < n_sidedefs; i++)
	{
		reader.ReadSidedefsData(WAD_data, WAD_dirs[index].lump_offset + i * sizeof(WADSidedef), sidedef);
		map->addSidedef(sidedef);
		std::cout << "Cargando SIDEDEF" << std::endl;
		std::cout << std::endl;
	}
	return true;
}

bool WADLoader::LoadPalette(DisplayManager* pDisplayManager)
{
	std::cout << "Cargando paletas de colores..." << std::endl;
	int iPlaypalIndex = FindLumpByName("PLAYPAL");
	if (strcmp(WAD_dirs[iPlaypalIndex].lump_name, "PLAYPAL") != 0)
	{
		std::cout << "ERROR ENCONTRANDO LUMP DE LAS PALETAS DE COLORES!" << std::endl;
		return false;
	}
	WADPalette palette;
	for (int i = 0; i < NUM_PALETAS; ++i)	//14 paletas
	{
		std::cout << "Leyendo paleta " << i << std::endl;
		reader.ReadPalette(WAD_data, WAD_dirs[iPlaypalIndex].lump_offset + (i * 3 * 256), palette);
		pDisplayManager->AddColorPalette(palette);
	}
}

bool WADLoader::LoadPatch(std::string patchName)
{
	AssetsManager* pAssetsManager = AssetsManager::getInstance();
	int iPatchIndex = FindLumpByName(patchName);
	if (strcmp(WAD_dirs[iPatchIndex].lump_name, patchName.c_str()) != 0)
	{
		return false;
	}
	WADPatchHeader patchHeader;
	reader.ReadPatchHeader(WAD_data, WAD_dirs[iPatchIndex].lump_offset, patchHeader);
	Patch* pPatch = pAssetsManager->AddPatch(patchName, patchHeader);
	WADPatchColumn patchCol;
	for (int i = 0; i < patchHeader.Width; i++)
	{
		int offset = WAD_dirs[iPatchIndex].lump_offset + patchHeader.ColumnOffset[i];
		pPatch->AppendColumnStartIndex();
		do
		{
			offset = reader.ReadPatchColumn(WAD_data, offset, patchCol);
			pPatch->AppendPatchColumn(patchCol);
		} while (patchCol.TopDelta != 0xFF);
	}
	return true;
}

bool WADLoader::LoadTextures(const std::string& texName)
{
	AssetsManager* pAssetsManager = AssetsManager::getInstance();
	int iTextureIndex = FindLumpByName(texName);

	if (iTextureIndex < 0)
	{
		return false;
	}

	if (strcmp(WAD_dirs[iTextureIndex].lump_name, texName.c_str()) != 0)
	{
		return false;
	}

	WADTextureHeader TextureHeader;
	reader.ReadTextureHeader(WAD_data, WAD_dirs[iTextureIndex].lump_offset, TextureHeader);

	WADTextureData TextureData;
	for (int i = 0; i < TextureHeader.TexturesCount; ++i)
	{
		reader.ReadTextureData(WAD_data, WAD_dirs[iTextureIndex].lump_offset + TextureHeader.pTexturesDataOffset[i], TextureData);
		pAssetsManager->AddTexture(TextureData);
		delete[] TextureData.pTexturePatch;
		TextureData.pTexturePatch = nullptr;
	}

	delete[] TextureHeader.pTexturesDataOffset;
	TextureHeader.pTexturesDataOffset = nullptr;
	return true;
}

bool WADLoader::LoadPNames()
{
	AssetsManager* pAssetsManager = AssetsManager::getInstance();
	int iPNameIndex = FindLumpByName("PNAMES");
	if (strcmp(WAD_dirs[iPNameIndex].lump_name, "PNAMES") != 0)
	{
		return false;
	}

	WADPNames PNames;
	reader.ReadPName(WAD_data, WAD_dirs[iPNameIndex].lump_offset, PNames);
	char Name[9];
	Name[8] = '\0';
	for (int i = 0; i < PNames.PNameCount; ++i)
	{
		reader.Read8Characters(WAD_data, PNames.PNameOffset, Name);
		pAssetsManager->AddPName(Name);
		PNames.PNameOffset += 8;
	}

	return true;
}

int WADLoader::FindLumpByName(const std::string& lump_name)
{
	//std::cout << "Buscando: " << lump_name << " entre " << WAD_dirs.size() << std:: endl;
	for (int i = 0; i < WAD_dirs.size(); i++)
	{
		if (WAD_dirs[i].lump_name == lump_name)
		{
			return i;
		}
	}
	return -1;
}

bool WADLoader::LoadMapData(Map* map)
{
	//Si alguna falla se aborta el proceso, 
	//	las propias funciones informan por pantalla del fallo así que no hace falta poner nada más aquí
	return (ReadMapVertex(map) && ReadMapLinedef(map) && ReadMapThing(map) && ReadMapNodes(map) && ReadMapSegs(map) && ReadMapSubs(map) && ReadMapSectors(map) && ReadMapSidedefs(map) && LoadPalette(m_pDisplayManager));
}
