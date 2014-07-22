/*
 *
 *
 *  Created on: Jun 25, 2014
 *      Author: nickhathaway <nickjhathaway@gmail.com>
 */
#include "compro/utils/numUtils.hpp"

namespace compro {
double roundDecPlaces(double num, int decPlaces) {
  double rounder = pow(10, decPlaces);
  return (floor(num * rounder + 0.5) / rounder);
}

unsigned uAbsdiff( unsigned a, unsigned b ){
      unsigned n= (unsigned)(
         (long long)((unsigned long long)a - (unsigned long long)b)>>32
                      ); // same n as 2nd example
      unsigned result = a-b;
      return (result^n)-n; // 'result' if n = 0; '-result' if n = 0xFFFFFFFF
}

} // compro
