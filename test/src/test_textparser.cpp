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
}

// Test case for TextParserTxtFileComma open method
TEST(TextParserTxtFileTest, CommaOpen) {
    TextParserTxtFileComma txtParser("test.txt");
    EXPECT_FALSE(txtParser.OpenParseFile());
}

