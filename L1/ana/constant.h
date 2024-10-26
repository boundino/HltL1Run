#include <string>
#include <vector>
#include "config.h"

namespace l1trigger
{
  const int nDirs = 2; // 0 - Plus; 1 - Minus;
  const std::string mDir[nDirs] = {"Plus", "Minus"};

  const int nNeus = 4;
  // const double mNeuZDCLow[nDirs][nNeus] = {
  //   {0, 4.2e3, 10.e3, 20.e3},
  //   {0, 6.0e3, 16.e3, 32.e3}
  // };
  // // const double mNeuZDCLow[nDirs][nNeus] = {
  //   {-1.e10, 1.1e3, 3.8e3, 7.e3},
  //   {-1.e10, 1.1e3, 3.8e3, 7.e3}
  // };
  const double mNeuZDCLow[nDirs][nNeus] = {
    {-1.e10, 1.1e3, 4.e3, 6.65e3},
    {-1.e10, 1.1e3, 4.e3, 6.65e3}
  };
  // const double mNeuZDCHi[nDirs][nNeus] = {
  //   {4.2e3, 10.e3, 20.e3, 5.e5},
  //   {6.0e3, 16.e3, 32.e3, 6.e5}
  // };

  std::vector<std::vector<short int>> cents = {{0, 200}, 
                                           {0, 10, 140, 160, 170, 180, 200}};
  std::vector<short int> cent; int ncent; std::vector<float> fcent;
  void setcent(xjjc::config& conf)
  {
    int i = conf.vi("Opt_cent");
    cent = cents[i];
    ncent = cent.size()-1;
    fcent.resize(cent.size());
    for (int j=0; j<cent.size(); j++)
      fcent[j] = (float)(cent[j]);
  }

  const int nadc = 30, nbincent = 20;
}
