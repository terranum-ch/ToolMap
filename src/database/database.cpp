/***************************************************************************
								database.cpp
				Basic function for connecting a MySQL embedded database
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

/*!
    @file database.cpp
    @brief code for the class DataBase

	The Database class is used for Opening, creating and processing request
	to a MySQL embedded Database.
    @author Lucien Schreiber (c) CREALP 2007
*/


#if defined(__GNUG__) && !defined(NO_GCC_PRAGMA)
#pragma implementation "database.cpp"
#endif

// For compilers that support precompilation, includes "wx/wx.h".
#include "wx/wxprec.h"
#ifdef __BORLANDC__
#pragma hdrstop
#endif
#include <wx/filename.h>    // to create the database path and name.
#include <wx/stdpaths.h>	// std path for logging too.

#include "database.h"
#include "databaseresult.h"
#include "database-config.h" // for logging



DataBase::DataBase(const wxString & errmsgpath)
{
	m_IsDatabaseOpened		= false;
	m_IsLibraryStarted		= false;
	m_MySQL					= NULL;
	m_MySQLRes				= NULL;
	m_DBName				= wxEmptyString;
	m_DBPath				= wxEmptyString;
    m_ErrMsgPath            = errmsgpath;
}



DataBase::~DataBase()
{
	if (DBResultsNotNull()==true)
		DataBaseClearResults();

	if (m_IsLibraryStarted)
	{
		DBLibraryEnd();
		m_IsLibraryStarted = false;
	}
}



bool DataBase::DBLibraryInit (const wxString & datadir){
	// path validity
	wxFileName myValidPath (datadir, _T(""));
	if (myValidPath.IsDirReadable()==false){
		wxLogError(_("Directory : %s doesn't exists or isn't readable"),datadir.c_str());
		return false;
	}

	// get application path
	wxFileName f(wxStandardPaths::Get().GetExecutablePath());
	wxFileName windows_msg_path(f.GetPath(), "mysql", "");

	//init library
	wxString myDatadir = _T("--datadir=") + myValidPath.GetPath(wxPATH_GET_VOLUME,wxPATH_NATIVE);
#ifdef MYSQL_IS_LOGGING
	wxFileName myLogDirName (wxStandardPaths::Get().GetAppDocumentsDir(),_T("toolmap_mysql_log.sql"));
	wxString myLogDirString = _T("--general-log-file=");
	myLogDirString.Append(myLogDirName.GetFullPath());
#endif
    wxString mylanguagedir = wxEmptyString;
    if (m_ErrMsgPath != wxEmptyString) {
        mylanguagedir = _T("--lc-messages-dir=") + m_ErrMsgPath;
    }
    else
    {

#if defined(__WINDOWS__)
		//mylanguagedir = "--lc-messages-dir=./mysql";
		mylanguagedir = "--lc-messages-dir=" + windows_msg_path.GetFullPath();
#elif defined(__WXMAC__)
        mylanguagedir = "--lc-messages-dir=./ToolMap.app/Contents/mysql";
#else
        // Linux standard with MySQL installed with package manager.
        mylanguagedir = "--skip-grant-tables";
#endif
    }

	char const *server_args[] =
	{
		"this_program",       /* this string is not used*/
		myDatadir.mb_str(wxConvUTF8),
		mylanguagedir.mb_str(wxConvUTF8),
		"--character-set-server=utf8",
        "--default-storage-engine=MyISAM",
#ifdef __LINUX__  // Linux still uses version 5.5
        "--skip-innodb"
#else  // this is needed for version 5.6
        "--default_tmp_storage_engine=MyISAM"
#endif

#if defined (MYSQL_IS_LOGGING)
        ,"--general-log=1"
        ,myLogDirString.mb_str(wxConvUTF8)
#endif
	};


	char const *server_groups[] ={
		"embedded",
		"server",
		"this_program_SERVER",
		(char *)NULL
	};

	int num_elements = (sizeof(server_args) / sizeof(char *));
	int myReturn = mysql_library_init(num_elements, const_cast<char**>(server_args), const_cast<char**>(server_groups));
	if (myReturn != 0){
		wxLogError(DataBaseGetLastError());
		return false;
	}

	m_MySQL = mysql_init(NULL);
	mysql_options(m_MySQL, MYSQL_OPT_USE_EMBEDDED_CONNECTION, NULL);
	mysql_options(m_MySQL, MYSQL_SET_CHARSET_NAME, "utf8");
	return true;
}



