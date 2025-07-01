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
  
  auto inff = TFile::Open(inf.c_str());

  auto hiroot = (TTree*)inff->Get("hiEvtAnalyzer/HiTree");
  auto skimroot = (TTree*)inff->Get("skimanalysis/HltTree");
  auto zdcdigiroot = (TTree*)inff->Get("zdcanalyzer/zdcdigi");
  auto zdcrechitroot = (TTree*)inff->Get("zdcanalyzer/zdcrechit");
  auto hltroot = (TTree*)inff->Get("hltanalysis/HltTree");
  auto l1ADC = (TTree*)inff->Get("HFAdcana/adc");

  auto outf = new TFile(outfile.c_str(), "recreate");
  auto nt = new mbntuplizer(l1ADC, hiroot, skimroot, zdcrechitroot, zdcdigiroot, hltroot);

  int entries = (nentries>0&&nentries<l1ADC->GetEntries())?nentries:l1ADC->GetEntries();
  for(int j = 0; j<entries; j++) {
    
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
