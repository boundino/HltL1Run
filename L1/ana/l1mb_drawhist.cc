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

float frate = 20;
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
std::string tag_, subtag_;
float ZBrate;
int printrate(std::vector<TH1F*> vh);

template<class T>
void drawshadow(T* hempty, Color_t cc = kGray, float guide = 15)
{
  hempty->Draw("AXIS");
  xjjroot::drawCMS("Preliminary", tag_.c_str());
  xjjroot::drawtex(0, 0.01, subtag_.c_str(), 0.035, 11, 52, kGray+1);
  if(cc > 0)
    {
      float prescls[] = {1, 1.5};
      for(auto& pp : prescls)
        {
          xjjroot::drawline(pp*guide, hempty->GetYaxis()->GetXmin(), pp*guide, hempty->GetYaxis()->GetXmax()<2?1:hempty->GetYaxis()->GetXmax(), cc, 5, 2);
          xjjroot::drawtexnum(pp*guide-0.5, hempty->GetYaxis()->GetXmin() + (hempty->GetYaxis()->GetXmax()-hempty->GetYaxis()->GetXmin())/20., 
                              Form("Prescl = %s", xjjc::number_remove_zero(pp).c_str()), 0.032, 32, 62, cc);
        }
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

void drawhfth(TGraph* gr, TH2* hempty)
{
  for(int i=0; i<gr->GetN(); i++)
    {
      double x = gr->GetPointX(i), y = gr->GetPointY(i);
      if (x >= hempty->GetXaxis()->GetXmin() && x <= hempty->GetXaxis()->GetXmax())
        xjjroot::drawtexnum(x, y+(hempty->GetYaxis()->GetXmax()-hempty->GetYaxis()->GetXmin())/20., Form("%d", i), 0.03, 23, 42, gr->GetMarkerColor());
    }
}

int nearest(TH1F* h, float frate)
{
  int iresult = -1;
  float dev = 1.e+10;
  for(int i=0; i<h->GetXaxis()->GetNbins(); i++) {
    float delta = h->GetBinContent(i+1) - frate;
    // if (delta > 0) continue;
    if (fabs(delta) > dev) {
      if (delta > 0)
        iresult = iresult - 1;
      break;
    }
    dev = delta;
    iresult = i;
  }
  std::cout<<__FUNCTION__<<": \e[1m"<<frate<<"kHz\e[0m: \e[1m("<<h->GetName()<<")\e[0m => \e[1m"<<Form("%.2f kHz", h->GetBinContent(iresult+1))<<" (ADC > "<<iresult<<")\e[0m"<<std::endl;
  return iresult;
}

int macro(std::string param)
{
  xjjc::config conf(param);
  conf.print();
  std::string outputdir = conf["Output"];
  tag_ = conf["Tag"]; subtag_ = conf["SubTag"];
  int nneu = conf.vi("nneu"), drawhlt = conf.vi("drawHLT");
  std::vector<int> aHFnow = conf.vvi("aHFnow");

  l1trigger::setcent(conf);
  int colors[] = {kGray+3, xjjroot::mycolor_satmiddle["red"], xjjroot::mycolor_satmiddle["azure"], xjjroot::mycolor_satmiddle["green"], kMagenta+1, kCyan+1};
  // int colors[] = {kBlack, kRed, kBlue, kGreen+2, kMagenta+1, kCyan+1};

  TFile* inf = TFile::Open(Form("rootfiles/%s/savehist.root", outputdir.c_str()));

  ZBrate = ((TH1F*)inf->Get("hrateZB"))->GetBinContent(1);
  std::cout<<ZBrate<<std::endl;
  TH1F* hcent = (TH1F*)inf->Get("hcent");
  xjjroot::sethempty(hcent);
  xjjroot::setthgrstyle(hcent, kBlack, 20, 0.2, kBlack, 1, 2);
  auto hcent_hlt_rate = (TH1F*)inf->Get("hcent_hlt_rate"); float hlt_rate = hcent_hlt_rate->GetBinContent(1);
  auto hcent_hlt_fake = (TH1F*)inf->Get("hcent_hlt_fake"); float hlt_fake = hcent_hlt_fake->GetBinContent(1);
  auto hcent_hlt_eff = (TH1F*)inf->Get("hcent_hlt_eff");
  xjjroot::setthgrstyle(hcent_hlt_rate, colors[nneu], 47, 1.5, colors[nneu], 1, 2);
  auto hls_rate = (TH1F*)inf->Get("hls_rate"); 
  xjjroot::sethempty(hls_rate);
  xjjroot::setthgrstyle(hls_rate, kBlack, 20, 1, kBlack, 1, 2);
  auto hls_fake = (TH1F*)inf->Get("hls_fake");
  xjjroot::sethempty(hls_fake);
  xjjroot::setthgrstyle(hls_fake, kBlack, 20, 1, kBlack, 1, 2);

  // hist
  std::vector<TH1F*> hZDCdis(l1trigger::nDirs, 0), hZDCdisGeV(l1trigger::nDirs, 0),
    hrate_And_ZDCAnd(l1trigger::nNeus, 0), hrate_And_ZDCOr(l1trigger::nNeus, 0), 
    hfake_And_ZDCAnd(l1trigger::nNeus, 0), hfake_And_ZDCOr(l1trigger::nNeus, 0), 
    hrate_And_ZDCAnd_pix(l1trigger::nNeus, 0), hrate_And_ZDCOr_pix(l1trigger::nNeus, 0),
    hrate_And_ZDCAnd_frac(l1trigger::nNeus, 0), hrate_And_ZDCOr_frac(l1trigger::nNeus, 0);
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
      hrate_And_ZDCOr[k] = (TH1F*)inf->Get(Form("hrate_And_ZDCOr_%dn", k));
      hfake_And_ZDCAnd[k] = (TH1F*)inf->Get(Form("hfake_And_ZDCAnd_%dn", k));
      hfake_And_ZDCOr[k] = (TH1F*)inf->Get(Form("hfake_And_ZDCOr_%dn", k));
      hrate_And_ZDCAnd_pix[k] = (TH1F*)inf->Get(Form("hrate_And_ZDCAnd_pix_%dn", k));
      hrate_And_ZDCOr_pix[k] = (TH1F*)inf->Get(Form("hrate_And_ZDCOr_pix_%dn", k));
      hrate_And_ZDCAnd_frac[k] = (TH1F*)inf->Get(Form("hrate_And_ZDCAnd_frac_%dn", k));
      hrate_And_ZDCOr_frac[k] = (TH1F*)inf->Get(Form("hrate_And_ZDCOr_frac_%dn", k));
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
  auto groc_And_ZDCAnd = xjjc::array2d<TGraph*>(l1trigger::nNeus, l1trigger::ncent),
    groc_And_ZDCOr = xjjc::array2d<TGraph*>(l1trigger::nNeus, l1trigger::ncent),
    gfake_And_ZDCAnd = xjjc::array2d<TGraph*>(l1trigger::nNeus, l1trigger::ncent),
    gfake_And_ZDCOr = xjjc::array2d<TGraph*>(l1trigger::nNeus, l1trigger::ncent),
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
      xjjroot::setthgrstyle(grate_And_ZDCAnd[k], colors[k], 20, 1.1, colors[k], 2, 2);
      grate_And_ZDCOr[k] = roc(hrate_And_ZDCOr[k], hrate_And_ZDCOr_pix[k], Form("grate_And_ZDCOr_%dn", k));
      xjjroot::setthgrstyle(grate_And_ZDCOr[k], colors[k], 21, 1.1, colors[k], 1, 2);
      for(int l=0; l<l1trigger::ncent; l++)
        {
          groc_And_ZDCAnd[k][l] = roc(hrate_And_ZDCAnd[k], heff_And_ZDCAnd[k][l], Form("groc_And_ZDCAnd_%dn_%d", k, l));
          xjjroot::setthgrstyle(groc_And_ZDCAnd[k][l], colors[k], 20, 1.1, colors[k], 2, 2);
          groc_And_ZDCAnd_pix[k][l] = roc(hrate_And_ZDCAnd[k], heff_And_ZDCAnd_pix[k][l], Form("groc_And_ZDCAnd_pix_%dn_%d", k, l)); //
          xjjroot::setthgrstyle(groc_And_ZDCAnd_pix[k][l], colors[k], 46, 1.1, colors[k], 6, 2);
          gfake_And_ZDCAnd[k][l] = roc(hfake_And_ZDCAnd[k], heff_And_ZDCAnd[k][l], Form("gfake_And_ZDCAnd_%dn_%d", k, l));
          xjjroot::setthgrstyle(gfake_And_ZDCAnd[k][l], colors[k], 20, 1.1, colors[k], 2, 2);
          greleff_And_ZDCAnd[k][l] = roc(hrate_And_ZDCAnd[k], hreleff_And_ZDCAnd_pix[k][l], Form("greleff_And_ZDCAnd_%dn_%d", k, l));
          xjjroot::setthgrstyle(greleff_And_ZDCAnd[k][l], colors[k], 20, 1.1, colors[k], 2, 2);

          groc_And_ZDCOr[k][l] = roc(hrate_And_ZDCOr[k], heff_And_ZDCOr[k][l], Form("groc_And_ZDCOr_%dn_%d", k, l));
          xjjroot::setthgrstyle(groc_And_ZDCOr[k][l], colors[k], 21, 1.1, colors[k], 1, 2);
          groc_And_ZDCOr_pix[k][l] = roc(hrate_And_ZDCOr[k], heff_And_ZDCOr_pix[k][l], Form("groc_And_ZDCOr_pix_%dn_%d", k, l));
          xjjroot::setthgrstyle(groc_And_ZDCOr_pix[k][l], colors[k], 47, 1.1, colors[k], 6, 2);
          gfake_And_ZDCOr[k][l] = roc(hfake_And_ZDCOr[k], heff_And_ZDCOr[k][l], Form("gfake_And_ZDCOr_%dn_%d", k, l));
          xjjroot::setthgrstyle(gfake_And_ZDCOr[k][l], colors[k], 21, 1.1, colors[k], 1, 2);
          greleff_And_ZDCOr[k][l] = roc(hrate_And_ZDCOr[k], hreleff_And_ZDCOr_pix[k][l], Form("greleff_And_ZDCOr_%dn_%d", k, l));
          xjjroot::setthgrstyle(greleff_And_ZDCOr[k][l], colors[k], 21, 1.1, colors[k], 1, 2);
        }
      for(int a=0; a<l1trigger::nadc; a++)
        {
          geffcent_And_ZDCAnd[k][a] = xjjana::shifthistcenter(heffcent_And_ZDCAnd[k][a], Form("geffcent_And_ZDCAnd_%dn_%d", k, a), 0, "X0Y0");
          xjjroot::setthgrstyle(geffcent_And_ZDCAnd[k][a], colors[k], 20, 1.1, colors[k], 2, 2);
          geffcent_And_ZDCOr[k][a] = xjjana::shifthistcenter(heffcent_And_ZDCOr[k][a], Form("geffcent_And_ZDCOr_%dn_%d", k, a), 0, "X0Y0");
          xjjroot::setthgrstyle(geffcent_And_ZDCOr[k][a], colors[k], 21, 1.1, colors[k], 1, 2);
        }
    }

  std::vector<TGraphErrors*> gr_heff_AND_HFOnly(l1trigger::ncent);
  for(int l=0; l<l1trigger::ncent; l++)
    {
      gr_heff_AND_HFOnly[l] = xjjana::shifthistcenter(heff_And_ZDCAnd[0][l], Form("gr_%s", heff_And_ZDCAnd[0][l]->GetName()), (int)-1);
      xjjroot::setthgrstyle(gr_heff_AND_HFOnly[l], colors[l], 20, 1.1, colors[l], 1, 2);
    }

  // leg
  auto t_And_ZDCAnd = xjjroot::drawtex(0.50, 0.77, "HF_AND & ZDC_#bf{AND}", 0.037, 32, 42, kBlack, 0, false),
    t_And_ZDCOr = xjjroot::drawtex(0.50, 0.83, "HF_AND & ZDC_#bf{OR}", 0.037, 32, 42, kBlack, 0, false);
  auto *leg_And_ZDCAnd = new TLegend(0.52, 0.74, 0.88, 0.80),
    *leg_And_ZDCAnd_pix = new TLegend(0.52, 0.80, 0.88, 0.86),
    *leg_And_ZDCOr = new TLegend(0.52, 0.80, 0.88, 0.86),
    *leg_And_ZDCOr_pix = new TLegend(0.52, 0.74, 0.88, 0.80),
    *leg_And_HFOnly = new TLegend(0.22, 0.55-0.0375*l1trigger::ncent, 0.42, 0.55);
  leg_And_ZDCAnd->SetNColumns(4);
  xjjroot::setleg(leg_And_ZDCAnd, 0.037);
  leg_And_ZDCAnd_pix->SetNColumns(4);
  xjjroot::setleg(leg_And_ZDCAnd_pix, 0.037);
  leg_And_ZDCOr->SetNColumns(4);
  xjjroot::setleg(leg_And_ZDCOr, 0.037);
  leg_And_ZDCOr_pix->SetNColumns(4);
  xjjroot::setleg(leg_And_ZDCOr_pix, 0.037);
  for(int k=0; k<l1trigger::nNeus; k++)
    {
      leg_And_ZDCAnd->AddEntry(groc_And_ZDCAnd[k][0], Form("(%dn)", k), "pl");
      leg_And_ZDCAnd_pix->AddEntry(groc_And_ZDCAnd_pix[k][0], Form("(%dn)", k), "pl");
      leg_And_ZDCOr->AddEntry(groc_And_ZDCOr[k][0], Form("(%dn)", k), "pl");
      leg_And_ZDCOr_pix->AddEntry(groc_And_ZDCOr_pix[k][0], Form("(%dn)", k), "pl");
    }
  xjjroot::setleg(leg_And_HFOnly, 0.037);
  for(int l=0; l<l1trigger::ncent; l++)
    leg_And_HFOnly->AddEntry(gr_heff_AND_HFOnly[l], Form("Cent. %d-%d%s", l1trigger::cent[l]/2, l1trigger::cent[l+1]/2, "%"), "p");
  auto leg_HLT = new TLegend(0.66, 0.33, 0.85, 0.38);
  xjjroot::setleg(leg_HLT, 0.037);
  leg_HLT->AddEntry(hcent_hlt_rate, "Actually HLT", "p");
  
  auto hempty = new TH2F("hempty", ";L1 Rate [kHz];Efficiency", 10, 10, 50, 10, 0, 1.35);
  xjjroot::sethempty(hempty);
  auto hemptyfake = new TH2F("hemptyfake", ";Fake rate (Inclusive centrality);Efficiency", 10, 0, 0.7, 10, 0, 1.35);
  xjjroot::sethempty(hemptyfake);
  auto hemptyfake_zoom = new TH2F("hemptyfake", ";Fake rate (Inclusive centrality);Efficiency", 10, 0, 0.6, 10, 0, 1.15);
  xjjroot::sethempty(hemptyfake_zoom);
  auto hemptylog = new TH2F("hemptylog", ";L1 Rate [kHz];Efficiency", 10, 6, 1000, 10, 0, 1.35);
  xjjroot::sethempty(hemptylog);
  auto hemptyeff = new TH2F("hemptyeff", ";L1 Rate [kHz];#frac{Efficiency (w/ Ntrk > 0)}{Efficiency (w/o Ntrk > 0)}", 10, 10, 50, 10, 0, 1.35);
  xjjroot::sethempty(hemptyeff);
  auto hemptyrate = new TH2F("hemptyrate", ";L1 Rate [kHz];HLT Rate (w/ Ntrk > 0) [kHz]", 10, 10, 50, 10, 0, 20);
  xjjroot::sethempty(hemptyrate);
  auto hemptyeffcent = new TH2F("hemptyeffcent", ";Centrality [%];Efficiency", 10, 0, 100, 10, 0, 1.35);
  xjjroot::sethempty(hemptyeffcent);
  auto hemptyeffadc = new TH2F("hemptyeffadc", ";HF Threshold [ADC];Efficiency", 10, 0, l1trigger::nadc, 10, 0, 1.35);
  xjjroot::sethempty(hemptyeffadc);

  xjjroot::setgstyle(1);
  gStyle->SetPadRightMargin(xjjroot::margin_pad_right*2);
  xjjroot::mypdf* pdf = new xjjroot::mypdf("figspdf/" + outputdir + "/per.pdf", "c", 700, 600);

  pdf->prepare();
  drawshadow(hemptyeffadc, 0);
  for(int l=0; l<l1trigger::ncent; l++)
    gr_heff_AND_HFOnly[l]->Draw("plX same");
  leg_And_HFOnly->Draw();
  xjjroot::drawtex(0.23, 0.80, "HF only", 0.035, 12, 62);
  pdf->getc()->RedrawAxis();
  pdf->write("figs/" + outputdir + "/per-eff-cent.pdf");

  // HF_And_ZDCAnd/Or ROC
  for(int l=0; l<l1trigger::ncent; l++)
    {
      pdf->prepare();
      // pdf->getc()->SetLogx();
      drawshadow(hempty, 0);

      for(int k=0; k<l1trigger::nNeus; k++)
        {
          if (drawhlt && k != nneu) continue;
          if (!drawhlt)
            groc_And_ZDCAnd[k][l]->Draw("same pl");
          groc_And_ZDCOr[k][l]->Draw("same pl");
          if (k == nneu) {
            int aa = aHFnow[k];
            xjjroot::drawline(groc_And_ZDCOr[k][l]->GetPointX(aa), hemptyfake->GetYaxis()->GetXmin(),
                              groc_And_ZDCOr[k][l]->GetPointX(aa), groc_And_ZDCOr[k][l]->GetPointY(aa), colors[k],
                              6, 2, 0.9);
          }
          drawhfth(groc_And_ZDCOr[k][l], hempty);
        }
      leg_And_ZDCAnd->Draw();
      leg_And_ZDCOr->Draw();
      t_And_ZDCAnd->Draw();
      t_And_ZDCOr->Draw();
      if (drawhlt) {
        xjjroot::drawpoint(hlt_rate, hcent_hlt_eff->GetBinContent(l+1), colors[nneu], 47, 1.5);
        leg_HLT->Draw();
      }
      xjjroot::drawtex(0.88, 0.30, Form("Centrality %d-%d%s", l1trigger::cent[l]/2, l1trigger::cent[l+1]/2, "%"), 0.038, 32);
      pdf->getc()->RedrawAxis();
      pdf->write(Form("figs/%s/per-eff-rate-%d-%d.pdf", outputdir.c_str(), l1trigger::cent[l]/2, l1trigger::cent[l+1]/2));
      // pdf->getc()->SetLogx(0);
    }

  // Rate vs LS
  pdf->getc()->SetLogy(0);
  pdf->prepare();
  hls_rate->SetMinimum(0);
  drawshadow(hls_rate, 0);
  hls_rate->Draw("p same");
  // xjjroot::drawtex(0.24, 0.85, "Applied Offline Event Selection", 0.038, 13);
  pdf->getc()->RedrawAxis();
  pdf->write();

  // HF_And_ZDCAnd/Or ROC
  for(int l=0; l<l1trigger::ncent; l++) {
    pdf->prepare();
    // pdf->getc()->SetLogx();
    drawshadow(hemptyfake, 0);
      
    gfake_And_ZDCAnd[0][l]->Draw("same pl");
    for(int k=0; k<l1trigger::nNeus; k++) {
      if (drawhlt && k != nneu) continue;
      int aa = aHFnow[k];
      if (k == nneu)
        xjjroot::drawline(gfake_And_ZDCOr[k][l]->GetPointX(aa), hemptyfake->GetYaxis()->GetXmin(),
                          gfake_And_ZDCOr[k][l]->GetPointX(aa), gfake_And_ZDCOr[k][l]->GetPointY(aa), colors[k],
                          6, 2, 0.9);
      gfake_And_ZDCOr[k][l]->Draw("same pl");
      drawhfth(gfake_And_ZDCOr[k][l], hemptyfake);
    }
    // leg_And_ZDCAnd->Draw();
    leg_And_ZDCOr->Draw();
    // t_And_ZDCAnd->Draw();
    t_And_ZDCOr->Draw();
    if (drawhlt) {
      xjjroot::drawpoint(hlt_fake, hcent_hlt_eff->GetBinContent(l+1), colors[nneu], 47, 1.5);
      leg_HLT->Draw();
    }
    xjjroot::drawtex(0.88, 0.30, Form("Centrality %d-%d%s", l1trigger::cent[l]/2, l1trigger::cent[l+1]/2, "%"), 0.038, 32);
    pdf->getc()->RedrawAxis();
    pdf->write(Form("figs/%s/per-eff-fake-%d-%d.pdf", outputdir.c_str(), l1trigger::cent[l]/2, l1trigger::cent[l+1]/2));
    // pdf->getc()->SetLogx(0);
  }

  // Fake vs LS
  pdf->getc()->SetLogy(0);
  pdf->prepare();
  hls_fake->SetMinimum(0);
  drawshadow(hls_fake, 0);
  hls_fake->Draw("p same");
  // xjjroot::drawtex(0.24, 0.85, "Applied Offline Event Selection", 0.038, 13);
  pdf->getc()->RedrawAxis();
  pdf->write();

  // HFcent_And_ZDCAnd/Or
  for (float prescale : {0.75, 1., 1.5, 2.0}) {
    pdf->prepare();
    drawshadow(hemptyeffcent, 0);
    for(int k=0; k<l1trigger::nNeus; k++)
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
    pdf->write(Form("figs/%s/per-eff-cent-prescl%.1f.pdf", outputdir.c_str(), prescale));
  }

  for (int a = 11; a<19; a++) {
    pdf->prepare();
    drawshadow(hemptyeffcent, 0);
    geffcent_And_ZDCAnd[0][a]->Draw("same pl");
    int aOr = nearest(hrate_And_ZDCOr[nneu], hrate_And_ZDCAnd[0]->GetBinContent(a+1));
    geffcent_And_ZDCOr[nneu][aOr]->Draw("same pl");
    xjjroot::drawtex(0.23, 0.30, Form("Rate [HF only] = %.1f kHz (HF thre = %d)", hrate_And_ZDCAnd[0]->GetBinContent(a+1), a), 0.038, 12);
    xjjroot::drawtex(0.23, 0.25, Form("Rate [ZDC Or %dn] = %.1f kHz (HF thre = %d)", nneu, hrate_And_ZDCOr[nneu]->GetBinContent(aOr+1), aOr), 0.038, 12, 42, colors[nneu]);
    xjjroot::drawtex(0.23, 0.35, "Similar rate w/ vs. w/o ZDCOR", 0.035, 12, 62);
    pdf->getc()->RedrawAxis();
    pdf->write(Form("figs/%s/per-eff-cent-thred%d.pdf", outputdir.c_str(), a));
  }

  // pdf->prepare();
  // drawshadow(hemptyeffcent, 0);
  // geffcent_And_ZDCAnd[0][aHFnow[0]]->Draw("same pl");
  // geffcent_And_ZDCOr[nneu][aHFnow[nneu]]->Draw("same pl");
  // xjjroot::drawtex(0.23, 0.30, Form("Rate [HF only] = %.1f kHz (HF thre = %d)", hrate_And_ZDCAnd[0]->GetBinContent(aHFnow[0]+1), aHFnow[0]), 0.038, 12);
  // xjjroot::drawtex(0.23, 0.25, Form("Rate [ZDC Or %dn] = %.1f kHz (HF thre = %d)", nneu, hrate_And_ZDCOr[nneu]->GetBinContent(aHFnow[nneu]+1), aHFnow[nneu]), 0.038, 12, 42, colors[nneu]);
  // xjjroot::drawtex(0.23, 0.35, "Current working point", 0.035, 12, 62);
  // pdf->getc()->RedrawAxis();
  // pdf->write(Form("figs/%s/per-eff-cent-current.pdf", outputdir.c_str()));

  // for (int k=0; k<l1trigger::nNeus; k++) {
  //   pdf->prepare();
  //   drawshadow(hemptyeffcent, 0);
  //   auto leg = new TLegend(0.23, 0.25, 0.50, 0.25+0.04*4);
  //   xjjroot::setleg(leg);
  //   for (int a = 12; a<16; a++) {
  //     auto geffcent_And_ZDCOr_clone = xjjana::copyobject(geffcent_And_ZDCOr[k][a], Form("geffcent_And_ZDCOr_clone%d%d", k, a));
  //     geffcent_And_ZDCOr_clone->SetMarkerStyle(47);
  //     geffcent_And_ZDCOr_clone->SetMarkerColor(xjjroot::colorlist_middle[a-12]);
  //     geffcent_And_ZDCOr_clone->SetLineColor(xjjroot::colorlist_middle[a-12]);
  //     geffcent_And_ZDCOr_clone->Draw("same pl");
  //     leg->AddEntry(geffcent_And_ZDCOr_clone, Form("HF thre = %d", a), "pl");
  //   }
  //   leg->Draw();
  //   xjjroot::drawtex(0.23, 0.80, (k>0?Form("HF + ZDCOR (%dn)", k):"HF only"), 0.038, 12);
  //   xjjroot::drawlinevertical(90, hemptyeffcent);
  //   pdf->getc()->RedrawAxis();
  //   pdf->write(Form("figs/%s/per-eff-cent-neu%d.pdf", outputdir.c_str(), k));
  // }
  
  // // HF_And_ZDCAnd_pix/Or_pix Ntrk efficiency
  // for(int l=0; l<l1trigger::ncent; l++)
  //   {
  //     pdf->prepare();
  //     drawshadow(hemptyeff, 0);
  //     for(int k=0; k<l1trigger::nNeus; k++)
  //       {
  //         greleff_And_ZDCAnd[k][l]->Draw("same pl");
  //         greleff_And_ZDCOr[k][l]->Draw("same pl");
  //       }
  //     leg_And_ZDCAnd->Draw();
  //     leg_And_ZDCOr->Draw();
  //     t_And_ZDCAnd->Draw();
  //     t_And_ZDCOr->Draw();
  //     xjjroot::drawtex(0.88, 0.30, Form("Centrality %d-%d%s", l1trigger::cent[l]/2, l1trigger::cent[l+1]/2, "%"), 0.038, 32);
  //     pdf->write();
  //   }

  // // HF_And_ZDCAnd_pix/Or_pix HLT rate
  // pdf->prepare();
  // drawshadow(hemptyrate, 0);
  // for(int k=0; k<l1trigger::nNeus; k++)
  //   {
  //     grate_And_ZDCAnd[k]->Draw("same pl");
  //     grate_And_ZDCOr[k]->Draw("same pl");
  //   }
  // leg_And_ZDCAnd->Draw();
  // leg_And_ZDCOr->Draw();
  // t_And_ZDCAnd->Draw();
  // t_And_ZDCOr->Draw();
  // pdf->write();

  // ZDC distribution
  pdf->getc()->SetLogy();
  for(int j=0; j<l1trigger::nDirs; j++)
    {
      pdf->prepare();
      drawshadow(hZDCdis[j], 0);
      hZDCdis[j]->Draw("p same");
      for(int k=1; k<l1trigger::nNeus; k++)
        {
          float biny = hZDCdis[j]->GetBinContent(hZDCdis[j]->FindBin(l1trigger::mNeuZDCLow[j][k]));
          xjjroot::drawline(l1trigger::mNeuZDCLow[j][k], hZDCdis[j]->GetMinimum(), 
                            l1trigger::mNeuZDCLow[j][k], biny,
                            colors[k], 7, 4);
          xjjroot::drawtexnum(l1trigger::mNeuZDCLow[j][k], biny*5, Form("(%dn)", k), 0.038, 22, 62, colors[k]);
        }
      pdf->write();
    }
  // for(int j=0; j<l1trigger::nDirs; j++)
  //   {
  //     pdf->prepare();
  //     drawshadow(hZDCdisGeV[j], 0);
  //     hZDCdisGeV[j]->Draw("p same");
  //     for(int k=1; k<l1trigger::nNeus; k++)
  //       {
  //         float biny = hZDCdisGeV[j]->GetBinContent(hZDCdisGeV[j]->FindBin(l1trigger::mNeuZDCLow[j][k]));
  //         xjjroot::drawline(l1trigger::mNeuZDCLow[j][k], hZDCdisGeV[j]->GetMinimum(), 
  //                           l1trigger::mNeuZDCLow[j][k], biny,
  //                           colors[k], 7, 4);
  //         xjjroot::drawtexnum(l1trigger::mNeuZDCLow[j][k], biny*5, Form("(%dn)", k), 0.038, 22, 62, colors[k]);
  //       }
  //     pdf->write();
  //   }

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


  // --> Individuals (only HF and 2n)
  auto leg_ind1 = new TLegend(0.24, 0.80, 0.87, 0.85);
  leg_ind1->SetNColumns(2);
  xjjroot::setleg(leg_ind1, 0.038);
  leg_ind1->AddEntry(groc_And_ZDCAnd[0][0], "HF Only", "pl");
  leg_ind1->AddEntry(groc_And_ZDCOr[nneu][0], Form("HF + ZDC_OR (%dn)", nneu), "pl");

  // >>
  // for (float prescale : {1.5, 2.0, 2.5}) {
  //   int a0 = nearest(hrate_And_ZDCAnd[0], frate*prescale),
  //     a2 = nearest(hrate_And_ZDCOr[2], frate*prescale);
  //   pdf->prepare();
  //   drawshadow(hemptyeffcent, 0);
  //   drawgrval(geffcent_And_ZDCAnd[0][a0], hemptyeffcent, false);
  //   drawgrval(geffcent_And_ZDCOr[2][a2], hemptyeffcent, false);
  //   xjjroot::drawtex(0.88, 0.30, Form("L1 MB rate #approx %.0f kHz (Presl %.1f)", frate*prescale, prescale), 0.038, 32);
  //   leg_ind1->Draw();
  //   pdf->write(Form("figs/%s/per-eff-cent-prescl%.1f-clean.pdf", outputdir.c_str(), prescale));
  // }

  // >>
  for(int l=0; l<l1trigger::ncent; l++)
    {
      pdf->prepare();
      drawshadow(hempty, 0);
      // drawgrval(groc_And_ZDCAnd[0][l], hempty);
      // drawgrval(groc_And_ZDCOr[nneu][l], hempty);
      groc_And_ZDCAnd[0][l]->Draw("pl same");
      groc_And_ZDCOr[nneu][l]->Draw("pl same");
      drawhfth(groc_And_ZDCOr[0][l], hempty);
      drawhfth(groc_And_ZDCOr[nneu][l], hempty);
      leg_ind1->Draw();
      xjjroot::drawtex(0.88, 0.30, Form("Centrality %d-%d%s", l1trigger::cent[l]/2, l1trigger::cent[l+1]/2, "%"), 0.038, 32);
      pdf->write(Form("figs/%s/per-eff-rate-%d-%d-clean.pdf", outputdir.c_str(), l1trigger::cent[l]/2, l1trigger::cent[l+1]/2));
    }

  // >>
  for(auto& h : groc_And_ZDCOr[nneu]) h->SetLineStyle(1);
  auto leg_ind = new TLegend(0.24, 0.76, 0.87, 0.86);
  leg_ind->SetNColumns(2);
  xjjroot::setleg(leg_ind, 0.038);
  leg_ind->AddEntry(groc_And_ZDCAnd[0][0], "HF Only", "pl");
  leg_ind->AddEntry(groc_And_ZDCOr[nneu][0], Form("HF + ZDC_OR (%dn)", nneu), "pl");
  leg_ind->AddEntry(groc_And_ZDCAnd_pix[0][0], "HF Only + Ntrk", "pl");
  leg_ind->AddEntry(groc_And_ZDCOr_pix[nneu][0], Form("HF + ZDC_OR (%dn) + Ntrk", nneu), "pl");
  for(int l=0; l<l1trigger::ncent; l++)
    {
      pdf->prepare(); 
      drawshadow(hempty, 0);
      drawgrval(groc_And_ZDCAnd[0][l], hempty);
      drawgrval(groc_And_ZDCAnd_pix[0][l], hempty);
      drawgrval(groc_And_ZDCOr[nneu][l], hempty);
      drawgrval(groc_And_ZDCOr_pix[nneu][l], hempty);
      leg_ind->Draw();
      xjjroot::drawtex(0.88, 0.30, Form("Centrality %d-%d%s", l1trigger::cent[l]/2, l1trigger::cent[l+1]/2, "%"), 0.038, 32);
      pdf->write();
    }

  // >>
  // auto grate_And_ZDCOr_2clone = xjjana::copyobject(grate_And_ZDCOr[nneu], "grate_And_ZDCOr_2clone");
  // grate_And_ZDCOr_2clone->SetLineStyle(1);
  // pdf->prepare();
  // drawshadow(hemptyrate, 0);
  // drawgrval(grate_And_ZDCAnd[0], hemptyrate);
  // drawgrval(grate_And_ZDCOr_2clone, hemptyrate);
  // leg_ind->Draw();
  // pdf->write();

  // Empty frame
  // pdf->prepare();
  // drawshadow(hempty, kRed);
  // pdf->getc()->RedrawAxis();
  // pdf->write();

  // HF_And_ZDCAnd/Or ROC log
  // for(int l=0; l<l1trigger::ncent; l++)
  //   {
  //     pdf->prepare();
  //     pdf->getc()->SetLogx();
  //     drawshadow(hemptylog, 0);

  //     for(int k=0; k<l1trigger::nNeus; k++)
  //       {
  //         groc_And_ZDCAnd[k][l]->Draw("same pl");
  //         groc_And_ZDCOr[k][l]->Draw("same pl");
  //       }
  //     drawhfth(groc_And_ZDCAnd[0][l], hemptylog);
  //     // drawhfth(groc_And_ZDCOr[1][l], hemptylog);
  //     drawhfth(groc_And_ZDCOr[nneu][l], hemptylog);
  //     // drawhfth(groc_And_ZDCOr[3][l], hemptylog);
  //     leg_And_ZDCAnd->Draw();
  //     leg_And_ZDCOr->Draw();
  //     t_And_ZDCAnd->Draw();
  //     t_And_ZDCOr->Draw();
  //     xjjroot::drawtex(0.88, 0.30, Form("Centrality %d-%d%s", l1trigger::cent[l]/2, l1trigger::cent[l+1]/2, "%"), 0.038, 32);
  //     pdf->getc()->RedrawAxis();
  //     pdf->write(Form("figs/%s/per-eff-rate-%d-%d.pdf", outputdir.c_str(), l1trigger::cent[l]/2, l1trigger::cent[l+1]/2));
  //     // pdf->getc()->SetLogx(0);
  //   }

  // publish
  auto *leg_And_HF = new TLegend(0.25, 0.80, 0.38, 0.86);
  xjjroot::setleg(leg_And_HF, 0.037);
  leg_And_HF->AddEntry(groc_And_ZDCAnd[0][0], "HF Only", "p");
  auto *leg_And_ZDC = new TLegend(0.66, 0.80, 0.86, 0.86);
  leg_And_ZDC->SetNColumns(2);
  xjjroot::setleg(leg_And_ZDC, 0.037);
  leg_And_ZDC->AddEntry(groc_And_ZDCOr[1][0], "(1n)", "p");
  leg_And_ZDC->AddEntry(groc_And_ZDCOr[2][0], "(2n)", "p");
  auto t_And_ZDC = xjjroot::drawtex(0.64, 0.83, "HF & ZDC_#bf{OR}", 0.037, 32, 42, kBlack, 0, false);
  
  for(int l=0; l<l1trigger::ncent; l++) {
    pdf->prepare();
    drawshadow(hemptyfake_zoom, 0);

    int aa = 15;
    for (int aa : {15, 16}) {
      xjjroot::drawbox(gfake_And_ZDCOr[2][l]->GetPointX(aa), gfake_And_ZDCOr[2][l]->GetPointY(aa),
                       gfake_And_ZDCOr[0][l]->GetPointX(aa), gfake_And_ZDCOr[0][l]->GetPointY(aa));
    }
    
    gfake_And_ZDCAnd[0][l]->Draw("same pl");
    gfake_And_ZDCOr[1][l]->Draw("same pl");
    gfake_And_ZDCOr[2][l]->Draw("same pl");

    leg_And_HF->Draw();
    leg_And_ZDC->Draw();
    t_And_ZDC->Draw();
    xjjroot::drawtex(0.88, 0.30, Form("Centrality %d-%d%s", l1trigger::cent[l]/2, l1trigger::cent[l+1]/2, "%"), 0.038, 32);
    pdf->getc()->RedrawAxis();
    pdf->write(Form("figs/%s/per-eff-fake-%d-%d-official.pdf", outputdir.c_str(), l1trigger::cent[l]/2, l1trigger::cent[l+1]/2));
  }
  
  pdf->close();
  std::vector<TH1F*> vh = {hrate_And_ZDCAnd[0], hrate_And_ZDCOr[1], hrate_And_ZDCOr[2]};
  printrate(vh);

  return 0;
}

int main(int argc, char* argv[])
{
  if(argc==2) return macro(argv[1]);
  return 1;
}

int printrate(std::vector<TH1F*> vh)
{
  xjjc::prt_divider();
  std::cout<<"HF thre, ";
  for (auto& h : vh) {
    std::cout<<std::setw(9)<<Form("%s,", h->GetName());
  }
  std::cout<<std::endl;
  for(int i=0; i<vh[0]->GetXaxis()->GetNbins(); i++) {
    std::cout<<std::left<<std::setw(9)<<Form("%d,", i);
    for (auto& h : vh) {
      std::cout<<std::setw(9)<<Form("%f,", h->GetBinContent(i+1));
    }
    std::cout<<std::endl;
  }
  return 0;
}
