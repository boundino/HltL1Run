#include <TTree.h>
#include <TFile.h>
#include <TDirectory.h>
#include <iostream>
#include <iomanip>
#include <fstream>
#include <TString.h>
#include "L1AnalysisEventDataFormat.h"
#include "L1AnalysisL1CaloTowerDataFormat.h"

int evtmatching(TString infforest, TString infl1, TString outfile, Long64_t nentries = -1)
{
  TFile* fforest = new TFile(infforest);
  TFile* fl1 = new TFile(infl1);

  TTree* hiroot = (TTree*)fforest->Get("hiEvtAnalyzer/HiTree");
  TTree* skimroot = (TTree*)fforest->Get("skimanalysis/HltTree");
  TTree* l1EvtTree = (TTree*)fl1->Get("l1EventTree/L1EventTree");
  TTree* l1TowerTree = (TTree*)fl1->Get("l1CaloTowerEmuTree/L1CaloTowerTree");
  TTree* l1ADC = (TTree*)fl1->Get("HFAdcana/adc");

  TFile* outf = new TFile(outfile,"recreate");
  TDirectory* dhiroot = outf->mkdir("hiEvtAnalyzer","");
  TDirectory* dskimroot = outf->mkdir("skimanalysis","");
  TDirectory* dl1EvtTree = outf->mkdir("l1EventTree","");
  TDirectory* dl1TowerTree = outf->mkdir("l1CaloTowerEmuTree","");
  TDirectory* dl1ADC = outf->mkdir("HFAdcana","");

  dhiroot->cd();
  TTree* new_hiroot = hiroot->CloneTree(0);
  dskimroot->cd();
  TTree* new_skimroot = skimroot->CloneTree(0);
  dl1EvtTree->cd();
  TTree* new_l1EvtTree = l1EvtTree->CloneTree(0);
  dl1TowerTree->cd();
  TTree* new_l1TowerTree = l1TowerTree->CloneTree(0);
  dl1ADC->cd();
  TTree* new_l1ADC = l1ADC->CloneTree(0);

  hiroot->SetBranchStatus("*", 0);
  hiroot->SetBranchStatus("run", 1);
  UInt_t run; hiroot->SetBranchAddress("run", &run);
  hiroot->SetBranchStatus("evt", 1);
  ULong64_t evt; hiroot->SetBranchAddress("evt", &evt);
  hiroot->SetBranchStatus("lumi", 1);
  UInt_t lumi; hiroot->SetBranchAddress("lumi", &lumi);
  L1Analysis::L1AnalysisEventDataFormat *Event = new L1Analysis::L1AnalysisEventDataFormat();
  l1EvtTree->SetBranchAddress("Event", &Event);

  std::cout<<"---- Check evt no. for three trees"<<std::endl;
  std::cout<<hiroot->GetEntries()<<", "<<l1EvtTree->GetEntries()<<", "<<l1ADC->GetEntries()<<std::endl;
  Long64_t nhiroot = hiroot->GetEntries();
  Long64_t nl1EvtTree = l1EvtTree->GetEntries();
  Long64_t nl1ADC = l1ADC->GetEntries();

  std::cout<<"---- Event reading"<<std::endl;
  Long64_t entries = (nentries<nl1EvtTree&&nentries>0)?nentries:nl1EvtTree;
  std::vector<UInt_t> hiroot_run;
  std::vector<ULong64_t> hiroot_evt;
  std::vector<UInt_t> hiroot_lumi;
  for(Long64_t k = 0; k<nhiroot; k++)
    {
      if(k % 10000 == 0) std::cout<<k<<" / "<<nhiroot<<"\r"<<std::flush; 
      hiroot->GetEntry(k); //
      hiroot_run.push_back(run);
      hiroot_evt.push_back(evt);
      hiroot_lumi.push_back(lumi);      
    }
  std::cout<<std::endl<<"---- Extracted event info"<<std::endl;
  int countmatch = 0, lastmatch = -1;
  std::vector<Long64_t> matchingtable;
  for(Long64_t j = 0; j<entries; j++)
    {
      if(j % 1000 == 0) std::cout<<j<<" / "<<entries<<"\r"<<std::flush;

      l1EvtTree->GetEntry(j); //
      Long64_t k = lastmatch+1;
      for(int no=0; no < hiroot_run.size(); no++)
        {
          k = k%hiroot_run.size();
          if(hiroot_evt[k]==Event->event && hiroot_lumi[k]==Event->lumi && hiroot_run[k]==Event->run)
            break;
          k++;
        }
      if(k>=nhiroot) k = -1;
      lastmatch = k;
      // else
      //   {
      //     hiroot_run.erase(hiroot_run.begin() + k);
      //     hiroot_evt.erase(hiroot_evt.begin() + k);
      //     hiroot_lumi.erase(hiroot_lumi.begin() + k);
      //   }
      matchingtable.push_back(k);
      if(k>=0) { countmatch++; }
    }
  std::cout<<std::endl;
  std::cout<<"countmatch = "<<countmatch<<std::endl;

  std::cout<<"---- Writing to tree"<<std::endl;
  hiroot->SetBranchStatus("*", 1);
  for(Long64_t j = 0; j<entries; j++)
    {
      if(j % 10000 == 0) std::cout<<j<<" / "<<entries<<"\r"<<std::flush;
      Long64_t k = matchingtable[j];
      if(k<0) continue;
      hiroot->GetEntry(k); //
      skimroot->GetEntry(k); //
      l1ADC->GetEntry(j); //
      l1EvtTree->GetEntry(j); //
      l1TowerTree->GetEntry(j); //

      dl1EvtTree->cd();
      new_l1EvtTree->Fill();
      dl1TowerTree->cd();
      new_l1TowerTree->Fill();
      dl1ADC->cd();
      new_l1ADC->Fill();
      dhiroot->cd();
      new_hiroot->Fill();
      dskimroot->cd();
      new_skimroot->Fill();
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
