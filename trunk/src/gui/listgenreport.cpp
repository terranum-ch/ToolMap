 /*******************************************************//**
@file ListGenReport.cpp
@brief wxListCtrl derived class for dealing with function 
parameters (definition)
@details This file contain the description of the class 
ListGenReport. This class is derived from the wxListCtrl 
class and is used for managing the list control containing
all parameters and return values
@author Lucien Schreiber (c) CREALP 2007
@date 15 November 2007
 ***********************************************************/


#include "listgenreport.h"

 


const int ListGenReport::ID_LIST = wxID_ANY;

ListGenReport::ListGenReport (wxWindow * parent, wxWindowID id, wxArrayString * pColsName, wxArrayInt * pColsSize, wxSize size)
	: wxListCtrl(parent,id,wxDefaultPosition,
	size,wxLC_REPORT) // | wxLC_SINGLE_SEL)
{
	CreateColumns(pColsName,pColsSize);
	OnInit();
}

ListGenReport::ListGenReport(wxWindow * parent, wxWindowID id, wxSize size) 
	: wxListCtrl(parent,id,wxDefaultPosition,
	size,wxLC_REPORT) // | wxLC_SINGLE_SEL)
{
	OnInit();
}

void ListGenReport::CreateColumns(wxArrayString * pColsName, wxArrayInt * pColsSize)
{
	int myColWidth = 100;
	
	if (pColsName != NULL)
	{
		for (unsigned int i=0; i<pColsName->GetCount(); i++)
		{
			if (pColsSize != NULL)
			{
				myColWidth = pColsSize->Item(i);
			}
			InsertColumn (i,pColsName->Item(i), wxLIST_FORMAT_LEFT, myColWidth);
		}
	}
}


//TextParser * ListGenReport::CreateParserBasedOnType (const TEXTPARSER_TYPE & myParserType)


bool ListGenReport::EditDataToList (const wxArrayString & myValue, int index)
{
	// check that data are stored in the array to fill the list.
	// if the array contain more data than the list could afford, 
	// only first data will be used. If the array is too small for
	// filling totally the list the only the n first col will be 
	// used.
	int iArrayItemCount = myValue.GetCount();
	int iRunNb = 0;
	iArrayItemCount > GetColumnCount() ? iRunNb = GetColumnCount() : iRunNb = iArrayItemCount;
	
	if (iArrayItemCount > 0)
	{
		// add the first line in the list if index is = -1
		if (index == -1)
		{
			AddItemToList(myValue.Item(0));
			index = GetItemCount()-1;
		}
		else
			SetItemText(index, 0, myValue.Item(0));
		
		for (int i=1; i<iRunNb; i++)
		{
			SetItemText(index,i, myValue.Item(i));
		}
		
		return TRUE;
	}
	return FALSE;
	
}


int ListGenReport::ImportParsedFileToListCtrl(const wxString & filename, 
											  const int & FilterIndex)
{
	wxArrayString myArrValues;
	int iLineCount = 0;
	
	// create parser depending on the selected format and set a file
	// for that parser
	m_ImportParser = TextParser::CreateParserBasedOnType(FilterIndex);
	m_ImportParser->SetParseFileName(filename);
		
	// check that the parser is not null or may crash
	wxASSERT(m_ImportParser != NULL);
	
	// try to open the file for parsing
	if(m_ImportParser->OpenParseFile())
	{
		wxLogDebug(_T("Opening OK, parser is : %s"), 
				   m_ImportParser->GetParserType().c_str());
		
		// loop for parsing all line
		iLineCount = m_ImportParser->GetLineCount();
		for (int i=0; i < iLineCount; i++)
		{
			m_ImportParser->ParseNextLine(myArrValues);
			
			// add values to the list
			EditDataToList(myArrValues);
			
			// clear the array
			myArrValues.Clear();
		}
		m_ImportParser->CloseParseFile();
		
	}
	if (m_ImportParser != NULL)
		delete m_ImportParser;
	return iLineCount;
	
}


