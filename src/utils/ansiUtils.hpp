#pragma once
/*
 * 
 *

*  Created on: Jun 25, 2014
 *      Author: nickhathaway <nickjhathaway@gmail.com>
 */
#include "common.h"

namespace compro{
std::string centerText(const std::string & text, uint32_t maxWidth);
std::string boldText(const std::string& title, const std::string& colorCode);
std::string boldBlackText(const std::string& title) ;
std::string centerTitle(const std::string & text, uint32_t maxWidth);
std::string addFlashingColor(const std::string & colorCode);
std::string addFlashingColor(uint32_t colorCode);
std::string addAnsiAtr(const std::string & atr);
std::string changeBackground(const std::string & colorCode);
std::string changeColor(const std::string & colorCode);
std::string changeColor(uint32_t colorCode);
std::string changeBackground(uint32_t colorCode);
std::string endAllAttributes(const std::string & add);
}
