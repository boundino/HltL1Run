#ifndef __MAKETREE_H_
#define __MAKETREE_H_

// #include "cent/cent_MC_122X.h"
// #include "cent/cent_data.h"
#include "cent/cent_data_2022.h"
#include "zdc.h"

const int nMaxEvtSelBits = 18;
const int nMaxTrigHLTBits = 12;
const int nZdcRechit = 18;
const int nZdcDigi = 56;
const int nZdcTs = 10;
const int nZdcHAD = 4;

const std::vector<std::string> hltpaths = {"HLT_HIMinimumBiasHF1AND_v3", "HLT_HIMinimumBiasHF1ANDZDC1nOR_v1", "HLT_HIMinimumBiasHF1ANDZDC2nOR_v3", "", "",
                                           "", "", "", "", "HLT_HIZeroBias_HighRate_v3",
                                           "", ""};

class EvtTowerInfoNTuple
{
public:
  EvtTowerInfoNTuple(TTree* AdcTree, TTree* HiEvtTree, TTree* SkimTree=0, TTree* ZdcRechitTree=0, TTree* ZdcDigiTree=0, TTree* HltTree=0);
  TTree* t;
  void calculate();
  void getentry(int j);

private:
  TTree *mAdcTree, *mHiEvtTree, *mSkimTree, *mZdcRechitTree, *mZdcDigiTree, *mHltTree;

  // mHiEvtTree
  UInt_t run;
  UInt_t lumi;
  ULong64_t evt;
  int hiBin;
  float hiHF;
  float vx, vy, vz;
  float hiZDCplus, hiZDCminus;
  int hiNpix, hiNtracks, hiNpixelTracks;

  // mSkimTree
  int pphfCoincFilter2Th4;
  int pprimaryVertexFilter;
  int pclusterCompatibilityFilter;

  // mZdcRechitTree
  int n_rec;
  int zside_rec[nZdcRechit];
  int section_rec[nZdcRechit];
  int channel_rec[nZdcRechit];
  float e_rec[nZdcRechit];

  // mZdcDigiTree
  int n_digi;
  // int zside_digi[nZdcDigi];
  // int section_digi[nZdcDigi];
  // int channel_digi[nZdcDigi];
  int adcTs2[nZdcDigi];
  int adcTs1[nZdcDigi];
  // float chargefCTs5[nZdcDigi];
  // float chargefCTs4[nZdcDigi];
  // float chargefCTs3[nZdcDigi];
  float chargefCTs2[nZdcDigi];
  float chargefCTs1[nZdcDigi];
  // float chargefCTs0[nZdcDigi];

  // mAdcTree
  int m_MaxL1HFAdcPlus = 0;
  int m_MaxL1HFAdcMinus = 0;
  int nhfp;
  int nhfn;
  float hft;
  float hftp;
  float hftm;

  // mHltTree
  int m_hlt[nMaxTrigHLTBits];

  // new tree
  unsigned int mRunNb = 0;
  unsigned int mLSNb = 0;
  unsigned int mEventNb = 0;
  short        mCenBin = -1;
  float        mVx, mVy, mVz;
  float        mZDCPlus, mZDCMinus;
  float        mZDCRechitPlus, mZDCRechitMinus;
  float        mZDCDigiPlus, mZDCDigiMinus;
  float        mZDCDigifromADCPlus, mZDCDigifromADCMinus;
  int          mHFnhfp, mHFnhfn;
  float        mHFhft, mHFhftp, mHFhftm;
  int          mMaxL1HFAdcPlus = 0, mMaxL1HFAdcMinus = 0;
  int          mNpixel, mNtrkoffline, mNpixelTracks;
  Bool_t       mTrigHLT[nMaxTrigHLTBits];
  Bool_t       mEvtSel[nMaxEvtSelBits];

  void setbranches();
  void branches();
};

EvtTowerInfoNTuple::EvtTowerInfoNTuple(TTree* AdcTree, TTree* HiEvtTree, TTree* SkimTree, TTree* ZdcRechitTree, TTree* ZdcDigiTree, TTree* HltTree) :
  mAdcTree(AdcTree), mHiEvtTree(HiEvtTree), mSkimTree(SkimTree), mZdcRechitTree(ZdcRechitTree), mZdcDigiTree(ZdcDigiTree), mHltTree(HltTree)
{
  t = new TTree("EvtTowerInfoNTuple", "EvtTowerInfoNTuple");
  setbranches();
  branches();
}

