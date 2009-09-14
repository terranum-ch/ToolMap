/***************************************************************************
								tmpercent.cpp
                    For computing percent value (progress dialog)
                             -------------------
    copyright            : (C) 2007 CREALP Lucien Schreiber 
    email                : lucien.schreiber at crealp dot vs dot ch
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

// comment doxygen

#include "tmpercent.h"

tmPercent::tmPercent()
{
	InitMemberValue();
}


tmPercent::~tmPercent()
{
	
}


tmPercent::tmPercent(long value)
{
	InitMemberValue();
	Create(value);
}

void tmPercent::Create(long value)
{
	m_TotalValue = value;
	m_Increment = m_TotalValue / 100;
}


void tmPercent::InitMemberValue()
{
	m_inited = false;
	m_TotalValue = 0;
	m_ActualValue = 0;
	m_PreviousPercent = 0;
	m_Increment = 0.0;
}

void tmPercent::SetValue (long actualvalue)
{
	m_ActualValue = actualvalue;
}

bool tmPercent::IsNewStep()
{
	int iActualPercent = (int) GetPercent();
	if (iActualPercent != m_PreviousPercent)
	{
		m_PreviousPercent = iActualPercent;
		return true;
	}
	
	return false;
}

int tmPercent::GetPercent()
{
	if (m_ActualValue > m_TotalValue)
		return 100;
	
	return (int) m_ActualValue * 100 / m_TotalValue;
}