bool DataBase::DBUseDataBase(const wxString & dbname){
    if(mysql_real_connect(m_MySQL,NULL,NULL,NULL,(const char *) dbname.mb_str(wxConvUTF8),
						  3309,NULL,CLIENT_MULTI_STATEMENTS) == NULL){
		wxLogError(DataBaseGetLastError());
		return false;
	}

	if (dbname != wxEmptyString){
		wxLogMessage(_("Opening database : ") + dbname);
    }
	return true;
}



void DataBase::DBLibraryEnd ()
{
	m_DBName = wxEmptyString;
	m_DBPath = wxEmptyString;

	mysql_close(m_MySQL);
	mysql_library_end();
}



wxString DataBase::DataBaseGetLastError (){
	return wxString::Format(_("MySQL Error : %s"), wxString(mysql_error(m_MySQL)));
}


bool DataBase::DataBaseCreateNew(const wxString & datadir, const wxString & name)
{
	if (m_IsLibraryStarted)
	{
		DBLibraryEnd();
		m_IsLibraryStarted=false;
	}

	m_IsLibraryStarted = DBLibraryInit(datadir);
	if (m_IsLibraryStarted == false)
		return false;

	m_IsDatabaseOpened = DBUseDataBase(wxEmptyString);
	if (m_IsDatabaseOpened == false)
		return false;

	wxString myDBNewQuery (name);
	myDBNewQuery.Prepend(_T("CREATE DATABASE "));
	myDBNewQuery.Append (_T(" DEFAULT CHARACTER SET utf8 COLLATE utf8_general_ci;"));
    if(DataBaseQueryNoResults(myDBNewQuery)==false)
	{
		wxLogError(_("Error creating database : ") + name);
		return false;
	}

    return DataBaseOpen(datadir, name);
}


bool DataBase::DataBaseOpen(const wxString & datadir, const wxString & name)
{
	if (m_IsLibraryStarted)
	{
		DBLibraryEnd();
		m_IsLibraryStarted = false;
	}

	m_IsLibraryStarted = DBLibraryInit(datadir);
	if (m_IsLibraryStarted == false)
		return false;

	m_IsDatabaseOpened = DBUseDataBase(name);
	if (m_IsDatabaseOpened == false)
		return false;

	m_DBName = name;
	m_DBPath = datadir;

	return true;
}


bool DataBase::DataBaseDelete()
{
	if (DBIsDataBaseReady() == false)
		return false;
    wxLogMessage(_T("DROP DATABASE ") + DataBaseGetName());
	if (DataBaseQuery(_T("DROP DATABASE ") + DataBaseGetName())==false)
		return false;

	long ldeltables = wxNOT_FOUND;
	if (DataBaseGetNextResult(ldeltables)==true){
		wxLogDebug(_T("%ld tables deleted"), ldeltables);
    }

	DataBaseClearResults();
	return true;
}



wxString DataBase::DataBaseGetName ()
{
	return m_DBName;
}


wxString DataBase::DataBaseGetPath ()
{
	return m_DBPath;
}



