/***************************************************************************
							wxRubberBand.h
				Definition of a Rubber Band class 
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

/***************************************************************************//**
 @file wxrubberband.h
 @brief Definition of a Rubber Band class
 @details This is mainly used for drawing a selection box arround
 something
 @version 1.1
 @author Lucien Schreiber (c) CREALP 2008
 @date 25 July 2008
 *******************************************************************************/


#ifndef __WXRUBBERBAND_H__
#define __WXRUBBERBAND_H__

// For compilers that support precompilation, includes "wx/wx.h".
#include "wx/wxprec.h"

// Include wxWidgets' headers
#ifndef WX_PRECOMP
#include <wx/wx.h>
#endif

#include <wx/dcclient.h>

///\brief Class used for drawing a rubber band.
/** This class was created for drawing a selection box 
* it is a very simple one. 
* We draw a rectangle using lines despite rectangle to avoid flickering
* \warning For the moment only positive rectangle are supported
*/
class wxRubberBand : public wxObject
{
private:
	/** \brief [PRIVATE MEMBER] used for storing last rectangle coordinates */
	wxRect theOldRubberRect;
	wxPoint m_OldPosStart;
	wxPoint m_OldPosEnd;
	/** \brief [PRIVATE MEMBER] we store the pen */
	wxPen thePen;
	/** \brief [PRIVATE MEMBER] used for window storing */
	wxWindow * theWnd;
	
	/** \brief [PRIVATE FUNCTION] used during wxRubberBand initialisation */
	void RubberInitValues ();
	
	/** \brief [PRIVATE FUNCTION] used for lines drawing 
	 *
	 * We draw lines and not rectangle directely to avoid filckering. This function is called
	 * from the SetGeometry One*/
	//void DrawRectangleLines (wxDC *MyDC,int x, int y, int width, int height);
	
	/** \brief [PRIVATE FUNCTION] Line verification against inverse 
	 *selection rectangle 
	 *
	 * The following verification are made : x et y must not be < 0
	 * width and height must be > 0 
	 * \return TRUE if all verifications are good*/
	//bool VerifyRubberLines (int x, int y, int width, int height);
	
	void EraseLastRubber (wxDC * dc);
	

	
	
	
	
public:
	/** \brief default constructor */
	wxRubberBand();
	/**\brief wxRubberBand Constructor */
	wxRubberBand(wxWindow * wnd);
	/**\brief wxRubberBand Destructor */
	~wxRubberBand();
	/** \brief Resize the rubber band */
	//void SetGeometry (int x, int y, int width, int height);
	/** \brief Resize the rubber band using two wxPoint */
	void SetGeometry (const wxPoint & posStart,const  wxPoint & posEnd);
	/** \brief Set the default pen */
	void SetPen ();
	/** \brief Set an specific pen */
	void SetPen (wxPen &pen);
	/** \brief Clear theOldRubberRect needed each time
	 * the mouse button is released
	 *
	 * This function is needed when used try to change 
	 * the pen for exemple. */
	void ClearOldRubberRect ();
	
	bool IsSelectedRectangleValid();
	bool IsSelectedRectanglePositive();
	wxRect GetSelectedRectangle();
	
	
	
	
};


#endif
