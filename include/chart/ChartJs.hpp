#ifndef CHARTAPI_CHART_CHARTJS_HPP
#define CHARTAPI_CHART_CHARTJS_HPP

#include <cmath>

#include <api/api.hpp>
#include <json/Json.hpp>

namespace chart {

class ChartJsFlags {
public:
  enum positions {
    position_top,
    position_left,
    position_bottom,
    position_right
  };

  static var::StringView position_to_string(enum positions value) {
    switch (value) {
    case position_top:
      return "top";
    case position_left:
      return "left";
    case position_bottom:
      return "bottom";
    case position_right:
      return "right";
    }
    return "left";
  }

  enum aligns { align_start, align_center, align_end };

  static var::StringView align_to_string(enum aligns value) {
    switch (value) {
    case align_start:
      return "start";
    case align_center:
      return "center";
    case align_end:
      return "end";
    }
    return "left";
  }
};

class ChartJsColor {
public:
  ChartJsColor() {}

  bool is_valid() const { return m_is_valid; }

  ChartJsColor(var::StringView hex_code);

  ChartJsColor &set_red(u8 value) {
    m_red = value;
    m_is_valid = true;
    return *this;
  }

  ChartJsColor &set_green(u8 value) {
    m_green = value;
    m_is_valid = true;
    return *this;
  }

  ChartJsColor &set_blue(u8 value) {
    m_blue = value;
    m_is_valid = true;
    return *this;
  }

  ChartJsColor &set_alpha(u8 value) {
    m_alpha = value;
    m_is_valid = true;
    return *this;
  }

  var::String to_string() const {
    return var::String().format("rgba(%d,%d,%d,%0.2f)", m_red, m_green, m_blue,
                                m_alpha * 1.0f / 255);
  }

  static ChartJsColor create_red(u8 value = 255) {
    return ChartJsColor().set_red(value).set_green(0).set_blue(0).set_alpha(
        255);
  }

  static ChartJsColor create_green(u8 value = 255) {
    return ChartJsColor().set_red(0).set_green(value).set_blue(0).set_alpha(
        255);
  }

  static ChartJsColor create_blue(u8 value = 255) {
    return ChartJsColor().set_red(0).set_green(0).set_blue(value).set_alpha(
        255);
  }

  static ChartJsColor create_transparent() {
    return ChartJsColor().set_valid().set_alpha(0);
  }

  static ChartJsColor create_white() { return ChartJsColor().set_valid(); }

  static ChartJsColor create_black() {
    return ChartJsColor()
        .set_red(0)
        .set_green(0)
        .set_blue(0)
        .set_valid()
        .set_alpha(255);
  }

  static ChartJsColor create_gray(u8 value = 128) {
    return ChartJsColor()
        .set_red(value)
        .set_green(value)
        .set_blue(value)
        .set_valid()
        .set_alpha(255);
  }

  static ChartJsColor get_standard(u32 idx) {
    var::Vector<ChartJsColor> palette = create_standard_palette();
    return palette.at(idx % palette.count());
  }

  static var::Vector<ChartJsColor> create_standard_palette() {
    var::Vector<ChartJsColor> result = {
        ChartJsColor("9BA4B2"), ChartJsColor("F1D651"), ChartJsColor("9F5D6B"),
        ChartJsColor("343D68"), ChartJsColor("94BFDE"), ChartJsColor("AA3C55"),
        ChartJsColor("255C35"), ChartJsColor("AA9E9B"), ChartJsColor("38323D"),
        ChartJsColor("84817E"), ChartJsColor("A19B9A"), ChartJsColor("88BCAA"),
        ChartJsColor("787FCC"), ChartJsColor("2F6192"), ChartJsColor("A2795B"),
        ChartJsColor("2C1B2E")};
    return result;
  }

private:
  ChartJsColor &set_valid(bool value = true) {
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
  json::JsonObject to_object() const {
    return json::JsonObject()
        .insert("x", json::JsonString(x()))
        .insert("y", json::JsonString(y()));
  }

private:
  API_AC(ChartJsStringDataPoint, var::String, x);
  API_AC(ChartJsStringDataPoint, var::String, y);
};

class ChartJsIntegerDataPoint {
public:
  json::JsonObject to_object() const {
    return json::JsonObject()
        .insert("x", json::JsonInteger(x()))
        .insert("y", json::JsonInteger(y()));
  }

private:
  API_AF(ChartJsIntegerDataPoint, int, x, 0);
  API_AF(ChartJsIntegerDataPoint, int, y, 0);
};

class ChartJsRealDataPoint {
public:
  json::JsonObject to_object() const {
    return json::JsonObject()
        .insert("x", json::JsonReal(x()))
        .insert("y", json::JsonReal(y()));
  }

private:
  API_AF(ChartJsRealDataPoint, float, x, 0.0f);
  API_AF(ChartJsRealDataPoint, float, y, 0.0f);
};

class ChartJsDataSet {
public:
  ChartJsDataSet() {}

