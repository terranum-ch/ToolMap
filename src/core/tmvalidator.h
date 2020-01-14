/***************************************************************************
 tmvalidator.h
 Create various validator
 -------------------
 copyright : (C) 2007 CREALP Lucien Schreiber
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/



#ifndef _TMVALIDATOR_H_
#define _TMVALIDATOR_H_



// For compilers that support precompilation, includes "wx/wx.h".
#include "wx/wxprec.h"

// Include wxWidgets' headers
#ifndef WX_PRECOMP
    #include <wx/wx.h>
#endif

#define tmFILTER_EXCLUDE_CHAR_RESTRICTED 0x0100
#define tmFILTER_EXCLUDE_CHAR_DATABASE 0x0200
#define tmFILTER_EXCLUDE_CHAR_NUMERIC_STRICT 0x0400
#define tmFILTER_EXCLUDE_CHAR_COMPLEX 0x0800


/***************************************************************************//**
 @brief Validator for text controls
 @details This class may be used for controls validations. It extend the
 wxTextValidator with some more strict styles (see tmValidator::tmValidator()
 for allowed styles).
 Exemple :
 @code
 wxTextCtrl myCtrl;
 myCtrl.SetValidator(tmValidator(tmFILTER_EXCLUDE_CHAR_RESTRICTED));
 @endcode
 @author Lucien Schreiber (c) CREALP 2008
 @date 20 May 2008
 *******************************************************************************/
class tmValidator : public wxTextValidator
{
private:
    void SetCharRestricted();

    void SetCharDataBase();

    void SetCharNumericStric();


    void GetAlphaList(wxArrayString &mylist);

    void GetStrictNumList(wxArrayString &mylist);

    void GetComplexList(wxArrayString &mylist);

    bool SetEnhancedStyle(long style);

    void GetExcludeChars(wxArrayString &mylist);

public:
    tmValidator(long style = wxFILTER_NONE);
};


#endif