int ListGenReport::ExportListParsedToFile (const wxString & filename,
										   const int & FilterIndex)
{
	wxArrayString myArrValues;
	int iLineCount = 0;
	int iArrayItemCount = 0;
	
	// create parser depending on the selected format and set a file
	// for that parser
	m_ImportParser = TextParser::CreateParserBasedOnType(FilterIndex);
	m_ImportParser->SetParseFileName(filename);

	
	// check that the parser is not null or may crash
	wxASSERT(m_ImportParser != NULL);
	
	// try to open the file for parsing in create mode
	if(m_ImportParser->OpenParseFile(TRUE))
	{
		// loop for parsing all item in list
		iLineCount = GetItemCount();
		for (int i=0; i < iLineCount; i++)
		{
			// get values from the list
			iArrayItemCount = GetAllDataAsStringArray(myArrValues, i);
			
			// send those data to the file
			m_ImportParser->WriteNextLine(myArrValues);
			
			// clear the array
			myArrValues.Clear();
		}
	
		m_ImportParser->CloseParseFile();
	}
	if (m_ImportParser != NULL)
		delete m_ImportParser;
	return iLineCount;
}


void ListGenReport::OnInit ()
{
	m_ListContextMenu = NULL;
	m_ListContextMenu = new ListGenMenu();
	m_ImportParser = NULL;
	
	// test for connection event
	//Connect(ID_LIST,wxEVT_COMMAND_LIST_ITEM_ACTIVATED,wxCommandEventHandler());
}



BEGIN_EVENT_TABLE(ListGenReport,wxListCtrl)
	EVT_LIST_ITEM_ACTIVATED(ID_LIST, ListGenReport::OnDoubleClickItem)
	EVT_LIST_KEY_DOWN (ID_LIST,ListGenReport::OnPressBackSpace)
	//EVT_LIST_BEGIN_DRAG (ID_LIST, ListGenReport::OnStartDrag)
	EVT_LIST_ITEM_RIGHT_CLICK(ID_LIST, ListGenReport::OnContextMenu)
	EVT_MENU_RANGE(ID_MENU_MOVE_TOP,ID_MENU_MOVE_BOTTOM,ListGenReport::OnMoveItemInList)
END_EVENT_TABLE()



ListGenReport::~ListGenReport()
{
	delete m_ListContextMenu;
}

void ListGenReport::AddItemToList(wxString myValue, int iPosition)
{
	int iPos = iPosition;
	if (iPosition == -1)
		iPos = GetItemCount();
	
	InsertItem(iPos,myValue);
	SetItem(iPos, 0,  myValue);
}

void ListGenReport::OnDoubleClickItem (wxListEvent & event)
{
		
		wxString myAcutalValue = GetItemColText(event.GetIndex(), 1);
		wxTextEntryDialog myTextDlg(this, _("Modify Value :"),_("Value"));
		myTextDlg.SetValue(myAcutalValue);
		myTextDlg.ShowModal();
		SetItemText(event.GetIndex(),1,myTextDlg.GetValue());
}

void ListGenReport::OnPressBackSpace (wxListEvent & event)
{
	if (event.GetKeyCode() == WXK_BACK || event.GetKeyCode() == WXK_DELETE)
	{
		//long iSelectedItem = GetSelectedItem();
//		if (iSelectedItem != -1)
//		{
//			DeleteItem(iSelectedItem);
//		} 
		DeleteSelectedItem();
		
	}
	event.Skip();
}

void ListGenReport::OnContextMenu (wxListEvent & event)
{			
	int flags = 0;
	wxArrayLong mySelected;
	
	// get all selected items 
	int iCountSelected = GetAllSelectedItem(mySelected);
	if (iCountSelected > 0)
	{
		// if first item is selected we disable the
		// contextual top menu
		if (mySelected.Item(0) == 0) 
			flags = flags | MENU_DISABLE_TOP;
		
		// if last item is selected we disable the
		// contextual top menu
		if (mySelected.Last() == GetItemCount()-1)
			flags = flags | MENU_DISABLE_BOTTOM;
	
		m_ListContextMenu->DisableMenuMove(flags);
		
		PopupMenu(m_ListContextMenu->GetTheMenu());
		
	}
	event.Skip();
}