void EvtTowerInfoNTuple::getentry(int j)
{
  if(mHiEvtTree) mHiEvtTree->GetEntry(j);
  if(mSkimTree) mSkimTree->GetEntry(j);
  if(mAdcTree) mAdcTree->GetEntry(j);
  if(mZdcRechitTree) mZdcRechitTree->GetEntry(j);
  if(mZdcDigiTree) mZdcDigiTree->GetEntry(j);
  if(mHltTree) mHltTree->GetEntry(j);
}  

void EvtTowerInfoNTuple::setbranches()
{
  if(mHiEvtTree)
    {
      std::cout<<__FUNCTION__<<" \e[32m(o) set tree: mHiEvtTree\e[0m"<<std::endl;
      mHiEvtTree->SetBranchAddress("run", &run);
      mHiEvtTree->SetBranchAddress("lumi", &lumi);
      mHiEvtTree->SetBranchAddress("evt", &evt);
      mHiEvtTree->SetBranchAddress("hiBin", &hiBin);
      mHiEvtTree->SetBranchAddress("hiHF", &hiHF);
      mHiEvtTree->SetBranchAddress("vx", &vx);
      mHiEvtTree->SetBranchAddress("vy", &vy);
      mHiEvtTree->SetBranchAddress("vz", &vz);
      mHiEvtTree->SetBranchAddress("hiZDCplus", &hiZDCplus);
      mHiEvtTree->SetBranchAddress("hiZDCminus", &hiZDCminus);
      mHiEvtTree->SetBranchAddress("hiNpix", &hiNpix);
      mHiEvtTree->SetBranchAddress("hiNtracks", &hiNtracks);
      mHiEvtTree->SetBranchAddress("hiNpixelTracks", &hiNpixelTracks);
    }
  else
    std::cout<<__FUNCTION__<<" \e[31m(x) no tree: mHiEvtTree\e[0m"<<std::endl;

  if(mSkimTree)
    {
      std::cout<<__FUNCTION__<<" \e[32m(o) set tree: mSkimTree\e[0m"<<std::endl;
      mSkimTree->SetBranchAddress("pphfCoincFilter2Th4", &pphfCoincFilter2Th4);
      mSkimTree->SetBranchAddress("pprimaryVertexFilter", &pprimaryVertexFilter);
      mSkimTree->SetBranchAddress("pclusterCompatibilityFilter", &pclusterCompatibilityFilter);
    }
  else
    std::cout<<__FUNCTION__<<" \e[31m(x) no tree: mSkimTree\e[0m"<<std::endl;

  if(mAdcTree)
    {
      std::cout<<__FUNCTION__<<" \e[32m(o) set tree: mAdcTree\e[0m"<<std::endl;
      mAdcTree->SetBranchAddress("mMaxL1HFAdcPlus", &m_MaxL1HFAdcPlus);
      mAdcTree->SetBranchAddress("mMaxL1HFAdcMinus", &m_MaxL1HFAdcMinus);
      mAdcTree->SetBranchAddress("nhfp", &nhfp);
      mAdcTree->SetBranchAddress("nhfn", &nhfn);
      mAdcTree->SetBranchAddress("hftp", &hftp);
      mAdcTree->SetBranchAddress("hftm", &hftm);
      mAdcTree->SetBranchAddress("hft", &hft);
    }
  else
    std::cout<<__FUNCTION__<<" \e[31m(x) no tree: mAdcTree\e[0m"<<std::endl;

  if(mZdcRechitTree)
    {
      std::cout<<__FUNCTION__<<" \e[32m(o) set tree: mZdcRechitTree\e[0m"<<std::endl;
      mZdcRechitTree->SetBranchAddress("n", &n_rec);
      mZdcRechitTree->SetBranchAddress("zside", zside_rec);
      mZdcRechitTree->SetBranchAddress("section", section_rec);
      mZdcRechitTree->SetBranchAddress("channel", channel_rec);
      mZdcRechitTree->SetBranchAddress("e", e_rec);
    }
  else
    std::cout<<__FUNCTION__<<" \e[31m(x) no tree: mZdcRechitTree\e[0m"<<std::endl;

  if(mZdcDigiTree)
    {
      std::cout<<__FUNCTION__<<" \e[32m(o) set tree: mZdcDigiTree\e[0m"<<std::endl;
      mZdcDigiTree->SetBranchAddress("n", &n_digi);
      // mZdcDigiTree->SetBranchAddress("zside", zside_digi);
      // mZdcDigiTree->SetBranchAddress("section", section_digi);
      // mZdcDigiTree->SetBranchAddress("channel", channel_digi);
      mZdcDigiTree->SetBranchAddress("adcTs2", adcTs2);
      mZdcDigiTree->SetBranchAddress("adcTs1", adcTs1);
      // mZdcDigiTree->SetBranchAddress("chargefCTs5", chargefCTs5);
      // mZdcDigiTree->SetBranchAddress("chargefCTs4", chargefCTs4);
      // mZdcDigiTree->SetBranchAddress("chargefCTs3", chargefCTs3);
      mZdcDigiTree->SetBranchAddress("chargefCTs2", chargefCTs2);
      mZdcDigiTree->SetBranchAddress("chargefCTs1", chargefCTs1);
      // mZdcDigiTree->SetBranchAddress("chargefCTs0", chargefCTs0);
    }
  else
    std::cout<<__FUNCTION__<<" \e[31m(x) no tree: mZdcDigiTree\e[0m"<<std::endl;

  for (int i=0; i<hltpaths.size(); i++) {
    m_hlt[i] = 1;
  }
  if(mHltTree)
    {
      std::cout<<__FUNCTION__<<" \e[32m(o) set tree: mHltTree\e[0m"<<std::endl;
      mHltTree->SetBranchStatus("*", 0);
      for (int i=0; i<hltpaths.size(); i++) {
        if (hltpaths[i] == "") {
          m_hlt[i] = 0;
          continue;
        }
        mHltTree->SetBranchStatus(hltpaths[i].c_str(), 1);
        mHltTree->SetBranchAddress(hltpaths[i].c_str(), &(m_hlt[i]));
      }
    }
  else
    std::cout<<__FUNCTION__<<" \e[31m(x) no tree: mHltTree\e[0m"<<std::endl;

}

