/***************************************************************************
								textparser.cpp
		Superclass for parsing text, used for importing list of values
                             -------------------
    copyright            : (C) 2007 CREALP Lucien Schreiber 
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


#include "textparser.h"


// initialisation of static members
int TextParser::m_iActualLine = 0;

TextParser::TextParser()
{
	m_ParseFileType = _T("") ; //wxEmptyString;
	m_ParseFileName = wxFileName(_T("")); //wxEmptyString);
	m_LineCount = 0;
	m_NbFieldToParse = 0;
}


TextParser::~TextParser()
{
	
}


void TextParser::SetParseFileName (const wxString & myFileName)
{
	m_ParseFileName = wxFileName(myFileName);
}

void TextParser::SetParseFileName (const wxFileName & myFileName)
{
	m_ParseFileName = myFileName;
}

bool TextParser::CheckParseFileExist()
{
	//m_ParseFileName.IsFileReadable()
	
	if (wxFile::Exists(m_ParseFileName.GetFullPath()))
		return TRUE;
	return FALSE;
}

bool TextParser::OpenParseFile (bool bCreate)
{	
	//m_iAcutalLine = 0;
	return FALSE;
	
}

bool TextParser::CloseParseFile()
{
	return FALSE;
}

int TextParser::ParseNextLine (wxArrayString & myValues)
{
	//m_iAcutalLine = 0;
	return -1;
	
}


void TextParser::IncrementActualLineNumber (int iIncrement)
{
	m_iActualLine = m_iActualLine + iIncrement;
}


wxString TextParser::GetAllSupportedParserWildCards()
{
	wxString myReturnedWildcard = _T("");
	
	for (unsigned int i=0;i< (sizeof(TEXTPARSER_WILDCARDS) / sizeof (wxString)); i++)
	{
		myReturnedWildcard.Append(TEXTPARSER_WILDCARDS[i]);
		
		// append a separator if not the last turn
		if (i+1 < (sizeof(TEXTPARSER_WILDCARDS) / sizeof (wxString)))
			myReturnedWildcard.Append(_T("|"));
	}
	return myReturnedWildcard;
}


// static function that one can use to create the good parser based on
// the filetype selected
TextParser * TextParser::CreateParserBasedOnType (const int & textparser_index, const wxFileName & filename)
{
    // create parser depending on the selected format
	if (textparser_index == TXTFILE_COMMA){
		return  new TextParserTxtFileComma();
	}
	if (textparser_index == TXTFILE_TAB){
		return new TextParserTxtFileTab();
	}
    
    // try to guess parser based on extension
    if (filename.GetExt() == _T("csv")) {
        return  new TextParserTxtFileComma();
    }
    if (filename.GetExt() == _T("txt")) {
        return new TextParserTxtFileTab();
    }

    wxFAIL_MSG(_("No Parser Found!!!"));
	return NULL;
}

/***************************************************************************//**
 @brief Set the number of fields we are looking for
 @details This function must be called for letting the parser know how many
 fields must been found in the file. This number is used for checking the
 file.
 @note If this function isn't called, a message is issued in debug mode
 @param inbfield The number of fields whom must been present in the file
 @author Lucien Schreiber (c) CREALP 2008
 @date 19 May 2008
 *******************************************************************************/
void TextParser::SetNumberOfFields (int inbfield)
{
	m_NbFieldToParse = inbfield;
}


/*************************TEXT PARSER FOR TEXT FILES ************************/
TextParserTxtFile::TextParserTxtFile()
{
	InitParserValue();
}

TextParserTxtFile::TextParserTxtFile(const wxString & filename)
{
	InitParserValue();
	SetParseFileName(filename);
}

TextParserTxtFile::TextParserTxtFile(const wxFileName & filename)
{
	InitParserValue();
	SetParseFileName(filename);
}

TextParserTxtFile::~TextParserTxtFile()
{
	if (m_File != NULL)
		delete m_File;
}

void TextParserTxtFile::InitParserValue()
{
	m_ParseFileType = TEXTPARSER_TYPE_TXTFILE;
	m_File = NULL;
	m_TextSeparator = wxEmptyString;
	m_WriteMode = FALSE;
}

bool TextParserTxtFile::OpenParseFile (bool bCreate)
{
	// set the mode 
	m_WriteMode = bCreate;
	
	// set line number to zero
	InitActualLineNumber();
	
	// check that the file exist.
	// if the file exist, we open it 
	// otherwise if in edition mode we create
	// the new file.
	
	m_File = new wxTextFile(m_ParseFileName.GetFullPath());
	
	if (CheckParseFileExist())
	{
		m_File->Open();
		if (m_File->IsOpened())
		{
			m_LineCount = m_File->GetLineCount();
			return TRUE;
		}
	}
	else 
	{
		if (m_WriteMode == TRUE)
		{
			m_File->Create();
			m_File->Open();
			return TRUE;
		}
	}
	return FALSE;
}

