#include <TFile.h>
#include <TTree.h>
#include <TH1F.h>
#include <TEfficiency.h>

#include <string>
#include <vector>
#include <algorithm>

#include "adceff.h"
#include "xjjcuti.h"
#include "xjjrootuti.h"

void adceff_savehist(std::string inputname, std::string tag, int nevt = -1)
{
  std::string tagname = xjjc::str_replaceallspecial(tag);

  TFile* inf = TFile::Open(inputname.c_str());
  TTree* l1Adc = (TTree*)inf->Get("HFAdcana/adc");
  TTree* hiroot = (TTree*)inf->Get("hiEvtAnalyzer/HiTree");
  TTree* skimtree = (TTree*)inf->Get("skimanalysis/HltTree");

  l1Adc->AddFriend(hiroot);
  l1Adc->AddFriend(skimtree);

  int hiBin; hiroot->SetBranchAddress("hiBin", &hiBin);
  int pprimaryVertexFilter; skimtree->SetBranchAddress("pprimaryVertexFilter", &pprimaryVertexFilter);
  int phfCoincFilter2Th4; skimtree->SetBranchAddress("phfCoincFilter2Th4", &phfCoincFilter2Th4);
  int pclusterCompatibilityFilter; skimtree->SetBranchAddress("pclusterCompatibilityFilter", &pclusterCompatibilityFilter);
  int amplmax_plus; l1Adc->SetBranchAddress("amplmax_plus", &amplmax_plus);
  int amplmax_minus; l1Adc->SetBranchAddress("amplmax_minus", &amplmax_minus);
  int longmax_plus; l1Adc->SetBranchAddress("longmax_plus", &longmax_plus);
  int longmax_minus; l1Adc->SetBranchAddress("longmax_minus", &longmax_minus);
  int shortmax_plus; l1Adc->SetBranchAddress("shortmax_plus", &shortmax_plus);
  int shortmax_minus; l1Adc->SetBranchAddress("shortmax_minus", &shortmax_minus);

  //
  TH1F* hEffAndEvtfilnom = new TH1F("hEffAndEvtfilnom", ";HF threshold (ADC);Efficiency", adceff::nbin, 0, adceff::nbin); hEffAndEvtfilnom->Sumw2();
  TH1F* hEffAndEvtfilden = new TH1F("hEffAndEvtfilden", ";HF threshold (ADC);", adceff::nbin, 0, adceff::nbin); hEffAndEvtfilden->Sumw2();
  TH1F* hfibEffAndEvtfilnom = new TH1F("hfibEffAndEvtfilnom", ";HF threshold (ADC);Efficiency", adceff::nbin, 0, adceff::nbin); hfibEffAndEvtfilnom->Sumw2();
  TH1F* hfibEffAndEvtfilden = new TH1F("hfibEffAndEvtfilden", ";HF threshold (ADC);", adceff::nbin, 0, adceff::nbin); hfibEffAndEvtfilden->Sumw2();
  std::vector<TH1F*> hEffAndEvtfilnomcent(adceff::nhiBins), hEffAndEvtfildencent(adceff::nhiBins);
  std::vector<TH1F*> hfibEffAndEvtfilnomcent(adceff::nhiBins), hfibEffAndEvtfildencent(adceff::nhiBins);
  for(int k=0; k<adceff::nhiBins; k++)
    { 
      hEffAndEvtfilnomcent[k] = new TH1F(Form("hEffAndEvtfilnomcent%d",k), ";HF threshold (ADC);Efficiency", adceff::nbin, 0, adceff::nbin); hEffAndEvtfilnomcent[k]->Sumw2();
      hEffAndEvtfildencent[k] = new TH1F(Form("hEffAndEvtfildencent%d",k), ";HF threshold (ADC);Efficiency", adceff::nbin, 0, adceff::nbin); hEffAndEvtfildencent[k]->Sumw2();
      hfibEffAndEvtfilnomcent[k] = new TH1F(Form("hfibEffAndEvtfilnomcent%d",k), ";HF threshold (ADC);Efficiency", adceff::nbin, 0, adceff::nbin); hfibEffAndEvtfilnomcent[k]->Sumw2();
      hfibEffAndEvtfildencent[k] = new TH1F(Form("hfibEffAndEvtfildencent%d",k), ";HF threshold (ADC);Efficiency", adceff::nbin, 0, adceff::nbin); hfibEffAndEvtfildencent[k]->Sumw2();
    }
  TH1F* hEffRate = new TH1F("hEffRate", ";HF threshold (ADC);Rate", adceff::nbin, 0, adceff::nbin); hEffRate->Sumw2();
  TH1F* hfibEffRate = new TH1F("hfibEffRate", ";HF threshold (ADC);Rate", adceff::nbin, 0, adceff::nbin); hfibEffRate->Sumw2();

  //
  int nentries = nevt>0&&nevt<l1Adc->GetEntries()?nevt:l1Adc->GetEntries();
  for(int i=0;i<nentries;i++)
    {
      if(i%1000==0) { xjjc::progressbar(i, nentries); }

      l1Adc->GetEntry(i);
      hiroot->GetEntry(i);
      skimtree->GetEntry(i);

      bool collisionEventSelection = pprimaryVertexFilter && phfCoincFilter2Th4 && pclusterCompatibilityFilter;
      int fibermax_plus = std::max(longmax_plus, shortmax_plus);
      int fibermax_minus = std::max(longmax_minus, shortmax_minus);

      int k = 0;
      for(k=0; k<adceff::nhiBins; k++) { if(hiBin < adceff::hiBins[k+1]) { break; } }
      for(int j=0; j<adceff::nbin; j++)
        {
          if(collisionEventSelection)
            {
              hEffAndEvtfilden->Fill(j, 1);
              hfibEffAndEvtfilden->Fill(j, 1);
              if(amplmax_plus > j && amplmax_minus > j) { hEffAndEvtfilnom->Fill(j, 1); }
              if(fibermax_plus > j && fibermax_minus > j) { hfibEffAndEvtfilnom->Fill(j, 1); }

              hEffAndEvtfildencent[k]->Fill(j, 1); 
              hfibEffAndEvtfildencent[k]->Fill(j, 1); 
              if(amplmax_plus > j && amplmax_plus > j) { hEffAndEvtfilnomcent[k]->Fill(j, 1); } 
              if(fibermax_plus > j && fibermax_plus > j) { hfibEffAndEvtfilnomcent[k]->Fill(j, 1); } 
            }
          if(amplmax_plus > j && amplmax_minus > j) hEffRate->Fill(j, 1);
          if(fibermax_plus > j && fibermax_minus > j) hfibEffRate->Fill(j, 1);
        }
    }
  xjjc::progressbar_summary(nentries);

  TEfficiency* hEffAndEvtfil = new TEfficiency(*hEffAndEvtfilnom, *hEffAndEvtfilden);
  hEffAndEvtfil->SetName("hEffAndEvtfil");
  TEfficiency* hfibEffAndEvtfil = new TEfficiency(*hfibEffAndEvtfilnom, *hfibEffAndEvtfilden);
  hfibEffAndEvtfil->SetName("hfibEffAndEvtfil");
  std::vector<TEfficiency*> hEffAndEvtfilcent(adceff::nhiBins);
  std::vector<TEfficiency*> hfibEffAndEvtfilcent(adceff::nhiBins);
  for(int k=0; k<adceff::nhiBins; k++)
    { 
      hEffAndEvtfilcent[k] = new TEfficiency(*(hEffAndEvtfilnomcent[k]), *(hEffAndEvtfildencent[k])); 
      hEffAndEvtfilcent[k]->SetName(Form("hEffAndEvtfilcent%d", k));
      hfibEffAndEvtfilcent[k] = new TEfficiency(*(hfibEffAndEvtfilnomcent[k]), *(hfibEffAndEvtfildencent[k])); 
      hfibEffAndEvtfilcent[k]->SetName(Form("hfibEffAndEvtfilcent%d", k));
    }
  hEffRate->Scale(1./nentries);
  hfibEffRate->Scale(1./nentries);

  std::string outputname = "rootfiles/"+tagname+"/savehist.root";
  xjjroot::mkdir(outputname.c_str());
  TFile* output = new TFile(outputname.c_str(), "recreate");
  hEffAndEvtfil->Write();
  hfibEffAndEvtfil->Write();
  for(auto& hh : hEffAndEvtfilcent) hh->Write();
  for(auto& hh : hfibEffAndEvtfilcent) hh->Write();
  hEffRate->Write();
  hfibEffRate->Write();
  output->Close();
}

int main(int argc, char* argv[])
{
  if(argc==4) { adceff_savehist(argv[1], argv[2], atoi(argv[3])); return 0;}
  if(argc==3) { adceff_savehist(argv[1], argv[2]); return 0;}
  return 1;
}

