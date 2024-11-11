#ifndef __L1_CONSTANT_
#define __L1_CONSTANT_

#include <string>
#include <vector>
#include "config.h"

namespace l1trigger
{
  const int nDirs = 2; // 0 - Plus; 1 - Minus;
  const std::string mDir[nDirs] = {"Plus", "Minus"};

  const int nNeus = 4;
  const double mNeuZDCLow[nDirs][nNeus] =
    { {-1.e10, 1.1e3, 4.e3, 6.65e3},
      {-1.e10, 1.1e3, 4.e3, 6.65e3}
    };

  // std::vector<std::vector<short int>> cents = {{0, 200}, 
  //                                              {0, 10, 140, 160, 180, 200}};
  std::vector<short int> cent = {0, 10, 140, 160, 180, 200};
  const int ncent = cent.size() - 1;
  std::vector<float> fcent(cent.size());
  void setcent() {
    for (int j=0; j<cent.size(); j++)
      fcent[j] = (float)(cent[j]);
  }

  int MBindex, nneu, drawhlt;
  void sethlt(std::string MBhlt) {
    MBindex = -1;
    nneu = 0;
    drawhlt = 1;
    if (MBhlt != "") {
      if (xjjc::str_contains(MBhlt, "ZDC1n")) {
        MBindex = 2;
        nneu = 1;
      } else if (xjjc::str_contains(MBhlt, "ZDC2n")) {
        MBindex = 3;
        nneu = 2;
      } else if (xjjc::str_contains(MBhlt, "MinimumBiasHF1")) {
        MBindex = 1;
        nneu = 0;
      } else {
        drawhlt = 0;
        std::cout<<"warning: HLT not properly set: "<<MBhlt<<"."<<std::endl;
      }
    } else {
      drawhlt = 0;
    }
  }

  const int nadc = 30, nbincent = 20;

  float rate_min = 0, rate_max = 50;
  void setraterange(float nBunch) {
    if (nBunch > 0) {
      float expected = 2/53.*nBunch;
      rate_min = expected * (1-0.4);
      rate_max = expected * (1+0.4);
    }
  }
}

#endif
