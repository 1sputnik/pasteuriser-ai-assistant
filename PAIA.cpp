#include "PAIA.h"

using namespace std;

int main()
{
	srand(time(NULL));

	size_t input_range = 4;
	size_t hidden_range = 48;
	size_t output_range = 2;

	string file_name;
	std::cout << "Enter the data file name: ";
	std::cin >> file_name;

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
	
	LSTM lstm(0.001, 0, input_range, hidden_range, output_range);
	lstm.fit(train_data);
	
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

	system("python visual.py");

	return 0;
}
