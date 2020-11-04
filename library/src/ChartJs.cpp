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
      String::Base::hexadecimal);

  m_green = hex_code
                .get_substring(
                    StringView::GetSubstring().set_position(2).set_length(2))
                .to_unsigned_long(StringView::Base::hexadecimal);

  m_blue = hex_code
               .get_substring(
                   StringView::GetSubstring().set_position(4).set_length(2))
               .to_unsigned_long(StringView::Base::hexadecimal);
}

var::StringView ChartJs::convert_type_to_string(Type value) {
  switch (value) {
  case Type::line:
    return "line";
  case Type::bar:
    return "bar";
  case Type::doughnut:
    return "doughnut";
  case Type::pie:
    return "pie";
  case Type::radar:
    return "radar";
  case Type::scatter:
    return "scatter";
  }
  return "line";
}

var::StringView ChartJsDataSet::get_point_style_string(PointStyle value) {
  switch (value) {
  case PointStyle::circle:
    return "circle";
  case PointStyle::cross:
    return "cross";
  case PointStyle::cross_rot:
    return "crossRot";
  case PointStyle::dash:
    return "dash";
  case PointStyle::line:
    return "line";
  case PointStyle::rectangle:
    return "rect";
  case PointStyle::rectangle_rounded:
    return "rectRounded";
  case PointStyle::rectangle_rot:
    return "rectRot";
  case PointStyle::star:
    return "star";
  case PointStyle::triangle:
    return "triangle";
  }
  return "circle";
}

var::StringView ChartJsDataSet::get_cubic_interpolation_mode_string(
    CubicInterpolationMode value) {
  switch (value) {
  case CubicInterpolationMode::default_:
    return "default";
  case CubicInterpolationMode::monotone:
    return "monotone";
  }
  return "default";
}

var::StringView ChartJsDataSet::get_stepped_line_string(SteppedLine value) {
  switch (value) {
  case SteppedLine::yes:
    return "true";
  case SteppedLine::no:
    return "false";
  case SteppedLine::before:
    return "before";
  case SteppedLine::after:
    return "after";
  case SteppedLine::middle:
    return "middle";
  }
  return "true";
}

var::StringView
ChartJsDataSet::get_border_join_style_string(BorderJoinStyle value) {
  switch (value) {
  case BorderJoinStyle::undefined:
    return "undefined";
  case BorderJoinStyle::bevel:
    return "bevel";
  case BorderJoinStyle::round:
    return "round";
  case BorderJoinStyle::miter:
    return "miter";
  }
  return "miter";
}

var::StringView
ChartJsDataSet::get_border_cap_style_string(BorderCapStyle value) {
  switch (value) {
  case BorderCapStyle::butt:
    return "butt";
  case BorderCapStyle::round:
    return "round";
  case BorderCapStyle::square:
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
  result.insert_bool("fill", is_fill());

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

  if (hover_border_join_style() != BorderJoinStyle::undefined) {
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

  result.insert_bool("showLine", is_show_line());
  result.insert_bool("spanGaps", is_span_gaps());

  {
    const var::StringView stepped_line_value =
        get_stepped_line_string(stepped_line());
    if (stepped_line_value == "true") {
      result.insert_bool("steppedLine", true);
    } else if (stepped_line_value == "false") {
      result.insert_bool("steppedLine", false);
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
