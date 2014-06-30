/*
 * 
 *
 *  Created on: Jun 25, 2014

 *      Author: nickhathaway <nickjhathaway@gmail.com>
 */
#include "substitutionMatrix.hpp"
namespace compro{

	void substituteMatrix::setWithZeros(){
		for(const auto & row : iter::range(mat_.size())){
			for(const auto & col : iter::range(mat_[row].size())){
				mat_[row][col] = 0;
			}
		}
	}

	void substituteMatrix::setWithSimple(int32_t match, int32_t mismatch){
		for(const auto & row : iter::range(mat_.size())){
			for(const auto & col : iter::range(mat_[row].size())){
				if(row == col){
					mat_[row][col] = match;
				}else{
					mat_[row][col] = mismatch;
				}
			}
		}
	}

	void substituteMatrix::setWithCaseInsensitive(int32_t match, int32_t mismatch){
	  for (auto i : iter::range(mat_.size())) {
	    for (auto j : iter::range(mat_[i].size())) {
	      if (i == j) {
	      	mat_[i][j] = match;
	      } else {
	      	mat_[i][j] = mismatch;
	      }
	    }
	  }
	  std::vector<char> upperCase(26);
	  std::iota(upperCase.begin(), upperCase.end(), 'A');
	  std::vector<char> lowerCase(26);
	  std::iota(lowerCase.begin(), lowerCase.end(), 'a');
	  for (const auto& i : iter::range(lowerCase.size())) {
	  	mat_[upperCase[i]][lowerCase[i]] = match;
	  	mat_[lowerCase[i]][upperCase[i]] = match;
	  }
	}
}
