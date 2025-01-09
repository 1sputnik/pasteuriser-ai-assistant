#include "StrConverter.Tests.h"

TEST(Common_StrConverter, String_To_Bool) {
	bool test;
	bool result;

	std::string invalid_str = "hello";
	EXPECT_FALSE(string_to_bool(invalid_str, test));

	std::string wrong_str = "2";
	EXPECT_FALSE(string_to_bool(wrong_str, test));

	std::string zero_one_str = "01";
	EXPECT_FALSE(string_to_bool(zero_one_str, test));

	std::string void_str = "";
	EXPECT_FALSE(string_to_bool(zero_one_str, test));

	std::string zero = "0";
	EXPECT_TRUE(string_to_bool(zero, test));
	EXPECT_FALSE(test);

	std::string one = "1";
	EXPECT_TRUE(string_to_bool(one, test));
	EXPECT_TRUE(test);
}

TEST(Common_StrConverter, Is_Integer) {
	std::string invalid_str = "hello";
	EXPECT_FALSE(is_integer(invalid_str));

	std::string void_str = "";
	EXPECT_FALSE(is_integer(void_str));

	std::string corrupted_str = "231##33?2";
	EXPECT_FALSE(is_integer(corrupted_str));

	std::string real_n_dot_str = "34.545";
	EXPECT_FALSE(is_integer(real_n_dot_str));

	std::string real_n_comma_str = "34,124";
	EXPECT_FALSE(is_integer(real_n_comma_str));

	std::string valid_str = "12345";
	EXPECT_TRUE(is_integer(valid_str));
}

TEST(Common_StrConverter, Is_Double) {
	std::string invalid_str = "hello";
	EXPECT_FALSE(is_double(invalid_str));

	std::string void_str = "";
	EXPECT_FALSE(is_double(void_str));

	std::string corrupted_str = "231##33?2";
	EXPECT_FALSE(is_double(corrupted_str));

	std::string real_n_dot_str = "34.545";
	EXPECT_TRUE(is_double(real_n_dot_str));

	std::string real_n_comma_str = "34,124";
	EXPECT_FALSE(is_double(real_n_comma_str));

	std::string valid_str = "12345";
	EXPECT_TRUE(is_double(valid_str));
}

TEST(Common_StrConverter, Is_Menu_Point) {
	std::string invalid_str = "hello";
	EXPECT_FALSE(is_menu_point(invalid_str));

	std::string void_str = "";
	EXPECT_FALSE(is_menu_point(void_str));

	std::string corrupted_str = "231##33?2";
	EXPECT_FALSE(is_menu_point(corrupted_str));

	std::string real_n_dot_str = "34.545";
	EXPECT_FALSE(is_menu_point(real_n_dot_str));

	std::string integer_str = "12345";
	EXPECT_FALSE(is_menu_point(integer_str));

	std::string one_str = "1";
	EXPECT_TRUE(is_menu_point(one_str));

	std::string nine_str = "9";
	EXPECT_TRUE(is_menu_point(nine_str));

	std::string di_str = "d";
	EXPECT_FALSE(is_menu_point(di_str));

	std::string comma_str = ",";
	EXPECT_FALSE(is_menu_point(comma_str));
}

TEST(Common_StrConverter, Is_Target_Format_Data) {
	std::string invalid_str = "hello";
	EXPECT_FALSE(is_target_format_data(invalid_str));

	std::string void_str = "";
	EXPECT_FALSE(is_target_format_data(void_str));

	std::string corrupted_str = "231##33?2";
	EXPECT_FALSE(is_target_format_data(corrupted_str));

	std::string real_n_dot_str = "34.545";
	EXPECT_TRUE(is_target_format_data(real_n_dot_str));

	std::string real_n_comma_str = "34,124";
	EXPECT_FALSE(is_target_format_data(real_n_comma_str));

	std::string valid_str = "1;23;45.4";
	EXPECT_TRUE(is_target_format_data(valid_str));
}