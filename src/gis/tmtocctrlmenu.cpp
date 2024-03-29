/***************************************************************************
 tmtocctrlmenu.cpp
 Display contextual menu for item in the TOC Ctrl
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

// comment doxygen

#include "tmtocctrlmenu.h"

#include "tmlayerproperties.h"

tmTOCCtrlMenu::tmTOCCtrlMenu(tmLayerProperties* item, int pos, int nbItems)
    : wxMenu() {
    m_LayerProperties = item;
    m_SelectedPos = pos;

    if (nbItems > 1) {
        return;
    }

    // create menu
    if (m_LayerProperties == nullptr) {
        _CreateTOCAddGroup();
    } else if (m_LayerProperties->GetType() == TOC_NAME_GROUP) {
        _CreateTOCAddGroup();
        _CreateTOCRenameGroup();
        _CreateTOCRemoveGroup();
    } else {
        _CreateTOCBasic();
        _CreateTOCShowVertex();
        _CreateTOCProperties();
    }
}

tmTOCCtrlMenu::~tmTOCCtrlMenu() {}

void tmTOCCtrlMenu::_CreateTOCAddGroup() {
    Append(ID_TOCMENU_ADD_GROUP, _("Add new group"));
}

void tmTOCCtrlMenu::_CreateTOCRenameGroup() {
    Append(ID_TOCMENU_RENAME_GROUP, _("Rename group"));
}

void tmTOCCtrlMenu::_CreateTOCRemoveGroup() {
    Append(ID_TOCMENU_REMOVE_GROUP, _("Remove group"));
}

void tmTOCCtrlMenu::_CreateTOCBasic() {
    wxASSERT(m_LayerProperties);
    if (m_LayerProperties->GetType() > TOC_NAME_NOT_GENERIC) {
        Append(ID_TOCMENU_REMOVE, _("Remove layer"));
    } else {
        AppendCheckItem(ID_TOCMENU_EDIT_LAYER, _("Edit layer"));
        Check(ID_TOCMENU_EDIT_LAYER, m_LayerProperties->IsEditing());
    }
}

void tmTOCCtrlMenu::_CreateTOCShowVertex() {
    wxASSERT(m_LayerProperties);
    int myflags = (tmDRAWING_FLAGS)m_LayerProperties->GetVertexFlags();
    switch (m_LayerProperties->GetSpatialType()) {
        case LAYER_SPATIAL_LINE:
        case LAYER_SPATIAL_POLYGON: {
            wxMenu* menushow = new wxMenu();
            menushow->AppendCheckItem(ID_TOCMENU_SHOW_VERTEX_NONE, _("None"));
            menushow->AppendCheckItem(ID_TOCMENU_SHOW_VERTEX_ALL, _("All"));
            menushow->AppendCheckItem(ID_TOCMENU_SHOW_VERTEX_BEGIN_END, _("Begin/End"));
            menushow->Check(ID_TOCMENU_SHOW_VERTEX_NONE + myflags, true);
            Append(wxID_ANY, _("Show vertex"), menushow);
            break;
        }
        case LAYER_SPATIAL_RASTER:
        case LAYER_SPATIAL_POINT: {
            // do nothing :-) nice no !
            break;
        }

        default:
            wxLogError(_T("Unknown spattype = %d, case note supported"), m_LayerProperties->GetSpatialType());
            break;
    }
}

void tmTOCCtrlMenu::_CreateTOCProperties() {
    wxASSERT(m_LayerProperties);
    if (m_LayerProperties->GetType() == TOC_NAME_SHP) {
        Append(ID_TOCMENU_LABELS, _("Labels..."));
        AppendSeparator();
        Append(ID_TOCMENU_PROPERTIES_SAVE, _("Save Symbology..."));
        Append(ID_TOCMENU_PROPERTIES_LOAD, _("Load Symbology..."));
    }
    Append(ID_TOCMENU_PROPERTIES, _("Symbology..."));
}
