/***************************************************************************
 datalistreportctrl.cpp
 -------------------
 copyright            : (C) 2010 CREALP Lucien Schreiber
 ***************************************************************************/

#include "datalistreportctrl.h"

#include "../gui/bitmaps.h"

int DataListReportCtrl::_Compare(DATALIST_COLTYPE coltype, const wxString& text1, const wxString& text2,
                                 bool ascending) {
    if (ascending) {
        switch (coltype) {
            case DATALIST_COLTYPE_TEXT:
                if (text1 > text2)
                    return 1;
                else if (text1 < text2)
                    return -1;
                else
                    return 0;
                break;

            case DATALIST_COLTYPE_NUMBER:
                if (wxAtof(text1) > wxAtof(text2))
                    return 1;
                else if (wxAtof(text1) < wxAtof(text2))
                    return -1;
                else
                    return 0;
                break;

            case DATALIST_COLTYPE_DATE: {
                wxDateTime dt = wxDateTime::Now();
                wxDateTime myDate1;
                wxDateTime myDate2;
                wxString::const_iterator end;
                myDate1.ParseFormat(text1, _T("%d %b %Y"), dt, &end);
                myDate2.ParseFormat(text2, _T("%d %b %Y"), dt, &end);
                if (myDate1.IsLaterThan(myDate2)) {
                    return 1;
                } else if (myDate1.IsEarlierThan(myDate2)) {
                    return -1;
                } else {
                    return 0;
                }
            } break;

            case DATALIST_COLTYPE_TIME: {
                // wxDateTime dt = wxDateTime::Now();
                wxDateTime myTime1;
                wxDateTime myTime2;
                wxString::const_iterator end;
                myTime1.ParseTime(text1, &end);
                myTime2.ParseTime(text2, &end);
                if (myTime1.IsLaterThan(myTime2)) {
                    return 1;
                } else if (myTime1.IsEarlierThan(myTime2)) {
                    return -1;
                } else {
                    return 0;
                }
            } break;

            case DATALIST_COLTYPE_COLOUR: {
                wxColour myCol1(text1);
                int myTotCol1 = myCol1.Red() + myCol1.Green() + myCol1.Blue();
                wxColour myCol2(text2);
                int myTotCol2 = myCol2.Red() + myCol2.Green() + myCol2.Blue();
                if (myTotCol1 > myTotCol2) {
                    return -1;
                } else if (myTotCol1 < myTotCol2) {
                    return 1;
                } else {
                    return 0;
                }
            } break;

            case DATALIST_COLTYPE_IMAGE:
                if (wxAtoi(text1) > wxAtoi(text2)) {
                    return 1;
                } else if (wxAtoi(text1) < wxAtoi(text2)) {
                    return -1;
                } else {
                    return 0;
                }
                break;

            default:
                wxCHECK_MSG(true, 0, "Not implemented");
                break;
        }
    }
    // DESCENDING
    else {
        switch (coltype) {
            case DATALIST_COLTYPE_TEXT:
                if (text1 > text2)
                    return -1;
                else if (text1 < text2)
                    return 1;
                else
                    return 0;
                break;

            case DATALIST_COLTYPE_NUMBER:
                if (wxAtof(text1) > wxAtof(text2))
                    return -1;
                else if (wxAtof(text1) < wxAtof(text2))
                    return 1;
                else
                    return 0;
                break;

            case DATALIST_COLTYPE_COLOUR: {
                wxColour myCol1(text1);
                int myTotCol1 = myCol1.Red() + myCol1.Green() + myCol1.Blue();
                wxColour myCol2(text2);
                int myTotCol2 = myCol2.Red() + myCol2.Green() + myCol2.Blue();
                if (myTotCol1 > myTotCol2) {
                    return 1;
                } else if (myTotCol1 < myTotCol2) {
                    return -1;
                } else {
                    return 0;
                }
            } break;

            case DATALIST_COLTYPE_IMAGE:
                if (wxAtoi(text1) > wxAtoi(text2)) {
                    wxLogMessage("%s > %s", text1, text2);
                    return -1;
                } else if (wxAtoi(text1) < wxAtoi(text2)) {
                    wxLogMessage("%s < %s", text1, text2);
                    return 1;
                } else {
                    wxLogMessage("%s == %s", text1, text2);
                    return 0;
                }
                break;

            case DATALIST_COLTYPE_DATE: {
                wxDateTime dt = wxDateTime::Now();
                wxDateTime myDate1;
                wxDateTime myDate2;
                wxString::const_iterator end;
                myDate1.ParseFormat(text1, _T("%d %b %Y"), dt, &end);
                myDate2.ParseFormat(text2, _T("%d %b %Y"), dt, &end);
                if (myDate1.IsLaterThan(myDate2)) {
                    return -1;
                } else if (myDate1.IsEarlierThan(myDate2)) {
                    return 1;
                } else {
                    return 0;
                }
            } break;

            case DATALIST_COLTYPE_TIME: {
                // wxDateTime dt = wxDateTime::Now();
                wxDateTime myTime1;
                wxDateTime myTime2;
                wxString::const_iterator end;
                myTime1.ParseTime(text1, &end);
                myTime2.ParseTime(text2, &end);
                if (myTime1.IsLaterThan(myTime2)) {
                    return -1;
                } else if (myTime1.IsEarlierThan(myTime2)) {
                    return 1;
                } else {
                    return 0;
                }
            } break;

            default:
                wxCHECK_MSG(true, 0, "Not implemented");
                break;
        }
    }
    return 0;
}

