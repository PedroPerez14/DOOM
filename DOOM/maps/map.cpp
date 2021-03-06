/*
* Name: map.cpp
* Date: 06/03/2021 (DD/MM/YYYY)
* Author: V?ctor Mart?nez Lascorz (738845)
* Author: Pedro Jos? P?rez garc?a (756642)
* Coms: Mapas de DOOM.
*/

#pragma once

#include "map.h"
#include "../doomdef.h"
#include "SFML/Graphics.hpp"
#include "../Enemy/Enemy.h"
#include "../Enemy/Soldier.h"
//#include "Windows.h"          //Descomentar para debug

Map::Map(std::string n, Player* player) : name(n), automap_info({ INT16_MIN, INT16_MAX, INT16_MIN, INT16_MAX }), m_pPlayer(player), map_index(-1)
{
    m_pSectors = new std::vector<WADSector>();
    m_pSidedefs = new std::vector<WADSidedef>();
    m_pLinedefs = new std::vector<WADLinedef>();
    m_pSegs = new std::vector<WADSeg>();
}

Map::~Map()
{}

std::string Map::GetName()
{
	return name;
}

void Map::Init()
{
    BuildSectors();
    BuildSidedefs();
    BuildLinedefs();
    BuildSegs();
}

void Map::addVertex(Vertex& v)
{
    if (v.x > automap_info.maxX)
        automap_info.maxX = v.x;
    if (v.y > automap_info.maxY)
        automap_info.maxY = v.y;
    if (v.x < automap_info.minX)
        automap_info.minX = v.x;
    if (v.y < automap_info.minY)
        automap_info.minY = v.y;

	map_verts.push_back(v);
}

void Map::addLinedef(WADLinedef& l)
{
    m_pLinedefs->push_back(l);
}

void Map::addSidedef(WADSidedef& sd)
{
    m_pSidedefs->push_back(sd);
}

void Map::addThing(Thing& th)
{
    map_things.push_back(th);
}

void Map::addNode(BSP_Node& node)
{
    map_nodes.push_back(node);
}

void Map::addSeg(WADSeg& seg)
{
    m_pSegs->push_back(seg);
}

void Map::addSsect(Subsector& ssec)
{
    map_subsecs.push_back(ssec);
}

void Map::addSect(WADSector& sec)
{
    m_pSectors->push_back(sec);
}

Linedef Map::getLinedef(const int& n)
{
	return map_linedefs[n];
}

Vertex Map::getVertex(const int& n)
{
	return map_verts[n];
}

BSP_Node Map::getNode(const int& n)
{
    return map_nodes[n];
}

Subsector Map::getSSec(const int& n)
{
    return map_subsecs[n];
}

Seg Map::getSeg(const int& n)
{
    return map_segs[n];
}

int Map::getLinedefsSize()
{
	return map_linedefs.size();
}

int Map::getVertexSize()
{
	return map_verts.size();
}

int Map::getThingsSize()
{
    return map_things.size();
}

int Map::getNodesSize()
{
    return map_nodes.size();
}

int Map::getSubsecSize()
{
    return map_subsecs.size();
}

void Map::LoadPlayer()
{
    for (int i = 0; i < getThingsSize(); i++)
    {
        if (map_things[i].Type == ePLAYER1START)
        {
            //std::cout << "Encontrado spawn del jugador en X= " << map_things[i].XPos << ",Y= " << map_things[i].YPos << std::endl;
            m_pPlayer->SetXPos(map_things[i].XPos);
            m_pPlayer->SetYPos(map_things[i].YPos);
            m_pPlayer->SetAngle(map_things[i].Angle);
        }
    }
}

bool Map::IsPointOnLeftSide(int XPosition, int YPosition, int nodeID)
{
    int dx = XPosition - getNode(nodeID).XPartition;
    int dy = YPosition - getNode(nodeID).YPartition;

    return (((dx * getNode(nodeID).YPartDir) - (dy * getNode(nodeID).XPartDir)) <= 0);    //Prod. vectorial
}

float Map::getPlayerSubsecHeight(int16_t& subsector_id)
{
    int16_t ssecID = (int16_t)map_nodes.size() - 1;
    while (!((int16_t)(ssecID & SUBSECTORIDENTIFIER)))
    {
        if (IsPointOnLeftSide(m_pPlayer->GetXPos(), m_pPlayer->GetYPos(), ssecID))
        {
            ssecID = getNode(ssecID).LeftChild;
        }
        else
        {
            ssecID = getNode(ssecID).RightChild;
        }
    }
    Subsector& subsector = map_subsecs[(int16_t)(ssecID & (~SUBSECTORIDENTIFIER))];
    Seg& seg = map_segs[subsector.first_segID];
    subsector_id = (int16_t)(ssecID & (~SUBSECTORIDENTIFIER));
    return seg.pRightSector->FloorHeight;
}

float Map::getEnemySubsecHeight(float x_, float y_, int16_t& subsector_id)
{
    int16_t ssecID = (int16_t)map_nodes.size() - 1;
    while (!((int16_t)(ssecID & SUBSECTORIDENTIFIER)))
    {
        if (IsPointOnLeftSide(x_, y_, ssecID))
        {
            ssecID = getNode(ssecID).LeftChild;
        }
        else
        {
            ssecID = getNode(ssecID).RightChild;
        }
    }
    Subsector& subsector = map_subsecs[(int16_t)(ssecID & (~SUBSECTORIDENTIFIER))];
    Seg& seg = map_segs[subsector.first_segID];
    subsector_id = (int16_t)(ssecID & (~SUBSECTORIDENTIFIER));
    return seg.pRightSector->FloorHeight;
}

