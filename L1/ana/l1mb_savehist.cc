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
  con.init(conf);
  if (!con.valid()) return 2;
  std::string inputname = conf.get("Input"), outputdir = con.label;

  std::cout<<std::endl<<"  [ "<<(con.isMC() ? "MC" : "data")<<" ]"<<std::endl<<std::endl;
  
  auto inf = TFile::Open(inputname.c_str());
  auto tt = (TTree*)inf->Get("mbnt");
  auto nt = new l1trigger::evtl1ntuple(tt);

  // hist
  auto hcent = new TH1F("hcent", ";Centrality [%];Entries", l1trigger::nbincent, 0, 100);
  auto hcent_hlt_effden = new TH1F("hcent_hlt_effden", ";Centrality [%];Entries", l1trigger::ncent, l1trigger::fcent.data());
  auto hcent_hlt_eff = new TH1F("hcent_hlt_eff", ";Centrality [%];Entries", l1trigger::ncent, l1trigger::fcent.data());
  auto hcent_hlt_rate = new TH1F("hcent_hlt_rate", ";;Entries", 1, 0, 1);
  auto hcent_hlt_fake = new TH1F("hcent_hlt_fake", ";;Entries", 1, 0, 1);

  create_hist();
  
  // run & fill
  auto nentries = tt->GetEntries();
  int nZB_HLT = 0, ncolEvtSel = 0;
  for (int i=0; i<nentries; i++) {

    xjjc::progressslide(i, nentries, 100000);
    nt->GetEntry(i);

    // select ZB events
    if (!nt->HLT_ZB) continue;
    if (con.minLS > 0 && nt->br.mLS < con.minLS) continue;
    if (con.maxLS > 0 && nt->br.mLS > con.maxLS) continue;
    
    nZB_HLT++;
      
    int icent = xjjc::findibin<Short_t>(l1trigger::cent, nt->br.mhiBin);
    if (icent<0) { /*std::cout<<"error: bad icent. ("<<nt->br.mhiBin<<")"<<std::endl;*/ icent = 0; }

    // ZDC distribution 
    hZDCdisGeV[0]->Fill(nt->ZDCplus);
    hZDCdisGeV[1]->Fill(nt->ZDCminus);

    // hZDCdisGeV[0]->Fill(nt->br.mhiHFPlus, nt->ZDCplus);
    // hZDCdisGeV[1]->Fill(nt->br.mhiHFMinus, nt->ZDCminus);

    // HLT
    if (con.MBindex >= 0 && nt->br.mTrigHLT[con.MBindex]) {
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
      ncolEvtSel++;
    }
      
    // rate && eff
    for (int a=0; a<l1trigger::nadc; a++) {
      if (nt->colEvtSel) {
        heffden[icent]->Fill(a);
        heffden_incl->Fill(a);
      }
      for (int k=0; k<l1trigger::nNeus; k++) {
        if (nt->AdcAND > a) {
          if (nt->ZDCplus >= con.ZDCthre[0][k] && nt->ZDCminus >= con.ZDCthre[1][k]) {
            // And_ZDCAnd
            hrate_And_ZDCAnd[k]->Fill(a);
            if (nt->colEvtSel) {
              heff_And_ZDCAnd[k][icent]->Fill(a);
              heff_And_ZDCAnd_incl[k]->Fill(a);
              heffcent_And_ZDCAnd[k][a]->Fill(nt->br.mhiBin/2.);
            }
            else {
              hfake_And_ZDCAnd[k]->Fill(a);
            }
          } 
          if (nt->ZDCplus >= con.ZDCthre[0][k] || nt->ZDCminus >= con.ZDCthre[1][k]) {
            // And_ZDCOr
            hrate_And_ZDCOr[k]->Fill(a);
            if (nt->colEvtSel) {
              heff_And_ZDCOr[k][icent]->Fill(a);
              heff_And_ZDCOr_incl[k]->Fill(a);
              heffcent_And_ZDCOr[k][a]->Fill(nt->br.mhiBin/2.);
            }
            else {
              hfake_And_ZDCOr[k]->Fill(a);
            }
          }
        } // if (nt->AdcAND > a) {
        if (nt->AdcOR > a) {
          if (nt->ZDCplus >= con.ZDCthre[0][k] && nt->ZDCminus >= con.ZDCthre[1][k]) {
            // Or_ZDCAnd
            hrate_Or_ZDCAnd[k]->Fill(a);
            if (nt->colEvtSel) {
              heff_Or_ZDCAnd[k][icent]->Fill(a);
              heff_Or_ZDCAnd_incl[k]->Fill(a);
              heffcent_Or_ZDCAnd[k][a]->Fill(nt->br.mhiBin/2.);
            }
            else {
              hfake_Or_ZDCAnd[k]->Fill(a);
            }
          }          
          if (nt->ZDCplus >= con.ZDCthre[0][k] || nt->ZDCminus >= con.ZDCthre[1][k]) {
            // Or_ZDCOr
            hrate_Or_ZDCOr[k]->Fill(a);
            if (nt->colEvtSel) {
              heff_Or_ZDCOr[k][icent]->Fill(a);
              heff_Or_ZDCOr_incl[k]->Fill(a);
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

  auto* hrateZB = new TH1F("hrateZB", ";L1 HF threshold (ADC);", 1, 0, 1);
  hrateZB->SetBinContent(1, con.ZBrate);
  // fake
  hcent_hlt_fake->Divide(hcent_hlt_rate);
  for (int k=0; k<l1trigger::nNeus; k++) {
#define DIVIDEEFF(h, z) hfake_##h##_ZDC##z[k]->Divide(hrate_##h##_ZDC##z[k]);
    COMBINE(DIVIDEEFF);
  }
  // rate
  float rate_scale = con.ZBrate/nZB_HLT/1.e+3;
  hcent_hlt_rate->Scale(rate_scale);
#define SCALERATE(h, z) for (auto& h : hrate_##h##_ZDC##z) h->Scale(rate_scale);
  COMBINE(SCALERATE);

  // efficiency
  hcent_hlt_eff->Sumw2();
  hcent_hlt_eff->Divide(hcent_hlt_effden);
  for (int l=0; l<l1trigger::ncent; l++) {
    if (l >= l1trigger::l_interest) continue;
    heffden_interest->Add(heffden[l]);
    for (int k=0; k<l1trigger::nNeus; k++) {
#define ADDINTEREST(h, z) heff_##h##_ZDC##z##_interest[k]->Add(heff_##h##_ZDC##z[k][l]);
      COMBINE(ADDINTEREST);
    }
  }
#define MAKEEFF(h, z)                                           \
  for (int l=0; l<l1trigger::ncent; l++) {                      \
    heff_##h##_ZDC##z[k][l]->Sumw2();                           \
    heff_##h##_ZDC##z[k][l]->Divide(heffden[l]);                \
  }                                                             \
  for (int a=0; a<l1trigger::nadc; a++) {                       \
    heffcent_##h##_ZDC##z[k][a]->Sumw2();                       \
    heffcent_##h##_ZDC##z[k][a]->Divide(hcent);                 \
  }                                                             \
  heff_##h##_ZDC##z##_incl[k]->Sumw2();                         \
  heff_##h##_ZDC##z##_incl[k]->Divide(heffden_incl);            \
  heff_##h##_ZDC##z##_interest[k]->Sumw2();                     \
  heff_##h##_ZDC##z##_interest[k]->Divide(heffden_interest);    \

  for (int k=0; k<l1trigger::nNeus; k++) {
    COMBINE(MAKEEFF);
  }

  std::string outputname = "rootfiles/" + outputdir + "/savehist.root";
  xjjroot::mkdir(outputname);
  auto outf = new TFile(outputname.c_str(), "recreate");

  write_hist();

  xjjroot::writehist(hrateZB);
  xjjroot::writehist(hcent);
  xjjroot::writehist(hcent_hlt_rate);
  xjjroot::writehist(hcent_hlt_fake);
  xjjroot::writehist(hcent_hlt_eff);
  outf->Close();

  std::cout<<"ZB count: \e[4m"<<nZB_HLT<<"\e[0m"<<std::endl;
  std::cout<<"ZB rate: \e[4m"<<con.ZBrate<<"\e[0m"<<std::endl;
  std::cout<<"Event sel count: \e[4m"<<ncolEvtSel<<"\e[0m"<<std::endl;
  
  return 0;
}

int main(int argc, char* argv[])
{
  if (argc==2) return macro(argv[1]);
  return 1;
}

