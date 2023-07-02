#include "network.h"

// ---------- ������������ ������� ----------

// �������������� �������
inline double sigm(double x) {
	return 1.0 / (1.0 + exp(-x));
}
inline double dif_sigm(double y) {
	return y * (1.0 - y);
}
inline double dif_tanh(double y) {
	return 1.0 - y * y;
}

// ������� (���������� �������)
inline void create_hollow_vector(double*& arr, int size) {
	arr = new double[size];
}
inline void randfill_vector(double*& arr, int size, double tiff) {
	// tiff - ������� ����� ����� � �������� �����: value = 0.<tiff><[0, rank - 1]>
	double rank = 1000; // ������� ������ ��� ����� ��� �������: [0, rank - 1]
	for (size_t i = 0; i < size; i++) 
		arr[i] = double((rand() % (2 * int(rank) + 1)) - int(1.0 * rank))
								/ (tiff * double(rank));
}
inline void create_random_vector(double*& arr, int size, double tiff = 1.0) {
	create_hollow_vector(arr, size);
	randfill_vector(arr, size, tiff);
}
inline void copy_vector(double*& in_data, double*& out_data, size_t size) {
	delete[] in_data;
	in_data = new double[size];
	for (int i = 0; i < size; i++)
		in_data[i] = out_data[i];
}

inline double* create_input_vector(DataVector& data, int cut_border, int b) {
	double* x = new double[cut_border];
	for (int i = 0; i < cut_border; i++)
		x[i] = data[i + b].value;
	return x;
};

// ������� (��������� �������)
inline void create_hollow_matrix(double**& arr, size_t rows, size_t colls) {
	arr = new double* [rows];
	for (size_t i = 0; i < rows; i++)
		arr[i] = new double[colls];
}
inline void randfill_matrix(double**& arr, size_t rows, size_t colls, double tiff) {
	// tiff - ������� ����� ����� � �������� �����: value = 0.<tiff><[0, rank -1]>
	double rank = 1000; // ������� ������ ��� ����� ��� �������: [0, rank - 1]
	for (size_t i = 0; i < rows; i++) 
		for (size_t j = 0; j < colls; j++)
			arr[i][j] = double((rand() % (2 * int(rank) + 1)) - int(1.0 * rank))
								/ (tiff * double(rank));
}
inline void fill_matrix(double**& arr, size_t rows, size_t colls, double value = 0) {
	for (size_t i = 0; i < rows; i++)
		for (size_t j = 0; j < colls; j++)
			arr[i][j] = value;
}
inline void create_random_matrix(double**& arr, size_t rows, size_t colls, double tiff = 1.0) {
	create_hollow_matrix(arr, rows, colls);
	randfill_matrix(arr, rows, colls, tiff);
}
inline void create_matrix(double**& arr, size_t rows, size_t colls, double value = 0) {
	create_hollow_matrix(arr, rows, colls);
	fill_matrix(arr, rows, colls, value);
}
inline void copy_matrix(double**& in, double**& out, size_t rows, size_t colls) {
	for (size_t i = 0; i < rows; i++)
		for (size_t j = 0; j < colls; j++)
			in[i][j] = out[i][j];
}
inline void delete_matrix(double**& arr, size_t rows) {
	for (size_t i = 0; i < rows; i++)
		delete[] arr[i];
	delete[] arr;
}


// ---------- Neuron ----------

RecurrentNeuron::RecurrentNeuron(double rate, unint16 epochs, unint16 input_range) {
	this->rate = rate;
	this->epochs = epochs;
	this->input_range = input_range;
}


// ---------- LSTM ----------