wxString DataBase::DataBaseGetSize (int precision, const wxString & failmsg)
{
	if (DBIsDataBaseReady() == false)
		return failmsg;

	wxFileName myDBFileName (DataBaseGetPath(), DataBaseGetName());
	if (myDBFileName.IsOk()==false)
		return failmsg;

	wxArrayString mySkipedFiles;
	wxULongLong myDBDirSize = wxDir::GetTotalSize(myDBFileName.GetFullPath(), &mySkipedFiles);
	if (mySkipedFiles.GetCount() > 0){
		wxLogMessage(_T("%d files skipped, size may not be 100% accurate"), mySkipedFiles.GetCount());
    }


	wxString myDBStringSize = wxFileName::GetHumanReadableSize(myDBDirSize, failmsg, precision);
	return 	myDBStringSize;
}



wxString DataBase::DataBaseGetVersion ()
{
	return wxString(mysql_get_client_info(), wxConvUTF8);
}

/***************************************************************************//**
 @brief Init internal variables for new thread
 @details Call this function to init internal values for each new
 thread.
 @return TRUE if thread init success, FALSE in case of faillure. An
 LogDebug message is also issued
 @author Lucien Schreiber (c) CREALP 2008
 @date 24 July 2008
 *******************************************************************************/
bool DataBase::DataBaseThreadInit()
{
	if (mysql_thread_init() == 0)
	{
		return true;
	}
	else
	{
		// try to get the last error
		wxLogError(DataBaseGetLastError());
		return false;
	}
}


/***************************************************************************//**
 @brief End internal variables for new thread
 @details Call this function for each thread you have called
 DataBase::DataBaseNewThreadInit()
 @author Lucien Schreiber (c) CREALP 2008
 @date 24 July 2008
 *******************************************************************************/
void DataBase::DataBaseThreadEnd()
{
	mysql_thread_end();
}



bool DataBase::DataBaseHasResults()
{
	if (DBResultsNotNull()==false)
		return false;

	long lrow = 0;
	if (DataBaseGetResultSize(NULL, &lrow)==false)
	{
		wxLogError(_T("Unable to compute number of results"));
	}

	if (lrow <= 0)
	{
		DataBaseClearResults();
		return false;
	}

	return true;
}


void DataBase::DataBaseClearResults()
{
	if (m_MySQLRes != NULL)
	{
		mysql_free_result(m_MySQLRes);
		m_MySQLRes = NULL;
	}
}


bool DataBase::DataBaseGetResultSize (unsigned int * pcols, long * prows)
{
	if (DBIsDataBaseReady()==false)
		return false;

	if (DBResultsNotNull() == false)
		return false;

	if (pcols != NULL)
		*pcols = mysql_num_fields(m_MySQLRes);

	if (prows != NULL)
		*prows = mysql_num_rows(m_MySQLRes);

	return true;
}


bool DataBase::DataBaseGetNextResult(wxString & result)
{
	result = wxEmptyString;

	MYSQL_ROW record = NULL;
	if (DBGetNextRecord(record)==false)
		return false;

	result = wxString ( record[0], wxConvUTF8);
	return true;
}




bool DataBase::DataBaseGetNextResult(wxArrayString & results)
{
	results.Clear();

	MYSQL_ROW record = NULL;
	if (DBGetNextRecord(record)==false)
		return false;

	unsigned int myCols = 0;
	DataBaseGetResultSize(&myCols, NULL);
	if (myCols == 1)
	{
		wxLogDebug(_T("Only one columns returned, use the ")
				   _T("DataBaseGetNextResult(wxString) function instead of this one"));
	}

	for (unsigned int i = 0; i< myCols; i++)
	{
		results.Add(wxString ( record[i], wxConvUTF8));
	}
	return true;
}



bool DataBase::DataBaseGetNextResult(long & result)
{
	result = wxNOT_FOUND;

	MYSQL_ROW record = NULL;
	if (DBGetNextRecord(record)==false)
		return false;

	result = atol(record[0]);
	return true;
}


