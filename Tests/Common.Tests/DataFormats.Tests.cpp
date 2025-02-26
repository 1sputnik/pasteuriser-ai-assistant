#include "DataFormats.Tests.h"

TEST(Common_DataFormats, OCDF_Default_Constructor) {
	OCDF ocdf;
	EXPECT_EQ(ocdf.cid, 0);
	EXPECT_EQ(ocdf.time, 0);
	EXPECT_EQ(ocdf.value, 0);
}

TEST(Common_DataFormats, OCDF_Values_Constructor) {
	OCDF ocdf(4, 3456, 546.56);
	EXPECT_EQ(ocdf.cid, 4);
	EXPECT_EQ(ocdf.time, 3456);
	EXPECT_EQ(ocdf.value, 546.56f);
}

TEST(Common_DataFormats, OCDF_Copy_Constructor) {
	OCDF ocdf1(1, 2, 3.4567);
	OCDF ocdf2(ocdf1);
	EXPECT_EQ(ocdf2.cid, 1);
	EXPECT_EQ(ocdf2.time, 2);
	EXPECT_EQ(ocdf2.value, 3.4567f);
}

TEST(Common_DataFormats, OCDF_Assigning) {
	OCDF ocdf1(4, 3423423, 99.4567);
	OCDF ocdf2;
	ocdf2 = ocdf1;
	EXPECT_EQ(ocdf2.cid, 4);
	EXPECT_EQ(ocdf2.time, 3423423);
	EXPECT_EQ(ocdf2.value, 99.4567f);
}

TEST(Common_DataFormats, OCDF_Output) {
	OCDF ocdf;
	testing::internal::CaptureStdout();
	std::cout << ocdf;
	auto output = testing::internal::GetCapturedStdout();
	std::string test_str = R"(0;0;0)";
	EXPECT_EQ(output, test_str);
}

TEST(Common_DataFormats, OCDF_Input) {
	OCDF ocdf;
	std::ifstream in_file;
	string file_name = "One_Line.ocdf.csv";
	in_file.open(file_name, std::ios::in);
	in_file >> ocdf;
	in_file.close();
	EXPECT_EQ(ocdf.cid, 1);
	EXPECT_EQ(ocdf.time, 23);
	EXPECT_EQ(ocdf.value, 33.44f);
}


TEST(Common_DataFormats, TDF_Default_Constructor) {
	TDF tdf;
	EXPECT_EQ(tdf.cid_1_value, 0);
	EXPECT_EQ(tdf.cid_2_value, 0);
	EXPECT_EQ(tdf.cid_3_value, 0);
	EXPECT_EQ(tdf.cid_4_value, 0);
	EXPECT_EQ(tdf.cid_5_value, 0);
	EXPECT_EQ(tdf.cid_6_value, 0);
	EXPECT_EQ(tdf.time, 0);
}

TEST(Common_DataFormats, TDF_Values_Constructor) {
	TDF tdf(123456, 34.34, 101.56, 56.43, 23.44, 55.68, 91.23);
	ASSERT_EQ(tdf.cid_1_value, 34.34f);
	ASSERT_EQ(tdf.cid_2_value, 101.56f);
	ASSERT_EQ(tdf.cid_3_value, 56.43f);
	ASSERT_EQ(tdf.cid_4_value, 23.44f);
	ASSERT_EQ(tdf.cid_5_value, 55.68f);
	ASSERT_EQ(tdf.cid_6_value, 91.23f);
	ASSERT_EQ(tdf.time, 123456);
}

TEST(Common_DataFormats, TDF_Copy_Constructor) {
	TDF tdf1(123456, 34.34, 101.56, 56.43, 23.44, 55.68, 91.23);
	TDF tdf2(tdf1);
	EXPECT_EQ(tdf2.cid_1_value, 34.34f);
	EXPECT_EQ(tdf2.cid_2_value, 101.56f);
	EXPECT_EQ(tdf2.cid_3_value, 56.43f);
	EXPECT_EQ(tdf2.cid_4_value, 23.44f);
	EXPECT_EQ(tdf2.cid_5_value, 55.68f);
	EXPECT_EQ(tdf2.cid_6_value, 91.23f);
	EXPECT_EQ(tdf2.time, 123456);
}

TEST(Common_DataFormats, TDF_Assigning) {
	TDF tdf1(123456, 34.34, 101.56, 56.43, 23.44, 55.68, 91.23);
	TDF tdf2;
	tdf2 = tdf1;
	EXPECT_EQ(tdf2.cid_1_value, 34.34f);
	EXPECT_EQ(tdf2.cid_2_value, 101.56f);
	EXPECT_EQ(tdf2.cid_3_value, 56.43f);
	EXPECT_EQ(tdf2.cid_4_value, 23.44f);
	EXPECT_EQ(tdf2.cid_5_value, 55.68f);
	EXPECT_EQ(tdf2.cid_6_value, 91.23f);
	EXPECT_EQ(tdf2.time, 123456);
}

TEST(Common_DataFormats, TDF_Output) {
	TDF tdf;
	testing::internal::CaptureStdout();
	std::cout << tdf;
	auto output = testing::internal::GetCapturedStdout();
	std::string test_str = R"(0;0;0;0;0;0;0)";
	EXPECT_EQ(output, test_str);
}

TEST(Common_DataFormats, TDF_Input) {
	TDF tdf;
	std::ifstream in_file;
	string file_name = "One_Line.tdf.csv";
	in_file.open(file_name, std::ios::in);
	in_file >> tdf;
	in_file.close();
	EXPECT_EQ(tdf.time, 66419296);
	EXPECT_EQ(tdf.cid_1_value, 0);
	EXPECT_EQ(tdf.cid_2_value, 0);
	EXPECT_EQ(tdf.cid_3_value, 94);
	EXPECT_EQ(tdf.cid_4_value, 3.2f);
	EXPECT_EQ(tdf.cid_5_value, 95.3f);
	EXPECT_EQ(tdf.cid_6_value, 37.7f);
}
