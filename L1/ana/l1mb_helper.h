#ifndef __L1_HELPER_
#define __L1_HELPER_

#include "TH2F.h"

#include "xjjrootuti.h"
#include "xjjanauti.h"
#include "l1mb_constant.h"

std::string tag_, subtag_;

namespace l1style {
  int colors[] = {kBlack, kRed, kBlue, kGreen+2, kMagenta+1, kCyan+1};
  Size_t mmsize = 1.1;
  Width_t llwidth = 2;
  float y_up = 0.86, y_down = 0.80;
  class legt {
  public:
    legt(std::string text,
         Style_t mstyle=-1, Style_t lstyle=-1,
         float yy = 0.86) {
      lleg = new TLegend(0.52, yy-0.06, 0.88, yy);
      lleg->SetNColumns(4);
      xjjroot::setleg(lleg, 0.037);
      for (int k=0; k<l1trigger::nNeus; k++) {
        xjjroot::addentrybystyle(lleg, Form("(%dn)", k), "pl", colors[k], mstyle, mmsize, colors[k], lstyle, llwidth);
      }
      tt = xjjroot::drawtex(0.50, yy-0.03, text.c_str(), 0.037, 32, 42, kBlack, 0, false);
    }
    void draw() {
      lleg->Draw();
      tt->Draw();
    }
  private:
    TLegend* lleg;
    TLatex* tt;
  };
  
} // namespace l1style {

// common h
std::vector<TH1F*> hZDCdisGeV(l1trigger::nDirs, 0);
std::vector<TH1F*> hrate_And_ZDCAnd(l1trigger::nNeus, 0), hfake_And_ZDCAnd(l1trigger::nNeus, 0),
  hrate_And_ZDCOr(l1trigger::nNeus, 0), hfake_And_ZDCOr(l1trigger::nNeus, 0),
  hrate_Or_ZDCAnd(l1trigger::nNeus, 0), hfake_Or_ZDCAnd(l1trigger::nNeus, 0);
xjjc::array2D<TH1F*> heff_And_ZDCAnd = xjjc::array2d<TH1F*>(l1trigger::nNeus, l1trigger::ncent),
  heff_And_ZDCOr = xjjc::array2d<TH1F*>(l1trigger::nNeus, l1trigger::ncent),
  heff_Or_ZDCAnd = xjjc::array2d<TH1F*>(l1trigger::nNeus, l1trigger::ncent),
  heffcent_And_ZDCAnd = xjjc::array2d<TH1F*>(l1trigger::nNeus, l1trigger::nadc),
  heffcent_And_ZDCOr = xjjc::array2d<TH1F*>(l1trigger::nNeus, l1trigger::nadc),
  heffcent_Or_ZDCAnd = xjjc::array2d<TH1F*>(l1trigger::nNeus, l1trigger::nadc);
std::vector<TH1F*> heff_And_ZDCAnd_int(l1trigger::nNeus, 0),
  heff_And_ZDCOr_int(l1trigger::nNeus, 0),
  heff_Or_ZDCAnd_int(l1trigger::nNeus, 0);
std::vector<TH1F*> heffden(l1trigger::ncent, 0);
TH1F *heffden_int;

