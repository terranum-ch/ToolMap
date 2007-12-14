 /*******************************************************//**
@file ListGenReport.h
@brief wxListCtrl derived class for dealing with function 
parameters (header)
@details This file contain the description of the class 
ListGenReport. This class is derived from the wxListCtrl 
class and is used for managing the list control containing
all parameters and return values
@bug use this code to allow the list to work in mac :
wxSystemOptions::SetOption( wxT("mac.listctrl.always_use_generic"), 1 ); 
@author Lucien Schreiber (c) CREALP 2007
@date 15 November 2007
 ***********************************************************/

#ifndef LISTGENREPORT_H
#define LISTGENREPORT_H

// For compilers that support precompilation, includes "wx/wx.h".
#include "wx/wxprec.h"

// Include wxWidgets' headers
#ifndef WX_PRECOMP
    #include <wx/wx.h>
#endif

#include <wx/listctrl.h>
#include <wx/textdlg.h>
#include <wx/arrstr.h>
#include <wx/dnd.h>
#include <wx/menu.h>

/*!
 * Control identifiers for menu
 */

////@begin control identifiers
#define ID_MENU_MOVE_TOP 20000
#define ID_MENU_MOVE_UP 20001
#define ID_MENU_MOVE_DOWN 20002
#define ID_MENU_MOVE_BOTTOM 20003
////@end control identifiers

class ListGenMenu; // forward declaration
class ListGenDialog; // forward declaration

/*******************************************************//**
 @brief Manage a report list
 @details This class is derived from wxListCtrl and is mainly 
 used for managing all sort of report list. This class is 
 intended to be derivated
 This is a graphical class 
 @author Lucien Schreiber (c) CREALP 2007
 @date 15 November 2007
 ***********************************************************/
