#ifndef __MAKETREE_H_
#define __MAKETREE_H_

// #include "cent/cent_MC_122X.h"
// #include "cent/cent_data.h"
#include "cent/cent_data_2022.h"
#include "zdc.h"
#include "../includes/ntbranches.h"

#define MAX_DIGI 56

class mbntuplizer
{
public:
  mbntuplizer(TTree* AdcTree, TTree* HiEvtTree, TTree* SkimTree=0, TTree* ZdcRechitTree=0, TTree* ZdcDigiTree=0, TTree* HltTree=0,
              std::vector<std::string> hltpaths = {"HLT_HIZeroBias_HighRate_v7", "HLT_HIMinimumBiasHF1AND_v7", "HLT_HIMinimumBiasHF1ANDZDC1nOR_v4", "HLT_HIMinimumBiasHF1ANDZDC2nOR_v7"});
  TTree* t;
  void calculate();
  void getentry(int j);

private:
  TTree *mAdcTree, *mHiEvtTree, *mSkimTree, *mZdcRechitTree, *mZdcDigiTree, *mHltTree;
  std::vector<std::string> mhltpaths;

  // mHiEvtTree
  UInt_t run;
  UInt_t lumi;
  ULong64_t evt;
  int hiBin;
  float hiHF;
  float vz;
  int hiNpix, hiNtracks, hiNpixelTracks;

  // mSkimTree
  int pphfCoincFilter2Th4;
  int pphfCoincFilterPF2Th4;
  int pphfCoincFilterPF2Th5;
  int pphfCoincFilterPF2Th6;
  int pphfCoincFilterPF2Th7;
  int pphfCoincFilterPF2Th8;
  int pphfCoincFilterPF2Th9;
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
  
#define SETBRANCH_WARNING(q, t, c)                                      \
  if (t->FindBranch(#q))                                                \
    t->SetBranchAddress(#q, &q);                                        \
  else                                                                  \
    std::cout<<"  \e[" #c "m(x) no branch: " #q "\e[0m"<<std::endl;     \
  
  if (mHiEvtTree) {
    std::cout<<__FUNCTION__<<" \e[32m(o) set tree: mHiEvtTree\e[0m"<<std::endl;
    mHiEvtTree->SetBranchAddress("run", &run);
    mHiEvtTree->SetBranchAddress("lumi", &lumi);
    mHiEvtTree->SetBranchAddress("evt", &evt);
    mHiEvtTree->SetBranchAddress("hiBin", &hiBin);
    mHiEvtTree->SetBranchAddress("hiHF", &hiHF);
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
    mSkimTree->SetBranchAddress("pphfCoincFilter2Th4", &pphfCoincFilter2Th4);
    mSkimTree->SetBranchAddress("pphfCoincFilterPF2Th4", &pphfCoincFilterPF2Th4);
    mSkimTree->SetBranchAddress("pphfCoincFilterPF2Th5", &pphfCoincFilterPF2Th5);
    mSkimTree->SetBranchAddress("pphfCoincFilterPF2Th6", &pphfCoincFilterPF2Th6);
    mSkimTree->SetBranchAddress("pphfCoincFilterPF2Th7", &pphfCoincFilterPF2Th7);
    mSkimTree->SetBranchAddress("pphfCoincFilterPF2Th8", &pphfCoincFilterPF2Th8);
    mSkimTree->SetBranchAddress("pphfCoincFilterPF2Th9", &pphfCoincFilterPF2Th9);
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
    br.mhiHF = hiHF;
    br.mhiBin = hiBin;
    // br.mhiBin = getHiBinFromhiHF(hiHF);
    br.mNpixel = hiNpix;
    br.mNtrkoffline = hiNtracks;
    br.mNpixelTracks = hiNpixelTracks;
  }
  else if (mAdcTree) {
    br.mhiHF = hft;
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
    br.mpphfCoincFilter2Th4 = pphfCoincFilter2Th4;
    br.mpphfCoincFilterPF2Th4 = pphfCoincFilterPF2Th4;
    br.mpphfCoincFilterPF2Th5 = pphfCoincFilterPF2Th5;
    br.mpphfCoincFilterPF2Th6 = pphfCoincFilterPF2Th6;
    br.mpphfCoincFilterPF2Th7 = pphfCoincFilterPF2Th7;
    br.mpphfCoincFilterPF2Th8 = pphfCoincFilterPF2Th8;
    br.mpphfCoincFilterPF2Th9 = pphfCoincFilterPF2Th9;
  }
  else if (mAdcTree) {
    br.mpphfCoincFilter2Th4 = nhfp > 1 && nhfn > 1;
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