  enum types { type_string, type_real, type_object };

  enum point_styles {
    point_style_circle,
    point_style_cross,
    point_style_cross_rot,
    point_style_dash,
    point_style_line,
    point_style_rectangle,
    point_style_rectangle_rounded,
    point_style_rectangle_rot,
    point_style_star,
    point_style_triangle
  };

  enum cubic_interpolation_modes {
    cubic_interpolation_mode_default,
    cubic_interpolation_mode_monotone
  };

  enum stepped_lines {
    stepped_line_true,
    stepped_line_false,
    stepped_line_before,
    stepped_line_after,
    stepped_line_middle
  };

  enum border_join_styles {
    border_join_style_undefined,
    border_join_style_bevel,
    border_join_style_round,
    border_join_style_miter
  };

  enum border_cap_styles {
    border_cap_style_butt,
    border_cap_style_round,
    border_cap_style_square
  };

  ChartJsDataSet &append(const json::JsonValue &value) {
    data().push_back(value);
    return *this;
  }

  json::JsonObject to_object() const;

  var::Vector<json::JsonValue> &data() { return m_data; }
  const var::Vector<json::JsonValue> &data() const { return m_data; }

private:
  API_AC(ChartJsDataSet, ChartJsColor, background_color);
  API_AF(ChartJsDataSet, enum border_cap_styles, border_cap_style,
         border_cap_style_butt);
  API_AC(ChartJsDataSet, ChartJsColor, border_color);
  API_AC(ChartJsDataSet, var::Vector<s32>, border_dash_list);
  API_AF(ChartJsDataSet, float, border_dash_offset, 0.0f);
  API_AF(ChartJsDataSet, enum border_join_styles, border_join_style,
         border_join_style_miter);
  API_AF(ChartJsDataSet, float, border_width, 3.0f);
  API_AF(ChartJsDataSet, enum cubic_interpolation_modes,
         cubic_interpolation_mode, cubic_interpolation_mode_default);
  API_AB(ChartJsDataSet, fill, true);
  // clip
  API_AC(ChartJsDataSet, ChartJsColor, hover_background_color);
  API_AF(ChartJsDataSet, enum border_cap_styles, hover_border_cap_style,
         border_cap_style_butt);
  API_AC(ChartJsDataSet, ChartJsColor, hover_border_color);
  API_AC(ChartJsDataSet, var::Vector<s32>, hover_border_dash_list);
  API_AF(ChartJsDataSet, float, hover_border_dash_offset, 0.0f);
  API_AF(ChartJsDataSet, enum border_join_styles, hover_border_join_style,
         border_join_style_undefined);
  API_AF(ChartJsDataSet, float, hover_border_width, HUGE_VALF);
  API_AC(ChartJsDataSet, var::String, label);
  API_AF(ChartJsDataSet, float, line_tension, 0.4f);
  API_AF(ChartJsDataSet, int, order, 0);
  API_AC(ChartJsDataSet, ChartJsColor, point_background_color);
  API_AC(ChartJsDataSet, ChartJsColor, point_border_color);
  API_AF(ChartJsDataSet, float, point_border_width, 1.0f);
  API_AF(ChartJsDataSet, float, point_hit_radius, 1.0f);
  API_AC(ChartJsDataSet, ChartJsColor, point_hover_background_color);
  API_AC(ChartJsDataSet, ChartJsColor, point_hover_border_color);
  API_AF(ChartJsDataSet, float, point_hover_border_width, 1.0f);
  API_AF(ChartJsDataSet, float, point_hover_radius, 4.0f);
  API_AF(ChartJsDataSet, float, point_radius, 3.0f);
  API_AF(ChartJsDataSet, float, point_rotation, 0.0f);
  API_AF(ChartJsDataSet, enum point_styles, point_style, point_style_circle);
  API_AB(ChartJsDataSet, show_line, true);
  API_AB(ChartJsDataSet, span_gaps, true);
  API_AF(ChartJsDataSet, enum stepped_lines, stepped_line, stepped_line_false);
  API_AC(ChartJsDataSet, var::String, x_axis_id);
  API_AC(ChartJsDataSet, var::String, y_axis_id);

