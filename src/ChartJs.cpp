#include "chart/ChartJs.hpp"

using namespace chart;
using namespace var;

ChartJs::ChartJs() {}

ChartJsColor::ChartJsColor(StringView hex_code) {

  if (hex_code.length() != 6) {
    return;
  }

  set_valid();

  m_red = hex_code.get_substring_with_length(2).to_unsigned_long(
      String::Base::hexidecimal);

  m_green = hex_code
                .get_substring(
                    StringView::GetSubstring().set_position(2).set_length(2))
                .to_unsigned_long(StringView::Base::hexidecimal);

  m_blue = hex_code
               .get_substring(
                   StringView::GetSubstring().set_position(4).set_length(2))
               .to_unsigned_long(StringView::Base::hexidecimal);
}

var::StringView ChartJs::convert_type_to_string(enum types value) {
  switch (value) {
  case type_line:
    return "line";
  case type_bar:
    return "bar";
  case type_doughnut:
    return "doughnut";
  case type_pie:
    return "pie";
  case type_radar:
    return "radar";
  case type_scatter:
    return "scatter";
  }
  return "line";
}

var::StringView
ChartJsDataSet::get_point_style_string(enum point_styles value) {
  switch (value) {
  case point_style_circle:
    return "circle";
  case point_style_cross:
    return "cross";
  case point_style_cross_rot:
    return "crossRot";
  case point_style_dash:
    return "dash";
  case point_style_line:
    return "line";
  case point_style_rectangle:
    return "rect";
  case point_style_rectangle_rounded:
    return "rectRounded";
  case point_style_rectangle_rot:
    return "rectRot";
  case point_style_star:
    return "star";
  case point_style_triangle:
    return "triangle";
  }
  return "circle";
}

var::StringView ChartJsDataSet::get_cubic_interpolation_mode_string(
    enum cubic_interpolation_modes value) {
  switch (value) {
  case cubic_interpolation_mode_default:
    return "default";
  case cubic_interpolation_mode_monotone:
    return "monotone";
  }
  return "default";
}

var::StringView
ChartJsDataSet::get_stepped_line_string(enum stepped_lines value) {
  switch (value) {
  case stepped_line_true:
    return "true";
  case stepped_line_false:
    return "false";
  case stepped_line_before:
    return "before";
  case stepped_line_after:
    return "after";
  case stepped_line_middle:
    return "middle";
  }
  return "true";
}

var::StringView
ChartJsDataSet::get_border_join_style_string(enum border_join_styles value) {
  switch (value) {
  case border_join_style_undefined:
    return "undefined";
  case border_join_style_bevel:
    return "bevel";
  case border_join_style_round:
    return "round";
  case border_join_style_miter:
    return "miter";
  }
  return "miter";
}

var::StringView
ChartJsDataSet::get_border_cap_style_string(enum border_cap_styles value) {
  switch (value) {
  case border_cap_style_butt:
    return "butt";
  case border_cap_style_round:
    return "round";
  case border_cap_style_square:
    return "round";
  }
  return "butt";
}

json::JsonObject ChartJsDataSet::to_object() const {
  json::JsonObject result;

  if (background_color().is_valid()) {
    result.insert("backgroundColor",
                  json::JsonString(background_color().to_string()));
  }

  result.insert("borderCapStyle", json::JsonString(get_border_cap_style_string(
                                      border_cap_style())));

  if (border_color().is_valid()) {
    result.insert("borderColor", json::JsonString(border_color().to_string()));
  }

  if (border_dash_list().count()) {
    result.insert("borderDash", json::JsonArray(border_dash_list()));
  }

  result.insert("borderDashOffset", json::JsonReal(border_dash_offset()));
  result.insert(
      "borderJoinStyle",
      json::JsonString(get_border_join_style_string(border_join_style())));
  result.insert("borderWidth", json::JsonReal(border_width()));
  result.insert("cubicInterpolationMode",
                json::JsonString(get_cubic_interpolation_mode_string(
                    cubic_interpolation_mode())));
  // clip
  result.insert("fill", is_fill());

  if (hover_background_color().is_valid()) {
    result.insert("hoverBackgroundColor",
                  json::JsonString(hover_background_color().to_string()));
  }
  result.insert(
      "hoverBorderCapStyle",
      json::JsonString(get_border_cap_style_string(hover_border_cap_style())));

  if (hover_border_color().is_valid()) {
    result.insert("hoverBorderColor",
                  json::JsonString(hover_border_color().to_string()));
  }

  if (hover_border_dash_list().count()) {
    result.insert("hoverBorderDash", json::JsonArray(hover_border_dash_list()));
  }

  if (hover_border_dash_offset() != HUGE_VALF) {
    result.insert("hoverBorderDashOffset",
                  json::JsonReal(hover_border_dash_offset()));
  }

  if (hover_border_join_style() != border_join_style_undefined) {
    result.insert("hoverBorderJoinStyle",
                  json::JsonString(
                      get_border_join_style_string(hover_border_join_style())));
  }

  if (hover_border_width() != HUGE_VALF) {
    result.insert("hoverBorderWidth", json::JsonReal(hover_border_width()));
  }

  if (!label().is_empty()) {
    result.insert("label", json::JsonString(label()));
  }

  result.insert("lineTension", json::JsonReal(line_tension()));
  result.insert("order", json::JsonInteger(order()));

  if (point_background_color().is_valid()) {
    result.insert("pointBackgroundColor",
                  json::JsonString(point_background_color().to_string()));
  }

  if (point_border_color().is_valid()) {
    result.insert("pointBorderColor",
                  json::JsonString(point_border_color().to_string()));
  }

  result.insert("pointBorderWidth", json::JsonReal(point_border_width()));
  result.insert("pointHitRadius", json::JsonReal(point_hit_radius()));

  if (point_hover_background_color().is_valid()) {
    result.insert("pointHoverBackgroundColor",
                  json::JsonString(point_hover_background_color().to_string()));
  }

  if (point_hover_border_color().is_valid()) {
    result.insert("pointHoverBorderColor",
                  json::JsonString(point_hover_border_color().to_string()));
  }

  result.insert("pointHoverRadius", json::JsonReal(point_hover_radius()));
  result.insert("pointRadius", json::JsonReal(point_radius()));
  result.insert("pointRotation", json::JsonReal(point_rotation()));
  result.insert("pointStyle",
                json::JsonString(get_point_style_string(point_style())));

  result.insert("showLine", is_show_line());
  result.insert("spanGaps", is_span_gaps());

  {
    const var::StringView stepped_line_value =
        get_stepped_line_string(stepped_line());
    if (stepped_line_value == "true") {
      result.insert("steppedLine", true);
    } else if (stepped_line_value == "false") {
      result.insert("steppedLine", false);
    } else {
      result.insert("steppedLine", json::JsonString(stepped_line_value));
    }
  }

  if (!x_axis_id().is_empty()) {
    result.insert("xAxisID", json::JsonString(x_axis_id()));
  }

  if (!y_axis_id().is_empty()) {
    result.insert("xAxisID", json::JsonString(y_axis_id()));
  }

  json::JsonArray data_array;
  for (const auto &data : m_data) {
    data_array.append(data);
  }
  result.insert("data", data_array);

  return result;
}
