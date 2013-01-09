/***************************************************************************
 siparam.cpp
 -------------------
 copyright            : (C) 2012 CREALP Lucien Schreiber
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

#include "siparam.h"

siParam::siParam(const wxString & separator) {
    m_Separator = separator;
}



siParam::~siParam() {
}



wxString siParam::GetParam(const wxString & text, const wxString & paramname, bool & error) {
    error = true;
    if (text.IsEmpty() == true) {
        return wxEmptyString;
    }
    
    if (paramname.IsEmpty()) {
        return wxEmptyString;
    }
    
    wxStringTokenizer tokenizer(text, m_Separator);
    int myCountToken = tokenizer.CountTokens();
    if (myCountToken < 2) {
        if (myCountToken == 1) {
            if(tokenizer.GetNextToken() == paramname){
                error = false;
            }
        }
        return wxEmptyString;
    }
        
    wxString myParamName = tokenizer.GetNextToken();
    wxString myParamValue = tokenizer.GetNextToken();
    
    if (myParamName != paramname) {
        return wxEmptyString;
    }

    error = false;
    return myParamValue;
}

wxString siParam::GetParamByCol(const wxString & text, int column, bool & error) {
    error = true;
    if (text.IsEmpty() == true) {
        return wxEmptyString;
    }
    
    wxStringTokenizer tokenizer(text, m_Separator);
    wxString myTemp = wxEmptyString;
    for (unsigned int i = 0; i<= column; i++) {
        myTemp = tokenizer.GetNextToken();
    }
    error = false;
    return myTemp;
}



int siParam::GetParamCount(const wxString & text) {
    if (text.IsEmpty() == true) {
        return 0;
    }
    
    wxStringTokenizer tokenizer(text, m_Separator);
    return tokenizer.CountTokens();
}



bool siParam::IsEmpty(const wxString & text) {
    if (text.IsEmpty()) {
        return true;
    }
    
    wxStringTokenizer tokenizer(text, m_Separator);
    while (tokenizer.HasMoreTokens()) {
        if (tokenizer.GetNextToken().IsEmpty() != true) {
            return false;
        }
    }
    return true;
}



bool siParam::GetRowIDs(const wxString & text, wxArrayLong & array) {
    array.Clear();
    
    if (text.IsEmpty()) {
        return false;
    }
    
    wxStringTokenizer tokenizer(text, m_Separator);
    // skip first column
    wxString myTemp = tokenizer.GetNextToken();
    while (tokenizer.HasMoreTokens()) {
        long myKindId = wxNOT_FOUND;
        wxString myText = tokenizer.GetNextToken();
        if(myText.ToLong(&myKindId)==false){
            wxLogError(_("Converting '%s' to long failed!"), myText);
        }
        array.Add(myKindId);
    }
    return true;
}




