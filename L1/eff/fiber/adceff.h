#include <TH2F.h>
#include "xjjrootuti.h"

namespace adceff
{
  const int nbin = 40, nbinfib = 70;

  int FGthre[] = {12, 15, 19};
  const int nFGthre = sizeof(FGthre) / sizeof(FGthre[0]);

  int hiBins[] = {0, 60, 120, 140, 160, 180, 200};
  const int nhiBins = sizeof(hiBins) / sizeof(hiBins[0]) - 1;

  void drawthre(TH2F* hempty)
  {
    for(auto& x : FGthre)
      xjjroot::drawbox(x, hempty->GetYaxis()->GetXmin(), x+1, 1);
  }
}