  enum types m_type = type_string;
  var::Vector<json::JsonValue> m_data;

  static var::StringView get_point_style_string(enum point_styles value);
  static var::StringView
  get_cubic_interpolation_mode_string(enum cubic_interpolation_modes value);
  static var::StringView get_stepped_line_string(enum stepped_lines value);
  static var::StringView
  get_border_join_style_string(enum border_join_styles value);
  static var::StringView
  get_border_cap_style_string(enum border_cap_styles value);
};

class ChartJsData {
public:
  ChartJsData() {}

  ChartJsData &append(const ChartJsDataSet &data_set) {
    m_dataset_list.push_back(data_set);
    return *this;
  }

  json::JsonObject to_object() const {
    json::JsonObject result;
    result.insert("labels", json::JsonArray(m_label_list));
    json::JsonArray dataset_array;
    for (const auto &dataset : m_dataset_list) {
      dataset_array.append(dataset.to_object());
    }
    result.insert("datasets", dataset_array);
    return result;
  }

  var::StringList &label_list() { return m_label_list; }
  const var::StringList &label_list() const { return m_label_list; }

private:
  var::StringList m_label_list;
  var::Vector<ChartJsDataSet> m_dataset_list;
};

class ChartJsAxisTicks {
public:
  bool is_valid() const { return minimum() != maximum(); }

  json::JsonObject to_object() const {
    if (is_valid()) {
      return json::JsonObject()
          .insert("stepSize", json::JsonReal(step_size()))
          .insert("min", json::JsonReal(minimum()))
          .insert("max", json::JsonInteger(maximum()));
    }

    return json::JsonObject();
  }

private:
  API_AF(ChartJsAxisTicks, float, minimum, 0);
  API_AF(ChartJsAxisTicks, float, maximum, 0);
  API_AF(ChartJsAxisTicks, float, step_size, 0);
};

class ChartJsScaleLabel {
public:
  bool is_valid() const { return !label().is_empty(); }

  json::JsonObject to_object() const {
    return json::JsonObject()
        .insert("display", is_display())
        .insert("labelString", json::JsonString(label()));
  }

private:
  API_AB(ChartJsScaleLabel, display, true);
  API_AC(ChartJsScaleLabel, var::String, label);
};

class ChartJsAxis {
public:
  enum types { type_linear, type_logarithmic, type_category, type_time };

  json::JsonObject to_object() const {
    json::JsonObject result;
    result.insert("display", is_display())
        .insert("type", json::JsonString(type_to_string(type())))
        .insert("weight", json::JsonInteger(weight()));

    if (ticks().is_valid()) {
      result.insert("ticks", ticks().to_object());
    }

    if (scale_label().is_valid()) {
      result.insert("scaleLabel", scale_label().to_object());
    }

    if (is_stacked()) {
      result.insert("stacked", is_stacked());
    }

    return result;
  }

private:
  API_AB(ChartJsAxis, display, true);
  API_AF(ChartJsAxis, int, weight, 0);
  API_AC(ChartJsAxis, ChartJsAxisTicks, ticks);
  API_AC(ChartJsAxis, ChartJsScaleLabel, scale_label);
  API_AC(ChartJsAxis, var::String, id);
  API_AB(ChartJsAxis, stacked, false);
  API_AC(ChartJsAxis, enum types, type);

  static var::StringView type_to_string(enum types value) {
    switch (value) {
    case type_linear:
      return "linear";
    case type_logarithmic:
      return "logarithmic";
    case type_category:
      return "category";
    case type_time:
      return "time";
    }
    return "linear";
  }
};

class ChartJsScales {
public:
  ChartJsScales &append_x_axis(const ChartJsAxis &axis) {
    x_axes().push_back(axis);
    return *this;
  }

