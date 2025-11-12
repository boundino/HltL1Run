#ifndef _L1TRIGGER_EVTL1NTUPLE_H_
#define _L1TRIGGER_EVTL1NTUPLE_H_

#include <TFile.h>
#include <TTree.h>
#include "../includes/ntbranches.h"

namespace l1trigger
{
  class evtl1ntuple
  {
  public:
    evtl1ntuple(TTree* nt_);
    void GetEntry(int i) { fnt->GetEntry(i); morecalculation(); }
    TTree* nt() { return fnt; }
    //
    float ZDCplus;
    float ZDCminus;
    int HLT_ZB;
    bool colEvtSel;
    int AdcAND;
    int AdcOR;

    ntbranches br;
    
  private:
    TTree* fnt;

    void setbranchaddress();
    void morecalculation();
  };
}

l1trigger::evtl1ntuple::evtl1ntuple(TTree* nt_) : fnt(nt_) {
  setbranchaddress();
}

void l1trigger::evtl1ntuple::morecalculation() {
  HLT_ZB                     = br.mTrigHLT[0];
  colEvtSel                  = (bool)(br.mpphfCoincFilterPF2Th7 && br.mpprimaryVertexFilter && br.mpclusterCompatibilityFilter);
  // colEvtSel                  = (bool)(br.mpprimaryVertexFilter && br.mpclusterCompatibilityFilter);

  ZDCplus = br.mzdcsumPlus;
  ZDCminus = br.mzdcsumMinus;

  AdcAND = std::min(br.mMaxL1HFAdcPlus, br.mMaxL1HFAdcMinus);
  AdcOR = std::max(br.mMaxL1HFAdcPlus, br.mMaxL1HFAdcMinus);

}

void l1trigger::evtl1ntuple::setbranchaddress() {
  nt_setbranchaddress(fnt, br);
}

#endif
