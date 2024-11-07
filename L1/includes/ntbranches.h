
#define MAX_EVTSEL 5
#define MAX_HLT 4
// HLT_HIZeroBias_HighRate_v, HLT_HIMinimumBiasHF1AND_v, HLT_HIMinimumBiasHF1ANDZDC1nOR_v, HLT_HIMinimumBiasHF1ANDZDC2nOR_v
#define STR_HELPER(x) #x
#define STR(x) STR_HELPER(x)

struct ntbranches {
  unsigned int mRun;
  unsigned int mLS;
  unsigned int mEvent;
  float        mhiHF;
  short        mhiBin;
  float        mzdcsumPlus, mzdcsumMinus;
  int          mMaxL1HFAdcPlus, mMaxL1HFAdcMinus;
  int          mMaxietaPlus, mMaxietaMinus;
  int          mMaxiphiPlus, mMaxiphiMinus;
  int          mMaxdepthPlus, mMaxdepthMinus;
  int          mNpixel, mNtrkoffline, mNpixelTracks;
  bool         mTrigHLT[MAX_HLT];
  bool         mEvtSel[MAX_EVTSEL];
};

void nt_branch(TTree* t, ntbranches& br) {
  t->Branch("mRun", &br.mRun, "mRun/i");
  t->Branch("mLS", &br.mLS, "mLS/i");
  t->Branch("mEvent", &br.mEvent, "mEvent/i");
  t->Branch("mhiHF", &br.mhiHF, "mhiHF/F");
  t->Branch("mhiBin", &br.mhiBin, "mhiBin/S");
  t->Branch("mzdcsumPlus", &br.mzdcsumPlus, "mzdcsumPlus/F");
  t->Branch("mzdcsumMinus", &br.mzdcsumMinus, "mzdcsumMinus/F");
  t->Branch("mMaxL1HFAdcPlus", &br.mMaxL1HFAdcPlus, "mMaxL1HFAdcPlus/I");
  t->Branch("mMaxL1HFAdcMinus", &br.mMaxL1HFAdcMinus, "mMaxL1HFAdcMinus/I");
  t->Branch("mMaxietaPlus", &br.mMaxietaPlus, "mMaxietaPlus/I");
  t->Branch("mMaxietaMinus", &br.mMaxietaMinus, "mMaxietaMinus/I");
  t->Branch("mMaxiphiPlus", &br.mMaxiphiPlus, "mMaxiphiPlus/I");
  t->Branch("mMaxiphiMinus", &br.mMaxiphiMinus, "mMaxiphiMinus/I");
  t->Branch("mMaxdepthPlus", &br.mMaxdepthPlus, "mMaxdepthPlus/I");
  t->Branch("mMaxdepthMinus", &br.mMaxdepthMinus, "mMaxdepthMinus/I");
  t->Branch("mNpixel", &br.mNpixel, "mNpixel/I");
  t->Branch("mNtrkoffline", &br.mNtrkoffline, "mNtrkoffline/I");
  t->Branch("mNpixelTracks", &br.mNpixelTracks, "mNpixelTracks/I");
  // t->Branch("mTrigHLT", br.mTrigHLT, Form("mTrigHLT[%d]/O", MAX_HLT));
  // t->Branch("mEvtSel", br.mEvtSel, Form("mEvtSel[%d]/O", MAX_EVTSEL));
  t->Branch("mTrigHLT", br.mTrigHLT, "mTrigHLT[" STR(MAX_HLT) "]/O");
  t->Branch("mEvtSel", br.mEvtSel, "mEvtSel[" STR(MAX_EVTSEL) "]/O");
}

void nt_setbranchaddress(TTree* t, ntbranches& br) {
  t->SetBranchAddress("mRun", &br.mRun);
  t->SetBranchAddress("mLS", &br.mLS);
  t->SetBranchAddress("mEvent", &br.mEvent);
  t->SetBranchAddress("mhiHF", &br.mhiHF);
  t->SetBranchAddress("mhiBin", &br.mhiBin);
  t->SetBranchAddress("mzdcsumPlus", &br.mzdcsumPlus);
  t->SetBranchAddress("mzdcsumMinus", &br.mzdcsumMinus);
  t->SetBranchAddress("mMaxL1HFAdcPlus", &br.mMaxL1HFAdcPlus);
  t->SetBranchAddress("mMaxL1HFAdcMinus", &br.mMaxL1HFAdcMinus);
  t->SetBranchAddress("mMaxietaPlus", &br.mMaxietaPlus);
  t->SetBranchAddress("mMaxietaMinus", &br.mMaxietaMinus);
  t->SetBranchAddress("mMaxiphiPlus", &br.mMaxiphiPlus);
  t->SetBranchAddress("mMaxiphiMinus", &br.mMaxiphiMinus);
  t->SetBranchAddress("mMaxdepthPlus", &br.mMaxdepthPlus);
  t->SetBranchAddress("mMaxdepthMinus", &br.mMaxdepthMinus);
  t->SetBranchAddress("mNpixel", &br.mNpixel);
  t->SetBranchAddress("mNtrkoffline", &br.mNtrkoffline);
  t->SetBranchAddress("mNpixelTracks", &br.mNpixelTracks);
  t->SetBranchAddress("mTrigHLT", br.mTrigHLT);
  t->SetBranchAddress("mEvtSel", br.mEvtSel);
}

void nt_cleanbranch(ntbranches& br) {
  br.mRun = -1;
  br.mLS = -1;
  br.mEvent = -1;
  br.mhiHF = -1;
  br.mhiBin = -1;
  br.mzdcsumPlus = -1;
  br.mzdcsumMinus = -1;
  br.mMaxL1HFAdcPlus = -1;
  br.mMaxL1HFAdcMinus = -1;
  br.mMaxietaPlus = -99;
  br.mMaxietaMinus = -99;
  br.mMaxiphiPlus = -99;
  br.mMaxiphiMinus = -99;
  br.mMaxdepthPlus = -99;
  br.mMaxdepthMinus = -99;
  br.mNpixel = -1;
  br.mNtrkoffline = -1;
  br.mNpixelTracks = -1;
  for (int i=0; i<MAX_EVTSEL; i++)
    br.mEvtSel[i] = true;
  for (int i=0; i<MAX_HLT; i++)
    br.mTrigHLT[i] = true;
}
