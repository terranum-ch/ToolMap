/***************************************************************************
 tmimportdatawiz.cpp
 
 -------------------
 copyright            : (C) 2018 TERRANUM Pascal Horton
 email                : pascal.horton at terranum dot ch
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/


#include "tmimportdatawiz.h"
#include "../gis/tmimport.h"

ImportDataWizard::ImportDataWizard(wxWindow *window, wxWindowID id) :
        tmWizardImport::tmWizardImport(window, id, _("Import data"))
{

}

ImportDataWizard::~ImportDataWizard()
{

}