void ListGenReport::OnMoveItemInList (wxCommandEvent & event)
{
	// get all selected item
	wxArrayLong mySelectedItems;
	int iCountSelected = GetAllSelectedItem(mySelectedItems);
	int i=0;
	int idestpos = 0;
	
	switch (event.GetId())
	{
		case ID_MENU_MOVE_TOP:
			// compute movement 
			idestpos = mySelectedItems[0] - 0;

			for (i=0; i< iCountSelected; i++)
			{
				MoveItem(mySelectedItems[i],mySelectedItems[i] - idestpos);
			}
			break;
			
		case ID_MENU_MOVE_UP:
			for (i=0; i< iCountSelected; i++)
			{
				SwapRow(mySelectedItems[i],mySelectedItems[i] - 1);
			}
			break;
	
		case ID_MENU_MOVE_DOWN:
			for (i = iCountSelected -1; i >= 0 ; i--)
			{
				SwapRow(mySelectedItems[i],mySelectedItems[i] + 1);
			}
			//MoveItem(iSelectedItem, iSelectedItem+2);
			break;
		case ID_MENU_MOVE_BOTTOM:
			// compute movement 
			idestpos = (GetItemCount() - 1) - mySelectedItems.Last();
			
			for (i = iCountSelected -1; i >= 0 ; i--)
			{
				MoveItem(mySelectedItems[i],mySelectedItems[i] + idestpos + 1);
			}
			
			
			break;
	}
}

wxString ListGenReport::GetItemColText(int iItem, int iCol)
{
	wxListItem Item;
	
	Item.SetId(iItem);
	Item.SetMask (wxLIST_MASK_TEXT);
	Item.SetColumn(iCol);
	GetItem (Item);
	return Item.GetText ();
}

long ListGenReport::GetSelectedItem()
{
	return GetNextItem(-1, wxLIST_NEXT_ALL, wxLIST_STATE_SELECTED);
}


int ListGenReport::GetAllSelectedItem(wxArrayLong & results)
{
	
	int i = -1;
	while (1)
	{
		i = GetNextItem(i, wxLIST_NEXT_ALL, wxLIST_STATE_SELECTED);
		
		// break if nothing found
		if (i == -1)
			break;
		else 
		{
			results.Add(i);
		}
		
		
	}
	wxLogDebug(_T("Found %d items selected in list"), results.GetCount());
	return results.GetCount();
}



bool ListGenReport::SetItemText(int iItem, int iCol, wxString text)
{
	wxListItem Item;
	Item.SetId(iItem);
	Item.SetColumn(iCol);
	Item.SetText (text);
	return SetItem (Item);
}

//void ListGenReport::OnStartDrag (wxListEvent & event)
//{
//	wxTextDataObject myData (_T("Test"));
//	
//	wxLogMessage(_T("OnStartDrag"));
//	
//	wxDropSource dragSource (this);
//	dragSource.SetData(myData);
//	wxDragResult dragResult = dragSource.DoDragDrop (wxDrag_AllowMove);
//}

void ListGenReport::MoveItem (int iItem, int iNewPos)
{	
	wxArrayString myItemAllText;
	int i=0;
	unsigned int j=0;
	
	// get item
	if (iItem != -1 && iNewPos < GetItemCount()+1)
	{
		for (i=0; i< GetColumnCount(); i++)
		{
			myItemAllText.Add( GetItemColText(iItem, i));
		}	
	
	// copy to new item
	AddItemToList(myItemAllText.Item(0),iNewPos);
	
	// add text to new item
	for (j=1; j< myItemAllText.GetCount(); j++)
	{
		SetItemText(iNewPos, j, myItemAllText.Item(j));
	}
	myItemAllText.Clear();
	
	// delete old item
	if (iNewPos < iItem)
	{
		iItem ++;
	}
	
	DeleteItem(iItem);
	
	}
}


bool ListGenReport::DataToList(ListGenDialog * pdialog,  wxArrayString & myValues)
{
	if (pdialog) // check for null pointer
	{
		// if array not empty, we send data to the dialog
		if(myValues.GetCount() > 0)
		{
			pdialog->SetDlgData(myValues);	
		}
		
		if(pdialog->ShowModal() == wxID_OK)
		{
			// clear the array
			myValues.Clear();
			
			// get data from the dialog
			pdialog->GetDlgData(myValues);
			return TRUE;
		}
	}
	else 
	{
		wxLogDebug(_T("Pointer to the dialog is null"));	
	}
	return FALSE;
}


