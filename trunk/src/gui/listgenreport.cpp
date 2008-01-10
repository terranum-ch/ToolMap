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
	size,wxLC_REPORT | wxLC_SINGLE_SEL)
{
	CreateColumns(pColsName,pColsSize);
	OnInit();
}

ListGenReport::ListGenReport(wxWindow * parent, wxWindowID id, wxSize size) 
	: wxListCtrl(parent,id,wxDefaultPosition,
	size,wxLC_REPORT | wxLC_SINGLE_SEL)
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
	EVT_LIST_BEGIN_DRAG (ID_LIST, ListGenReport::OnStartDrag)
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
	int iSelectedItem = GetSelectedItem();
	
	// check if an item is selected
	if (iSelectedItem != -1)
	{
		PopupMenu(m_ListContextMenu->GetTheMenu());
	}
	event.Skip();
}


void ListGenReport::OnMoveItemInList (wxCommandEvent & event)
{
	int iSelectedItem = GetSelectedItem();
	switch (event.GetId())
	{
		case ID_MENU_MOVE_TOP:
			MoveItem(iSelectedItem, 0);
			break;
		case ID_MENU_MOVE_UP:
			MoveItem(iSelectedItem, iSelectedItem-1);
			break;
		case ID_MENU_MOVE_DOWN:
			MoveItem(iSelectedItem, iSelectedItem+2);
			break;
		case ID_MENU_MOVE_BOTTOM:
			MoveItem(iSelectedItem, GetItemCount());
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

bool ListGenReport::SetItemText(int iItem, int iCol, wxString text)
{
	wxListItem Item;
	Item.SetId(iItem);
	Item.SetColumn(iCol);
	Item.SetText (text);
	return SetItem (Item);
}

void ListGenReport::OnStartDrag (wxListEvent & event)
{
	wxTextDataObject myData (_T("Test"));
	
	wxLogMessage(_T("OnStartDrag"));
	
	wxDropSource dragSource (this);
	dragSource.SetData(myData);
	wxDragResult dragResult = dragSource.DoDragDrop (wxDrag_AllowMove);
}

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
	long iSelectedItem = GetSelectedItem();
	if (iSelectedItem != -1)
	{
		DeleteItem(iSelectedItem);
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


int ListGenReportWithDialog::ImportParsedFileToListCtrl(const wxString & filename, 
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
		wxLogDebug(_T("Opening OK, my nice parser is : %s"), 
				   m_ImportParser->GetParserType().c_str());
		
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
		
	}
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

