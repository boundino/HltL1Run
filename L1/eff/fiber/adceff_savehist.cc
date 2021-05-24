#include <TFile.h>
#include <TTree.h>
#include <TH1F.h>
#include <TEfficiency.h>
#include "L1AnalysisEventDataFormat.h"

#include <string>
#include <vector>
#include <algorithm>

#include "adceff.h"
#include "xjjcuti.h"
#include "xjjrootuti.h"

void adceff_savehist(std::string inputname_eff, std::string inputname_rate, std::string tag, int runno = -1)
{
  std::string tagname = xjjc::str_replaceallspecial(tag);

  // efficiency

  TH1F* hEffAndEvtfilnom = new TH1F("hEffAndEvtfilnom", ";HF threshold (ADC);Efficiency", adceff::nbin, 0, adceff::nbin); hEffAndEvtfilnom->Sumw2();
  TH1F* hEffAndEvtfilden = new TH1F("hEffAndEvtfilden", ";HF threshold (ADC);", adceff::nbin, 0, adceff::nbin); hEffAndEvtfilden->Sumw2();
  TH1F* hfibEffAndEvtfilnom = new TH1F("hfibEffAndEvtfilnom", ";HF threshold (ADC);Efficiency", adceff::nbinfib, 0, adceff::nbinfib); hfibEffAndEvtfilnom->Sumw2();
  TH1F* hfibEffAndEvtfilden = new TH1F("hfibEffAndEvtfilden", ";HF threshold (ADC);", adceff::nbinfib, 0, adceff::nbinfib); hfibEffAndEvtfilden->Sumw2();
  TH1F* hEffAndEvtfil8595nom = new TH1F("hEffAndEvtfil8595nom", ";HF threshold (ADC);Efficiency", adceff::nbin, 0, adceff::nbin); hEffAndEvtfil8595nom->Sumw2();
  TH1F* hEffAndEvtfil8595den = new TH1F("hEffAndEvtfil8595den", ";HF threshold (ADC);", adceff::nbin, 0, adceff::nbin); hEffAndEvtfil8595den->Sumw2();
  TH1F* hfibEffAndEvtfil8595nom = new TH1F("hfibEffAndEvtfil8595nom", ";HF threshold (ADC);Efficiency", adceff::nbinfib, 0, adceff::nbinfib); hfibEffAndEvtfil8595nom->Sumw2();
  TH1F* hfibEffAndEvtfil8595den = new TH1F("hfibEffAndEvtfil8595den", ";HF threshold (ADC);", adceff::nbinfib, 0, adceff::nbinfib); hfibEffAndEvtfil8595den->Sumw2();
  std::vector<TH1F*> hEffAndEvtfilnomcent(adceff::nhiBins), hEffAndEvtfildencent(adceff::nhiBins);
  std::vector<TH1F*> hfibEffAndEvtfilnomcent(adceff::nhiBins), hfibEffAndEvtfildencent(adceff::nhiBins);
  for(int k=0; k<adceff::nhiBins; k++)
    { 
      hEffAndEvtfilnomcent[k] = new TH1F(Form("hEffAndEvtfilnomcent%d",k), ";HF threshold (ADC);Efficiency", adceff::nbin, 0, adceff::nbin); hEffAndEvtfilnomcent[k]->Sumw2();
      hEffAndEvtfildencent[k] = new TH1F(Form("hEffAndEvtfildencent%d",k), ";HF threshold (ADC);Efficiency", adceff::nbin, 0, adceff::nbin); hEffAndEvtfildencent[k]->Sumw2();
      hfibEffAndEvtfilnomcent[k] = new TH1F(Form("hfibEffAndEvtfilnomcent%d",k), ";HF threshold (ADC);Efficiency", adceff::nbinfib, 0, adceff::nbinfib); hfibEffAndEvtfilnomcent[k]->Sumw2();
      hfibEffAndEvtfildencent[k] = new TH1F(Form("hfibEffAndEvtfildencent%d",k), ";HF threshold (ADC);Efficiency", adceff::nbinfib, 0, adceff::nbinfib); hfibEffAndEvtfildencent[k]->Sumw2();
    }

  TFile* inf = TFile::Open(inputname_eff.c_str());
  TTree* l1Adc = (TTree*)inf->Get("HFAdcana/adc");
  TTree* hiroot = (TTree*)inf->Get("hiEvtAnalyzer/HiTree");
  TTree* skimtree = (TTree*)inf->Get("skimanalysis/HltTree");

  int hiBin; hiroot->SetBranchAddress("hiBin", &hiBin);
  UInt_t run; hiroot->SetBranchAddress("run", &run);
  int pprimaryVertexFilter; skimtree->SetBranchAddress("pprimaryVertexFilter", &pprimaryVertexFilter);
  int phfCoincFilter2Th4; skimtree->SetBranchAddress("phfCoincFilter2Th4", &phfCoincFilter2Th4);
  int pclusterCompatibilityFilter; skimtree->SetBranchAddress("pclusterCompatibilityFilter", &pclusterCompatibilityFilter);
  int amplmax_plus; l1Adc->SetBranchAddress("amplmax_plus", &amplmax_plus);
  int amplmax_minus; l1Adc->SetBranchAddress("amplmax_minus", &amplmax_minus);
  int longmax_plus; l1Adc->SetBranchAddress("longmax_plus", &longmax_plus);
  int longmax_minus; l1Adc->SetBranchAddress("longmax_minus", &longmax_minus);
  int shortmax_plus; l1Adc->SetBranchAddress("shortmax_plus", &shortmax_plus);
  int shortmax_minus; l1Adc->SetBranchAddress("shortmax_minus", &shortmax_minus);

  int count = 0;

  //
  int nentries = l1Adc->GetEntries();
  for(int i=0;i<nentries;i++)
    {
      xjjc::progressbar(i, nentries, 10000);

      l1Adc->GetEntry(i);
      hiroot->GetEntry(i);
      skimtree->GetEntry(i);

      if(runno >= 0 && run != runno) continue;
      // if(run != 326676 && run != 326718) continue;

      count++;

      bool collisionEventSelection = pprimaryVertexFilter && phfCoincFilter2Th4 && pclusterCompatibilityFilter;
      int fibermax_plus = std::max(longmax_plus, shortmax_plus);
      int fibermax_minus = std::max(longmax_minus, shortmax_minus);

      int k = 0;
      for(k=0; k<adceff::nhiBins; k++) { if(hiBin < adceff::hiBins[k+1]) { break; } }
      bool is8595 = hiBin >= 85*2 && hiBin < 95*2;

      for(int j=0; j<adceff::nbin; j++)
        {
          if(collisionEventSelection)
            {
              hEffAndEvtfilden->Fill(j, 1);
              if(amplmax_plus > j && amplmax_minus > j) { hEffAndEvtfilnom->Fill(j, 1); }

              hEffAndEvtfildencent[k]->Fill(j, 1); 
              if(amplmax_plus > j && amplmax_minus > j) { hEffAndEvtfilnomcent[k]->Fill(j, 1); } 

              if(is8595)
                {
                  hEffAndEvtfil8595den->Fill(j, 1);
                  if(amplmax_plus > j && amplmax_minus > j) { hEffAndEvtfil8595nom->Fill(j, 1); }
                }
            }
        }

      for(int j=0; j<adceff::nbinfib; j++)
        {
          if(collisionEventSelection)
            {
              hfibEffAndEvtfilden->Fill(j, 1);
              if(fibermax_plus > j && fibermax_minus > j) { hfibEffAndEvtfilnom->Fill(j, 1); }

              hfibEffAndEvtfildencent[k]->Fill(j, 1); 
              if(fibermax_plus > j && fibermax_minus > j) { hfibEffAndEvtfilnomcent[k]->Fill(j, 1); } 

              if(is8595)
                {
                  hfibEffAndEvtfil8595den->Fill(j, 1);
                  if(fibermax_plus > j && fibermax_minus > j) { hfibEffAndEvtfil8595nom->Fill(j, 1); }
                }
            }
        }
    }
  xjjc::progressbar_summary(nentries);
  std::cout<<"-- MB (\e[4;1m"<<runno<<"\e[0m): \e[33m"<<count<<"\e[0m, \e[33;1m"<<Form("%.1f", count*1.0/nentries*100.)<<"%\e[0m"<<std::endl;

  TEfficiency* hEffAndEvtfil = new TEfficiency(*hEffAndEvtfilnom, *hEffAndEvtfilden);
  hEffAndEvtfil->SetName("hEffAndEvtfil");
  TEfficiency* hfibEffAndEvtfil = new TEfficiency(*hfibEffAndEvtfilnom, *hfibEffAndEvtfilden);
  hfibEffAndEvtfil->SetName("hfibEffAndEvtfil");
  TEfficiency* hEffAndEvtfil8595 = new TEfficiency(*hEffAndEvtfil8595nom, *hEffAndEvtfil8595den);
  hEffAndEvtfil8595->SetName("hEffAndEvtfil8595");
  TEfficiency* hfibEffAndEvtfil8595 = new TEfficiency(*hfibEffAndEvtfil8595nom, *hfibEffAndEvtfil8595den);
  hfibEffAndEvtfil8595->SetName("hfibEffAndEvtfil8595");
  std::vector<TEfficiency*> hEffAndEvtfilcent(adceff::nhiBins);
  std::vector<TEfficiency*> hfibEffAndEvtfilcent(adceff::nhiBins);
  for(int k=0; k<adceff::nhiBins; k++)
    { 
      hEffAndEvtfilcent[k] = new TEfficiency(*(hEffAndEvtfilnomcent[k]), *(hEffAndEvtfildencent[k])); 
      hEffAndEvtfilcent[k]->SetName(Form("hEffAndEvtfilcent%d", k));
      hfibEffAndEvtfilcent[k] = new TEfficiency(*(hfibEffAndEvtfilnomcent[k]), *(hfibEffAndEvtfildencent[k])); 
      hfibEffAndEvtfilcent[k]->SetName(Form("hfibEffAndEvtfilcent%d", k));
    }

  // rate

  TH1F* hEffRate = new TH1F("hEffRate", ";HF threshold (ADC);Rate", adceff::nbin, 0, adceff::nbin); hEffRate->Sumw2();
  TH1F* hfibEffRate = new TH1F("hfibEffRate", ";HF threshold (ADC);Rate", adceff::nbinfib, 0, adceff::nbinfib); hfibEffRate->Sumw2();

  inf = TFile::Open(inputname_rate.c_str());
  l1Adc = (TTree*)inf->Get("HFAdcana/adc");
  TTree* l1EvtTree = (TTree*)inf->Get("l1EventTree/L1EventTree");
  L1Analysis::L1AnalysisEventDataFormat *Event = new L1Analysis::L1AnalysisEventDataFormat();
  l1EvtTree->SetBranchAddress("Event", &Event); 

  l1Adc->SetBranchAddress("amplmax_plus", &amplmax_plus);
  l1Adc->SetBranchAddress("amplmax_minus", &amplmax_minus);
  l1Adc->SetBranchAddress("longmax_plus", &longmax_plus);
  l1Adc->SetBranchAddress("longmax_minus", &longmax_minus);
  l1Adc->SetBranchAddress("shortmax_plus", &shortmax_plus);
  l1Adc->SetBranchAddress("shortmax_minus", &shortmax_minus);

  //
  nentries = l1Adc->GetEntries();
  count = 0;
  for(int i=0;i<nentries;i++)
    {
      xjjc::progressbar(i, nentries, 100000);

      l1Adc->GetEntry(i);
      l1EvtTree->GetEntry(i);

      if(runno >= 0 && Event->run != runno) continue;
      // if(Event->run != 326676 && Event->run != 326718) continue;

      count++;

      int fibermax_plus = std::max(longmax_plus, shortmax_plus);
      int fibermax_minus = std::max(longmax_minus, shortmax_minus);

      for(int j=0; j<adceff::nbin; j++)
        {
          if(amplmax_plus > j && amplmax_minus > j) hEffRate->Fill(j, 1);
        }
      for(int j=0; j<adceff::nbinfib; j++)
        {
          if(fibermax_plus > j && fibermax_minus > j) hfibEffRate->Fill(j, 1);
        }
    }
  xjjc::progressbar_summary(nentries);
  std::cout<<"-- L1 (\e[4;1m"<<runno<<"\e[0m): \e[33m"<<count<<"\e[0m, \e[33;1m"<<Form("%.1f", count*1.0/nentries*100.)<<"%\e[0m"<<std::endl;
  hEffRate->Scale(1./count);
  hfibEffRate->Scale(1./count);

  // output

  std::string outputname = "rootfiles/"+tagname+"/savehist.root";
  xjjroot::mkdir(outputname.c_str());
  TFile* output = new TFile(outputname.c_str(), "recreate");
  hEffAndEvtfil->Write();
  hfibEffAndEvtfil->Write();
  hEffAndEvtfil8595->Write();
  hfibEffAndEvtfil8595->Write();
  for(auto& hh : hEffAndEvtfilcent) hh->Write();
  for(auto& hh : hfibEffAndEvtfilcent) hh->Write();
  hEffRate->Write();
  hfibEffRate->Write();
  output->Close();

  std::cout<<std::endl;
}

int main(int argc, char* argv[])
{
  if(argc==5) { adceff_savehist(argv[1], argv[2], argv[3], atoi(argv[4])); return 0;}
  return 1;
}

