#include <iostream>
#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include "../src/HorcruxCreator.h"
#include "../src/Base64Transformer.h"

#ifndef TRUE
#define TRUE 1
#endif

#ifndef FALSE
#define FALSE 0
#endif

inline bool fileExists(const std::string& iFileName) {
    if (FILE *aFile = fopen(iFileName.c_str(), "r"))
    {
        fclose(aFile);
        return true;
    }
    else
    {
        return false;
    }   
}

TEST(horcruxCreatorTest, firstHorcruxCreatorTest)
{
    FILE * testIN;
    int iHorcruxAmount = 3;
    int iFileSize = 11;
    const std::string iHorcruxName = "fakeHorcruxName";
    const std::string iPath = "";

    testIN = fopen("horcruxUnitTestFile.txt", "wb");
    fputs("HelloWorld!", testIN);
    fclose(testIN);

    testIN = fopen("horcruxUnitTestFile.txt", "rb");
    HorcruxCreator::horcruxCreator(testIN, iHorcruxAmount, iFileSize, iHorcruxName, iPath);
    fclose(testIN);

    EXPECT_TRUE(fileExists("fakeHorcruxNameHorcrux1.txt"));
    EXPECT_TRUE(fileExists("fakeHorcruxNameHorcrux2.txt"));
    EXPECT_TRUE(fileExists("fakeHorcruxNameHorcrux3.txt"));

    remove("fakeHorcruxNameHorcrux1.txt");
    remove("fakeHorcruxNameHorcrux2.txt");
    remove("fakeHorcruxNameHorcrux3.txt");
}


TEST(horcruxCreatorTest, secondHorcruxCreatorTest)
{
    FILE * testIN;
    int iHorcruxAmount = 5;
    int iFileSize = 11;
    const std::string iHorcruxName = "secondFakeHorcruxName";
    const std::string iPath = "";

    testIN = fopen("horcruxUnitTestFile.txt", "wb");
    fputs("HelloWorld!", testIN);
    fclose(testIN);

    testIN = fopen("horcruxUnitTestFile.txt", "rb");
    HorcruxCreator::horcruxCreator(testIN, iHorcruxAmount, iFileSize, iHorcruxName, iPath);
    fclose(testIN);

    EXPECT_TRUE(fileExists("secondFakeHorcruxNameHorcrux1.txt"));
    EXPECT_TRUE(fileExists("secondFakeHorcruxNameHorcrux2.txt"));
    EXPECT_TRUE(fileExists("secondFakeHorcruxNameHorcrux3.txt"));
    EXPECT_TRUE(fileExists("secondFakeHorcruxNameHorcrux4.txt"));
    EXPECT_TRUE(fileExists("secondFakeHorcruxNameHorcrux5.txt"));

    remove("secondFakeHorcruxNameHorcrux1.txt");
    remove("secondFakeHorcruxNameHorcrux2.txt");
    remove("secondFakeHorcruxNameHorcrux3.txt");
    remove("secondFakeHorcruxNameHorcrux4.txt");
    remove("secondFakeHorcruxNameHorcrux5.txt");
}

TEST(Base64Test, encodeBase64Test)
{
    std::string stringToEncode = "HelloWorld!";
    std::string aEncodedString = base64EncoderDecoder(TRUE, stringToEncode);

    EXPECT_EQ(aEncodedString, "SGVsbG9Xb3JsZCE=");
}

TEST(Base64Test, decodeBase64Test)
{
    std::string stringToDecode = "SGVsbG9Xb3JsZCE=";
    std::string aDecodedString = base64EncoderDecoder(FALSE, stringToDecode);

    EXPECT_EQ(aDecodedString, "HelloWorld!");
}

int main(int argc, char **argv) {
  testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}