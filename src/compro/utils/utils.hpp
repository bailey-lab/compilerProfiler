#pragma once
/*
 * 
 *

 *  Created on: Jun 25, 2014
 *      Author: nickhathaway <nickjhathaway@gmail.com>
 */

#include "compro/common.h"

namespace compro{

std::string convertBoolToString(bool convert);

bool fexists(const std::string &filename);

void openTextFile(std::ofstream &file, std::string filename,
                  bool overWrite, bool append,
                  bool exitOnFailure);

}
