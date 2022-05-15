#include "constant.h"
#include <TFile.h>
#include <TH1F.h>
#include <TH2F.h>
#include <TLegend.h>
#include <string>
#include "xjjcuti.h"
#include "xjjrootuti.h"
#include "xjjanauti.h"
#include "xjjmypdf.h"

TGraph* roc(TH1F* hx, TH1F* hy, std::string name="")
{
  int n = hx->GetXaxis()->GetNbins();
  std::vector<float> x(n), y(n);
  for(int i=0; i<n; i++)
    {
      x[i] = hx->GetBinContent(i+1);
      y[i] = hy->GetBinContent(i+1);
    }
  TGraph* gr = new TGraph(n, x.data(), y.data());
  gr->SetName(name.c_str());
  return gr;
}
std::string tag_;
float ZBrate;

void drawshadow(TH2* hempty, Color_t cc = kGray+1)
{
  hempty->Draw("AXIS");
  xjjroot::drawCMS("Internal", tag_.c_str());
  if(cc > 0)
    {
      for(int p=2; p<=4; p++)
        {
          xjjroot::drawbox(8*p, hempty->GetYaxis()->GetXmin(), 10*p, hempty->GetYaxis()->GetXmax(), kGray, 0.2);
          xjjroot::drawtexnum(9*p, hempty->GetYaxis()->GetXmin() + (hempty->GetYaxis()->GetXmax()-hempty->GetYaxis()->GetXmin())/20., Form("Prescl = %d", p), 0.032, 22, 62, cc);
        }
      // xjjroot::drawtex(0.88, 0.24, Form("ZB rate: %.1f #times10^{6} Hz", ZBrate/1.e+6), 0.04, 32);
    }
}

void drawgrval(TGraph* gr, TH2* hempty, bool val = true)
{
  gr->Draw("plY0 same");
  if(val)
    for(int i=0; i<gr->GetN(); i++)
      {
        double x = gr->GetPointX(i), y = gr->GetPointY(i);
        if((x>16 && x<32))
          xjjroot::drawtexnum(x, y+(hempty->GetYaxis()->GetXmax()-hempty->GetYaxis()->GetXmin())/20., Form("%.2f", y), 0.03, 23, 42, gr->GetMarkerColor());
      }
}

int nearest(TH1F* h, float frate)
{
  int iresult = -1;
  float dev = 1.e+10;
  for(int i=0; i<h->GetXaxis()->GetNbins(); i++)
    {
      float delta = fabs(h->GetBinContent(i+1) - frate);
      if(delta > dev) break;
      dev = delta;
      iresult = i;
    }
  std::cout<<__FUNCTION__<<": \e[1m"<<frate<<"kHz -> ("<<h->GetName()<<") ==> "<<h->GetBinContent(iresult+1)<<"\e[0m"<<std::endl;
  return iresult;
}

