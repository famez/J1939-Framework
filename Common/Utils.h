/*
 * Utils.h
 *
 *  Created on: Mar 11, 2018
 *      Author: famez
 */

#ifndef UTILS_H_
#define UTILS_H_

#include <iostream>
#include <time.h>

#include "Types.h"

#define MIN(a,b) ((a)<(b)?(a):(b))
#define MAX(a,b) ((a)>(b)?(a):(b))

namespace std {
  template <typename _CharT, typename _Traits>
  inline basic_ostream<_CharT, _Traits> &
  tab(basic_ostream<_CharT, _Traits> &__os) {
    return __os.put(__os.widen('\t'));
  }
}


u32 getElapsedMillis(timespec* start, timespec* end);



#endif /* UTILS_H_ */
