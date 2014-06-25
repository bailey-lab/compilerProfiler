#pragma once
/*
 * 
 *
 *  Created on: Jun 25, 2014
 *      Author: nickhathaway <nickjhathaway@gmail.com>
 */
#include "common.h"
#include "alignment/substitutionMatrix.hpp"
#include "alignment/alignerUtils.hpp"
#include "alignment/alignInfoHolder.hpp"

/*! \brief Aligner Class
 *
 *
 *  This class can do local or global alignment using simple scoring or using a
 *provide scoring matrix
 */
template<typename NUM>
class alignerTemp {
  /*! \brief An alignment score matrix cell
   *
   *
   *  This structure is used to hold the score for an alignment score matrix
   */
  //template <typename NUM>
  struct scoreMatrixCell {
    NUM upInherit;
    NUM leftInherit;
    NUM diagInherit;
    // for traceback: 'U' = up, 'L' = Left, 'D' = diagonal, 'B' either up or left
    char upInheritPtr;
    char leftInheritPtr;
    char diagInheritPtr;
  };

 public:
  // constructors
  alignerTemp() : maxSize_(400), gapScores_(gapScoringParameters<NUM>()),
  scoring_(substituteMatrix(2,-2)),
  ScoreMatrix_(std::vector<std::vector<scoreMatrixCell>>(
      maxSize_, std::vector<scoreMatrixCell>(maxSize_))), currentSetUp_("none") {}
  // score maxtrix maximum functions
  /*! \brief Constructor with Scoring array
   *
   *
   *  This constructor sets up the scoring for a simple 4 by 4 array
   */
  // score maxtrix maximum functions
  /*! \brief Constructor with Scoring map
   *
   *
   *  This constructor sets up the scoring for a determined by a map of scores
   *allowing for more complex scoring set up
   */
  alignerTemp(uint32_t maxSize, const gapScoringParameters <NUM>& gapPars,
          const substituteMatrix& scoreMatrix): maxSize_(maxSize + 10),
          		gapScores_(gapPars),scoring_(scoreMatrix),
          		ScoreMatrix_(std::vector<std::vector<scoreMatrixCell>>(
              maxSize_, std::vector<scoreMatrixCell>(maxSize_))), currentSetUp_("none")  {

  }
  // score maxtrix maximum functions
  /*! \brief Constructor with for simple string alignment
   *
   *
   *  This constructor sets up the scoring for a determined by a map of scores
   *allowing for more complex scoring set up
   */


  // to hold the sequence alignments
  std::string alignObjectA_;
  std::string alignObjectB_;

  // alignment score
  NUM score_;

