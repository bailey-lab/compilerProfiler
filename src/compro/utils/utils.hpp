#pragma once
/*
 *
 *

 *  Created on: Jun 25, 2014
 *      Author: nickhathaway <nickjhathaway@gmail.com>
 */

#include "compro/common.h"

namespace compro {

std::string convertBoolToString(bool convert);

bool fexists(const std::string &filename);

void openTextFile(std::ofstream &file, std::string filename, bool overWrite,
                  bool append, bool exitOnFailure);
std::map<std::string, std::pair<std::string, bool>> getFiles(
    const std::string &directoryName, const std::string &contains,
    const std::string &filesOrDirectories, bool specific, bool recursive);
std::map<std::string, std::pair<std::string, bool>> getFiles(
    const std::string &directoryName, const VecStr &contains,
    const std::string &filesOrDirectories, bool specific, bool recursive);
std::map<std::string, std::pair<std::string, bool>> listFilesInDir(
    const std::string &directoryName, bool recursive);
int getdir(const std::string &dir,
           std::map<std::string, std::pair<std::string, bool>> &files);
}
