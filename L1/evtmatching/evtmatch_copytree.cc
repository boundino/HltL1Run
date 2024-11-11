#include <TTree.h>
#include <TFile.h>
#include <TChain.h>
#include <TDirectory.h>
#include <iostream>
#include <iomanip>
#include <fstream>
#include "L1AnalysisEventDataFormat.h"

namespace xjjc { void progressbar(int index_, int total_, int step=10000, int morespace_=0); }
int evtmatching(std::string infforest, std::string infl1, std::string outfile, Long64_t nentries = -1)
{
  auto hiroot = new TChain("hiEvtAnalyzer/HiTree");
  hiroot->Add(infforest.c_str());
  auto skimroot = new TChain("skimanalysis/HltTree");
  skimroot->Add(infforest.c_str());
  auto muonroot = new TChain("muonAnalyzer/MuonTree");
  muonroot->Add(infforest.c_str());
  auto l1EvtTree = new TChain("l1EventTree/L1EventTree");
  l1EvtTree->Add(infl1.c_str());
  auto l1ATree = new TChain("l1UpgradeEmuTree/L1UpgradeTree");
  l1ATree->Add(infl1.c_str());
  auto l1BTree = new TChain("l1uGTEmuTree/L1uGTTree");
  l1BTree->Add(infl1.c_str());

  auto outf = new TFile(outfile.c_str(), "recreate");
  auto dhiroot = outf->mkdir("hiEvtAnalyzer","");
  dhiroot->cd(); TTree* new_hiroot = hiroot->CloneTree(0);
  auto dskimroot = outf->mkdir("skimanalysis","");
  dskimroot->cd(); TTree* new_skimroot = skimroot->CloneTree(0);
  auto dmuonroot = outf->mkdir("muonAnalyzer","");
  dmuonroot->cd(); TTree* new_muonroot = muonroot->CloneTree(0);
  auto dl1ATree = outf->mkdir("l1UpgradeEmuTree","");
  dl1ATree->cd(); TTree* new_l1ATree = l1ATree->CloneTree(0);
  auto dl1BTree = outf->mkdir("l1uGTEmuTree","");
  dl1BTree->cd(); TTree* new_l1BTree = l1BTree->CloneTree(0);

  hiroot->SetBranchStatus("*", 0);
  hiroot->SetBranchStatus("run", 1);
  UInt_t run; hiroot->SetBranchAddress("run", &run);
  hiroot->SetBranchStatus("evt", 1);
  ULong64_t evt; hiroot->SetBranchAddress("evt", &evt);
  hiroot->SetBranchStatus("lumi", 1);
  UInt_t lumi; hiroot->SetBranchAddress("lumi", &lumi);
  L1Analysis::L1AnalysisEventDataFormat *Event = new L1Analysis::L1AnalysisEventDataFormat();
  l1EvtTree->SetBranchAddress("Event", &Event);

  std::cout<<"-- Check event number"<<std::endl;
  auto nmaptree = hiroot->GetEntries(), nmatchtree = l1EvtTree->GetEntries();

  std::cout<<"-- Read HI tree events"<<std::endl;
  std::vector<UInt_t> hiroot_run;
  std::vector<ULong64_t> hiroot_evt;
  std::vector<UInt_t> hiroot_lumi;
  for(Long64_t k = 0; k<nmaptree; k++) {
    xjjc::progressbar(k, nmaptree, 10000);
    hiroot->GetEntry(k); //
    hiroot_run.push_back(run);
    hiroot_evt.push_back(evt);
    hiroot_lumi.push_back(lumi);
  }
  std::cout<<std::endl<<"-- Matching events"<<std::endl;
  int countmatch = 0, lastmatch = -1;
  std::vector<Long64_t> matchingtable;
  Long64_t entries = (nentries<nmatchtree&&nentries>0)?nentries:nmatchtree;
  for(Long64_t j = 0; j<entries; j++) {

    xjjc::progressbar(j, entries, 1000);

    l1EvtTree->GetEntry(j); //
    Long64_t k = lastmatch+1, no = 0;
    for(; no < hiroot_run.size(); no++) {
      k = k%hiroot_run.size();
      if(hiroot_evt[k]==Event->event && hiroot_lumi[k]==Event->lumi && hiroot_run[k]==Event->run)
        break;
      k++;
    }
    bool matched = no<hiroot_run.size();
    if(!matched) k = -1;
    lastmatch = k;
    //
    matchingtable.push_back(k);

    if(matched) {
      countmatch++; 
      hiroot_run.erase(hiroot_run.begin() + k);
      hiroot_lumi.erase(hiroot_lumi.begin() + k);
      hiroot_evt.erase(hiroot_evt.begin() + k);
    }
  }
  std::cout<<std::endl;
  std::cout<<"\e[4m"<<countmatch<<"\e[0m events matched."<<std::endl;

  std::cout<<"-- Write to tree"<<std::endl;
  hiroot->SetBranchStatus("*", 1);
  for(Long64_t j = 0; j<entries; j++)
    {
      xjjc::progressbar(j, entries, 1000);
      Long64_t k = matchingtable[j];
      if(k<0) continue;
      hiroot->GetEntry(k); //
      skimroot->GetEntry(k); //
      muonroot->GetEntry(k); //
      // l1EvtTree->GetEntry(j); //
      l1ATree->GetEntry(j); //
      l1BTree->GetEntry(j); //

      dhiroot->cd();
      new_hiroot->Fill();
      dskimroot->cd();
      new_skimroot->Fill();
      dmuonroot->cd();
      new_muonroot->Fill();
      // dl1EvtTree->cd();
      // new_l1EvtTree->Fill();
      dl1ATree->cd(); // slow tree
      new_l1ATree->Fill();
      dl1BTree->cd(); // slow tree
      new_l1BTree->Fill();
    }
  std::cout<<std::endl;

  outf->Write();
  std::cout<<"-- Writing tree done"<<std::endl;
  outf->Close();
  std::cout<<"output: \e[4m"<<outfile<<"\e[0m"<<std::endl;

  return 0;
}

int main(int argc, char* argv[])
{
  if(argc==5) { return evtmatching(argv[1], argv[2], argv[3], atoi(argv[4])); }
  if(argc==4) { return evtmatching(argv[1], argv[2], argv[3]); }
  return 1;
}


void xjjc::progressbar(int index_, int total_, int step, int morespace_/*=0*/)
{
  if(index_%step==0 || index_ == total_-1)
    {
      if(total_ > 0)
        std::cout << std::setiosflags(std::ios::left)
                  << "  [ \e[36m" << std::setw(10+morespace_) << index_ << "\e[0m"
                  << " / " << std::setw(10+morespace_) << total_ << " ] "
                  << "\e[36m" << round(100.*index_/total_) << "%\e[0m" << "\r" << std::flush;
      else
        std::cout << std::setiosflags(std::ios::left)
                  << "  [ \e[36m" << std::setw(10+morespace_) << index_ << "\e[0m ]" << "\r" << std::flush;
    }
}