bool ListGenReport::DeleteSelectedItem()
{
	wxArrayLong itemToDelete;
	int iNbItemToDelete = GetAllSelectedItem(itemToDelete);
	if (iNbItemToDelete > 0)
	{
		for (int i= iNbItemToDelete -1 ; i >= 0 ; i--)
		{
			// delete from end to avoid deleting off limits
			DeleteItem(itemToDelete[i]);
		}
		return TRUE;
	}
	
	wxLogDebug(_T("No item deleted, select an item"));
	return FALSE;
}


int ListGenReport::GetAllDataAsStringArray(wxArrayString & myStringArray, long index)
{
	int iColNumber = GetColumnCount();
	wxString myTempString = _T("");
	
	// check that some columns exists
	// and check also that item exist at
	// specified index
	if (iColNumber == 0 || !ItemExist(index))
	{
		wxFAIL;
		return -1;
	}
	
	// get data from all colums at the specified index
	for (int i=0; i< iColNumber; i++)
	{
		myTempString = GetItemColText(index, i);
		myStringArray.Add(myTempString);
	}
	return iColNumber;
}

bool ListGenReport::ItemExist(long index)
{
	wxListItem info;
	info.SetId(index);
	if (!GetItem(info))
	{
		wxFAIL;
		return FALSE;
	}
	return TRUE;
}


/************************ LISTGENMNEU **********************************/

ListGenMenu::ListGenMenu() : wxMenu()
{
	MenuInit();
}

ListGenMenu::ListGenMenu(const wxString & title, long style) : 
	wxMenu(title, style)
{
	MenuInit();
}

void ListGenMenu::MenuInit ()
{
	m_ContextMenu =  new wxMenu;
	m_ContextMenu = CreateContextMenu();
}

wxMenu * ListGenMenu::CreateContextMenu()
{
    wxMenu* itemMenu1 = new wxMenu;
    itemMenu1->Append(ID_MENU_MOVE_TOP, _("Move item to the &top"), _T(""), wxITEM_NORMAL);
    itemMenu1->Append(ID_MENU_MOVE_UP, _("Move item &up"), _T(""), wxITEM_NORMAL);
    itemMenu1->AppendSeparator();
    itemMenu1->Append(ID_MENU_MOVE_DOWN, _("Move item &down"), _T(""), wxITEM_NORMAL);
    itemMenu1->Append(ID_MENU_MOVE_BOTTOM, _("Move item to the &bottom"), _T(""), wxITEM_NORMAL);
    return itemMenu1;
}


void ListGenMenu::DisableMenuMove (int flags)
{
	m_ContextMenu->Enable(ID_MENU_MOVE_UP, TRUE);
	m_ContextMenu->Enable(ID_MENU_MOVE_TOP, TRUE);
	m_ContextMenu->Enable(ID_MENU_MOVE_DOWN, TRUE);
	m_ContextMenu->Enable(ID_MENU_MOVE_BOTTOM, TRUE);
		
	// if flags = MENU_DISABLE_TOP, disable two first item
	if ((flags & MENU_DISABLE_TOP) == MENU_DISABLE_TOP)
	{
		m_ContextMenu->Enable(ID_MENU_MOVE_UP, FALSE);
		m_ContextMenu->Enable(ID_MENU_MOVE_TOP, FALSE);
	}
	
	if ((flags & MENU_DISABLE_BOTTOM) == MENU_DISABLE_BOTTOM)
	{
		m_ContextMenu->Enable(ID_MENU_MOVE_DOWN, FALSE);
		m_ContextMenu->Enable(ID_MENU_MOVE_BOTTOM, FALSE);
	}	
}


/************************ LISTGENREPORTWITHDIALOG**********************************/
ListGenReportWithDialog::ListGenReportWithDialog (wxWindow * parent, wxWindowID id, 
												  wxArrayString * pColsName, 
												  wxArrayInt * pColsSize, 
												  wxSize size) : 
ListGenReport (parent, id,pColsName, pColsSize, size)
{
	InitMembers();	
}


ListGenReportWithDialog::ListGenReportWithDialog(wxWindow * parent, 
												 wxWindowID id, 
												 wxSize size) :
ListGenReport(parent, id, size)
{
	InitMembers();
}


