/*
* Name: map.cpp
* Date: 06/03/2021 (DD/MM/YYYY)
* Author: Víctor Martínez Lascorz (738845)
* Author: Pedro José Pérez garcía (756642)
* Coms: Mapas de DOOM.
*/

#include "map.h"
#include "../doomdef.h"
#include "SFML/Graphics.hpp"
//#include "Windows.h"          //Descomentar para debug

Map::Map(std::string n, Player* player) : name(n), automap_info({ INT16_MIN, INT16_MAX, INT16_MIN, INT16_MAX }), m_pPlayer(player), map_index(-1)
{}

Map::~Map()
{}

std::string Map::GetName()
{
	return name;
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

void Map::addLinedef(Linedef& l)
{
    map_linedefs.push_back(l);
}

void Map::addThing(Thing& th)
{
    map_things.push_back(th);
}

void Map::addNode(BSP_Node& node)
{
    map_nodes.push_back(node);
}

void Map::addSeg(Seg& seg)
{
    map_segs.push_back(seg);
}

void Map::addSsect(Subsector& ssec)
{
    map_subsecs.push_back(ssec);
}

Linedef Map::getLinedef(const int& n)
{
	return map_linedefs[n];
}

Vertex Map::getVertex(const int& n)
{
	return map_verts[n];
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

void Map::Automap(sf::RenderWindow* r_window)
{
    AutomapPlayer(r_window);
    AutomapWalls(r_window);
    //AutomapNodes(r_window);   //TODO rework
    RenderBSPNodes(r_window);
}

//WIP, me la he inventado. habrá que modificarla mucho en el futuro, o incluso borrarla
void Map::LoadPlayer()
{
    for (int i = 0; i < getThingsSize(); i++)
    {
        if (map_things[i].Type == ePLAYER1START)
        {
            std::cout << "Encontrado spawn del jugador en X= " << map_things[i].XPos << ",Y= " << map_things[i].YPos << std::endl;
            m_pPlayer->SetXPos(map_things[i].XPos);
            m_pPlayer->SetYPos(map_things[i].YPos);
            m_pPlayer->SetAngle(map_things[i].Angle);
        }
    }
}

void Map::RenderBSPNodes(sf::RenderWindow* r_window)
{
    RenderBSPNodes(map_nodes.size() - 1, r_window);
}

void Map::AutomapPlayer(sf::RenderWindow* r_window)
{
    int w = r_window->getView().getSize().x;
    int h = r_window->getView().getSize().y;
    float x_pos, y_pos;

    convertToScreenCords(m_pPlayer->GetXPos(), x_pos, m_pPlayer->GetYPos(), y_pos, w, h, SCREENWIDTH, SCREENHEIGHT);
    sf::CircleShape player_triangle(1.0f, 3);   //Variar tamaño al escalar la pantalla
    player_triangle.setFillColor(sf::Color::Green);
    sf::Vector2f v = sf::Vector2f(x_pos, y_pos);
    player_triangle.setPosition(v);
    r_window->draw(player_triangle);
}

void Map::AutomapWalls(sf::RenderWindow* r_window)
{
    const float scaleFactor = 15.0f;    //TODO está bien aquí?
    int w = r_window->getView().getSize().x;
    int h = r_window->getView().getSize().y;

    for (int i = 0; i < getLinedefsSize(); i++)
    {
        Vertex v = getVertex(getLinedef(i).vert1);
        Vertex v2 = getVertex(getLinedef(i).vert2);
        float x_pos, y_pos, x_pos2, y_pos2;
        convertToScreenCords(v.x, x_pos, v.y, y_pos, w, h, SCREENWIDTH, SCREENHEIGHT);
        convertToScreenCords(v2.x, x_pos2, v2.y, y_pos2, w, h, SCREENWIDTH, SCREENHEIGHT);
        sf::Vertex vertex[] = { 
            sf::Vector2f(x_pos, y_pos),
            sf::Vector2f(x_pos2, y_pos2)
        };
        r_window->draw(vertex, 2, sf::Lines);
    }
    /*
    sf::Vertex point[] = { (sf::Vector2f(10, 10))
    , (sf::Vector2f(10, r_window->getView().getSize().y - 10)) };
    r_window->draw(point, 2, sf::Lines);
    */ //Dejo esto aquí como recordatorio de que soy gilipollas
}

void Map::AutomapNodes(sf::RenderWindow* r_window)
{
    //TODO esta función sobrará más adelante, solo saca los dos primeros nodos
    BSP_Node node = map_nodes[map_nodes.size() - 1];

    const float scaleFactor = 15.0f;    //TODO está bien aquí?
    int w = r_window->getSize().x;
    int h = r_window->getSize().y;
    float x1, y1, x2, y2;               //Para las coordenadas de los rectángulos
    sf::VertexArray line(sf::Lines, 2);

    /* Dibujar nodo derecho en verde */
    convertToScreenCords(node.RightBoxLeft, x1, node.RightBoxTop, y1, w, h, SCREENWIDTH, SCREENHEIGHT);
    convertToScreenCords(node.RightBoxRight, x2, node.RightBoxBottom, y2, w, h, SCREENWIDTH, SCREENHEIGHT);
    sf::RectangleShape rectangle(sf::Vector2f(x2 - x1, y2 - y1));
    rectangle.setFillColor(sf::Color::Transparent);
    rectangle.setOutlineColor(sf::Color::Green);
    rectangle.setOutlineThickness(1.0f);
    rectangle.setPosition(sf::Vector2f(x1, y1));
    r_window->draw(rectangle);

    /* Dibujar nodo izquierdo en rojo */
    convertToScreenCords(node.LeftBoxLeft, x1, node.LeftBoxTop, y1, w, h, SCREENWIDTH, SCREENHEIGHT);
    convertToScreenCords(node.LeftBoxRight, x2, node.LeftBoxBottom, y2, w, h, SCREENWIDTH, SCREENHEIGHT);
    rectangle = sf::RectangleShape(sf::Vector2f(x2 - x1, y2 - y1));
    rectangle.setFillColor(sf::Color::Transparent);
    rectangle.setOutlineColor(sf::Color::Red);
    rectangle.setOutlineThickness(1.0f);
    rectangle.setPosition(sf::Vector2f(x1, y1));
    r_window->draw(rectangle);

    /* Dibujar línea de partición en azul */
    convertToScreenCords(node.XPartition, x1, node.YPartition, y1, w, h, SCREENWIDTH, SCREENHEIGHT);
    line[0].position = sf::Vector2f(x1, y1);
    line[0].color = sf::Color::Blue;
    convertToScreenCords(node.XPartition + node.XPartDir, x1, node.YPartition + node.YPartDir, y1, w, h, SCREENWIDTH, SCREENHEIGHT);
    line[1].position = sf::Vector2f(x1, y1);
    line[1].color = sf::Color::Blue;
    r_window->draw(line);
}

int Map::getMapIndex()
{
    return map_index;
}

void Map::setMapIndex(int idx)
{
    map_index = idx;
}

void Map::convertToScreenCords(const float& Xin, float& Xout, const float& Yin, float& Yout, const float& currentwidth, const float& currentheight, const float& width, const float& height)
{
    //Magia, tú créetelo y disfruta
    Xout = 5.0f + (Xin + (float)-automap_info.minX) / (float)(15.0f * width / currentwidth);
    Yout = currentheight - 5.0f - (Yin + (float)-automap_info.minY) / (float)(15.0f * height / currentheight);
    //El 3+... y el h-1-... son porque la pantalla va de 0 a h-1, no de 1 a h. El 3 no es un 1 por arreglar imprecisiones pequeñas
}

bool Map::IsPointOnLeftSide(int XPosition, int YPosition, int iNodeID)
{
    int dx = XPosition - map_nodes[iNodeID].XPartition;
    int dy = YPosition - map_nodes[iNodeID].YPartition;

    return (((dx * map_nodes[iNodeID].YPartDir) - (dy * map_nodes[iNodeID].XPartDir)) <= 0);    //Prod. vectorial
}

void Map::RenderBSPNodes(int16_t nodeID, sf::RenderWindow* r_window)
{
    //std::cout << "RECORRIENDO NODO: " << nodeID << std::endl;
    //Comprobar con la máscara si es un nodo hoja == subsector (el que buscamos)
    if((int16_t)(nodeID & SUBSECTORIDENTIFIER))
    {
        //std::cout << "ENCONTRADO SUBSECTOR HOJA: " << (int16_t)(nodeID & (~SUBSECTORIDENTIFIER)) << std::endl;
        RenderSubsector((int16_t)(nodeID & (~SUBSECTORIDENTIFIER)), r_window);
        return;
        //hay que volver a hacer el casteo porque si no saca -326XX en vez del ID que debería y explota
    }

    if (IsPointOnLeftSide(m_pPlayer->GetXPos(), m_pPlayer->GetYPos(), nodeID))
    {
        RenderBSPNodes(map_nodes[nodeID].LeftChild, r_window);
        RenderBSPNodes(map_nodes[nodeID].RightChild, r_window);
        //izquierda y luego derecha, de cerca a lejos (creo que es por eso)
    }
    else
    {
        RenderBSPNodes(map_nodes[nodeID].RightChild, r_window);
        RenderBSPNodes(map_nodes[nodeID].LeftChild, r_window);
        //derecha y luego izquierda, de cerca a lejos (creo que es por eso)
    }


}

void Map::RenderSubsector(int subsectorID, sf::RenderWindow* r_window)
{
    const float scaleFactor = 15.0f;    //TODO la pongo en algún .h o como solo la uso para la preview 2d la dejo así?
    Subsector subsector = map_subsecs[subsectorID];
    int w = r_window->getView().getSize().x;
    int h = r_window->getView().getSize().y;

    sf::Color color(rand() % 255, rand() % 255, rand() % 255);

    for (int i = 0; i < subsector.seg_count; i++)
    {   
        //Para todos los segmentos del ssec pinto de un color aleatorio las paredes
        Seg seg = map_segs[subsector.first_segID + i];
        Vertex v = getVertex(seg.vert1);
        Vertex v2 = getVertex(seg.vert2);
        Angle a1, a2;   //para invocar a clipvertexesinFOV()

        if (m_pPlayer->ClipVertexesInFOV(v, v2, a1, a2))
        {
            float x_pos, y_pos, x_pos2, y_pos2;
            convertToScreenCords(v.x, x_pos, v.y, y_pos, w, h, SCREENWIDTH, SCREENHEIGHT);
            convertToScreenCords(v2.x, x_pos2, v2.y, y_pos2, w, h, SCREENWIDTH, SCREENHEIGHT);
            sf::Vertex vertex[] = {
                sf::Vertex(sf::Vector2f(x_pos, y_pos), color),
                sf::Vertex(sf::Vector2f(x_pos2, y_pos2), color)
            };

            r_window->draw(vertex, 2, sf::Lines);
        }   
    }
    //Sleep(250);           //Para ver que funciona bien
    //r_window->display();  //Descomentar arriba del todo el include de windows.h
}
