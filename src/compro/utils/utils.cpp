/*
 * 
 *
 *  Created on: Jun 25, 2014
 *      Author: nickhathaway <nickjhathaway@gmail.com>
 */
#include "compro/utils/utils.hpp"
namespace compro{
std::string convertBoolToString(bool convert) {
  if (convert) {
    return "true";
  } else {
    return "false";
  }
}
// check to see if a file exists
bool fexists(const std::string &filename) {
  /*boost::filesystem::path p(filename);
  if(boost::filesystem::exists(p)){
          std::cout << "exists" << std::endl;
  }*/
  std::ifstream ifile(filename.c_str());
  if (ifile) {
    return true;
  } else {
    return false;
  }
}

void openTextFile(std::ofstream &file, std::string filename,
                  bool overWrite,
                  bool append,
                  bool exitOnFailure) {
  // std::ofstream file;
  if (fexists(filename) && !overWrite) {
  	if(append){
  		file.open(filename.data(), std::ios::app);
  	}else{
      std::cout << filename << " already exists" << std::endl;
      if (exitOnFailure) {
        exit(1);
      }
  	}

  } else {
    file.open(filename.data());
    if (!file) {
      std::cout << "Error in opening " << filename << std::endl;
      if (exitOnFailure) {
        exit(1);
      }
    } else {
      // chmod(filename.c_str(), S_IRWU|S_IRGRP|S_IWGRP|S_IROTH);
      chmod(filename.c_str(),
            S_IWUSR | S_IRUSR | S_IRGRP | S_IWGRP | S_IROTH | S_IWOTH);
      // chmod(filename.c_str(), S_IRWXU|S_IRGRP|S_IXGRP|S_IWGRP|S_IROTH);
    }
  }
}
}
