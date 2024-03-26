#include "DataFormats.h"

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////// OCDF ////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

OneCIDDataFormat::OneCIDDataFormat() {
	this->cid = 0;
	this->time = 0;
	this->value = 0;
}
OneCIDDataFormat::OneCIDDataFormat(const OneCIDDataFormat& data) {
	this->cid = data.cid;
	this->time = data.time;
	this->value = data.value;
}
OneCIDDataFormat::OneCIDDataFormat(short cid, int time, float value) {
	this->cid = cid;
	this->time = time;
	this->value = value;
}
std::ostream& operator<<(std::ostream& os, const OneCIDDataFormat& data) {
	os << data.cid << ";" << data.time << ";" << data.value;
	return os;
}
std::istream& operator>>(std::istream& is, OneCIDDataFormat& data) {
	char buffer_char;
	is >> data.cid >> buffer_char >> data.time >> buffer_char >> data.value;
	return is;
}
void OneCIDDataFormat::operator=(const OneCIDDataFormat& data) {
	this->cid = data.cid;
	this->time = data.time;
	this->value = data.value;
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////// TDF ////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

TableDataFormat::TableDataFormat() {
	this->time = 0;
	this->cid_1_value = 0;
	this->cid_2_value = 0;
	this->cid_3_value = 0;
	this->cid_4_value = 0;
	this->cid_5_value = 0;
	this->cid_6_value = 0;
}
TableDataFormat::TableDataFormat(const TableDataFormat& data) {
	this->time = data.time;
	this->cid_1_value = data.cid_1_value;
	this->cid_2_value = data.cid_2_value;
	this->cid_3_value = data.cid_3_value;
	this->cid_4_value = data.cid_4_value;
	this->cid_5_value = data.cid_5_value;
	this->cid_6_value = data.cid_6_value;
}
TableDataFormat::TableDataFormat(int time, float cid_1_value, float cid_2_value, float cid_3_value, float cid_4_value, float cid_5_value, float cid_6_value) {
	this->time = time;
	this->cid_1_value = cid_1_value;
	this->cid_2_value = cid_2_value;
	this->cid_3_value = cid_3_value;
	this->cid_4_value = cid_4_value;
	this->cid_5_value = cid_5_value;
	this->cid_6_value = cid_6_value;
}
std::ostream& operator<<(std::ostream& os, const TableDataFormat& data) {
	os << data.time << ";" << data.cid_1_value << ";" << data.cid_2_value << ";" << data.cid_3_value << ";" << data.cid_4_value << ";" << data.cid_5_value << ";" << data.cid_6_value;
	return os;
}
std::istream& operator>>(std::istream& is, TableDataFormat& data) {
	char buffer_char;
	is >> data.time >> buffer_char >> data.cid_1_value >> buffer_char >> data.cid_2_value >> buffer_char >> data.cid_3_value
		>> buffer_char >> data.cid_4_value >> buffer_char >> data.cid_5_value >> buffer_char >> data.cid_6_value;
	return is;
}
void TableDataFormat::operator=(const TableDataFormat& data) {
	this->time = data.time;
	this->cid_1_value = data.cid_1_value;
	this->cid_2_value = data.cid_2_value;
	this->cid_3_value = data.cid_3_value;
	this->cid_4_value = data.cid_4_value;
	this->cid_5_value = data.cid_5_value;
	this->cid_6_value = data.cid_6_value;
}