  ChartJsScales &append_y_axis(const ChartJsAxis &axis) {
    y_axes().push_back(axis);
    return *this;
  }

  json::JsonObject to_object() const {
    json::JsonObject result;

    if (x_axes().count() > 0) {
      result.insert("xAxes", axes_to_array(x_axes()));
    }

    if (y_axes().count() > 0) {
      result.insert("yAxes", axes_to_array(y_axes()));
    }

    return result;
  }

private:
  API_AC(ChartJsScales, var::Vector<ChartJsAxis>, x_axes);
  API_AC(ChartJsScales, var::Vector<ChartJsAxis>, y_axes);

  json::JsonArray axes_to_array(const var::Vector<ChartJsAxis> &axes) const {
    json::JsonArray result;
    for (const auto &axis : axes) {
      result.append(axis.to_object());
    }
    return result;
  }
};

class ChartJsTitle : public ChartJsFlags {
public:
  json::JsonObject to_object() const {
    return json::JsonObject()
        .insert("display", is_display())
        .insert("fontSize", json::JsonInteger(font_size()))
        .insert("position", json::JsonString(position_to_string(position())))
        .insert("text", json::JsonString(text()));
  }

private:
  API_AB(ChartJsTitle, display, true);
  API_AF(ChartJsTitle, enum positions, position, position_top);
  API_AF(ChartJsTitle, int, font_size, 12);
  API_AC(ChartJsTitle, var::String, text);
};

class ChartJsLegend : public ChartJsFlags {
public:
  json::JsonObject to_object() const {
    return json::JsonObject()
        .insert("align", json::JsonString(align_to_string(align())))
        .insert("position", json::JsonString(position_to_string(position())))
        .insert("display", is_display())
        .insert("rtl", is_right_to_left())
        .insert("reverse", is_reverse())
        .insert("fullWidth", is_full_width());
  }

private:
  API_AB(ChartJsLegend, display, true);
  API_AB(ChartJsLegend, full_width, true);
  API_AB(ChartJsLegend, reverse, false);
  API_AB(ChartJsLegend, right_to_left, false);
  API_AF(ChartJsLegend, enum positions, position, position_top);
  API_AF(ChartJsLegend, enum aligns, align, align_center);
};

class ChartJsOptions {
public:
  ChartJsOptions() {}
  json::JsonObject to_object() const {
    json::JsonObject result;
    result.copy(m_value, json::JsonValue::IsDeepCopy::yes);
    return result;
  }

  ChartJsOptions &set_property(const char *key, const json::JsonValue &value) {
    m_value.insert(key, value);
    return *this;
  }

  ChartJsOptions &set_scales(const ChartJsScales &value) {
    m_value.insert("scales", value.to_object());
    return *this;
  }

  ChartJsOptions &set_legend(const ChartJsLegend &value) {
    m_value.insert("legend", value.to_object());
    return *this;
  }

  ChartJsOptions &set_title(const ChartJsTitle &value) {
    m_value.insert("title", value.to_object());
    return *this;
  }

  static ChartJsOptions create_time(const char *unit, var::StringView format) {
    ChartJsOptions result;
    result.set_property("unit", json::JsonString(unit));
    ChartJsOptions display_format;
    display_format.set_property(unit, json::JsonString(format));
    result.set_property("displayFormats", display_format.object());
    return result;
  }

  const json::JsonObject &object() { return m_value; }

private:
  json::JsonObject m_value;
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
    type_scatter,
  };

  json::JsonObject to_object() const {
    json::JsonObject result;
    result.insert("type", json::JsonString(convert_type_to_string(m_type)));

    result.insert("options", options().to_object());

    result.insert("data", data().to_object());

    return result;
  }

  ChartJsData &data() { return m_data; }
  const ChartJsData &data() const { return m_data; }

  ChartJsOptions &options() { return m_options; }
  const ChartJsOptions &options() const { return m_options; }

private:
  API_AF(ChartJs, enum types, type, type_line);

  ChartJsData m_data;
  ChartJsOptions m_options;

  static var::StringView convert_type_to_string(enum types value);
};

} // namespace chart

#endif // CHARTAPI_CHART_CHARTJS_HPP
