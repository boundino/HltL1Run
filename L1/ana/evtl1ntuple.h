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
    bool HLT_ZB;
    bool colEvtSel;
    bool hfCoincFilter2Th4;
    bool primaryVertexFilter;
    bool clusterCompatibilityFilter;
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
  colEvtSel                  = br.mEvtSel[0];
  hfCoincFilter2Th4          = br.mEvtSel[1];
  primaryVertexFilter        = br.mEvtSel[2];
  clusterCompatibilityFilter = br.mEvtSel[3];

  ZDCplus = br.mzdcsumPlus;
  ZDCminus = br.mzdcsumMinus;

  AdcAND = std::min(br.mMaxL1HFAdcPlus, br.mMaxL1HFAdcMinus);
  AdcOR = std::max(br.mMaxL1HFAdcPlus, br.mMaxL1HFAdcMinus);

}

void l1trigger::evtl1ntuple::setbranchaddress() {
  nt_setbranchaddress(fnt, br);
}

#endif
