/***************************************************************************
 database.h
 Basic function for connecting a MySQL embedded database
 -------------------
 copyright            : (C) 2007 CREALP Lucien Schreiber
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
    @file database.h
    @brief   Definition of class DataBase

 Class used to connect and manage the embedded MySQL server. This class needs
 mysql.h and the embedded MySql Library.
 @author Lucien Schreiber (c) CREALP 2007
*/

#ifndef __database_H__
#define __database_H__

#if defined(__GNUG__) && !defined(NO_GCC_PRAGMA)
#pragma interface "database.cpp"
#endif

#ifndef WX_PRECOMP
#include <wx/wx.h>
#endif

#include "mysql.h"
// #include <wx/arrstr.h> // array string
// #include <wx/strconv.h> // unicode conversion

// #include <wx/filename.h>

#include <wx/buffer.h>
#include <wx/config.h>
#include <wx/dir.h>  // directory operation (size)
#include <wx/fileconf.h>
#include <wx/tokenzr.h>  // tokenizer of string

WX_DEFINE_ARRAY_LONG(unsigned long, tmArrayULong);

class DataBaseResult;

// WX_DEFINE_ARRAY_(int, ArrayOfInts);

class DataBase {
  private:
    // member
    bool m_IsLibraryStarted;
    bool m_IsDatabaseOpened;
    bool m_IsDatabaseConnected;
    MYSQL* m_MySQL;
    MYSQL_RES* m_MySQLRes;
    wxString m_DBName;
    wxString m_DBPath;
    wxString m_ErrMsgPath;

    // functions
    bool DBLibraryInit(const wxString& datadir);

    void DBLibraryEnd();

    bool DBUseDataBase(const wxString& dbname);

    bool DBIsDataBaseReady();

    bool DBGetNextRecord(MYSQL_ROW& record);

    bool DBResultsNotNull();

  protected:
  public:
    DataBase(const wxString& errmsgpath = wxEmptyString);

    virtual ~DataBase();

    // database operations
    bool DataBaseCreateNew(const wxString& datadir, const wxString& name);

    bool DataBaseOpen(const wxString& datadir, const wxString& name);

    bool DataBaseDelete();

    wxString DataBaseGetName();

    wxString DataBaseGetPath();

    wxString DataBaseGetSize(int precision = 2, const wxString& failmsg = _("Not available"));

    static wxString DataBaseGetVersion();

    wxString DataBaseGetLastError();

    // thread operations
    bool DataBaseThreadInit();

    void DataBaseThreadEnd();

    // query operations
    bool DataBaseQueryNoResults(const wxString& query, bool logerror = true);

    bool DataBaseQuery(const wxString& query, bool logerror = true);

    int DataBaseQueriesNumber(const wxString& query);

    long DataBaseGetLastInsertedID();

    long DataBaseGetAffectedRows();

    // query transformations
    static bool DataBaseStringEscapeQuery(const wxString& query, wxString& results);

    // results operations
    bool DataBaseHasResults();

    void DataBaseClearResults();

    // results by rows (clear after use)
    bool DataBaseGetResultSize(unsigned int* pcols, long* prows);

    bool DataBaseGetNextResult(wxString& result);

    bool DataBaseGetNextResult(wxArrayString& results);

    bool DataBaseGetNextResult(long& result);

    bool DataBaseGetNextResult(wxArrayLong& results);

    bool DataBaseGetNextResult(wxDouble& result);

    bool DataBaseGetNextResult(wxArrayDouble& results);

    bool DataBaseGetNextRowResult(MYSQL_ROW& row, tmArrayULong& lengths);

    // results set (auto cleared after use)
    bool DataBaseGetResults(wxArrayString& results);

    bool DataBaseGetResults(wxArrayLong& results);

    bool DataBaseGetResults(wxArrayDouble& results);

    bool DataBaseGetResults(DataBaseResult* results);
};

