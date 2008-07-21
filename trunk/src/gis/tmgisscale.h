/***************************************************************************
								tmgisscale.h
                    Deals with GIS scale and conversion
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


#ifndef _TM_GIS_SCALE_H_
#define _TM_GIS_SCALE_H_

// For compilers that support precompilation, includes "wx/wx.h".
#include "wx/wxprec.h"

// Include wxWidgets' headers
#ifndef WX_PRECOMP
    #include <wx/wx.h>
#endif


/***************************************************************************//**
 @brief Class representing real rectangle
 @details Used for storing bounding box values
 @author Lucien Schreiber (c) CREALP 2008
 @date 14 July 2008
 *******************************************************************************/
class tmRealRect 
	{
	public:
		double x_min;
		double y_min;
		double x_max;
		double y_max;
		
		tmRealRect() : x_min(0.0), y_min(0.0), x_max(0.0), y_max(0.0) { }
		tmRealRect(double xmin, double ymin, double xmax, double ymax) :
		x_min(xmin), y_min(ymin), x_max(xmax), y_max(ymax) { }
		
		bool operator==(const tmRealRect& pt) const
		{
			return wxIsSameDouble(x_min, pt.x_min) && wxIsSameDouble(y_min, pt.y_min)
			&& wxIsSameDouble(x_max, pt.x_max) && wxIsSameDouble(y_max, pt.y_max);
		}
	};



class tmGISScale : public wxObject
	{
	private:
		tmRealRect m_ExtentMaxLayers;
		wxRect m_ExtentWnd;
		
		void InitMemberValues();
		
	protected:
	public:
		tmGISScale();
		~tmGISScale();
		
		// static functions
		static tmRealRect ComputeMaxExtent (const tmRealRect & r1, const tmRealRect & r2);
		
		// setter and getter for layers
		void SetMaxLayersExtentAsExisting (const tmRealRect & r);
		void SetMaxLayersExtent (const tmRealRect & r){ m_ExtentMaxLayers = r;}
		tmRealRect GetMaxLayersExtent(){ return m_ExtentMaxLayers;}
		
		// setter and getters for windows 
		void SetWindowExtent (const wxRect & extent){m_ExtentWnd = extent;}
		wxRect GetWindowExtent () {return m_ExtentWnd;}
		
		// computing scale and reduction factor
		double ComputeDivFactor ();
		
		
	};




#endif
