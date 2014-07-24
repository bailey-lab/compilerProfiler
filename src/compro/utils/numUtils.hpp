#pragma once
/*
 *
 *

 *  Created on: Jun 25, 2014
 *      Author: nickhathaway <nickjhathaway@gmail.com>
 */

#include "compro/common.h"

namespace compro {
inline unsigned uAbsdiff( unsigned a, unsigned b ){
      unsigned n= (unsigned)(
         (long long)((unsigned long long)a - (unsigned long long)b)>>32
                      ); // same n as 2nd example
      unsigned result = a-b;
      return (result^n)-n; // 'result' if n = 0; '-result' if n = 0xFFFFFFFF
}
inline double roundDecPlaces(double num, int decPlaces) {
  double rounder = pow(10, decPlaces);
  return (floor(num * rounder + 0.5) / rounder);
}



inline bool isPrimeInline (uint64_t num){
	if(num % 2 == 0){
		if(num == 2){
			return true;
		}else{
			return false;
		}
	} else {
		uint64_t stop = std::sqrt(num);
		for(uint64_t i = 3; i <= stop; i+=2){
			if(num % i == 0){
				return false;
			}
		}
	}
	return true;
}

std::vector<double> getRange(double start, double stop, uint32_t num);


}