bool DataBase::DataBaseGetNextResult(wxArrayLong & results)
{
	results.Clear();

	MYSQL_ROW record = NULL;
	if (DBGetNextRecord(record)==false)
		return false;

	unsigned int myCols = 0;
	DataBaseGetResultSize(&myCols, NULL);
	if (myCols == 1)
	{
		wxLogDebug(_T("Only one columns returned, use the ")
				   _T("DataBaseGetNextResult(long) function instead of this one"));
	}

	for (unsigned int i = 0; i< myCols; i++)
	{
		results.Add(atol(record[i]));
	}
	return true;
}


bool DataBase::DataBaseGetNextResult(wxDouble & result)
{
	result = 0.0;

	MYSQL_ROW record = NULL;
	if (DBGetNextRecord(record)==false)
		return false;
    wxString myStringDouble (record[0]);
    myStringDouble.ToCDouble(&result);
	return true;
}



bool DataBase::DataBaseGetNextResult(wxArrayDouble & results)
{
	results.Clear();

	MYSQL_ROW record = NULL;
	if (DBGetNextRecord(record)==false)
		return false;

	unsigned int myCols = 0;
	DataBaseGetResultSize(&myCols, NULL);
	if (myCols == 1)
	{
		wxLogDebug(_T("Only one columns returned, use the ")
				   _T("DataBaseGetNextResult(double) function instead of this one"));
	}

	for (unsigned int i = 0; i< myCols; i++)
	{
		wxDouble myDouble = 0.0;
        wxString myDoubleString (record[i]);
        myDoubleString.ToCDouble(&myDouble);
        results.Add(myDouble);
	}
	return true;
}




bool DataBase::DataBaseGetNextRowResult(MYSQL_ROW & row, tmArrayULong & lengths)
{
	row = NULL;
	lengths.Clear();



	if (DBGetNextRecord(row)==false)
		return false;

	unsigned int myNumFields = mysql_field_count(m_MySQL);
	wxASSERT(myNumFields > 0);

	unsigned long * myTempLength = mysql_fetch_lengths(m_MySQLRes);
	wxASSERT(*myTempLength != 0);

	for (unsigned int i =  0; i<myNumFields;i++)
		lengths.Add(myTempLength[i]);

	//delete [] myTempLength; (not needed ?)
	return true;
}



bool DataBase::DataBaseGetResults(wxArrayString & results)
{
	results.Clear();
	MYSQL_ROW record = NULL;

	while (1)
	{
		if (DBGetNextRecord(record)==false)
			break;

		results.Add(wxString ( record[0], wxConvUTF8));
	}

	DataBaseClearResults();
	switch (results.GetCount())
	{
		case 0:
			return false;
			break;

		case 1:
			//wxLogDebug(_T("Only one value returned, maybe should use the")
			//		   _T(" DataBaseGetNextResult(wxString) function instead ?"));
			break;

		default:
			break;
	}
	return true;
}



bool DataBase::DataBaseGetResults(wxArrayLong & results)
{
	results.Clear();
	MYSQL_ROW record = NULL;

	while (1)
	{
		if (DBGetNextRecord(record)==false)
			break;

		results.Add(atol(record[0]));
	}


	DataBaseClearResults();
	switch (results.GetCount())
	{
		case 0:
			return false;
			break;

		case 1:
			//wxLogDebug(_T("Only one value returned, maybe should use the")
			//		   _T(" DataBaseGetNextResult(long) function instead ?"));
			break;

		default:
			break;
	}
	return true;
}



bool DataBase::DataBaseGetResults(wxArrayDouble & results)
{
	results.Clear();
	MYSQL_ROW record = NULL;

	while (1)
	{
		if (DBGetNextRecord(record)==false)
			break;

		results.Add(atof(record[0]));
	}


	DataBaseClearResults();
	switch (results.GetCount())
	{
		case 0:
			return false;
			break;

		case 1:
			//wxLogDebug(_T("Only one value returned, maybe should use the")
			//		   _T(" DataBaseGetNextResult(double) function instead ?"));
			break;

		default:
			break;
	}
	return true;
}