void create_hist() {
  for (int j=0; j<l1trigger::nDirs; j++) {
    hZDCdisGeV[j] = new TH1F(Form("hZDCdisGeV%s", l1trigger::mDir[j].c_str()), Form(";ZDC %s (GeV);Entries", l1trigger::mDir[j].c_str()), 500, 0, 1.e+4);
  }
  for (int k=0; k<l1trigger::nNeus; k++) {
    hrate_And_ZDCAnd[k] = new TH1F(Form("hrate_And_ZDCAnd_%dn", k), ";L1 HF threshold (ADC);", l1trigger::nadc, 0, l1trigger::nadc);
    hfake_And_ZDCAnd[k] = new TH1F(Form("hfake_And_ZDCAnd_%dn", k), ";L1 HF threshold (ADC);", l1trigger::nadc, 0, l1trigger::nadc);
    hrate_And_ZDCOr[k] = new TH1F(Form("hrate_And_ZDCOr_%dn", k), ";L1 HF threshold (ADC);", l1trigger::nadc, 0, l1trigger::nadc);
    hfake_And_ZDCOr[k] = new TH1F(Form("hfake_And_ZDCOr_%dn", k), ";L1 HF threshold (ADC);", l1trigger::nadc, 0, l1trigger::nadc);
    hrate_Or_ZDCAnd[k] = new TH1F(Form("hrate_Or_ZDCAnd_%dn", k), ";L1 HF threshold (ADC);", l1trigger::nadc, 0, l1trigger::nadc);
    hfake_Or_ZDCAnd[k] = new TH1F(Form("hfake_Or_ZDCAnd_%dn", k), ";L1 HF threshold (ADC);", l1trigger::nadc, 0, l1trigger::nadc);
    for (int l=0; l<l1trigger::ncent; l++) {
      heff_And_ZDCAnd[k][l] = new TH1F(Form("heff_And_ZDCAnd_%dn_%d", k, l), ";L1 HF threshold (ADC);", l1trigger::nadc, 0, l1trigger::nadc);
      heff_And_ZDCOr[k][l] = new TH1F(Form("heff_And_ZDCOr_%dn_%d", k, l), ";L1 HF threshold (ADC);", l1trigger::nadc, 0, l1trigger::nadc);
      heff_Or_ZDCAnd[k][l] = new TH1F(Form("heff_Or_ZDCAnd_%dn_%d", k, l), ";L1 HF threshold (ADC);", l1trigger::nadc, 0, l1trigger::nadc);
    }
    for (int a=0; a<l1trigger::nadc; a++) {
      heffcent_And_ZDCAnd[k][a] = new TH1F(Form("heffcent_And_ZDCAnd_%dn_%d", k, a), ";Centrality [%];", l1trigger::nbincent, 0, 100);
      heffcent_And_ZDCOr[k][a] = new TH1F(Form("heffcent_And_ZDCOr_%dn_%d", k, a), ";Centrality [%];", l1trigger::nbincent, 0, 100);
      heffcent_Or_ZDCAnd[k][a] = new TH1F(Form("heffcent_Or_ZDCAnd_%dn_%d", k, a), ";Centrality [%];", l1trigger::nbincent, 0, 100);
    }
    heff_And_ZDCAnd_int[k] = new TH1F(Form("heff_And_ZDCAnd_int_%dn", k), ";L1 HF threshold (ADC);", l1trigger::nadc, 0, l1trigger::nadc);
    heff_And_ZDCOr_int[k] = new TH1F(Form("heff_And_ZDCOr_int_%dn", k), ";L1 HF threshold (ADC);", l1trigger::nadc, 0, l1trigger::nadc);
    heff_Or_ZDCAnd_int[k] = new TH1F(Form("heff_Or_ZDCAnd_int_%dn", k), ";L1 HF threshold (ADC);", l1trigger::nadc, 0, l1trigger::nadc);
  }
  for (int l=0; l<l1trigger::ncent; l++) {
    heffden[l] = new TH1F(Form("heffden_%d", l), ";L1 HF threshold (ADC);", l1trigger::nadc, 0, l1trigger::nadc);
  }
  heffden_int = new TH1F("heffden_int", ";L1 HF threshold (ADC);", l1trigger::nadc, 0, l1trigger::nadc);
}

