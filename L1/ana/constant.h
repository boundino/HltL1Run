#include <string>
#include <vector>

namespace l1trigger
{
  const int nDirs = 2; // 0 - Plus; 1 - Minus;
  const std::string mDir[nDirs] = {"Plus", "Minus"};

  const int nNeus = 4;
  // const double mNeuZDCLow[nDirs][nNeus] = {
  //   {0, 4.2e3, 10.e3, 20.e3},
  //   {0, 6.0e3, 16.e3, 32.e3}
  // };
  const double mNeuZDCLow[nDirs][nNeus] = {
    {0, 1.5e3, 3.8e3, 7.e3},
    {0, 1.3e3, 3.8e3, 7.e3}
  };
  // const double mNeuZDCHi[nDirs][nNeus] = {
  //   {4.2e3, 10.e3, 20.e3, 5.e5},
  //   {6.0e3, 16.e3, 32.e3, 6.e5}
  // };

  std::vector<short int> cent = {0, 10, 140, 160, 180, 200};
  int ncent = cent.size()-1;

  const int nadc = 30, nbincent = 20;
}
