#include "StrDivider.Tests.h"

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