ListGenReportWithDialog::~ListGenReportWithDialog()
{
	
}


void ListGenReportWithDialog::InitMembers()
{
	m_pDialog = NULL;
	
}


void ListGenReport::SortListItem(int x_col, int low , int high , int typecol, bool bAscending )
{
	wxASSERT(GetColumnCount() > x_col );
	
	
	if( high == -1 )
		high = GetItemCount() - 1;
	
	int lo = low;
	int hi = high;
	wxString midItem;
	
	if( hi <= lo ) return ; 
	
	wxListItem info;	info.m_itemId = (lo+hi)/2;
	info.m_mask = wxLIST_MASK_TEXT|wxLIST_MASK_DATA|wxLIST_MASK_IMAGE;
	info.m_col = x_col;
	GetItem( info );
	midItem = info.GetText();
	
	// loop through the list until indices cross
	while( lo <= hi )
	{
		while( ( lo < high ) && ( Compare( typecol, GetItemColText(lo,
		 x_col), midItem, bAscending ) == -1 ) )
		 ++lo;
		 
		 while( ( hi > low ) && ( Compare( typecol, GetItemColText(hi,
		 x_col), midItem, bAscending ) == 1 ) )
		 --hi;
		 
		// if the indexes have not crossed, swap
		// and if the items are not equal
		if( lo <= hi )
		{
			// swap only if the items are not equal
			if( GetItemColText(lo, x_col) != GetItemColText(hi, x_col))
			{
				SwapRow( lo, hi );
			}
			++lo;
			--hi;
		}
	}
	
	// If the right index has not reached the left side of array
	// must now sort the left partition.
	if( low < hi )
		SortListItem( x_col, low, hi, typecol, bAscending);
	
	// If the left index has not reached the right side of array
	// must now sort the right partition.
	if( lo < high )
		SortListItem( x_col, lo, high, typecol, bAscending);
}



/***************************************************************************//**
 @brief Swap two items value
 @details Swap the values of items (color, item data, even icon) between two
 items. A verification is made and nothing is done if two items are similar
 @param x_row1 zero based index of item one
 @param x_row2 zero based index of item two
 @author Lucien Schreiber (c) CREALP 2007
 @date 03 April 2008
 *******************************************************************************/
void ListGenReport::SwapRow( int x_row1, int x_row2 )
{
	// dont change Row A with Row A - its nonsense
	if( x_row1 == x_row2 )
		return;
	
	wxListItem item1; item1.m_itemId = x_row1; item1.m_col = 0;
	item1.m_mask = wxLIST_MASK_TEXT|wxLIST_MASK_DATA; //|wxLIST_MASK_IMAGE;
	GetItem( item1 );
	
	int iIcon		= item1.GetImage();
	wxColour rowColor = item1.GetTextColour(); //m_pList->GetItemTextColour(x_row1 );
	long lData		= GetItemData( x_row1 );
	
	wxColour item2color = GetItemTextColour( x_row2 );
	
	SetItemTextColour( x_row1, item2color );
	wxListItem item2; item2.m_itemId = x_row2; item2.m_col = 0;
	item2.m_mask = wxLIST_MASK_TEXT|wxLIST_MASK_DATA; //|wxLIST_MASK_IMAGE;
	GetItem( item2 );
	SetItemImage( x_row1, item2.GetImage(), item2.GetImage() );
	SetItemData( x_row1, GetItemData(x_row2) );
	
	//SetItemTextColour( x_row2, iIcon );
	SetItemImage( x_row2, iIcon, iIcon  );
	SetItemData( x_row2, lData );
	
	int maxCol = GetColumnCount();
	
	//////////////////////////////////
	// swap column Values too
	for( int c = 0; c < maxCol; ++c)
	{
		static wxListItem info, info2;
		
		info.m_itemId = x_row1;
		info.m_col = c;
		info.m_mask = wxLIST_MASK_TEXT|wxLIST_MASK_DATA|wxLIST_MASK_IMAGE;
		bool b = GetItem(  info );
		wxASSERT( b );
		
		info2.m_itemId = x_row2;
		info2.m_col = c;
		info2.m_mask = wxLIST_MASK_TEXT|wxLIST_MASK_DATA|wxLIST_MASK_IMAGE;
		GetItem( info2 );
		
		SetItem( x_row1, c, info2.GetText(), info2.GetImage() );
		
		SetItem( x_row2, c, info.GetText(), info.GetImage() );
		
	} // for
	
}