void LSTM::select_memory_for_temp_weight() {
	create_hollow_matrix(_W_f, hidden_range, input_range);
	create_hollow_matrix(_W_i, hidden_range, input_range);
	create_hollow_matrix(_W_g, hidden_range, input_range);
	create_hollow_matrix(_W_o, hidden_range, input_range);

	create_hollow_matrix(_U_f, hidden_range, hidden_range);
	create_hollow_matrix(_U_i, hidden_range, hidden_range);
	create_hollow_matrix(_U_g, hidden_range, hidden_range);
	create_hollow_matrix(_U_o, hidden_range, hidden_range);

	create_hollow_matrix(_W_y, output_range, hidden_range);
}
void LSTM::copy_weight() {
	copy_matrix(_W_f, W_f, hidden_range, input_range);
	copy_matrix(_W_i, W_i, hidden_range, input_range);
	copy_matrix(_W_o, W_o, hidden_range, input_range);
	copy_matrix(_W_g, W_g, hidden_range, input_range);

	copy_matrix(_U_f, U_f, hidden_range, hidden_range);
	copy_matrix(_U_i, U_i, hidden_range, hidden_range);
	copy_matrix(_U_o, U_o, hidden_range, hidden_range);
	copy_matrix(_U_g, U_g, hidden_range, hidden_range);
	
	copy_matrix(_W_y, W_y, output_range, hidden_range);
}
void LSTM::free_temp_weigth() { 
	delete_matrix(_W_f, hidden_range);
	delete_matrix(_W_i, hidden_range);
	delete_matrix(_W_g, hidden_range);
	delete_matrix(_W_o, hidden_range);

	delete_matrix(_U_f, hidden_range);
	delete_matrix(_U_i, hidden_range);
	delete_matrix(_U_g, hidden_range);
	delete_matrix(_U_o, hidden_range);

	delete_matrix(_W_y, output_range);
}

