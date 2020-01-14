/////////////////////////////////////////////////////////////////////////////
// Name:
// Purpose:
// Author:      Lucien Schreiber
// Modified by:
// Created:     Mon  6 Aug 15:31:16 2007
// RCS-ID:
// Copyright:   (c) CREALP 2007
// Licence:
/////////////////////////////////////////////////////////////////////////////

#ifndef _APP_RESOURCES_H_
#define _APP_RESOURCES_H_

/*!
 * Control identifiers
 */

////@begin control identifiers
#define ID_MENU_NEW_PRJ_EMPTY 10019
#define ID_MENU_NEW_PRJ_EXISTING 10148
#define ID_MENU_OPEN_PRJ 10020
#define ID_MENU3 10126
#define ID_MENU_BACKUP_PRJ 10021
#define ID_MENU_RESTORE_PRJ 10110
#define ID_MENU_EXPORT_LAYER 10033
#define ID_MENU_EXPORT_FULL 10034
#define ID_MENU_PRJ_DEF 10229
#define ID_MENU_OBJ_DEF 10230
#define ID_MENU_PRJ_SETTINGS 10175
#define ID_MENU_ADD_SPATIAL_DATA 10022
#define ID_MENU_IMPORT_GIS_DATA 10112
#define ID_MENU_ZOOM_PREVIOUS_EXTEND 10023
#define ID_MENU_ZOOM 10164
#define ID_MENU_PAN 10165
#define ID_MENU_ZOOM_FIT 10113
#define ID_MENU_ZOOM_GIVEN_SCALE 10120
#define ID_MENU_ZOOM_COORDINATE 10210
#define ID_MENU_ZOOM_NEXT_SCALE 10184
#define ID_MENU_PREVIOUS_SCALE 10185
#define ID_MENU_SELECTION 10131
#define ID_MENU_SELECTED_LAYER 10208
#define ID_MENU_UNDO 10179
#define ID_MENU_REDO 10180
#define ID_MENU_DRAW 10176
#define ID_MENU_MODIFY 10177
#define ID_MENU_CUT_LINES 10178
#define ID_MENU_EDIT_VERTEX_POS 10211
#define ID_MENU_MERGE_LINES 10003
#define ID_MENU_CREATE_INTERSECTIONS 10174
#define ID_MENU_FLIP_LINE 10004
#define ID_MENU_DELETE_OBJ 10122
#define ID_MENU_ADJUST_SNAPPING 10001
#define ID_MENU_ATTRIB_TYPES 10166
#define ID_MENU_ATTRIB_ATTRIBUTES 10175
#define ID_MENU_ATTRIB_BATCH 10167
#define ID_MENU_COPY_PASTE_ATTRIB 10231
#define ID_MENU_SHORTCUTS 10183
#define ID_MENU_CHECK_GEOM 10123
#define ID_MENU_CHECK_DANGLING 10048
#define ID_MENU_SELECT 10163
#define ID_MENU_SELECT_ALL 10162
#define ID_MENU_SELECT_NONE 10160
#define ID_MENU_SELECT_INVERSE 10161
#define ID_MENU_QUERIES 10019
#define ID_MENUITEM11 10046
#define ID_MENUITEM12 10047
#define ID_MENU_TOC_WINDOW 10037
#define ID_MENU_INFO_WINDOW 10140
#define ID_MENU_LOG_WINDOW 10207
#define ID_MENU_CHECK_UPDATE 10125
#define ID_MENUITEM17 10077
#define ID_MENUITEM14 10072
#define ID_MENUITEM20 10111
#define ID_MENU6 10075
#define ID_MENUITEM16 10076
#define ID_MENUITEM45 10181
#define ID_MENUITEM46 10182
#define ID_MENUITEM18 10078
////@end control identifiers

class AppResources {
 public:
  AppResources() {}

  /*!
   * Resource functions
   */

  ////@begin AppResources resource functions
  /// Menubar creation function for ID_MENU_MAIN
  static wxMenuBar* CreateMenuMenubar();

  /// Menu creation function for ID_MENU
  static wxMenu* CreateMenuMenu();

  /// Retrieves bitmap resources
  static wxBitmap GetBitmapResource(const wxString& name);

  /// Retrieves icon resources
  static wxIcon GetIconResource(const wxString& name);
  ////@end AppResources resource functions
};

#endif
// _APP_RESOURCES_H_
