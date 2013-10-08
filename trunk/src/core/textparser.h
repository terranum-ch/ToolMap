/***************************************************************************
								textparser.h
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

/***************************************************************************//**
 @file textparser.h
 @brief Super-class for parsing text files
 @details This super-class is used for parsing text from a file. It is used for
 importing data to list for exemples. Specialized class would be derived from
 this one to deals with specific file format (txt, csv and eventually dbf)
 @author Lucien Schreiber (c) CREALP 2007
 @date 11 December 2007
 *******************************************************************************/


#ifndef TEXTPARSER_H
#define TEXTPARSER_H

// For compilers that support precompilation, includes "wx/wx.h".
#include "wx/wxprec.h"

// Include wxWidgets' headers
#ifndef WX_PRECOMP
    #include <wx/wx.h>
#endif

#include <wx/filename.h>
#include <wx/textfile.h>
#include <wx/tokenzr.h>


#define TEXTPARSER_TYPE_TXTFILE _("Text file parser")

enum TEXTPARSER_TYPE
{
	TXTFILE_COMMA = 0,
	TXTFILE_TAB = 1
};

static wxString TEXTPARSER_WILDCARDS[] = 
		{wxTRANSLATE("Semi-colon delimited text files (*.csv)|*.csv"),
		wxTRANSLATE("Tab delimited text files (*.txt)|*.txt")};


static wxString TEXTPARSER_NAME[] = 
		{wxTRANSLATE("Semi-colon delimited text files"),
		wxTRANSLATE("Tab delimited text files")};

/***************************************************************************//**
 @brief Super-class for parsing different file
 @details Mainly used for parsing list of values. Actually, only comma separated
 file (TextParserComma) and point-comma files with the derived class
 (TextParser...). But it could be easily enhanced in the future with for
 exemple the DBase support
 @author Lucien Schreiber (c) CREALP 2007
 @date 11 December 2007
 *******************************************************************************/
class TextParser : public wxObject
	{
	protected:
		
		
		wxString m_ParseFileType;
		wxFileName m_ParseFileName;
		static int m_iActualLine;
		int m_LineCount;
		int m_NbFieldToParse;
		
		bool CheckParseFileExist();
		
		
		
	public:
		
		
		
		TextParser();
		~TextParser();
		
		wxString GetParserType() {return m_ParseFileType;}
		void SetParseFileName (const wxString & myFileName);
		void SetParseFileName(const wxFileName & myFileName);
		virtual bool OpenParseFile (bool bCreate = FALSE);
		virtual int ParseNextLine (wxArrayString & myValues);
		virtual bool WriteNextLine (const wxArrayString & myValues){return TRUE;}
		virtual bool CloseParseFile ();
		int GetActualLineNumber () {return m_iActualLine;}
		void InitActualLineNumber () {m_iActualLine = 0;}
		inline void IncrementActualLineNumber (int iIncrement = 1);
		int GetLineCount () {return m_LineCount;}
		static wxString GetAllSupportedParserWildCards();
		static TextParser * CreateParserBasedOnType (const int & textparser_index, const wxFileName & filename);
		virtual bool CheckFileToParse (){return TRUE;}
		void SetNumberOfFields (int inbfield);
	};



/***************************************************************************//**
 @brief for parsing text based  file
 @details It reads a simple textual file and parse the values found on a line by
 line basis.
 @author Lucien Schreiber (c) CREALP 2007
 @date 11 December 2007
 *******************************************************************************/
class TextParserTxtFile : public TextParser
	{
	protected:
		wxTextFile  * m_File;
		wxString m_TextSeparator;
		virtual void InitParserValue();
		bool m_WriteMode;
		
		
	public:
		TextParserTxtFile();
		TextParserTxtFile(const wxString & filename);
		TextParserTxtFile(const wxFileName & filename);
		~TextParserTxtFile();
		
		virtual bool OpenParseFile(bool bCreate = FALSE);
		virtual bool CloseParseFile ();
		virtual int ParseNextLine (wxArrayString & myValues);
		virtual bool WriteNextLine (const wxArrayString & myValues);
		virtual bool CheckFileToParse ();
		
	};



class TextParserTxtFileComma : public TextParserTxtFile
	{
	protected:
		 virtual void InitParserValue();
	public:
		TextParserTxtFileComma();
		TextParserTxtFileComma(const wxString & filename);
		~TextParserTxtFileComma(){;}
	
	};



class TextParserTxtFileTab : public TextParserTxtFile
	{
	protected:
		virtual void InitParserValue();
	public:
		TextParserTxtFileTab();
		TextParserTxtFileTab(const wxString & filename);
		~TextParserTxtFileTab(){;}
		
	};


#endif
