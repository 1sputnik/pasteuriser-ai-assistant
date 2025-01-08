#include "DataReader.Tests.h"

TEST(Common_DataReader, OCDF_Read_Void) {
	vector<OCDF> ocdfvec;
	string file_name = "Void.csv";

	int full_size = 0;
	ocdfvec = load_OCDF_data(file_name, full_size);

	EXPECT_EQ(ocdfvec.size(), 0);
}

TEST(Common_DataReader, OCDF_Read_Corrupted) {
	vector<OCDF> ocdfvec;
	string file_name = "Corrupted.ocdf.csv";

	int full_size = 0;
	ocdfvec = load_OCDF_data(file_name, full_size);

	EXPECT_EQ(ocdfvec.size(), 0);
}

TEST(Common_DataReader, OCDF_Read_Holey) {
	vector<OCDF> ocdfvec;
	string file_name = "Holey.ocdf.csv";

	int full_size = 0;
	ocdfvec = load_OCDF_data(file_name, full_size);

	EXPECT_EQ(ocdfvec.size(), 0);
}

TEST(Common_DataReader, OCDF_Read_Invalid) {
	vector<OCDF> ocdfvec;
	string file_name = "Invalid.csv";

	int full_size = 0;
	ocdfvec = load_OCDF_data(file_name, full_size);

	EXPECT_EQ(ocdfvec.size(), 0);
}

TEST(Common_DataReader, OCDF_Read_One_Line) {
	vector<OCDF> ocdfvec;
	string file_name = "One_Line.ocdf.csv";

	int full_size = 100;
	ocdfvec = load_OCDF_data(file_name, full_size);

	EXPECT_EQ(ocdfvec.size(), 1);
}

TEST(Common_DataReader, OCDF_Read_Right_Data_All) {
	vector<OCDF> ocdfvec;
	string file_name = "Rigth.ocdf.csv";

	int full_size = 100;
	ocdfvec = load_OCDF_data(file_name, full_size);

	ASSERT_EQ(ocdfvec.size(), 100);
	EXPECT_EQ(ocdfvec[0].value, 96.4f);
	EXPECT_EQ(ocdfvec[99].time, 66418168);

	int half_size = 50;
	ocdfvec = load_OCDF_data(file_name, half_size);

	ASSERT_EQ(ocdfvec.size(), 50);
	EXPECT_EQ(ocdfvec[0].value, 96.4f);
	EXPECT_EQ(ocdfvec[49].time, 66418059);

	int over_size = 10000;
	ocdfvec = load_OCDF_data(file_name, over_size);
	EXPECT_EQ(ocdfvec.size(), 100);

	int milli_size = 1;
	ocdfvec = load_OCDF_data(file_name, milli_size);
	EXPECT_EQ(ocdfvec.size(), 1);

	int magic_size = 0;
	ocdfvec = load_OCDF_data(file_name, magic_size);
	EXPECT_EQ(ocdfvec.size(), 100);
}

TEST(Common_DataReader, OCDF_BinRead_Right_Data) {
	vector<OCDF> ocdfvec;
	string file_name = "Rigth.ocdf.bin";

	int full_size = 100;
	ocdfvec = binload_OCDF_data(file_name, full_size);

	ASSERT_EQ(ocdfvec.size(), 100);
	EXPECT_EQ(ocdfvec[0].value, 96.4f);
	EXPECT_EQ(ocdfvec[99].time, 66418168);

	int half_size = 50;
	ocdfvec = binload_OCDF_data(file_name, half_size);

	ASSERT_EQ(ocdfvec.size(), 50);
	EXPECT_EQ(ocdfvec[0].value, 96.4f);
	EXPECT_EQ(ocdfvec[49].time, 66418059);

	int over_size = 10000;
	ocdfvec = binload_OCDF_data(file_name, over_size);
	EXPECT_EQ(ocdfvec.size(), 100);

	int milli_size = 1;
	ocdfvec = binload_OCDF_data(file_name, milli_size);
	EXPECT_EQ(ocdfvec.size(), 1);

	int magic_size = 0;
	ocdfvec = binload_OCDF_data(file_name, magic_size);
	EXPECT_EQ(ocdfvec.size(), 100);
}