/***************************************************************************//**
 @brief Compare two values for sorting data
 @details This function compare values of two items based on the column type
 wich could be one of the #LIST_FIELD_TYPE
 @param iColumnCompareType One of the #LIST_FIELD_TYPE value indicating wich
 sort of column we are trying to sort
 @param x_strValue1 The string value of item 1 (may be converted internally
 based on column type)
 @param x_strValue2 The string value of item 2 (may be converted internally
 based on column type)
 @param bAscending a TRUE value indicate an ascending sort and a FALSE one
 indicate a descending sort
 @return  if value 1 is greater than value 2 return 1, if equal return 0 and
 finaly if value 1 is smaller than value 2 return -1
 @author Lucien Schreiber (c) CREALP 2007
 @date 03 April 2008
 *******************************************************************************/
int ListGenReport::Compare( int iColumnCompareType, const wxString
							   &x_strValue1,  const wxString &x_strValue2, bool bAscending )
{
	if( bAscending == TRUE)
	{
		if( iColumnCompareType == FIELD_STRING) // simple String value compare
		{
			if( x_strValue1 > x_strValue2 )
				return 1;
			else if( x_strValue1 < x_strValue2 )
				return -1;
			else
				return 0;
		}
		else if( iColumnCompareType == FIELD_NUMBER ) // Numbers compare
		{
			if( wxAtof(x_strValue1) > wxAtof(x_strValue2) )
				return 1;
			else if( wxAtof(x_strValue1) < wxAtof(x_strValue2) )
				return -1;
			else
				return 0;
		}
		else if( iColumnCompareType == FIELD_DATE ) // Date compare -
		{
			wxDateTime dt1;
			//int d=0, m=0 , y=0;
			if (dt1.ParseDate(x_strValue1)== NULL)
			{
				dt1 = wxInvalidDateTime;
			}
			wxDateTime dt2;
			if (dt2.ParseDate(x_strValue2)== NULL)
			{
				dt2 = wxInvalidDateTime;
			}

			if( dt1.IsValid() == false )
				return -1;
			if( dt2.IsValid() == false )
				return 1;
			
			if( dt1 > dt2 )
				return 1;
			else if( dt1 < dt2 )
				return -1;
			else
				return 0;
			
		}
	}
	else
	{ // Reihenfolge umdrehen
		if( iColumnCompareType == FIELD_STRING )
		{
			if( x_strValue1 > x_strValue2 )
				return -1;
			else if( x_strValue1 < x_strValue2 )
				return 1;
			else
				return 0;
		}
		else if( iColumnCompareType == FIELD_NUMBER ) // Numbers compare
		{
			if( wxAtof(x_strValue1) > wxAtof(x_strValue2) )
				return -1;
			else if( wxAtof(x_strValue1) < wxAtof(x_strValue2) )
				return 1;
			else
				return 0;
		} // Number Compare
		else if( iColumnCompareType == FIELD_DATE ) // Date compare -
		{
			wxDateTime dt1;
			//int d=0, m=0 , y=0;
			if (dt1.ParseDate(x_strValue1)== NULL)
			{
				dt1 = wxInvalidDateTime;
			}

			wxDateTime dt2;
			
			if(dt2.ParseDate(x_strValue2)==NULL)
			{
				dt2 = wxInvalidDateTime;
			}
			
			if( dt1.IsValid() == false )
				return 1;
			if( dt2.IsValid() == false )
				return -1;
			
			if( dt1 > dt2 )
				return -1;
			else if( dt1 < dt2 )
				return 1;
			else
				return 0;
		} // Date Compare
	}
	return 0;
}



void ListGenReportWithDialog::AddItem()
{	
	BeforeAdding();
	
	// check that the dialog pointer is not null
	// otherwise no operations are allowed
	wxASSERT_MSG (m_pDialog, wxT("Pointer to the dialog not initialised, init this pointer first"));

	// show the dialog for edition,
	// the dialog must implement the TransfertDataToWindow
	// for beeing usable
	if (m_pDialog->ShowModal()==wxID_OK)
	{
		AfterAdding(TRUE);
	}
	else
		AfterAdding(FALSE);
	
	
}