void EvtTowerInfoNTuple::branches()
{
  t->Branch("mRunNb", &mRunNb, "mRunNb/i");
  t->Branch("mLSNb", &mLSNb, "mLSNb/i");
  t->Branch("mEventNb", &mEventNb, "mEventNb/i");
  t->Branch("mCenBin", &mCenBin, "mCenBin/S");
  t->Branch("mVx", &mVx, "mVx/F");
  t->Branch("mVy", &mVy, "mVy/F");
  t->Branch("mVz", &mVz, "mVz/F");
  t->Branch("mZDCPlus", &mZDCPlus, "mZDCPlus/F");
  t->Branch("mZDCMinus", &mZDCMinus, "mZDCMinus/F");
  t->Branch("mZDCRechitPlus", &mZDCRechitPlus, "mZDCRechitPlus/F");
  t->Branch("mZDCRechitMinus", &mZDCRechitMinus, "mZDCRechitMinus/F");
  t->Branch("mMaxL1HFAdcPlus", &mMaxL1HFAdcPlus, "mMaxL1HFAdcPlus/I");
  t->Branch("mMaxL1HFAdcMinus", &mMaxL1HFAdcMinus, "mMaxL1HFAdcMinus/I");
  t->Branch("mHFnhfp", &mHFnhfp, "mHFnhfp/I");
  t->Branch("mHFnhfn", &mHFnhfn, "mHFnhfn/I");
  t->Branch("mHFhftp", &mHFhftp, "mHFhftp/F");
  t->Branch("mHFhftm", &mHFhftm, "mHFhftm/F");
  t->Branch("mHFhft", &mHFhft, "mHFhft/F");
  t->Branch("mNpixel", &mNpixel, "mNpixel/I");
  t->Branch("mNtrkoffline", &mNtrkoffline, "mNtrkoffline/I");
  t->Branch("mNpixelTracks", &mNpixelTracks, "mNpixelTracks/I");
  t->Branch("mTrigHLT", mTrigHLT, Form("mTrigHLT[%d]/O", nMaxTrigHLTBits));
  t->Branch("mEvtSel", mEvtSel, Form("mEvtSel[%d]/O", nMaxEvtSelBits));
  t->Branch("mZDCDigiPlus", &mZDCDigiPlus, "mZDCDigiPlus/F");
  t->Branch("mZDCDigiMinus", &mZDCDigiMinus, "mZDCDigiMinus/F");
  t->Branch("mZDCDigifromADCPlus", &mZDCDigifromADCPlus, "mZDCDigifromADCPlus/F");
  t->Branch("mZDCDigifromADCMinus", &mZDCDigifromADCMinus, "mZDCDigifromADCMinus/F");
}

