#include <TTree.h>
#include <TChain.h>
#include <TFile.h>
#include <TDirectory.h>
#include <iostream>
#include <iomanip>
#include <fstream>
#include <string>
#include "mbntuplizer.h"

#include "xjjcuti.h"

mbntuplizer* set_forest(std::string inf) {
  mbntuplizer* nt = 0;

  auto hi = new TChain("hiEvtAnalyzer/HiTree");
  auto skim = new TChain("skimanalysis/HltTree");
  auto zdcdigi = new TChain("zdcanalyzer/zdcdigi");
  auto zdcrechit = new TChain("zdcanalyzer/zdcrechit");
  auto hlt = new TChain("hltanalysis/HltTree");
  auto l1adc = new TChain("HFAdcana/adc");

#define ADDFILE                                 \
  std::cout<<filename<<std::endl;               \
  hi->Add(filename.c_str());                    \
  skim->Add(filename.c_str());                  \
  zdcdigi->Add(filename.c_str());               \
  zdcrechit->Add(filename.c_str());             \
  hlt->Add(filename.c_str());                   \
  l1adc->Add(filename.c_str());                 \
  count++;                                      \
  
  int count = 0;
  std::string filename;
  if (inf.ends_with(".root")) {
    filename = inf;

    ADDFILE
      
  } else {
    std::ifstream getfile(inf.c_str());
    std::cout<<inf<<std::endl;
    while (true) {
      getfile >> filename;
      if(getfile.eof()) { break; }

      ADDFILE
        
    }
  }
  std::cout<<"Added \e[31m"<<count<<"\e[0m files."<<std::endl;
  nt = new mbntuplizer(l1adc, hi, skim, zdcrechit, zdcdigi, hlt);
  return nt;
}

int evtmatching(std::string inf, std::string outputname, int nentries = -1) {
  TTree::SetMaxTreeSize(1LL * 1024 * 1024 * 1024 * 1024);

  auto outf = TFile::Open(outputname.c_str(), "recreate", "", 101);
  auto nt = set_forest(inf);

  int entries = (nentries>0&&nentries<nt->getentries())?nentries:nt->getentries();
  for(int j = 0; j<entries; j++) {
    
    xjjc::progressslide(j, entries, 10000);

    nt->getentry(j);
    nt->calculate();
    
    if (!nt->getbr().mTrigHLT[0]) continue;
    // if (nt->getbr().mLS < 150) continue;

    nt->t->Fill();
  }
  xjjc::progressbar_summary(entries);

  outf->Write();
  std::cout<<"---- Writing tree done"<<std::endl;
  outf->Close();
  std::cout<<outputname<<std::endl;

  return 0;
}

int main(int argc, char* argv[]) {
  if(argc==3) { return evtmatching(argv[1], argv[2]); }
  if(argc==4) { return evtmatching(argv[1], argv[2], atoi(argv[3])); }
  return 1;
}
