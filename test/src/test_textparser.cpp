#include "gtest/gtest.h"
#include "test_param.h"
#include "../../src/core/textparser.h"



// Test case for TextParserTxtFileTab
TEST(TextParserTxtFileTabTest, Constructor) {
    TextParserTxtFileTab tabParser("test.txt");
    EXPECT_EQ(tabParser.GetParserType(), TEXTPARSER_NAME[TXTFILE_TAB]);
}

// Test case for TextParserTxtFileComma
TEST(TextParserTxtFileCommaTest, Constructor) {
    TextParserTxtFileComma commaParser("test.txt");
    EXPECT_EQ(commaParser.GetParserType(), TEXTPARSER_NAME[TXTFILE_COMMA]);
}

// Test case for TextParserTxtFileTab open method
TEST(TextParserTxtFileTest, TabOpen) {
    TextParserTxtFileTab txtParser("test.txt");
    EXPECT_FALSE(txtParser.OpenParseFile());
}

// Test case for TextParserTxtFileTab open method (create the file)
TEST(TextParserTxtFileTest, TabOpenCreate) {
    wxFileName myFileName(g_TestPathEXPORT, "test_parser_create.txt");
    TextParserTxtFileTab txtParser(myFileName.GetFullPath());
    if (myFileName.Exists()) {
        wxRemoveFile(myFileName.GetFullPath());
    }
    EXPECT_FALSE(myFileName.Exists());
    EXPECT_TRUE(txtParser.OpenParseFile(true));

    wxArrayString myValues;
    myValues.Add(_T("test1"));
    myValues.Add(_T("test2"));
    myValues.Add(_T("test3"));
    EXPECT_TRUE(txtParser.WriteNextLine(myValues));

    EXPECT_TRUE(txtParser.CloseParseFile());
    EXPECT_TRUE(myFileName.Exists());

    // Read the file
    TextParserTxtFileTab txtParser2(myFileName.GetFullPath());
    EXPECT_TRUE(txtParser2.OpenParseFile());
    EXPECT_FALSE(txtParser2.CheckFileToParse()); // nb of fields not set
    txtParser2.SetNumberOfFields(3);
    EXPECT_TRUE(txtParser2.CheckFileToParse()); // nb of fields not set

    wxArrayString results;
    EXPECT_TRUE(txtParser2.ParseNextLine(results));
    EXPECT_EQ(results[0], _T("test1"));
    EXPECT_EQ(results[1], _T("test2"));
    EXPECT_EQ(results[2], _T("test3"));
    EXPECT_TRUE(txtParser2.CloseParseFile());
}

// Test case for TextParserTxtFileComma open method
TEST(TextParserTxtFileTest, CommaOpen) {
    TextParserTxtFileComma txtParser("test.txt");
    EXPECT_FALSE(txtParser.OpenParseFile());
}

// Test case for CheckFileToParse with an empty file
TEST(TextParserTxtFileTest, CheckFileToParseEmpty) {
    wxFileName txtFileName(g_TestPathEXPORT, "test_parser_empty.txt");
    if (txtFileName.Exists()) {
        wxRemoveFile(txtFileName.GetFullPath());
    }
    wxTextFile txtFile(txtFileName.GetFullPath());
    txtFile.Create();
    txtFile.Close();
    EXPECT_TRUE(txtFileName.Exists());

    TextParserTxtFileComma txtParser(txtFileName.GetFullPath());
    EXPECT_TRUE(txtParser.OpenParseFile());
    txtParser.SetNumberOfFields(3);
    EXPECT_FALSE(txtParser.CheckFileToParse()); // file is empty
    EXPECT_TRUE(txtParser.CloseParseFile());
}