wxString DataListReportCtrl::_GetTextSpecial(long index, int col, DATALIST_COLTYPE typecol) {
    if (typecol != DATALIST_COLTYPE_IMAGE) {
        return GetText(index, col);
    }

    // special case for sorting columns containing only images.
    // we did return the image index as text.
    // wxLogMessage(wxString::Format("index = %ld, value =%d", index, GetItemColumnImage(index, col)));
    return wxString::Format("%d", GetItemColumnImage(index, col));
}

DataListReportCtrl::DataListReportCtrl(wxWindow* parent, wxWindowID id, const wxPoint& pos, const wxSize& size,
                                       long style)
    : wxListCtrl(parent, id, pos, size, style) {
    _Init();
}

bool DataListReportCtrl::Create(wxWindow* parent, wxWindowID id, const wxPoint& pos, const wxSize& size, long style) {
    _Init();
    return wxListCtrl::Create(parent, id, pos, size, style);
}

void DataListReportCtrl::_Init() {
    m_ColourGrayed = wxSystemSettings::GetColour(wxSYS_COLOUR_GRAYTEXT);
    m_ColourNormal = wxSystemSettings::GetColour(wxSYS_COLOUR_LISTBOXTEXT);
}

DataListReportCtrl::~DataListReportCtrl() {}

long DataListReportCtrl::GetSelectedFirst() {
    return GetNextItem(-1, wxLIST_NEXT_ALL, wxLIST_STATE_SELECTED);
}

wxString DataListReportCtrl::GetText(long index, int col) {
    wxListItem Item;
    Item.SetId(index);
    Item.SetMask(wxLIST_MASK_TEXT);
    Item.SetColumn(col);
    GetItem(Item);
    return Item.GetText();
}

void DataListReportCtrl::SetText(long index, const wxString& text, int col) {
    long myResult = SetItem(index, col, text);
    wxASSERT(myResult >= 0);
}

int DataListReportCtrl::GetItemColumnImage(long index, int col) {
    wxListItem myItem;
    myItem.SetId(index);
    myItem.SetColumn(col);
    myItem.SetMask(wxLIST_MASK_IMAGE);
    GetItem(myItem);
    return myItem.GetImage();
}

void DataListReportCtrl::SetColumnImage(int imageindex, int col) {
    wxListItem myColItem;
    myColItem.SetColumn(col);
    myColItem.SetMask(wxLIST_MASK_IMAGE);
    myColItem.SetImage(imageindex);
    SetColumn(col, myColItem);
}

