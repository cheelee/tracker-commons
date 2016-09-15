#ifndef __WCONCPP_WRAPOBJ_WCONWORMS_H_
#define __WCONCPP_WRAPOBJ_WCONWORMS_H_

#include "wconCpp_wrapObj_MeasurementUnit.h"

#include <string>
#include <list>
using namespace std;

class WrapWCONWorms {
 public:
  static WrapWCONWorms& load_from_file(string wconpath);
  void save_to_file(string output_path, int pretty_print, int compressed);
  WrapWCONWorms& to_canon();
  WrapWCONWorms& operator+(const WrapWCONWorms& worms);
  bool operator==(const WrapWCONWorms& worms);
  WrapMeasurementUnit& units();
  void *data(); // currently just use anonymous pointers
  long num_worms();
  // According to the python implementation, a list of list of ids (string)
  list<list<string>> *worm_ids(); 
  void *data_as_odict(); // currently just use anonymous pointers
 private:
  // Named constructor idiom
  WrapWCONWorms() {};
};

#endif /* __WCONCPP_WRAPOBJ_WCONWORMS_H_ */
