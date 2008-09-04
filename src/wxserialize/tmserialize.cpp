/***************************************************************************
								tmserialize.cpp
                    for serializating object in a simple way
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

#include "tmserialize.h"


tmSerialize::tmSerialize(wxInputStream &stream) : 
m_idstr(stream),
m_otmp(&m_tmpostr),
m_odstr(m_otmp),
m_itmp(m_tmpistr)
{
	m_writeMode = FALSE;
	//m_idstr = stream;
	
	// all ok, use this stream
    if(stream.IsOk())
    {
		
		
	}		
	
}


tmSerialize::tmSerialize(wxOutputStream & stream) : 
 m_odstr(stream),
 m_itmp(m_tmpistr),
 m_idstr(m_itmp),
 m_otmp(&m_tmpostr)
{
	m_writeMode = TRUE;
	//m_odstr = stream;
	if (!m_odstr.IsOk())
		wxLogDebug(_T("Stream not ok"));

}


bool tmSerialize::CanStore()
{
    // are we in an ok state?
    if(m_writeMode)
    {
        return (TRUE);
    }
    else
        wxLogDebug(_T("Not able to write"));
	
	return false;
}


tmSerialize & tmSerialize::operator <<(bool value)
{
	if (m_writeMode)
		m_odstr.Write((void *)&value, sizeof(bool));
	
	return *this;
}


tmSerialize & tmSerialize::operator << (wxString value)
{
	size_t len = value.Len();
	if(len > 0)
	{
		// we write in unicode even when we are not compiled
		// in unicode. This means writing pairs of wxInt16
		// bytes. We need optional conversion in wxUint16
		for(unsigned int i = 0; i < len; i++)
			SaveUint16((wxUint16)value.GetChar(i));
	}
	return *this;
}

void tmSerialize::SaveUint16(wxUint16 value)
{
	wxUint16 tmpval = wxUINT16_SWAP_ON_LE(value);
	if(CanStore())
        m_odstr.Write(&tmpval, sizeof(wxUint16));
}


