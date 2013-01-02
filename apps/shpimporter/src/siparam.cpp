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
    if (myCountToken != 2) {
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