void LSTM::train(double* x, double* y_real, size_t k) {
	// ----- ������ ������ -----
	// ������ �������
	double* forgate_gate = new double[this->hidden_range] {0};
	double* input_gate = new double[this->hidden_range] {0};
	double* output_gate = new double[this->hidden_range] {0};
	double* state_gate = new double[this->hidden_range] {0};

	// ������ ������ LSTM
	for (int i = 0; i < this->hidden_range; i++) { // ����������� ������� ����
		for (int j = 0; j < this->input_range; j++) {
			forgate_gate[i] += _W_f[i][j] * x[j];
			input_gate[i] += _W_i[i][j] * x[j];
			output_gate[i] += _W_o[i][j] * x[j];
			state_gate[i] += _W_g[i][j] * x[j];
		}
		for (int j = 0; j < this->hidden_range; j++) {
			forgate_gate[i] += _U_f[i][j] * h[k][j];
			input_gate[i] += _U_i[i][j] * h[k][j];
			output_gate[i] += _U_o[i][j] * h[k][j];
			state_gate[i] += _U_g[i][j] * h[k][j];
		}
		forgate_gate[i] = sigm(forgate_gate[i]);
		input_gate[i] = sigm(input_gate[i]); 
		output_gate[i] = sigm(output_gate[i]);
		state_gate[i] = tanh(state_gate[i]);
	}

	// �������� ������������
	double e = 0;
	double* y_predict = new double [this->output_range] {0};
	for (size_t i = 0; i < this->output_range; i++) {
		for (size_t j = 0; j < this->hidden_range; j++) {
			y_predict[i] += _W_y[i][j] * h[k + 1][j];
		}
		y_predict[i] = sigm(y_predict[i]); // �������� ������������ � ����������� �����
		e += 0.5 * (y_real[i] - y_predict[i]) * (y_real[i] - y_predict[i]);
	}
	e_predict += e / double(output_range);

	// ----- ����������� ����� -----

	if (fabs(e) > this->precision) {
		double* de_dy = new double[this->output_range];
		for (size_t i = 0; i < this->output_range; i++) {
			de_dy[i] = (y_predict[i] - y_real[i]) * dif_sigm(y_predict[i]);
		}

		double* de_dO = new double[this->hidden_range];
		double* de_dF = new double[this->hidden_range];
		double* de_dI = new double[this->hidden_range];
		double* de_dG = new double[this->hidden_range];
		double* de_dC = new double[this->hidden_range];
		double* de_dh = new double[this->hidden_range] {0};
		for (int i = 0; i < this->hidden_range; i++) {
			for (size_t j = 0; j < this->output_range; j++) {
				de_dh[i] += de_dy[j] * _W_y[j][i];
			}
			for (int j = 0; j < this->hidden_range; j++) {
				de_dh[i] += de_dF_futur[j] * _U_f[i][j] + de_dG_futur[j] * _U_g[i][j]
					+ de_dI_futur[j] * _U_i[i][j] + de_dO_futur[j] * _U_o[i][j];
			}
			de_dO[i] = de_dh[i] * tanh(C[k + 1][i]) * dif_sigm(output_gate[i]);
			de_dC[i] = de_dh[i] * output_gate[i] * dif_tanh(tanh(C[k + 1][i])) + de_dC_futur[i] * forgate_futur[i];
			de_dF[i] = de_dC[i] * C[k][i] * dif_sigm(forgate_gate[i]);
			de_dI[i] = de_dC[i] * state_gate[i] * dif_sigm(input_gate[i]);
			de_dG[i] = de_dC[i] * input_gate[i] * dif_tanh(state_gate[i]);
			for (int j = 0; j < this->input_range; j++) {
				this->W_i[i][j] -= this->rate * de_dI[i] * x[j];
				this->W_f[i][j] -= this->rate * de_dF[i] * x[j];
				this->W_o[i][j] -= this->rate * de_dO[i] * x[j];
				this->W_g[i][j] -= this->rate * de_dG[i] * x[j];
			}
			for (int j = 0; j < this->hidden_range; j++) {
				this->U_i[i][j] -= this->rate * de_dI[i] * h[k][j];
				this->U_f[i][j] -= this->rate * de_dF[i] * h[k][j];
				this->U_o[i][j] -= this->rate * de_dO[i] * h[k][j];
				this->U_g[i][j] -= this->rate * de_dG[i] * h[k][j];
			}
			for (size_t j = 0; j < this->output_range; j++) {
				this->W_y[j][i] -= this->rate * de_dy[j] * h[k + 1][i];
			}
		}

		// ���������� �������
		copy_vector(de_dF_futur, de_dF, this->hidden_range);
		copy_vector(de_dO_futur, de_dO, this->hidden_range);
		copy_vector(de_dI_futur, de_dI, this->hidden_range);
		copy_vector(de_dG_futur, de_dG, this->hidden_range);
		copy_vector(de_dC_futur, de_dC, this->hidden_range);
		delete[] de_dy;
		delete[] de_dh;
		delete[] de_dC;
		delete[] de_dO;
		delete[] de_dF;
		delete[] de_dI;
		delete[] de_dG;
	}
	else {
		for (int i = 0; i < this->hidden_range; i++) {
			de_dC_futur[i] = 0.0;
			de_dG_futur[i] = 0.0;
			de_dI_futur[i] = 0.0;
			de_dO_futur[i] = 0.0;
			de_dF_futur[i] = 0.0;
		}
	}
	copy_vector(forgate_futur, forgate_gate, this->hidden_range);

	// ������� ������, ���������� �� ������� ����
	delete[] forgate_gate;
	delete[] input_gate;
	delete[] state_gate;
	delete[] output_gate;
}
// TODO ��������� ������� � ������ output range 
void LSTM::fit(DataVector& train_data) {
	// ������ � ������������� �������
	Scaler sc;
	sc.configure(120, 0);

	// ������� ������� ������
	sc.scale(train_data);

	// �������� ���� � ����������� �� ������ ������ ������� �������
	size_t mode_bias = this->input_range;

	// ���������� � ��������
	double** x = new double* [train_data.size() - mode_bias]; // ������� ������� ������� ��� ������ ��������
	double** y_real = new double* [train_data.size() - mode_bias]; // ������ �������� �������� ������ ��������
	C = new double* [train_data.size() + 1 - mode_bias]; // + 1 ��� ������� �������� C[t + 1] (��������� ��������)
	h = new double* [train_data.size() + 1 - mode_bias];
	for (size_t i = 0; i <= train_data.size() - mode_bias; i++) { // ���������� ��������� ������������ � ������������� ������
		C[i] = new double[this->hidden_range] {0};
		h[i] = new double[this->hidden_range] {0};
	}

	// �������� ������ ��� ������������� ����
	select_memory_for_temp_weight();

	// �������� ����
	for (size_t i = 0; i < this->epochs; i++) {
		std::cout << "epoch = " << i + 1 << "/" << this->epochs << " ";
		std::clock_t start, finish;
		start = clock();

		// ��������� ������� �������� �����
		copy_weight();

		// �������� ������� ������ ����� ������������
		e_predict = 0;

		// ��������� ������� ������� � �� ��������������� �������� ��������, � �������� ����� ����������� ��������� ������������
		for (int j = 0; j < train_data.size() - mode_bias; j++) {
			x[j] = create_input_vector(train_data, mode_bias, j);
			//y_real[j] = train_data[mode_bias + j].value;
		}
		
		for (size_t j = 0; j < train_data.size() - mode_bias; j++) {
			forecast(x[j], j); // ���� ���� �����, ����� ��������� ������ ������� C � h ��� ������ �������� 
		}
		
		for (int j = train_data.size() - 1 - mode_bias; j >= 0; j--) {
			train(x[j], y_real[j], j); // ����� ��������� ��������������� �� �������		
			delete[] x[j]; // �������� ��� ����������� ����������
		}

		finish = clock();
		std::cout << "| time = " << (double)(finish - start) / CLOCKS_PER_SEC << " sec "
			<< "| E = " << e_predict / 2 << "\n";
	}

	// ��������� ������ ���� ����� ���������� � ������� ��� ��������
	copy_vector(last_C_from_train, C[train_data.size() - mode_bias], this->hidden_range);
	copy_vector(last_h_from_train, h[train_data.size() - mode_bias], this->hidden_range);

	// ������� ������
	free_temp_weigth();
	for (int i = 0; i <= train_data.size() - mode_bias; i++) {
		delete[] C[i];
		delete[] h[i];
	}
	delete[] C;
	delete[] h;
	delete[] x;
	delete[] y_real;

	// ��������������� ������
	sc.unscale(train_data);
}

