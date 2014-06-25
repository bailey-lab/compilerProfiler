#pragma once
/*
 * 
 *

 *  Created on: Jun 25, 2014
 *      Author: nickhathaway <nickjhathaway@gmail.com>
 */
#include "allSystemIncludes.h"

template <typename NUM>
std::string to_string(NUM number) {
  std::stringstream tempStream;
  tempStream << number;
  return tempStream.str();
}

template <typename Container>
void sort(Container& con) {
  return std::sort(con.begin(), con.end());
}

template <typename Container, typename Compare>
void sort(Container& con, Compare comp) {
  return std::sort(con.begin(), con.end(), comp);
}

template <typename Container>
void reverse(Container& con) {
  std::reverse(con.begin(), con.end());
}
