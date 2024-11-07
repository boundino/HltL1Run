#include <TTree.h>
#include <TFile.h>
#include <TDirectory.h>
#include <iostream>
#include <iomanip>
#include <fstream>
#include <string>
#include "mbntuplizer.h"

#include "xjjcuti.h"

int evtmatching(std::string inf, std::string outfile, int nentries = -1) {
  
  TFile* inff = TFile::Open(inf.c_str());

  TTree* hiroot = (TTree*)inff->Get("hiEvtAnalyzer/HiTree");
  TTree* skimroot = (TTree*)inff->Get("skimanalysis/HltTree");
  TTree* zdcdigiroot = (TTree*)inff->Get("zdcanalyzer/zdcdigi");
  TTree* zdcrechitroot = (TTree*)inff->Get("zdcanalyzer/zdcrechit");
  TTree* hltroot = (TTree*)inff->Get("hltanalysis/HltTree");
  TTree* l1ADC = (TTree*)inff->Get("HFAdcana/adc");

  TFile* outf = new TFile(outfile.c_str(), "recreate");
  mbntuplizer* nt = new mbntuplizer(l1ADC, hiroot, skimroot, zdcrechitroot, zdcdigiroot, hltroot);

  int entries = (nentries>0&&nentries<l1ADC->GetEntries())?nentries:l1ADC->GetEntries();
  for(Long64_t j = 0; j<entries; j++) {
    
    xjjc::progressslide(j, entries, 10000);

    nt->getentry(j);
    nt->calculate();

    nt->t->Fill();
  }
  xjjc::progressbar_summary(entries);

  outf->Write();
  std::cout<<"---- Writing tree done"<<std::endl;
  outf->Close();
  std::cout<<outfile<<std::endl;

  return 0;
}

int main(int argc, char* argv[]) {
  if(argc==3) { return evtmatching(argv[1], argv[2]); }
  if(argc==4) { return evtmatching(argv[1], argv[2], atoi(argv[3])); }
  return 1;
}
