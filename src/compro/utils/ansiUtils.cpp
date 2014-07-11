/*
 * 
 *
 *  Created on: Jun 25, 2014
 *      Author: nickhathaway <nickjhathaway@gmail.com>
 */
#include "compro/utils/ansiUtils.hpp"

namespace compro{
std::string centerText(const std::string & text, uint32_t maxWidth){
	uint32_t halfWay = std::round(maxWidth/2.0);
	uint32_t halfText = std::round(text.size()/2.0);
	return std::string(halfWay - halfText, ' ') + text;
}


std::string boldText(const std::string& title, const std::string& colorCode) {
  return "\033[1;" + colorCode + "m" + title + "\033[0m";
}
std::string boldBlackText(const std::string& title) {
  return boldText(title, "30");
}
std::string centerTitle(const std::string & text, uint32_t maxWidth){
	return centerText(boldBlackText(text), maxWidth);
}
std::string addFlashingColor(const std::string & colorCode){
	return "\033[38;5;" + colorCode + ";147;5" + "m";
}
std::string addFlashingColor(uint32_t colorCode){
	return addFlashingColor(to_string(colorCode));
}
std::string addAnsiAtr(const std::string & atr){
	return "\033[" + atr + "m";
}
std::string changeBackground(const std::string & colorCode){
	return "\033[48;5;" + colorCode + "m";
}
std::string changeColor(const std::string & colorCode){
	return "\033[38;5;" + colorCode + "m";
}
std::string changeColor(uint32_t colorCode){
	return changeColor(to_string(colorCode));
}
std::string changeBackground(uint32_t colorCode){
	return changeBackground(to_string(colorCode));
}


std::string endAllAttributes(const std::string & add){
	std::string output = "\033[0m";
	if(add != ""){
		output.append("\033[" + add + "m");
	}
	return output;
}
}