bool DataBase::DataBaseGetResults(DataBaseResult * results){

	if (DBIsDataBaseReady()==false)
		return false;

	if (DBResultsNotNull()==false)
		return false;

	wxASSERT(results);
	wxASSERT(m_MySQLRes);
	results->Create(&m_MySQLRes);
	return true;
}




bool DataBase::DataBaseQueryNoResults(const wxString & query, bool logerror)
{
	if (DBIsDataBaseReady() == false)
		return false;

	if (DBResultsNotNull())
	{
		wxASSERT_MSG(0, _T("Not able to run query, results were not cleared"));
		wxLogError(_("Not able to run query, results were not cleared"));
		return false;
	}

	if (query.IsEmpty()){
		wxLogError (_("Trying to run empty query"));
		return false;
	}

	if (mysql_query(m_MySQL, query.mb_str(wxConvUTF8)) != 0)
	{
		if (logerror == true) {
			wxLogError(DataBaseGetLastError());
		}
		else {
			wxLogMessage(DataBaseGetLastError());
		}
		return false;
	}

	m_MySQLRes = mysql_store_result(m_MySQL);
	DataBaseClearResults();
	return true;
}



bool DataBase::DataBaseQuery (const wxString & query, bool logerror)
{
	if (DBIsDataBaseReady() == false)
		return false;

	if (DBResultsNotNull())
	{
		wxASSERT_MSG(0, _T("Not able to run query, results were not cleared"));
		wxLogError(_("Not able to run query, results were not cleared"));
		return false;
	}

	if (mysql_query(m_MySQL, query.mb_str(wxConvUTF8)) != 0)
	{
        if (logerror == true){
            wxLogError(DataBaseGetLastError());
        }
		return false;
	}
	m_MySQLRes = mysql_store_result(m_MySQL);
	return true;
}



int DataBase::DataBaseQueriesNumber (const wxString & query)
{
	wxStringTokenizer tokenizer(query, _T(";"), wxTOKEN_DEFAULT);
	return tokenizer.CountTokens();
}



long DataBase::DataBaseGetLastInsertedID()
{
	long myIID = wxNOT_FOUND;
	if (DBIsDataBaseReady()==false){
		return myIID;
    }

	myIID = mysql_insert_id(m_MySQL);
	if (myIID == 0){
		myIID = wxNOT_FOUND;
    }

	return myIID;
}



long DataBase::DataBaseGetAffectedRows(){
    long myAffected = wxNOT_FOUND;
    if (DBIsDataBaseReady() == false) {
        return myAffected;
    }

    myAffected = mysql_affected_rows(m_MySQL);
    return myAffected;
}



bool DataBase::DataBaseStringEscapeQuery (const wxString & query, wxString & results)
{
	results.Clear();

	if (query.IsEmpty()) {
		wxLogError(_("Trying to escape an empty query !"));
		return false;
	}

	results = query;
    results.Replace(_T("'"), _T("\\'"));
	results.Replace(_T("\""), _T("\\\""));
	return true;
}


bool DataBase::DBIsDataBaseReady ()
{
	if (m_IsLibraryStarted == false)
	{
		wxLogError(_("MySQL library not started"));
		return false;
	}

	if (m_IsDatabaseOpened == false)
	{
		wxLogError(_("No database open"));
		return false;
	}
	return true;
}


bool DataBase::DBGetNextRecord (MYSQL_ROW & record)
{
	if (DBIsDataBaseReady()==false)
		return false;

	if (DBResultsNotNull()==false)
		return false;


	record = mysql_fetch_row(m_MySQLRes);
	if (record == NULL)
		return false;

	return true;
}


bool DataBase::DBResultsNotNull ()
{
	if (m_MySQLRes == NULL){
		return false;
	}

	return true;
}




