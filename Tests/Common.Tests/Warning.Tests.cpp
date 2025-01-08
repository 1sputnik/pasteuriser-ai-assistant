#include "Warning.Tests.h"

void msg_warning_subhook(std::string msg = "", short color = 0) {
	return;
}

const auto MSG_WARNING_SUBHOOK = subhook_new(
	reinterpret_cast<void*>(&msg_warning),
	reinterpret_cast<void*>(&msg_warning_subhook),
	SUBHOOK_64BIT_OFFSET);

TEST(Common_StrConverter, String_Symbol_To_Bool) {
	bool test;
	bool result;

	subhook_install(MSG_WARNING_SUBHOOK);

	std::string invalid_str = "hello";
	EXPECT_FALSE(string_symbol_to_bool(invalid_str, test));

	std::string wrong_str = "2";
	EXPECT_FALSE(string_symbol_to_bool(wrong_str, test));
	
	std::string zero_one_str = "01";
	EXPECT_FALSE(string_symbol_to_bool(zero_one_str, test));

	std::string void_str = "";
	EXPECT_FALSE(string_symbol_to_bool(zero_one_str, test));

	std::string zero = "0";
	EXPECT_TRUE(string_symbol_to_bool(zero, test));
	EXPECT_FALSE(test);

	std::string one = "1";
	EXPECT_TRUE(string_symbol_to_bool(one, test));
	EXPECT_TRUE(test);

	subhook_remove(MSG_WARNING_SUBHOOK);
}

TEST(Common_StrConverter, Is_int) {
	std::string invalid_str = "hello";
	EXPECT_FALSE(is_int_numeric(invalid_str));

	std::string void_str = "";
	EXPECT_FALSE(is_int_numeric(void_str));

	std::string corrupted_str = "231##33?2";
	EXPECT_FALSE(is_int_numeric(corrupted_str));

	std::string real_n_dot_str = "34.545";
	EXPECT_FALSE(is_int_numeric(real_n_dot_str));

	std::string real_n_comma_str = "34,124";
	EXPECT_FALSE(is_int_numeric(real_n_comma_str));

	std::string valid_str = "12345";
	EXPECT_TRUE(is_int_numeric(valid_str));
}

TEST(Common_StrConverter, Is_double) {
	std::string invalid_str = "hello";
	EXPECT_FALSE(is_double_numeric(invalid_str));

	std::string void_str = "";
	EXPECT_FALSE(is_double_numeric(void_str));

	std::string corrupted_str = "231##33?2";
	EXPECT_FALSE(is_double_numeric(corrupted_str));

	std::string real_n_dot_str = "34.545";
	EXPECT_TRUE(is_double_numeric(real_n_dot_str));

	std::string real_n_comma_str = "34,124";
	EXPECT_FALSE(is_double_numeric(real_n_comma_str));

	std::string valid_str = "12345";
	EXPECT_TRUE(is_double_numeric(valid_str));
}

TEST(Common_StrDivider, Extract_Last_N_Chars) {
	std::string answer;

	std::string void_str = "";
	answer = extractLastNChars(void_str, 5);
	EXPECT_EQ(answer, "");

	std::string file_name = "D:\\PASTER-CORE\\PAIA\\CMakeLists.txt";
	answer = extractLastNChars(file_name, 3);
	EXPECT_EQ(answer, "txt");

	std::string str = "test_str";
	int str_len_1 = str.length();
	answer = extractLastNChars(str, str_len_1);
	EXPECT_EQ(answer, "test_str");

	int str_len_2 = str.length() - 3;
	answer = extractLastNChars(str, str_len_2);
	EXPECT_EQ(answer, "t_str");

	int str_len_3 = str.length() + 2;
	answer = extractLastNChars(str, str_len_3);
	EXPECT_EQ(answer, "test_str");
}

TEST(Common_StrDivider, Split_String) {
	std::vector<std::string> answer;
	char descriptor;

	std::string void_str = "";
	answer = split_string(void_str, descriptor);
	EXPECT_EQ(answer.size(), 1);

	std::string hello_world_str = "hello world ";

	descriptor = ';';
	answer = split_string(hello_world_str, descriptor);
	EXPECT_EQ(answer.size(), 1);

	descriptor = ' ';
	answer = split_string(hello_world_str, descriptor);
	EXPECT_EQ(answer.size(), 3);
	EXPECT_EQ(answer[1], "world");
	EXPECT_EQ(answer[2], "");

	hello_world_str += ' ';
	answer = split_string(hello_world_str, descriptor);
	EXPECT_EQ(answer.size(), 4);
	EXPECT_EQ(answer[0], "hello");

	hello_world_str = " " + hello_world_str;
	answer = split_string(hello_world_str, descriptor);
	EXPECT_EQ(answer.size(), 5);
	EXPECT_EQ(answer[1], "hello");
	EXPECT_EQ(answer[0], "");

	std::string spased_str = "       ";
	answer = split_string(spased_str, descriptor);
	EXPECT_EQ(answer.size(), 8);
	EXPECT_EQ(answer[4], "");
	EXPECT_EQ(answer[0], "");
}