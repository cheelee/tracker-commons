#ifndef __WCONCPP_WRAPOBJ_MEASUREMENTUNIT_H_
#define __WCONCPP_WRAPOBJ_MEASUREMENTUNIT_H_

#include <string>
using namespace std;

class WrapMeasurementUnit {
 public:
  static WrapMeasurementUnit& create(string unitStr);
  double to_canon(double val);
  double from_canon(double val);
  string& unit_string();
  string& canonical_unit_string();
 private:
  // named constructor idiom
  WrapMeasurementUnit() {};
};

#endif /*  __WCONCPP_WRAPOBJ_MEASUREMENTUNIT_H_ */