double* LSTM::forecast(double* x, size_t k) {
	// ������ ������� � ����������� ������ ����
	double* forgate_gate = new double[this->hidden_range] {0};
	double* input_gate = new double[this->hidden_range] {0};
	double* output_gate = new double[this->hidden_range] {0};
	double* state_gate = new double[this->hidden_range] {0};
	for (int i = 0; i < this->hidden_range; i++) {  
		for (int j = 0; j < this->input_range; j++) { 
			forgate_gate[i] += W_f[i][j] * x[j];
			input_gate[i] += W_i[i][j] * x[j];   
			output_gate[i] += W_o[i][j] * x[j];  
			state_gate[i] += W_g[i][j] * x[j]; 
		}
		for (int j = 0; j < this->hidden_range; j++) { 
			forgate_gate[i] += U_f[i][j] * h[k][j]; 
			input_gate[i] += U_i[i][j] * h[k][j];
			output_gate[i] += U_o[i][j] * h[k][j]; 
			state_gate[i] += U_g[i][j] * h[k][j];   
		}
		forgate_gate[i] = sigm(forgate_gate[i]); 
		input_gate[i] = sigm(input_gate[i]); 
		output_gate[i] = sigm(output_gate[i]);
		state_gate[i] = tanh(state_gate[i]);
		// ����������� ����� ������������ ������
		C[k + 1][i] = forgate_gate[i] * C[k][i] + input_gate[i] * state_gate[i];
		// ����������� ����� ������������� ������
		h[k + 1][i] = output_gate[i] * tanh(C[k + 1][i]);
	}
	
	// ������������ �������
	double* y_predict = new double[this->output_range];
	for (size_t i = 0; i < this->output_range; i++) {
		for (size_t j = 0; j < this->hidden_range; j++) 
			y_predict[i] += W_y[i][j] * h[k + 1][j];
		y_predict[i] = sigm(y_predict[i]); // �������� ������������ � ����������� �����
	}

	// ������� ������, ���������� �� ������� ����
	delete[] forgate_gate;
	delete[] input_gate;
	delete[] state_gate;
	delete[] output_gate;

	// ���������� ������������� ����
	return y_predict;
}
void LSTM::predict(DataVector& test_data) {
	// ������ � ������������� �������
	Scaler sc;
	sc.configure(120, 0);

	// ������� ������� ������
	sc.scale(test_data);

	// ��������������, ��� ���������� ����� ������� �� ��� � ������, ������� ��������� ���
	size_t dist = test_data.count_distance();
	
	
	// TODO ���������� ��������� temp �� output range
	DataVector temp(1);
	temp = test_data;
	// --------------------------


	// ������ ������������ � ������������� ������
	C = new double* [2];
	h = new double* [2];
	C[0] = nullptr;
	h[0] = nullptr;
	copy_vector(C[0], last_C_from_train, this->hidden_range);
	copy_vector(h[0], last_h_from_train, this->hidden_range);
	C[1] = new double[this->hidden_range] {0};
	h[1] = new double[this->hidden_range] {0};

	// ��������� ������������
	double* x = nullptr;
	double* y_predict = nullptr;
	for (size_t i = 0; i < test_data.size(); i = i + this->output_range) {
		if (i > test_data.size() - this->input_range) {
			i = test_data.size() - this->input_range;
		}
		x = create_input_vector(test_data, this->input_range, i);
		y_predict = forecast(x); 
		for (int j = 0; j < this->output_range; j++) {
			temp[i + this->input_range + j].cid = test_data[0].cid;
			temp[i + this->input_range + j].time = test_data[0].time + dist * (i + j);
			temp[i + this->input_range + j].value = y_predict[j];
		}
		copy_vector(C[0], C[1], this->hidden_range);
		copy_vector(h[0], h[1], this->hidden_range);
		delete[] x;
		delete[] y_predict;
	}
	
	test_data = temp;

	// ������� ������������ � ������������� ������
	delete[] C[1];
	delete[] C[0];
	delete[] C;

	delete[] h[1];
	delete[] h[0];
	delete[] h;

	// �������������� ������� �� ���������� [0, 1] � ���������� ����������
	sc.unscale(test_data);
}

