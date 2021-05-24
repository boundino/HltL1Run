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
  TTree* root = (TTree*)inf->Get("EvtTowerInfoNTuple");

  short hiBin; root->SetBranchAddress("mCenBin", &hiBin);
  UInt_t run; root->SetBranchAddress("mRunNb", &run);
  bool mEvtSel[18]; root->SetBranchAddress("mEvtSel", mEvtSel);
  bool mTrigHLT[12]; root->SetBranchAddress("mTrigHLT", mTrigHLT);
  int amplmax_plus; root->SetBranchAddress("mMaxL1HFAdcPlus", &amplmax_plus);
  int amplmax_minus; root->SetBranchAddress("mMaxL1HFAdcMinus", &amplmax_minus);

  int count = 0;

  //
  int nentries = root->GetEntries();
  for(int i=0;i<nentries;i++)
    {
      xjjc::progressbar(i, nentries, 10000);

      root->GetEntry(i);

      if(runno >= 0 && run != runno) continue;
      if(!mTrigHLT[9]) continue;

      count++;

      bool pprimaryVertexFilter = mEvtSel[2];
      bool phfCoincFilter2Th4 = mEvtSel[1];
      bool pclusterCompatibilityFilter = mEvtSel[3];

      bool collisionEventSelection = pprimaryVertexFilter && phfCoincFilter2Th4 && pclusterCompatibilityFilter;

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
              if(amplmax_plus > j && amplmax_plus > j) { hEffAndEvtfilnomcent[k]->Fill(j, 1); } 

              if(is8595)
                {
                  hEffAndEvtfil8595den->Fill(j, 1);
                  if(amplmax_plus > j && amplmax_minus > j) { hEffAndEvtfil8595nom->Fill(j, 1); }
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
  root = (TTree*)inf->Get("EvtTowerInfoNTuple");
  root->SetBranchAddress("mCenBin", &hiBin);
  root->SetBranchAddress("mRunNb", &run);
  root->SetBranchAddress("mEvtSel", mEvtSel);
  root->SetBranchAddress("mTrigHLT", mTrigHLT);
  root->SetBranchAddress("mMaxL1HFAdcPlus", &amplmax_plus);
  root->SetBranchAddress("mMaxL1HFAdcMinus", &amplmax_minus);

  //
  nentries = root->GetEntries();
  count = 0;
  for(int i=0;i<nentries;i++)
    {
      xjjc::progressbar(i, nentries, 100000);

      root->GetEntry(i);

      if(runno >= 0 && run != runno) continue;
      if(!mTrigHLT[9]) continue;

      count++;

      for(int j=0; j<adceff::nbin; j++)
        {
          if(amplmax_plus > j && amplmax_minus > j) hEffRate->Fill(j, 1);
        }
    }
  xjjc::progressbar_summary(nentries);
  std::cout<<"-- L1 (\e[4;1m"<<runno<<"\e[0m): \e[33m"<<count<<"\e[0m, \e[33;1m"<<Form("%.1f", count*1.0/nentries*100.)<<"%\e[0m"<<std::endl;
  hEffRate->Scale(1./nentries);
  hfibEffRate->Scale(1./nentries);

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

