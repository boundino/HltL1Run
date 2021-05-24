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

  // 
  TH1F* hEffRate = (TH1F*)inf->Get("hEffRate");
  xjjroot::setthgrstyle(hEffRate, kBlack, 21, 1, kBlack);
  TH1F* hfibEffRate = (TH1F*)inf->Get("hfibEffRate");
  xjjroot::setthgrstyle(hfibEffRate, kBlack, 21, 1, kBlack);

  TEfficiency* hEffAndEvtfil = (TEfficiency*)inf->Get("hEffAndEvtfil");
  xjjroot::setthgrstyle(hEffAndEvtfil, kBlack, 21, 0.9, kBlack);
  std::vector<TEfficiency*> hEffAndEvtfilcent(adceff::nhiBins);
  for(int k=0;k<adceff::nhiBins;k++) { hEffAndEvtfilcent[adceff::nhiBins-1-k] = (TEfficiency*)inf->Get(Form("hEffAndEvtfilcent%d",adceff::nhiBins-1-k)); 
    xjjroot::setthgrstyle(hEffAndEvtfilcent[adceff::nhiBins-1-k], xjjroot::colorlist_middle[k], 21, 1, xjjroot::colorlist_middle[k]); }
  TEfficiency* hfibEffAndEvtfil = (TEfficiency*)inf->Get("hfibEffAndEvtfil");
  xjjroot::setthgrstyle(hfibEffAndEvtfil, kBlack, 25, 0.9, kBlack);
  std::vector<TEfficiency*> hfibEffAndEvtfilcent(adceff::nhiBins);
  for(int k=0;k<adceff::nhiBins;k++) { hfibEffAndEvtfilcent[adceff::nhiBins-1-k] = (TEfficiency*)inf->Get(Form("hfibEffAndEvtfilcent%d",adceff::nhiBins-1-k)); 
    xjjroot::setthgrstyle(hfibEffAndEvtfilcent[adceff::nhiBins-1-k], xjjroot::colorlist_middle[k], 21, 1, xjjroot::colorlist_middle[k]); }
  TEfficiency* hEffAndEvtfil8595 = (TEfficiency*)inf->Get("hEffAndEvtfil8595");
  TEfficiency* hfibEffAndEvtfil8595 = (TEfficiency*)inf->Get("hfibEffAndEvtfil8595");

  std::vector<float> yhEff, yhfibEff, 
    yhEff_8595, yhfibEff_8595,
    xhRate, xhfibRate;
  for(int i=0; i<adceff::nbin; i++)
    {
      xhRate.push_back(hEffRate->GetBinContent(i+1) * 1.e+4);
      yhEff.push_back(hEffAndEvtfil->GetEfficiency(i+1));
      yhEff_8595.push_back(hEffAndEvtfil8595->GetEfficiency(i+1));
    }
  for(int i=0; i<adceff::nbinfib; i++)
    {
      xhfibRate.push_back(hfibEffRate->GetBinContent(i+1) * 1.e+4);
      yhfibEff.push_back(hfibEffAndEvtfil->GetEfficiency(i+1));
      yhfibEff_8595.push_back(hfibEffAndEvtfil8595->GetEfficiency(i+1));
    }
  TGraph* grhEff = new TGraph(adceff::nbin, xhRate.data(), yhEff.data());
  grhEff->SetName("grhEff");
  xjjroot::setthgrstyle(grhEff, xjjroot::mycolor_satmiddle2["blue"], 24, 1, xjjroot::mycolor_satmiddle2["blue"], 1, 1);
  TGraph* grhfibEff = new TGraph(adceff::nbinfib, xhfibRate.data(), yhfibEff.data());
  grhfibEff->SetName("grhfibEff");
  xjjroot::setthgrstyle(grhfibEff, xjjroot::mycolor_satmiddle2["red"], 24, 1, xjjroot::mycolor_satmiddle2["red"], 1, 1);

  TGraph* grhEff_8595 = new TGraph(adceff::nbin, xhRate.data(), yhEff_8595.data());
  grhEff_8595->SetName("grhEff_8595");
  xjjroot::setthgrstyle(grhEff_8595, grhEff->GetMarkerColor(), 20, 1, grhEff->GetMarkerColor(), 1, 1);
  TGraph* grhfibEff_8595 = new TGraph(adceff::nbinfib, xhfibRate.data(), yhfibEff_8595.data());
  grhfibEff_8595->SetName("grhfibEff_8595");
  xjjroot::setthgrstyle(grhfibEff_8595, grhfibEff->GetMarkerColor(), 20, 1, grhfibEff->GetMarkerColor(), 1, 1);

  //
  TH2F* hemptyeff = new TH2F("hemptyeff", ";HF threshold (ADC);Efficiency", 10, 0, adceff::nbin, 10, 0, 1.2);
  xjjroot::sethempty(hemptyeff, 0, 0.06);
  TH2F* hemptyefffib = new TH2F("hemptyefffib", ";HF threshold (ADC);Efficiency", 10, 0, adceff::nbinfib, 10, 0, 1.2);
  xjjroot::sethempty(hemptyefffib, 0, 0.06);
  TH2F* hemptyrate = new TH2F("hemptyrate", ";HF threshold (ADC);Rate / ZB rate", 10, 0, adceff::nbin, 10, 1.e-4, 2);
  xjjroot::sethempty(hemptyrate, 0, 0.06);
  TH2F* hemptyratefib = new TH2F("hemptyratefib", ";HF threshold (ADC);Rate / ZB rate", 10, 0, adceff::nbinfib, 10, 1.e-4, 2);
  xjjroot::sethempty(hemptyratefib, 0, 0.06);
  // TH2F* hemptygr = new TH2F("hemptygr", ";Rate / ZB rate #times 10^{4};Efficiency", 10, std::min(xhRate.back(), xhfibRate.back())/2, 1.e+4, 10, 0.2, 1.04);
  TH2F* hemptygr = new TH2F("hemptygr", ";Rate / ZB rate #times 10^{4};Efficiency", 10, xhRate.back()/2., 1.e+4, 10, 0.2, 1.04);
  xjjroot::sethempty(hemptygr, 0, 0.06);
  hemptygr->GetXaxis()->SetNdivisions(-505);
  TH2F* hemptygr_zoom = new TH2F("hemptygr_zoom", ";Rate / ZB rate #times 10^{4};Efficiency", 10, std::ceil(xhRate.back())-1, std::ceil(xhRate.back())+29, 10, 0.5, 1);
  xjjroot::sethempty(hemptygr_zoom, 0, 0.06);
  hemptygr_zoom->GetXaxis()->SetNdivisions(506);

  //
  TLegend* legEff = new TLegend(0.20, 0.20, 0.55, 0.20+0.045*2);
  xjjroot::setleg(legEff);
  legEff->AddEntry(hEffAndEvtfil, "Channel", "pl");
  legEff->AddEntry(hfibEffAndEvtfil, "Fiber", "pl");
  TLegend* legEffRate = new TLegend(0.50, 0.30, 0.85, 0.30+0.045*3);
  legEffRate->SetNColumns(2);
  xjjroot::setleg(legEffRate);
  legEffRate->AddEntry((TObject*)0, "#bf{MB}", NULL);
  legEffRate->AddEntry((TObject*)0, "#bf{85-95%}", NULL);
  legEffRate->AddEntry(grhEff, "Channel", "p");
  legEffRate->AddEntry(grhEff_8595, "Channel", "p");
  legEffRate->AddEntry(grhfibEff, "Fiber", "p");
  legEffRate->AddEntry(grhfibEff_8595, "Fiber", "p");

  xjjroot::mkdir(std::string("plots/"+tagname+"/x"));
  xjjroot::setgstyle(1);

  // 2D
  TCanvas* cgrhEff = new TCanvas("cgrhEff", "", 700, 600);
  cgrhEff->SetLogx();
  hemptygr->Draw();
  xjjroot::drawCMSleft();
  xjjroot::drawCMSright(tag.c_str());
  cgrhEff->SaveAs(Form("plots/%s/cgrhEff_empty.pdf", tagname.c_str()));  
  grhEff->Draw("pl same");
  grhfibEff->Draw("pl same");
  grhEff_8595->Draw("pl same");
  grhfibEff_8595->Draw("pl same");
  legEffRate->Draw();
  cgrhEff->SaveAs(Form("plots/%s/cgrhEff.pdf", tagname.c_str()));  

  TCanvas* cgrhEff_zoom = new TCanvas("cgrhEff_zoom", "", 700, 600);
  hemptygr_zoom->Draw();
  xjjroot::drawCMSleft();
  xjjroot::drawCMSright(tag.c_str());
  grhEff->Draw("pl same");
  grhfibEff->Draw("pl same");
  grhEff_8595->Draw("pl same");
  grhfibEff_8595->Draw("pl same");
  legEffRate->Draw();
  cgrhEff_zoom->SaveAs(Form("plots/%s/cgrhEff_zoom.pdf", tagname.c_str()));

  // Rate
  TCanvas* chEffRate = new TCanvas("chEffRate", "", 700, 600);
  chEffRate->SetLogy();
  hemptyrate->Draw();
  adceff::drawthre(hemptyrate);
  hEffRate->Draw("pe same");
  xjjroot::drawCMSleft();
  xjjroot::drawCMSright(tag.c_str());
  xjjroot::drawtex(0.88, 0.84, "Channel", 0.04, 32, 62);
  legEff->Draw();
  chEffRate->SaveAs(Form("plots/%s/chEffRate.pdf", tagname.c_str()));  

  TCanvas* chEffRatefib = new TCanvas("chEffRatefib", "", 700, 600);
  chEffRatefib->SetLogy();
  hemptyratefib->Draw();
  hfibEffRate->Draw("pe same");
  xjjroot::drawCMSleft();
  xjjroot::drawCMSright(tag.c_str());
  xjjroot::drawtex(0.88, 0.84, "Fiber", 0.04, 32, 62);
  legEff->Draw();
  chEffRatefib->SaveAs(Form("plots/%s/chEffRatefib.pdf", tagname.c_str()));  

  // Efficiency
  TCanvas* chEffEvtfil = new TCanvas("chEffEvtfil", "", 700, 600);
  hemptyeff->Draw();
  adceff::drawthre(hemptyeff);
  hEffAndEvtfil->Draw("ep same");
  xjjroot::drawCMSleft();
  xjjroot::drawCMSright(tag.c_str());
  xjjroot::drawtex(0.21, 0.84, "collisionEventSelection", 0.04, 12, 62);
  xjjroot::drawtex(0.88, 0.84, "Channel", 0.04, 32, 62);
  // legEff->Draw();
  chEffEvtfil->SaveAs(Form("plots/%s/chEffAndEvtfil.pdf", tagname.c_str()));  

  TCanvas* chEffEvtfilfib = new TCanvas("chEffEvtfilfib", "", 700, 600);
  hemptyefffib->Draw();
  hfibEffAndEvtfil->Draw("ep same");
  xjjroot::drawCMSleft();
  xjjroot::drawCMSright(tag.c_str());
  xjjroot::drawtex(0.21, 0.84, "collisionEventSelection", 0.04, 12, 62);
  xjjroot::drawtex(0.88, 0.84, "Fiber", 0.04, 32, 62);
  // legEff->Draw();
  chEffEvtfilfib->SaveAs(Form("plots/%s/chEffAndEvtfilfib.pdf", tagname.c_str()));  

  // CENT
  TLegend* legEffcent = new TLegend(0.20, 0.20, 0.55, 0.20+7*0.045);
  xjjroot::setleg(legEffcent);
  for(int k=0; k<adceff::nhiBins; k++) { legEffcent->AddEntry(hEffAndEvtfilcent[k], Form("%.0f-%.0f%s", adceff::hiBins[k]/2., adceff::hiBins[k+1]/2., "%"), "pl"); }
  TLegend* legAnd = new TLegend(0.65, 0.82, 0.93, 0.86);
  xjjroot::setleg(legAnd);
  legAnd->SetNColumns(2);
  legAnd->AddEntry(hEffAndEvtfil, "Channel", "p");
  legAnd->AddEntry(hfibEffAndEvtfil, "Fiber", "p");

  TCanvas* chEffEvtfilcent = new TCanvas("chEffEvtfilcent", "",700, 600);
  hemptyeff->Draw();
  adceff::drawthre(hemptyeff);
  for(int k=0; k<adceff::nhiBins; k++) { hEffAndEvtfilcent[k]->Draw("ep same"); }
  xjjroot::drawCMSleft();
  xjjroot::drawCMSright(tag.c_str());
  xjjroot::drawtex(0.21, 0.84, "collisionEventSelection", 0.04, 12, 62);
  xjjroot::drawtex(0.88, 0.84, "Channel", 0.04, 32, 62);
  legEffcent->Draw();
  // legAnd->Draw();
  chEffEvtfilcent->SaveAs(Form("plots/%s/chEffEvtfilcent.pdf", tagname.c_str()));  

  TCanvas* chEffEvtfilcentfib = new TCanvas("chEffEvtfilcentfib", "",700, 600);
  hemptyefffib->Draw();
  for(int k=0; k<adceff::nhiBins; k++) { hfibEffAndEvtfilcent[k]->Draw("ep same"); }
  xjjroot::drawCMSleft();
  xjjroot::drawCMSright(tag.c_str());
  xjjroot::drawtex(0.21, 0.84, "collisionEventSelection", 0.04, 12, 62);
  xjjroot::drawtex(0.88, 0.84, "Fiber", 0.04, 32, 62);
  legEffcent->Draw();
  // legAnd->Draw();
  chEffEvtfilcentfib->SaveAs(Form("plots/%s/chEffEvtfilcentfib.pdf", tagname.c_str()));  
}

int main(int argc, char* argv[])
{
  if(argc==2) { adceff_drawhist(argv[1]); return 0; }
  return 1;
}