LSTM::LSTM(double rate, unint16 epochs, unint16 input_range, unint16 hidden_range, unint16 output_range) : RecurrentNeuron(rate, epochs, input_range) {
	this->hidden_range = hidden_range;
	this->output_range = output_range;

	// ����������, �������������� ������ ����
	de_dF_futur = new double[this->hidden_range] {0};
	de_dO_futur = new double[this->hidden_range] {0};
	de_dG_futur = new double[this->hidden_range] {0};
	de_dI_futur = new double[this->hidden_range] {0};
	de_dC_futur = new double[this->hidden_range] {0};
	forgate_futur = new double[this->hidden_range] {0};

	// ���� 
	create_random_matrix(W_f, hidden_range, input_range);
	create_random_matrix(W_i, hidden_range, input_range);
	create_random_matrix(W_g, hidden_range, input_range);
	create_random_matrix(W_o, hidden_range, input_range);

	create_random_matrix(U_f, hidden_range, hidden_range);
	create_random_matrix(U_i, hidden_range, hidden_range);
	create_random_matrix(U_g, hidden_range, hidden_range);
	create_random_matrix(U_o, hidden_range, hidden_range);

	create_random_matrix(W_y, output_range, hidden_range);
}
LSTM::~LSTM() {
	// ����������, �������������� ������ ����
	delete[] de_dF_futur;
	delete[] de_dO_futur;
	delete[] de_dI_futur;
	delete[] de_dG_futur;
	delete[] de_dC_futur;
	delete[] forgate_futur;

	// ���� 
	delete_matrix(W_f, hidden_range);
	delete_matrix(W_i, hidden_range);
	delete_matrix(W_g, hidden_range);
	delete_matrix(W_o, hidden_range);

	delete_matrix(U_f, hidden_range);
	delete_matrix(U_i, hidden_range);
	delete_matrix(U_g, hidden_range);
	delete_matrix(U_o, hidden_range);

	delete_matrix(W_y, output_range);
}