// Test case for CheckFileToParse with first line empty
TEST(TextParserTxtFileTest, CheckFileToParseEmptyLine) {
    wxFileName txtFileName(g_TestPathEXPORT, "test_parser_empty_line.txt");
    if (txtFileName.Exists()) {
        wxRemoveFile(txtFileName.GetFullPath());
    }
    wxTextFile txtFile(txtFileName.GetFullPath());
    txtFile.Create();
    txtFile.AddLine(_T(""));
    txtFile.Write();
    txtFile.Close();
    EXPECT_TRUE(txtFileName.Exists());

    TextParserTxtFileComma txtParser(txtFileName.GetFullPath());
    EXPECT_TRUE(txtParser.OpenParseFile());
    txtParser.SetNumberOfFields(3);
    EXPECT_FALSE(txtParser.CheckFileToParse()); // first line is empty
    EXPECT_TRUE(txtParser.CloseParseFile());
}

// Test case for wrong type of parser
TEST(TextParserTxtFileTest, CheckFileToParseWrongParser) {
    wxFileName txtFileName(g_TestPathEXPORT, "test_parser_wrong.txt");
    if (txtFileName.Exists()) {
        wxRemoveFile(txtFileName.GetFullPath());
    }
    wxTextFile txtFile(txtFileName.GetFullPath());
    txtFile.Create();
    txtFile.AddLine(_T("test1;test2;test3"));
    txtFile.Write();
    txtFile.Close();
    EXPECT_TRUE(txtFileName.Exists());

    TextParserTxtFileTab txtParser(txtFileName.GetFullPath());
    EXPECT_TRUE(txtParser.OpenParseFile());
    txtParser.SetNumberOfFields(3);
    EXPECT_FALSE(txtParser.CheckFileToParse()); // wrong type of parser
    EXPECT_TRUE(txtParser.CloseParseFile());
}


// Test case for CheckFileToParse with wrong number of fields in the file
TEST(TextParserTxtFileTest, CheckFileToParseWrongFields) {
    wxFileName txtFileName(g_TestPathEXPORT, "test_parser_wrong.txt");
    if (txtFileName.Exists()) {
        wxRemoveFile(txtFileName.GetFullPath());
    }
    wxTextFile txtFile(txtFileName.GetFullPath());
    txtFile.Create();
    txtFile.AddLine(_T("test1;test2;test3"));
    txtFile.Write();
    txtFile.Close();
    EXPECT_TRUE(txtFileName.Exists());

    TextParserTxtFileComma txtParser(txtFileName.GetFullPath());
    EXPECT_TRUE(txtParser.OpenParseFile());
    txtParser.SetNumberOfFields(2);
    EXPECT_FALSE(txtParser.CheckFileToParse()); // wrong number of fields
    EXPECT_TRUE(txtParser.CloseParseFile());
}

// Test case for CreateParserBasedOnType function
TEST(TextParserTxtFileTest, CreateParserBasedOnType) {
    auto * txtParser = TextParserTxtFile::CreateParserBasedOnType(TXTFILE_TAB, wxFileName(g_TestPathEXPORT, "test_parser_tab.txt"));
    EXPECT_EQ(txtParser->GetParserType(), TEXTPARSER_NAME[TXTFILE_TAB]);
    delete txtParser;
}

TEST(TextParserTxtFileTest, CreateParserBasedOnType2) {
    auto * txtParser = TextParserTxtFile::CreateParserBasedOnType(TXTFILE_COMMA, wxFileName(g_TestPathEXPORT, "test_parser_tab.txt"));
    EXPECT_EQ(txtParser->GetParserType(), TEXTPARSER_NAME[TXTFILE_COMMA]);
    delete txtParser;
}

TEST(TextParserTxtFileTest, CreateParserBasedOnType3) {
    auto * txtParser = TextParserTxtFile::CreateParserBasedOnType(wxNOT_FOUND, wxFileName(g_TestPathEXPORT, "test_parser_tab.csv"));
    EXPECT_EQ(txtParser->GetParserType(), TEXTPARSER_NAME[TXTFILE_COMMA]);
    delete txtParser;
}

TEST(TextParserTxtFileTest, CreateParserBasedOnType4) {
    auto * txtParser = TextParserTxtFile::CreateParserBasedOnType(wxNOT_FOUND, wxFileName(g_TestPathEXPORT, "test_parser_tab.txt"));
    EXPECT_EQ(txtParser->GetParserType(), TEXTPARSER_NAME[TXTFILE_TAB]);
    delete txtParser;
}
