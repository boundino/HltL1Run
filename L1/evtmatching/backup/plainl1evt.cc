#include <TTree.h>
#include <TFile.h>
#include <TDirectory.h>
#include <iostream>
#include <iomanip>
#include <fstream>
#include <TString.h>
#include "L1AnalysisEventDataFormat.h"
// #include "L1AnalysisL1CaloTowerDataFormat.h"

int plainl1evt(TString infl1, TString outfile, Long64_t nentries = -1)
{
  TFile* fl1 = new TFile(infl1);

  TTree* l1EvtTree = (TTree*)fl1->Get("l1EventTree/L1EventTree");
  // TTree* l1TowerTree = (TTree*)fl1->Get("l1CaloTowerEmuTree/L1CaloTowerTree");
  TTree* l1ADC = (TTree*)fl1->Get("HFAdcana/adc");

  TFile* outf = new TFile(outfile, "recreate");
  TTree* new_l1EvtTree = new TTree("l1", "");

  L1Analysis::L1AnalysisEventDataFormat *Event = new L1Analysis::L1AnalysisEventDataFormat();
  l1EvtTree->SetBranchAddress("Event", &Event);
  int mMaxL1HFAdcPlus; l1ADC->SetBranchAddress("mMaxL1HFAdcPlus", &mMaxL1HFAdcPlus);
  int mMaxL1HFAdcMinus; l1ADC->SetBranchAddress("mMaxL1HFAdcMinus", &mMaxL1HFAdcMinus);

  unsigned int l1_run; new_l1EvtTree->Branch("l1_run", &l1_run, "l1_run/i");
  unsigned int l1_lumi; new_l1EvtTree->Branch("l1_lumi", &l1_lumi, "l1_lumi/i");
  unsigned int l1_evt; new_l1EvtTree->Branch("l1_evt", &l1_evt, "l1_evt/i");
  new_l1EvtTree->Branch("mMaxL1HFAdcPlus", &mMaxL1HFAdcPlus, "mMaxL1HFAdcPlus/I");
  new_l1EvtTree->Branch("mMaxL1HFAdcMinus", &mMaxL1HFAdcMinus, "mMaxL1HFAdcMinus/I");

  std::cout<<"---- Write to tree"<<std::endl;
  Long64_t entries = (nentries<l1EvtTree->GetEntries()&&nentries>0)?nentries:l1EvtTree->GetEntries();
  for(Long64_t j = 0; j<entries; j++)
    {
      if(j % 100000 == 0) std::cout<<j<<" / "<<entries<<"\r"<<std::flush;
      l1ADC->GetEntry(j); //
      l1EvtTree->GetEntry(j); //
      // l1TowerTree->GetEntry(j); //

      l1_run = Event->run;
      l1_lumi = Event->lumi;
      l1_evt = Event->event;

      new_l1EvtTree->Fill();
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
  if(argc==4) { return plainl1evt(argv[1], argv[2], atoi(argv[3])); }
  if(argc==3) { return plainl1evt(argv[1], argv[2]); }
  return 1;
}