void load_model(LSTM& lstm, string file_name) {
	std::ifstream work_file;
	work_file.open(file_name, std::ios::binary | std::ios::in);

	// ranges
	unint16 temp_range;
	work_file.read((char*)&temp_range, sizeof(lstm.input_range));
	if (temp_range != lstm.input_range) {
		system("cls");
		std::cerr << "��� ������ ������ ������ � ����� ��������� ������: ������� ����������� ���������� ������ �� ��������� � ���������� � �������!\n";
		exit(0);
	}
	work_file.read((char*)&temp_range, sizeof(lstm.hidden_range));
	if (temp_range != lstm.hidden_range) {
		system("cls");
		std::cerr << "��� ������ ������ ������ � ����� ��������� ������: ������� ����������� ���������� ������ �� ��������� � ���������� � �������!\n";
		exit(0);
	}
	work_file.read((char*)&temp_range, sizeof(lstm.output_range));
	if (temp_range != lstm.output_range) {
		system("cls");
		std::cerr << "��� ������ ������ ������ � ����� ��������� ������: �������� ����������� ���������� ������ �� ��������� � ���������� � �������!\n";
		exit(0);
	}

	// ���������
	work_file.read((char*)&lstm.precision, sizeof(double));
	work_file.read((char*)&lstm.rate, sizeof(double));
	work_file.read((char*)&lstm.e_predict, sizeof(double));
	work_file.read((char*)&lstm.epochs, sizeof(unint16));

	lstm.last_h_from_train = new double[lstm.hidden_range];
	lstm.last_C_from_train = new double[lstm.hidden_range];
	
	for (size_t i = 0; i < lstm.hidden_range; i++) {
		for (size_t j = 0; j < lstm.hidden_range; j++) {
			work_file.read((char*)&lstm.U_f[i][j], sizeof(lstm.U_f[i][j]));
			work_file.read((char*)&lstm.U_i[i][j], sizeof(lstm.U_i[i][j]));
			work_file.read((char*)&lstm.U_o[i][j], sizeof(lstm.U_o[i][j]));
			work_file.read((char*)&lstm.U_g[i][j], sizeof(lstm.U_g[i][j]));
		}
		for (size_t j = 0; j < lstm.input_range; j++) {
			work_file.read((char*)&lstm.W_f[i][j], sizeof(lstm.W_f[i][j]));
			work_file.read((char*)&lstm.W_i[i][j], sizeof(lstm.W_i[i][j]));
			work_file.read((char*)&lstm.W_o[i][j], sizeof(lstm.W_o[i][j]));
			work_file.read((char*)&lstm.W_g[i][j], sizeof(lstm.W_g[i][j]));
		}
		for (size_t j = 0; j < lstm.output_range; j++) {
			work_file.read((char*)&lstm.W_y[i][j], sizeof(lstm.W_y[i][j]));
		}
		work_file.read((char*)&lstm.last_h_from_train[i], sizeof(lstm.last_h_from_train[i]));
		work_file.read((char*)&lstm.last_C_from_train[i], sizeof(lstm.last_C_from_train[i]));
	}
	work_file.close();
}
void dump_model(LSTM& lstm, string file_name) {
	if (lstm.last_h_from_train == nullptr || lstm.last_C_from_train == nullptr) {
		system("cls");
		std::cerr << "��� ���������� ������ ��������� ������: ������ ��������� ����������� ������!\n";
		exit(0);
	}

	std::ofstream result_file;
	result_file.open(file_name, std::ios::binary | std::ios::out);

	// ranges
	result_file.write((char*)&lstm.input_range, sizeof(lstm.input_range));
	result_file.write((char*)&lstm.hidden_range, sizeof(lstm.hidden_range));
	result_file.write((char*)&lstm.output_range, sizeof(lstm.output_range));

	// ���������
	result_file.write((char*)&lstm.precision, sizeof(lstm.precision));
	result_file.write((char*)&lstm.rate, sizeof(lstm.rate));
	result_file.write((char*)&lstm.e_predict, sizeof(lstm.e_predict));
	result_file.write((char*)&lstm.epochs, sizeof(lstm.epochs));

	// ���� � ������ 
	for (size_t i = 0; i < lstm.hidden_range; i++) {
		for (size_t j = 0; j < lstm.hidden_range; j++) {
			result_file.write((char*)&lstm.U_f[i][j], sizeof(lstm.U_f[i][j]));
			result_file.write((char*)&lstm.U_i[i][j], sizeof(lstm.U_i[i][j]));
			result_file.write((char*)&lstm.U_o[i][j], sizeof(lstm.U_o[i][j]));
			result_file.write((char*)&lstm.U_g[i][j], sizeof(lstm.U_g[i][j]));
		}
		for (size_t j = 0; j < lstm.input_range; j++) {
			result_file.write((char*)&lstm.W_f[i][j], sizeof(lstm.W_f[i][j]));
			result_file.write((char*)&lstm.W_i[i][j], sizeof(lstm.W_i[i][j]));
			result_file.write((char*)&lstm.W_o[i][j], sizeof(lstm.W_o[i][j]));
			result_file.write((char*)&lstm.W_g[i][j], sizeof(lstm.W_g[i][j]));
		}
		for (size_t j = 0; j < lstm.output_range; j++) {
			result_file.write((char*)&lstm.W_y[i][j], sizeof(lstm.W_y[i][j]));
		}
		result_file.write((char*)&lstm.last_h_from_train[i], sizeof(lstm.last_h_from_train[i]));
		result_file.write((char*)&lstm.last_C_from_train[i], sizeof(lstm.last_C_from_train[i]));
	}
	result_file.close();
}
