#ifndef __L1_CONSTANT_
#define __L1_CONSTANT_

#include <string>
#include <vector>
#include "config.h"

namespace l1trigger
{
  constexpr int nDirs = 2; // 0 - Plus; 1 - Minus;
  constexpr std::string mDir[nDirs] = {"Plus", "Minus"};
  constexpr int nNeus = 4;
  const int nadc = 30, nbincent = 20;

  // centrality
  // std::vector<short int> cent = {0, 200};
  std::vector<short int> cent = {0, 10, 140, 160, 180, 200};
  const int ncent = cent.size() - 1, l_incl = ncent;
  // int l_interest = l_incl;
  int l_interest = l_incl - 1; // 0-90%

  class l1const {
    /*
      [ Inputs from conf ]
      (Mandate)
      Output, Input, ZBrate
      (Optional value)
      Tag, minLS, maxLS, MBhlt, ZDCthres
      (Optional switch)
      noZDC, MC
    */
  public:
    l1const() { valid_ = false; }
    l1const(const xjjc::config &conf) { init(conf); }
    void init(const xjjc::config &conf);
    bool valid() { return valid_; }
    bool isMC() { return isMC_; }
    // ZDC
    bool hasZDC() { return hasZDC_; }
    double ZDCthre[nDirs][nNeus] =
      { // default -> better always give thresholds in configs
        {-1.e10, 1.1e3, 4.e3, 6.65e3},
        {-1.e10, 1.1e3, 4.e3, 6.65e3}
      };
    // HLT
    bool drawHLT() { return MBindex >= 0; }
    int MBindex, Ineu, Iadc;
    // rate
    float ZBrate, rate_min, rate_max;
    // data for savehist
    int minLS, maxLS;
    // Label
    std::string label;
    
    void drawtag() {
      xjjroot::drawCMS(isMC_ ? xjjroot::CMS::simulation : xjjroot::CMS::internal, tag_);
      xjjroot::drawtex(0, 0.01, subtag_.c_str(), 0.035, 11, 52, kGray+1);
    }
  private:
    bool valid_, hasZDC_, isMC_;
    // Label
    std::string tag_, subtag_;
  };
  
  std::vector<float> setcent(std::vector<short int>& cent_) {
    std::vector<float> fcent_;
    for (auto& c : cent_)
      fcent_.push_back((float)c);
    return fcent_;
  }
  auto fcent = setcent(cent);
}

void l1trigger::l1const::init(const xjjc::config &conf) {
  valid_ = true;
  
  // hlt: MBindex, Ineu
  MBindex = -1;
  Ineu = 0;
  if (conf.has("MBhlt")) {
    auto MBhlt = conf["MBhlt"];
    // const std::vector<std::string> prehltpaths = {"HLT_HIZeroBias_HighRate_v8", "HLT_HIMinimumBiasHF1AND_v8", "HLT_HIMinimumBiasHF1ANDZDC1nOR_v6", "HLT_HIMinimumBiasHF1ANDZDC2nOR_v9"};
    if (xjjc::str_contains(MBhlt, "ZDC1nOR")) {
      MBindex = 2; Ineu = 1;
    } else if (xjjc::str_contains(MBhlt, "ZDC2nOR")) {
      MBindex = 3; Ineu = 2;
    } else if (xjjc::str_contains(MBhlt, "HF1AND")) {
      MBindex = 1; Ineu = 0;
    } else {
      std::cout<<"warning: HLT not properly set: "<<MBhlt<<"."<<std::endl;
    }
  } 
  // Iadc
  Iadc = 16;
  if (conf.has("Iadc")) { Iadc = conf.get<int>("Iadc"); }

  // ZDC draw
  hasZDC_ = !conf.has("noZDC") && !isMC_;
  // ZDC thresholds
  if (conf.has("ZDCthres")) {
    auto ZDCthres = conf.get_vec<float>("ZDCthres");
    if (ZDCthres.size() == 4) {
      for (int s = 0; s<nDirs; s++) {
        for (int k = 0; k<nNeus; k++) {
          ZDCthre[s][k] = ZDCthres[k];
        }
      }
    }
  }
  // rate: [ ZBrate, rate_min, rate_max ]
  ZBrate = 1;
  rate_min = 0; rate_max = 50;
  if (conf.has("ZBrate")) {
    ZBrate = conf.get<float>("ZBrate");
    if (ZBrate > 0) {
      // float expected = 1.e-3/*Hz->kHz*/*ZBrate*(1/7.); // light ion
      float expected = 1/2.6e5*ZBrate; // PbPb
      rate_min = expected * (1-0.5);
      rate_max = expected * (1+0.8);
    }
  } else {
    std::cout<<"error: need parameter \e[1mZBrate\e[0m in config."<<std::endl;
    valid_ = false;
  }
  // LS
  minLS = -1; maxLS = -1;
  if (conf.has("minLS")) minLS = conf.get<int>("minLS");
  if (conf.has("maxLS")) maxLS = conf.get<int>("maxLS");
  // Label
  if (conf.has("Output")) label = conf.get("Output");
  else { valid_ = false; }
  // Tag
  tag_ = conf.get("Tag");
  subtag_ = conf.get("SubTag");
  // MC
  isMC_ = conf.has("MC");
}

l1trigger::l1const con;

#endif