void read_hist(TFile* inf) {
  for (int j=0; j<l1trigger::nDirs; j++) {
    hZDCdisGeV[j] = (TH1F*)inf->Get(Form("hZDCdisGeV%s", l1trigger::mDir[j].c_str()));
    hZDCdisGeV[j]->GetXaxis()->SetMaxDigits(3);
    xjjroot::sethempty(hZDCdisGeV[j]);
    xjjroot::setthgrstyle(hZDCdisGeV[j], kBlack, 20, 0.9, kBlack, 1, 1);
  }
  for (int k=0; k<l1trigger::nNeus; k++) {
    hrate_And_ZDCAnd[k] = (TH1F*)inf->Get(Form("hrate_And_ZDCAnd_%dn", k));
    hfake_And_ZDCAnd[k] = (TH1F*)inf->Get(Form("hfake_And_ZDCAnd_%dn", k));
    hrate_And_ZDCOr[k] = (TH1F*)inf->Get(Form("hrate_And_ZDCOr_%dn", k));
    hfake_And_ZDCOr[k] = (TH1F*)inf->Get(Form("hfake_And_ZDCOr_%dn", k));
    // hrate_Or_ZDCAnd[k] = (TH1F*)inf->Get(Form("hrate_Or_ZDCAnd_%dn", k));
    // hfake_Or_ZDCAnd[k] = (TH1F*)inf->Get(Form("hfake_Or_ZDCAnd_%dn", k));
    for (int l=0; l<l1trigger::ncent; l++) {
      heff_And_ZDCAnd[k][l] = (TH1F*)inf->Get(Form("heff_And_ZDCAnd_%dn_%d", k, l));
      heff_And_ZDCOr[k][l] = (TH1F*)inf->Get(Form("heff_And_ZDCOr_%dn_%d", k, l));
      // heff_Or_ZDCAnd[k][l] = (TH1F*)inf->Get(Form("heff_Or_ZDCAnd_%dn_%d", k, l));
    }
    for (int a=0; a<l1trigger::nadc; a++) {
      heffcent_And_ZDCAnd[k][a] = (TH1F*)inf->Get(Form("heffcent_And_ZDCAnd_%dn_%d", k, a));
      heffcent_And_ZDCOr[k][a] = (TH1F*)inf->Get(Form("heffcent_And_ZDCOr_%dn_%d", k, a));
      // heffcent_Or_ZDCAnd[k][a] = (TH1F*)inf->Get(Form("heffcent_Or_ZDCAnd_%dn_%d", k, a));
    }
    heff_And_ZDCAnd_int[k] = (TH1F*)inf->Get(Form("heff_And_ZDCAnd_int_%dn", k));
    heff_And_ZDCOr_int[k] = (TH1F*)inf->Get(Form("heff_And_ZDCOr_int_%dn", k));
    // heff_Or_ZDCAnd_int[k] = (TH1F*)inf->Get(Form("heff_Or_ZDCAnd_int_%dn", k));
  }
}

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

// gr
auto groc_And_ZDCAnd = xjjc::array2d<TGraph*>(l1trigger::nNeus, l1trigger::ncent),
  gfake_And_ZDCAnd = xjjc::array2d<TGraph*>(l1trigger::nNeus, l1trigger::ncent),
  groc_And_ZDCOr = xjjc::array2d<TGraph*>(l1trigger::nNeus, l1trigger::ncent),
  gfake_And_ZDCOr = xjjc::array2d<TGraph*>(l1trigger::nNeus, l1trigger::ncent),
  groc_Or_ZDCAnd = xjjc::array2d<TGraph*>(l1trigger::nNeus, l1trigger::ncent),
  gfake_Or_ZDCAnd = xjjc::array2d<TGraph*>(l1trigger::nNeus, l1trigger::ncent);
auto geffcent_And_ZDCAnd = xjjc::array2d<TGraphAsymmErrors*>(l1trigger::nNeus, l1trigger::nadc),
  geffcent_And_ZDCOr = xjjc::array2d<TGraphAsymmErrors*>(l1trigger::nNeus, l1trigger::nadc),
  geffcent_Or_ZDCAnd = xjjc::array2d<TGraphAsymmErrors*>(l1trigger::nNeus, l1trigger::nadc);
std::vector<TGraphErrors*> gr_heff_And_HFOnly(l1trigger::ncent), gr_heff_And_ZDCOr_int(l1trigger::nNeus);