void DataListReportCtrl::SortList(int col, int lowindex, int highindex, DATALIST_COLTYPE typecol, bool ascending) {
    wxASSERT(GetColumnCount() > col);

    if (highindex == -1) {
        highindex = GetItemCount() - 1;
    }

    int lo = lowindex;
    int hi = highindex;

    if (hi <= lo) {
        return;
    }

    /*wxListItem info; info.m_itemId = (lo+hi)/2;
    info.m_mask = wxLIST_MASK_TEXT|wxLIST_MASK_DATA|wxLIST_MASK_IMAGE;
    info.m_col = col;
    GetItem( info );
    midItem = info.GetText();*/

    wxString midItem = _GetTextSpecial((lo + hi) / 2.0, col, typecol);

    // loop through the list until indices cross
    while (lo <= hi) {
        while ((lo < highindex) && (_Compare(typecol, _GetTextSpecial(lo, col, typecol), midItem, ascending) == -1))
            ++lo;

        while ((hi > lowindex) && (_Compare(typecol, _GetTextSpecial(hi, col, typecol), midItem, ascending) == 1)) --hi;

        // if the indexes have not crossed, swap
        // and if the items are not equal
        if (lo <= hi) {
            // swap only if the items are not equal
            if (_GetTextSpecial(lo, col, typecol) != _GetTextSpecial(hi, col, typecol)) {
                SwapRow(lo, hi);
            }
            ++lo;
            --hi;
        }
    }

    // If the right index has not reached the left side of array
    // must now sort the left partition.
    if (lowindex < hi) {
        SortList(col, lowindex, hi, typecol, ascending);
    }

    // If the left index has not reached the right side of array
    // must now sort the right partition.
    if (lo < highindex) {
        SortList(col, lo, highindex, typecol, ascending);
    }
}

void DataListReportCtrl::SwapRow(int row1, int row2) {
    // dont change Row A with Row A - its nonsense
    if (row1 == row2) {
        return;
    }

    // swap generalities (row color, data, selection status)
    long row1data = GetItemData(row1);
    long row2data = GetItemData(row2);
    SetItemData(row1, row2data);
    SetItemData(row2, row1data);

    wxColour row1Colour = GetItemTextColour(row1);
    wxColour row2Colour = GetItemTextColour(row2);
    SetItemTextColour(row1, row2Colour);
    SetItemTextColour(row2, row1Colour);

    int row1State = GetItemState(row1, wxLIST_STATE_SELECTED);
    int row2State = GetItemState(row2, wxLIST_STATE_SELECTED);
    SetItemState(row1, row2State, wxLIST_STATE_SELECTED);
    SetItemState(row2, row1State, wxLIST_STATE_SELECTED);

    int image1 = GetItemColumnImage(row1, 0);
    int image2 = GetItemColumnImage(row2, 0);

    // swap column Values too
    for (int c = 0; c < GetColumnCount(); ++c) {
        static wxListItem info1, info2;

        info1.SetId(row1);
        info1.SetColumn(c);
        info1.SetMask(wxLIST_MASK_TEXT | wxLIST_MASK_DATA | wxLIST_MASK_IMAGE);
        GetItem(info1);

        info2.SetId(row2);
        info2.SetColumn(c);
        info2.SetMask(wxLIST_MASK_TEXT | wxLIST_MASK_DATA | wxLIST_MASK_IMAGE);
        GetItem(info2);

        SetItem(row1, c, info2.GetText(), info2.GetImage());
        SetItem(row2, c, info1.GetText(), info1.GetImage());
    }

    SetItemImage(row1, image2);
    SetItemImage(row2, image1);
}

wxBitmap DataListReportCtrl::CreateArrowDown(const wxSize& size) {
    return Bitmaps::GetMiscBitmap(Bitmaps::ID_MISC::SORT_DOWN, size);
}

wxBitmap DataListReportCtrl::CreateArrowUp(const wxSize& size) {
    return Bitmaps::GetMiscBitmap(Bitmaps::ID_MISC::SORT_UP, size);
}
