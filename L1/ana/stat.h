struct mbyear {
  std::string label;
  float nevts; // MB number of events
  float lumi; // CMS recorded luminosity
  std::string run;
  // std::string energy;
};

std::vector<mbyear> datasets { // mub-1
                              {"2010", 24.1 /*http://arxiv.org/pdf/1105.2438.pdf*/, 7.4, ""}, // http://arxiv.org/pdf/1105.2438.pdf
                              {"2011", 24.1 /*?*/, 174 /*150 https://arxiv.org/pdf/1205.0206.pdf*/, "#bf{Run 1}\n"+xjjroot::CMS::snn+"2.76 TeV"}, // ?
                              {"2015", 300., 531, ""}, //
                              {"2018", 4000., 1700, "#bf{Run 2}\n5.02 TeV"}, // AN2019_138_v8.pdf
                              {"2023", 13500, 1820, "#bf{Run 3}\n5.36 TeV"} //
};

