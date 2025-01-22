#include "DataReformer.Tests.h"

TEST(Common_DataReformer, OCDF_Cut_Quantity) {
	vector<OCDF> ocdfvec;
	string file_name = "TestData.ocdf.bin";
	int full_size = 0;
	ocdfvec = binload_OCDF_data(file_name, full_size);
	ASSERT_EQ(ocdfvec.size(), 100);

	vector<OCDF> ocdfvec_right;
	ocdfvec_right = cut_quntity_data(ocdfvec, 40, false);
	ASSERT_EQ(ocdfvec_right.size(), 40);
	EXPECT_EQ(ocdfvec_right[39].time, 66418044);
	ocdfvec_right.clear();

	vector<OCDF> ocdfvec_left;
	ocdfvec_left = cut_quntity_data(ocdfvec, 40, true);
	ASSERT_EQ(ocdfvec_left.size(), 40);
	EXPECT_EQ(ocdfvec_left[39].time, 66418168);
	ocdfvec_left.clear();

	vector<OCDF> ocdfvec_over;
	ocdfvec_over = cut_quntity_data(ocdfvec, 1000, true);
	ASSERT_EQ(ocdfvec_over.size(), 100);
	ocdfvec_over.clear();
	ocdfvec.clear();
}

TEST(Common_DataReformer, OCDF_Cut_Percent) {
	vector<OCDF> ocdfvec;
	string file_name = "TestData.ocdf.bin";
	int full_size = 0;
	ocdfvec = binload_OCDF_data(file_name, full_size);
	ASSERT_EQ(ocdfvec.size(), 100);

	vector<OCDF> ocdfvec_right;
	ocdfvec_right = cut_percent_data(ocdfvec, 0.4, false);
	ASSERT_EQ(ocdfvec_right.size(), 40);
	EXPECT_EQ(ocdfvec_right[39].time, 66418044);
	ocdfvec_right.clear();

	vector<OCDF> ocdfvec_left;
	ocdfvec_left = cut_percent_data(ocdfvec, 0.4, true);
	ASSERT_EQ(ocdfvec_left.size(), 40);
	EXPECT_EQ(ocdfvec_left[39].time, 66418168);
	ocdfvec_left.clear();

	vector<OCDF> ocdfvec_over;
	ocdfvec_over = cut_percent_data(ocdfvec, 1000, true);
	ASSERT_EQ(ocdfvec_over.size(), 100);
	ocdfvec_over.clear();

	vector<OCDF> ocdfvec_milli;
	ocdfvec_milli = cut_percent_data(ocdfvec, 0.00001, true);
	ASSERT_EQ(ocdfvec_milli.size(), 0);
	ocdfvec_milli.clear();
	ocdfvec.clear();
}

TEST(Common_DataReformer, OCDF_Cut_Per_Time) {
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
	ocdfvec_00.clear();

	vector<OCDF> ocdfvec_01;
	ocdfvec_01 = cut_data_per_time(ocdfvec, time_border, false, true);
	ASSERT_EQ(ocdfvec_01.size(), 71);
	EXPECT_EQ(ocdfvec_01[70].time, time_border);
	ocdfvec_01.clear();

	vector<OCDF> ocdfvec_10;
	ocdfvec_10 = cut_data_per_time(ocdfvec, time_border, true, false);
	ASSERT_EQ(ocdfvec_10.size(), 29);
	EXPECT_EQ(ocdfvec_10[0].time, 66418120);
	ocdfvec_10.clear();

	vector<OCDF> ocdfvec_11;
	ocdfvec_11 = cut_data_per_time(ocdfvec, time_border, true, true);
	ASSERT_EQ(ocdfvec_11.size(), 31);
	EXPECT_EQ(ocdfvec_11[0].time, time_border);
	ocdfvec_11.clear();

	time_border = 999999999;
	vector<OCDF> ocdfvec_20;
	ocdfvec_20 = cut_data_per_time(ocdfvec, time_border, false, false);
	ASSERT_EQ(ocdfvec_20.size(), 100);
	ocdfvec_20.clear();

	time_border = 0;
	vector<OCDF> ocdfvec_21;
	ocdfvec_21 = cut_data_per_time(ocdfvec, time_border, false, false);
	ASSERT_EQ(ocdfvec_21.size(), 0);
	ocdfvec_21.clear();
	ocdfvec.clear();

	int begin_time = 66417975;
	vector<OCDF> tdfvec_30;
	tdfvec_30 = cut_data_per_time(ocdfvec, begin_time, false, false);
	ASSERT_EQ(tdfvec_30.size(), 0);
	tdfvec_30.clear();
}

