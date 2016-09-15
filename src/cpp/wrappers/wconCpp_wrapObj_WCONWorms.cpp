#include "wconCpp_wrapObj_WCONWorms.h"

// C++ Wrapper object for the WCONWorms Python object

WrapWCONWorms& WrapWCONWorms::load_from_file(string wconpath) {
  WrapWCONWorms *newWorm = new WrapWCONWorms();
  return *newWorm;
}

void WrapWCONWorms::save_to_file(string output_path, int pretty_print, int compressed) {
  
}

WrapWCONWorms& WrapWCONWorms::to_canon() {
  return *this;
}

WrapWCONWorms& WrapWCONWorms::operator+(const WrapWCONWorms& worms) {
  return *this;
}

bool WrapWCONWorms::operator==(const WrapWCONWorms& worms) {
  return true;
}

WrapMeasurementUnit& WrapWCONWorms::units() {
  return WrapMeasurementUnit::create("mm");
}

void* WrapWCONWorms::data() {
  return NULL;
}

long WrapWCONWorms::num_worms() {
  return 0;
}

list<list<string>>* WrapWCONWorms::worm_ids() {
  return NULL;
}

void* WrapWCONWorms::data_as_odict() {
  return NULL;
}
