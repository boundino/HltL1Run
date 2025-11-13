#ifndef __L1_HELPER_
#define __L1_HELPER_

#include "TH2F.h"

#include "xjjrootuti.h"
#include "xjjanauti.h"
#include "l1mb_constant.h"

#define COMBINE1D(EXPAND)                       \
  EXPAND(And)                                   \
  EXPAND(Or)                                    \
  //

#define COMBINE(EXPAND)                         \
  EXPAND(And, Or)                               \
  EXPAND(And, And)                              \
  EXPAND(Or, Or)                                \
  EXPAND(Or, And)                               \
  //

#define COMBINE3D(EXPAND, t)                    \
  EXPAND(And, Or, t)                            \
  EXPAND(And, And, t)                           \
  EXPAND(Or, Or, t)                             \
  EXPAND(Or, And, t)                            \
  //

#define COMBINE3D_ZAND(EXPAND, t)               \
  EXPAND(And, And, t)                           \
  EXPAND(Or, And, t)                            \
  //

#define COMBINESTYLE(EXPAND)                    \
  EXPAND(And, Or, 24, 2)                        \
  EXPAND(And, And, 20, 1)                       \
  EXPAND(Or, Or, 25, 2)                         \
  EXPAND(Or, And, 21, 1)                        \
  //

namespace l1style {
  int colors[] = { kBlack, kRed, kBlue, kGreen+2, kMagenta+1, kCyan+1 };
  Size_t mmsize = 1.1;
  Width_t llwidth = 2;
  enum position { Up, Down };
  float pos_y[] = { 0.86, 0.80 };
  float pos_x_split = 0.34;
  enum algo { No, And, Or };
  std::string t_algo[] = { "", "AND", "OR" };
  //
  void drawleg(algo hf, algo zdc,
               Style_t mstyle, Style_t lstyle,
               position pos) {
    if (hf) {
      xjjroot::drawtex(pos_x_split, pos_y[pos]-0.035, Form("HF_#bf{%s}", t_algo[hf].c_str()), 0.037, 32);
    }
    if (mstyle > 0) {
      int ncol = zdc?4:1;
      auto pos_x_leg = zdc?pos_x_split+0.16:pos_x_split;
      auto lleg = new TLegend(pos_x_leg+0.02, pos_y[pos]-0.06,
                              pos_x_leg+0.02+0.09*ncol, pos_y[pos]);
      lleg->SetNColumns(ncol);
      xjjroot::setleg(lleg, 0.037);
      if (zdc) {
        for (int k=0; k<l1trigger::nNeus; k++) {
          xjjroot::addentrybystyle(lleg, Form("(%dn)", k), "pl", colors[k], mstyle, mmsize, colors[k], lstyle, llwidth);
        }
      } else {
        xjjroot::addentrybystyle(lleg, "", "pl", colors[0], mstyle, mmsize, colors[0], lstyle, llwidth);
      }
      lleg->Draw();
      if (zdc)
        xjjroot::drawtex(pos_x_split, pos_y[pos]-0.035, Form("%s ZDC_#bf{%s}", (hf?" &":""), t_algo[zdc].c_str()), 0.037, 12);
    }
  }
  template<class T = TH1>
  void drawleg(algo hf = And, algo zdc = Or, T* gr = 0, position pos = Up) {
    if (gr) {
      drawleg(hf, zdc, gr->GetMarkerStyle(), gr->GetLineStyle(), pos);
    } else {
      drawleg(hf, zdc, -1, -1, pos);
    }
  }
  //
} // namespace l1style {

// common h
std::vector<TH1F*> hZDCdisGeV(l1trigger::nDirs, 0);
std::vector<TH2F*> hZDC_HF(l1trigger::nDirs, 0);
#define DECLAREH(h, z)                                                  \
  std::vector<TH1F*> hrate_##h##_ZDC##z(l1trigger::nNeus, 0);           \
  std::vector<TH1F*> hfake_##h##_ZDC##z(l1trigger::nNeus, 0);           \
  auto heff_##h##_ZDC##z = xjjc::array2d<TH1F*>(l1trigger::nNeus, l1trigger::ncent); \
  auto heffcent_##h##_ZDC##z = xjjc::array2d<TH1F*>(l1trigger::nNeus, l1trigger::nadc); \
  std::vector<TH1F*> heff_##h##_ZDC##z##_incl(l1trigger::nNeus, 0);     \
  std::vector<TH1F*> heff_##h##_ZDC##z##_interest(l1trigger::nNeus, 0); \

