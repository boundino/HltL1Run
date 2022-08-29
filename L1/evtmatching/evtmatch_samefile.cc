#include <TTree.h>
#include <TFile.h>
#include <TDirectory.h>
#include <iostream>
#include <iomanip>
#include <fstream>
#include <string>
#include "maketree.h"

int evtmatching(std::string inf, std::string outfile)
{
  TFile* inff = TFile::Open(inf.c_str());

  TTree* hiroot = (TTree*)inff->Get("hiEvtAnalyzer/HiTree");
  TTree* skimroot = (TTree*)inff->Get("skimanalysis/HltTree");
  TTree* zdcdigiroot = (TTree*)inff->Get("zdcanalyzer/zdcdigi");
  TTree* zdcrechitroot = (TTree*)inff->Get("zdcanalyzer/zdcrechit");
  TTree* l1ADC = (TTree*)inff->Get("HFAdcana/adc");

  TFile* outf = new TFile(outfile.c_str(), "recreate");
  EvtTowerInfoNTuple* nt = new EvtTowerInfoNTuple(l1ADC, hiroot, skimroot, zdcrechitroot, zdcdigiroot);

  int entries = hiroot->GetEntries();
  for(Long64_t j = 0; j<entries; j++)
    {
      if(j % 10000 == 0) std::cout<<j<<" / "<<entries<<"\r"<<std::flush;

      nt->getentry(j);
      nt->calculate();

      nt->t->Fill();
    }
  std::cout<<std::endl;

  outf->Write();
  std::cout<<"---- Writing tree done"<<std::endl;
  outf->Close();
  std::cout<<outfile<<std::endl;

  return 0;
}

int main(int argc, char* argv[])
{
  if(argc==3) { return evtmatching(argv[1], argv[2]); }
  return 1;
}