void ListGenReportWithDialog::EditItem()
{
		
	BeforeEditing();
	
	
	// check that the dialog pointer is not null
	// otherwise no operations are allowed
	wxASSERT_MSG (m_pDialog, wxT("Pointer to the dialog not initialised, init this pointer first"));

	// show the dialog for edition,
	// the dialog must implement the TransfertDataToWindow
	// for beeing usable
	if (m_pDialog->ShowModal()==wxID_OK)
	{
		AfterEditing(TRUE);
	}
	else
		AfterEditing(FALSE);
	
	
	
}


void ListGenReportWithDialog::DeleteItem()
{
	BeforeDeleting();
	DeleteSelectedItem();
}


void ListGenReportWithDialog::OnDoubleClickItem (wxListEvent & event)
{
	// default actions : call edititem, you may want to derive
	// this action
	EditItem();
}


void ListGenReportWithDialog::OnPressBackSpace (wxListEvent & event)
{
	if (event.GetKeyCode() == WXK_BACK || event.GetKeyCode() == WXK_DELETE)
	{
		DeleteItem();
	}
	event.Skip();
}


/***************************************************************************//**
 @brief Import file data to list
 @details This functions uses the #TextParser class for importing data. A text
 parser derived object is created using the FilterIndex parameter.
 @param filename The name of the file to parse for import
 @param FilterIndex the zero based index of the selected filter (One of the
 #TEXTPARSER_TYPE values).
 @return  The number of lines found or -1 if an error occur
 @author Lucien Schreiber (c) CREALP 2008
 @date 19 May 2008
 *******************************************************************************/
int ListGenReportWithDialog::ImportParsedFileToListCtrl(const wxString & filename, 
											  const int & FilterIndex)
{
	wxArrayString myArrValues;
	int iLineCount = 0;
	
	// create parser depending on the selected format and set a file
	// for that parser
	m_ImportParser = TextParser::CreateParserBasedOnType(FilterIndex);
	m_ImportParser->SetParseFileName(filename);
	
	// specify the number of cols we are looking for
	m_ImportParser->SetNumberOfFields(GetColumnCount());
	
	
	// check that the parser is not null or may crash
	wxASSERT(m_ImportParser != NULL);
	
	// try to open the file for parsing
	if(!m_ImportParser->OpenParseFile())
	{
		wxLogError(_("Error opening file %s for parsing. Format may be incorrect"),
				   filename.c_str());
		return -1;
	}
	
	
	// file is now open for parsing
	wxLogDebug(_T("Parser is : %s"), 
			   m_ImportParser->GetParserType().c_str());
	
	
	
	// check file to parse for avoiding errors
	wxString sError = _("Error importing files, see log window for more informations\n\n");
	sError += _("Log window may be displayed using : Window -> Log Window");
	if (!m_ImportParser->CheckFileToParse())
	{
		wxMessageBox(sError,_("Error importing files"), wxOK | wxICON_ERROR);
		return -1;
	}
	
	
	// loop for parsing all line
	iLineCount = m_ImportParser->GetLineCount();
	for (int i=0; i < iLineCount; i++)
	{
		m_ImportParser->ParseNextLine(myArrValues);
		
		// add values to the array
		AddingValueToArray(myArrValues);
		
		// add values to the list
		EditDataToList(myArrValues);
		
		// clear the array
		myArrValues.Clear();
	}
	m_ImportParser->CloseParseFile();
	
	
	if (m_ImportParser != NULL)
		delete m_ImportParser;
	return iLineCount;
	
}


/************************ LISTGENDIALOG **********************************/
//ListGenDialog::ListGenDialog()
//{
//	Init();
//}

//ListGenDialog::ListGenDialog( wxWindow* parent,wxWindowID id , const wxString& caption,
//							 const wxPoint& pos,const wxSize& size,long style)
//{
//	Init();
//	SetExtraStyle(wxWS_EX_BLOCK_EVENTS);
//    wxDialog::Create( parent, id, caption, pos, size, style );
//    CreateDlgControls();
//    if (GetSizer())
//    {
//        GetSizer()->SetSizeHints(this);
//    }
//    Centre();
//}