bool TextParserTxtFile::CloseParseFile ()
{
	if (m_File != NULL)
	{
		if (m_WriteMode == TRUE)
		{
			m_File->Write();
			wxLogDebug(_T("File data written on disk, now closing"));
		}
		m_File->Close();
		return TRUE;
	}
	else
		wxLogDebug(_T("Trying to close a not open file"));
	return FALSE;

}


int TextParserTxtFile::ParseNextLine (wxArrayString & myValues)
{
	wxASSERT(!m_TextSeparator.IsEmpty());
	wxASSERT(m_File != NULL);
	
	// check that the file is open
	if (m_File->IsOpened())
	{
		
		wxString myValueToParse = _T("");
		
		// get the line based on the static int m_iActualLine
		myValueToParse =	m_File->GetLine(GetActualLineNumber());
		
		IncrementActualLineNumber();
		
		wxStringTokenizer tkz(myValueToParse, m_TextSeparator);
		while ( tkz.HasMoreTokens() ) 
		{ 
			// Add values from the array
			myValues.Add( tkz.GetNextToken());
		} 
	
		return myValues.GetCount();
	}
	return -1;
}

bool TextParserTxtFile::WriteNextLine (const wxArrayString & myValues)
{
	wxString myLine = _T("");
	int iSizeOfArray = myValues.GetCount();
	
	
	wxASSERT(!m_TextSeparator.IsEmpty());
	wxASSERT(m_File != NULL);
	
	
	if (m_File->IsOpened())
	{
		
		// clear the file if we are writting the first line
		// into the file. People don't expect to have data
		// added into an existing file
		if (m_iActualLine == 0)
		{
			m_File->Clear();
		}
		
		// preparing the line from the string array
		for (int i=0; i< iSizeOfArray; i++)
		{
			myLine.Append(myValues.Item(i));
			
			// if we run the last turn, we dont add the ending
			// separator (stored in m_TextSeparator)
			if (i+1 < iSizeOfArray)
			{
				myLine.Append(m_TextSeparator);
			}
		}
		
		
		// write the line to the file. Data will be only written when closing
		// the file.
		m_File->AddLine(myLine);
		
		IncrementActualLineNumber();
		return TRUE;
	}
	return FALSE;
}


/***************************************************************************//**
 @brief Check that the file has the correct separator 
 @note open the file first ! and specify the number of fields we are looking for
 using the TextParser::SetNumberOfFields() function.
 @author Lucien Schreiber (c) CREALP 2008
 @date 19 May 2008
 *******************************************************************************/
bool TextParserTxtFile::CheckFileToParse ()
{
	if (!m_File->IsOpened()) 
	{
		wxLogDebug(_T("No file open for parsing, please open file first"));
		return FALSE;
	}
	
	// debug check : m_NbFieldToParse must be set
	if(m_NbFieldToParse == 0)
	{
		wxLogDebug(_T("Number of fields not specified, parsing not allowed"));
		return FALSE;
	}
	
	// check that the file isn't empty
	wxString myTestLine = m_File->GetLine(0);
	if (myTestLine.IsEmpty())
	{
		wxLogError(_("File %s is empty, nothing to parse"), 
				   m_ParseFileName.GetFullName().c_str());
		return FALSE;
	}
	
	// check that we are using the good parser format
	if(myTestLine.Find(m_TextSeparator) == wxNOT_FOUND)
	{
		wxLogError(_("Format of %s seems to be incorrect. Parser is waiting for : %s"),
				   m_ParseFileName.GetFullName().c_str(),
				   GetParserType().c_str());
		return FALSE;
	}
	
	
	// check that we have the good number of fields in the file
	int iFoundNbField = myTestLine.Freq(m_TextSeparator[0]) + 1;
	if (iFoundNbField != m_NbFieldToParse)
	{
		wxLogError(_("Number of fields not correct : found %d field(s), waiting for %d field(s)"),
				   iFoundNbField, m_NbFieldToParse);
		return FALSE;
	}
	
	
	return TRUE;
}



/*************************TEXT PARSER FOR COMMA SEPARATED TXT FILES ************************/
TextParserTxtFileComma::TextParserTxtFileComma()
{
	InitParserValue();
}


TextParserTxtFileComma::TextParserTxtFileComma(const wxString & filename) 
: TextParserTxtFile(filename)
{
	InitParserValue();
}

void TextParserTxtFileComma::InitParserValue()
{
	m_ParseFileType = TEXTPARSER_NAME[TXTFILE_COMMA];
	m_TextSeparator = _T(";");
}






/*************************TEXT PARSER FOR TAB SEPARATED TXT FILES ************************/
TextParserTxtFileTab::TextParserTxtFileTab()
{
	InitParserValue();
}


TextParserTxtFileTab::TextParserTxtFileTab(const wxString & filename) 
: TextParserTxtFile(filename)
{
	InitParserValue();
}

void TextParserTxtFileTab::InitParserValue()
{
	m_ParseFileType = TEXTPARSER_NAME[TXTFILE_TAB];
	m_TextSeparator = _T("\t");
}


