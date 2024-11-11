#include <iostream>
#include <iomanip>
#include <vector>

#include <TFile.h>
#include <TTree.h>
#include <TH1F.h>
#include <TMath.h>
#include <TH2F.h>

#include "xjjrootuti.h"
#include "xjjcuti.h"
#include "config.h"

void adcToGeV(std::string param, int nevt = 2.e+3)
{
  xjjc::config conf(param);
  conf.print();
  std::string inputname = conf["Input_adcToGeV"], outputdir = conf["Output"];

  TFile* inf = new TFile(inputname.c_str());
  TTree* l1Adc = (TTree*)inf->Get("HFAdcana/adc");
  // TTree* hiroot = (TTree*)inf->Get("hiEvtAnalyzer/HiTree");

  //
  int nampl; l1Adc->SetBranchAddress("nampl", &nampl);
  std::vector<int>*    ampl       = 0; l1Adc->SetBranchAddress("ampl",       &ampl);
  std::vector<int>*    ieta       = 0; l1Adc->SetBranchAddress("ieta",       &ieta);
  std::vector<int>*    iphi       = 0; l1Adc->SetBranchAddress("iphi",       &iphi);
  std::vector<int>*    depth      = 0; l1Adc->SetBranchAddress("depth",      &depth);
  std::vector<double>* charge     = 0; l1Adc->SetBranchAddress("charge",     &charge);
  std::vector<double>* charge_ped = 0; l1Adc->SetBranchAddress("charge_ped", &charge_ped);
  std::vector<double>* energy     = 0; l1Adc->SetBranchAddress("energy",     &energy);
  std::vector<double>* energy_ped = 0; l1Adc->SetBranchAddress("energy_ped", &energy_ped);

  // int ntower; hiroot->SetBranchAddress("ntower", &ntower);
  // float towerEmax; hiroot->SetBranchAddress("towerEmax", &towerEmax);
  // std::vector<float>* towerE    = 0; hiroot->SetBranchAddress("towerE",    &towerE);
  // std::vector<int>*   towerieta = 0; hiroot->SetBranchAddress("towerieta", &towerieta);
  // std::vector<int>*   toweriphi = 0; hiroot->SetBranchAddress("toweriphi", &toweriphi);

  const int NBIN_IETA = 101, NBIN_IPHI = 80; 

  //
  std::vector<TH2F*> vhist;
  TH2F* hcorr_ADC_Eped = new TH2F("hcorr_ADC_Eped", ";HF ADC;HF energy (before ped sub) [GeV]", 30, 0, 30, 100, 0, 20);
  vhist.push_back(hcorr_ADC_Eped);
  TH2F* hcorr_ADC_Epedplus = new TH2F("hcorr_ADC_Epedplus", ";HF+ ADC;HF+ energy (before ped sub) [GeV]", 30, 0, 30, 100, 0, 20);
  vhist.push_back(hcorr_ADC_Epedplus);
  TH2F* hcorr_ADC_Epedminus = new TH2F("hcorr_ADC_Epedminus", ";HF- ADC;HF- energy (before ped sub) [GeV]", 30, 0, 30, 100, 0, 20);
  vhist.push_back(hcorr_ADC_Epedminus);
  TH2F* hcorr_ADC_E = new TH2F("hcorr_ADC_E", ";HF ADC;HF energy (after ped sub) [GeV]", 30, 0, 30, 100, 0, 20);
  vhist.push_back(hcorr_ADC_E);
  TH2F* hcorr_Eped_E = new TH2F("hcorr_Eped_E", ";HF energy (before ped sub) [GeV];HF energy (after ped sub) [GeV]", 100, 0, 20, 100, 0, 20);
  vhist.push_back(hcorr_Eped_E);
  TH2F* hcorr_fCped_Eped = new TH2F("hcorr_fCped_Eped", ";Charge input fC (before ped sub);HF energy (before ped sub) [GeV]", 100, 0, 100, 100, 0, 20);
  vhist.push_back(hcorr_fCped_Eped);
  TH2F* hcorr_fC_E = new TH2F("hcorr_fC_E", ";Charge input fC (after ped sub);HF energy (after ped sub) [GeV]", 100, 0, 100, 100, 0, 20);
  vhist.push_back(hcorr_fC_E);
  // TH2F* hcorrtower_ADC_Eoffline = new TH2F("hcorrtower_ADC_Eoffline", ";HF ADC;offline HF tower energy / 4 [GeV]", 30, 0, 30, 100, 0, 20); 
  // vhist.push_back(hcorrtower_ADC_Eoffline);
  // TH2F* hcorrtower_E_Eoffline = new TH2F("hcorrtower_E_Eoffline", ";HF energy (after ped sub) [GeV];offline HF tower energy / 4 [GeV]", 100, 0, 20, 100, 0, 20);
  // vhist.push_back(hcorrtower_E_Eoffline);
  // TH2F* hcorrtower_ADCsum_Eoffline = new TH2F("hcorrtower_ADCsum_Eoffline", ";HF tower ADC / 4;offline HF tower energy / 4 [GeV]", 30, 0, 30, 100, 0, 20); 
  // vhist.push_back(hcorrtower_ADCsum_Eoffline);
  // TH2F* hcorrtower_Esum_Eoffline = new TH2F("hcorrtower_Esum_Eoffline", ";HF tower energy (after ped sub) / 4 [GeV];offline HF tower energy / 4 [GeV]", 100, 0, 20, 100, 0, 20);
  // vhist.push_back(hcorrtower_Esum_Eoffline);

  int nentries = nevt>0&&nevt<l1Adc->GetEntries()?nevt:l1Adc->GetEntries();
  auto hevt = new TH1F("hevt", "", 1, 0, 1); hevt->SetBinContent(1, nentries);
  for(int i=0;i<nentries;i++)
    {
      xjjc::progressbar(i, nentries, 100);
      l1Adc->GetEntry(i);
      // hiroot->GetEntry(i);

      // std::vector<std::vector<int>> ampltower(NBIN_IETA*100 + NBIN_IPHI, std::vector<int>());
      // std::vector<std::vector<double>> Etower(NBIN_IETA*100 + NBIN_IPHI, std::vector<double>());
      int nchannels = ampl->size();
      for(int l=0;l<nchannels;l++)
        {
          if(TMath::Abs((*ieta)[l]) < 29) { std::cout << "warning: channel not HF" << std::endl; continue; }
          // int index = ((*ieta)[l]+50) * 100 + (*iphi)[l];
          // ampltower[index].push_back( (*ampl)[l] );
          // Etower[index].push_back( (*energy)[l] );

          hcorr_ADC_Eped->Fill((*ampl)[l], (*energy_ped)[l]);
          if( (*ieta)[l] > 0 ) hcorr_ADC_Epedplus->Fill((*ampl)[l], (*energy_ped)[l]);
          if( (*ieta)[l] < 0 ) hcorr_ADC_Epedminus->Fill((*ampl)[l], (*energy_ped)[l]);
          hcorr_ADC_E->Fill((*ampl)[l], (*energy)[l]);
          hcorr_Eped_E->Fill((*energy_ped)[l], (*energy)[l]);
          hcorr_fCped_Eped->Fill((*charge_ped)[l], (*energy_ped)[l]);
          hcorr_fC_E->Fill((*charge)[l], (*energy)[l]);
        }
      // for(int j=0;j<ntower;j++) 
      //   { 
      //     int index = ((*towerieta)[j]+50) * 100 + (*toweriphi)[j];
      //     int thisampl = 0;
      //     double thisE = 0;
      //     for( auto& k : ampltower[index] ) { hcorrtower_ADC_Eoffline->Fill(k, (*towerE)[j] / 4.); thisampl += k; }
      //     for( auto& k : Etower[index] ) { hcorrtower_E_Eoffline->Fill(k, (*towerE)[j] / 4.); thisE += k; }
      //     hcorrtower_ADCsum_Eoffline->Fill(thisampl / 4., (*towerE)[j] / 4.);
      //     hcorrtower_Esum_Eoffline->Fill(thisE / 4., (*towerE)[j] / 4.);
      //   }
    }
  xjjc::progressbar_summary(nentries);

  std::string outputname = "rootfiles/"+outputdir+"/adcToGeV.root";
  xjjroot::mkdir(outputname);
  TFile* output = new TFile(Form("%s", outputname.c_str()), "recreate");
  hcorr_ADC_Eped->Write();
  hcorr_ADC_Epedplus->Write();
  hcorr_ADC_Epedminus->Write();
  hcorr_ADC_E->Write();
  hcorr_Eped_E->Write();
  hcorr_fCped_Eped->Write();
  hcorr_fC_E->Write();
  // hcorrtower_ADC_Eoffline->Write();
  // hcorrtower_E_Eoffline->Write();
  // hcorrtower_ADCsum_Eoffline->Write();
  // hcorrtower_Esum_Eoffline->Write();
  hevt->Write();
  output->Close();
}

int main(int argc, char* argv[])
{
  if(argc==2) { adcToGeV(argv[1]); return 0;}
  if(argc==3) { adcToGeV(argv[1], atoi(argv[2])); return 0;}
  return 1;
}