#if (0)
const wxString DATABASE_TYPE_STRING = _T("MYSQL");

/*!
    @brief enumeration of allowed values for
    inisialisation.
    */
enum Lang_Flag {
    LANG_UTF8,
    LANG_LATIN1
};

//----------------------------------------------------------------------------
// DataBase
//----------------------------------------------------------------------------
/*!
    @class DataBase
    @brief    Used for connecting to an embedded MySQL Database

    This class used the embedded library of MySQL to open and connect to databases.
*/

class DataBase {
  public:
    // constructors and destructors
    /*!
    @brief   Constructor, do nothing
    */
    // DataBase();
    /*!

    @brief Destructor, do nothing
    */
    //~DataBase();
    /*!
    @brief   Function used to init the MySql embedded server and to open a database

    This function must be called before other Database functions.
    @param      path wxString containing the directory where live the database
    @param flag Character set for opening Database (see #Lang_Flag)
    @result     return true if library and connexion to the database ok
    */
    //   bool DataBaseOpen(wxString path,enum Lang_Flag flag);

    // int DataBaseInitLibrary (const wxString & path);

    /*!
    @brief   Function called to close the database and free the library

    This function must be called before quitting the program
    @result return TRUE if the database was close correctely
    */
    // bool DataBaseClose ();

    /*!
    @brief  return TRUE if a database is open
    @result return TRUE if the database is open
    */
    // bool DataBaseIsOpen();

    // wxString DataBaseGetLastError();

    /*!


    @brief   return an array of string containing all the database tables
    @result  an array of string (wxArrayString) containing all the tables names
    */
    wxArrayString DataBaseListTables();

    /*!
    @brief   Return a wxString containing the version of the embedded server

    This function is the only one who may be called before DataBaseOpen().
    @result  a wxString containing the version number
    */
    // static wxString DatabaseGetVersion();

    // long DatabaseGetCountResults();

    // int DatabaseGetCountCols();

    /*!
    @brief Return an array containing all the selected table fields
    @param      sTable a String containing the table name
    @result     an array of string containing the fields name.
    */
    wxArrayString DatabaseListFields(wxString sTable);

    /*!
    @brief Get all data stored in a table.

    This function just process the request, to get the result row
    by row, call the DataBaseGetNextResult() function.

    @param      sTable a String containing the table name
    @result     TRUE if the request is valid.
    */
    bool DataBaseGetAllTableContent(wxString sTable);

    /*!
    @brief Return the result of the last request.

    @result     An array of strings containing the values of one row.
    */
    // wxArrayString DataBaseGetNextResult();

    long DataBaseGetLastInsertID();

    // bool DataBaseGetNextResult(wxString & result);

    unsigned long* DataBaseGetNextRowResult(MYSQL_ROW& row);

    bool DataBaseTableExist(const wxString& tableName);

    // int DataBaseGetResultAsInt(bool ClearResultDirect = TRUE);

    // long DataBaseGetNextResultAsLong();

    // void DataBaseGetNextResultAsLong(wxArrayLong & resultArray);

    // double DataBaseGetResultAsDouble();

    bool DataBaseIsTableEmpty(const wxString& tableName);

    /*!
    @brief Used to execute multiple query on the database

    This function is mainly created to execute multiple query without results.
    User have to split the multiple queries into unique query and loop this function
    for each queries. Splitting queries could be made with the DataBaseCutRequest()
    member function.

    @warning  This function process the query and then destroy the query results,
    so no results could be see using DataBaseGetNextResult().
    @param     myQuery the string containing the query
    @param    logerror If set to true, log sql error in debug mode.
    @result    return 0 if the query works and the MYSQL error code otherwise.
    @see DataBaseCutRequest(), DataBaseQuery()

    */
    // bool DataBaseQueryNoResult(wxString myQuery, bool logerror = false);
    /*!
    @brief Used to execute one query on the database and then get the result

    This function works in a similar way as the DataBaseQueryNoResult() but is able
    to send back the result of a query.

    @param myQuery the string containing the query
    @param logerror If set to true, we log the error if the query doesn't work as expected.
    @result true if the query was successfuly made otherwise false.
    @warning User have to check the return value, and if false then one souldn't
    call the DataBaseGetNextResult()
    @see DataBaseQueryNoResult()
     */
    // bool DataBaseQuery(const wxString & myQuery, bool logerror = false);