TEST(Common_DataReformer, OCDF_Pars) {
	vector<OCDF> ocdfvec;
	string file_name = "TestData.ocdf.bin";
	short cid = 0;
	int full_size = 0;
	ocdfvec = binload_OCDF_data(file_name, full_size);
	ASSERT_EQ(ocdfvec.size(), 100);

	cid = 1;
	vector<OCDF> first_cid;
	first_cid = parsing_data_per_cid(ocdfvec, cid);
	ASSERT_EQ(first_cid.size(), 0);
	first_cid.clear();

	cid = 2;
	vector<OCDF> second_cid;
	second_cid = parsing_data_per_cid(ocdfvec, cid);
	ASSERT_EQ(second_cid.size(), 2);
	EXPECT_EQ(second_cid[0].cid, 2);
	second_cid.clear(); 

	cid = 3;
	vector<OCDF> third_cid;
	third_cid = parsing_data_per_cid(ocdfvec, cid);
	ASSERT_EQ(third_cid.size(), 36);
	EXPECT_EQ(third_cid[35].cid, 3);
	third_cid.clear();

	cid = 4;
	vector<OCDF> forth_cid;
	forth_cid = parsing_data_per_cid(ocdfvec, cid);
	ASSERT_EQ(forth_cid.size(), 0);
	forth_cid.clear();

	cid = 5;
	vector<OCDF> fifth_cid;
	fifth_cid = parsing_data_per_cid(ocdfvec, cid);
	ASSERT_EQ(fifth_cid.size(), 32);
	EXPECT_EQ(fifth_cid[31].cid, 5);
	fifth_cid.clear();

	cid = 6;
	vector<OCDF> sixth_cid;
	sixth_cid = parsing_data_per_cid(ocdfvec, cid);
	ASSERT_EQ(sixth_cid.size(), 30);
	EXPECT_EQ(sixth_cid[29].cid, 6);
	sixth_cid.clear();
}

TEST(Common_DataReformer, OCDF_Rigth_Range) {
	vector<OCDF> ocdfvec;
	string file_name = "TestData.ocdf.bin";
	long range = 2;
	short cid = 5;
	int full_size = 0;
	ocdfvec = binload_OCDF_data(file_name, full_size);
	ASSERT_EQ(ocdfvec.size(), 100);

	vector<OCDF> rr2ocdfvec;
	rr2ocdfvec = parsing_data_per_cid(ocdfvec, cid);
	ASSERT_EQ(rr2ocdfvec.size(), 32);
	rr2ocdfvec = right_range(rr2ocdfvec, range);
	ASSERT_EQ(rr2ocdfvec.size(), 96);
	rr2ocdfvec.clear();
}

TEST(Common_DataReformer, TDF_Cut_Quantity) {
	vector<TDF> tdfvec;
	string file_name = "TestData.tdf.bin";
	int full_size = 0;
	tdfvec = binload_TDF_data(file_name, full_size);
	ASSERT_EQ(tdfvec.size(), 100);

	vector<TDF> tdfvec_right;
	tdfvec_right = cut_quntity_data(tdfvec, 40, false);
	ASSERT_EQ(tdfvec_right.size(), 40);
	EXPECT_EQ(tdfvec_right[39].time, 66419491);
	tdfvec_right.clear();

	vector<TDF> tdfvec_left;
	tdfvec_left = cut_quntity_data(tdfvec, 40, true);
	ASSERT_EQ(tdfvec_left.size(), 40);
	EXPECT_EQ(tdfvec_left[39].time, 66419791);
	tdfvec_left.clear();

	vector<TDF> tdfvec_over;
	tdfvec_over = cut_quntity_data(tdfvec, 1000, true);
	ASSERT_EQ(tdfvec_over.size(), 100);
	tdfvec_over.clear();
	tdfvec.clear();
}

TEST(Common_DataReformer, TDF_Cut_Percent) {
	vector<TDF> tdfvec;
	string file_name = "TestData.tdf.bin";
	int full_size = 0;
	tdfvec = binload_TDF_data(file_name, full_size);
	ASSERT_EQ(tdfvec.size(), 100);

	vector<TDF> tdfvec_right;
	tdfvec_right = cut_percent_data(tdfvec, 0.4, false);
	ASSERT_EQ(tdfvec_right.size(), 40);
	EXPECT_EQ(tdfvec_right[39].time, 66419491);
	tdfvec_right.clear();

	vector<TDF> tdfvec_left;
	tdfvec_left = cut_percent_data(tdfvec, 0.4, true);
	ASSERT_EQ(tdfvec_left.size(), 40);
	EXPECT_EQ(tdfvec_left[39].time, 66419791);
	tdfvec_left.clear();

	vector<TDF> tdfvec_over;
	tdfvec_over = cut_percent_data(tdfvec, 1000, true);
	ASSERT_EQ(tdfvec_over.size(), 100);
	tdfvec_over.clear();

	vector<TDF> tdfvec_milli;
	tdfvec_milli = cut_percent_data(tdfvec, 0.00001, true);
	ASSERT_EQ(tdfvec_milli.size(), 0);
	tdfvec_milli.clear();
	tdfvec.clear();
}

