#ifndef CHARTJS_HPP
#define CHARTJS_HPP

#include <sapi/var/Json.hpp>

namespace chart {

class ChartJsFlags {
public:

	enum positions {
		position_top,
		position_left,
		position_bottom,
		position_right
	};

	static var::String position_to_string(enum positions value){
		switch(value){
			case position_top: return "top";
			case position_left: return "left";
			case position_bottom: return "bottom";
			case position_right: return "right";
		}
		return "left";
	}

	enum aligns {
		align_start,
		align_center,
		align_end
	};

	static var::String align_to_string(enum aligns value){
		switch(value){
			case align_start: return "start";
			case align_center: return "center";
			case align_end: return "end";
		}
		return "left";
	}

};

class ChartJsColor {
public:
	ChartJsColor(
			){

	}

	bool is_valid() const {
		return m_is_valid;
	}

	ChartJsColor(const var::String & hex_code);

	ChartJsColor& set_red(u8 value){
		m_red = value;
		m_is_valid = true;
		return *this;
	}

	ChartJsColor& set_green(u8 value){
		m_green = value;
		m_is_valid = true;
		return *this;
	}

	ChartJsColor& set_blue(u8 value){
		m_blue = value;
		m_is_valid = true;
		return *this;
	}

	ChartJsColor& set_alpha(u8 value){
		m_alpha = value;
		m_is_valid = true;
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
				.set_valid()
				.set_alpha(0);
	}

	static ChartJsColor create_white(){
		return ChartJsColor()
				.set_valid();
	}

	static ChartJsColor create_black(){
		return ChartJsColor()
				.set_red(0)
				.set_green(0)
				.set_blue(0)
				.set_valid()
				.set_alpha(255);
	}

	static ChartJsColor create_gray(u8 value = 128){
		return ChartJsColor()
				.set_red(value)
				.set_green(value)
				.set_blue(value)
				.set_valid()
				.set_alpha(255);
	}

	static ChartJsColor get_standard(u32 idx){
		var::Vector<ChartJsColor> palette = create_standard_palette();
		return palette.at(idx % palette.count());
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

	ChartJsColor& set_valid(bool value = true){
		m_is_valid = value;
		return *this;
	}
	u8 m_red = 0xff;
	u8 m_green = 0xff;
	u8 m_blue = 0xff;
	u8 m_alpha = 0xff;
	bool m_is_valid = false;
};

class ChartJsStringDataPoint {
public:
	var::JsonObject to_object() const {
		return var::JsonObject()
				.insert("x", var::JsonString(x()))
				.insert("y", var::JsonString(y()));
	}
private:
	API_AC(ChartJsStringDataPoint,var::String,x);
	API_AC(ChartJsStringDataPoint,var::String,y);

};

class ChartJsIntegerDataPoint {
public:

	var::JsonObject to_object() const {
		return var::JsonObject()
				.insert("x", var::JsonInteger(x()))
				.insert("y", var::JsonInteger(y()));
	}
private:
	API_AF(ChartJsIntegerDataPoint,int,x,0);
	API_AF(ChartJsIntegerDataPoint,int,y,0);

};

class ChartJsRealDataPoint {
public:

	var::JsonObject to_object() const {
		return var::JsonObject()
				.insert("x", var::JsonReal(x()))
				.insert("y", var::JsonReal(y()));
	}
private:
	API_AF(ChartJsRealDataPoint,float,x,0.0f);
	API_AF(ChartJsRealDataPoint,float,y,0.0f);

};

class ChartJsDataSet {
public:
	ChartJsDataSet(){}

	enum types {
		type_string,
		type_real,
		type_object
	};


	ChartJsDataSet& append(const var::JsonValue & value){
		data().push_back(value);
		return *this;
	}