void EvtTowerInfoNTuple::calculate()
{
  if(mHiEvtTree)
    {
      mRunNb = run;
      mLSNb = lumi;
      mEventNb = evt;
      // mCenBin = hiBin;
      mCenBin = getHiBinFromhiHF(hiHF);
      mVx = vx;
      mVy = vy;
      mVz = vz;
      mZDCPlus = hiZDCplus;
      mZDCMinus = hiZDCminus;
      if (hiZDCplus<0) mZDCPlus = 0;
      if (hiZDCminus<0) mZDCMinus = 0;
      mNpixel = hiNpix;
      mNtrkoffline = hiNtracks;
      mNpixelTracks = hiNpixelTracks;
    }
  else if(mAdcTree)
    mCenBin = getHiBinFromhiHF(hft);

  if(mAdcTree)
    {
      mMaxL1HFAdcPlus = m_MaxL1HFAdcPlus;
      mMaxL1HFAdcMinus = m_MaxL1HFAdcMinus;
    }

  for (int i=0; i<hltpaths.size(); i++) {
    mTrigHLT[i] = (bool)(m_hlt[i]);
  }
  // for(int i=0; i<nMaxTrigHLTBits; i++)
  //   mTrigHLT[i] = true;

  for(int i=0; i<nMaxEvtSelBits; i++)
    mEvtSel[i] = true;
  if(mSkimTree)
    {
      mEvtSel[1] = (bool)pphfCoincFilter2Th4;
      mEvtSel[2] = (bool)pprimaryVertexFilter;
      mEvtSel[3] = (bool)pclusterCompatibilityFilter;
    }
  else if(mAdcTree)
    {
      mEvtSel[1] = nhfp > 1 && nhfn > 1;
    }
  mEvtSel[0] = mEvtSel[1] && mEvtSel[2] && mEvtSel[3];

  // ZDC rechit
  mZDCRechitPlus = 0;
  mZDCRechitMinus = 0;
  if(mZdcRechitTree)
    {
      for(int i=0; i<nZdcRechit; i++)
        {
          float ei = e_rec[i];
          if(zside_rec[i] == 1) mZDCRechitPlus += ei;
          else if(zside_rec[i] == -1) mZDCRechitMinus += ei;
          else std::cout<<"error: invalid zside_rec."<<std::endl;
        }
    }

  // ZDC digi
  mZDCDigiPlus= 0;
  mZDCDigiMinus = 0;
  mZDCDigifromADCPlus= 0;
  mZDCDigifromADCMinus = 0;
  if(mZdcDigiTree)
    {
      getsum(adcTs2, adcTs1, mZDCDigifromADCMinus, mZDCDigifromADCPlus);
      getsumch(chargefCTs2, chargefCTs1, mZDCDigiMinus, mZDCDigiPlus);
      
      // for(int i=0; i<18; i++)
      //   {
      //     if(section_digi[i] != 2) continue;
      //     if(channel_digi[i] < 1 || channel_digi[i] > 4)
      //       { std::cout<<"error: invalid channel_digi."<<std::endl; continue; }

      //     int signal = adcTs[3][i]+adcTs[4][i] - 0.5*(adcTs[0][i] + adcTs[1][i]); // https://arxiv.org/abs/2102.06640
      //     if(zside_digi[i] == 1)
      //       mZDCHadPlus[channel_digi[i]-1] += signal;
      //     else if(zside_digi[i] == -1)
      //       mZDCHadMinus[channel_digi[i]-1] += signal;
      //     else std::cout<<"error: invalid zside_digi."<<std::endl;
      //   }
    }
}

#endif