int Map::getMapIndex()
{
    return map_index;
}

void Map::setMapIndex(int idx)
{
    map_index = idx;
}

Automap_info Map::getAutomapInfo()
{
    return automap_info;
}

void Map::BuildSectors()
{
    Sector sec;
    WADSector w_sec;
    for (int i = 0; i < m_pSectors->size(); i++)
    {
        w_sec = m_pSectors->at(i);

        sec.FloorHeight = w_sec.FloorHeight;
        sec.CeilingHeight = w_sec.CeilingHeight;
        strncpy_s(sec.FloorTexture, w_sec.FloorTexture, 8);
        sec.FloorTexture[8] = '\0';
        strncpy_s(sec.CeilingTexture, w_sec.CeilingTexture, 8);
        sec.CeilingTexture[8] = '\0';
        sec.Lightlevel = w_sec.Lightlevel;
        sec.Type = w_sec.Type;
        sec.Tag = w_sec.Tag;

        map_sectors.push_back(sec);
    }
    delete m_pSectors;
    m_pSectors = nullptr;
}

void Map::BuildSidedefs()
{
    WADSidedef wad_sidedef;
    Sidedef sidedef;
    for(int i = 0; i < m_pSidedefs->size(); i++)
    {
        wad_sidedef = m_pSidedefs->at(i);
        sidedef.XOffset = wad_sidedef.XOffset;
        sidedef.YOffset = wad_sidedef.YOffset;
        strncpy_s(sidedef.UpperTexture, wad_sidedef.UpperTexture, 8);
        sidedef.UpperTexture[8] = '\0';
        strncpy_s(sidedef.LowerTexture, wad_sidedef.LowerTexture, 8);
        sidedef.LowerTexture[8] = '\0';
        strncpy_s(sidedef.MiddleTexture, wad_sidedef.MiddleTexture, 8);
        sidedef.MiddleTexture[8] = '\0';
        sidedef.pSector = &map_sectors[wad_sidedef.SectorID];

        map_sidedefs.push_back(sidedef);
    }
    delete m_pSidedefs;
    m_pSidedefs = nullptr;
}

void Map::BuildLinedefs()
{
    WADLinedef wad_ld;
    Linedef ld;
    for (int i = 0; i < m_pLinedefs->size(); i++)
    {
        wad_ld = m_pLinedefs->at(i);

        ld.vert1 = &map_verts[wad_ld.vert1];
        ld.vert2 = &map_verts[wad_ld.vert2];
        ld.flags = wad_ld.flags;
        ld.line_type = wad_ld.line_type;
        ld.sector_tag = wad_ld.sector_tag;

        ld.sidedef_l = wad_ld.sidedef_l == 0xFFFF ? nullptr : &map_sidedefs[wad_ld.sidedef_l];
        ld.sidedef_r = wad_ld.sidedef_r == 0xFFFF ? nullptr : &map_sidedefs[wad_ld.sidedef_r];

        map_linedefs.push_back(ld); //Me lo estaba dejando y me ha tirado excepciones durante un buen rato :DDD
    }
    delete m_pLinedefs;
    m_pLinedefs = nullptr;
}

void Map::BuildSegs()
{
    Seg seg;
    WADSeg wad_seg;
    for (int i = 0; i < m_pSegs->size(); i++)
    {
        wad_seg = m_pSegs->at(i);

        seg.vert1 = &map_verts[wad_seg.vert1];
        seg.vert2 = &map_verts[wad_seg.vert2];
        seg.angle = ((float)(wad_seg.angle << 16) * 8.38190317e-8); //Pasar de ?ngulos binarios (BAMS) a float
        seg.pLinedef = &map_linedefs[wad_seg.linedef_index];
        seg.dir = wad_seg.dir;
        seg.offset = (float)(wad_seg.offset << 16) / (float)(1 << 16);
        
        //Ahora cogemos los sidedefs izquierdo y derecho (en este caso sectores) como punteros
        Sidedef* pRightSidedef;
        Sidedef* pLeftSidedef;

        if (seg.dir)
        {
            pRightSidedef = seg.pLinedef->sidedef_l;
            pLeftSidedef = seg.pLinedef->sidedef_r;
        }
        else
        {
            pRightSidedef = seg.pLinedef->sidedef_r;
            pLeftSidedef = seg.pLinedef->sidedef_l;
        }

        if (pRightSidedef)
        {
            seg.pRightSector = pRightSidedef->pSector;
        }
        else
        {
            seg.pRightSector = nullptr;
        }

        if (pLeftSidedef)
        {
            seg.pLeftSector = pLeftSidedef->pSector;
        }
        else
        {
            seg.pLeftSector = nullptr;
        }
        //Finalmente lo metemos en el vector
        map_segs.push_back(seg);
    }
    delete m_pSegs;
    m_pSegs = nullptr;
}


std::vector<Thing> Map::getThings() {
    return map_things;
}