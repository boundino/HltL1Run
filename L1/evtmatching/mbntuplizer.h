#ifndef __MAKETREE_H_
#define __MAKETREE_H_

#include "cent/cent_data_2024.h"
#include "zdc.h"
#include "../includes/ntbranches.h"
#include "../includes/define.h"

#define MAX_DIGI 56

#define DECLARE(p) int p;
#define SETBRANCH_WARNING(q, t, c)                                      \
  if (t->FindBranch(XSTR(q))) {                                         \
    t->SetBranchStatus(XSTR(q), 1);                                     \
    t->SetBranchAddress(XSTR(q), &q);                                   \
  } else {                                                              \
    std::cout<<"  \e[" #c "m(x) no branch: " XSTR(q) "\e[0m"<<std::endl; \
  }
#define SETVALUE(q) br.XPASTE(m, q) = q;
  

const std::vector<std::string> prehltpaths = {"HLT_HIZeroBias_HighRate_v8", "HLT_HIMinimumBiasHF1AND_v8", "HLT_HIMinimumBiasHF1ANDZDC1nOR_v6", "HLT_HIMinimumBiasHF1ANDZDC2nOR_v9"};
class mbntuplizer
{
public:
  mbntuplizer(TTree* AdcTree, TTree* HiEvtTree, TTree* SkimTree=0, TTree* ZdcRechitTree=0, TTree* ZdcDigiTree=0, TTree* HltTree=0,
              std::vector<std::string> hltpaths = prehltpaths);
  TTree* t;
  void calculate();
  void getentry(int j);
  int getentries() { return mAdcTree->GetEntries(); }

  const ntbranches& getbr() { return br; }

private:
  TTree *mAdcTree, *mHiEvtTree, *mSkimTree, *mZdcRechitTree, *mZdcDigiTree, *mHltTree;
  std::vector<std::string> mhltpaths;

  // mHiEvtTree
  UInt_t run;
  UInt_t lumi;
  ULong64_t evt;
  int hiBin;
  float hiHF_pf;
  float hiHFPlus_pf;
  float hiHFMinus_pf;
  float vz;
  int hiNpix, hiNtracks, hiNpixelTracks;

  // mSkimTree
  HFCOINC(DECLARE);
  int pprimaryVertexFilter;
  int pclusterCompatibilityFilter;

  // mZdcRechitTree  
  float sumPlus;
  float sumMinus;

  // mZdcDigiTree
  int n_digi;
  int zside_digi[MAX_DIGI];
  int section_digi[MAX_DIGI];
  int channel_digi[MAX_DIGI];
  int adcTs2[MAX_DIGI];
  int adcTs1[MAX_DIGI];
  int adcTs0[MAX_DIGI];

  // mAdcTree
  int mMaxL1HFAdcPlus;
  int mMaxL1HFAdcMinus;
  int mMaxietaPlus;
  int mMaxietaMinus;
  int mMaxiphiPlus;
  int mMaxiphiMinus;
  int mMaxdepthPlus;
  int mMaxdepthMinus;
  int nhfp;
  int nhfn;
  float hft;
  float hftp;
  float hftm;

  // mHltTree
  int hlt[MAX_HLT];

  // new tree
  ntbranches br;
  
  void setbranches();
  void branches();
};

mbntuplizer::mbntuplizer(TTree* AdcTree, TTree* HiEvtTree, TTree* SkimTree, TTree* ZdcRechitTree, TTree* ZdcDigiTree, TTree* HltTree, std::vector<std::string> hltpath) :
  mAdcTree(AdcTree), mHiEvtTree(HiEvtTree), mSkimTree(SkimTree), mZdcRechitTree(ZdcRechitTree), mZdcDigiTree(ZdcDigiTree), mHltTree(HltTree), mhltpaths(hltpath) {

  t = new TTree("mbnt", "mbnt");
  setbranches();
  branches();
}

void mbntuplizer::getentry(int j) {
  
  if(mHiEvtTree) mHiEvtTree->GetEntry(j);
  if(mSkimTree) mSkimTree->GetEntry(j);
  if(mAdcTree) mAdcTree->GetEntry(j);
  if(mZdcRechitTree) mZdcRechitTree->GetEntry(j);
  if(mZdcDigiTree) mZdcDigiTree->GetEntry(j);
  if(mHltTree) mHltTree->GetEntry(j);
}