    // wxArrayLong * DataBaseQuerySafe (const wxString & myQuery);

    // bool DataBaseHasResult ();

    // bool DataBaseResultExists();

    // void DataBaseDestroyResults ();

    /*! Execute multiple query
     @bug NOT WORKING, CRASH WITH VERSION 5.1.23-rc (on windows and Mac)
     */
    // int DataBaseQueryMultiple (const wxString & myQuery);

    // int DataBaseQueryReal (wxString myQuery);

    /*!
    @brief  return the path of the database

    This function must be called only after DataBaseOpen().
    @result return the path of the database
    */
    // wxString DataBaseGetPath();

    /*!
    @brief  return the Name of the database

    This function must be called only after DataBaseOpen().
    @result return the name of the database
    */
    // wxString DataBaseGetName();

    /*!
    @brief  return the character set used for the transaction

    This function must be called only after DataBaseOpen().
    @result return the name of the character set used.
    */
    // wxString DatabaseGetCharacterSet();

    /*!
    @brief  create a new database

    @param DataBasePath a wxString containing the Path for the new Database
    @param DataBaseName a wxString containing the new database name.
    @param Flag a Lang_Flag used for selecting the character set,
    allowed values are (default)LANG_UTF8, LANG_LATIN1,
    @result return TRUE if the new database was created succesfully.
    */
    // bool DataBaseCreateNew(wxString DataBasePath, wxString DataBaseName,enum Lang_Flag Flag=LANG_UTF8);

    /*!
    @brief  cut a big request into unique request.

    A big request implying multiple operations could not be proceed in one operation
    one must cut this request into multiple small operations.

    @param theRequest a wxString containing the full request.
    @result a wxArrayString containing a unique request for each array.
    */
    // wxArrayString DataBaseCutRequest (wxString theRequest);

    /*!
    @brief   Compute the database file size
    @param  iPrecision an integer giving the requested precision for the result
    (default is 2 decimals after the dot)
    @result  Return an human readable string containing the database size plus
    the unit  (MB)
    */
    // wxString DataBaseGetSize (int iPrecision=2);

    // static void DataBaseCloseLibrary();

    //   static void DataBaseConvertWindowsPath (wxString & spath);

    bool DataBaseNewThreadInit();

    void DataBaseNewThreadUnInit();

    // bool DataBaseQueryBinary(const char * query, bool DestroyResult = FALSE);

  protected:
    // MYSQL * pMySQL;

    // for storing restults
    // MYSQL_RES * pResults;
    // int m_resultNumber;
    // long m_resultCount;

    // storing database path and name.
    // wxString m_DBPath;
    // wxString m_DBName;

    // bool IsDatabaseOpen;

    // static bool bIsLibInit;

    /*!
    @brief  convert the path into path and database name

    @result true if all is OK.
    */
    // bool DataBaseConvertFullPath(wxString fullpath);

    /*!
    @brief Function used in windows to convert all the '\' path separator
    into a '/' separator.

    If the path allready contain '/' separator, this function does nothing
    @param      originalPath a wxSting containing the path to check/change
    @result     the modified path in a wxString. This modified path may be used for
    loading the MYSQL server.
    */
    // wxString DataBaseConvertMYSQLPath(wxString originalPath);

    /*!
    @brief Function used to set the character set used by the embedded server

    This function is call by DataBaseOpen(), do not call it directly.
    */
    // bool DataBaseSetCharacterSet (enum Lang_Flag myFlag);
};
#endif

#endif
