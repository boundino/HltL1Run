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
  float ZBrate = conf.vf("ZBrate"),
    nBunchRatio = conf.vf("nBunchRatio");
  int minLS = conf.vi("minLS"), maxLS = conf.vi("maxLS"),
    MBindex = conf.vi("MBindex");
  std::string inputname = conf["Input"], outputdir = conf["Output"];
  l1trigger::setcent(conf);

  TFile* inf = TFile::Open(inputname.c_str());
  TTree* tt = (TTree*)inf->Get("EvtTowerInfoNTuple");
  l1trigger::evtl1ntuple* nt = new l1trigger::evtl1ntuple(tt);

  // hist
  auto hcent = new TH1F("hcent", ";Centrality [%];Entries", l1trigger::nbincent, 0, 100);
  auto hcent_hlt_effden = new TH1F("hcent_hlt_effden", ";Centrality [%];Entries", l1trigger::ncent, l1trigger::fcent.data());
  auto hcent_hlt_eff = new TH1F("hcent_hlt_eff", ";Centrality [%];Entries", l1trigger::ncent, l1trigger::fcent.data());
  auto hcent_hlt_rate = new TH1F("hcent_hlt_rate", ";;Entries", 1, 0, 1);
  auto hcent_hlt_fake = new TH1F("hcent_hlt_fake", ";;Entries", 1, 0, 1);

  auto hls_fake = new TH1F("hls_fake", ";LS;Fake rate", maxLS+1-minLS, minLS, maxLS+1);
  auto hls_rate = new TH1F("hls_rate", ";LS;Rate (kHz)", maxLS+1-minLS, minLS, maxLS+1);
  std::vector<TH1F*> hZDCdis(l1trigger::nDirs, 0), hZDCdisGeV(l1trigger::nDirs, 0), heffden(l1trigger::ncent, 0), 
    hrate_And_ZDCAnd(l1trigger::nNeus, 0), hrate_And_ZDCAnd_pix(l1trigger::nNeus, 0), hfake_And_ZDCAnd(l1trigger::nNeus, 0),
    hrate_And_ZDCOr(l1trigger::nNeus, 0), hrate_And_ZDCOr_pix(l1trigger::nNeus, 0), hfake_And_ZDCOr(l1trigger::nNeus, 0);
  
  std::vector<TH1F*> hrate_And_ZDCAnd_frac(l1trigger::nNeus, 0), hrate_And_ZDCOr_frac(l1trigger::nNeus, 0);
  xjjc::array2D<TH1F*> heff_And_ZDCAnd = xjjc::array2d<TH1F*>(l1trigger::nNeus, l1trigger::ncent), 
    heff_And_ZDCOr = xjjc::array2d<TH1F*>(l1trigger::nNeus, l1trigger::ncent), 
    heff_And_ZDCAnd_pix = xjjc::array2d<TH1F*>(l1trigger::nNeus, l1trigger::ncent),
    heff_And_ZDCOr_pix = xjjc::array2d<TH1F*>(l1trigger::nNeus, l1trigger::ncent),
    heffcent_And_ZDCAnd = xjjc::array2d<TH1F*>(l1trigger::nNeus, l1trigger::nadc),
    heffcent_And_ZDCOr = xjjc::array2d<TH1F*>(l1trigger::nNeus, l1trigger::nadc);
  std::vector<TH1F*> heffint_And_ZDCAnd(l1trigger::nNeus, 0), heffint_And_ZDCOr(l1trigger::nNeus, 0),
    hfakeden_And_ZDCAnd(l1trigger::nNeus, 0), hfakeden_And_ZDCOr(l1trigger::nNeus, 0);
  for(int j=0; j<l1trigger::nDirs; j++)
    {
      hZDCdis[j] = new TH1F(Form("hZDCdis%s", l1trigger::mDir[j].c_str()), Form(";ZDC %s (a.u.);Entries", l1trigger::mDir[j].c_str()), 500, 0, 1.e+4);
      hZDCdisGeV[j] = new TH1F(Form("hZDCdisGeV%s", l1trigger::mDir[j].c_str()), Form(";ZDC %s (GeV);Entries", l1trigger::mDir[j].c_str()), 500, 0, 1.e+4);
    }
  for(int k=0; k<l1trigger::nNeus; k++)
    {
      hrate_And_ZDCAnd[k] = new TH1F(Form("hrate_And_ZDCAnd_%dn", k), ";L1 HF threshold (ADC);", l1trigger::nadc, 0, l1trigger::nadc);
      hrate_And_ZDCAnd_pix[k] = new TH1F(Form("hrate_And_ZDCAnd_pix_%dn", k), ";L1 HF threshold (ADC);", l1trigger::nadc, 0, l1trigger::nadc);
      hfake_And_ZDCAnd[k] = new TH1F(Form("hfake_And_ZDCAnd_%dn", k), ";L1 HF threshold (ADC);", l1trigger::nadc, 0, l1trigger::nadc);
      hfake_And_ZDCOr[k] = new TH1F(Form("hfake_And_ZDCOr_%dn", k), ";L1 HF threshold (ADC);", l1trigger::nadc, 0, l1trigger::nadc);
      hrate_And_ZDCOr[k] = new TH1F(Form("hrate_And_ZDCOr_%dn", k), ";L1 HF threshold (ADC);", l1trigger::nadc, 0, l1trigger::nadc);
      hrate_And_ZDCOr_pix[k] = new TH1F(Form("hrate_And_ZDCOr_pix_%dn", k), ";L1 HF threshold (ADC);", l1trigger::nadc, 0, l1trigger::nadc);
      for(int l=0; l<l1trigger::ncent; l++)
        {
          heff_And_ZDCAnd[k][l] = new TH1F(Form("heff_And_ZDCAnd_%dn_%d", k, l), ";L1 HF threshold (ADC);", l1trigger::nadc, 0, l1trigger::nadc);
          heff_And_ZDCAnd_pix[k][l] = new TH1F(Form("heff_And_ZDCAnd_pix_%dn_%d", k, l), ";L1 HF threshold (ADC);", l1trigger::nadc, 0, l1trigger::nadc);
          heff_And_ZDCOr[k][l] = new TH1F(Form("heff_And_ZDCOr_%dn_%d", k, l), ";L1 HF threshold (ADC);", l1trigger::nadc, 0, l1trigger::nadc);
          heff_And_ZDCOr_pix[k][l] = new TH1F(Form("heff_And_ZDCOr_pix_%dn_%d", k, l), ";L1 HF threshold (ADC);", l1trigger::nadc, 0, l1trigger::nadc);
        }
      for(int a=0; a<l1trigger::nadc; a++)
        {
          heffcent_And_ZDCAnd[k][a] = new TH1F(Form("heffcent_And_ZDCAnd_%dn_%d", k, a), ";Centrality [%];", l1trigger::nbincent, 0, 100);
          heffcent_And_ZDCOr[k][a] = new TH1F(Form("heffcent_And_ZDCOr_%dn_%d", k, a), ";Centrality [%];", l1trigger::nbincent, 0, 100);
        }
      heffint_And_ZDCAnd[k] = new TH1F(Form("heffint_And_ZDCAnd_%dn", k), ";L1 HF threshold (ADC);", l1trigger::nadc, 0, l1trigger::nadc);
      heffint_And_ZDCOr[k] = new TH1F(Form("heffint_And_ZDCOr_%dn", k), ";L1 HF threshold (ADC);", l1trigger::nadc, 0, l1trigger::nadc);
    }
  for(int l=0; l<l1trigger::ncent; l++)
    heffden[l] = new TH1F(Form("heffden_%d", l), ";L1 HF threshold (ADC);", l1trigger::nadc, 0, l1trigger::nadc);

  // run & fill
  int nentries = tt->GetEntries();
  int nZB_HLT = 0;
  std::vector<int> nZB_HLT_ls(maxLS+1-minLS, 0);
  for(int i=0; i<nentries; i++)
    {
      xjjc::progressslide(i, nentries, 100000);
      nt->GetEntry(i);

      // select ZB events      
      if(!nt->ZB_HLTBit)  continue;
      if (minLS > 0 && nt->mLSNb < minLS) continue;
      if (maxLS > 0 && nt->mLSNb > maxLS) continue;

      // std::cout<<"test"<<std::endl;
      
      nZB_HLT++;
      
      // nZB_HLT_ls[nt->mLSNb-minLS]++;
      int icent = xjjc::findibin<Short_t>(l1trigger::cent, nt->mCenBin);
      // if(icent<0) { /*std::cout<<"error: bad icent. ("<<nt->mCenBin<<")"<<std::endl;*/ continue; }
      if(icent<0) { /*std::cout<<"error: bad icent. ("<<nt->mCenBin<<")"<<std::endl;*/ icent = 0; }

      // std::cout<<nt->mZDCPlus<<std::endl;
      
      // ZDC distribution 
      hZDCdis[0]->Fill(nt->ZDCplus);
      hZDCdis[1]->Fill(nt->ZDCminus);
      hZDCdisGeV[0]->Fill(nt->mZDCRechitPlus);
      hZDCdisGeV[1]->Fill(nt->mZDCRechitMinus);

      // bool hltls = (nt->mLSNb >= minLS && nt->mLSNb <= maxLS);
      if (MBindex >= 0 && nt->mTrigHLT[MBindex]) {
        hcent_hlt_rate->Fill(0);
        hls_rate->Fill(nt->mLSNb);
        if (nt->colEvtSel)
          hcent_hlt_eff->Fill(nt->mCenBin);
        else {
          hcent_hlt_fake->Fill(0);
          hls_fake->Fill(nt->mLSNb);
        }
      }
      if (nt->colEvtSel) {
        hcent->Fill(nt->mCenBin/2.);
        hcent_hlt_effden->Fill(nt->mCenBin);
      }
      // rate && eff
      for(int a=0; a<l1trigger::nadc; a++)
        {
          if(nt->colEvtSel)
            heffden[icent]->Fill(a);

          for(int k=0; k<l1trigger::nNeus; k++)
            {
              if(nt->AdcAND > a)
                {
                  if(nt->ZDCplus >= l1trigger::mNeuZDCLow[0][k] && nt->ZDCminus >= l1trigger::mNeuZDCLow[1][k])
                    {
                      // ZDC_AND
                      hrate_And_ZDCAnd[k]->Fill(a);
                      if(nt->mNtrkoffline > 0)
                        hrate_And_ZDCAnd_pix[k]->Fill(a);
                      if(nt->colEvtSel)
                        {
                          heff_And_ZDCAnd[k][icent]->Fill(a);
                          heffint_And_ZDCAnd[k]->Fill(a);
                          heffcent_And_ZDCAnd[k][a]->Fill(nt->mCenBin/2.);
                          if(nt->mNtrkoffline > 0)
                            heff_And_ZDCAnd_pix[k][icent]->Fill(a);
                        }
                      else {
                          hfake_And_ZDCAnd[k]->Fill(a);
                      }
                    } 
                  if(nt->ZDCplus >= l1trigger::mNeuZDCLow[0][k] || nt->ZDCminus >= l1trigger::mNeuZDCLow[1][k])
                    {
                      // ZDC_OR
                      hrate_And_ZDCOr[k]->Fill(a);
                      // if (k == nneu && a == aHFnow[nneu]) {
                      // }
                      if(nt->mNtrkoffline > 0)
                        hrate_And_ZDCOr_pix[k]->Fill(a);
                      if(nt->colEvtSel)
                        {
                          heff_And_ZDCOr[k][icent]->Fill(a);
                          heffint_And_ZDCOr[k]->Fill(a);
                          heffcent_And_ZDCOr[k][a]->Fill(nt->mCenBin/2.);
                          if(nt->mNtrkoffline > 0)
                            heff_And_ZDCOr_pix[k][icent]->Fill(a);
                        }
                      else {
                        hfake_And_ZDCOr[k]->Fill(a);
                      }
                    } 
                }
            }
        }
    }
  xjjc::progressbar_summary(nentries);

  std::cout<<ZBrate*nBunchRatio<<", "<<nZB_HLT<<std::endl;
  
  TH1F* hrateZB = new TH1F("hrateZB", ";L1 HF threshold (ADC);", 1, 0, 1);
  hrateZB->SetBinContent(1, ZBrate*nBunchRatio);
  // fake
  hcent_hlt_fake->Divide(hcent_hlt_rate);
  hls_fake->Divide(hls_rate);
  for(int k=0; k<l1trigger::nNeus; k++)
    {
      hrate_And_ZDCAnd_frac[k] = (TH1F*)hrate_And_ZDCAnd[k]->Clone(Form("hrate_And_ZDCAnd_frac_%dn", k));
      hrate_And_ZDCOr_frac[k] = (TH1F*)hrate_And_ZDCOr[k]->Clone(Form("hrate_And_ZDCOr_frac_%dn", k));
      hfake_And_ZDCAnd[k]->Divide(hrate_And_ZDCAnd[k]);
      hfake_And_ZDCOr[k]->Divide(hrate_And_ZDCOr[k]);      
    }
  // rate
  float rate_scale = ZBrate*nBunchRatio/nZB_HLT/1.e+3;
  hcent_hlt_rate->Scale(rate_scale);
  // for (int i=0; i<hls_rate->GetXaxis()->GetNbins(); i++) {
  //   float irate_scale = ZBrate*nBunchRatio/nZB_HLT_ls[i]/1.e+3;
  //   hls_rate->SetBinContent(i+1, hls_rate->GetBinContent(i+1)*irate_scale);
  // }
  for(auto& h : hrate_And_ZDCAnd) h->Scale(rate_scale);
  for(auto& h : hrate_And_ZDCAnd_pix) h->Scale(rate_scale);
  for(auto& h : hrate_And_ZDCOr) h->Scale(rate_scale);
  for(auto& h : hrate_And_ZDCOr_pix) h->Scale(rate_scale);
  for(auto& h : hrate_And_ZDCAnd_frac) h->Scale(1./nZB_HLT);
  for(auto& h : hrate_And_ZDCOr_frac) h->Scale(1./nZB_HLT);
  // efficiency
  hcent_hlt_eff->Sumw2();
  hcent_hlt_eff->Divide(hcent_hlt_effden);
  for(int k=0; k<l1trigger::nNeus; k++)
    {
      for(int l=0; l<l1trigger::ncent; l++)
        {
          heff_And_ZDCAnd[k][l]->Sumw2();
          heff_And_ZDCAnd[k][l]->Divide(heffden[l]);
          heff_And_ZDCAnd_pix[k][l]->Sumw2();
          heff_And_ZDCAnd_pix[k][l]->Divide(heffden[l]);
          heff_And_ZDCOr[k][l]->Sumw2();
          heff_And_ZDCOr[k][l]->Divide(heffden[l]);
          heff_And_ZDCOr_pix[k][l]->Sumw2();
          heff_And_ZDCOr_pix[k][l]->Divide(heffden[l]);
        }
      for(int a=0; a<l1trigger::nadc; a++)
        {
          heffcent_And_ZDCAnd[k][a]->Sumw2();
          heffcent_And_ZDCAnd[k][a]->Divide(hcent);
          heffcent_And_ZDCOr[k][a]->Sumw2();
          heffcent_And_ZDCOr[k][a]->Divide(hcent);
        }
    }

  std::string outputname = "rootfiles/"+outputdir+"/savehist.root";
  xjjroot::mkdir(outputname);
  TFile* outf = new TFile(outputname.c_str(), "recreate");

  for(auto& h : hZDCdis) xjjroot::writehist(h);
  for(auto& h : hZDCdisGeV) xjjroot::writehist(h);
  for(auto& h : hrate_And_ZDCAnd) xjjroot::writehist(h);
  for(auto& h : hrate_And_ZDCOr) xjjroot::writehist(h);
  for(auto& h : hrate_And_ZDCAnd_pix) xjjroot::writehist(h);
  for(auto& h : hrate_And_ZDCOr_pix) xjjroot::writehist(h);
  for(auto& h : hrate_And_ZDCAnd_frac) xjjroot::writehist(h);
  for(auto& h : hrate_And_ZDCOr_frac) xjjroot::writehist(h);
  for(auto& h : hfake_And_ZDCAnd) xjjroot::writehist(h);
  for(auto& h : hfake_And_ZDCOr) xjjroot::writehist(h);
  for(auto& h : heff_And_ZDCAnd)
    for(auto& hh : h) xjjroot::writehist(hh);
  for(auto& h : heff_And_ZDCOr)
    for(auto& hh : h) xjjroot::writehist(hh);
  for(auto& h : heff_And_ZDCAnd_pix)
    for(auto& hh : h) xjjroot::writehist(hh);
  for(auto& h : heff_And_ZDCOr_pix)
    for(auto& hh : h) xjjroot::writehist(hh);
  for(auto& h : heffcent_And_ZDCAnd)
    for(auto& hh : h) xjjroot::writehist(hh);
  for(auto& h : heffcent_And_ZDCOr)
    for(auto& hh : h) xjjroot::writehist(hh);
  xjjroot::writehist(hrateZB);
  xjjroot::writehist(hcent);
  xjjroot::writehist(hcent_hlt_rate);
  xjjroot::writehist(hcent_hlt_fake);
  xjjroot::writehist(hcent_hlt_eff);
  xjjroot::writehist(hls_rate);
  xjjroot::writehist(hls_fake);
  outf->Close();

  return 0;
}

int main(int argc, char* argv[])
{
  if(argc==2) return macro(argv[1]);
  return 1;
}

