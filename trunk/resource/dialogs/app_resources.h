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
#define ID_MENU4 10147
#define ID_MENUITEM 10019
#define ID_MENUITEM30 10148
#define ID_MENUITEM1 10020
#define ID_MENU3 10126
#define ID_MENUITEM2 10021
#define ID_MENUITEM19 10110
#define ID_MENU1 10035
#define ID_MENUITEM6 10033
#define ID_MENUITEM7 10034
#define ID_MENU8 10145
#define ID_MENUITEM55 10229
#define ID_MENUITEM56 10230
#define ID_MENUITEM39 10175
#define ID_MENUITEM28 10127
#define ID_MENUITEM3 10022
#define ID_MENUITEM21 10112
#define ID_MENUITEM4 10023
#define ID_MENUITEM35 10164
#define ID_MENUITEM36 10165
#define ID_MENUITEM22 10113
#define ID_MENUITEM23 10120
#define ID_MENUITEM52 10210
#define ID_MENUITEM48 10184
#define ID_MENUITEM49 10185
#define ID_MENUITEM8 10131
#define ID_MENUITEM51 10208
#define ID_MENUITEM43 10179
#define ID_MENUITEM44 10180
#define ID_MENUITEM40 10176
#define ID_MENUITEM41 10177
#define ID_MENUITEM42 10178
#define ID_MENUITEM53 10211
#define ID_MENUITEM14 10003
#define ID_MENUITEM26 10174
#define ID_MENUITEM15 10004
#define ID_MENUITEM24 10122
#define ID_MENUITEM5 10001
#define ID_MENUITEM37 10166
#define ID_MENUITEM38 10167
#define ID_MENUITEM54 10231
#define ID_MENUITEM47 10183
#define ID_MENUITEM25 10123
#define ID_MENUITEM13 10048
#define ID_MENU 10000
#define ID_MENUITEM34 10163
#define ID_MENUITEM33 10162
#define ID_MENUITEM31 10160
#define ID_MENUITEM32 10161
#define ID_MENUITEM11 10046
#define ID_MENUITEM12 10047
#define ID_MENUITEM9 10037
#define ID_MENUITEM29 10140
#define ID_MENUITEM50 10207
#define ID_MENUITEM27 10125
#define ID_MENUITEM17 10077
#define ID_MENUITEM14 10072
#define ID_MENUITEM20 10111
#define ID_MENU6 10075
#define ID_MENUITEM16 10076
#define ID_MENUITEM45 10181
#define ID_MENUITEM46 10182
#define ID_MENUITEM18 10078
////@end control identifiers

class AppResources
{
public:
    AppResources() {}

/*!
 * Resource functions
 */

////@begin AppResources resource functions
    /// Menubar creation function for ID_MENUBAR
    static wxMenuBar* CreateMenuMenubar();

    /// Menu creation function for ID_MENU
    static wxMenu* CreateMenuMenu();

    /// Retrieves bitmap resources
    static wxBitmap GetBitmapResource( const wxString& name );

    /// Retrieves icon resources
    static wxIcon GetIconResource( const wxString& name );
////@end AppResources resource functions

};

#endif
    // _APP_RESOURCES_H_
