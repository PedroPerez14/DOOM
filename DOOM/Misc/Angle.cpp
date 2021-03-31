/*
* Name: Angle.cpp
* Date: 29/03/2021 (DD/MM/YYYY)
* Author: Víctor Martínez Lascorz (738845)
* Author: Pedro José Pérez garcía (756642)
* Coms: Clase para manejar ángulos, DOOM usa BAM (binary angle measurement), de tal forma que el valor aumenta en sentido antihorario.
*/

#include "Angle.h"
#include "math.h"

Angle::Angle() : m_Angle(0.0f)
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
	return Angle(rhs);
}

Angle Angle::operator+(const Angle& rhs)
{
	return Angle(m_Angle + rhs.GetValue());
}

Angle Angle::operator-(const Angle& rhs)
{
	return Angle(m_Angle - rhs.GetValue());
}

Angle Angle::operator-()
{
	return Angle(-m_Angle);
}

Angle& Angle::operator+=(const float& rhs)
{
	m_Angle += rhs;
	return *this;
}

Angle& Angle::operator-=(const float& rhs)
{
	m_Angle -= rhs;
	return *this;
}

bool Angle::operator<(const Angle& rhs)
{
	return m_Angle < rhs.GetValue();
}

bool Angle::operator<(const float& rhs)
{
	return m_Angle < rhs;
}

bool Angle::operator<=(const Angle& rhs)
{
	return m_Angle <= rhs.GetValue();
}

bool Angle::operator<=(const float& rhs)
{
	return m_Angle <= rhs;
}

bool Angle::operator>(const Angle& rhs)
{
	return m_Angle > rhs.GetValue();
}

bool Angle::operator>(const float& rhs)
{
	return m_Angle > rhs;
}

bool Angle::operator>=(const Angle& rhs)
{
	return m_Angle >= rhs.GetValue();
}

bool Angle::operator>=(const float& rhs)
{
	return m_Angle >= rhs;
}

float Angle::GetValue() const
{
	return m_Angle;
}

void Angle::Normalize360()
{
	m_Angle = fmodf(m_Angle, 360);
	if (m_Angle < 0)
		m_Angle += 360;
}
