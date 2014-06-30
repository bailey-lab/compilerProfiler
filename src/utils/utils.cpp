/*
 * 
 *
 *  Created on: Jun 25, 2014
 *      Author: nickhathaway <nickjhathaway@gmail.com>
 */
#include "utils.hpp"
namespace compro{
std::string convertBoolToString(bool convert) {
  if (convert) {
    return "true";
  } else {
    return "false";
  }
}
}
