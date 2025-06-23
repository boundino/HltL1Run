#include <TTree.h>
#include <map>

const int MIN_NHFTOWER = 1, MAX_NHFTOWER = 5;
const int MIN_HFTOWERE = 3, MAX_HFTOWERE = 10;

class forestgo {
public:
  forestgo(TFile* inf);

  float Npart;
  float hiHF;
  float hiHF_pf;

  int pclusterCompatibilityFilter;
  int pprimaryVertexFilter;
  std::map<std::string, int> pphfCoincFilterPF;

  int mMaxL1HFAdcPlus;
  int mMaxL1HFAdcMinus;

  int nPF;
  std::vector<int>* pfId;
  std::vector<float>* pfEta;
  std::vector<float>* pfE;
  
  int GetEntries() { return nentries; }
  void GetEntry(int i);

private:
  TTree *tevt, *tskim, *tadc, *tpf;
  int nentries;

  void setbranchaddress(TTree* nt, const char* bname, void* addr) {
    nt->SetBranchStatus(bname, 1);
    nt->SetBranchAddress(bname, addr);
  }

  void print(std::string name, bool good);
};

forestgo::forestgo(TFile* inf) :
  tskim(0), tevt(0), tadc(0) {

  nentries = 0;
  
  tevt = (TTree*)inf->Get("hiEvtAnalyzer/HiTree");
  print("hiEvtAnalyzer/HiTree", tevt);
  if (tevt) {
    tevt->SetBranchStatus("*", 0);
    setbranchaddress(tevt, "Npart", &Npart);
    setbranchaddress(tevt, "hiHF", &hiHF);
    setbranchaddress(tevt, "hiHF_pf", &hiHF_pf);

    if (tevt->GetEntries() < nentries || nentries == 0) {
      nentries = tevt->GetEntries();
    }
  }

  tskim = (TTree*)inf->Get("skimanalysis/HltTree");
  print("skimanalysis/HltTree", tskim);
  if (tskim) { 
    tskim->SetBranchStatus("*", 0);
    setbranchaddress(tskim, "pclusterCompatibilityFilter", &pclusterCompatibilityFilter);
    setbranchaddress(tskim, "pprimaryVertexFilter", &pprimaryVertexFilter);
    for (int i=MIN_NHFTOWER; i<MAX_NHFTOWER+1; i++) {
      for (int j=MIN_HFTOWERE; j<MAX_HFTOWERE+1; j++) {
        auto key = Form("%dTh%d", i, j);
        pphfCoincFilterPF[key] = 0;
        setbranchaddress(tskim, Form("pphfCoincFilterPF%s", key), &pphfCoincFilterPF[key]);
      }
    }
    
    if (tskim->GetEntries() < nentries || nentries == 0) {
      nentries = tskim->GetEntries();
    }
  }

  tadc = (TTree*)inf->Get("HFAdcana/adc");
  print("HFAdcana/adc", tadc);
  if (tadc) {
    tadc->SetBranchStatus("*", 0);
    setbranchaddress(tadc, "mMaxL1HFAdcPlus", &mMaxL1HFAdcPlus);
    setbranchaddress(tadc, "mMaxL1HFAdcMinus", &mMaxL1HFAdcMinus);

    if (tadc->GetEntries() < nentries || nentries == 0) {
      nentries = tadc->GetEntries();
    }
  }

  tpf = (TTree*)inf->Get("particleFlowAnalyser/pftree");
  print("particleFlowAnalyser/pftree", tpf);
  if (tpf) {
    tpf->SetBranchStatus("*", 0);
    setbranchaddress(tpf, "nPF", &nPF);
    pfId = 0;
    setbranchaddress(tpf, "pfId", &pfId);
    pfEta = 0;
    setbranchaddress(tpf, "pfEta", &pfEta);
    pfE = 0;
    setbranchaddress(tpf, "pfE", &pfE);

    if (tpf->GetEntries() < nentries || nentries == 0) {
      nentries = tpf->GetEntries();
    }
  }

  
}

void forestgo::GetEntry(int i) {
  if (tevt) tevt->GetEntry(i);
  if (tskim) tskim->GetEntry(i);
  if (tadc) tadc->GetEntry(i);
  if (tpf) tpf->GetEntry(i);
}

void forestgo::print(std::string name, bool good) {
  std::string color = good?"\e[32m":"\e[31m";
  std::cout<<" "<<color<<"("<<(good?"o":"x")<<") "<<name<<"\e[0m"<<std::endl;
}