void mbntuplizer::setbranches() {
  nt_cleanbranch(br);
  
  if (mHiEvtTree) {
    std::cout<<__FUNCTION__<<" \e[32m(o) set tree: mHiEvtTree\e[0m"<<std::endl;
    mHiEvtTree->SetBranchAddress("run", &run);
    mHiEvtTree->SetBranchAddress("lumi", &lumi);
    mHiEvtTree->SetBranchAddress("evt", &evt);
    mHiEvtTree->SetBranchAddress("hiBin", &hiBin);
    mHiEvtTree->SetBranchAddress("hiHF_pf", &hiHF_pf);
    mHiEvtTree->SetBranchAddress("hiHFPlus_pf", &hiHFPlus_pf);
    mHiEvtTree->SetBranchAddress("hiHFMinus_pf", &hiHFMinus_pf);
    mHiEvtTree->SetBranchAddress("vz", &vz);
    mHiEvtTree->SetBranchAddress("hiNpix", &hiNpix);
    mHiEvtTree->SetBranchAddress("hiNtracks", &hiNtracks);
    mHiEvtTree->SetBranchAddress("hiNpixelTracks", &hiNpixelTracks);
  }
  else
    std::cout<<__FUNCTION__<<" \e[31m(x) no tree: mHiEvtTree\e[0m"<<std::endl;

  if (mSkimTree) {
    std::cout<<__FUNCTION__<<" \e[32m(o) set tree: mSkimTree\e[0m"<<std::endl;
    mSkimTree->SetBranchAddress("pprimaryVertexFilter", &pprimaryVertexFilter);
    mSkimTree->SetBranchAddress("pclusterCompatibilityFilter", &pclusterCompatibilityFilter);

    HFCOINC3D(SETBRANCH_WARNING, mSkimTree, 31);
  }
  else
    std::cout<<__FUNCTION__<<" \e[31m(x) no tree: mSkimTree\e[0m"<<std::endl;

  if (mAdcTree) {
    std::cout<<__FUNCTION__<<" \e[32m(o) set tree: mAdcTree\e[0m"<<std::endl;
    SETBRANCH_WARNING(mMaxL1HFAdcPlus, mAdcTree, 31)
      SETBRANCH_WARNING(mMaxL1HFAdcMinus, mAdcTree, 31)
      SETBRANCH_WARNING(mMaxietaPlus, mAdcTree, 31)
      SETBRANCH_WARNING(mMaxietaMinus, mAdcTree, 31)
      SETBRANCH_WARNING(mMaxiphiPlus, mAdcTree, 31)
      SETBRANCH_WARNING(mMaxiphiMinus, mAdcTree, 31)
      SETBRANCH_WARNING(mMaxdepthPlus, mAdcTree, 31)
      SETBRANCH_WARNING(mMaxdepthMinus, mAdcTree, 31)
      SETBRANCH_WARNING(nhfp, mAdcTree, 2)
      SETBRANCH_WARNING(nhfn, mAdcTree, 2)
      SETBRANCH_WARNING(hft, mAdcTree, 2)
      }
  else
    std::cout<<__FUNCTION__<<" \e[31m(x) no tree: mAdcTree\e[0m"<<std::endl;

  if (mZdcRechitTree) {
    std::cout<<__FUNCTION__<<" \e[32m(o) set tree: mZdcRechitTree\e[0m"<<std::endl;
    SETBRANCH_WARNING(sumPlus, mZdcRechitTree, 31)
      SETBRANCH_WARNING(sumMinus, mZdcRechitTree, 31)
      }
  else
    std::cout<<__FUNCTION__<<" \e[31m(x) no tree: mZdcRechitTree\e[0m"<<std::endl;

  if (mZdcDigiTree) {
    std::cout<<__FUNCTION__<<" \e[32m(o) set tree: mZdcDigiTree\e[0m"<<std::endl;
    mZdcDigiTree->SetBranchAddress("n", &n_digi);
    mZdcDigiTree->SetBranchAddress("zside", zside_digi);
    mZdcDigiTree->SetBranchAddress("section", section_digi);
    mZdcDigiTree->SetBranchAddress("channel", channel_digi);
    mZdcDigiTree->SetBranchAddress("adcTs2", adcTs2);
    mZdcDigiTree->SetBranchAddress("adcTs1", adcTs1);
    mZdcDigiTree->SetBranchAddress("adcTs0", adcTs0);
  }
  else
    std::cout<<__FUNCTION__<<" \e[31m(x) no tree: mZdcDigiTree\e[0m"<<std::endl;

  for (int i=0; i<mhltpaths.size(); i++) { hlt[i] = 1; }
  if (mHltTree) {
    std::cout<<__FUNCTION__<<" \e[32m(o) set tree: mHltTree\e[0m"<<std::endl;
    mHltTree->SetBranchStatus("*", 0);
    for (int i=0; i<mhltpaths.size(); i++) {
      if (mhltpaths[i] == "") {
        hlt[i] = 0;
        continue;
      }
      mHltTree->SetBranchStatus(mhltpaths[i].c_str(), 1);
      mHltTree->SetBranchAddress(mhltpaths[i].c_str(), &(hlt[i]));
    }
  }
  else
    std::cout<<__FUNCTION__<<" \e[31m(x) no tree: mHltTree\e[0m"<<std::endl;
}

