///////////////////////////////////////////////////////////////////////////
// C++ code generated with wxFormBuilder (version Jan 23 2018)
// http://www.wxformbuilder.org/
//
// PLEASE DO *NOT* EDIT THIS FILE!
///////////////////////////////////////////////////////////////////////////

#ifndef __WIZARD_IMPORT_DATA_H__
#define __WIZARD_IMPORT_DATA_H__

#include <wx/artprov.h>
#include <wx/xrc/xmlres.h>
#include <wx/string.h>
#include <wx/stattext.h>
#include <wx/gdicmn.h>
#include <wx/font.h>
#include <wx/colour.h>
#include <wx/settings.h>
#include <wx/radiobut.h>
#include <wx/sizer.h>
#include <wx/bitmap.h>
#include <wx/image.h>
#include <wx/icon.h>
#include <wx/filepicker.h>
#include <wx/statbox.h>
#include <wx/choice.h>
#include <wx/scrolwin.h>
#include <wx/wizard.h>
#include <wx/dynarray.h>
WX_DEFINE_ARRAY_PTR( wxWizardPageSimple*, WizardPages );

///////////////////////////////////////////////////////////////////////////


///////////////////////////////////////////////////////////////////////////////
/// Class tmWizardImport
///////////////////////////////////////////////////////////////////////////////
class tmWizardImport : public wxWizard 
{
	private:
	
	protected:
		wxStaticText* m_staticText1;
		wxRadioButton* m_radioBtnSHP;
		wxRadioButton* m_radioBtnCSV;
		wxBoxSizer* m_sizerFilePath;
		wxStaticText* m_staticText3;
		wxFilePickerCtrl* m_filePicker;
		wxStaticText* m_infoLabelCtrl1;
		wxStaticText* m_infoValueCtrl1;
		wxStaticText* m_infoLabelCtrl2;
		wxStaticText* m_infoValueCtrl2;
		wxStaticText* m_staticText9;
		wxStaticText* m_staticText11;
		wxChoice* m_choiceX;
		wxStaticText* m_staticText12;
		wxChoice* m_choiceY;
		wxStaticText* m_staticText91;
		wxStaticText* m_staticText101;
		wxChoice* m_choiceTarget;
		wxStaticText* m_staticText13;
		wxChoice* m_choiceLayer;
		wxStaticText* m_staticText14;
		wxScrolledWindow* m_scrolledWindow1;
		wxFlexGridSizer* m_fgSizerAttributes;
		wxStaticText* m_staticText141;
		wxScrolledWindow* m_scrolledWindow2;
		wxFlexGridSizer* m_fgSizerEnums;
		
		// Virtual event handlers, overide them in your derived class
		virtual void OnWizardFinished( wxWizardEvent& event ) { event.Skip(); }
		virtual void OnFileChanged( wxFileDirPickerEvent& event ) { event.Skip(); }
		
	
	public:
		
		tmWizardImport( wxWindow* parent, wxWindowID id = wxID_ANY, const wxString& title = wxEmptyString, const wxBitmap& bitmap = wxNullBitmap, const wxPoint& pos = wxDefaultPosition, long style = wxDEFAULT_DIALOG_STYLE );
		WizardPages m_pages;
		~tmWizardImport();
	
};

#endif //__WIZARD_IMPORT_DATA_H__
