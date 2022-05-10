#include "evtl1ntuple.h"
#include <TFile.h>
#include <TTree.h>
#include <TH1F.h>
#include <string>
#include "xjjcuti.h"
#include "xjjrootuti.h"
#include "constant.h"

int macro(std::string inputname, std::string outputdir, float ZBrate=1.e+7)
{
  TFile* inf = TFile::Open(inputname.c_str());
  TTree* tt = (TTree*)inf->Get("EvtTowerInfoNTuple");
  l1trigger::evtl1ntuple* nt = new l1trigger::evtl1ntuple(tt);

  // hist
  TH1F* hcent = new TH1F("hcent", ";Centrality;Entries", l1trigger::nbincent, 0, 100);
  std::vector<TH1F*> hZDCdis(l1trigger::nDirs, 0), heffden(l1trigger::ncent, 0), 
    hrate_And_ZDCAnd(l1trigger::nNeus, 0), hrate_And_ZDCAnd_pix(l1trigger::nNeus, 0),
    hrate_And_ZDCOr(l1trigger::nNeus, 0), hrate_And_ZDCOr_pix(l1trigger::nNeus, 0);
  xjjc::array2D<TH1F*> heff_And_ZDCAnd = xjjc::array2d<TH1F*>(l1trigger::nNeus, l1trigger::ncent), 
    heff_And_ZDCOr = xjjc::array2d<TH1F*>(l1trigger::nNeus, l1trigger::ncent), 
    heff_And_ZDCAnd_pix = xjjc::array2d<TH1F*>(l1trigger::nNeus, l1trigger::ncent),
    heff_And_ZDCOr_pix = xjjc::array2d<TH1F*>(l1trigger::nNeus, l1trigger::ncent),
    heffcent_And_ZDCAnd = xjjc::array2d<TH1F*>(l1trigger::nNeus, l1trigger::nadc),
    heffcent_And_ZDCOr = xjjc::array2d<TH1F*>(l1trigger::nNeus, l1trigger::nadc);
  for(int j=0; j<l1trigger::nDirs; j++)
    hZDCdis[j] = new TH1F(Form("hZDCdis%s", l1trigger::mDir[j].c_str()), Form(";ZDC %s (a.u.);Entries", l1trigger::mDir[j].c_str()), 500, 0, 5.e+4);
  for(int k=0; k<l1trigger::nNeus; k++)
    {
      hrate_And_ZDCAnd[k] = new TH1F(Form("hrate_And_ZDCAnd_%dn", k), ";L1 HF threshold (ADC);", l1trigger::nadc, 0, l1trigger::nadc);
      hrate_And_ZDCAnd_pix[k] = new TH1F(Form("hrate_And_ZDCAnd_pix_%dn", k), ";L1 HF threshold (ADC);", l1trigger::nadc, 0, l1trigger::nadc);
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
          heffcent_And_ZDCAnd[k][a] = new TH1F(Form("heffcent_And_ZDCAnd_%dn_%d", k, a), ";Centrality;", l1trigger::nbincent, 0, 100);
          heffcent_And_ZDCOr[k][a] = new TH1F(Form("heffcent_And_ZDCOr_%dn_%d", k, a), ";Centrality;", l1trigger::nbincent, 0, 100);
        }
    }
  for(int l=0; l<l1trigger::ncent; l++)
    heffden[l] = new TH1F(Form("heffden_%d", l), ";L1 HF threshold (ADC);", l1trigger::nadc, 0, l1trigger::nadc);

  // run & fill
  int nentries = tt->GetEntries();
  int nZB_HLT = 0;
  for(int i=0; i<nentries; i++)
    {
      xjjc::progressbar(i, nentries, 100000);
      nt->GetEntry(i);

      // select ZB events      
      if(!nt->ZB_HLTBit) continue;
      nZB_HLT++;
      int icent = xjjc::findibin<Short_t>(l1trigger::cent, nt->mCenBin);
      if(icent<0) { std::cout<<"error: bad icent. ("<<nt->mCenBin<<")"<<std::endl; continue; }

      // ZDC distribution 
      hZDCdis[0]->Fill(nt->mZDCPlus);
      hZDCdis[1]->Fill(nt->mZDCMinus);

      if(nt->colEvtSel) hcent->Fill(nt->mCenBin/2.);

      // rate && eff
      for(int a=0; a<l1trigger::nadc; a++)
        {
          if(nt->colEvtSel)
            heffden[icent]->Fill(a);
          for(int k=0; k<l1trigger::nNeus; k++)
            {
              if(nt->AdcAND >= a)
                {
                  if(nt->mZDCPlus >= l1trigger::mNeuZDCLow[0][k] && nt->mZDCMinus >= l1trigger::mNeuZDCLow[1][k])
                    {
                      // ZDC_AND
                      hrate_And_ZDCAnd[k]->Fill(a);
                      if(nt->mNtrkoffline > 0)
                        hrate_And_ZDCAnd_pix[k]->Fill(a);
                      if(nt->colEvtSel)
                        {
                          heff_And_ZDCAnd[k][icent]->Fill(a);
                          heffcent_And_ZDCAnd[k][a]->Fill(nt->mCenBin/2.);
                          if(nt->mNtrkoffline > 0)
                            heff_And_ZDCAnd_pix[k][icent]->Fill(a);
                        }
                    } 
                  if(nt->mZDCPlus >= l1trigger::mNeuZDCLow[0][k] || nt->mZDCMinus >= l1trigger::mNeuZDCLow[1][k])
                    {
                      // ZDC_OR
                      hrate_And_ZDCOr[k]->Fill(a);
                      if(nt->mNtrkoffline > 0)
                        hrate_And_ZDCOr_pix[k]->Fill(a);
                      if(nt->colEvtSel)
                        {
                          heff_And_ZDCOr[k][icent]->Fill(a);
                          heffcent_And_ZDCOr[k][a]->Fill(nt->mCenBin/2.);
                          if(nt->mNtrkoffline > 0)
                            heff_And_ZDCOr_pix[k][icent]->Fill(a);
                        }
                    } 
                }
            }
        }
    }
  xjjc::progressbar_summary(nentries);

  TH1F* hrateZB = new TH1F("hrateZB", ";L1 HF threshold (ADC);", 1, 0, 1);
  hrateZB->SetBinContent(1, ZBrate);
  for(auto& h : hrate_And_ZDCAnd) h->Scale(ZBrate/nZB_HLT/1.e+3);
  for(auto& h : hrate_And_ZDCAnd_pix) h->Scale(ZBrate/nZB_HLT/1.e+3);
  for(auto& h : hrate_And_ZDCOr) h->Scale(ZBrate/nZB_HLT/1.e+3);
  for(auto& h : hrate_And_ZDCOr_pix) h->Scale(ZBrate/nZB_HLT/1.e+3);
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
  for(auto& h : hrate_And_ZDCAnd) xjjroot::writehist(h);
  for(auto& h : hrate_And_ZDCOr) xjjroot::writehist(h);
  for(auto& h : hrate_And_ZDCAnd_pix) xjjroot::writehist(h);
  for(auto& h : hrate_And_ZDCOr_pix) xjjroot::writehist(h);
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
  outf->Close();

  return 0;
}

int main(int argc, char* argv[])
{
  if(argc==4) return macro(argv[1], argv[2], atof(argv[3]));
  if(argc==3) return macro(argv[1], argv[2]);
  return 1;
}

