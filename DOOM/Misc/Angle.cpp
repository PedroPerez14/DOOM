/*
* Name: Angle.cpp
* Date: 29/03/2021 (DD/MM/YYYY)
* Author: Víctor Martínez Lascorz (738845)
* Author: Pedro José Pérez garcía (756642)
* Coms: Clase para manejar ángulos, DOOM usa BAM (binary angle measurement), de tal forma que el valor aumenta en sentido antihorario.
*/

#include "Angle.h"
#include <math.h>
#include <corecrt_math_defines.h>

Angle::Angle() : m_Angle(0)
{
}

Angle::Angle(float angle)
{
    m_Angle = angle;
    Normalize360();
}

Angle::~Angle()
{
}

Angle Angle::operator=(const float& rhs)
{
    m_Angle = rhs;
    Normalize360();
    return *this;
}

Angle Angle::operator+(const Angle& rhs)
{
    Angle angle(m_Angle + rhs.m_Angle);
    return angle;
}

Angle Angle::operator-(const Angle& rhs)
{
    Angle angle(m_Angle - rhs.m_Angle);
    return angle;
}

Angle Angle::operator-()
{
    Angle angle(360 - m_Angle);
    return angle;
}

void Angle::Normalize360()
{
    m_Angle = fmodf(m_Angle, 360);
    if (m_Angle < 0)
        m_Angle += 360;
}

float Angle::GetValue()
{
    return m_Angle;
}

float Angle::getCos()
{
    return cosf(m_Angle * M_PI / 180.0f);
}

float Angle::getSin()
{
    return sinf(m_Angle * M_PI / 180.0f);
}

float Angle::getTan()
{
    return tanf(m_Angle * M_PI / 180.0f);
}

float Angle::getSignedValue()
{
    if (m_Angle > 180)
    {
        return m_Angle - 360;
    }

    return m_Angle;
}

Angle& Angle::operator+=(const float& rhs)
{
    m_Angle += rhs;
    Normalize360();
    return *this;
}

Angle& Angle::operator-=(const float& rhs)
{
    m_Angle -= rhs;
    Normalize360();
    return *this;
}

bool Angle::operator<(const Angle& rhs)
{
    return (m_Angle < rhs.m_Angle);
}

bool Angle::operator<(const float& rhs)
{
    return (m_Angle < rhs);
}

bool Angle::operator<=(const Angle& rhs)
{
    return (m_Angle <= rhs.m_Angle);
}

bool Angle::operator<=(const float& rhs)
{
    return (m_Angle <= rhs);
}

bool Angle::operator>(const Angle& rhs)
{
    return (m_Angle > rhs.m_Angle);
}

bool Angle::operator>(const float& rhs)
{
    return (m_Angle > rhs);
}

bool Angle::operator>=(const Angle& rhs)
{
    return (m_Angle >= rhs.m_Angle);
}

bool Angle::operator>=(const float& rhs)
{
    return (m_Angle >= rhs);
}