COMBINE(DECLAREH);

std::vector<TH1F*> heffden(l1trigger::ncent, 0);
TH1F *heffden_incl = 0, *heffden_interest = 0;

#define CREATEH(h, z)                                                   \
  hrate_##h##_ZDC##z[k] = new TH1F(Form("hrate_" #h "_ZDC" #z "_%dn", k), ";L1 HF threshold (ADC);", l1trigger::nadc, 0, l1trigger::nadc); \
  hfake_##h##_ZDC##z[k] = new TH1F(Form("hfake_" #h "_ZDC" #z "_%dn", k), ";L1 HF threshold (ADC);", l1trigger::nadc, 0, l1trigger::nadc); \
  for (int l=0; l<l1trigger::ncent; l++) {                              \
    heff_##h##_ZDC##z[k][l] = new TH1F(Form("heff_" #h "_ZDC" #z "_%dn_%d", k, l), ";L1 HF threshold (ADC);", l1trigger::nadc, 0, l1trigger::nadc); \
  }                                                                     \
  for (int a=0; a<l1trigger::nadc; a++) {                               \
    heffcent_##h##_ZDC##z[k][a] = new TH1F(Form("heffcent_" #h "_ZDC" #z "_%dn_%d", k, a), ";Centrality [%];", l1trigger::nbincent, 0, 100); \
  }                                                                     \
  heff_##h##_ZDC##z##_incl[k] = new TH1F(Form("heff_" #h "_ZDC" #z "_incl_%dn", k), ";L1 HF threshold (ADC);", l1trigger::nadc, 0, l1trigger::nadc); \
  heff_##h##_ZDC##z##_interest[k] = new TH1F(Form("heff_" #h "_ZDC" #z "_interest_%dn", k), ";L1 HF threshold (ADC);", l1trigger::nadc, 0, l1trigger::nadc); \


void create_hist() {
  for (int j=0; j<l1trigger::nDirs; j++) {
    hZDCdisGeV[j] = new TH1F(Form("hZDCdisGeV%s", l1trigger::mDir[j].c_str()), Form(";ZDC %s (GeV);Entries", l1trigger::mDir[j].c_str()), 500, 0, 1.2e+4);
    hZDC_HF[j] = new TH2F(Form("hZDC_HF%s", l1trigger::mDir[j].c_str()), Form(";HF %s E_{T} Sum (GeV);ZDC %s (GeV)", l1trigger::mDir[j].c_str(), l1trigger::mDir[j].c_str()), 100, 0, 10000, 100, 0, 1.2e+4);
  }
  for (int k=0; k<l1trigger::nNeus; k++) {
    COMBINE(CREATEH);
  }
  for (int l=0; l<l1trigger::ncent; l++) {
    heffden[l] = new TH1F(Form("heffden_%d", l), ";L1 HF threshold (ADC);", l1trigger::nadc, 0, l1trigger::nadc);
  }
  heffden_incl = new TH1F("heffden_incl", ";L1 HF threshold (ADC);", l1trigger::nadc, 0, l1trigger::nadc);
  heffden_interest = new TH1F("heffden_interest", ";L1 HF threshold (ADC);", l1trigger::nadc, 0, l1trigger::nadc);
}

