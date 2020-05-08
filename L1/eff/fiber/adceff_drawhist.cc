#include <TFile.h>
#include <TH2F.h>
#include <TEfficiency.h>
#include <TLegend.h>
#include <TCanvas.h>
#include <TGraph.h>

#include <vector>
#include <string>

#include "xjjrootuti.h"
#include "xjjcuti.h"
#include "adceff.h"

void adceff_drawhist(std::string tag="2017 XeXe MinBias")
{
  std::string tagname = xjjc::str_replaceallspecial(tag);
  std::string inputname = "rootfiles/"+tagname+"/savehist.root";
  TFile* inf = TFile::Open(inputname.c_str());

  TH2F* hempty = new TH2F("hempty", ";HF threshold (ADC);Efficiency", 10, 0, adceff::nbin, 10, 0, 1.2);
  xjjroot::sethempty(hempty, 0, 0.04);
  TH1F* hEffRate = (TH1F*)inf->Get("hEffRate");
  xjjroot::sethempty(hEffRate);
  xjjroot::setthgrstyle(hEffRate, kBlack, 21, 1, kBlack);
  TH1F* hfibEffRate = (TH1F*)inf->Get("hfibEffRate");
  xjjroot::sethempty(hfibEffRate);
  xjjroot::setthgrstyle(hfibEffRate, kBlack, 25, 1, kBlack);

  TEfficiency* hEffAndEvtfil = (TEfficiency*)inf->Get("hEffAndEvtfil");
  xjjroot::setthgrstyle(hEffAndEvtfil, kBlack, 21, 0.9, kBlack);
  std::vector<TEfficiency*> hEffAndEvtfilcent(adceff::nhiBins);
  for(int k=0;k<adceff::nhiBins;k++) { hEffAndEvtfilcent[adceff::nhiBins-1-k] = (TEfficiency*)inf->Get(Form("hEffAndEvtfilcent%d",adceff::nhiBins-1-k)); 
    xjjroot::setthgrstyle(hEffAndEvtfilcent[adceff::nhiBins-1-k], xjjroot::mycolor_middle[xjjroot::cc[k]], 21, 1, xjjroot::mycolor_middle[xjjroot::cc[k]]); }
  TEfficiency* hfibEffAndEvtfil = (TEfficiency*)inf->Get("hfibEffAndEvtfil");
  xjjroot::setthgrstyle(hfibEffAndEvtfil, kBlack, 25, 0.9, kBlack);
  std::vector<TEfficiency*> hfibEffAndEvtfilcent(adceff::nhiBins);
  for(int k=0;k<adceff::nhiBins;k++) { hfibEffAndEvtfilcent[adceff::nhiBins-1-k] = (TEfficiency*)inf->Get(Form("hfibEffAndEvtfilcent%d",adceff::nhiBins-1-k)); 
    xjjroot::setthgrstyle(hfibEffAndEvtfilcent[adceff::nhiBins-1-k], xjjroot::mycolor_middle[xjjroot::cc[k]], 25, 1, xjjroot::mycolor_middle[xjjroot::cc[k]]); }

  std::vector<float> xhEff, yhEff, xhfibEff, yhfibEff;
  for(int i=0; i<adceff::nbin; i++)
    {
      xhEff.push_back(hEffAndEvtfil->GetEfficiency(i+1));
      yhEff.push_back(hEffRate->GetBinContent(i+1));
      xhfibEff.push_back(hfibEffAndEvtfil->GetEfficiency(i+1));
      yhfibEff.push_back(hfibEffRate->GetBinContent(i+1));
    }
  TGraph* grhEff = new TGraph(adceff::nbin, xhEff.data(), yhEff.data());
  grhEff->SetName("grhEff");
  xjjroot::setthgrstyle(grhEff, kBlack, 20, 1);
  TGraph* grhfibEff = new TGraph(adceff::nbin, xhfibEff.data(), yhfibEff.data());
  grhfibEff->SetName("grhfibEff");
  xjjroot::setthgrstyle(grhfibEff, kBlack, 24, 1);
  TH2F* hemptygr = new TH2F("hemptygr", ";Efficiency;Rate", 10, 0.5, 1, 10, 1.e-4, 2);
  xjjroot::sethempty(hemptygr, 0, 0);
  hemptygr->GetXaxis()->SetNdivisions(-505);

  TLegend* legEff = new TLegend(0.20, 0.20, 0.55, 0.20+0.045*2);
  xjjroot::setleg(legEff);
  legEff->AddEntry(hEffAndEvtfil, "Channel", "pl");
  legEff->AddEntry(hfibEffAndEvtfil, "Fiber", "pl");
  TLegend* legEffRate = new TLegend(0.20, 0.75, 0.55, 0.75+0.045*2);
  xjjroot::setleg(legEffRate);
  legEffRate->AddEntry(grhEff, "Channel", "p");
  legEffRate->AddEntry(grhfibEff, "Fiber", "p");

  xjjroot::mkdir(std::string("plots/"+tagname+"/x"));
  xjjroot::setgstyle(1);

  TCanvas* cgrhEff = new TCanvas("cgrhEff", "", 600, 600);
  cgrhEff->SetLogy();
  hemptygr->Draw();
  grhEff->Draw("p same");
  grhfibEff->Draw("p same");
  xjjroot::drawCMSleft();
  xjjroot::drawCMSright(tag.c_str());
  legEffRate->Draw();
  cgrhEff->SaveAs(Form("plots/%s/cgrhEff.pdf", tagname.c_str()));  

  TCanvas* chEffRate = new TCanvas("chEffRate", "", 600, 600);
  chEffRate->SetLogy();
  hEffRate->SetMaximum(3);
  hEffRate->Draw("pe");
  hfibEffRate->Draw("pe same");
  xjjroot::drawCMSleft();
  xjjroot::drawCMSright(tag.c_str());
  // xjjroot::drawtex(0.21, 0.85, "collisionEventSelection", 0.04, 12, 62);
  legEff->Draw();
  chEffRate->SaveAs(Form("plots/%s/chEffRate.pdf", tagname.c_str()));  

  TCanvas* chEffEvtfil = new TCanvas("chEffEvtfil", "", 600, 600);
  hempty->Draw();
  hEffAndEvtfil->Draw("ep same");
  hfibEffAndEvtfil->Draw("ep same");
  xjjroot::drawCMSleft();
  xjjroot::drawCMSright(tag.c_str());
  xjjroot::drawtex(0.21, 0.85, "collisionEventSelection", 0.04, 12, 62);
  legEff->Draw();
  chEffEvtfil->SaveAs(Form("plots/%s/chEffAndEvtfil.pdf", tagname.c_str()));  

  // CENT
  TLegend* legEffcent = new TLegend(0.20, 0.20, 0.55, 0.20+7*0.045);
  xjjroot::setleg(legEffcent);
  for(int k=0; k<adceff::nhiBins; k++) { legEffcent->AddEntry(hEffAndEvtfilcent[k], Form("%.0f-%.0f%s", adceff::hiBins[k]/2., adceff::hiBins[k+1]/2., "%"), "pl"); }
  TLegend* legAnd = new TLegend(0.65, 0.82, 0.93, 0.86);
  xjjroot::setleg(legAnd);
  legAnd->SetNColumns(2);
  legAnd->AddEntry(hEffAndEvtfil, "Channel", "p");
  legAnd->AddEntry(hfibEffAndEvtfil, "Fiber", "p");

  TCanvas* chEffEvtfilcent = new TCanvas("chEffEvtfilcent", "",600, 600);
  hempty->Draw();
  for(int k=0; k<adceff::nhiBins; k++) { hEffAndEvtfilcent[k]->Draw("ep same"); }
  for(int k=0; k<adceff::nhiBins; k++) { hfibEffAndEvtfilcent[k]->Draw("ep same"); }
  xjjroot::drawCMSleft();
  xjjroot::drawCMSright(tag.c_str());
  xjjroot::drawtex(0.21, 0.85, "collisionEventSelection", 0.04, 12, 62);
  legEffcent->Draw();
  legAnd->Draw();
  chEffEvtfilcent->SaveAs(Form("plots/%s/chEffEvtfilcent.pdf", tagname.c_str()));  
}

int main(int argc, char* argv[])
{
  if(argc==2) { adceff_drawhist(argv[1]); return 0; }
  return 1;
}
