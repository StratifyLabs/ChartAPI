#include "chart/ChartJs.hpp"

using namespace chart;

ChartJs::ChartJs()
{

}

ChartJsColor::ChartJsColor(const var::String & hex_code){

	if( hex_code.length() != 6 ){
		return;
	}

	m_red = hex_code
			.create_sub_string(
				var::String::Position(0),
				var::String::Length(2))
			.to_unsigned_long(var::String::base_16);

	m_green = hex_code
			.create_sub_string(
				var::String::Position(2),
				var::String::Length(2))
			.to_unsigned_long(var::String::base_16);

	m_blue = hex_code
			.create_sub_string(
				var::String::Position(4),
				var::String::Length(2))
			.to_unsigned_long(var::String::base_16);
}


var::String ChartJs::convert_type_to_string(enum type value){
	switch(value){
		case type_line: return "line";
		case type_bar: return "bar";
		case type_doughnut: return "doughnut";
		case type_pie: return "pie";
		case type_radar: return "radar";
		case type_scatter: return "scatter";
	}
	return "line";
}
