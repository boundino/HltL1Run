#include "evtl1ntuple.h"
#include <TFile.h>
#include <TTree.h>
#include <TH1F.h>
#include <string>
#include "xjjcuti.h"
#include "xjjrootuti.h"
#include "config.h"

#include "l1mb_constant.h"
#include "l1mb_helper.h"

int macro(std::string param)
{
  xjjc::config conf(param);
  conf.print();
  if (l1trigger::setconfig(conf)) return 2;
  int minLS = conf.vi("minLS"), maxLS = conf.vi("maxLS");
  std::string inputname = conf["Input"], outputdir = conf["Output"];

  std::cout<<std::endl<<"  [ "<<(l1trigger::ismc?"MC":"data")<<" ]"<<std::endl<<std::endl;
  
  TFile* inf = TFile::Open(inputname.c_str());
  TTree* tt = (TTree*)inf->Get("mbnt");
  l1trigger::evtl1ntuple* nt = new l1trigger::evtl1ntuple(tt);

  // hist
  auto hcent = new TH1F("hcent", ";Centrality [%];Entries", l1trigger::nbincent, 0, 100);
  auto hcent_hlt_effden = new TH1F("hcent_hlt_effden", ";Centrality [%];Entries", l1trigger::ncent, l1trigger::fcent.data());
  auto hcent_hlt_eff = new TH1F("hcent_hlt_eff", ";Centrality [%];Entries", l1trigger::ncent, l1trigger::fcent.data());
  auto hcent_hlt_rate = new TH1F("hcent_hlt_rate", ";;Entries", 1, 0, 1);
  auto hcent_hlt_fake = new TH1F("hcent_hlt_fake", ";;Entries", 1, 0, 1);

  create_hist();
  
  // run & fill
  int nentries = tt->GetEntries();
  int nZB_HLT = 0, ncolEvtSel = 0;
  for (int i=0; i<nentries; i++) {

    xjjc::progressslide(i, nentries, 100000);
    nt->GetEntry(i);

    // select ZB events
    if (!l1trigger::ismc) {
      if (!nt->HLT_ZB) continue;
      if (minLS > 0 && nt->br.mLS < minLS) continue;
      if (maxLS > 0 && nt->br.mLS > maxLS) continue;
    }
    
    nZB_HLT++;
      
    int icent = xjjc::findibin<Short_t>(l1trigger::cent, nt->br.mhiBin);
    if (icent<0) { /*std::cout<<"error: bad icent. ("<<nt->br.mhiBin<<")"<<std::endl;*/ icent = 0; }

    // ZDC distribution 
    hZDCdisGeV[0]->Fill(nt->ZDCplus);
    hZDCdisGeV[1]->Fill(nt->ZDCminus);

    // HLT
    if (l1trigger::MBindex >= 0 && nt->br.mTrigHLT[l1trigger::MBindex]) {
      hcent_hlt_rate->Fill(0);
      if (nt->colEvtSel)
        hcent_hlt_eff->Fill(nt->br.mhiBin);
      else {
        hcent_hlt_fake->Fill(0);
      }
    }
    if (l1trigger::ismc || nt->colEvtSel) {
      hcent->Fill(nt->br.mhiBin/2.);
      hcent_hlt_effden->Fill(nt->br.mhiBin);
      ncolEvtSel++;
    }
      
    // rate && eff
    for (int a=0; a<l1trigger::nadc; a++) {
      if (l1trigger::ismc || nt->colEvtSel) {
        heffden[icent]->Fill(a);
        heffden_int->Fill(a);
      }
      for (int k=0; k<l1trigger::nNeus; k++) {
        if (nt->AdcAND > a) {
          if (nt->ZDCplus >= l1trigger::mNeuZDCLow[0][k] && nt->ZDCminus >= l1trigger::mNeuZDCLow[1][k]) {
            // And_ZDCAnd
            hrate_And_ZDCAnd[k]->Fill(a);
            if (l1trigger::ismc || nt->colEvtSel) {
              heff_And_ZDCAnd[k][icent]->Fill(a);
              heff_And_ZDCAnd_int[k]->Fill(a);
              heffcent_And_ZDCAnd[k][a]->Fill(nt->br.mhiBin/2.);
            }
            else {
              hfake_And_ZDCAnd[k]->Fill(a);
            }
          } 
          if (nt->ZDCplus >= l1trigger::mNeuZDCLow[0][k] || nt->ZDCminus >= l1trigger::mNeuZDCLow[1][k]) {
            // And_ZDCOr
            hrate_And_ZDCOr[k]->Fill(a);
            if (l1trigger::ismc || nt->colEvtSel) {
              heff_And_ZDCOr[k][icent]->Fill(a);
              heff_And_ZDCOr_int[k]->Fill(a);
              heffcent_And_ZDCOr[k][a]->Fill(nt->br.mhiBin/2.);
            }
            else {
              hfake_And_ZDCOr[k]->Fill(a);
            }
          }
        } // if (nt->AdcAND > a) {
        if (nt->AdcOR > a) {
          if (nt->ZDCplus >= l1trigger::mNeuZDCLow[0][k] && nt->ZDCminus >= l1trigger::mNeuZDCLow[1][k]) {
            // Or_ZDCAnd
            hrate_Or_ZDCAnd[k]->Fill(a);
            if (l1trigger::ismc || nt->colEvtSel) {
              heff_Or_ZDCAnd[k][icent]->Fill(a);
              heff_Or_ZDCAnd_int[k]->Fill(a);
              heffcent_Or_ZDCAnd[k][a]->Fill(nt->br.mhiBin/2.);
            }
            else {
              hfake_Or_ZDCAnd[k]->Fill(a);
            }
          }          
          if (nt->ZDCplus >= l1trigger::mNeuZDCLow[0][k] || nt->ZDCminus >= l1trigger::mNeuZDCLow[1][k]) {
            // Or_ZDCOr
            hrate_Or_ZDCOr[k]->Fill(a);
            if (l1trigger::ismc || nt->colEvtSel) {
              heff_Or_ZDCOr[k][icent]->Fill(a);
              heff_Or_ZDCOr_int[k]->Fill(a);
              heffcent_Or_ZDCOr[k][a]->Fill(nt->br.mhiBin/2.);
            }
            else {
              hfake_Or_ZDCOr[k]->Fill(a);
            }
          }
        } // if (nt->AdcOR > a) {
      } // for (int k=0; k<l1trigger::nNeus; k++) {
    } //  for (int a=0; a<l1trigger::nadc; a++) {
  }
  xjjc::progressbar_summary(nentries);

  TH1F* hrateZB = new TH1F("hrateZB", ";L1 HF threshold (ADC);", 1, 0, 1);
  hrateZB->SetBinContent(1, l1trigger::ZBrate*l1trigger::nBunchRatio);
  // fake
  hcent_hlt_fake->Divide(hcent_hlt_rate);
  for (int k=0; k<l1trigger::nNeus; k++) {
    hfake_And_ZDCAnd[k]->Divide(hrate_And_ZDCAnd[k]);
    hfake_And_ZDCOr[k]->Divide(hrate_And_ZDCOr[k]);      
    hfake_Or_ZDCAnd[k]->Divide(hrate_Or_ZDCAnd[k]);      
    hfake_Or_ZDCOr[k]->Divide(hrate_Or_ZDCOr[k]);      
  }
  // rate
  float rate_scale = l1trigger::ZBrate*l1trigger::nBunchRatio/nZB_HLT/1.e+3;
  hcent_hlt_rate->Scale(rate_scale);
  for (auto& h : hrate_And_ZDCAnd) h->Scale(rate_scale);
  for (auto& h : hrate_And_ZDCOr) h->Scale(rate_scale);
  for (auto& h : hrate_Or_ZDCAnd) h->Scale(rate_scale);
  for (auto& h : hrate_Or_ZDCOr) h->Scale(rate_scale);
  // efficiency
  hcent_hlt_eff->Sumw2();
  hcent_hlt_eff->Divide(hcent_hlt_effden);
  for (int l=0; l<l1trigger::ncent; l++) {
    if (l > l1trigger::l_interest) continue;
    heffden_interest->Add(heffden[l]);
    for (int k=0; k<l1trigger::nNeus; k++) {
      heff_And_ZDCAnd_interest[k]->Add(heff_And_ZDCAnd[k][l]);
      heff_And_ZDCOr_interest[k]->Add(heff_And_ZDCOr[k][l]);
      heff_Or_ZDCAnd_interest[k]->Add(heff_Or_ZDCAnd[k][l]);
      heff_Or_ZDCOr_interest[k]->Add(heff_Or_ZDCOr[k][l]);
    }
  }
  for (int k=0; k<l1trigger::nNeus; k++) {
    for (int l=0; l<l1trigger::ncent; l++) {
      heff_And_ZDCAnd[k][l]->Sumw2();
      heff_And_ZDCAnd[k][l]->Divide(heffden[l]);
      heff_And_ZDCOr[k][l]->Sumw2();
      heff_And_ZDCOr[k][l]->Divide(heffden[l]);
      heff_Or_ZDCAnd[k][l]->Sumw2();
      heff_Or_ZDCAnd[k][l]->Divide(heffden[l]);
      heff_Or_ZDCOr[k][l]->Sumw2();
      heff_Or_ZDCOr[k][l]->Divide(heffden[l]);
    }
    for (int a=0; a<l1trigger::nadc; a++) {
      heffcent_And_ZDCAnd[k][a]->Sumw2();
      heffcent_And_ZDCAnd[k][a]->Divide(hcent);
      heffcent_And_ZDCOr[k][a]->Sumw2();
      heffcent_And_ZDCOr[k][a]->Divide(hcent);
      heffcent_Or_ZDCAnd[k][a]->Sumw2();
      heffcent_Or_ZDCAnd[k][a]->Divide(hcent);
      heffcent_Or_ZDCOr[k][a]->Sumw2();
      heffcent_Or_ZDCOr[k][a]->Divide(hcent);
    }
    heff_And_ZDCAnd_int[k]->Sumw2();
    heff_And_ZDCAnd_int[k]->Divide(heffden_int);
    heff_And_ZDCOr_int[k]->Sumw2();
    heff_And_ZDCOr_int[k]->Divide(heffden_int);
    heff_Or_ZDCAnd_int[k]->Sumw2();
    heff_Or_ZDCAnd_int[k]->Divide(heffden_int);
    heff_Or_ZDCOr_int[k]->Sumw2();
    heff_Or_ZDCOr_int[k]->Divide(heffden_int);
    heff_And_ZDCAnd_interest[k]->Sumw2();
    heff_And_ZDCAnd_interest[k]->Divide(heffden_interest);
    heff_And_ZDCOr_interest[k]->Sumw2();
    heff_And_ZDCOr_interest[k]->Divide(heffden_interest);
    heff_Or_ZDCAnd_interest[k]->Sumw2();
    heff_Or_ZDCAnd_interest[k]->Divide(heffden_interest);
    heff_Or_ZDCOr_interest[k]->Sumw2();
    heff_Or_ZDCOr_interest[k]->Divide(heffden_interest);
  }

  std::string outputname = "rootfiles/"+outputdir+"/savehist.root";
  xjjroot::mkdir(outputname);
  TFile* outf = new TFile(outputname.c_str(), "recreate");

  write_hist();

  xjjroot::writehist(hrateZB);
  xjjroot::writehist(hcent);
  xjjroot::writehist(hcent_hlt_rate);
  xjjroot::writehist(hcent_hlt_fake);
  xjjroot::writehist(hcent_hlt_eff);
  outf->Close();

  std::cout<<"ZB count: \e[4m"<<nZB_HLT<<"\e[0m"<<std::endl;
  std::cout<<"ZB rate (nb ratio = \e[4m"<<l1trigger::nBunchRatio<<"\e[0m): \e[4m"<<l1trigger::ZBrate*l1trigger::nBunchRatio<<"\e[0m"<<std::endl;
  std::cout<<"Event sel count: \e[4m"<<ncolEvtSel<<"\e[0m"<<std::endl;
  
  return 0;
}

int main(int argc, char* argv[])
{
  if (argc==2) return macro(argv[1]);
  return 1;
}

