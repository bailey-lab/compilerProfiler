/*
 *
 *
 *  Created on: Jun 25, 2014
 *      Author: nickhathaway <nickjhathaway@gmail.com>
 */
#include "compro/utils/utils.hpp"
namespace compro {
std::string convertBoolToString(bool convert) {
  if (convert) {
    return "true";
  } else {
    return "false";
  }
}

// check to see if a file exists
bool fexists(const std::string &filename) {
  std::ifstream ifile(filename.c_str());
  if (ifile) {
    return true;
  } else {
    return false;
  }
}

void openTextFile(std::ofstream &file, std::string filename, bool overWrite,
                  bool append, bool exitOnFailure) {
  // std::ofstream file;
  if (fexists(filename) && !overWrite) {
    if (append) {
      file.open(filename.data(), std::ios::app);
    } else {
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
std::map<std::string, std::pair<std::string, bool>> getFiles(
    const std::string &directoryName, const std::string &contains,
    const std::string &filesOrDirectories, bool specific, bool recursive) {
  std::map<std::string, std::pair<std::string, bool>> allFiles =
      listFilesInDir(directoryName, recursive);
  std::map<std::string, std::pair<std::string, bool>>::iterator fileIter;

  if (filesOrDirectories == "file") {
    std::map<std::string, std::pair<std::string, bool>> allFileFiles;
    for (fileIter = allFiles.begin(); fileIter != allFiles.end(); ++fileIter) {
      if (fileIter->second.first == filesOrDirectories) {
        allFileFiles.insert(*fileIter);
      }
    }
    allFiles = allFileFiles;
  } else if (filesOrDirectories == "directory") {
    std::map<std::string, std::pair<std::string, bool>> allDirectoryFiles;
    for (fileIter = allFiles.begin(); fileIter != allFiles.end(); ++fileIter) {
      if (fileIter->second.first == filesOrDirectories) {
        allDirectoryFiles.insert(*fileIter);
      }
    }
    allFiles = allDirectoryFiles;
  }

  if (specific) {
    std::map<std::string, std::pair<std::string, bool>> specificFiles;
    for (fileIter = allFiles.begin(); fileIter != allFiles.end(); ++fileIter) {
      if (fileIter->first.find(contains) != std::string::npos) {
        specificFiles.insert(*fileIter);
      }
    }
    return specificFiles;
  } else {
    return allFiles;
  }
}
std::map<std::string, std::pair<std::string, bool>> getFiles(
    const std::string &directoryName, const VecStr &contains,
    const std::string &filesOrDirectories, bool specific, bool recursive) {
  std::map<std::string, std::pair<std::string, bool>> allFiles =
      listFilesInDir(directoryName, recursive);
  std::map<std::string, std::pair<std::string, bool>>::iterator fileIter;
  if (filesOrDirectories == "file") {
    std::map<std::string, std::pair<std::string, bool>> allFileFiles;
    for (fileIter = allFiles.begin(); fileIter != allFiles.end(); ++fileIter) {
      if (fileIter->second.first == filesOrDirectories) {
        allFileFiles.insert(*fileIter);
      }
    }
    allFiles = allFileFiles;
  } else if (filesOrDirectories == "directory") {
    std::map<std::string, std::pair<std::string, bool>> allDirectoryFiles;
    for (fileIter = allFiles.begin(); fileIter != allFiles.end(); ++fileIter) {
      if (fileIter->second.first == filesOrDirectories) {
        allDirectoryFiles.insert(*fileIter);
      }
    }
    allFiles = allDirectoryFiles;
  }

  if (specific) {
    std::map<std::string, std::pair<std::string, bool>> specificFiles;
    for (fileIter = allFiles.begin(); fileIter != allFiles.end(); ++fileIter) {
      bool containsAll = true;
      for (VecStr::const_iterator conIter = contains.begin();
           conIter != contains.end(); ++conIter) {
        if (fileIter->first.find(*conIter) == std::string::npos) {
          containsAll = false;
          break;
        }
      }
      if (containsAll) {
        specificFiles.insert(*fileIter);
      }
    }
    return specificFiles;
  } else {
    return allFiles;
  }
}
int getdir(const std::string &dir,
           std::map<std::string, std::pair<std::string, bool>> &files) {
  DIR *dp;
  struct dirent *dirp;
  if ((dp = opendir(dir.c_str())) == NULL) {
    std::cout << "Error(" << errno << ") opening " << dir << std::endl;
    return errno;
  }
  int status = 0;
  while ((dirp = readdir(dp)) != NULL) {

    struct stat st_buf;
    std::stringstream tempName;
    tempName << dir << "/" << std::string(dirp->d_name).c_str();
    // status=stat(std::string(dirp->d_name).c_str(),&st_buf);
    status = stat(tempName.str().c_str(), &st_buf);
    if (std::string(dirp->d_name) == "." || std::string(dirp->d_name) == "..") {
    } else {
      if (S_ISDIR(st_buf.st_mode)) {
        files.insert(
            std::make_pair(tempName.str(), std::make_pair("directory", false)));
      } else {
        files.insert(
            std::make_pair(tempName.str(), std::make_pair("file", false)));
      }
    }
  }
  closedir(dp);
  return status;
}

std::map<std::string, std::pair<std::string, bool>> listFilesInDir(
    const std::string &directoryName, bool recursive) {
  std::map<std::string, std::pair<std::string, bool>> files;
  std::map<std::string, std::pair<std::string, bool>>::iterator fileIter;
  getdir(directoryName, files);
  if (recursive) {
    bool searching = true;
    while (searching) {
      for (fileIter = files.begin(); fileIter != files.end(); ++fileIter) {
        if (fileIter->second.first == "directory" && !fileIter->second.second) {
          getdir(fileIter->first, files);
          fileIter->second.second = true;
        }
      }
      for (fileIter = files.begin(); fileIter != files.end(); ++fileIter) {
        searching = false;
        if (fileIter->second.first == "directory" && !fileIter->second.second) {
          searching = true;
          break;
        }
      }
    }
  }
  return files;
}
} // compro
