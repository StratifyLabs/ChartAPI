#ifndef CHARTJS_HPP
#define CHARTJS_HPP

#include <sapi/var/Json.hpp>

namespace chart {

class ChartJsColor {
public:
	ChartJsColor(
			){

	}

	ChartJsColor(const var::String & hex_code);

	ChartJsColor& set_red(u8 value){
		m_red = value;
		return *this;
	}

	ChartJsColor& set_green(u8 value){
		m_green = value;
		return *this;
	}

	ChartJsColor& set_blue(u8 value){
		m_blue = value;
		return *this;
	}

	ChartJsColor& set_alpha(u8 value){
		m_alpha = value;
		return *this;
	}

	var::String to_string() const {
		return var::String().format(
					"rgba(%d,%d,%d,%0.2f)",
					m_red,
					m_green,
					m_blue,
					m_alpha * 1.0f / 255
					);
	}

	static ChartJsColor create_red(u8 value = 255){
		return ChartJsColor()
				.set_red(value)
				.set_green(0)
				.set_blue(0)
				.set_alpha(255);
	}

	static ChartJsColor create_green(u8 value = 255){
		return ChartJsColor()
				.set_red(0)
				.set_green(value)
				.set_blue(0)
				.set_alpha(255);
	}

	static ChartJsColor create_blue(u8 value = 255){
		return ChartJsColor()
				.set_red(0)
				.set_green(0)
				.set_blue(value)
				.set_alpha(255);
	}

	static ChartJsColor create_transparent(){
		return ChartJsColor()
				.set_alpha(0);
	}

	static ChartJsColor create_white(){
		return ChartJsColor();
	}

	static ChartJsColor create_black(){
		return ChartJsColor()
				.set_red(0)
				.set_green(0)
				.set_blue(0)
				.set_alpha(255);
	}

	static ChartJsColor create_gray(u8 value = 128){
		return ChartJsColor()
				.set_red(value)
				.set_green(value)
				.set_blue(value)
				.set_alpha(255);
	}

	static var::Vector<ChartJsColor> create_standard_palette(){
		var::Vector<ChartJsColor> result = {
			ChartJsColor("9BA4B2"),
			ChartJsColor("F1D651"),
			ChartJsColor("9F5D6B"),
			ChartJsColor("343D68"),
			ChartJsColor("94BFDE"),
			ChartJsColor("AA3C55"),
			ChartJsColor("255C35"),
			ChartJsColor("AA9E9B"),
			ChartJsColor("38323D"),
			ChartJsColor("84817E"),
			ChartJsColor("A19B9A"),
			ChartJsColor("88BCAA"),
			ChartJsColor("787FCC"),
			ChartJsColor("2F6192"),
			ChartJsColor("A2795B"),
			ChartJsColor("2C1B2E")
		};
		return result;
	}

private:
	u8 m_red = 0xff;
	u8 m_green = 0xff;
	u8 m_blue = 0xff;
	u8 m_alpha = 0xff;
};

class ChartJsDataSet {
public:
	ChartJsDataSet(){}

	enum type {
		type_string,
		type_real,
		type_object
	};

	ChartJsDataSet& set_property(
			const var::String & key,
			const var::JsonValue & value
			){
		m_properties.insert(key, value);
		return *this;
	}

	ChartJsDataSet& append(const var::JsonValue & value){
		data().push_back(value);
		return *this;
	}

	var::JsonObject to_object() const {
		var::JsonObject result;
		result.copy(m_properties);
		var::JsonArray data_array;
		for(const auto & data: m_data){
			data_array.append(data);
		}
		result.insert(
					"data",
					data_array
					);

		return result;
	}

	var::Vector<var::JsonValue>& data(){ return m_data; }
	const var::Vector<var::JsonValue>& data() const { return m_data; }


private:
	enum type m_type = type_string;
	var::JsonObject m_properties;
	var::Vector<var::JsonValue> m_data;
};

class ChartJsData {
public:
	ChartJsData(){}

	ChartJsData& append(const ChartJsDataSet& data_set){
		m_dataset_list.push_back(data_set);
		return *this;
	}