int macro(std::string outputdir, std::string tag="")
{
  TFile* inf = TFile::Open(Form("rootfiles/%s/savehist.root", outputdir.c_str()));
  tag_ = tag;

  ZBrate = ((TH1F*)inf->Get("hrateZB"))->GetBinContent(1);
  std::cout<<ZBrate<<std::endl;
  TH1F* hcent = (TH1F*)inf->Get("hcent");
  xjjroot::sethempty(hcent);
  xjjroot::setthgrstyle(hcent, kBlack, 20, 0.2, kBlack, 1, 2);

  // hist
  std::vector<TH1F*> hZDCdis(l1trigger::nDirs, 0), hZDCdisGeV(l1trigger::nDirs, 0),
    hrate_And_ZDCAnd(l1trigger::nNeus, 0), hrate_And_ZDCAnd_pix(l1trigger::nNeus, 0),
    hrate_And_ZDCOr(l1trigger::nNeus, 0), hrate_And_ZDCOr_pix(l1trigger::nNeus, 0);
  auto heff_And_ZDCAnd = xjjc::array2d<TH1F*>(l1trigger::nNeus, l1trigger::ncent),
    heff_And_ZDCOr = xjjc::array2d<TH1F*>(l1trigger::nNeus, l1trigger::ncent),
    heff_And_ZDCAnd_pix = xjjc::array2d<TH1F*>(l1trigger::nNeus, l1trigger::ncent),
    heff_And_ZDCOr_pix = xjjc::array2d<TH1F*>(l1trigger::nNeus, l1trigger::ncent),
    heffcent_And_ZDCAnd = xjjc::array2d<TH1F*>(l1trigger::nNeus, l1trigger::nadc),
    heffcent_And_ZDCOr = xjjc::array2d<TH1F*>(l1trigger::nNeus, l1trigger::nadc),
    hreleff_And_ZDCAnd_pix = xjjc::array2d<TH1F*>(l1trigger::nNeus, l1trigger::ncent),
    hreleff_And_ZDCOr_pix = xjjc::array2d<TH1F*>(l1trigger::nNeus, l1trigger::ncent);

  for(int j=0; j<l1trigger::nDirs; j++)
    {
      hZDCdis[j] = (TH1F*)inf->Get(Form("hZDCdis%s", l1trigger::mDir[j].c_str()));
      hZDCdis[j]->GetXaxis()->SetMaxDigits(3);
      xjjroot::sethempty(hZDCdis[j]);
      xjjroot::setthgrstyle(hZDCdis[j], kBlack, 20, 0.9);
      hZDCdisGeV[j] = (TH1F*)inf->Get(Form("hZDCdisGeV%s", l1trigger::mDir[j].c_str()));
      hZDCdisGeV[j]->GetXaxis()->SetMaxDigits(3);
      xjjroot::sethempty(hZDCdisGeV[j]);
      xjjroot::setthgrstyle(hZDCdisGeV[j], kBlack, 20, 0.9);
    }
  for(int k=0; k<l1trigger::nNeus; k++)
    {
      hrate_And_ZDCAnd[k] = (TH1F*)inf->Get(Form("hrate_And_ZDCAnd_%dn", k));
      hrate_And_ZDCAnd_pix[k] = (TH1F*)inf->Get(Form("hrate_And_ZDCAnd_pix_%dn", k));
      hrate_And_ZDCOr[k] = (TH1F*)inf->Get(Form("hrate_And_ZDCOr_%dn", k));
      hrate_And_ZDCOr_pix[k] = (TH1F*)inf->Get(Form("hrate_And_ZDCOr_pix_%dn", k));
      for(int l=0; l<l1trigger::ncent; l++)
        {
          heff_And_ZDCAnd[k][l] = (TH1F*)inf->Get(Form("heff_And_ZDCAnd_%dn_%d", k, l));
          heff_And_ZDCAnd_pix[k][l] = (TH1F*)inf->Get(Form("heff_And_ZDCAnd_pix_%dn_%d", k, l));
          heff_And_ZDCOr[k][l] = (TH1F*)inf->Get(Form("heff_And_ZDCOr_%dn_%d", k, l));
          heff_And_ZDCOr_pix[k][l] = (TH1F*)inf->Get(Form("heff_And_ZDCOr_pix_%dn_%d", k, l));

          hreleff_And_ZDCAnd_pix[k][l] = (TH1F*)heff_And_ZDCAnd_pix[k][l]->Clone(Form("hreleff_And_ZDCAnd_pix_%dn_%d", k, l));
          hreleff_And_ZDCAnd_pix[k][l]->Divide(heff_And_ZDCAnd[k][l]);
          hreleff_And_ZDCOr_pix[k][l] = (TH1F*)heff_And_ZDCOr_pix[k][l]->Clone(Form("hreleff_And_ZDCOr_pix_%dn_%d", k, l));
          hreleff_And_ZDCOr_pix[k][l]->Divide(heff_And_ZDCOr[k][l]);
        }
      for(int a=0; a<l1trigger::nadc; a++)
        {
          heffcent_And_ZDCAnd[k][a] = (TH1F*)inf->Get(Form("heffcent_And_ZDCAnd_%dn_%d", k, a));
          heffcent_And_ZDCOr[k][a] = (TH1F*)inf->Get(Form("heffcent_And_ZDCOr_%dn_%d", k, a));
        }
    }

  // gr
  int color[] = {kBlack, kRed, kBlue, kGreen+2};
  auto groc_And_ZDCAnd = xjjc::array2d<TGraph*>(l1trigger::nNeus, l1trigger::ncent),
    groc_And_ZDCOr = xjjc::array2d<TGraph*>(l1trigger::nNeus, l1trigger::ncent),
    groc_And_ZDCAnd_pix = xjjc::array2d<TGraph*>(l1trigger::nNeus, l1trigger::ncent),
    groc_And_ZDCOr_pix = xjjc::array2d<TGraph*>(l1trigger::nNeus, l1trigger::ncent),
    greleff_And_ZDCAnd = xjjc::array2d<TGraph*>(l1trigger::nNeus, l1trigger::ncent),
    greleff_And_ZDCOr = xjjc::array2d<TGraph*>(l1trigger::nNeus, l1trigger::ncent);
  auto geffcent_And_ZDCAnd = xjjc::array2d<TGraphAsymmErrors*>(l1trigger::nNeus, l1trigger::nadc),
    geffcent_And_ZDCOr = xjjc::array2d<TGraphAsymmErrors*>(l1trigger::nNeus, l1trigger::nadc);

  std::vector<TGraph*> grate_And_ZDCAnd(l1trigger::nNeus), 
    grate_And_ZDCOr(l1trigger::nNeus);
  for(int k=0; k<l1trigger::nNeus; k++)
    {
      grate_And_ZDCAnd[k] = roc(hrate_And_ZDCAnd[k], hrate_And_ZDCAnd_pix[k], Form("grate_And_ZDCAnd_%dn", k));
      xjjroot::setthgrstyle(grate_And_ZDCAnd[k], color[k], 20, 1.1, color[k], 1, 3);
      grate_And_ZDCOr[k] = roc(hrate_And_ZDCOr[k], hrate_And_ZDCOr_pix[k], Form("grate_And_ZDCOr_%dn", k));
      xjjroot::setthgrstyle(grate_And_ZDCOr[k], color[k], 25, 1.1, color[k], 2, 3);
      for(int l=0; l<l1trigger::ncent; l++)
        {
          groc_And_ZDCAnd[k][l] = roc(hrate_And_ZDCAnd[k], heff_And_ZDCAnd[k][l], Form("groc_And_ZDCAnd_%dn_%d", k, l));
          xjjroot::setthgrstyle(groc_And_ZDCAnd[k][l], color[k], 20, 1.1, color[k], 1, 3);
          groc_And_ZDCAnd_pix[k][l] = roc(hrate_And_ZDCAnd[k], heff_And_ZDCAnd_pix[k][l], Form("groc_And_ZDCAnd_pix_%dn_%d", k, l)); //
          xjjroot::setthgrstyle(groc_And_ZDCAnd_pix[k][l], color[k], 47, 1.1, color[k], 6, 3);
          greleff_And_ZDCAnd[k][l] = roc(hrate_And_ZDCAnd[k], hreleff_And_ZDCAnd_pix[k][l], Form("greleff_And_ZDCAnd_%dn_%d", k, l));
          xjjroot::setthgrstyle(greleff_And_ZDCAnd[k][l], color[k], 20, 1.1, color[k], 1, 3);
          groc_And_ZDCOr[k][l] = roc(hrate_And_ZDCOr[k], heff_And_ZDCOr[k][l], Form("groc_And_ZDCOr_%dn_%d", k, l));
          xjjroot::setthgrstyle(groc_And_ZDCOr[k][l], color[k], 25, 1.1, color[k], 2, 3);
          groc_And_ZDCOr_pix[k][l] = roc(hrate_And_ZDCOr[k], heff_And_ZDCOr_pix[k][l], Form("groc_And_ZDCOr_pix_%dn_%d", k, l));
          xjjroot::setthgrstyle(groc_And_ZDCOr_pix[k][l], color[k], 46, 1.1, color[k], 6, 3);
          greleff_And_ZDCOr[k][l] = roc(hrate_And_ZDCOr[k], hreleff_And_ZDCOr_pix[k][l], Form("greleff_And_ZDCOr_%dn_%d", k, l));
          xjjroot::setthgrstyle(greleff_And_ZDCOr[k][l], color[k], 25, 1.1, color[k], 2, 3);
        }
      for(int a=0; a<l1trigger::nadc; a++)
        {
          geffcent_And_ZDCAnd[k][a] = xjjana::shifthistcenter(heffcent_And_ZDCAnd[k][a], Form("geffcent_And_ZDCAnd_%dn_%d", k, a), 0, "X0Y0");
          xjjroot::setthgrstyle(geffcent_And_ZDCAnd[k][a], color[k], 20, 1.1, color[k], 1, 3);
          geffcent_And_ZDCOr[k][a] = xjjana::shifthistcenter(heffcent_And_ZDCOr[k][a], Form("geffcent_And_ZDCOr_%dn_%d", k, a), 0, "X0Y0");
          xjjroot::setthgrstyle(geffcent_And_ZDCOr[k][a], color[k], 25, 1.1, color[k], 2, 3);
        }
    }

  // leg
  auto t_And_ZDCAnd = xjjroot::drawtex(0.52, 0.83, "HF_AND & ZDC_#bf{AND}", 0.04, 32, 42, kBlack, 0, false),
    t_And_ZDCOr = xjjroot::drawtex(0.52, 0.77, "HF_AND & ZDC_#bf{OR}", 0.04, 32, 42, kBlack, 0, false);
  auto *leg_And_ZDCAnd = new TLegend(0.54, 0.80, 0.93, 0.86),
    *leg_And_ZDCAnd_pix = new TLegend(0.54, 0.74, 0.93, 0.80),
    *leg_And_ZDCOr = new TLegend(0.54, 0.74, 0.93, 0.80),
    *leg_And_ZDCOr_pix = new TLegend(0.54, 0.74, 0.93, 0.80);
  leg_And_ZDCAnd->SetNColumns(4);
  xjjroot::setleg(leg_And_ZDCAnd, 0.04);
  leg_And_ZDCAnd_pix->SetNColumns(4);
  xjjroot::setleg(leg_And_ZDCAnd_pix, 0.04);
  leg_And_ZDCOr->SetNColumns(4);
  xjjroot::setleg(leg_And_ZDCOr, 0.04);
  leg_And_ZDCOr_pix->SetNColumns(4);
  xjjroot::setleg(leg_And_ZDCOr_pix, 0.04);
  for(int k=0; k<l1trigger::nNeus; k++)
    {
      leg_And_ZDCAnd->AddEntry(groc_And_ZDCAnd[k][0], Form("(%dn)", k), "pl");
      leg_And_ZDCAnd_pix->AddEntry(groc_And_ZDCAnd_pix[k][0], Form("(%dn)", k), "pl");
      leg_And_ZDCOr->AddEntry(groc_And_ZDCOr[k][0], Form("(%dn)", k), "pl");
      leg_And_ZDCOr_pix->AddEntry(groc_And_ZDCOr_pix[k][0], Form("(%dn)", k), "pl");
    }
  auto hempty = new TH2F("hempty", ";L1 Rate [kHz];Efficiency", 10, 10, 40, 10, 0, 1.35);
  xjjroot::sethempty(hempty);
  auto hemptyeff = new TH2F("hemptyeff", ";L1 Rate [kHz];#frac{Efficiency (w/ Ntrk > 0)}{Efficiency (w/o Ntrk > 0)}", 10, 10, 40, 10, 0, 1.35);
  xjjroot::sethempty(hemptyeff);
  auto hemptyrate = new TH2F("hemptyrate", ";L1 Rate [kHz];HLT Rate (w/ Ntrk > 0) [kHz]", 10, 10, 40, 10, 10, 20);
  xjjroot::sethempty(hemptyrate);
  auto hemptyeffcent = new TH2F("hemptyeffcent", ";Centrality;Efficiency", 10, 0, 100, 10, 0, 1.35);
  xjjroot::sethempty(hemptyeffcent);

  xjjroot::setgstyle(1);
  gStyle->SetPadRightMargin(xjjroot::margin_pad_right*2);
  xjjroot::mypdf* pdf = new xjjroot::mypdf("plots/" + outputdir + "/per.pdf", "c", 700, 600);

  // HF_And_ZDCAnd/Or ROC
  for(int l=0; l<l1trigger::ncent; l++)
    {
      pdf->prepare();
      drawshadow(hempty);

      for(int k=0; k<l1trigger::nNeus; k++)
        {
          groc_And_ZDCAnd[k][l]->Draw("same pl");
          groc_And_ZDCOr[k][l]->Draw("same pl");
        }
      leg_And_ZDCAnd->Draw();
      leg_And_ZDCOr->Draw();
      t_And_ZDCAnd->Draw();
      t_And_ZDCOr->Draw();
      xjjroot::drawtex(0.88, 0.30, Form("Centrality %d-%d%s", l1trigger::cent[l]/2, l1trigger::cent[l+1]/2, "%"), 0.04, 32);
      pdf->getc()->RedrawAxis();
      pdf->write();
    }

  // HFcent_And_ZDCAnd/Or
  pdf->prepare();
  float frate = 20;
  drawshadow(hemptyeffcent, 0);
  for(int k=0; k<l1trigger::nNeus; k++)
    {
      int aAnd = nearest(hrate_And_ZDCAnd[k], frate),
        aOr = nearest(hrate_And_ZDCOr[k], frate);
      geffcent_And_ZDCAnd[k][aAnd]->Draw("same pl");
      geffcent_And_ZDCOr[k][aOr]->Draw("same pl");
    }
  leg_And_ZDCAnd->Draw();
  leg_And_ZDCOr->Draw();
  t_And_ZDCAnd->Draw();
  t_And_ZDCOr->Draw();
  xjjroot::drawtex(0.88, 0.24, Form("L1 MB rate #approx %.0f kHz", frate), 0.04, 32);
  pdf->getc()->RedrawAxis();
  pdf->write();

  pdf->prepare();
  frate = 30;
  drawshadow(hemptyeffcent, 0);
  for(int k=0; k<l1trigger::nNeus; k++)
    {
      int aAnd = nearest(hrate_And_ZDCAnd[k], frate),
        aOr = nearest(hrate_And_ZDCOr[k], frate);
      geffcent_And_ZDCAnd[k][aAnd]->Draw("same pl");
      geffcent_And_ZDCOr[k][aOr]->Draw("same pl");
    }
  leg_And_ZDCAnd->Draw();
  leg_And_ZDCOr->Draw();
  t_And_ZDCAnd->Draw();
  t_And_ZDCOr->Draw();
  xjjroot::drawtex(0.88, 0.24, Form("L1 MB rate #approx %.0f kHz", frate), 0.04, 32);
  pdf->getc()->RedrawAxis();
  pdf->write();


  // HF_And_ZDCAnd_pix/Or_pix Ntrk efficiency
  for(int l=0; l<l1trigger::ncent; l++)
    {
      pdf->prepare();
      drawshadow(hemptyeff);
      for(int k=0; k<l1trigger::nNeus; k++)
        {
          greleff_And_ZDCAnd[k][l]->Draw("same pl");
          greleff_And_ZDCOr[k][l]->Draw("same pl");
        }
      leg_And_ZDCAnd->Draw();
      leg_And_ZDCOr->Draw();
      t_And_ZDCAnd->Draw();
      t_And_ZDCOr->Draw();
      xjjroot::drawtex(0.88, 0.30, Form("Centrality %d-%d%s", l1trigger::cent[l]/2, l1trigger::cent[l+1]/2, "%"), 0.04, 32);
      pdf->write();
    }

  // HF_And_ZDCAnd_pix/Or_pix HLT rate
  pdf->prepare();
  drawshadow(hemptyrate);
  for(int k=0; k<l1trigger::nNeus; k++)
    {
      grate_And_ZDCAnd[k]->Draw("same pl");
      grate_And_ZDCOr[k]->Draw("same pl");
    }
  leg_And_ZDCAnd->Draw();
  leg_And_ZDCOr->Draw();
  t_And_ZDCAnd->Draw();
  t_And_ZDCOr->Draw();
  pdf->write();

  // ZDC distribution
  pdf->getc()->SetLogy();
  for(int j=0; j<l1trigger::nDirs; j++)
    {
      pdf->prepare();
      hZDCdis[j]->Draw("p");
      xjjroot::drawCMS("Internal", tag_.c_str());
      for(int k=1; k<l1trigger::nNeus; k++)
        {
          float biny = hZDCdis[j]->GetBinContent(hZDCdis[j]->FindBin(l1trigger::mNeuZDCLow[j][k]));
          xjjroot::drawline(l1trigger::mNeuZDCLow[j][k], hZDCdis[j]->GetMinimum(), 
                            l1trigger::mNeuZDCLow[j][k], biny,
                            color[k], 7, 4);
          xjjroot::drawtexnum(l1trigger::mNeuZDCLow[j][k], biny*5, Form("(%dn)", k), 0.04, 22, 62, color[k]);
        }
      pdf->write();
    }
  for(int j=0; j<l1trigger::nDirs; j++)
    {
      pdf->prepare();
      hZDCdisGeV[j]->Draw("p");
      xjjroot::drawCMS("Internal", tag_.c_str());
      for(int k=1; k<l1trigger::nNeus; k++)
        {
          float biny = hZDCdisGeV[j]->GetBinContent(hZDCdisGeV[j]->FindBin(l1trigger::mNeuZDCLow[j][k]));
          xjjroot::drawline(l1trigger::mNeuZDCLow[j][k], hZDCdisGeV[j]->GetMinimum(), 
                            l1trigger::mNeuZDCLow[j][k], biny,
                            color[k], 7, 4);
          xjjroot::drawtexnum(l1trigger::mNeuZDCLow[j][k], biny*5, Form("(%dn)", k), 0.04, 22, 62, color[k]);
        }
      pdf->write();
    }

  // Centrality distribution
  pdf->getc()->SetLogy(0);
  pdf->prepare();
  hcent->Draw("AXIS");
  xjjroot::drawCMS("Internal", tag_.c_str());
  xjjroot::drawbox(85, hcent->GetMinimum(), 100, hcent->GetMaximum());
  hcent->Draw("hist e same");
  xjjroot::drawtex(0.24, 0.85, "Applied Offline Event Selection", 0.04, 13);
  pdf->getc()->RedrawAxis();
  pdf->write();


  // --> Individuals
  auto leg_ind1 = new TLegend(0.24, 0.80, 0.87, 0.85);
  leg_ind1->SetNColumns(2);
  xjjroot::setleg(leg_ind1, 0.04);
  leg_ind1->AddEntry(groc_And_ZDCAnd[0][0], "HF Only", "pl");
  leg_ind1->AddEntry(groc_And_ZDCOr[2][0], "HF + ZDC_OR (2n)", "pl");

  // >>
  // float frate = 20;
  int a0 = nearest(hrate_And_ZDCAnd[0], frate),
    a2 = nearest(hrate_And_ZDCOr[2], frate);
  pdf->prepare();
  drawshadow(hemptyeffcent, 0);
  drawgrval(geffcent_And_ZDCAnd[0][a0], hemptyeffcent, false);
  drawgrval(geffcent_And_ZDCOr[2][a2], hemptyeffcent, false);
  xjjroot::drawtex(0.88, 0.24, Form("L1 MB rate #approx %.0f kHz", frate), 0.04, 32);
  leg_ind1->Draw();
  pdf->write();

  // >>
  for(int l=0; l<l1trigger::ncent; l++)
    {
      pdf->prepare();
      drawshadow(hempty);
      drawgrval(groc_And_ZDCAnd[0][l], hempty);
      drawgrval(groc_And_ZDCOr[2][l], hempty);
      leg_ind1->Draw();
      xjjroot::drawtex(0.88, 0.30, Form("Centrality %d-%d%s", l1trigger::cent[l]/2, l1trigger::cent[l+1]/2, "%"), 0.04, 32);
      pdf->write();
    }

  // >>
  for(auto& h : groc_And_ZDCOr[2]) h->SetLineStyle(1);
  auto leg_ind = new TLegend(0.24, 0.76, 0.87, 0.86);
  leg_ind->SetNColumns(2);
  xjjroot::setleg(leg_ind, 0.04);
  leg_ind->AddEntry(groc_And_ZDCAnd[0][0], "HF Only", "pl");
  leg_ind->AddEntry(groc_And_ZDCOr[2][0], "HF + ZDC_OR(2n)", "pl");
  leg_ind->AddEntry(groc_And_ZDCAnd_pix[0][0], "HF Only + Ntrk", "pl");
  leg_ind->AddEntry(groc_And_ZDCOr_pix[2][0], "HF + ZDC_OR(2n) + Ntrk", "pl");
  for(int l=0; l<l1trigger::ncent; l++)
    {
      pdf->prepare();
      drawshadow(hempty);
      drawgrval(groc_And_ZDCAnd[0][l], hempty);
      drawgrval(groc_And_ZDCAnd_pix[0][l], hempty);
      drawgrval(groc_And_ZDCOr[2][l], hempty);
      drawgrval(groc_And_ZDCOr_pix[2][l], hempty);
      leg_ind->Draw();
      xjjroot::drawtex(0.88, 0.30, Form("Centrality %d-%d%s", l1trigger::cent[l]/2, l1trigger::cent[l+1]/2, "%"), 0.04, 32);
      pdf->write();
    }

  // >>
  auto grate_And_ZDCOr_2clone = xjjana::copyobject(grate_And_ZDCOr[2], "grate_And_ZDCOr_2clone");
  grate_And_ZDCOr_2clone->SetLineStyle(1);
  pdf->prepare();
  drawshadow(hemptyrate);
  drawgrval(grate_And_ZDCAnd[0], hemptyrate);
  drawgrval(grate_And_ZDCOr_2clone, hemptyrate);
  leg_ind->Draw();
  pdf->write();

  pdf->prepare();
  drawshadow(hemptyrate);
  drawgrval(grate_And_ZDCAnd[0], hemptyrate);
  drawgrval(grate_And_ZDCOr_2clone, hemptyrate);
  leg_ind->Draw();
  pdf->write();

  // Empty frame
  pdf->prepare();
  drawshadow(hempty, kRed);
  pdf->getc()->RedrawAxis();
  pdf->write();


  pdf->close();

  return 0;
}

int main(int argc, char* argv[])
{
  if(argc==3) return macro(argv[1], argv[2]);
  return 1;
}

