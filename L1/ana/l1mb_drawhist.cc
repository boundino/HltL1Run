#include "constant.h"
#include "config.h"
#include <TFile.h>
#include <TH1F.h>
#include <TH2F.h>
#include <TLegend.h>
#include <string>
#include "xjjcuti.h"
#include "xjjrootuti.h"
#include "xjjanauti.h"
#include "xjjmypdf.h"

#include "l1mb_helper.h"

float frate = 20;

int macro(std::string param)
{
  xjjc::config conf(param);
  conf.print();
  std::string outputdir = conf["Output"], MBhlt = conf["MBhlt"];
  tag_ = conf["Tag"]; subtag_ = conf["SubTag"];
  l1trigger::sethlt(MBhlt);
  l1trigger::setcent();
  auto nBunch = conf.vf("nBunch");
  l1trigger::setraterange(nBunch);
  
  int colors[] = {kBlack, kRed, kBlue, kGreen+2, kMagenta+1, kCyan+1};

  auto inf = TFile::Open(Form("rootfiles/%s/savehist.root", outputdir.c_str()));

  float ZBrate = hist_to_value(inf, "hrateZB"),
    hlt_rate = hist_to_value(inf, "hcent_hlt_rate"),
    hlt_fake = hist_to_value(inf, "hcent_hlt_fake");
  std::cout<<ZBrate<<std::endl;

  TH1F* hcent = (TH1F*)inf->Get("hcent");
  xjjroot::sethempty(hcent);
  xjjroot::setthgrstyle(hcent, kBlack, 20, 0.2, kBlack, 1, 2);
  auto hcent_hlt_eff = (TH1F*)inf->Get("hcent_hlt_eff");

  // hist
  std::vector<TH1F*> hZDCdisGeV(l1trigger::nDirs, 0);
  for (int j=0; j<l1trigger::nDirs; j++) {
    hZDCdisGeV[j] = (TH1F*)inf->Get(Form("hZDCdisGeV%s", l1trigger::mDir[j].c_str()));
    hZDCdisGeV[j]->GetXaxis()->SetMaxDigits(3);
    xjjroot::sethempty(hZDCdisGeV[j]);
    xjjroot::setthgrstyle(hZDCdisGeV[j], kBlack, 20, 0.9, kBlack, 1, 1);
  }

  std::vector<TH1F*> hrate_And_ZDCAnd(l1trigger::nNeus, 0), hfake_And_ZDCAnd(l1trigger::nNeus, 0),
    hrate_And_ZDCOr(l1trigger::nNeus, 0), hfake_And_ZDCOr(l1trigger::nNeus, 0);
  xjjc::array2D<TH1F*> heff_And_ZDCAnd = xjjc::array2d<TH1F*>(l1trigger::nNeus, l1trigger::ncent),
    heff_And_ZDCOr = xjjc::array2d<TH1F*>(l1trigger::nNeus, l1trigger::ncent),
    heffcent_And_ZDCAnd = xjjc::array2d<TH1F*>(l1trigger::nNeus, l1trigger::nadc),
    heffcent_And_ZDCOr = xjjc::array2d<TH1F*>(l1trigger::nNeus, l1trigger::nadc);
  std::vector<TH1F*> heff_And_ZDCAnd_int(l1trigger::nNeus, 0), heff_And_ZDCOr_int(l1trigger::nNeus, 0);
  for (int k=0; k<l1trigger::nNeus; k++) {
    hrate_And_ZDCAnd[k] = (TH1F*)inf->Get(Form("hrate_And_ZDCAnd_%dn", k));
    hrate_And_ZDCOr[k] = (TH1F*)inf->Get(Form("hrate_And_ZDCOr_%dn", k));
    hfake_And_ZDCAnd[k] = (TH1F*)inf->Get(Form("hfake_And_ZDCAnd_%dn", k));
    hfake_And_ZDCOr[k] = (TH1F*)inf->Get(Form("hfake_And_ZDCOr_%dn", k));
    for (int l=0; l<l1trigger::ncent; l++) {
      heff_And_ZDCAnd[k][l] = (TH1F*)inf->Get(Form("heff_And_ZDCAnd_%dn_%d", k, l));
      heff_And_ZDCOr[k][l] = (TH1F*)inf->Get(Form("heff_And_ZDCOr_%dn_%d", k, l));
    }
    for (int a=0; a<l1trigger::nadc; a++) {
      heffcent_And_ZDCAnd[k][a] = (TH1F*)inf->Get(Form("heffcent_And_ZDCAnd_%dn_%d", k, a));
      heffcent_And_ZDCOr[k][a] = (TH1F*)inf->Get(Form("heffcent_And_ZDCOr_%dn_%d", k, a));
    }
    heff_And_ZDCAnd_int[k] = (TH1F*)inf->Get(Form("heff_And_ZDCAnd_int_%dn", k));
    heff_And_ZDCOr_int[k] = (TH1F*)inf->Get(Form("heff_And_ZDCOr_int_%dn", k));
  }
  
  // gr
  auto groc_And_ZDCAnd = xjjc::array2d<TGraph*>(l1trigger::nNeus, l1trigger::ncent),
    groc_And_ZDCOr = xjjc::array2d<TGraph*>(l1trigger::nNeus, l1trigger::ncent),
    gfake_And_ZDCAnd = xjjc::array2d<TGraph*>(l1trigger::nNeus, l1trigger::ncent),
    gfake_And_ZDCOr = xjjc::array2d<TGraph*>(l1trigger::nNeus, l1trigger::ncent);
  auto geffcent_And_ZDCAnd = xjjc::array2d<TGraphAsymmErrors*>(l1trigger::nNeus, l1trigger::nadc),
    geffcent_And_ZDCOr = xjjc::array2d<TGraphAsymmErrors*>(l1trigger::nNeus, l1trigger::nadc);

  for (int k=0; k<l1trigger::nNeus; k++) {
    for (int l=0; l<l1trigger::ncent; l++) {
      groc_And_ZDCAnd[k][l] = roc(hrate_And_ZDCAnd[k], heff_And_ZDCAnd[k][l], Form("groc_And_ZDCAnd_%dn_%d", k, l));
      xjjroot::setthgrstyle(groc_And_ZDCAnd[k][l], colors[k], 20, 1.1, colors[k], 1, 2);
      gfake_And_ZDCAnd[k][l] = roc(hfake_And_ZDCAnd[k], heff_And_ZDCAnd[k][l], Form("gfake_And_ZDCAnd_%dn_%d", k, l));
      xjjroot::setthgrstyle(gfake_And_ZDCAnd[k][l], colors[k], 20, 1.1, colors[k], 1, 2);

      groc_And_ZDCOr[k][l] = roc(hrate_And_ZDCOr[k], heff_And_ZDCOr[k][l], Form("groc_And_ZDCOr_%dn_%d", k, l));
      xjjroot::setthgrstyle(groc_And_ZDCOr[k][l], colors[k], 25, 1.1, colors[k], 2, 2);
      gfake_And_ZDCOr[k][l] = roc(hfake_And_ZDCOr[k], heff_And_ZDCOr[k][l], Form("gfake_And_ZDCOr_%dn_%d", k, l));
      xjjroot::setthgrstyle(gfake_And_ZDCOr[k][l], colors[k], 25, 1.1, colors[k], 2, 2);
    }
    for (int a=0; a<l1trigger::nadc; a++) {
      geffcent_And_ZDCAnd[k][a] = xjjana::shifthistcenter(heffcent_And_ZDCAnd[k][a], Form("geffcent_And_ZDCAnd_%dn_%d", k, a), 0, "X0Y0");
      xjjroot::setthgrstyle(geffcent_And_ZDCAnd[k][a], colors[k], 20, 1.1, colors[k], 1, 2);
      geffcent_And_ZDCOr[k][a] = xjjana::shifthistcenter(heffcent_And_ZDCOr[k][a], Form("geffcent_And_ZDCOr_%dn_%d", k, a), 0, "X0Y0");
      xjjroot::setthgrstyle(geffcent_And_ZDCOr[k][a], colors[k], 25, 1.1, colors[k], 2, 2);
    }
  }
  
  std::vector<TGraphErrors*> gr_heff_And_HFOnly(l1trigger::ncent);
  for (int l=0; l<l1trigger::ncent; l++) {
    gr_heff_And_HFOnly[l] = xjjana::shifthistcenter(heff_And_ZDCAnd[0][l], Form("gr_%s", heff_And_ZDCAnd[0][l]->GetName()), (int)-1);
    xjjroot::setthgrstyle(gr_heff_And_HFOnly[l], colors[l], 20, 1.1, colors[l], 1, 2);
  }
  std::vector<TGraphErrors*> gr_heff_And_ZDCOr_int(l1trigger::nNeus, 0);
  for (int k=0; k<l1trigger::nNeus; k++) {
    gr_heff_And_ZDCOr_int[k] = xjjana::shifthistcenter(heff_And_ZDCOr_int[k], Form("gr_%s", heff_And_ZDCOr_int[k]->GetName()), (int)-1);
    xjjroot::setthgrstyle(gr_heff_And_ZDCOr_int[k], kBlack, 20, 1.1, kBlack, 1, 2);    
  }

  // leg
  auto t_And_ZDCAnd = xjjroot::drawtex(0.50, 0.77, "HF_AND & ZDC_#bf{AND}", 0.037, 32, 42, kBlack, 0, false),
    t_And_ZDCOr = xjjroot::drawtex(0.50, 0.83, "HF_AND & ZDC_#bf{OR}", 0.037, 32, 42, kBlack, 0, false);
  auto *leg_And_ZDCAnd = new TLegend(0.52, 0.74, 0.88, 0.80),
    *leg_And_ZDCOr = new TLegend(0.52, 0.80, 0.88, 0.86),
    *leg_And_HFOnly = new TLegend(0.22, 0.55-0.0375*l1trigger::ncent, 0.42, 0.55);
  leg_And_ZDCAnd->SetNColumns(4);
  xjjroot::setleg(leg_And_ZDCAnd, 0.037);
  leg_And_ZDCOr->SetNColumns(4);
  xjjroot::setleg(leg_And_ZDCOr, 0.037);
  for (int k=0; k<l1trigger::nNeus; k++) {
    leg_And_ZDCAnd->AddEntry(groc_And_ZDCAnd[k][0], Form("(%dn)", k), "pl");
    leg_And_ZDCOr->AddEntry(groc_And_ZDCOr[k][0], Form("(%dn)", k), "pl");
  }
  xjjroot::setleg(leg_And_HFOnly, 0.037);
  for (int l=0; l<l1trigger::ncent; l++)
    leg_And_HFOnly->AddEntry(gr_heff_And_HFOnly[l], Form("Cent. %d-%d%s", l1trigger::cent[l]/2, l1trigger::cent[l+1]/2, "%"), "p");
  auto leg_HLT = new TLegend(0.66, 0.33, 0.85, 0.38);
  xjjroot::setleg(leg_HLT, 0.037);
  xjjroot::addentrybystyle(leg_HLT, "Online HLT", "p", colors[l1trigger::nneu], 47, 2.);
  
  auto hempty = new TH2F("hempty", ";L1 Rate [kHz];Efficiency", 10, l1trigger::rate_min, l1trigger::rate_max, 10, 0, 1.35);
  xjjroot::sethempty(hempty);
  auto hemptyfake = new TH2F("hemptyfake", ";Fake rate (Inclusive centrality);Efficiency", 10, 0, 0.4, 10, 0, 1.35);
  xjjroot::sethempty(hemptyfake);
  auto hemptyeffcent = new TH2F("hemptyeffcent", ";Centrality [%];Efficiency", 10, 0, 100, 10, 0, 1.35);
  xjjroot::sethempty(hemptyeffcent);
  auto hemptyeffadc = new TH2F("hemptyeffadc", ";HF Threshold [ADC];Efficiency", 10, 0, l1trigger::nadc, 10, 0, 1.35);
  xjjroot::sethempty(hemptyeffadc);

  xjjroot::setgstyle(1);
  gStyle->SetPadRightMargin(xjjroot::margin_pad_right*2);
  xjjroot::mypdf* pdf = new xjjroot::mypdf("figspdf/" + outputdir + "/per.pdf", "c", 700, 600);

  // Efficiency vs ADC 
  pdf->prepare();
  drawshadow(hemptyeffadc, 0);
  for (int l=0; l<l1trigger::ncent; l++)
    gr_heff_And_HFOnly[l]->Draw("plXe same");
  leg_And_HFOnly->Draw();
  xjjroot::drawtex(0.23, 0.80, "HF only", 0.035, 12, 62);
  pdf->getc()->RedrawAxis();
  pdf->write();

  pdf->prepare();
  drawshadow(hemptyeffadc, 0);
  gr_heff_And_ZDCOr_int[0]->Draw("plXe same");
  xjjroot::drawtex(0.23, 0.80, "HF only", 0.035, 12, 62);
  pdf->getc()->RedrawAxis();
  pdf->write();

  // ZDC distribution
  pdf->getc()->SetLogy();
  for (int j=0; j<l1trigger::nDirs; j++) {
    pdf->prepare();
    drawshadow(hZDCdisGeV[j], 0);
    hZDCdisGeV[j]->Draw("pe same");
    for (int k=1; k<l1trigger::nNeus; k++) {
      float biny = hZDCdisGeV[j]->GetBinContent(hZDCdisGeV[j]->FindBin(l1trigger::mNeuZDCLow[j][k]));
      xjjroot::drawline(l1trigger::mNeuZDCLow[j][k], hZDCdisGeV[j]->GetMinimum(), 
                        l1trigger::mNeuZDCLow[j][k], biny,
                        colors[k], 7, 4);
      xjjroot::drawtexnum(l1trigger::mNeuZDCLow[j][k], biny*5, Form("(%dn)", k), 0.038, 22, 62, colors[k]);
    }
    pdf->write();
  }
  pdf->getc()->SetLogy(0);

  // Centrality distribution
  pdf->getc()->SetLogy(0);
  pdf->prepare();
  hcent->SetMinimum(0);
  drawshadow(hcent, 0);
  // xjjroot::drawbox(85, hcent->GetMinimum(), 100, hcent->GetMaximum());
  hcent->Draw("hist e same");
  xjjroot::drawtex(0.24, 0.85, "Applied Offline Event Selection", 0.038, 13);
  pdf->getc()->RedrawAxis();
  pdf->write();

  // HF_And_ZDCAnd/Or ROC - rate
  for (int l=0; l<l1trigger::ncent; l++)
    {
      pdf->prepare();
      drawshadow(hempty, 0);

      groc_And_ZDCAnd[0][l]->Draw("same pl");
      for (int k=l1trigger::nNeus-1; k>=0; k--) {
        // groc_And_ZDCAnd[k][l]->Draw("same pl");
        groc_And_ZDCOr[k][l]->Draw("same pl");
        drawhfth(groc_And_ZDCOr[k][l], hempty);
      }
      // leg_And_ZDCAnd->Draw();
      // t_And_ZDCAnd->Draw();
      leg_And_ZDCOr->Draw();
      t_And_ZDCOr->Draw();
      if (l1trigger::drawhlt) {
        xjjroot::drawpoint(hlt_rate, hcent_hlt_eff->GetBinContent(l+1), colors[l1trigger::nneu], 47, 2.0);
        leg_HLT->Draw();
      }
      xjjroot::drawtex(0.88, 0.30, Form("Centrality %d-%d%s", l1trigger::cent[l]/2, l1trigger::cent[l+1]/2, "%"), 0.038, 32);
      pdf->getc()->RedrawAxis();
      pdf->write();
    }

  // HF_And_ZDCAnd/Or ROC - fake
  for (int l=0; l<l1trigger::ncent; l++) {
    pdf->prepare();
    drawshadow(hemptyfake, 0);
      
    gfake_And_ZDCAnd[0][l]->Draw("same pl");
      for (int k=l1trigger::nNeus-1; k>=0; k--) {
      // gfake_And_ZDCAnd[k][l]->Draw("same pl");
      gfake_And_ZDCOr[k][l]->Draw("same pl");
      drawhfth(gfake_And_ZDCOr[k][l], hemptyfake);
    }
    // leg_And_ZDCAnd->Draw();
    // t_And_ZDCAnd->Draw();
    leg_And_ZDCOr->Draw();
    t_And_ZDCOr->Draw();
    if (l1trigger::drawhlt) {
      xjjroot::drawpoint(hlt_fake, hcent_hlt_eff->GetBinContent(l+1), colors[l1trigger::nneu], 47, 2.);
      leg_HLT->Draw();
    }
    xjjroot::drawtex(0.88, 0.30, Form("Centrality %d-%d%s", l1trigger::cent[l]/2, l1trigger::cent[l+1]/2, "%"), 0.038, 32);
    pdf->getc()->RedrawAxis();
    pdf->write();
  }

  int thisnneu = 1;

  // HFcent_And_ZDCAnd/Or - scan HF adc
  // for (int a = 14; a<18; a++) {
  //   pdf->prepare();
  //   drawshadow(hemptyeffcent, 0);
  //   geffcent_And_ZDCAnd[0][a]->Draw("same pl");    
  //   int aOr = nearest(hrate_And_ZDCOr[thisnneu], hrate_And_ZDCAnd[0]->GetBinContent(a+1));
  //   geffcent_And_ZDCOr[thisnneu][aOr]->Draw("same pl");
  //   xjjroot::drawtex(0.23, 0.30, Form("Rate [HF only] = %.2f kHz (HF thre = %d)", hrate_And_ZDCAnd[0]->GetBinContent(a+1), a), 0.038, 12);
  //   xjjroot::drawtex(0.23, 0.25, Form("Rate [ZDC Or %dn] = %.2f kHz (HF thre = %d)", thisnneu, hrate_And_ZDCOr[thisnneu]->GetBinContent(aOr+1), aOr), 0.038, 12, 42, colors[thisnneu]);
  //   xjjroot::drawtex(0.23, 0.35, "Similar rate w/ vs. w/o ZDCOR", 0.035, 12, 62);
  //   pdf->getc()->RedrawAxis();
  //   pdf->write();
  // }

  // HFcent_And_ZDCAnd/Or - compare same HF adc
  for (int a = 14; a<18; a++) {
    pdf->prepare();
    drawshadow(hemptyeffcent, 0);
    geffcent_And_ZDCAnd[0][a]->Draw("same pl");    
    geffcent_And_ZDCOr[thisnneu][a]->Draw("same pl");
    xjjroot::drawtex(0.23, 0.30, Form("Rate [HF only] = %.2f kHz", hrate_And_ZDCAnd[0]->GetBinContent(a+1)), 0.038, 12);
    xjjroot::drawtex(0.23, 0.25, Form("Rate [ZDC Or %dn] = %.2f kHz", thisnneu, hrate_And_ZDCOr[thisnneu]->GetBinContent(a+1)), 0.038, 12, 42, colors[thisnneu]);
    xjjroot::drawtex(0.23, 0.35, Form("Same HF threshold = %d", a), 0.035, 12, 62);
    pdf->getc()->RedrawAxis();
    pdf->write();
  }

  // HFcent_And_ZDCAnd/Or - fixed rate
  for (float prescale : {1., 1.5, 2.0}) {
    pdf->prepare();
    drawshadow(hemptyeffcent, 0);
    for (int k=0; k<l1trigger::nNeus; k++)
      {
        int aAnd = nearest(hrate_And_ZDCAnd[k], frate*prescale),
          aOr = nearest(hrate_And_ZDCOr[k], frate*prescale);
        geffcent_And_ZDCAnd[k][aAnd]->Draw("same pl");
        geffcent_And_ZDCOr[k][aOr]->Draw("same pl");
      }
    leg_And_ZDCAnd->Draw();
    leg_And_ZDCOr->Draw();
    t_And_ZDCAnd->Draw();
    t_And_ZDCOr->Draw();
    // xjjroot::drawtex(0.88, 0.30, Form("L1 MB rate #approx %.0f kHz (Presl %.1f)", frate*prescale, prescale), 0.038, 32);
    xjjroot::drawtex(0.23, 0.30, Form("L1 MB rate #approx %.0f kHz", frate*prescale), 0.038, 12);
    xjjroot::drawtex(0.23, 0.35, "Similar rate w/ vs. w/o ZDCOR", 0.035, 12, 62);
    pdf->getc()->RedrawAxis();
    pdf->write();
  }

  // --> Individuals (only HF and 2n)
  // auto leg_ind1 = new TLegend(0.24, 0.80, 0.87, 0.85);
  // leg_ind1->SetNColumns(2);
  // xjjroot::setleg(leg_ind1, 0.038);
  // leg_ind1->AddEntry(groc_And_ZDCAnd[0][0], "HF Only", "pl");
  // leg_ind1->AddEntry(groc_And_ZDCOr[l1trigger::nneu][0], Form("HF + ZDC_OR (%dn)", l1trigger::nneu), "pl");

  // // >>
  // for (int l=0; l<l1trigger::ncent; l++)
  //   {
  //     pdf->prepare();
  //     drawshadow(hempty, 0);
  //     // drawgrval(groc_And_ZDCAnd[0][l], hempty);
  //     // drawgrval(groc_And_ZDCOr[l1trigger::nneu][l], hempty);
  //     groc_And_ZDCAnd[0][l]->Draw("pl same");
  //     groc_And_ZDCOr[l1trigger::nneu][l]->Draw("pl same");
  //     drawhfth(groc_And_ZDCOr[0][l], hempty);
  //     drawhfth(groc_And_ZDCOr[l1trigger::nneu][l], hempty);
  //     leg_ind1->Draw();
  //     xjjroot::drawtex(0.88, 0.30, Form("Centrality %d-%d%s", l1trigger::cent[l]/2, l1trigger::cent[l+1]/2, "%"), 0.038, 32);
  //     pdf->write();
  //   }

  // // >>
  // for (auto& h : groc_And_ZDCOr[l1trigger::nneu]) h->SetLineStyle(1);
  // auto leg_ind = new TLegend(0.24, 0.76, 0.87, 0.86);
  // leg_ind->SetNColumns(2);
  // xjjroot::setleg(leg_ind, 0.038);
  // leg_ind->AddEntry(groc_And_ZDCAnd[0][0], "HF Only", "pl");
  // leg_ind->AddEntry(groc_And_ZDCOr[l1trigger::nneu][0], Form("HF + ZDC_OR (%dn)", l1trigger::nneu), "pl");
  // for (int l=0; l<l1trigger::ncent; l++)
  //   {
  //     pdf->prepare(); 
  //     drawshadow(hempty, 0);
  //     drawgrval(groc_And_ZDCAnd[0][l], hempty);
  //     drawgrval(groc_And_ZDCOr[l1trigger::nneu][l], hempty);
  //     leg_ind->Draw();
  //     xjjroot::drawtex(0.88, 0.30, Form("Centrality %d-%d%s", l1trigger::cent[l]/2, l1trigger::cent[l+1]/2, "%"), 0.038, 32);
  //     pdf->write();
  //   }


  // publish
  // auto hemptyfake_zoom = new TH2F("hemptyfake_zoom", ";Fake rate (Inclusive centrality);Efficiency", 10, 0, 0.6, 10, 0, 1.15);
  // xjjroot::sethempty(hemptyfake_zoom);

  // auto *leg_And_HF = new TLegend(0.25, 0.80, 0.38, 0.86);
  // xjjroot::setleg(leg_And_HF, 0.037);
  // leg_And_HF->AddEntry(groc_And_ZDCAnd[0][0], "HF Only", "p");
  // auto *leg_And_ZDC = new TLegend(0.66, 0.80, 0.86, 0.86);
  // leg_And_ZDC->SetNColumns(2);
  // xjjroot::setleg(leg_And_ZDC, 0.037);
  // leg_And_ZDC->AddEntry(groc_And_ZDCOr[1][0], "(1n)", "p");
  // leg_And_ZDC->AddEntry(groc_And_ZDCOr[2][0], "(2n)", "p");
  // auto t_And_ZDC = xjjroot::drawtex(0.64, 0.83, "HF & ZDC_#bf{OR}", 0.037, 32, 42, kBlack, 0, false);
  
  // for (int l=0; l<l1trigger::ncent; l++) {
  //   pdf->prepare();
  //   drawshadow(hemptyfake_zoom, 0);

  //   int aa = 15;
  //   for (int aa : {15, 16}) {
  //     xjjroot::drawbox(gfake_And_ZDCOr[2][l]->GetPointX(aa), gfake_And_ZDCOr[2][l]->GetPointY(aa),
  //                      gfake_And_ZDCOr[0][l]->GetPointX(aa), gfake_And_ZDCOr[0][l]->GetPointY(aa));
  //   }
    
  //   gfake_And_ZDCAnd[0][l]->Draw("same pl");
  //   gfake_And_ZDCOr[1][l]->Draw("same pl");
  //   gfake_And_ZDCOr[2][l]->Draw("same pl");

  //   leg_And_HF->Draw();
  //   leg_And_ZDC->Draw();
  //   t_And_ZDC->Draw();
  //   xjjroot::drawtex(0.88, 0.30, Form("Centrality %d-%d%s", l1trigger::cent[l]/2, l1trigger::cent[l+1]/2, "%"), 0.038, 32);
  //   pdf->getc()->RedrawAxis();
  //   pdf->write();
  // }
  
  pdf->close();
  // std::vector<TH1F*> vh = {hrate_And_ZDCAnd[0], hrate_And_ZDCOr[1], hrate_And_ZDCOr[2]};
  // printrate(vh);

  return 0;
}

int main(int argc, char* argv[])
{
  if (argc==2) return macro(argv[1]);
  return 1;
}