	var::JsonObject to_object() const {
		var::JsonObject result;
		result.insert(
					"labels",
					var::JsonArray(m_label_list)
					);
		var::JsonArray dataset_array;
		for(const auto & dataset: m_dataset_list){
			dataset_array.append(dataset.to_object());
		}
		result.insert(
					"datasets",
					dataset_array
					);
		return result;
	}

	var::StringList& label_list(){ return m_label_list; }
	const var::StringList& label_list() const { return m_label_list; }


private:
	var::StringList m_label_list;
	var::Vector<ChartJsDataSet> m_dataset_list;
};

class ChartJsOptions {
public:
	ChartJsOptions(){}
	var::JsonObject to_object() const {
		var::JsonObject result;
		result.copy(m_value, var::JsonValue::IsDeep(true));
		return result;
	}

	ChartJsOptions& set_property(
			const var::String & key,
			const var::JsonValue & value
			){
		m_value.insert(key, value);
		return *this;
	}


	ChartJsOptions& set_y_axes(
			const var::JsonArray & array
			){
		var::JsonObject scales_object = m_value.at("scales").to_object();
		scales_object.insert("yAxes", array);
		m_value.insert("scales", scales_object);
		return *this;
	}

	ChartJsOptions& set_x_axes(
			const var::JsonArray & array
			){
		var::JsonObject scales_object = m_value.at("scales").to_object();
		scales_object.insert("xAxes", array);
		m_value.insert("scales", scales_object);
		return *this;
	}

	ChartJsOptions& set_y_axis(
			const var::JsonObject & object
			){
		var::JsonArray array;
		array.append(object);
		set_y_axes(array);
		return *this;
	}

	ChartJsOptions& set_x_axis(
			const var::JsonObject & object
			){
		var::JsonArray array;
		array.append(object);
		set_x_axes(array);
		return *this;
	}

	static var::JsonObject create_title(const var::String & text){
		var::JsonObject result;
		result.insert("display", var::JsonTrue());
		result.insert("text", var::JsonString(text));
		return result;
	}

	static var::JsonObject create_axis(const var::String & type){
		var::JsonObject result;
		result.insert("display", var::JsonTrue());
		if( type.is_empty() == false ){
			result.insert("type", var::JsonString(type));
		}
		return result;
	}

	static var::JsonObject create_scale_label(const var::String & label){
		var::JsonObject result;
		result.insert("display", var::JsonTrue());
		result.insert("labelString", var::JsonString(label));
		return result;
	}

	static var::JsonObject create_legend(bool is_display){
		var::JsonObject result;
		if( is_display ){
			result.insert("display", var::JsonTrue());
		} else {
			result.insert("display", var::JsonFalse());
		}
		return result;
	}

	static var::JsonObject create_time(const var::String & unit, const var::String & format){
		var::JsonObject result;
		result.insert("unit", var::JsonString(unit));
		var::JsonObject display_format;
		display_format.insert(unit, var::JsonString(format));
		result.insert("displayFormats", display_format);
		return result;
	}




private:
	var::JsonObject m_value;


};

class ChartJs {
public:
	ChartJs();

	enum type {
		type_line,
		type_bar,
		type_doughnut,
		type_pie,
		type_radar,
		type_scatter
	};

	ChartJs& set_type(enum type value){
		m_type = value;
		return *this;
	}

	var::JsonObject to_object() const {
		var::JsonObject result;
		result.insert("type",
									var::JsonString( convert_type_to_string(m_type) )
									);

		result.insert(
					"options",
					options().to_object()
					);

		result.insert(
					"data",
					data().to_object()
					);

		return result;
	}

	ChartJsData& data(){ return m_data; }
	const ChartJsData& data() const { return m_data; }

	ChartJsOptions& options(){ return m_options; }
	const ChartJsOptions& options() const { return m_options; }

private:
	enum type m_type;
	ChartJsData m_data;
	ChartJsOptions m_options;

	static var::String convert_type_to_string(enum type value);

};

}

#endif // CHARTJS_HPP
