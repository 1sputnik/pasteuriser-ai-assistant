#include "DataDumper.Tests.h"

TEST(Common_DataDumper, OCDF_dump) {
	vector<OCDF> ocdf{
		OCDF(1,2,34.5),
		OCDF(2,3,45.64),
		OCDF(3,4,345.34),
		OCDF(4,5,534.32),
		OCDF(5,6,23.234)
	};
	string dump_file_name = "dump_test.ocdf.csv";

	dump_data(ocdf, dump_file_name);

	ocdf.clear();

	ocdf = load_OCDF_data(dump_file_name);
	EXPECT_EQ(ocdf.size(), 5);
	EXPECT_EQ(ocdf[0].value, 34.5);
}

TEST(Common_DataDumper, OCDF_bindump) {
	vector<OCDF> ocdf{
		OCDF(1,2,34.5),
		OCDF(2,3,45.64),
		OCDF(3,4,345.34),
		OCDF(4,5,534.32),
		OCDF(5,6,23.234)
	};
	string dump_file_name = "dump_test.ocdf.bin";

	bindump_data(ocdf, dump_file_name);

	ocdf.clear();

	ocdf = binload_OCDF_data(dump_file_name);
	EXPECT_EQ(ocdf.size(), 5);
	EXPECT_EQ(ocdf[0].value, 34.5);
}

TEST(Common_DataDumper, TDF_dump) {
	vector<TDF> tdf{
		TDF(1,23.4,34.5,24.3,345.123,65.34,234.3),
		TDF(2,5345.6,432.543,2534.54,342.45435,23.45435,23.43453),
		TDF(3,2343.4,3234.5,2643.4,2653.4,2533.4,22343.4),
		TDF(4,23.452,3453.5,23.554,22533.4,23.455,23.455),
		TDF(5,1243.4,344.455,243.454,24543.4,23.454,22353.234)
	};
	string dump_file_name = "dump_test.tdf.csv";

	dump_data(tdf, dump_file_name);

	tdf.clear();

	tdf = load_TDF_data(dump_file_name);
	EXPECT_EQ(tdf.size(), 5);
}

TEST(Common_DataDumper, TDF_bindump) {
	vector<TDF> tdf{
		TDF(1,23.4,34.5,24.3,345.123,65.34,234.3),
		TDF(2,5345.6,432.543,2534.54,342.45435,23.45435,23.43453),
		TDF(3,2343.4,3234.5,2643.4,2653.4,2533.4,22343.4),
		TDF(4,23.452,3453.5,23.554,22533.4,23.455,23.455),
		TDF(5,1243.4,344.455,243.454,24543.4,23.454,22353.234)
	};
	string dump_file_name = "dump_test.tdf.bin";

	bindump_data(tdf, dump_file_name);

	tdf.clear();

	tdf = binload_TDF_data(dump_file_name);
	EXPECT_EQ(tdf.size(), 5);
}