class ListGenReport : public wxListCtrl
	{		
	protected:
		ListGenMenu  * m_ListContextMenu;
			
		void CreateColumns(wxArrayString * pColsName, wxArrayInt * pColsSize=NULL);
		 
		
		// events
		virtual void OnDoubleClickItem (wxListEvent & event);
		void OnPressBackSpace (wxListEvent & event);
		void OnContextMenu (wxListEvent & event);
		void OnMoveItemInList (wxCommandEvent & event);
		
		void OnStartDrag (wxListEvent & event);
		void OnInit();
		
	public:
		/*********************************************//**
		 @brief Constructor for derived class
		 @details This is the basic constructor mainly used
		 for derivated class. This constructor
		 dosen't invoke the CreateColumns()
		 method and therfore the created list
		 will not contain any columns.
		 @param parent Parent of the list
		 @param id Id of the List control
		 @author Lucien Schreiber (c) CREALP
		 @date 19 November 2007
		 *************************************************/
		ListGenReport(wxWindow * parent, wxWindowID id, wxSize size=wxDefaultSize);
		/*********************************************//**
		 @brief Constructor
		 @details This constructor invoke the
		 CreateColumns() method and therfore the
		 created list will contain columns passed
		 as parameters (see pColsName and pColsSize)
		 @param parent Parent of the list
		 @param id Id of the List Control
		 @param pColsName pointer to an String array
		 containing columns header. The
		 number of constructed columns is
		 dependant of the number of items
		 in this array
		 @param pColsSize=NULL pointer to an array of
		 integer containing the
		 width for each columns. If
		 no values are specified
		 then a default size of 100
		 @author Lucien Schreiber (c) CREALP
		 @date 19 November 2007
		 *************************************************/
		ListGenReport (wxWindow * parent, wxWindowID id, wxArrayString * pColsName, wxArrayInt * pColsSize=NULL, wxSize size=wxDefaultSize);
		/*********************************************//**
		 @brief Desctructor
		 @details Do nothing for the moment
		 @param parent Parent of the list
		 @author Lucien Schreiber (c) CREALP
		 @date 19 November 2007
		 *************************************************/
		~ListGenReport();
		static const int ID_LIST;
		/*********************************************//**
		 @brief Add an item to the list
		 @details This function adds an item to the list
		 and set the value of this item to the
		 specified value (myValue)
		 @param myValue The value of the new item
		 @author Lucien Schreiber (c) CREALP
		 @date 19 November 2007
		 *************************************************/
		void AddItemToList(wxString myValue, int iPosition=-1);
		/*********************************************//**
		 @brief Get the text of a specified column for a
		 specified item
		 @details Retrive the text of a specified item
		 (iItem) for a specified column (iCol)
		 @return The string contained in the
		 specified position (empty string
		 if null)
		 @param iItem the zero based index of the item in
		 the list
		 @param iCol the zero based column
		 @author Lucien Schreiber (c) CREALP
		 @date 19 November 2007
		 *************************************************/
		wxString GetItemColText (int iItem, int iCol);
		/*********************************************//**
		 @brief Set the text of a specified column for a
		 specified item
		 @details Allow to change the text of a specified
		 item in an easy way 
		 @return return true if the item exist
		 @param iItem the item to change
		 @param iCol the zero based column 
		 @param text the new text to set
		 @author Lucien Schreiber (c) CREALP
		 @date 19 November 2007
		 *************************************************/
		bool SetItemText (int iItem, int iCol, wxString text);
		/*********************************************//**
		 @brief Get the first item selected
		 @return  the zero based index of the first item
		 selected
		 @author Lucien Schreiber (c) CREALP 2007
		 @date 20 November 2007
		 *************************************************/
		long GetSelectedItem ();
		/*****************************************************************//**
		 @brief Move item in the list
		 @details This function is intended to move item between two position
		 in the list. This function could be used for exemple in response to
		 a DnD event or to a popup menu
		 @param item the zero based index of the item to move
		 @param iNewPos=0 The new position for the object iItem (default is 0,
		 top of the list)
		 @author Lucien Schreiber (c) CREALP 2007
		 @date 23 November 2007
		 *********************************************************************/
		void MoveItem (int iItem, int iNewPos=0);
		
		bool DataToList(ListGenDialog * pdialog,  wxArrayString & myValues);
		
		bool DeleteSelectedItem();
		
		int GetAllDataAsStringArray(wxArrayString & myStringArray, long index);
		
		bool ItemExist(long index);
		
		bool EditDataToList (const wxArrayString & myValue, int index =-1);
		
		DECLARE_EVENT_TABLE();
	};

/*****************************************************************//**
 @brief contextual menu for ListGenReport
 @details This class manage the popup menu for ListGenReport, the
 default menu allows to move item in the list. For an other menu
 derive this class and implement the virtual function
 CreateContextMenu(). Then to get a pointer to the generated menu call
 the GetTheMenu() function. All events (except the defauts move item)
 must be implemented in the parent ListGenReport.
 @author Lucien Schreiber (c) CREALP 2007
 @date 23 November 2007
 *********************************************************************/
class ListGenMenu : public wxMenu
	{
		protected :
		void MenuInit();
		virtual wxMenu   *CreateContextMenu();
		
		wxMenu *m_ContextMenu;		
				
		public :
		ListGenMenu();
		ListGenMenu(const wxString& title, long style = 0);
		~ListGenMenu() {delete m_ContextMenu;}
		
		wxMenu * GetTheMenu() {return m_ContextMenu;}
		
	};


class ListGenDialog : public wxDialog 
	{
	protected:
		//virtual void Init(){;}
		//virtual void CreateDlgControls() {;}
	public:
		ListGenDialog(){;}
		//ListGenDialog( wxWindow* parent, 
		//			 wxWindowID id ,
		//			 const wxString& caption,
		//			 const wxPoint& pos,
		//			 const wxSize& size,
		//			 long style);

		~ListGenDialog() {;}
		
		
		
		virtual  void GetDlgData( wxArrayString & myStringArray) = 0;
		virtual void SetDlgData(wxArrayString & myStringArray)=0;
	};

#endif