#define READH(h, z)                                                     \
  hrate_##h##_ZDC##z[k] = (TH1F*)inf->Get(Form("hrate_" #h "_ZDC" #z "_%dn", k)); \
  hfake_##h##_ZDC##z[k] = (TH1F*)inf->Get(Form("hfake_" #h "_ZDC" #z "_%dn", k)); \
  for (int l=0; l<l1trigger::ncent; l++) {                              \
    heff_##h##_ZDC##z[k][l] = (TH1F*)inf->Get(Form("heff_" #h "_ZDC" #z "_%dn_%d", k, l)); \
  }                                                                     \
  for (int a=0; a<l1trigger::nadc; a++) {                               \
    heffcent_##h##_ZDC##z[k][a] = (TH1F*)inf->Get(Form("heffcent_" #h "_ZDC" #z "_%dn_%d", k, a)); \
  }                                                                     \
  heff_##h##_ZDC##z##_incl[k] = (TH1F*)inf->Get(Form("heff_" #h "_ZDC" #z "_incl_%dn", k)); \
  heff_##h##_ZDC##z##_interest[k] = (TH1F*)inf->Get(Form("heff_" #h "_ZDC" #z "_interest_%dn", k)); \


void read_hist(TFile* inf) {
  for (int j=0; j<l1trigger::nDirs; j++) {
    hZDCdisGeV[j] = (TH1F*)inf->Get(Form("hZDCdisGeV%s", l1trigger::mDir[j].c_str()));
    hZDCdisGeV[j]->GetXaxis()->SetMaxDigits(3);
    xjjroot::sethempty(hZDCdisGeV[j]);
    xjjroot::setthgrstyle(hZDCdisGeV[j], kBlack, 20, 0.9, kBlack, 1, 1);
    hZDC_HF[j] = (TH2F*)inf->Get(Form("hZDC_HF%s", l1trigger::mDir[j].c_str()));
    hZDC_HF[j]->GetXaxis()->SetMaxDigits(3);
    xjjroot::sethempty(hZDC_HF[j]);
    xjjroot::setthgrstyle(hZDC_HF[j], kBlack, 20, 0.9, kBlack, 1, 1);
  }
  for (int k=0; k<l1trigger::nNeus; k++) {
    COMBINE(READH);
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

#define DECLAREGR(h, z)                                                 \
  auto groc_##h##_ZDC##z = xjjc::array2d<TGraph*>(l1trigger::nNeus, l1trigger::ncent); \
  auto gfake_##h##_ZDC##z = xjjc::array2d<TGraph*>(l1trigger::nNeus, l1trigger::ncent); \
  auto geffcent_##h##_ZDC##z = xjjc::array2d<TGraphAsymmErrors*>(l1trigger::nNeus, l1trigger::nadc); \
  std::vector<TGraphErrors*> gr_heff_##h##_ZDC##z##_incl(l1trigger::nNeus, 0); \
  std::vector<TGraphErrors*> gr_heff_##h##_ZDC##z##_interest(l1trigger::nNeus, 0); \

COMBINE(DECLAREGR);
std::vector<TGraphErrors*> gr_heff_And_HFOnly(l1trigger::ncent), gr_heff_Or_HFOnly(l1trigger::ncent);


#define MAKEGR(h, z, ms, ls)                                            \
  for (int l=0; l<l1trigger::ncent; l++) {                              \
    groc_##h##_ZDC##z[k][l] = roc(hrate_##h##_ZDC##z[k], heff_##h##_ZDC##z[k][l], Form("groc_" #h "_ZDC" #z "_%dn_%d", k, l)); \
    xjjroot::setthgrstyle(groc_##h##_ZDC##z[k][l], l1style::colors[k], ms, 1.1, l1style::colors[k], ls, 2); \
    gfake_##h##_ZDC##z[k][l] = roc(hfake_##h##_ZDC##z[k], heff_##h##_ZDC##z[k][l], Form("gfake_" #h "_ZDC" #z "_%dn_%d", k, l)); \
    xjjroot::setthgrstyle(gfake_##h##_ZDC##z[k][l], l1style::colors[k], ms, 1.1, l1style::colors[k], ls, 2); \
  }                                                                     \
  for (int a=0; a<l1trigger::nadc; a++) {                               \
    geffcent_##h##_ZDC##z[k][a] = xjjana::shifthistcenter(heffcent_##h##_ZDC##z[k][a], Form("geffcent_" #h "_ZDC" #z "_%dn_%d", k, a), 0, "X0Y0"); \
    xjjroot::setthgrstyle(geffcent_##h##_ZDC##z[k][a], l1style::colors[k], ms, 1.1, l1style::colors[k], ls, 2); \
  }                                                                     \
  gr_heff_##h##_ZDC##z##_incl[k] = xjjana::shifthistcenter(heff_##h##_ZDC##z##_incl[k], Form("gr_%s", heff_##h##_ZDC##z##_incl[k]->GetName()), (int)-1); \
  xjjroot::setthgrstyle(gr_heff_##h##_ZDC##z##_incl[k], l1style::colors[k], ms, 1.1, l1style::colors[k], ls, 2); \
  gr_heff_##h##_ZDC##z##_interest[k] = xjjana::shifthistcenter(heff_##h##_ZDC##z##_interest[k], Form("gr_%s", heff_##h##_ZDC##z##_interest[k]->GetName()), (int)-1); \
  xjjroot::setthgrstyle(gr_heff_##h##_ZDC##z##_interest[k], l1style::colors[k], ms, 1.1, l1style::colors[k], ls, 2); \

void make_gr() {
  for (int k=0; k<l1trigger::nNeus; k++) {
    COMBINESTYLE(MAKEGR);
  }
  for (int l=0; l<l1trigger::ncent; l++) {
    gr_heff_And_HFOnly[l] = xjjana::shifthistcenter(heff_And_ZDCAnd[0][l], Form("gr_%s", heff_And_ZDCAnd[0][l]->GetName()), (int)-1);
    xjjroot::setthgrstyle(gr_heff_And_HFOnly[l], l1style::colors[l], 20, 1.1, l1style::colors[l], 1, 2);
    gr_heff_Or_HFOnly[l] = xjjana::shifthistcenter(heff_Or_ZDCAnd[0][l], Form("gr_%s", heff_Or_ZDCAnd[0][l]->GetName()), (int)-1);
    xjjroot::setthgrstyle(gr_heff_Or_HFOnly[l], l1style::colors[l], 21, 1.1, l1style::colors[l], 1, 2);
  }
}

// write
#define WRITEH(h, z)                                                    \
  for (auto& h : hrate_##h##_ZDC##z) xjjroot::writehist(h);             \
  for (auto& h : hfake_##h##_ZDC##z) xjjroot::writehist(h);             \
  for (auto& h : heff_##h##_ZDC##z)                                     \
    for (auto& hh : h) xjjroot::writehist(hh);                          \
  for (auto& h : heffcent_##h##_ZDC##z)                                 \
    for (auto& hh : h) xjjroot::writehist(hh);                          \
  for (auto& h : heff_##h##_ZDC##z##_incl) xjjroot::writehist(h);       \
  for (auto& h : heff_##h##_ZDC##z##_interest) xjjroot::writehist(h);   \

void write_hist() {
  for (auto& h : hZDCdisGeV) xjjroot::writehist(h);
  for (auto& h : hZDC_HF) xjjroot::writehist(h);

  COMBINE(WRITEH);
}

// draw

template<class T>
void drawshadow(T* hempty, Color_t cc = kGray, float guide = 15)
{
  hempty->Draw("AXIS");
  con.drawtag();
  if(cc > 0) {
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

void drawhfth(TGraph* gr, TH2* hempty, float dev=1./40)
{
  for(int i=0; i<gr->GetN(); i++)
    {
      double x = gr->GetPointX(i), y = gr->GetPointY(i);
      if (x >= hempty->GetXaxis()->GetXmin() && x <= hempty->GetXaxis()->GetXmax())
        xjjroot::drawtexnum(x, y+(hempty->GetYaxis()->GetXmax()-hempty->GetYaxis()->GetXmin())*dev, Form("%d", i), 0.03, (dev>0?21:23), 42, gr->GetMarkerColor());
    }
}

void drawyvalue(TGraph* gr, TH2* hempty, int ibin)
{
  double x = gr->GetPointX(ibin), y = gr->GetPointY(ibin);
  if (x >= hempty->GetXaxis()->GetXmin() && x <= hempty->GetXaxis()->GetXmax())
    xjjroot::drawtexnum(x, y+(hempty->GetYaxis()->GetXmax()-hempty->GetYaxis()->GetXmin())/15., Form("%.2f", y), 0.03, 23, 42, gr->GetMarkerColor());  
}

int nearest(TH1F* h, float frate, bool verbose = false)
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
  if (verbose)
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
