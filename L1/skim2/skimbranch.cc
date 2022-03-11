#include "skimbranch.h"
#include <TFile.h>
#include <TTree.h>
#include "xjjcuti.h"

int macro(std::string inputname, std::string outputname)
{
  TFile* inf = TFile::Open(inputname.c_str());
  TTree* nt = (TTree*)inf->Get("EvtTowerInfoNTuple");
  nt->SetBranchStatus("*", 0);
  for(auto& t : skimbranch::wishlist)
      nt->SetBranchStatus(t.c_str(), 1);
  TFile* outf = new TFile(outputname.c_str(), "recreate");
  TTree* nt_new = nt->CloneTree(0);

  int nentries = nt->GetEntries();
  for(int i=0; i<nentries; i++)
    {
      xjjc::progressbar(i, nentries);
      nt->GetEntry(i);
      nt_new->Fill();
    }
  xjjc::progressbar_summary(nentries);

  nt_new->Write();
  outf->Close();

  return 0;
}

int main(int argc, char* argv[])
{
  if(argc==3)
    return macro(argv[1], argv[2]);
  return 1;
}
