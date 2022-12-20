/*******************************************************/ /**
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
#include <wx/wxprec.h>

// Include wxWidgets' headers
#ifndef WX_PRECOMP
#include <wx/wx.h>
#endif

#include <wx/arrstr.h>
#include <wx/dnd.h>
#include <wx/listctrl.h>
#include <wx/menu.h>
#include <wx/textdlg.h>

#include "../core/textparser.h"

/*!
 * Control identifiers for menu
 */

////@begin control identifiers
#define ID_MENU_MOVE_TOP 20000
#define ID_MENU_MOVE_UP 20001
#define ID_MENU_MOVE_DOWN 20002
#define ID_MENU_MOVE_BOTTOM 20003
////@end control identifiers

class ListGenMenu;    // forward declaration
class ListGenDialog;  // forward declaration

enum LIST_FIELD_TYPE {
    FIELD_STRING = 0,
    FIELD_NUMBER = 1,
    FIELD_DATE = 2
};

/*******************************************************/ /**
  @brief Manage a report list
  @details This class is derived from wxListCtrl and is mainly
  used for managing all sort of report list. This class is
  intended to be derivated
  This is a graphical class
  @author Lucien Schreiber (c) CREALP 2007
  @date 15 November 2007
  ***********************************************************/
class ListGenReport : public wxListCtrl {
  protected:
    ListGenMenu* m_ListContextMenu;
    TextParser* m_ImportParser;

    void CreateColumns(wxArrayString* pColsName, wxArrayInt* pColsSize = nullptr);

    // events
    virtual void OnDoubleClickItem(wxListEvent& event);

    virtual void OnPressBackSpace(wxListEvent& event);

    virtual void OnContextMenu(wxListEvent& event);

    void OnMoveItemInList(wxCommandEvent& event);

    virtual void OnSortColumns(wxListEvent& event);

    // void OnStartDrag (wxListEvent & event); // not implemented now maybe layer...
    void OnInit();

  public:
    void SortListItem(int x_col, int low, int high, int typecol, bool bAscending = TRUE);

    void SwapRow(int x_row1, int x_row2);

    virtual int Compare(int iColumnCompareType, const wxString& x_strValue1, const wxString& x_strValue2,
                        bool bAscending);

    /*********************************************/ /**
   @brief Constructor for derived class
   @details This is the basic constructor mainly used
   for derivated class. This constructor
   dosen't invoke the CreateColumns()
   method and therfore the created list
   will not contain any columns.
   @param parent Parent of the list
   @param id Id of the List control
   @param size Size of the listctrl
   @author Lucien Schreiber (c) CREALP
   @date 19 November 2007
   *************************************************/
    ListGenReport(wxWindow* parent, wxWindowID id, wxSize size = wxDefaultSize);

    /*********************************************/ /**
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
   @param pColsSize pointer to an array of
   integer containing the
   width for each columns. If
   no values are specified
   then a default size of 100 will be used
   @param size the size of the listctrl
   @author Lucien Schreiber (c) CREALP
   @date 19 November 2007
   *************************************************/
    ListGenReport(wxWindow* parent, wxWindowID id, wxArrayString* pColsName, wxArrayInt* pColsSize = nullptr,
                  wxSize size = wxDefaultSize, long style = wxLC_REPORT);

    /*********************************************/ /**
   @brief Desctructor
   @details Do nothing for the moment
   @author Lucien Schreiber (c) CREALP
   @date 19 November 2007
   *************************************************/
    ~ListGenReport();

    static const int ID_LIST;

    /***************************************************************************/ /**
   @brief Add an item to the list
   @details This function adds an item to the list and set the value of this item
   to the string value specified in parameter myValue.
   @param myValue The string value of the new item
   @param iPosition The position where we should insert item in the list, default
   is -1, it means at the end of the list
   @author Lucien Schreiber (c) CREALP 2007
   @date 18 December 2007
   *******************************************************************************/
    void AddItemToList(wxString myValue, int iPosition = -1);

