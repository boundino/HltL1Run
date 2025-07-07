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
  double mNeuZDCLow[nDirs][nNeus] =
    {
     {-1.e10, 1.1e3, 4.e3, 6.65e3},
     {-1.e10, 1.1e3, 4.e3, 6.65e3}
     // {-1.e10, 1.76e3, 6.4e3, 10.64e3},
     // {-1.e10, 1.76e3, 6.4e3, 10.64e3}
     // {-1.e10, 1.6e3, 1.92e3, 2.08e3},
     // {-1.e10, 1.6e3, 1.92e3, 2.08e3}
    };
  int MBindex = -1, nneu = 0, drawhlt = 0, Iadc = 16;
  bool ismc = false;
  float ZBrate = 1, nBunchRatio = 1, rate_min = 0, rate_max = 50;
  std::string cmstag = xjjroot::CMS::preliminary, tag = "", subtag = "";

  // std::vector<short int> cent = {0, 10, 140, 160, 180, 200};
  // int l_interest = cent.size() - 2; // 0-90%
  std::vector<short int> cent = {0, 200};
  const int ncent = cent.size() - 1;
  int l_interest = 1;

  int setconfig(xjjc::config conf) {
    // hlt
    if (conf.goodkey("MBhlt")) {
      auto MBhlt = conf["MBhlt"];
      if (xjjc::str_contains(MBhlt, "ZDC1nOR")) {
        MBindex = 3;
        nneu = 1;
        drawhlt = 1;
      } else if (xjjc::str_contains(MBhlt, "ZDC2n")) {
        MBindex = 3;
        nneu = 2;
        drawhlt = 1;
      } else if (xjjc::str_contains(MBhlt, "MinimumBiasHF_OR")) {
        MBindex = 1;
        nneu = 0;
        drawhlt = 1;
      } else if (xjjc::str_contains(MBhlt, "MinimumBiasHF_AND")) {
        MBindex = 2;
        nneu = 0;
        drawhlt = 1;
      } else {
        std::cout<<"warning: HLT not properly set: "<<MBhlt<<"."<<std::endl;
      }
    }
    // ZDC thresholds
    if (conf.goodkey("ZDCthres")) {
      auto ZDCthres = conf.vvf("ZDCthres");
      if (ZDCthres.size() == 4) {
        for (int s = 0; s<nDirs; s++) {
          for (int k = 0; k<nNeus; k++) {
            mNeuZDCLow[s][k] = ZDCthres[k];
          }
        }
      }
    }
    // ZBrate
    if (conf.goodkey("ZBrate")) {
      ZBrate = conf.vf("ZBrate");
      if (ZBrate > 0) {
        float expected = 1.e-3/*Hz->kHz*/*ZBrate*(1/7.); 
        rate_min = expected * (1-0.5);
        rate_max = expected * (1+0.8);
      }
    } else {
      std::cout<<"error: need parameter \e[1mZBrate\e[0m in config."<<std::endl;
      return 1;
    }
    // Iadc
    if (conf.goodkey("Iadc")) { Iadc = conf.vi("Iadc"); }
    // nBunchRatio
    if (conf.goodkey("nBunchRatio")) { nBunchRatio = conf.vf("nBunchRatio"); }
    // Tag
    if (conf.goodkey("Tag")) { tag = conf["Tag"]; }
    if (conf.goodkey("SubTag")) { tag = conf["SubTag"]; }
    // MC
    if (conf.goodkey("MC")) { ismc = true; cmstag = xjjroot::CMS::simulation; }

    return 0;
  }

  std::vector<float> setcent(std::vector<short int>& cent_) {
    std::vector<float> fcent_;
    for (auto& c : cent_)
      fcent_.push_back((float)c);
    return fcent_;
  }
  auto fcent = setcent(cent);

  const int nadc = 30, nbincent = 20;

}

#endif
