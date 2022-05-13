#include <TTree.h>
#include <TFile.h>
#include <TDirectory.h>
#include <iostream>
#include <iomanip>
#include <fstream>
#include <TString.h>
#include "L1AnalysisEventDataFormat.h"
// #include "L1AnalysisL1CaloTowerDataFormat.h"

#include "maketree.h"

int evtmatching(TString infforest, TString infl1, TString outfile, Long64_t nentries = -1)
{
  TFile* fforest = TFile::Open(infforest);
  TFile* fl1 = TFile::Open(infl1);

  TTree* hiroot = (TTree*)fforest->Get("hiEvtAnalyzer/HiTree");
  TTree* skimroot = (TTree*)fforest->Get("skimanalysis/HltTree");
  TTree* zdcdigiroot = (TTree*)fforest->Get("zdcanalyzer/zdcdigi");
  TTree* zdcrechitroot = (TTree*)fforest->Get("zdcanalyzer/zdcrechit");
  TTree* l1EvtTree = (TTree*)fl1->Get("l1EventTree/L1EventTree");
  // TTree* l1TowerTree = (TTree*)fl1->Get("l1CaloTowerEmuTree/L1CaloTowerTree");
  TTree* l1ADC = (TTree*)fl1->Get("HFAdcana/adc");

  TFile* outf = new TFile(outfile, "recreate");
  EvtTowerInfoNTuple* nt = new EvtTowerInfoNTuple(l1ADC, hiroot, skimroot, zdcrechitroot, zdcdigiroot);

  hiroot->SetBranchStatus("*", 0);
  hiroot->SetBranchStatus("run", 1);
  UInt_t run; hiroot->SetBranchAddress("run", &run);
  hiroot->SetBranchStatus("evt", 1);
  ULong64_t evt; hiroot->SetBranchAddress("evt", &evt);
  hiroot->SetBranchStatus("lumi", 1);
  UInt_t lumi; hiroot->SetBranchAddress("lumi", &lumi);
  L1Analysis::L1AnalysisEventDataFormat *Event = new L1Analysis::L1AnalysisEventDataFormat();
  l1EvtTree->SetBranchAddress("Event", &Event);

  std::cout<<"---- Check event number"<<std::endl;
  // Long64_t nmaptree = hiroot->GetEntries(), nmatchtree = l1EvtTree->GetEntries();
  Long64_t nmaptree = l1EvtTree->GetEntries(), nmatchtree = hiroot->GetEntries();
  std::cout<<std::left<<std::setw(17)<<"[ hiroot"<<"] "<<hiroot->GetEntries()<<std::endl;
  std::cout<<std::left<<std::setw(17)<<"[ skimroot"<<"] "<<skimroot->GetEntries()<<std::endl;
  std::cout<<std::left<<std::setw(17)<<"[ zdcdigiroot"<<"] "<<zdcdigiroot->GetEntries()<<std::endl;
  std::cout<<std::left<<std::setw(17)<<"[ zdcrechitroot"<<"] "<<zdcrechitroot->GetEntries()<<std::endl;
  std::cout<<std::left<<std::setw(17)<<"[ l1EvtTree"<<"] "<<l1EvtTree->GetEntries()<<std::endl;
  std::cout<<std::left<<std::setw(17)<<"[ l1ADC"<<"] "<<l1ADC->GetEntries()<<std::endl;

  std::cout<<"---- Read HI tree events"<<std::endl;
  std::vector<UInt_t> map_run;
  std::vector<ULong64_t> map_evt;
  std::vector<UInt_t> map_lumi;
  for(Long64_t k = 0; k<nmaptree; k++)
    {
      if(k % 100000 == 0) std::cout<<k<<" / "<<nmaptree<<"\r"<<std::flush; 
      l1EvtTree->GetEntry(k); //
      map_run.push_back(Event->run);
      map_evt.push_back(Event->event);
      map_lumi.push_back(Event->lumi);
    }
  std::cout<<std::endl<<"---- Matching events"<<std::endl;
  int countmatch = 0, lastmatch = -1;
  std::vector<Long64_t> matchingtable;
  Long64_t entries = (nentries<nmatchtree&&nentries>0)?nentries:nmatchtree;
  for(Long64_t j = 0; j<entries; j++)
    {
      if(j % 1000 == 0) std::cout<<j<<" / "<<entries<<"\r"<<std::flush;

      hiroot->GetEntry(j); //
      Long64_t k = lastmatch+1, no = 0;
      for(; no < map_run.size(); no++)
        {
          k = k%map_run.size();
          // if(map_evt[k]==Event->event && map_lumi[k]==Event->lumi && map_run[k]==Event->run)
          if(map_evt[k]==evt && map_lumi[k]==lumi && map_run[k]==run)
            break;
          k++;
        }
      bool matched = no<map_run.size();
      if(!matched) k = -1;
      lastmatch = k;
      //
      matchingtable.push_back(k);
      if(j % 10000 == 0)
        {
          std::cout<<"check matching... ";
          // std::cout<<std::left<<std::setw(8)<<Event->run<<std::setw(5)<<Event->lumi<<std::setw(10)<<Event->event<<" | ";
          std::cout<<std::left<<std::setw(8)<<run<<std::setw(5)<<lumi<<std::setw(10)<<evt<<" | ";
          if(matched)
            std::cout<<std::left<<std::setw(8)<<map_run[k]<<std::setw(5)<<map_lumi[k]<<std::setw(10)<<map_evt[k];
          std::cout<<std::endl;
        }
      if(matched)
        { 
          countmatch++; 
          map_run.erase(map_run.begin() + k);
          map_lumi.erase(map_lumi.begin() + k);
          map_evt.erase(map_evt.begin() + k);
        }
    }
  std::cout<<std::endl;
  std::cout<<"countmatch = "<<countmatch<<std::endl;

  std::cout<<"---- Write to tree"<<std::endl;
  hiroot->SetBranchStatus("*", 1);
  for(Long64_t j = 0; j<entries; j++)
    {
      if(j % 10000 == 0) std::cout<<j<<" / "<<entries<<"\r"<<std::flush;
      Long64_t k = matchingtable[j];
      if(k<0) continue;
      hiroot->GetEntry(j); //
      skimroot->GetEntry(j); //
      zdcdigiroot->GetEntry(j); //
      zdcrechitroot->GetEntry(j); //
      l1ADC->GetEntry(k); //
      // l1EvtTree->GetEntry(k); //
      // l1TowerTree->GetEntry(k); //

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
  if(argc==5) { return evtmatching(argv[1], argv[2], argv[3], atoi(argv[4])); }
  if(argc==4) { return evtmatching(argv[1], argv[2], argv[3]); }
  return 1;
}