    /*********************************************/ /**
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
    wxString GetItemColText(int iItem, int iCol);

    /*********************************************/ /**
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
    bool SetItemText(int iItem, int iCol, wxString text);

    /*********************************************/ /**
   @brief Get the first item selected
   @return  the zero based index of the first item
   selected
   @author Lucien Schreiber (c) CREALP 2007
   @date 20 November 2007
   *************************************************/
    long GetSelectedItem();

    void SetSelectedItem(long index);

    /***************************************************************************/ /**
   @brief Get all selected item
   @details Return an array of long containing all selected items in the list
   (only usefull if list style isn't wxLC_SINGLE_SEL)
   @param results an Array of long wich will be filled with selected items
   @return  the number of items selected
   @author Lucien Schreiber (c) CREALP 2007
   @date 14 March 2008
   *******************************************************************************/
    int GetAllSelectedItem(wxArrayLong& results);

    /*****************************************************************/ /**
   @brief Move item in the list
   @details This function is intended to move item between two position
   in the list. This function could be used for exemple in response to
   a DnD event or to a popup menu
   @param iItem the zero based index of the item to move
   @param iNewPos The new position for the object iItem (default is 0,
   top of the list)
   @author Lucien Schreiber (c) CREALP 2007
   @date 23 November 2007
   *********************************************************************/
    void MoveItem(int iItem, int iNewPos = 0);

    /***************************************************************************/ /**
   @brief Change list values using a ListGenDialog
   @details For simple list, we can use a ListGenDialog derived dialog. This
   dialog should implement the SetDlgData() and GetDlgData virtual function for
   modification of items in response of a double click for exemple.
   @param pdialog The adress of a ListGenDialog
   @param myValues The string used to pass data to the dialog for modification
   @return  return true if the dialog was closed  with the wxID_OK code, FALSE
   otherwise.
   @author Lucien Schreiber (c) CREALP 2007
   @date 18 December 2007
   *******************************************************************************/
    bool DataToList(ListGenDialog* pdialog, wxArrayString& myValues);

    /***************************************************************************/ /**
   @brief Delete the selected item
   @details
   @return  TRUE if an item was selected and is now deleted, otherwise FALSE.
   @author Lucien Schreiber (c) CREALP 2007
   @date 18 December 2007
   *******************************************************************************/
    bool DeleteSelectedItem();

    /***************************************************************************/ /**
   @brief Get all data from a row into an array of string
   @details This function does some aditionnal checks, first of all it checks that
   some columns exists in the list control and also that the specified item exist
   (calling the ItemExist() function). A wxFAIL signal is sent in debug mode if
   thoses conditions aren't fullfiled.
   @param myStringArray reference to a wxArrayString for storing the list data as
   wxString.
   @param index Zero based index of the list item to get
   @return  the number of columns of the list control, -1 if all checks fails (see
   detailled description)
   @author Lucien Schreiber (c) CREALP 2007
   @date 18 December 2007
   *******************************************************************************/
    virtual int GetAllDataAsStringArray(wxArrayString& myStringArray, long index);

    /***************************************************************************/ /**
   @brief Check for an item existence
   @details
   @param index the zero based item index of the searched item
   @return  return TRUE if the item exist, in Debug mode, a WXFAIL event will be
   sent if the item dosen't exist.
   @author Lucien Schreiber (c) CREALP 2007
   @date 18 December 2007
   *******************************************************************************/
    bool ItemExist(long index);

    /***************************************************************************/ /**
   @brief Fill list using a string array
   @details This function could be used to set values into a list using a String
   array. This generic function may be used without knowledge of the list, it
   simply fill the number of cols according to the number of items in the string
   array. If both numbers are different, then only the first n items of the
   string array are used or only the first n columns are filled. Some internal
   checks are done to ensure that the passed array is not empty.
   @param myValue a string array containing the values in the same order as the
   columns
   @param index the zero based index of the item to set the text for. default is
   -1 meaning that we want to add a new item
   @return  return TRUE if the array contain data false otherwise
   @author Lucien Schreiber (c) CREALP 2007
   @date 18 December 2007
   *******************************************************************************/
    virtual bool EditDataToList(const wxArrayString& myValue, int index = -1);

