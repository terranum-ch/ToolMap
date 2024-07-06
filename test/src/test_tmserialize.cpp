#include "gtest/gtest.h"
#include "../../src/components/wxserialize/tmserialize.h"

TEST(tmSerializeTest, SerializeBool) {
    tmSerialize serializer;
    serializer << true;
    serializer << false;
    EXPECT_EQ(serializer.GetString(), _T("1|0|"));
}

TEST(tmSerializeTest, SerializeString) {
    tmSerialize serializer;
    serializer << _T("Hello");
    serializer << _T("World");
    EXPECT_EQ(serializer.GetString(), _T("Hello|World|"));
}

TEST(tmSerializeTest, SerializeInt) {
    tmSerialize serializer;
    serializer << 42;
    serializer << -1337;
    EXPECT_EQ(serializer.GetString(), _T("42|-1337|"));
}

TEST(tmSerializeTest, DeserializeBool) {
    tmSerialize serializer(_T("1|0|"));
    bool value;
    serializer >> value;
    EXPECT_EQ(value, true);
    serializer >> value;
    EXPECT_EQ(value, false);
}

TEST(tmSerializeTest, DeserializeString) {
    tmSerialize serializer(_T("Hello|World|"));
    wxString value;
    serializer >> value;
    EXPECT_EQ(value, _T("Hello"));
    serializer >> value;
    EXPECT_EQ(value, _T("World"));
}

TEST(tmSerializeTest, DeserializeInt) {
    tmSerialize serializer(_T("42|-1337|"));
    int value;
    serializer >> value;
    EXPECT_EQ(value, 42);
    serializer >> value;
    EXPECT_EQ(value, -1337);
}

TEST(tmSerializeTest, CanRead){
    tmSerialize serializer;
    int value = 0;
    serializer >> value;
    EXPECT_EQ(value, 0);

    tmSerialize serialize2(wxEmptyString);
    int value2 = 0;
    serialize2 >> value2;
    EXPECT_EQ(value2, 0);

}

TEST(tmSerialize, StoreReadLong){
    long value = 123456789;
    long result = 0;
    tmSerialize serializer;
    serializer << value;
    serializer << value;

    tmSerialize serializer2(_T("123456789|123456789|"));
    serializer2 >> result;
    EXPECT_EQ(value, result);
    serializer2 >> result;
    EXPECT_EQ(value, result);
}

TEST(tmSerialize, StoreReadColour){
    wxColour red (255, 0, 0);
    wxColour blue (0, 0, 255);
    wxColour result;
    tmSerialize serializer;
    serializer << red;
    serializer << blue;

    tmSerialize serializer2(_T("rgb(255,0,0)|rgb(0,0,255)|"));
    serializer2 >> result;
    EXPECT_EQ(red, result);
    serializer2 >> result;
    EXPECT_EQ(blue, result);
}

TEST(tmSerialize, WrongConvert) {
    wxString value = _T("Hello");
    int result = 0;
    tmSerialize serializer(value);
    serializer >> result;
    EXPECT_EQ(result, 0);
}

TEST(tmSerialize, StringSerialize) {
    wxString value = _T("Hello");
    wxString result = wxEmptyString;
    tmSerialize serialize2;
    serialize2 << value;
    serialize2 << value;

    tmSerialize serialize3(_T("Hello|Hello|"));
    serialize3 >> result;
    EXPECT_EQ(result, value);
    serialize3 >> result;
    EXPECT_EQ(result, value);

    tmSerialize serializer(value);
    serializer >> result;
    EXPECT_EQ(result, value);
}
