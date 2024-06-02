#include "PAIA.h"

using namespace std;

int main()
{
	srand(time(NULL));

	string file_name;
	std::cout << "Enter the data file name: ";
	std::cin >> file_name;
	
	size_t input_range;
	size_t hidden_range;
	size_t output_range;

	std::cout << "Enter the input range: ";
	std::cin >> input_range;

	std::cout << "Enter the hidden range: ";
	std::cin >> hidden_range;

	std::cout << "Enter the output range: ";
	std::cin >> output_range;

	std::clock_t start, finish;
	start = clock();
		DataVector data(file_name);
		load_data(data, file_name);

		DataVector train_data;
		cut_vector(train_data, data, 0.67);
		DataVector test_data;
		cut_vector(test_data, data, 0.33, true, input_range);
	finish = clock();
	std::cout << "data read and proc per time = " 
		<< (double)(finish - start) / CLOCKS_PER_SEC << " sec\n";
	
	LSTM lstm(0.001, 150, input_range, hidden_range, output_range);
	lstm.fit(train_data, 2.0, input_range * 1000);
	
	start = clock();
		lstm.predict(test_data);
	finish = clock();
	std::cout << "data predict per time = "
		<< (double)(finish - start) / CLOCKS_PER_SEC << " sec\n";

	start = clock();
		dump_data(train_data, "train.csv");
		dump_data(test_data, "test.csv");
	finish = clock();
	std::cout << "data dump per time = "
		<< (double)(finish - start) / CLOCKS_PER_SEC << " sec\n";

	start = clock();
		dump_model(lstm, "012-038-001 d1c1 Shaman.lstm");
	finish = clock();
	std::cout << "model dump per time = "
		<< (double)(finish - start) / CLOCKS_PER_SEC << " sec\n";

	file_name = "python visual.py " + file_name;
	system(file_name.c_str());

	return 0;
}
