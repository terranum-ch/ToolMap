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

TextParser::TextParser()
{
	m_ParseFileType = wxEmptyString;
	m_ParseFileName = wxFileName(wxEmptyString);
	//iAcutalLine = 0;
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
	if (m_ParseFileName.IsFileReadable())
		return TRUE;
	return FALSE;
}

bool TextParser::OpenParseFile ()
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
	m_iAcutalLine = m_iAcutalLine + iIncrement;
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
}

bool TextParserTxtFile::OpenParseFile ()
{
	// set line number to zero
	//m_iAcutalLine = 0;
	
	// check the file
	if (!CheckParseFileExist())
		return FALSE;
	
	// code for opening file
	m_File = new wxTextFile(m_ParseFileName.GetFullPath());
	m_File->Open();
	if (m_File->IsOpened())
	{
		return TRUE;
	}
	return FALSE;
}

bool TextParserTxtFile::CloseParseFile ()
{
	if (m_File != NULL)
	{
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
		myValueToParse =	m_File->GetLine(0);
		wxLogDebug(myValueToParse);
		
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
	m_ParseFileType = TEXTPARSER_TYPE_TXTFILE_COMMA;
	m_TextSeparator = _T(",");
}