void make_gr() {
  for (int k=0; k<l1trigger::nNeus; k++) {
    for (int l=0; l<l1trigger::ncent; l++) {
      groc_And_ZDCAnd[k][l] = roc(hrate_And_ZDCAnd[k], heff_And_ZDCAnd[k][l], Form("groc_And_ZDCAnd_%dn_%d", k, l));
      xjjroot::setthgrstyle(groc_And_ZDCAnd[k][l], l1style::colors[k], 20, 1.1, l1style::colors[k], 1, 2);
      gfake_And_ZDCAnd[k][l] = roc(hfake_And_ZDCAnd[k], heff_And_ZDCAnd[k][l], Form("gfake_And_ZDCAnd_%dn_%d", k, l));
      xjjroot::setthgrstyle(gfake_And_ZDCAnd[k][l], l1style::colors[k], 20, 1.1, l1style::colors[k], 1, 2);

      groc_And_ZDCOr[k][l] = roc(hrate_And_ZDCOr[k], heff_And_ZDCOr[k][l], Form("groc_And_ZDCOr_%dn_%d", k, l));
      xjjroot::setthgrstyle(groc_And_ZDCOr[k][l], l1style::colors[k], 25, 1.1, l1style::colors[k], 2, 2);
      gfake_And_ZDCOr[k][l] = roc(hfake_And_ZDCOr[k], heff_And_ZDCOr[k][l], Form("gfake_And_ZDCOr_%dn_%d", k, l));
      xjjroot::setthgrstyle(gfake_And_ZDCOr[k][l], l1style::colors[k], 25, 1.1, l1style::colors[k], 2, 2);

      // groc_Or_ZDCAnd[k][l] = roc(hrate_Or_ZDCAnd[k], heff_Or_ZDCAnd[k][l], Form("groc_Or_ZDCAnd_%dn_%d", k, l));
      // xjjroot::setthgrstyle(groc_Or_ZDCAnd[k][l], l1style::colors[k], 20, 1.1, l1style::colors[k], 1, 2);
      // gfake_Or_ZDCAnd[k][l] = roc(hfake_Or_ZDCAnd[k], heff_Or_ZDCAnd[k][l], Form("gfake_Or_ZDCAnd_%dn_%d", k, l));
      // xjjroot::setthgrstyle(gfake_Or_ZDCAnd[k][l], l1style::colors[k], 20, 1.1, l1style::colors[k], 1, 2);
    }
    for (int a=0; a<l1trigger::nadc; a++) {
      geffcent_And_ZDCAnd[k][a] = xjjana::shifthistcenter(heffcent_And_ZDCAnd[k][a], Form("geffcent_And_ZDCAnd_%dn_%d", k, a), 0, "X0Y0");
      xjjroot::setthgrstyle(geffcent_And_ZDCAnd[k][a], l1style::colors[k], 20, 1.1, l1style::colors[k], 1, 2);
      geffcent_And_ZDCOr[k][a] = xjjana::shifthistcenter(heffcent_And_ZDCOr[k][a], Form("geffcent_And_ZDCOr_%dn_%d", k, a), 0, "X0Y0");
      xjjroot::setthgrstyle(geffcent_And_ZDCOr[k][a], l1style::colors[k], 25, 1.1, l1style::colors[k], 2, 2);
      // geffcent_Or_ZDCAnd[k][a] = xjjana::shifthistcenter(heffcent_Or_ZDCAnd[k][a], Form("geffcent_Or_ZDCAnd_%dn_%d", k, a), 0, "X0Y0");
      // xjjroot::setthgrstyle(geffcent_Or_ZDCAnd[k][a], l1style::colors[k], 20, 1.1, l1style::colors[k], 1, 2);
    }
  }
  for (int l=0; l<l1trigger::ncent; l++) {
    gr_heff_And_HFOnly[l] = xjjana::shifthistcenter(heff_And_ZDCAnd[0][l], Form("gr_%s", heff_And_ZDCAnd[0][l]->GetName()), (int)-1);
    xjjroot::setthgrstyle(gr_heff_And_HFOnly[l], l1style::colors[l], 20, 1.1, l1style::colors[l], 1, 2);
  }
  for (int k=0; k<l1trigger::nNeus; k++) {
    gr_heff_And_ZDCOr_int[k] = xjjana::shifthistcenter(heff_And_ZDCOr_int[k], Form("gr_%s", heff_And_ZDCOr_int[k]->GetName()), (int)-1);
    xjjroot::setthgrstyle(gr_heff_And_ZDCOr_int[k], kBlack, 20, 1.1, kBlack, 1, 2);
  }
}

// draw

template<class T>
void drawshadow(T* hempty, Color_t cc = kGray, float guide = 15)
{
  hempty->Draw("AXIS");
  xjjroot::drawCMS(xjjroot::CMS::internal, tag_.c_str());
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

float hist_to_value(TFile* inf, std::string histname) {
  auto h = (TH1F*)inf->Get(histname.c_str());
  float value = h->GetBinContent(1);
  return value;
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

#endif
