/*
 * 
 *


 *  Created on: Jun 25, 2014
 *      Author: nickhathaway <nickjhathaway@gmail.com>
 */
#include "stringUtils.hpp"
#include "numUtils.hpp"

namespace compro{
std::string getTimeFormat(double timeInSecondsOriginal, bool wordy,
                          int secondsDecimalPlaces) {
  std::stringstream duration;
  double timeInSeconds = timeInSecondsOriginal;
  if (timeInSeconds > 31536000) {
    int years = (int)timeInSeconds / 31536000;
    if (wordy) {
      duration << "yrs:" << years << ",";
    } else {
      duration << years << ":";
    }
    timeInSeconds = timeInSeconds - years * 31536000.0;
  }
  if (timeInSeconds > 86400) {
    int days = (int)timeInSeconds / 86400;
    if (wordy) {
      duration << "days:" << leftPadNumStr(days, 365) << ",";
    } else {
      duration << days << ":";
    }
    timeInSeconds = timeInSeconds - days * 86400.0;
  } else if (timeInSecondsOriginal > 86400) {
    if (wordy) {
      duration << "days:000,";
    } else {
      duration << "000:";
    }
  }
  if (timeInSeconds > 3600) {
    int hrs = (int)timeInSeconds / 3600;
    if (wordy) {
      duration << "hrs:" << leftPadNumStr(hrs, 24) << ",";
    } else {
      duration << leftPadNumStr(hrs, 24) << ":";
    }
    timeInSeconds = timeInSeconds - hrs * 3600.0;
  } else if (timeInSecondsOriginal > 3600.0) {
    if (wordy) {
      duration << "hrs:00,";
    } else {
      duration << "00:";
    }
  }
  if (timeInSeconds > 60) {
    int minutes = (int)timeInSeconds / 60;
    if (wordy) {
      duration << "mins:" << leftPadNumStr(minutes, 60) << ",";
    } else {
      duration << leftPadNumStr(minutes, 60) << ":";
    }

    timeInSeconds = timeInSeconds - minutes * 60.0;
  } else if (timeInSecondsOriginal > 60) {
    if (wordy) {
      duration << "mins:00,";
    } else {
      duration << "00:";
    }
  }
  if (timeInSeconds > 0) {
    if (timeInSecondsOriginal < 1) {
      if (wordy) {
        duration << "secs:" << roundDecPlaces(timeInSeconds,
                                              secondsDecimalPlaces);
      } else {
        duration << roundDecPlaces(timeInSeconds, secondsDecimalPlaces);
      }
    } else {
      if (wordy) {
        duration << "secs:"
                 << leftPadNumStr(
                        roundDecPlaces(timeInSeconds, secondsDecimalPlaces),
                        60.0);
      } else {
        duration << leftPadNumStr(
                        roundDecPlaces(timeInSeconds, secondsDecimalPlaces),
                        60.0);
      }
    }
  } else {
    if (wordy) {
      duration << "secs:00";
    } else {
      duration << "00";
    }
  }
  return duration.str();
}


void stringToUpper(std::string& str) {
  for (auto& c : str) {
    c = toupper(c);
  }
}
void stringToLower(std::string& str) {
  for (auto& c : str) {
    c = tolower(c);
  }
}
std::string stringToUpperReturn(std::string str) {
  stringToUpper(str);
  return str;
}
std::string stringToLowerReturn(std::string str) {
  stringToLower(str);
  return str;
}
void changeStringVectorToLowerCase(VecStr& vec) {
  for (auto& v : vec) {
    stringToLower(v);
  }
}
std::string get_cwd() {
  // from http://stackoverflow.com/a/2869667
  const size_t chunkSize = 255;
  const int maxChunks =
      10240;  // 2550 KiBs of current path are more than enough

  char stackBuffer[chunkSize];  // Stack buffer for the "normal" case
  if (getcwd(stackBuffer, sizeof(stackBuffer)) != NULL) return stackBuffer;
  if (errno != ERANGE) {
    // It's not ERANGE, so we don't know how to handle it
    throw std::runtime_error("Cannot determine the current path.");
    // Of course you may choose a different error reporting method
  }
  // Ok, the stack buffer isn't long enough; fallback to heap allocation
  for (int chunks = 2; chunks < maxChunks; chunks++) {
    // With boost use scoped_ptr; in C++0x, use unique_ptr
    // If you want to be less C++ but more efficient you may want to use realloc
    std::unique_ptr<char> cwd(new char[chunkSize * chunks]);
    if (getcwd(cwd.get(), chunkSize * chunks) != NULL) return cwd.get();
    if (errno != ERANGE) {
      // It's not ERANGE, so we don't know how to handle it
      throw std::runtime_error("Cannot determine the current path.");
      // Of course you may choose a different error reporting method
    }
  }
  throw std::runtime_error(
      "Cannot determine the current path; the path is apparently unreasonably "
      "long");
  // return "";
}



std::vector<std::string> tokenizeString(const std::string& str, const std::string& delim,
                      bool addEmptyToEnd) {
	std::vector<std::string> output;
  if("whitespace" == delim){
    std::stringstream tempStream(str);
    while (!tempStream.eof()) {
      std::string tempName;
      tempStream >> tempName;
      output.emplace_back(tempName);
    }
  }else{
    if (str.find(delim.c_str()) == std::string::npos) {
      output.push_back(str);
    } else {
      std::size_t pos = str.find(delim, 0);
      std::size_t oldPos = -delim.size();
      while (pos != std::string::npos) {
        output.push_back(
            str.substr(oldPos + delim.size(), pos - oldPos - delim.size()));
        oldPos = pos;
        pos = str.find(delim, pos + 1);
      }
      if (oldPos + delim.size() == str.size()) {
        if (addEmptyToEnd) {
          output.push_back("");
        }
      } else {
        output.push_back(str.substr(oldPos + delim.size(), str.size() - 1));
      }
    }
  }
  return output;
}

std::vector<char> processAlphStrVecChar(const std::string & alphabetStr, const std::string & delim){
	std::vector<char> ans;
	std::vector<std::string> toks = tokenizeString(alphabetStr, delim);
	for(const auto & t : toks){
		ans.emplace_back(t[0]);
	}
	return ans;
}

std::pair<std::vector<char>, std::vector<uint32_t>> processAlphStrVecCharCounts(const std::string & alphabetStr,
		const std::string & delim){
	std::vector<char> ansLets;
	std::vector<uint32_t> ansCounts;
	std::vector<std::string> toks = tokenizeString(alphabetStr, delim);
	for(const auto & t : toks){
		ansLets.emplace_back(t[0]);
		if(t.length() > 2){
			ansCounts.emplace_back(std::stof(t.substr(1)));
		}else{
			ansCounts.emplace_back(1);
		}
	}
	return {ansLets, ansCounts};
}

bool stringContainsAllDigits(const std::string& str) {
  for (const auto& c : str) {
    if (!isdigit(c)) {
      return false;
    }
  }
  return true;
}
}
