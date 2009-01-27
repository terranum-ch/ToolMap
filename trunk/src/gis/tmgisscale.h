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


const int tmSCALE_MARGIN = 10; // margin between image and border in full screen mode



/***************************************************************************//**
 @brief Class representing real rectangle
 @details Used for storing bounding box values
 @author Lucien Schreiber (c) CREALP 2008
 @date 14 July 2008
 *******************************************************************************/
class tmRealRect 
	{
	private:
		double GetDifferences (const double & min, const double & max)
		{
			if (wxIsSameDouble(max, min))
				return 0;
			if (max <= 0 && min >= 0)
				return max + min;
			if (max >= 0 )//&& min < max)
				return max - min;
			
			// if case isn't taken into account
			wxString sFunction = wxString::FromAscii(__FUNCTION__);
			wxString sFunctionLineError = wxString::Format( _T("%s line %d : "),
														   sFunction.c_str(), __LINE__); 
			wxString sErrMsg = wxString::Format(_T("%s values are coord min-max : %.*f - %.*f "),
												sFunctionLineError.c_str(), 2, min, 2 , max);
			wxASSERT_MSG(0,sErrMsg);
			
			return 0;
		}
		
		
	public:
		double x_min;
		double y_min;
		double x_max;
		double y_max;
		
		tmRealRect() : x_min(0.0), y_min(0.0), x_max(0.0), y_max(0.0) { }
		tmRealRect(double xmin, double ymin, double xmax, double ymax) :
		x_min(xmin), y_min(ymin), x_max(xmax), y_max(ymax) { }
		
		bool Clip (const tmRealRect & src, tmRealRect & result);
		
		bool operator==(const tmRealRect& pt) const
		{
			return wxIsSameDouble(x_min, pt.x_min) && wxIsSameDouble(y_min, pt.y_min)
			&& wxIsSameDouble(x_max, pt.x_max) && wxIsSameDouble(y_max, pt.y_max);
		}
		
		double GetWidth () {return GetDifferences(x_min, x_max);}
		double GetHeight () {return GetDifferences(y_min, y_max);}
	
	};





class tmGISScale : public wxObject
	{
	private:
		tmRealRect m_ExtentMaxLayers;
		wxRect m_ExtentWnd;
		tmRealRect m_ExtentWndReal;
		double m_PixelSize;
		wxSize m_ExtentWndMM;
		long m_UnitScale;
		
		void InitMemberValues();
		
		long ComputeUnitScale ();
		
	protected:
	public:
		tmGISScale();
		~tmGISScale();
		
		// static functions
		static tmRealRect ComputeMaxCoord (const tmRealRect & r1, const tmRealRect & r2);
		
		
		// setter and getter for layers
		void SetMaxLayersExtentAsExisting (const tmRealRect & r);
		void SetMaxLayersExtent (const tmRealRect & r){ m_ExtentMaxLayers = r;}
		tmRealRect GetMaxLayersExtent(){ return m_ExtentMaxLayers;}
		double GetLayersExtentWidth();
		double GetLayersExtentHeight();
		
		// setter and getters for windows 
		void SetWindowExtent (const wxRect & extent){m_ExtentWnd = extent;}
		wxRect GetWindowExtent () {return m_ExtentWnd;}
		tmRealRect GetWindowExtentReal() {return m_ExtentWndReal;}
		void SetWindowExtentMM (const wxSize & size) 
		{
			m_ExtentWndMM = size;
			m_UnitScale = ComputeUnitScale();
		}
		long GetActualScale (){return m_UnitScale;}
		double GetWindowRealWidth();
		double GetwindowRealHeight();
		
		// computing scale and reduction factor
		double ComputeDivFactor (wxSize wnd_extent = wxDefaultSize);
		bool ComputeMaxExtent (); 
		wxSize ComputeCenterPxWnd (double divratio, wxSize wnd_extent = wxDefaultSize);
		bool ComputeMaxExtentReal (wxSize wnd_offset = wxDefaultSize);
		
		// size windows changed -> px size change too
		void ComputeNewPixelSize ();
		
		// scale function
		void DistanceFromScale (const long & scale, double & xdist, double & ydist);
		void ComputeNewScaleExtent (const long & scale);
		
		// zoom functions
		double GetBestDivFactor (const wxRect & selected_rect);
		void ComputeNewRealZoomExtent (const wxRect & calc_wnd_extent, const wxPoint & top_left);
		
		// pan functions
		void ComputeNewRealPanExtent (const wxPoint & offsetxtop);
		
		// converting pixels - real (with inverting y axis)
		inline wxRealPoint PixelToReal (wxPoint pt)
		{
			return (wxRealPoint( m_ExtentWndReal.x_min + (pt.x * m_PixelSize),
								m_ExtentWndReal.y_max - (pt.y * m_PixelSize)));
		}
		
		inline wxPoint RealToPixel (wxRealPoint realpt)
		{
			return (wxPoint((realpt.x - m_ExtentWndReal.x_min) / m_PixelSize,
				m_ExtentWnd.height - ((realpt.y - m_ExtentWndReal.y_min) / m_PixelSize)));
			
		}
		tmRealRect PixelsToReal (const wxRect & rectpx);
		
		
		static inline double DifferenceDouble (const double & d1, const double &d2)
		{
			if (wxIsSameDouble(d1, d2))
				return 0;
			if (d1 > d2)
				return d1 - d2;
			else 
				return d2-d1;
		}
		
		inline double DistanceToReal(int iDistance)
		{
			return ((double) iDistance) / m_PixelSize;
		}
		
				
		static inline double DifferenceCoord (const double & coordmax, const double & coordmin)
		{
			if (wxIsSameDouble(coordmax, coordmin))
				return 0;
			if (coordmax <= 0 && coordmin >= 0)
				return coordmax + coordmin;
			if (coordmax >= 0 )//&& coordmin < coordmax)
				return coordmax - coordmin;
			
			// if case isn't taken into account
			wxString sFunction = wxString::FromAscii(__FUNCTION__);
			wxString sFunctionLineError = wxString::Format( _T("%s line %d : "),
														   sFunction.c_str(), __LINE__); 
			wxString sErrMsg = wxString::Format(_T("%s values are coord min-max : %.*f - %.*f "),
												sFunctionLineError.c_str(), 2, coordmin, 2 , coordmax);
			wxASSERT_MSG(0,sErrMsg);
			
			return 0;
		}
		
		static inline double RemoveFromCoord (const double & coord1, const double & value)
		{
			if (coord1 > 0)
				return coord1 - value;
			else
				return coord1 + value;
		}
		
		static inline double AppendToCoord (const double & coord1, const double & value)
		{
			if (coord1 > 0)
				return coord1 + value;
			else
				return coord1 - value;
		}
		
		// extent validity
		bool IsLayerExtentValid();
		
		
	};




#endif