    /***************************************************************************/ /**
   @brief Import and parse file to a list
   @details Uses the TextParser class for importing and parsing file into a list.
   @param filename string containing the filename (with path and extension)
   @param FilterIndex Zero index based value of the filter selected in the Open
   file dialog. This value could be for exemple : TXTFILE_COMMA or TXTFILE_TAB.
   See description of class TextParser for more details.
   @return  number of line parsed
   @author Lucien Schreiber (c) CREALP 2007
   @date 14 December 2007
   *******************************************************************************/
    virtual int ImportParsedFileToListCtrl(const wxString& filename, const int& FilterIndex);

    /***************************************************************************/ /**
   @brief Export values of a list to a parsed file
   @details This uses the TextParser class for parsing all the list values into a
   file. Supported parser are managed by the TextParser class.
   @param filename string containing the full path and extension of the
   destination file
   @param FilterIndex Zero index based value of the filter selected in the "Save
   file dialog" see also TEXTPARSER_TYPE
   @return  number of lines written to the file or 0 if nothing was written.
   @author Lucien Schreiber (c) CREALP 2007
   @date 18 December 2007
   *******************************************************************************/
    int ExportListParsedToFile(const wxString& filename, const int& FilterIndex);

    int GetColumnClicked(wxWindow* parent, int iIndex, int iBorderMargin);

    DECLARE_EVENT_TABLE();
};

class ListGenReportWithDialog : public ListGenReport {
  protected:
    wxDialog* m_pDialog;

    void InitMembers();

    virtual void BeforeAdding() {
        ;
    }

    virtual void BeforeDeleting() {
        ;
    }

    virtual void AfterAdding(bool bRealyAddItem) {
        ;
    }

    virtual void BeforeEditing() {
        ;
    }

    virtual void AfterEditing(bool bRealyEdited) {
        ;
    }

    virtual void AddingValueToArray(wxArrayString& myImportedValues) {
        ;
    }

    virtual void OnDoubleClickItem(wxListEvent& event);

    virtual void OnPressBackSpace(wxListEvent& event);

  public:
    ListGenReportWithDialog(wxWindow* parent, wxWindowID id, wxSize size = wxDefaultSize);

    ListGenReportWithDialog(wxWindow* parent, wxWindowID id, wxArrayString* pColsName, wxArrayInt* pColsSize = nullptr,
                            wxSize size = wxDefaultSize, long style = wxLC_REPORT);

    ~ListGenReportWithDialog();

    virtual int ImportParsedFileToListCtrl(const wxString& filename, const int& FilterIndex);

    void SetDialog(wxDialog* pDialog) {
        m_pDialog = pDialog;
    }

    virtual void AddItem();

    virtual void DeleteItem();

    void EditItem();
};

// enum for disabling menu based on position
enum MENU_DISABLE {
    MENU_DISABLE_TOP = 1,
    MENU_DISABLE_BOTTOM = 2
};

/*****************************************************************/ /**
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
class ListGenMenu : public wxMenu {
  protected:
    void MenuInit();

    virtual wxMenu* CreateContextMenu();

    wxMenu* m_ContextMenu;

  public:
    ListGenMenu();

    ListGenMenu(const wxString& title, long style = 0);

    ~ListGenMenu() {
        delete m_ContextMenu;
    }

    // disable portion of menu
    void DisableMenuMove(int flags);

    wxMenu* GetTheMenu() {
        return m_ContextMenu;
    }
};

class ListGenDialog : public wxDialog {
  protected:
    // virtual void Init(){;}
    // virtual void CreateDlgControls() {;}
  public:
    ListGenDialog() {
        ;
    }
    // ListGenDialog( wxWindow* parent,
    //  wxWindowID id ,
    //  const wxString& caption,
    //  const wxPoint& pos,
    //  const wxSize& size,
    //  long style);

    ~ListGenDialog() {
        ;
    }

    virtual void GetDlgData(wxArrayString& myStringArray) = 0;

    virtual void SetDlgData(wxArrayString& myStringArray) = 0;
};

#endif