TEST(Common_DataReformer, TDF_Cut_Per_time) {
	vector<TDF> tdfvec;
	string file_name = "TestData.tdf.bin";
	int full_size = 0;
	long long time_border = 66419641;
	tdfvec = binload_TDF_data(file_name, full_size);
	ASSERT_EQ(tdfvec.size(), 100);

	vector<TDF> tdfvec_00;
	tdfvec_00 = cut_data_per_time(tdfvec, time_border, false, false);
	ASSERT_EQ(tdfvec_00.size(), 69);
	EXPECT_EQ(tdfvec_00[68].time, 66419636);
	tdfvec_00.clear();

	vector<TDF> tdfvec_01;
	tdfvec_01 = cut_data_per_time(tdfvec, time_border, false, true);
	ASSERT_EQ(tdfvec_01.size(), 70);
	EXPECT_EQ(tdfvec_01[69].time, time_border);
	tdfvec_01.clear();

	vector<TDF> tdfvec_10;
	tdfvec_10 = cut_data_per_time(tdfvec, time_border, true, false);
	ASSERT_EQ(tdfvec_10.size(), 30);
	EXPECT_EQ(tdfvec_10[0].time, 66419646);
	tdfvec_10.clear();

	vector<TDF> tdfvec_11;
	tdfvec_11 = cut_data_per_time(tdfvec, time_border, true, true);
	ASSERT_EQ(tdfvec_11.size(), 31);
	EXPECT_EQ(tdfvec_11[0].time, time_border);
	tdfvec_11.clear();

	time_border = 999999999;
	vector<TDF> tdfvec_20;
	tdfvec_20 = cut_data_per_time(tdfvec, time_border, false, false);
	ASSERT_EQ(tdfvec_20.size(), 100);
	tdfvec_20.clear();

	time_border = 0;
	vector<TDF> tdfvec_21;
	tdfvec_21 = cut_data_per_time(tdfvec, time_border, false, false);
	ASSERT_EQ(tdfvec_21.size(), 0);
	tdfvec_21.clear();

	int begin_time = 66417975;
	vector<TDF> tdfvec_30;
	tdfvec_30 = cut_data_per_time(tdfvec, begin_time, false, false);
	ASSERT_EQ(tdfvec_30.size(), 0);
	tdfvec_30.clear();
}

TEST(Common_DataReformer, TDF_Pars) {
	vector<TDF> TDFvec;
	string file_name = "TestData.TDF.bin";
	short cid = 0;
	int full_size = 0;
	TDFvec = binload_TDF_data(file_name, full_size);
	ASSERT_EQ(TDFvec.size(), 100);

	cid = 1;
	vector<OCDF> first_cid;
	first_cid = parsing_data_per_cid(TDFvec, cid);
	EXPECT_EQ(first_cid.size(), 100);
	first_cid.clear();

	cid = 2;
	vector<OCDF> second_cid;
	second_cid = parsing_data_per_cid(TDFvec, cid);
	EXPECT_EQ(second_cid.size(), 100);
	second_cid.clear();

	cid = 3;
	vector<OCDF> third_cid;
	third_cid = parsing_data_per_cid(TDFvec, cid);
	EXPECT_EQ(third_cid.size(), 100);
	third_cid.clear();

	cid = 4;
	vector<OCDF> forth_cid;
	forth_cid = parsing_data_per_cid(TDFvec, cid);
	EXPECT_EQ(forth_cid.size(), 100);
	forth_cid.clear();

	cid = 5;
	vector<OCDF> fifth_cid;
	fifth_cid = parsing_data_per_cid(TDFvec, cid);
	EXPECT_EQ(fifth_cid.size(), 100);
	fifth_cid.clear();

	cid = 6;
	vector<OCDF> sixth_cid;
	sixth_cid = parsing_data_per_cid(TDFvec, cid);
	EXPECT_EQ(sixth_cid.size(), 100);
	sixth_cid.clear();
}