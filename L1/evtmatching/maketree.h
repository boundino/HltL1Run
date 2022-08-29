#ifndef __MAKETREE_H_
#define __MAKETREE_H_

const int nMaxEvtSelBits = 18;
const int nMaxTrigHLTBits = 12;
const int nZdcRechit = 18;
const int nZdcDigi = 50;
const int nZdcTs = 10;
const int nZdcHAD = 4;

class EvtTowerInfoNTuple
{
public:
  EvtTowerInfoNTuple(TTree* AdcTree, TTree* HiEvtTree, TTree* SkimTree=0, TTree* ZdcRechitTree=0, TTree* ZdcDigiTree=0);
  TTree* t;
  void calculate();
  void getentry(int j);

private:
  TTree *mAdcTree, *mHiEvtTree, *mSkimTree, *mZdcRechitTree, *mZdcDigiTree;

  // mHiEvtTree
  UInt_t run;
  UInt_t lumi;
  ULong64_t evt;
  int hiBin;
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
  int zside_digi[nZdcDigi];
  int section_digi[nZdcDigi];
  int channel_digi[nZdcDigi];
  int adcTs[nZdcTs][nZdcDigi];

  // mAdcTree
  int m_MaxL1HFAdcPlus;
  int m_MaxL1HFAdcMinus;

  // new tree
  unsigned int mRunNb = 0;
  unsigned int mLSNb = 0;
  unsigned int mEventNb = 0;
  short        mCenBin = -1;
  float        mVx, mVy, mVz;
  float        mZDCPlus, mZDCMinus;
  float        mZDCRechitPlus, mZDCRechitMinus;
  int          mMaxL1HFAdcPlus, mMaxL1HFAdcMinus;
  int          mNpixel, mNtrkoffline, mNpixelTracks;
  Bool_t       mTrigHLT[nMaxTrigHLTBits];
  Bool_t       mEvtSel[nMaxEvtSelBits];
  int          mZDCHadPlus[nZdcHAD];
  int          mZDCHadMinus[nZdcHAD];

  void setbranches();
  void branches();
};

EvtTowerInfoNTuple::EvtTowerInfoNTuple(TTree* AdcTree, TTree* HiEvtTree, TTree* SkimTree, TTree* ZdcRechitTree, TTree* ZdcDigiTree) :
  mAdcTree(AdcTree), mHiEvtTree(HiEvtTree), mSkimTree(SkimTree), mZdcRechitTree(ZdcRechitTree), mZdcDigiTree(ZdcDigiTree)
{
  t = new TTree("EvtTowerInfoNTuple", "EvtTowerInfoNTuple");
  setbranches();
  branches();
}

void EvtTowerInfoNTuple::getentry(int j)
{
  mHiEvtTree->GetEntry(j);
  if(mSkimTree) mSkimTree->GetEntry(j);
  if(mAdcTree) mAdcTree->GetEntry(j);
  if(mZdcRechitTree) mZdcRechitTree->GetEntry(j);
  if(mZdcDigiTree) mZdcDigiTree->GetEntry(j);
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
    std::cout<<__FUNCTION__<<" \e[31m(x) no tree: mZdcDigiTree\e[0m"<<std::endl;

  if(mZdcDigiTree)
    {
      std::cout<<__FUNCTION__<<" \e[32m(o) set tree: mZdcDigiTree\e[0m"<<std::endl;
      mZdcDigiTree->SetBranchAddress("n", &n_digi);
      mZdcDigiTree->SetBranchAddress("zside", zside_digi);
      mZdcDigiTree->SetBranchAddress("section", section_digi);
      mZdcDigiTree->SetBranchAddress("channel", channel_digi);
      for(int i=0; i<nZdcTs; i++)
        mZdcDigiTree->SetBranchAddress(Form("adcTs%d", i), adcTs[i]);
    }
  else
    std::cout<<__FUNCTION__<<" \e[31m(x) no tree: mZdcDigiTree\e[0m"<<std::endl;

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
  t->Branch("mNpixel", &mNpixel, "mNpixel/I");
  t->Branch("mNtrkoffline", &mNtrkoffline, "mNtrkoffline/I");
  t->Branch("mNpixelTracks", &mNpixelTracks, "mNpixelTracks/I");
  t->Branch("mTrigHLT", mTrigHLT, Form("mTrigHLT[%d]/O", nMaxTrigHLTBits));
  t->Branch("mEvtSel", mEvtSel, Form("mEvtSel[%d]/O", nMaxEvtSelBits));
  t->Branch("mZDCHadPlus", mZDCHadPlus, Form("mZDCHadPlus[%d]/I", nZdcHAD));
  t->Branch("mZDCHadMinus", mZDCHadMinus, Form("mZDCHadMinus[%d]/I", nZdcHAD));
}

void EvtTowerInfoNTuple::calculate()
{
  mRunNb = run;
  mLSNb = lumi;
  mEventNb = evt;
  mCenBin = hiBin;
  mVx = vx;
  mVy = vy;
  mVz = vz;
  mZDCPlus = hiZDCplus;
  mZDCMinus = hiZDCminus;
  mNpixel = hiNpix;
  mNtrkoffline = hiNtracks;
  mNpixelTracks = hiNpixelTracks;

  // std::cout<<" "<<m_MaxL1HFAdcPlus<<std::endl;
  mMaxL1HFAdcPlus = m_MaxL1HFAdcPlus;
  mMaxL1HFAdcMinus = m_MaxL1HFAdcMinus;

  for(int i=0; i<nMaxTrigHLTBits; i++)
    mTrigHLT[i] = true;

  for(int i=0; i<nMaxEvtSelBits; i++)
    mEvtSel[i] = true;
  if(mSkimTree)
    {
      mEvtSel[0] = (bool)(pclusterCompatibilityFilter && pprimaryVertexFilter && pphfCoincFilter2Th4);
      mEvtSel[1] = (bool)pphfCoincFilter2Th4;
      mEvtSel[2] = (bool)pprimaryVertexFilter;
      mEvtSel[3] = (bool)pclusterCompatibilityFilter;
    }

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
  for(int i=0; i<nZdcHAD; i++)
    {
      mZDCHadPlus[i] = 0;
      mZDCHadMinus[i] = 0;
    }
  if(mZdcDigiTree)
    {
      for(int i=0; i<18; i++)
        {
          if(section_digi[i] != 2) continue;
          if(channel_digi[i] < 1 || channel_digi[i] > 4)
            { std::cout<<"error: invalid channel_digi."<<std::endl; continue; }

          int signal = adcTs[3][i]+adcTs[4][i] - 0.5*(adcTs[0][i] + adcTs[1][i]); // https://arxiv.org/abs/2102.06640
          if(zside_digi[i] == 1)
            mZDCHadPlus[channel_digi[i]-1] += signal;
          else if(zside_digi[i] == -1)
            mZDCHadMinus[channel_digi[i]-1] += signal;
          else std::cout<<"error: invalid zside_digi."<<std::endl;
        }
    }
}

#endif
