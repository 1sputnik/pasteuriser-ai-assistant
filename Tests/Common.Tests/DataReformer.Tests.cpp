#include "DataReformer.Tests.h"

TEST(Common_DataReformer, OCDF_cut_quantity) {
	vector<OCDF> ocdfvec;
	string file_name = "TestData.ocdf.bin";
	int full_size = 0;
	ocdfvec = binload_OCDF_data(file_name, full_size);
	ASSERT_EQ(ocdfvec.size(), 100);

	vector<OCDF> ocdfvec_right;
	ocdfvec_right = cut_quntity_data(ocdfvec, 40, false);
	ASSERT_EQ(ocdfvec_right.size(), 40);
	EXPECT_EQ(ocdfvec_right[39].time, 66418044);

	vector<OCDF> ocdfvec_left;
	ocdfvec_left = cut_quntity_data(ocdfvec, 40, true);
	ASSERT_EQ(ocdfvec_left.size(), 40);
	EXPECT_EQ(ocdfvec_left[39].time, 66418168);

	vector<OCDF> ocdfvec_over;
	ocdfvec_left = cut_quntity_data(ocdfvec, 1000, true);
	ASSERT_EQ(ocdfvec_left.size(), 100);
}

TEST(Common_DataReformer, OCDF_cut_percent) {
	vector<OCDF> ocdfvec;
	string file_name = "TestData.ocdf.bin";
	int full_size = 0;
	ocdfvec = binload_OCDF_data(file_name, full_size);
	ASSERT_EQ(ocdfvec.size(), 100);

	vector<OCDF> ocdfvec_right;
	ocdfvec_right = cut_percent_data(ocdfvec, 0.4, false);
	ASSERT_EQ(ocdfvec_right.size(), 40);
	EXPECT_EQ(ocdfvec_right[39].time, 66418044);

	vector<OCDF> ocdfvec_left;
	ocdfvec_left = cut_percent_data(ocdfvec, 0.4, true);
	ASSERT_EQ(ocdfvec_left.size(), 40);
	EXPECT_EQ(ocdfvec_left[39].time, 66418168);

	vector<OCDF> ocdfvec_over;
	ocdfvec_left = cut_percent_data(ocdfvec, 1000, true);
	ASSERT_EQ(ocdfvec_left.size(), 100);

	vector<OCDF> ocdfvec_milli;
	ocdfvec_left = cut_percent_data(ocdfvec, 0.00001, true);
	ASSERT_EQ(ocdfvec_left.size(), 0);
}

TEST(Common_DataReformer, OCDF_cut_pertime) {
	vector<OCDF> ocdfvec;
	string file_name = "TestData.ocdf.bin";
	int full_size = 0;
	long long time_border = 66418119;
	ocdfvec = binload_OCDF_data(file_name, full_size);
	ASSERT_EQ(ocdfvec.size(), 100);

	vector<OCDF> ocdfvec_00;
	ocdfvec_00 = cut_data_per_time(ocdfvec, time_border, false, false);
	ASSERT_EQ(ocdfvec_00.size(), 69);
	EXPECT_EQ(ocdfvec_00[68].time, 66418110);

	vector<OCDF> ocdfvec_01;
	ocdfvec_01 = cut_data_per_time(ocdfvec, time_border, false, true);
	ASSERT_EQ(ocdfvec_01.size(), 71);
	EXPECT_EQ(ocdfvec_01[70].time, time_border);

	vector<OCDF> ocdfvec_10;
	ocdfvec_10 = cut_data_per_time(ocdfvec, time_border, true, false);
	ASSERT_EQ(ocdfvec_10.size(), 29);
	EXPECT_EQ(ocdfvec_10[0].time, 66418120);

	vector<OCDF> ocdfvec_11;
	ocdfvec_11 = cut_data_per_time(ocdfvec, time_border, true, true);
	ASSERT_EQ(ocdfvec_11.size(), 31);
	EXPECT_EQ(ocdfvec_11[0].time, time_border);

	time_border = 999999999;
	vector<OCDF> ocdfvec_20;
	ocdfvec_20 = cut_data_per_time(ocdfvec, time_border, false, false);
	ASSERT_EQ(ocdfvec_20.size(), 100);

	time_border = 0;
	vector<OCDF> ocdfvec_21;
	ocdfvec_21 = cut_data_per_time(ocdfvec, time_border, false, false);
	ASSERT_EQ(ocdfvec_21.size(), 0);
}