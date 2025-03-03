#include "StrDivider.Tests.h"

TEST(Common_StrDivider, Extract_Last_N_Chars) {
	std::string answer;
	std::string src;
	int len;

	src = "";
	len = 5;
	answer = extract_last_n_chars(src, len);
	EXPECT_EQ(answer, "");

	src = "D:\\PASTER-CORE\\PAIA\\CMakeLists.txt";
	len = 3;
	answer = extract_last_n_chars(src, len);
	EXPECT_EQ(answer, "txt");

	src = "test_str";
	len = src.length();
	answer = extract_last_n_chars(src, len);
	EXPECT_EQ(answer, "test_str");

	len = src.length() - 3;
	answer = extract_last_n_chars(src, len);
	EXPECT_EQ(answer, "t_str");

	len = src.length() + 2;
	answer = extract_last_n_chars(src, len);
	EXPECT_EQ(answer, "test_str");

	len = 0;
	answer = extract_last_n_chars(src, len);
	EXPECT_EQ(answer, "");
}

TEST(Common_StrDivider, Extract_Last_N_Chars_EXC) {
	std::string answer;
	std::string src;
	int len;
	bool test_result = false;

	src = "test_str";
	len = (-4);
	try {
		answer = extract_last_n_chars(src, len);
	}
	catch (const CommonException& CmnEXC) {
		test_result = true;
	}
	EXPECT_TRUE(test_result);
}


TEST(Common_StrDivider, Extract_First_N_Chars) {
	std::string answer;
	std::string src;
	int len;

	src = "";
	len = 5;
	answer = extract_first_n_chars(src, len);
	EXPECT_EQ(answer, "");

	src = "D:\\PASTER-CORE\\PAIA\\CMakeLists.txt";
	len = 6;
	answer = extract_first_n_chars(src, len);
	EXPECT_EQ(answer, "D:\\PAS");

	src = "test_str";
	len = src.length();
	answer = extract_first_n_chars(src, len);
	EXPECT_EQ(answer, "test_str");

	len = src.length() - 3;
	answer = extract_first_n_chars(src, len);
	EXPECT_EQ(answer, "test_");

	len = src.length() + 2;
	answer = extract_first_n_chars(src, len);
	EXPECT_EQ(answer, "test_str");

	len = 0;
	answer = extract_first_n_chars(src, len);
	EXPECT_EQ(answer, "");
}

TEST(Common_StrDivider, Extract_First_N_Chars_EXC) {
	std::string answer;
	std::string src;
	int len;
	bool result = false;

	src = "test_str";
	len = (-4);
	try {
		answer = extract_first_n_chars(src, len);
	}
	catch (const CommonException& CmnEXC) {
		result = true;
	}
	EXPECT_TRUE(result);
}


TEST(Common_StrDivider, Split_String_Char_Descriptor) {
	std::vector<std::string> answer;
	char descriptor;

	std::string void_str = "";
	answer = split_string(void_str, descriptor);
	ASSERT_EQ(answer.size(), 1);

	std::string hello_world_str = "hello world ";

	descriptor = ';';
	answer = split_string(hello_world_str, descriptor);
	ASSERT_EQ(answer.size(), 1);

	descriptor = ' ';
	answer = split_string(hello_world_str, descriptor);
	ASSERT_EQ(answer.size(), 3);
	EXPECT_EQ(answer[1], "world");
	EXPECT_EQ(answer[2], "");

	hello_world_str += ' ';
	answer = split_string(hello_world_str, descriptor);
	ASSERT_EQ(answer.size(), 4);
	EXPECT_EQ(answer[0], "hello");

	hello_world_str = " " + hello_world_str;
	answer = split_string(hello_world_str, descriptor);
	ASSERT_EQ(answer.size(), 5);
	EXPECT_EQ(answer[1], "hello");
	EXPECT_EQ(answer[0], "");

	std::string spased_str = "       ";
	answer = split_string(spased_str, descriptor);
	ASSERT_EQ(answer.size(), 8);
	EXPECT_EQ(answer[4], "");
	EXPECT_EQ(answer[0], "");
}

TEST(Common_StrDivider, Split_String_Str_Symbol_Descriptor) {
	vector<string> answer;
	string descriptor;

	string void_str = "";
	answer = split_string(void_str, descriptor);
	ASSERT_EQ(answer.size(), 1);

	string hello_world_str = "hello world ";

	descriptor = ";";
	answer = split_string(hello_world_str, descriptor);
	ASSERT_EQ(answer.size(), 1);

	descriptor = " ";
	answer = split_string(hello_world_str, descriptor);
	ASSERT_EQ(answer.size(), 3);
	EXPECT_EQ(answer[1], "world");
	EXPECT_EQ(answer[2], "");

	hello_world_str += ' ';
	answer = split_string(hello_world_str, descriptor);
	ASSERT_EQ(answer.size(), 4);
	EXPECT_EQ(answer[0], "hello");

	hello_world_str = " " + hello_world_str;
	answer = split_string(hello_world_str, descriptor);
	ASSERT_EQ(answer.size(), 5);
	EXPECT_EQ(answer[1], "hello");
	EXPECT_EQ(answer[0], "");

	std::string spased_str = "       ";
	answer = split_string(spased_str, descriptor);
	ASSERT_EQ(answer.size(), 8);
	EXPECT_EQ(answer[4], "");
	EXPECT_EQ(answer[0], "");
}

TEST(Common_StrDivider, Split_String_Str_Text_Descriptor) {
	vector<string> answer;
	string descriptor;

	string text = "hello, my name is googletest, nice to meet you!";
	descriptor = ", ";
	answer = split_string(text, descriptor);
	ASSERT_EQ(answer.size(), 3);


	text += ", ";
	answer = split_string(text, descriptor);
	ASSERT_EQ(answer.size(), 4);

	descriptor = "xyx";
	answer = split_string(text, descriptor);
	ASSERT_EQ(answer.size(), 1);

	string hard_str = "xyxkdflxyxsdjfkxyxxyxjdlfjxyxyxsldxyxxyfl";
	answer = split_string(hard_str, descriptor);
	ASSERT_EQ(answer.size(), 7);
	EXPECT_EQ(answer[0], "");
	EXPECT_EQ(answer[1], "kdfl");
	EXPECT_EQ(answer[2], "sdjfk");
	EXPECT_EQ(answer[3], "");
	EXPECT_EQ(answer[4], "jdlfj");
	EXPECT_EQ(answer[5], "yxsld");
	EXPECT_EQ(answer[6], "xyfl");
}