  // the largest the size of the scoring matrix
  /*! \brief The max size for alignment
   *
   *  This defines the maximum length of a sequence to be aligned to pre-make the
   *  alignment score matrix to the appropriate size
   */
  uint32_t maxSize_;
  // gap scores
  gapScoringParameters <NUM> gapScores_;
  substituteMatrix scoring_;
  alnInfoGlobal gHolder_;
  alnInfoLocal lHolder_;
  // the matrix
  std::vector<std::vector<scoreMatrixCell>> ScoreMatrix_;
  // score maxtrix maximum functions
  /*! \brief Needle Maximum
   *
   *
   *  Function is used to find the maximum of an alignment cell using needle
   *(global) style alignment
   */
  //template <typename NUM>
  NUM needleMaximum(NUM u, NUM l, NUM d, char& p) {
    if (u == l && u == d) {
      p = 'B';
      return u;
    } else if (u >= l && u >= d) {
      if (u == l) {
        p = 'B';
        return u;
      } else {
        p = 'U';
        return u;
      }
    } else if (l >= u && l >= d) {
      if (l == u) {
        p = 'B';
        return l;
      } else {
        p = 'L';
        return l;
      }
    } else {
      p = 'D';
      return d;
    }
  }
  /*! \brief Smith Maximum
   *
   *
   *  Function is used to find the maximum of an alignment cell using smith
   *(local) style alignment
   */
  //template <typename NUM>
  NUM smithMaximum(NUM u, NUM l, NUM d, char& p) {
    if (u < 0 && l < 0 && d < 0) {
      p = '\0';
      return 0;
    }
    if (u == l && u == d) {
      p = 'B';
      return u;
    } else if (u >= l && u >= d) {
      if (u == l) {
        p = 'B';
        return u;
      } else {
        p = 'U';
        return u;
      }
    } else if (l >= u && l >= d) {
      if (l == u) {
        p = 'B';
        return l;
      } else {
        p = 'L';
        return l;
      }
    } else {
      p = 'D';
      return d;
    }
  }
  // scorematrix initializer
  void initializeNeedle(){
  	// Create the alignment score matrix to do the alignment, a column for each
		// letter in sequence b and a row for each letter in sequence a
		ScoreMatrix_[0][0].leftInherit = 0;
		ScoreMatrix_[0][0].upInherit = 0;
		ScoreMatrix_[0][0].diagInherit = 0;
		ScoreMatrix_[0][0].upInheritPtr = '\0';
		ScoreMatrix_[0][0].leftInheritPtr = '\0';
		ScoreMatrix_[0][0].diagInheritPtr = '\0';
		// initialize first column:
		for (int i = 1; i < maxSize_; i++) {
			if (i == 1) {
				ScoreMatrix_[i][0].upInherit = -gapScores_.gapLeftOpen_;
				ScoreMatrix_[i][0].leftInherit = 0;
				ScoreMatrix_[i][0].diagInherit = 0;
				ScoreMatrix_[i][0].upInheritPtr = 'U';
				ScoreMatrix_[i][0].leftInheritPtr = '\0';
				ScoreMatrix_[i][0].diagInheritPtr = '\0';
			} else {
				ScoreMatrix_[i][0].upInherit =
						ScoreMatrix_[i - 1][0].upInherit - gapScores_.gapLeftExtend_;
				ScoreMatrix_[i][0].leftInherit = 0;
				ScoreMatrix_[i][0].diagInherit = 0;
				ScoreMatrix_[i][0].upInheritPtr = 'U';
				ScoreMatrix_[i][0].leftInheritPtr = '\0';
				ScoreMatrix_[i][0].diagInheritPtr = '\0';
			}
		}
		// initialize first row:
		for (int j = 1; j < maxSize_; j++) {
			if (j == 1) {
				ScoreMatrix_[0][j].upInherit = 0;
				ScoreMatrix_[0][j].leftInherit = -gapScores_.gapLeftOpen_;
				ScoreMatrix_[0][j].diagInherit = 0;
				ScoreMatrix_[0][j].upInheritPtr = '\0';
				ScoreMatrix_[0][j].leftInheritPtr = 'L';
				ScoreMatrix_[0][j].diagInheritPtr = '\0';
			} else {
				ScoreMatrix_[0][j].upInherit = 0;
				ScoreMatrix_[0][j].leftInherit =
						ScoreMatrix_[0][j - 1].leftInherit - gapScores_.gapLeftExtend_;
				ScoreMatrix_[0][j].diagInherit = 0;
				ScoreMatrix_[0][j].upInheritPtr = '\0';
				ScoreMatrix_[0][j].leftInheritPtr = 'L';
				ScoreMatrix_[0][j].diagInheritPtr = '\0';
			}
		}
		currentSetUp_ = "needle";
  }
  void initializeSmith(){
  	 // Create the alignment score matrix to do the alignment, a column for each
		// letter in sequence b and a row for each letter in sequence a
		ScoreMatrix_[0][0].leftInherit = 0;
		ScoreMatrix_[0][0].upInherit = 0;
		ScoreMatrix_[0][0].diagInherit = 0;
		ScoreMatrix_[0][0].upInheritPtr = '\0';
		ScoreMatrix_[0][0].leftInheritPtr = '\0';
		ScoreMatrix_[0][0].diagInheritPtr = '\0';
		// initialize first column:
		for (int i = 1; i < maxSize_; i++) {
			ScoreMatrix_[i][0].upInherit = 0;
			ScoreMatrix_[i][0].leftInherit = 0;
			ScoreMatrix_[i][0].diagInherit = 0;
			ScoreMatrix_[i][0].upInheritPtr = 'U';
			ScoreMatrix_[i][0].leftInheritPtr = '\0';
			ScoreMatrix_[i][0].diagInheritPtr = '\0';
		}
		// initialize first row:
		for (int j = 1; j < maxSize_; j++) {
			ScoreMatrix_[0][j].upInherit = 0;
			ScoreMatrix_[0][j].leftInherit = 0;
			ScoreMatrix_[0][j].diagInherit = 0;
			ScoreMatrix_[0][j].upInheritPtr = '\0';
			ScoreMatrix_[0][j].leftInheritPtr = 'L';
			ScoreMatrix_[0][j].diagInheritPtr = '\0';
		}
		currentSetUp_ = "smith";
  }
  // the current set up for needle or smith
  std::string currentSetUp_;
  // Aligner
  void alignSeqSave(const std::string& firstSeq,
                const std::string& secondSeq, bool local){
  	if(local){
  		runSmithSave(firstSeq, secondSeq);
  	}else{
  		runNeedleSave(firstSeq, secondSeq);
  	}
  }
  void alignSeq(const std::string& firstSeq,
                const std::string& secondSeq, bool local){
		alignObjectA_ = firstSeq;
		alignObjectB_ = secondSeq;
  	if(local){
  		runSmithSave(firstSeq, secondSeq);
  		rearrangeLocal(alignObjectA_, alignObjectB_, '-');
  	}else{
  		//std::cout << "What1? " << std::endl;
  		runNeedleSave(firstSeq, secondSeq);
  		//std::cout << "What2? " << std::endl;
  		//std::cout << alignObjectA_ << std::endl;
  		//std::cout << alignObjectB_ << std::endl;
  		rearrangeGlobal(alignObjectA_, alignObjectB_, '-');
  		//std::cout << "What3? " << std::endl;
  	}
  }

