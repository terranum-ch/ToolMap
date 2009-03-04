//---------------------------------------------------------------------------
// $RCSfile: tmcimages.h $
// $Source: contrib/src/treemultictrl/tmcimages.h $
// $Revision: 1.83 $
// $Date: Jul 29, 2007 2:08:42 PM $
//---------------------------------------------------------------------------
// Author:      Jorgen Bodde
// Copyright:   (c) Jorgen Bodde
// License:     wxWidgets License
//---------------------------------------------------------------------------

#ifndef _TMCIMAGES_H_
#define _TMCIMAGES_H_

// pics for the expand / collapse buttons
#ifdef __APPLE__
  #include "mac_collapse.xpm"
  #include "mac_expand.xpm"
#else
  #ifdef LINUX
    #include "linux_collapse.xpm"
    #include "linux_expand.xpm"
  #else
    #include "default_collapse.xpm"
    #include "default_expand.xpm"
  #endif
#endif

#if(CHECKBOXVIEW)
  // icons for checkbox states
  #ifdef WINDOWS
    #include "win_unchecked_icon.xpm"
    #include "win_checked_icon.xpm"
    #include "win_tristate_icon.xpm"
  #else
    #include "default_unchecked_icon.xpm"
    #include "default_checked_icon.xpm"
    #include "default_tristate_icon.xpm"
  #endif // WINDOWS
#endif // #if(CHECKBOXVIEW)

#endif // _TMCIMAGES_H_
