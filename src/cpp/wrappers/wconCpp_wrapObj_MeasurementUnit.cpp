#include "wconCpp_wrapObj_MeasurementUnit.h"

WrapMeasurementUnit& WrapMeasurementUnit::create(string unitStr) {
  WrapMeasurementUnit *newUnit = new WrapMeasurementUnit();
  return *newUnit;
}

double WrapMeasurementUnit::to_canon(double val) {
  return val;
}

double WrapMeasurementUnit::from_canon(double val) {
  return val;
}

string& WrapMeasurementUnit::unit_string() {
  string *newString = new string("unit_string");
  return *newString;
}

string& WrapMeasurementUnit::canonical_unit_string() {
  string *newString = new string("canonical_string");
  return *newString;
}
