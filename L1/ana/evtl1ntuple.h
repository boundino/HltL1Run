#ifndef _L1TRIGGER_EVTL1NTUPLE_H_
#define _L1TRIGGER_EVTL1NTUPLE_H_

#include <TFile.h>
#include <TTree.h>

namespace l1trigger
{
  class evtl1ntuple
  {
  public:
    evtl1ntuple(TTree* nt_);
    void GetEntry(int i) { fnt->GetEntry(i); morecalculation(); }
    TTree* nt() { return fnt; }
    UInt_t          mRunNb;
    UInt_t          mLSNb;
    UInt_t          mEventNb;
    Short_t         mCenBin;
    Float_t         mVz;
    Float_t         mZDCPlus;
    Float_t         mZDCMinus;
    Float_t         mZDCRechitPlus;
    Float_t         mZDCRechitMinus;
    Float_t         mZDCDigiPlus;
    Float_t         mZDCDigiMinus;
    Int_t           mHFnhfp;
    Int_t           mHFnhfn;
    Int_t           mMaxL1HFAdcPlus;
    Int_t           mMaxL1HFAdcMinus;
    Int_t           mNpixel;
    Int_t           mNtrkoffline;
    Bool_t          mTrigHLT[12];
    Bool_t          mEvtSel[18];
    int  mMBindex;

    //
    Float_t  ZDCplus;
    Float_t  ZDCminus;
    Bool_t  ZB_HLTBit;
    Bool_t  colEvtSel;
    Bool_t  hfCoincFilter2Th4;
    Bool_t  primaryVertexFilter;
    Bool_t  clusterCompatibilityFilter;
    int  AdcAND;
    int  AdcOR;
  private:
    TTree* fnt;
    void setbranchaddress();
    void morecalculation();
  };
}

l1trigger::evtl1ntuple::evtl1ntuple(TTree* nt_) : fnt(nt_)
{
  setbranchaddress();
}

void l1trigger::evtl1ntuple::morecalculation()
{
  ZB_HLTBit                  = mTrigHLT[9];
  colEvtSel                  = mEvtSel[0];
  hfCoincFilter2Th4          = mEvtSel[1];
  primaryVertexFilter        = mEvtSel[2];
  clusterCompatibilityFilter = mEvtSel[3];

  // ZDCplus = mZDCDigiPlus;
  // ZDCminus = mZDCDigiMinus;

  ZDCplus = mZDCRechitPlus;
  ZDCminus = mZDCRechitMinus;

  AdcAND = std::min(mMaxL1HFAdcPlus, mMaxL1HFAdcMinus);
  AdcOR = std::max(mMaxL1HFAdcPlus, mMaxL1HFAdcMinus);
}

void l1trigger::evtl1ntuple::setbranchaddress()
{
  fnt->SetBranchAddress("mRunNb", &mRunNb);
  fnt->SetBranchAddress("mLSNb", &mLSNb);
  fnt->SetBranchAddress("mEventNb", &mEventNb);
  fnt->SetBranchAddress("mCenBin", &mCenBin);
  fnt->SetBranchAddress("mVz", &mVz);
  fnt->SetBranchAddress("mZDCPlus", &mZDCPlus);
  fnt->SetBranchAddress("mZDCMinus", &mZDCMinus);
  fnt->SetBranchAddress("mZDCRechitPlus", &mZDCRechitPlus);
  fnt->SetBranchAddress("mZDCRechitMinus", &mZDCRechitMinus);
  fnt->SetBranchAddress("mZDCDigiPlus", &mZDCDigiPlus);
  fnt->SetBranchAddress("mZDCDigiMinus", &mZDCDigiMinus);
  fnt->SetBranchAddress("mMaxL1HFAdcPlus", &mMaxL1HFAdcPlus);
  fnt->SetBranchAddress("mMaxL1HFAdcMinus", &mMaxL1HFAdcMinus);
  fnt->SetBranchAddress("mNpixel", &mNpixel);
  fnt->SetBranchAddress("mNtrkoffline", &mNtrkoffline);
  fnt->SetBranchAddress("mTrigHLT", mTrigHLT);
  fnt->SetBranchAddress("mEvtSel", mEvtSel);
}

#endif