void mbntuplizer::branches() {
  nt_branch(t, br);
}

void mbntuplizer::calculate() {

  if (mHiEvtTree) {
    br.mRun = run;
    br.mLS = lumi;
    br.mEvent = evt;
    br.mhiHF = hiHF_pf;
    br.mhiHFPlus = hiHFPlus_pf;
    br.mhiHFMinus = hiHFMinus_pf;
    // br.mhiBin = hiBin;
    br.mhiBin = getHiBinFromhiHF(hiHF_pf);
    br.mNpixel = hiNpix;
    br.mNtrkoffline = hiNtracks;
    br.mNpixelTracks = hiNpixelTracks;
  }
  else if (mAdcTree) { // if no event tree
    br.mhiHF = hft;
    br.mhiHFPlus = hftp;
    br.mhiHFMinus = hftm;
    br.mhiBin = getHiBinFromhiHF(hft);
  }
  
  if (mAdcTree) {
    br.mMaxL1HFAdcPlus = mMaxL1HFAdcPlus;
    br.mMaxL1HFAdcMinus = mMaxL1HFAdcMinus;
    br.mMaxietaPlus = mMaxietaPlus;
    br.mMaxietaMinus = mMaxietaMinus;
    br.mMaxiphiPlus = mMaxiphiPlus;
    br.mMaxiphiMinus = mMaxiphiMinus;
    br.mMaxdepthPlus = mMaxdepthPlus;
    br.mMaxdepthMinus = mMaxdepthMinus;
  }
  
  if (mHltTree) {
    for (int i=0; i<MAX_HLT; i++) {
      br.mTrigHLT[i] = hlt[i];
    }
  }

  if (mSkimTree) {
    br.mpprimaryVertexFilter = pprimaryVertexFilter;
    br.mpclusterCompatibilityFilter = pclusterCompatibilityFilter;
    HFCOINC(SETVALUE);
  }
  else if (mAdcTree) {
    br.mpphfCoincFilterPF2Th4 = nhfp > 1 && nhfn > 1;
  }
  // br.mEvtSel[0] = br.mEvtSel[1] && br.mEvtSel[2] && br.mEvtSel[3];

  // ZDC rechit
  if (mZdcRechitTree) {
    br.mzdcsumPlus = sumPlus;
    br.mzdcsumMinus = sumMinus;
  }

  // ZDC digi
  // br.mzdcsumPlus_digi= 0;
  // br.mzdcsumMinus_digi = 0;
  // if (mZdcDigiTree) {
  //   getsum(adcTs2, adcTs1, br.mzdcsumMinus_digi, br.mzdcsumPlus_digi);
  // }
}

#endif