  void runNeedleSave(const std::string& objA, const std::string& objB){
		// std::cout << "doing needle reg" << std::endl;
		// empty the alignment strings and qualities vectors to reset for the new
		// alignment
		// std::cout<<"mark needle non simple"<<std::endl;
		if (currentSetUp_ != "needle") {
			initializeNeedle();
		}
		gHolder_.gapInfos_.clear();
		gHolder_.addFromFile_ = false;
		// objectA=objA;
		// objectB=objB;

		//alignObjectA_.clear();
		//alignObjectB_.clear();
		// get the length of the strings to create the alignment score matrix
		uint32_t lena = objA.size() + 1;
		uint32_t lenb = objB.size() + 1;
		// std::cout<<"length of A"<<lena<<std::endl;
		// std::cout<<"length of B"<<lenb<<std::endl;
		for (uint32_t i = 1; i < lena; i++) {
			for (uint32_t j = 1; j < lenb; j++) {
				// std::cout<<"i: "<<i<<"j: "<<j<<std::endl;
				char ptrFlag;
				// first set the upInherit score. If we are in the first row, this will be
				// the value of the above cell's leftInherit score minus a gap open
				// penalty.
				// Otherwise, it will be the max of the three scores in the cell above,
				// with the appropriate penalty applied (either a gapScores_.gapOpen or
				// gapScores_.gapExtend_).
				if (i == 1) {
					if (j == lenb - 1) {
						ScoreMatrix_[i][j].upInherit =
								ScoreMatrix_[i - 1][j].leftInherit - gapScores_.gapRightOpen_;
					} else {
						ScoreMatrix_[i][j].upInherit =
								ScoreMatrix_[i - 1][j].leftInherit - gapScores_.gapOpen_;
					}
					ScoreMatrix_[i][j].upInheritPtr = 'L';
				} else {
					if (j == lenb - 1) {
						ScoreMatrix_[i][j].upInherit = needleMaximum(
								ScoreMatrix_[i - 1][j].upInherit - gapScores_.gapRightExtend_,
								ScoreMatrix_[i - 1][j].leftInherit - gapScores_.gapRightOpen_,
								ScoreMatrix_[i - 1][j].diagInherit - gapScores_.gapRightOpen_,
								ptrFlag);
						ScoreMatrix_[i][j].upInheritPtr = ptrFlag;
					} else {
						ScoreMatrix_[i][j].upInherit = needleMaximum(
								ScoreMatrix_[i - 1][j].upInherit - gapScores_.gapExtend_,
								ScoreMatrix_[i - 1][j].leftInherit - gapScores_.gapOpen_,
								ScoreMatrix_[i - 1][j].diagInherit - gapScores_.gapOpen_, ptrFlag);
						ScoreMatrix_[i][j].upInheritPtr = ptrFlag;
					}
				}

				// next set the leftInherit score. If we are in the first column, this
				// will be the value of the left cell's upInherit score minus a gap open
				// penalty.
				// Otherwise, it will be the max score of the three scores in the cell to
				// the left, with the appropriate penalty applied.
				if (j == 1) {
					if (i == lena - 1) {
						ScoreMatrix_[i][j].leftInherit =
								ScoreMatrix_[i][j - 1].upInherit - gapScores_.gapRightOpen_;
					} else {
						ScoreMatrix_[i][j].leftInherit =
								ScoreMatrix_[i][j - 1].upInherit - gapScores_.gapOpen_;
					}
					ScoreMatrix_[i][j].leftInheritPtr = 'U';
				} else {
					if (i == lena - 1) {
						ScoreMatrix_[i][j].leftInherit = needleMaximum(
								ScoreMatrix_[i][j - 1].upInherit - gapScores_.gapRightOpen_,
								ScoreMatrix_[i][j - 1].leftInherit - gapScores_.gapRightExtend_,
								ScoreMatrix_[i][j - 1].diagInherit - gapScores_.gapRightOpen_,
								ptrFlag);
						ScoreMatrix_[i][j].leftInheritPtr = ptrFlag;
					} else {
						ScoreMatrix_[i][j].leftInherit = needleMaximum(
								ScoreMatrix_[i][j - 1].upInherit - gapScores_.gapOpen_,
								ScoreMatrix_[i][j - 1].leftInherit - gapScores_.gapExtend_,
								ScoreMatrix_[i][j - 1].diagInherit - gapScores_.gapOpen_, ptrFlag);
						ScoreMatrix_[i][j].leftInheritPtr = ptrFlag;
					}
				}
				// int match = scoringArray[objA.seqBase_.seq_[i -
				// 1]-'A'][objB.seqBase_.seq_[j - 1]-'A'];
				int match = scoring_.mat_[objA[i - 1]][objB[j - 1]];

				if (i == 1) {
					ScoreMatrix_[i][j].diagInherit =
							ScoreMatrix_[i - 1][j - 1].leftInherit + match;
					ScoreMatrix_[i][j].diagInheritPtr = 'L';
				} else if (j == 1) {
					ScoreMatrix_[i][j].diagInherit =
							ScoreMatrix_[i - 1][j - 1].upInherit + match;
					ScoreMatrix_[i][j].diagInheritPtr = 'U';
				} else {
					ScoreMatrix_[i][j].diagInherit =
							match + needleMaximum(ScoreMatrix_[i - 1][j - 1].upInherit,
																		ScoreMatrix_[i - 1][j - 1].leftInherit,
																		ScoreMatrix_[i - 1][j - 1].diagInherit,
																		ptrFlag);
					ScoreMatrix_[i][j].diagInheritPtr = ptrFlag;
				}
			}
		}
	  int icursor = lena - 1;
	  int jcursor = lenb - 1;

	  // tracerNext holds to where to go next in the matrix, will be (D) diagonal,
	  // (U) up, or (L) left depending on the maximum score determined during the
	  // matrix set up.
	  char tracerNext = ' ';

	  // get the alignment score from the  bottom right cell and set the tacer to
	  // where to go next
	  // keep tracing back until at the begining of either sequence
	  // Traceback algorithm follows. Score is the max of all three scores stored in
	  // the bottom right cell.
	  // Alignments are constructed by following the correct pointer backwards at
	  // each stage.
	  // Since alignment strings are constructed in reverse, we must call the
	  // reverse() funcion after traceback.
	  score_ =
	      needleMaximum(ScoreMatrix_[icursor][jcursor].upInherit,
	                    ScoreMatrix_[icursor][jcursor].leftInherit,
	                    ScoreMatrix_[icursor][jcursor].diagInherit, tracerNext);
	  uint32_t gapBSize = 0;
	  uint32_t gapASize = 0;
	  // std::cout <<"rnv2" << std::endl;
	  while (icursor != 0 || jcursor != 0) {
	    if (tracerNext == 'U') {
	      ++gapBSize;
	      tracerNext = ScoreMatrix_[icursor][jcursor].upInheritPtr;
	      if (tracerNext != 'U' && tracerNext != 'B') {
	        gHolder_.gapInfos_.emplace_back(
	            gapInfo(jcursor, gapBSize, false));
	        gapBSize = 0;
	      }
	      icursor--;
	    } else if (tracerNext == 'L') {
	      ++gapASize;
	      tracerNext = ScoreMatrix_[icursor][jcursor].leftInheritPtr;
	      if (tracerNext != 'L') {
	      	gHolder_.gapInfos_.emplace_back(gapInfo(icursor, gapASize, true));
	        gapASize = 0;
	      }
	      jcursor--;
	    } else if (tracerNext == 'D') {
	      tracerNext = ScoreMatrix_[icursor][jcursor].diagInheritPtr;
	      icursor--;
	      jcursor--;
	    }
	    // if ambigous traceback (can go either left or up), we give precedence
	    // to an 'up' traceback.
	    else if (tracerNext == 'B') {
	      ++gapBSize;
	      tracerNext = ScoreMatrix_[icursor][jcursor].upInheritPtr;
	      if (tracerNext != 'U' && tracerNext != 'B') {
	      	gHolder_.gapInfos_.emplace_back(
	            gapInfo(jcursor, gapBSize, false));
	        gapBSize = 0;
	      }
	      icursor--;
	    }
	  }
	  // std::cout <<"rnv3" << std::endl;
	  if ((tracerNext == 'U' || tracerNext == 'B') && gapBSize != 0) {
	  	gHolder_.gapInfos_.emplace_back(gapInfo(jcursor, gapBSize, false));
	  } else if (tracerNext == 'L' && gapASize != 0) {
	  	gHolder_.gapInfos_.emplace_back(gapInfo(icursor, gapASize, true));
	  }
	  // std::cout <<"rnv4" << std::endl;
	  //rearrangeGlobal(objA, objB);
	  // std::cout <<"rnv5" << std::endl;
	  //alnInfoGlobal_.addFromFile_ = false;
	  //alnHolders_.globalHolder_[gapScores_.getIdentifer()]
	    //  .addAlnInfo(objA.seq_, objB.seq_, alnInfoGlobal_);
	  // std::cout <<"rnv6" << std::endl;
	  // std::cout << "alnHolders_.infos_[false][objA.seqBase_.seq_].size()" <<
	  // std::endl;
	  // std::cout << "\t" << alnHolders_.infos_[false][objA.seqBase_.seq_].size()
	  // << std::endl;

	}


