#include "evtl1ntuple.h"
#include <TFile.h>
#include <TTree.h>
#include <TH1F.h>
#include <string>
#include "xjjcuti.h"
#include "xjjrootuti.h"
#include "config.h"

#include "constant.h"

int macro(std::string param)
{
  xjjc::config conf(param);
  conf.print();
  float ZBrate = conf.vf("ZBrate"), nBunchRatio = conf.vf("nBunchRatio");
  int minLS = conf.vi("minLS"), maxLS = conf.vi("maxLS");
  std::string inputname = conf["Input"], outputdir = conf["Output"], MBhlt = conf["MBhlt"];
  l1trigger::sethlt(MBhlt);
  l1trigger::setcent();

  TFile* inf = TFile::Open(inputname.c_str());
  TTree* tt = (TTree*)inf->Get("mbnt");
  l1trigger::evtl1ntuple* nt = new l1trigger::evtl1ntuple(tt);

  // hist
  auto hcent = new TH1F("hcent", ";Centrality [%];Entries", l1trigger::nbincent, 0, 100);
  auto hcent_hlt_effden = new TH1F("hcent_hlt_effden", ";Centrality [%];Entries", l1trigger::ncent, l1trigger::fcent.data());
  auto hcent_hlt_eff = new TH1F("hcent_hlt_eff", ";Centrality [%];Entries", l1trigger::ncent, l1trigger::fcent.data());
  auto hcent_hlt_rate = new TH1F("hcent_hlt_rate", ";;Entries", 1, 0, 1);
  auto hcent_hlt_fake = new TH1F("hcent_hlt_fake", ";;Entries", 1, 0, 1);

  std::vector<TH1F*> hZDCdisGeV(l1trigger::nDirs, 0);
  for (int j=0; j<l1trigger::nDirs; j++) {
    hZDCdisGeV[j] = new TH1F(Form("hZDCdisGeV%s", l1trigger::mDir[j].c_str()), Form(";ZDC %s (GeV);Entries", l1trigger::mDir[j].c_str()), 500, 0, 1.e+4);
  }
  std::vector<TH1F*> hrate_And_ZDCAnd(l1trigger::nNeus, 0), hfake_And_ZDCAnd(l1trigger::nNeus, 0),
    hrate_And_ZDCOr(l1trigger::nNeus, 0), hfake_And_ZDCOr(l1trigger::nNeus, 0);
  xjjc::array2D<TH1F*> heff_And_ZDCAnd = xjjc::array2d<TH1F*>(l1trigger::nNeus, l1trigger::ncent),
    heff_And_ZDCOr = xjjc::array2d<TH1F*>(l1trigger::nNeus, l1trigger::ncent),
    heffcent_And_ZDCAnd = xjjc::array2d<TH1F*>(l1trigger::nNeus, l1trigger::nadc),
    heffcent_And_ZDCOr = xjjc::array2d<TH1F*>(l1trigger::nNeus, l1trigger::nadc);
  std::vector<TH1F*> heff_And_ZDCAnd_int(l1trigger::nNeus, 0), heff_And_ZDCOr_int(l1trigger::nNeus, 0);
  for (int k=0; k<l1trigger::nNeus; k++) {
    hrate_And_ZDCAnd[k] = new TH1F(Form("hrate_And_ZDCAnd_%dn", k), ";L1 HF threshold (ADC);", l1trigger::nadc, 0, l1trigger::nadc);
    hfake_And_ZDCAnd[k] = new TH1F(Form("hfake_And_ZDCAnd_%dn", k), ";L1 HF threshold (ADC);", l1trigger::nadc, 0, l1trigger::nadc);
    hrate_And_ZDCOr[k] = new TH1F(Form("hrate_And_ZDCOr_%dn", k), ";L1 HF threshold (ADC);", l1trigger::nadc, 0, l1trigger::nadc);
    hfake_And_ZDCOr[k] = new TH1F(Form("hfake_And_ZDCOr_%dn", k), ";L1 HF threshold (ADC);", l1trigger::nadc, 0, l1trigger::nadc);
    for (int l=0; l<l1trigger::ncent; l++) {
      heff_And_ZDCAnd[k][l] = new TH1F(Form("heff_And_ZDCAnd_%dn_%d", k, l), ";L1 HF threshold (ADC);", l1trigger::nadc, 0, l1trigger::nadc);
      heff_And_ZDCOr[k][l] = new TH1F(Form("heff_And_ZDCOr_%dn_%d", k, l), ";L1 HF threshold (ADC);", l1trigger::nadc, 0, l1trigger::nadc);
    }
    for (int a=0; a<l1trigger::nadc; a++) {
      heffcent_And_ZDCAnd[k][a] = new TH1F(Form("heffcent_And_ZDCAnd_%dn_%d", k, a), ";Centrality [%];", l1trigger::nbincent, 0, 100);
      heffcent_And_ZDCOr[k][a] = new TH1F(Form("heffcent_And_ZDCOr_%dn_%d", k, a), ";Centrality [%];", l1trigger::nbincent, 0, 100);
    }
    heff_And_ZDCAnd_int[k] = new TH1F(Form("heff_And_ZDCAnd_int_%dn", k), ";L1 HF threshold (ADC);", l1trigger::nadc, 0, l1trigger::nadc);
    heff_And_ZDCOr_int[k] = new TH1F(Form("heff_And_ZDCOr_int_%dn", k), ";L1 HF threshold (ADC);", l1trigger::nadc, 0, l1trigger::nadc);
  }
  std::vector<TH1F*> heffden(l1trigger::ncent, 0);
  for (int l=0; l<l1trigger::ncent; l++)
    heffden[l] = new TH1F(Form("heffden_%d", l), ";L1 HF threshold (ADC);", l1trigger::nadc, 0, l1trigger::nadc);
  auto heffden_int = new TH1F("heffden_int", ";L1 HF threshold (ADC);", l1trigger::nadc, 0, l1trigger::nadc);

  // run & fill
  int nentries = tt->GetEntries();
  int nZB_HLT = 0;
  for (int i=0; i<nentries; i++) {

    xjjc::progressslide(i, nentries, 100000);
    nt->GetEntry(i);

    // select ZB events     
    if (!nt->HLT_ZB) continue;
    if (minLS > 0 && nt->br.mLS < minLS) continue;
    if (maxLS > 0 && nt->br.mLS > maxLS) continue;

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
    if (nt->colEvtSel) {
      hcent->Fill(nt->br.mhiBin/2.);
      hcent_hlt_effden->Fill(nt->br.mhiBin);
    }
      
    // rate && eff
    for (int a=0; a<l1trigger::nadc; a++) {
      if (nt->colEvtSel) {
        heffden[icent]->Fill(a);
        heffden_int->Fill(a);
      }
      for (int k=0; k<l1trigger::nNeus; k++) {
        if (nt->AdcAND > a) {
          if (nt->ZDCplus >= l1trigger::mNeuZDCLow[0][k] && nt->ZDCminus >= l1trigger::mNeuZDCLow[1][k]) {
            // ZDC_AND
            hrate_And_ZDCAnd[k]->Fill(a);
            if (nt->colEvtSel) {
              heff_And_ZDCAnd[k][icent]->Fill(a);
              heff_And_ZDCAnd_int[k]->Fill(a);
              heffcent_And_ZDCAnd[k][a]->Fill(nt->br.mhiBin/2.);
            }
            else {
              hfake_And_ZDCAnd[k]->Fill(a);
            }
          } 
          if (nt->ZDCplus >= l1trigger::mNeuZDCLow[0][k] || nt->ZDCminus >= l1trigger::mNeuZDCLow[1][k]) {
            // ZDC_OR
            hrate_And_ZDCOr[k]->Fill(a);
            if (nt->colEvtSel) {
              heff_And_ZDCOr[k][icent]->Fill(a);
              heff_And_ZDCOr_int[k]->Fill(a);
              heffcent_And_ZDCOr[k][a]->Fill(nt->br.mhiBin/2.);
            }
            else {
              hfake_And_ZDCOr[k]->Fill(a);
            }
          } 
        }
      } // for (int k=0; k<l1trigger::nNeus; k++) {
    } //  for (int a=0; a<l1trigger::nadc; a++) {
  }
  xjjc::progressbar_summary(nentries);

  std::cout<<ZBrate*nBunchRatio<<", "<<nZB_HLT<<std::endl;
  
  TH1F* hrateZB = new TH1F("hrateZB", ";L1 HF threshold (ADC);", 1, 0, 1);
  hrateZB->SetBinContent(1, ZBrate*nBunchRatio);
  // fake
  hcent_hlt_fake->Divide(hcent_hlt_rate);
  for (int k=0; k<l1trigger::nNeus; k++) {
    hfake_And_ZDCAnd[k]->Divide(hrate_And_ZDCAnd[k]);
    hfake_And_ZDCOr[k]->Divide(hrate_And_ZDCOr[k]);      
  }
  // rate
  float rate_scale = ZBrate*nBunchRatio/nZB_HLT/1.e+3;
  hcent_hlt_rate->Scale(rate_scale);
  for (auto& h : hrate_And_ZDCAnd) h->Scale(rate_scale);
  for (auto& h : hrate_And_ZDCOr) h->Scale(rate_scale);
  // efficiency
  hcent_hlt_eff->Sumw2();
  hcent_hlt_eff->Divide(hcent_hlt_effden);
  for (int k=0; k<l1trigger::nNeus; k++) {
    for (int l=0; l<l1trigger::ncent; l++) {
      heff_And_ZDCAnd[k][l]->Sumw2();
      heff_And_ZDCAnd[k][l]->Divide(heffden[l]);
      heff_And_ZDCOr[k][l]->Sumw2();
      heff_And_ZDCOr[k][l]->Divide(heffden[l]);
    }
    for (int a=0; a<l1trigger::nadc; a++) {
      heffcent_And_ZDCAnd[k][a]->Sumw2();
      heffcent_And_ZDCAnd[k][a]->Divide(hcent);
      heffcent_And_ZDCOr[k][a]->Sumw2();
      heffcent_And_ZDCOr[k][a]->Divide(hcent);
    }
    heff_And_ZDCAnd_int[k]->Sumw2();
    heff_And_ZDCAnd_int[k]->Divide(heffden_int);
    heff_And_ZDCOr_int[k]->Sumw2();
    heff_And_ZDCOr_int[k]->Divide(heffden_int);
  }

  std::string outputname = "rootfiles/"+outputdir+"/savehist.root";
  xjjroot::mkdir(outputname);
  TFile* outf = new TFile(outputname.c_str(), "recreate");

  for (auto& h : hZDCdisGeV) xjjroot::writehist(h);
  for (auto& h : hrate_And_ZDCAnd) xjjroot::writehist(h);
  for (auto& h : hrate_And_ZDCOr) xjjroot::writehist(h);
  for (auto& h : hfake_And_ZDCAnd) xjjroot::writehist(h);
  for (auto& h : hfake_And_ZDCOr) xjjroot::writehist(h);
  for (auto& h : heff_And_ZDCAnd)
    for (auto& hh : h) xjjroot::writehist(hh);
  for (auto& h : heff_And_ZDCOr)
    for (auto& hh : h) xjjroot::writehist(hh);
  for (auto& h : heffcent_And_ZDCAnd)
    for (auto& hh : h) xjjroot::writehist(hh);
  for (auto& h : heffcent_And_ZDCOr)
    for (auto& hh : h) xjjroot::writehist(hh);
  for (auto& h : heff_And_ZDCAnd_int) xjjroot::writehist(h);
  for (auto& h : heff_And_ZDCOr_int) xjjroot::writehist(h);
  xjjroot::writehist(hrateZB);
  xjjroot::writehist(hcent);
  xjjroot::writehist(hcent_hlt_rate);
  xjjroot::writehist(hcent_hlt_fake);
  xjjroot::writehist(hcent_hlt_eff);
  outf->Close();

  return 0;
}

int main(int argc, char* argv[])
{
  if (argc==2) return macro(argv[1]);
  return 1;
}