	var::JsonObject to_object() const {
		var::JsonObject result;
		result.copy(m_properties);
		if( background_color().is_valid() ){
			result.insert("backgroundColor", var::JsonString(background_color().to_string()));
		}

		if( border_color().is_valid() ){
			result.insert("borderColor", var::JsonString(border_color().to_string()));
		}

		if( !label().is_empty() ){
			result.insert("label", var::JsonString(label()));
		}


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
	API_AC(ChartJsDataSet,var::String,label);
	API_AC(ChartJsDataSet,ChartJsColor,background_color);
	API_AC(ChartJsDataSet,ChartJsColor,border_color);
	enum types m_type = type_string;
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

class ChartJsAxisTicks {
public:

	bool is_valid() const {
		return minimum() != maximum();
	}

	var::JsonObject to_object() const {
		if( is_valid() ){
			return var::JsonObject()
					.insert("stepSize", var::JsonReal(step_size()))
					.insert("min", var::JsonReal(minimum()))
					.insert("max", var::JsonInteger(maximum()));
		}

		return var::JsonObject();
	}

private:
	API_AF(ChartJsAxisTicks,float,minimum,0);
	API_AF(ChartJsAxisTicks,float,maximum,0);
	API_AF(ChartJsAxisTicks,float,step_size,0);
};

class ChartJsScaleLabel {
public:

	bool is_valid() const {
		return !label().is_empty();
	}

	var::JsonObject to_object() const {
		return var::JsonObject()
				.insert("display", is_display())
				.insert("labelString", var::JsonString(label()));
	}

private:
	API_AB(ChartJsScaleLabel,display,true);
	API_AC(ChartJsScaleLabel,var::String,label);
};

class ChartJsAxis {
public:

	enum types {
		type_linear,
		type_logarithmic,
		type_category,
		type_time
	};

	var::JsonObject to_object() const {
		var::JsonObject result;
		result.insert("display", is_display())
				.insert("type", var::JsonString(type_to_string(type())))
				.insert("weight", var::JsonInteger(weight()));

		if( ticks().is_valid() ){
			result.insert("ticks", ticks().to_object());
		}

		if( scale_label().is_valid() ){
			result.insert("scaleLabel", scale_label().to_object());
		}

		return result;
	}

private:
	API_AB(ChartJsAxis,display,true);
	API_AF(ChartJsAxis,int,weight,0);
	API_AC(ChartJsAxis,ChartJsAxisTicks,ticks);
	API_AC(ChartJsAxis,ChartJsScaleLabel,scale_label);
	API_AC(ChartJsAxis,var::String,id);
	API_AC(ChartJsAxis,enum types,type);

	static var::String type_to_string(enum types value){
		switch(value){
			case type_linear: return "linear";
			case type_logarithmic: return "logarithmic";
			case type_category: return "category";
			case type_time: return "time";
		}
		return "linear";
	}
};

class ChartJsScales {
public:

	ChartJsScales& append_x_axis(const ChartJsAxis & axis){
		x_axes().push_back(axis);
		return *this;
	}

	ChartJsScales& append_y_axis(const ChartJsAxis & axis){
		y_axes().push_back(axis);
		return *this;
	}

	var::JsonObject to_object() const {
		var::JsonObject result;

		if( x_axes().count() > 0 ){
			result.insert("xAxes", axes_to_array(x_axes()));
		}

		if( y_axes().count() > 0 ){
			result.insert("yAxes", axes_to_array(y_axes()));
		}

		return result;
	}

private:
	API_AC(ChartJsScales,var::Vector<ChartJsAxis>,x_axes);
	API_AC(ChartJsScales,var::Vector<ChartJsAxis>,y_axes);

	var::JsonArray axes_to_array(const var::Vector<ChartJsAxis> & axes) const {
		var::JsonArray result;
		for(const auto & axis: axes){
			result.append(axis.to_object());
		}
		return result;
	}
};

class ChartJsTitle: public ChartJsFlags {
public:



	var::JsonObject to_object() const {
		return var::JsonObject()
				.insert("display", is_display())
				.insert("fontSize", var::JsonInteger(font_size()))
				.insert("position", var::JsonString(position_to_string(position())))
				.insert("text", var::JsonString(text()));
	}

private:
	API_AB(ChartJsTitle,display,true);
	API_AF(ChartJsTitle,enum positions,position,position_top);
	API_AF(ChartJsTitle,int,font_size,12);
	API_AC(ChartJsTitle,var::String,text);

};

class ChartJsLegend: public ChartJsFlags {
public:

	var::JsonObject to_object() const {
		return var::JsonObject()
				.insert("align", var::JsonString(align_to_string(align())))
				.insert("position", var::JsonString(position_to_string(position())))
				.insert("display", is_display())
				.insert("rtl", is_right_to_left())
				.insert("reverse", is_reverse())
				.insert("fullWidth", is_full_width());
	}

private:
	API_AB(ChartJsLegend,display,true);
	API_AB(ChartJsLegend,full_width,true);
	API_AB(ChartJsLegend,reverse,false);
	API_AB(ChartJsLegend,right_to_left,false);
	API_AF(ChartJsLegend,enum positions,position,position_top);
	API_AF(ChartJsLegend,enum aligns,align,align_center);

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

	ChartJsOptions& set_scales(const ChartJsScales & value){
		m_value.insert("scales", value.to_object());
		return *this;
	}

	ChartJsOptions& set_legend(const ChartJsLegend & value){
		m_value.insert("legend", value.to_object());
		return *this;
	}

	ChartJsOptions& set_title(const ChartJsTitle & value){
		m_value.insert("title", value.to_object());
		return *this;
	}

	static ChartJsOptions create_time(const var::String & unit, const var::String & format){
		ChartJsOptions result;
		result.set_property("unit", var::JsonString(unit));
		ChartJsOptions display_format;
		display_format.set_property(unit, var::JsonString(format));
		result.set_property("displayFormats", display_format.object());
		return result;
	}

	const var::JsonObject & object(){
		return m_value;
	}

private:
	var::JsonObject m_value;


};

class ChartJs {
public:
	ChartJs();

	enum types {
		type_line,
		type_bar,
		type_doughnut,
		type_pie,
		type_radar,
		type_scatter
	};

	var::JsonObject to_object() const {
		var::JsonObject result;
		result.insert(
					"type",
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
	API_AF(ChartJs, enum types, type, type_line);

	ChartJsData m_data;
	ChartJsOptions m_options;

	static var::String convert_type_to_string(enum types value);

};

}

#endif // CHARTJS_HPP