  void runSmithSave(const std::string& objA, const std::string& objB){
    // std::cout << "doing smith reg" << std::endl;
    // std::cout<<"mark smith non simple"<<std::endl;
    if (currentSetUp_ != "smith") {
      initializeSmith();
    }
    // to find the best score
    // objectA=objA;
    // objectB=objB;
    lHolder_.addFromFile_ = false;
    lHolder_.gapInfos_.clear();
    uint32_t bestJ = 0;
    uint32_t bestI = 0;
    NUM bestValue = 0;
    // empty the alignment strings and qualities vectors to reset for the new
    // alignment
    //alignObjectA_.clear();
    //alignObjectB_.clear();
    // get the lenth of the strings to create the alignment score matrix
		uint32_t lena = objA.size() + 1;
		uint32_t lenb = objB.size() + 1;
    for (uint32_t i = 1; i < lena; i++) {
      for (uint32_t j = 1; j < lenb; j++) {
        char ptrFlag;
        // first set the upInherit score. If we are in the first row, this will be
        // the value of the above cell's leftInherit score minus a gap open
        // penalty.
        // Otherwise, it will be the max of the three scores in the cell above,
        // with the appropriate penalty applied (either a gapScores_.gapOpen or
        // gapScores_.gapExtend_).
        ScoreMatrix_[i][j].upInherit = smithMaximum(
            ScoreMatrix_[i - 1][j].upInherit - gapScores_.gapExtend_,
            ScoreMatrix_[i - 1][j].leftInherit - gapScores_.gapOpen_,
            ScoreMatrix_[i - 1][j].diagInherit - gapScores_.gapOpen_, ptrFlag);
        ScoreMatrix_[i][j].upInheritPtr = ptrFlag;

        // next set the leftInherit score. If we are in the first column, this
        // will be the value of the left cell's upInherit score minus a gap open
        // penalty.
        // Otherwise, it will be the max score of the three scores in the cell to
        // the left, with the appropriate penalty applied.
        ScoreMatrix_[i][j].leftInherit = smithMaximum(
            ScoreMatrix_[i][j - 1].upInherit - gapScores_.gapOpen_,
            ScoreMatrix_[i][j - 1].leftInherit - gapScores_.gapExtend_,
            ScoreMatrix_[i][j - 1].diagInherit - gapScores_.gapOpen_, ptrFlag);
        ScoreMatrix_[i][j].leftInheritPtr = ptrFlag;
        // int match = scoringArray[objA.seqBase_.seq_[i -
        // 1]-'A'][objB.seqBase_.seq_[j - 1]-'A'];
        int match = scoring_.mat_[objA[i - 1]][objB[j - 1]];
        ScoreMatrix_[i][j].diagInherit =
            match + smithMaximum(ScoreMatrix_[i - 1][j - 1].upInherit,
                                 ScoreMatrix_[i - 1][j - 1].leftInherit,
                                 ScoreMatrix_[i - 1][j - 1].diagInherit, ptrFlag);
        ScoreMatrix_[i][j].diagInheritPtr = ptrFlag;
        NUM tempValue = smithMaximum(ScoreMatrix_[i][j].upInherit,
                                        ScoreMatrix_[i][j].leftInherit,
                                        ScoreMatrix_[i][j].diagInherit, ptrFlag);
        if (tempValue > bestValue) {
          bestValue = tempValue;
          bestI = i;
          bestJ = j;
        }
      }
    }
    // set the i (row) cursor and j (column) cursor to the bottom right corner

    int icursor = bestI;
    int jcursor = bestJ;
    // std::cout<<"icursor is "<<icursor<<std::endl;
    // std::cout<<"jcursor is "<<jcursor<<std::endl;

    // tracerNext holds to where to go next in the matrix, will be (D) diagonal,
    // (U) up, or (L) left depending on the maximum score determined during the
    // matrix set up.
    char tracerNext = ' ';

    // get the alignment score from the  bottom right cell and set the tacer to
    // where to go next
    // keep tracing back until at the begining of either sequence
    // Traceback algorithm follows. Score is the max of all three scores stored in
    // the bottom right cell.
    // Alignments are constructed by following the correct pointer backwards at
    // each stage.
    // Since alignment strings are constructed in reverse, we must call the
    // reverse() funcion after traceback.
    score_ = smithMaximum(ScoreMatrix_[icursor][jcursor].upInherit,
                          ScoreMatrix_[icursor][jcursor].leftInherit,
                          ScoreMatrix_[icursor][jcursor].diagInherit, tracerNext);
    double currentValue = score_;
    uint32_t gapBSize = 0;
    uint32_t gapASize = 0;
    // while (icursor!=0 || jcursor!=0 || (int)currentValue!=0) {
    while ((icursor != 0 || jcursor != 0) && currentValue != 0) {
      if (tracerNext == 'U') {
        ++gapBSize;
        tracerNext = ScoreMatrix_[icursor][jcursor].upInheritPtr;
        currentValue = ScoreMatrix_[icursor][jcursor].upInherit;
        if (currentValue != 0) {
          if (tracerNext != 'U' && tracerNext != 'B') {
            lHolder_.gapInfos_.emplace_back(
                gapInfo(jcursor, gapBSize, false));
            gapBSize = 0;
          }
          icursor--;
        }
      } else if (tracerNext == 'L') {
        ++gapASize;
        tracerNext = ScoreMatrix_[icursor][jcursor].leftInheritPtr;
        currentValue = ScoreMatrix_[icursor][jcursor].leftInherit;
        if (currentValue != 0) {
          if (tracerNext != 'L') {
            lHolder_.gapInfos_.emplace_back(
                gapInfo(icursor, gapASize, true));
            gapASize = 0;
          }
          jcursor--;
        }
      } else if (tracerNext == 'D') {
        tracerNext = ScoreMatrix_[icursor][jcursor].diagInheritPtr;
        currentValue = ScoreMatrix_[icursor][jcursor].diagInherit;
        icursor--;
        jcursor--;
        // overlaps++;
      }
      // if ambigous traceback (can go either left or up), we give precedence
      // to
      // an 'up' traceback. This will not affect the score, of course.
      else if (tracerNext == 'B') {
        ++gapBSize;
        tracerNext = ScoreMatrix_[icursor][jcursor].upInheritPtr;
        currentValue = ScoreMatrix_[icursor][jcursor].upInherit;
        if (currentValue != 0) {
          if (tracerNext != 'U' && tracerNext != 'B') {
          	lHolder_.gapInfos_.emplace_back(
                gapInfo(jcursor, gapBSize, false));
            gapBSize = 0;
          }
          icursor--;
        }
      } else {
        std::cout << "ERROR!!!!!" << std::endl;
      }
    }
    lHolder_.localAStart_ = icursor;
    lHolder_.localASize_ = bestI - icursor;
    lHolder_.localBStart_ = jcursor;
    lHolder_.localBSize_ = bestJ - jcursor;
    //rearrangeLocal(objA, objB);
    //alnInfoLocal_.addFromFile_ = false;
    //alnHolders_.localHolder_[gapScores_.getIdentifer()]
      //  .addAlnInfo(objA.seq_, objB.seq_, alnInfoLocal_);
  }

  template<typename T, typename F>
  void rearrangeGlobal(T& readA, T& readB, const F & fill) {
    for (const auto& g : gHolder_.gapInfos_) {
    	//g.printDescription(std::cout, true);
      if (g.gapInA_) {
      	readA.insert(readA.begin() + g.pos_, g.size_, fill);
      } else {
      	readB.insert(readB.begin() + g.pos_, g.size_, fill);
      }
    }
  }
  template<typename T, typename F>
  void rearrangeLocal(T& readA, T& readB, const F & fill) {
    readA = getSubVector(readA, lHolder_.localAStart_,
                     lHolder_.localASize_);
    readB = getSubVector(readB, lHolder_.localBStart_, lHolder_.localBSize_);

    for (const auto& g : lHolder_.gapInfos_) {
      if (g.gapInA_) {
        readA.insert(readA.begin() + g.pos_ - lHolder_.localAStart_, g.size_, fill);
      } else {
        readB.insert(readB.begin() + g.pos_ - lHolder_.localBStart_, g.size_, fill);
      }
    }
  }
};
