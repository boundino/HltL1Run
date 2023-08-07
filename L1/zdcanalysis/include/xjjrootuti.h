#ifndef _XJJROOT_UTI_H_
#define _XJJROOT_UTI_H_

/* xjjrootuti.h */

#include <TTree.h>
#include <TLatex.h>
#include <TLegend.h>
#include <TLine.h>
#include <TStyle.h>
#include <TH1.h>
#include <TGaxis.h>
#include <TF1.h>
#include <TMath.h>
#include <TEfficiency.h>
#include <TGraphErrors.h>
#include <TGraphAsymmErrors.h>
#include <TSystem.h>
#include <TFile.h>
#include <TCanvas.h>

#include <vector>
#include <iostream>
#include <iomanip>

#include "xjjcuti.h"

namespace xjjroot
{
  bool silence__ = false; void silence(bool s = true) { silence__ = s; }
  
  const float margin_pad_left = 0.18;
  const float margin_pad_right = 0.043;
  const float margin_pad_bottom = 0.145;
  const float margin_pad_top = 0.1;

  std::vector<Color_t> colorlist_light  = {kGreen-8, kRed-9, kAzure-9, kOrange-4, kMagenta-8, kCyan-8, kYellow-6, kBlue-8, kPink+1, kViolet-9};
  std::vector<Color_t> colorlist_middle = {kGreen+2, kRed-3, kAzure-3, kOrange-3, kMagenta-5, kCyan-2, kYellow+2, kBlue-5, kPink+2, kViolet+7};
  std::vector<Color_t> colorlist_dark   = {kGreen+3, kRed+2, kAzure-6, kOrange+5, kMagenta-1, kCyan+3, kYellow+3, kBlue-1, kPink+3, kViolet+4};
  std::vector<int> colorlist_bw = {TColor::GetColor("#000000"), TColor::GetColor("#191919"), TColor::GetColor("#323232"), TColor::GetColor("#4c4c4c"),
                                   TColor::GetColor("#5d5d5d"), TColor::GetColor("#666666"), TColor::GetColor("#7f7f7f"), TColor::GetColor("#999999"),
                                   TColor::GetColor("#b2b2b2"), TColor::GetColor("#cccccc"), TColor::GetColor("#d3d3d3"), TColor::GetColor("#e2e2e2")};
  // mycolor_satmiddle[cc], mycolor_light[cc], mycolor_middle[cc], mycolor_dark[cc]
  std::vector<std::string> cc = {"red", "azure", "green", "magenta", "orange", "greenblue", "pink", "cyan", "yellow", "blue", "violet"};  

  std::vector<Style_t> markerlist_solid = {21, 20, 34, 47, 33, 43, 22, 23};
  std::vector<Style_t> markerlist_open = {24, 25, 26, 27, 28, 30, 32, 42, 46, 44};
  
  void setgstyle(Int_t padtick=0, Width_t lwidth=2);
  template <class T> void sethempty(T* hempty, Float_t xoffset=0, Float_t yoffset=0, Float_t xsize=0.05, Float_t ysize=0.05);
  template <class T> void setthgr(T* hempty, Float_t xoffset=0, Float_t yoffset=0);
  template <class T> void setthgrstyle(T* h, Color_t mcolor=-1, Style_t mstyle=-1, Size_t msize=-1,
                                       Color_t lcolor=-1, Style_t lstyle=-1, Width_t lwidth=-1,
                                       Color_t fcolor=-1, Float_t falpha=-1, Style_t fstyle=-1,
                                       Float_t lalpha=-1, Float_t malpha=-1);
  template <class T> void setlinestyle(T* h, Color_t lcolor=-1, Style_t lstyle=-1, Width_t lwidth=-1, Float_t lalpha=-1);
  template <class T> void settfstyle(T* h, Color_t lcolor=-1, Style_t lstyle=-1, Width_t lwidth=-1,
                                     Color_t fcolor=-1, Float_t falpha=-1, Style_t fstyle=-1);
  template <class T> void setmarkerstyle(T* h, Color_t mcolor=-1, Style_t mstyle=-1, Size_t msize=-1);
  void drawCMSleft(TString content="#scale[1.25]{#bf{CMS}} #it{Internal}", Float_t xpos=0, Float_t ypos=0, Float_t tsize=0.04);
  void drawCMSright(TString content="#sqrt{s_{NN}} = 5.02 TeV", Float_t xpos=0, Float_t ypos=0, Float_t tsize=0.04);
  void drawCMS(TString contentleft="#scale[1.25]{#bf{CMS}} #it{Internal}", TString contentright="PbPb #sqrt{s_{NN}} = 5.02 TeV");
  void settex(TLatex* tex, Float_t tsize=0.04, Short_t align=12, Style_t font=42, Color_t color=kBlack, Float_t tangle=0);
  TLatex* drawtex(Double_t x, Double_t y, const char *text,
                  Float_t tsize=0.04, Short_t align=12, Style_t font=42, Color_t color=kBlack, Float_t tangle=0, bool draw = true);
  TLatex* drawtexnum(Double_t x, Double_t y, const char *text,
                     Float_t tsize=0.04, Short_t align=12, Style_t font=42, Color_t color=kBlack, Float_t tangle=0, bool draw = true);
  void drawtexgroup(Double_t x, Double_t y, std::vector<std::string> text, int ncol=1, Double_t colwid=0.2,
                    Float_t tsize=0.04, Short_t align=12, Style_t font=42, std::vector<Color_t> color=colorlist_middle);
  void setleg(TLegend* leg, Float_t tsize=0.04);
  void setlegndraw(TLegend* leg, Float_t tsize=0.04);
  TLine* drawline(Double_t x1, Double_t y1, Double_t x2, Double_t y2,
                  Color_t lcolor=kBlack, Style_t lstyle=1, Width_t lwidth=2, Float_t lalpha=1);
  TBox* drawbox(Double_t x1, Double_t y1, Double_t x2, Double_t y2,
                Color_t fcolor=kGray, Float_t falpha=0.4, Style_t fstyle=1001,
                Color_t lcolor=0, Style_t lstyle=1, Width_t lwidth=0);
  TGaxis* drawaxis(Double_t xmin, Double_t ymin, Double_t xmax, Double_t ymax, 
                   Double_t wmin, Double_t wmax, 
                   Color_t lcolor=kBlack, Style_t lstyle=1, Width_t lwidth=1,
                   Option_t *chopt="", 
                   Float_t labelsize=gStyle->GetLabelSize("Y"), Style_t labelfont=gStyle->GetLabelFont("Y"),
                   Int_t ndiv=510, Double_t gridlength=0);
  TGraph* drawpoint(Double_t x, Double_t y, Color_t mcolor=-1, Style_t mstyle=-1, Size_t msize=-1);

  template<class T> void printhist(T* hh, int w=10);
  template<class T> void printobject(T* hh);
  template<class T> void writehist(T* hh, int w=10) { if(!silence__) { printhist(hh, w); } hh->Write(); }
  void printhistvalue(TH1* hh, std::vector<int> bins={});
  template<class T> T* gethist(TFile* inf, std::string name);
  template<class T> T* gethist(std::string name);

  void mkdir(std::string outputfile);
  void saveas(TCanvas* c, std::string outputfile, std::string opt="WT");
  TFile* newfile(std::string outputfile);
  void drawcomment(std::string comment, std::string opt="lb") {
    xjjroot::drawtex((opt.front()=='r'?1:0), (opt.back()=='t'?1:0), comment.c_str(), 0.024,
                     ((opt.front()=='r')*2+1)*10+((opt.back()=='t')*2+1), 42, kGray); }
  void writetex(std::string tr, std::string br, std::string str);
  std::string readtex(TTree* t, std::string br);
}

/* ---------- */

void xjjroot::setgstyle(Int_t padtick/*=0*/, Width_t lwidth/*=2*/)
{
  gStyle->SetOptTitle(0);
  gStyle->SetOptStat(0);
  gStyle->SetEndErrorSize(0);
  gStyle->SetTextSize(0.05);
  gStyle->SetTextFont(42);
  gStyle->SetPadRightMargin(xjjroot::margin_pad_right);
  gStyle->SetPadLeftMargin(xjjroot::margin_pad_left);
  gStyle->SetPadTopMargin(xjjroot::margin_pad_top);
  gStyle->SetPadBottomMargin(xjjroot::margin_pad_bottom);
  gStyle->SetTitleX(.0f);
  gStyle->SetPadTickX(0);
  gStyle->SetPadTickY(0);
  gStyle->SetLineWidth(1);
  gStyle->SetFrameLineWidth(1);
  if(padtick==1 || padtick==3)
    {
      gStyle->SetPadTickX(1);
      gStyle->SetPadTickY(1);
    }
  if(padtick==2 || padtick==3) { gStyle->SetLineWidth(lwidth); gStyle->SetFrameLineWidth(lwidth);}
}

template <class T>
void xjjroot::sethempty(T* hempty, Float_t xoffset/*=0*/, Float_t yoffset/*=0*/, Float_t xsize/*=0.05*/, Float_t ysize/*=0.05*/)
{
  hempty->GetXaxis()->CenterTitle();
  hempty->GetYaxis()->CenterTitle();
  hempty->GetXaxis()->SetTitleOffset(1.10+xoffset);
  hempty->GetYaxis()->SetTitleOffset(1.30+yoffset);
  hempty->GetXaxis()->SetTitleSize(xsize);
  hempty->GetYaxis()->SetTitleSize(ysize);
  hempty->GetXaxis()->SetTitleFont(42);
  hempty->GetYaxis()->SetTitleFont(42);
  hempty->GetXaxis()->SetLabelFont(42);
  hempty->GetYaxis()->SetLabelFont(42);
  hempty->GetXaxis()->SetLabelSize(0.045);
  hempty->GetYaxis()->SetLabelSize(0.045);
  hempty->SetStats(0);
}

template <class T>
void xjjroot::setthgr(T* hempty, Float_t xoffset/*=0*/, Float_t yoffset/*=0*/)
{
  hempty->GetXaxis()->CenterTitle();
  hempty->GetYaxis()->CenterTitle();
  hempty->GetXaxis()->SetTitleOffset(1.10+xoffset);
  hempty->GetYaxis()->SetTitleOffset(1.30+yoffset);
  hempty->GetXaxis()->SetTitleSize(0.05);
  hempty->GetYaxis()->SetTitleSize(0.05);
  hempty->GetXaxis()->SetTitleFont(42);
  hempty->GetYaxis()->SetTitleFont(42);
  hempty->GetXaxis()->SetLabelFont(42);
  hempty->GetYaxis()->SetLabelFont(42);
  hempty->GetXaxis()->SetLabelSize(0.045);
  hempty->GetYaxis()->SetLabelSize(0.045);
  hempty->SetStats(0);
  hempty->SetMarkerStyle(20);
  hempty->SetMarkerSize(1.1);
  hempty->SetMarkerColor(kBlack);
  hempty->SetLineColor(kBlack);
}

template <class T>
void xjjroot::setthgrstyle(T* h, Color_t mcolor/*=-1*/, Style_t mstyle/*=-1*/, Size_t msize/*=-1*/,
                           Color_t lcolor/*=-1*/, Style_t lstyle/*=-1*/, Width_t lwidth/*=-1*/,
                           Color_t fcolor/*=-1*/, Float_t falpha/*=-1*/, Style_t fstyle/*=-1*/,
                           Float_t lalpha/*=-1*/, Float_t malpha/*=-1*/)
{
  if(mcolor>=0) h->SetMarkerColor(mcolor);
  if(mstyle>=0) h->SetMarkerStyle(mstyle);
  if(msize>=0)  h->SetMarkerSize(msize);
  if(lcolor>=0) h->SetLineColor(lcolor);
  if(lstyle>=0) h->SetLineStyle(lstyle);
  if(lwidth>=0) h->SetLineWidth(lwidth);
  if(fcolor>=0) h->SetFillColor(fcolor);
  if(falpha>=0) h->SetFillColorAlpha(fcolor, falpha);
  if(fstyle>=0) h->SetFillStyle(fstyle);
  if(lalpha>=0) h->SetLineColorAlpha(lcolor, lalpha);
  if(malpha>=0) h->SetMarkerColorAlpha(mcolor, malpha);
}

template <class T>
void xjjroot::setlinestyle(T* h, Color_t lcolor/*=-1*/, Style_t lstyle/*=-1*/, Width_t lwidth/*=-1*/, Float_t lalpha/*=-1*/)
{
  if(lcolor>=0) h->SetLineColor(lcolor);
  if(lalpha>=0) h->SetLineColorAlpha(lcolor, lalpha);
  if(lstyle>=0) h->SetLineStyle(lstyle);
  if(lwidth>=0) h->SetLineWidth(lwidth);
}

template <class T>
void xjjroot::settfstyle(T* h, Color_t lcolor/*=-1*/, Style_t lstyle/*=-1*/, Width_t lwidth/*=-1*/,
                         Color_t fcolor/*=-1*/, Float_t falpha/*=-1*/, Style_t fstyle/*=-1*/)
{
  h->SetNpx(1000);
  if(lcolor>=0) h->SetLineColor(lcolor);
  if(lstyle>=0) h->SetLineStyle(lstyle);
  if(lwidth>=0) h->SetLineWidth(lwidth);
  if(fcolor>=0) h->SetFillColor(fcolor);
  if(falpha>=0) h->SetFillColorAlpha(fcolor, falpha);
  if(fstyle>=0) h->SetFillStyle(fstyle);
}

template <class T>
void xjjroot::setmarkerstyle(T* h, Color_t mcolor/*=-1*/, Style_t mstyle/*=-1*/, Size_t msize/*=-1*/)
{
  if(mcolor>=0) h->SetMarkerColor(mcolor);
  if(mstyle>=0) h->SetMarkerStyle(mstyle);
  if(msize>=0)  h->SetMarkerSize(msize);
}

void xjjroot::drawCMS(TString contentleft/*="#scale[1.25]{#bf{CMS}} #it{Internal}"*/,
                      TString contentright/*="PbPb #sqrt{s_{NN}} = 5.02 TeV"*/)
{
  drawCMSleft(contentleft);
  drawCMSright(contentright);
}

void xjjroot::drawCMSleft(TString content/*="#scale[1.25]{#bf{CMS}} #it{Internal}"*/,
                          Float_t xpos/*=0*/, Float_t ypos/*=0*/, Float_t tsize/*=0.04*/)
{
  if(content=="") content = "Internal";
  if(content=="Preliminary" || 
     content=="Simulation" ||
     content=="Projection" ||
     content=="Internal") content = Form("#scale[1.25]{#bf{CMS}} #it{%s}", content.Data());
  if(content=="CMS") content = "#scale[1.25]{#bf{CMS}}";
  TLatex* texCms = new TLatex(gStyle->GetPadLeftMargin()+xpos,(1-gStyle->GetPadTopMargin())*1.02+ypos, content.Data());
  texCms->SetNDC();
  texCms->SetTextAlign(11);
  texCms->SetTextSize(tsize);
  texCms->SetTextFont(42);
  texCms->Draw();
}

void xjjroot::drawCMSright(TString content/*="#sqrt{s_{NN}} = 5.02 TeV"*/,
                           Float_t xpos/*=0*/, Float_t ypos/*=0*/, Float_t tsize/*=0.04*/)
{
  if(content=="pp" || content=="PbPb") content = Form("%s #sqrt{s_{NN}} = 5.02 TeV", content.Data());
  TLatex* texCol = new TLatex((1-gStyle->GetPadRightMargin())+xpos,(1-gStyle->GetPadTopMargin())*1.02+ypos, content.Data());
  texCol->SetNDC();
  texCol->SetTextAlign(31);
  texCol->SetTextSize(tsize);
  texCol->SetTextFont(42);
  texCol->Draw();
}

void xjjroot::settex(TLatex* tex, Float_t tsize/*=0.04*/, Short_t align/*=12*/,
                     Style_t font/*=42*/, Color_t color/*=kBlack*/, Float_t tangle/*=0*/)
{
  tex->SetNDC();
  tex->SetTextFont(font);
  tex->SetTextAlign(align);
  tex->SetTextSize(tsize);
  tex->SetTextColor(color);
  tex->SetTextAngle(tangle);
}

TLatex* xjjroot::drawtex(Double_t x, Double_t y, const char* text,
                         Float_t tsize/*=0.04*/, Short_t align/*=12*/, Style_t font/*=42*/,
                         Color_t color/*=kBlack*/, Float_t tangle/*=0*/, bool draw/*=true*/)
{
  TLatex* tex = new TLatex(x, y, text);
  xjjroot::settex(tex, tsize, align, font, color, tangle);
  if(draw) tex->Draw();
  return tex;
}

TLatex* xjjroot::drawtexnum(Double_t x, Double_t y, const char* text,
                            Float_t tsize/*=0.04*/, Short_t align/*=12*/, Style_t font/*=42*/,
                            Color_t color/*=kBlack*/, Float_t tangle/*=0*/, bool draw/*=true*/)
{
  TLatex* tex = new TLatex(x, y, text);
  xjjroot::settex(tex, tsize, align, font, color, tangle);
  tex->SetNDC(false);
  if(draw) tex->Draw();
  return tex;
}

void xjjroot::drawtexgroup(Double_t x, Double_t y, std::vector<std::string> text,
                           int ncol/*=1*/, Double_t colwid/*=0.2*/,
                           Float_t tsize/*=0.04*/, Short_t align/*=12*/, Style_t font/*=42*/, std::vector<Color_t> color/*=std::vector<Color_t>()*/)
{
  double lspace = tsize+0.005;
  bool left = true, top = true;
  if(align == 31 || align == 33) left = false;
  if(align == 11 || align == 31) top = false;
  for(unsigned t=0; t<text.size(); t++)
    {
      double xx = x + colwid*(t%ncol);
      if(!left) xx = x - colwid*(t%ncol);
      double yy = y - lspace*(t/ncol);
      if(!top) yy = y + lspace*(t/ncol);
      constexpr unsigned fallback = kBlack;
      Color_t cc = t<color.size()?color[t]:fallback;
      drawtex(xx, yy, text[t].c_str(), tsize, align, font, cc);
    }
}

void xjjroot::setleg(TLegend* leg, Float_t tsize/*=0.04*/)
{
  leg->SetBorderSize(0);
  leg->SetFillStyle(0);
  leg->SetTextFont(42);
  leg->SetTextSize(tsize);
}

void xjjroot::setlegndraw(TLegend* leg, Float_t tsize/*=0.04*/)
{
  xjjroot::setleg(leg, tsize);
  leg->Draw();
}

TLine* xjjroot::drawline(Double_t x1, Double_t y1, Double_t x2, Double_t y2,
                         Color_t lcolor/*=kBlack*/, Style_t lstyle/*=1*/, Width_t lwidth/*=2*/, Float_t lalpha/*=1*/)
{
  TLine* l = new TLine(x1, y1, x2, y2);
  xjjroot::setlinestyle(l, lcolor, lstyle, lwidth, lalpha);
  l->Draw();
  return l;
}

TBox* xjjroot::drawbox(Double_t x1, Double_t y1, Double_t x2, Double_t y2,
                       Color_t fcolor/*=kGray*/, Float_t falpha/*=0.4*/, Style_t fstyle/*=1001*/,
                       Color_t lcolor/*=0*/, Style_t lstyle/*=1*/, Width_t lwidth/*=0*/)
{
  TBox* b = new TBox(x1, y1, x2, y2);
  b->SetFillColor(fcolor);
  b->SetFillColorAlpha(fcolor, falpha);
  b->SetFillStyle(fstyle);
  b->SetLineColor(lcolor);
  b->SetLineStyle(lstyle);
  b->SetLineWidth(lwidth);
  b->Draw();
  return b;
}

TGaxis* xjjroot::drawaxis(Double_t xmin, Double_t ymin, Double_t xmax, Double_t ymax, 
                          Double_t wmin, Double_t wmax, 
                          Color_t lcolor/*=kBlack*/, Style_t lstyle/*=1*/, Width_t lwidth/*=1*/,
                          Option_t *chopt/*=""*/, 
                          Float_t labelsize/*=gStyle->GetLabelSize("Y")*/, Style_t labelfont/*=gStyle->GetLabelFont("Y")*/, 
                          Int_t ndiv/*=510*/, Double_t gridlength/*=0*/)
{
  TGaxis* g = new TGaxis(xmin, ymin, xmax, ymax,
                         wmin, wmax, ndiv, chopt, gridlength);
  g->SetLabelColor(lcolor);
  g->SetLabelSize(labelsize);
  g->SetLabelFont(labelfont);
  g->SetTitleColor(lcolor);
  g->SetLineColor(lcolor);
  g->SetLineStyle(lstyle);
  g->SetLineWidth(lwidth);
  g->Draw();
  return g;
}

TGraph* xjjroot::drawpoint(Double_t x, Double_t y,
                           Color_t mcolor/*=-1*/, Style_t mstyle/*=-1*/, Size_t msize/*=-1*/)
{
  double vx[] = {x}, vy[] = {y};
  TGraph* gr = new TGraph(1, vx, vy);
  setthgrstyle(gr, mcolor, mstyle, msize);
  gr->Draw("p same");
  return gr;
}

/* ----- */

void xjjroot::printhistvalue(TH1* hh, std::vector<int> bins)
{ 
  std::cout<<std::left<<" \e[2m"<<hh->GetName()<<"\e[0m\e[36;1m ("<<hh->GetEntries()<<")\e[0m"<<std::endl;
  size_t lcenter = 0, lcontent = 0, lerror = 0;
  for(int i=1; i<=hh->GetXaxis()->GetNbins(); i++) {
    size_t ilcenter = xjjc::number_remove_zero(hh->GetBinCenter(i)).length();
    lcenter = std::max(lcenter, ilcenter);
    size_t ilcontent = xjjc::number_remove_zero(hh->GetBinContent(i)).length();
    lcontent = std::max(lcontent, ilcontent);
    size_t ilerror = xjjc::number_remove_zero(hh->GetBinError(i)).length();
    lerror = std::max(lerror, ilerror);
  }
  
  std::cout<<std::left<<" \e[38:5:232m\e[48:5:248m\e[4m "<<std::setw(lcenter+1)<<""<<"\e[38:5:232m\e[48:5:232m\e[4m"
           <<" \e[38:5:232m\e[48:5:248m\e[4m "<<std::setw(lcontent)<<"value"<<"\e[38:5:232m\e[48:5:232m\e[4m"
           <<" \e[38:5:232m\e[48:5:248m\e[4m "<<std::setw(lerror)<<"error"<<"\e[0m"
           <<std::endl; 
  for(int i=1; i<=hh->GetXaxis()->GetNbins(); i++) {
    if(bins.size()>0 && std::find(bins.begin(), bins.end(), i)==bins.end()) continue;
    // 
    std::cout<<std::left
             <<" \e[38:5:232m\e[48:5:248m "<<std::setw(lcenter+1)<<hh->GetBinCenter(i)<<"\e[38:5:232m\e[48:5:232m"
             <<" \e[38:5:232m\e[48:5:248m "<<std::setw(lcontent)<<hh->GetBinContent(i)<<"\e[38:5:232m\e[48:5:232m"
             <<" \e[38:5:232m\e[48:5:248m "<<std::setw(lerror)<<hh->GetBinError(i)<<"\e[0m"
             <<std::endl;
  }
}

template<class T> 
void xjjroot::printhist(T* hh, int w)
{ 
  std::cout<<std::left<<"\e[2m"<<std::setw(w)<<hh->GetName()<<"\e[0m\e[36;1m ("<<hh->GetEntries()<<")\e[0m"<<std::endl; 
}

template<class T> 
void xjjroot::printobject(T* hh)
{ 
  std::cout<<std::left<<"\e[2m"<<hh->GetName()<<"\e[0m"<<std::endl;
}

template<class T> 
T* xjjroot::gethist(TFile* inf, std::string name)
{ 
  T* hh = 0;
  if(!inf) { std::cout<<std::left<<"\e[31m(x) "<<name<<"\e[0m"<<std::endl; return hh; }
  hh = (T*)inf->Get(name.c_str());
  if(!hh) { std::cout<<std::left<<"\e[31m(x) "<<name<<"\e[0m"<<std::endl; return hh; }
  if(!silence__) printobject(hh); 
  return hh;
}

template<class T> 
T* xjjroot::gethist(std::string name)
{ 
  T* hh = 0;
  auto inputname = xjjc::str_divide(name, "::");
  auto inf = TFile::Open(inputname[0].c_str());
  if(!inf) { std::cout<<std::left<<"\e[31m(x) "<<inputname[0]<<"\e[0m"<<std::endl; return hh; }
  hh = gethist<T>(inf, inputname[1]);
  return hh;
}

void xjjroot::mkdir(std::string outputfile)
{
  if(outputfile.find("/")==std::string::npos) return;
  std::string dir("");
  size_t lastpos = 0;
  size_t pos = outputfile.find("/", 0);
  while(pos != std::string::npos)
    {
      std::string thistoken = outputfile.substr(lastpos, pos-lastpos);
      dir += (thistoken + "/");
      lastpos = pos+1;
      pos = outputfile.find("/", pos+1);
    }
  gSystem->Exec(Form("mkdir -p %s", dir.c_str()));
}

void xjjroot::saveas(TCanvas* c, std::string outputfile, std::string opt)
{
  if (opt.find("T") != std::string::npos)
    {
      std::time_t t = std::time(0);
      std::string datetime(100,0);
      datetime.resize(std::strftime(&datetime[0], datetime.size(),
                                    "%a %d %b %-H:%M:%S %Z %Y", std::localtime(&t)));
      drawcomment(datetime, "rb");
    }
  if (opt.find("W") != std::string::npos)
    drawcomment(outputfile);
  
  mkdir(outputfile);
  std::cout<<"\e[1m";
  c->SaveAs(outputfile.c_str());
  std::cout<<"\e[0m";
}

TFile* xjjroot::newfile(std::string outputfile)
{
  mkdir(outputfile);
  TFile* outf = new TFile(outputfile.c_str(), "recreate");
  outf->cd();
  return outf;
}

void xjjroot::writetex(std::string tr, std::string br, std::string str)
{
  std::string s(str);
  TTree* t = new TTree(tr.c_str(), "info");
  t->Branch(br.c_str(), &s);
  t->Fill();
  t->Write();
}

std::string xjjroot::readtex(TTree* t, std::string br)
{
  std::string *s = 0;
  t->SetBranchAddress(br.c_str(), &s);
  t->GetEntry(0);
  std::string ss(*s);
  return ss;
}

namespace xjjroot
{
  // int dummy = (TColor::SetColorThreshold(0), 0);
  std::map<std::string, int> mycolor_middle = {
    {"greenblue", TColor::GetColor("#6CA892")},
    {"orange",    TColor::GetColor("#C67D4B")},
    {"red",       TColor::GetColor("#BA6E6E")},
    {"azure",     TColor::GetColor("#4B6D97")},
    {"magenta",   TColor::GetColor("#AA7799")},
    {"green",     TColor::GetColor("#839169")},
    {"cyan",      TColor::GetColor("#44929b")},
    {"yellow",    TColor::GetColor("#DEA63B")},
    {"blue",      TColor::GetColor("#4C599C")},
    {"pink",      TColor::GetColor("#C07A97")},
    {"violet",    TColor::GetColor("#8473B3")},
  };
  std::map<std::string, int> mycolor_light = {
    {"greenblue", TColor::GetColor("#A7C5A5")},
    {"orange",    TColor::GetColor("#DFB89D")},
    {"red",       TColor::GetColor("#D7ABAB")},
    {"azure",     TColor::GetColor("#96AECB")},
    {"magenta",   TColor::GetColor("#CCAABB")},
    {"green",     TColor::GetColor("#95A273")},
    {"cyan",      TColor::GetColor("#8EBDC3")},
    {"yellow",    TColor::GetColor("#F3D391")},
    {"blue",      TColor::GetColor("#989FC6")},
    {"pink",      TColor::GetColor("#DDBFC6")},
    {"violet",    TColor::GetColor("#C4BFD0")},
  };
  std::map<std::string, int> mycolor_dark = {
    {"greenblue", TColor::GetColor("#406457")},
    {"orange",    TColor::GetColor("#8A5734")},
    {"red",       TColor::GetColor("#863F3F")},
    {"azure",     TColor::GetColor("#324864")},
    {"magenta",   TColor::GetColor("#754966")},
    {"green",     TColor::GetColor("#6D7850")},
    {"cyan",      TColor::GetColor("#2D6066")},
    {"yellow",    TColor::GetColor("#BB8413")},
    {"blue",      TColor::GetColor("#2D365E")},
    {"pink",      TColor::GetColor("#926678")},
    {"violet",    TColor::GetColor("#5E4D8D")},
  };
  std::map<std::string, int> mycolor_satmiddle = {
    {"greenblue", TColor::GetColor("#5BB997")},
    {"orange",    TColor::GetColor("#D77D3C")},
    {"red",       TColor::GetColor("#CF5959")},
    {"azure",     TColor::GetColor("#2D6BB4")},
    {"magenta",   TColor::GetColor("#BD659F")},
    {"green",     TColor::GetColor("#89AF4B")},
    {"cyan",      TColor::GetColor("#329FAE")},
    {"yellow",    TColor::GetColor("#EAAD31")},
    {"blue",      TColor::GetColor("#394CAC")},
    {"pink",      TColor::GetColor("#CF6E96")},
    {"violet",    TColor::GetColor("#7D64C4")},
  };
  std::map<std::string, int> mycolor_satmiddle2 = {
    {"greenblue", TColor::GetColor("#2CE8A4")},
    {"orange",    TColor::GetColor("#F37920")},
    {"red",       TColor::GetColor("#F43434")},
    {"azure",     TColor::GetColor("#0B68D5")},
    {"magenta",   TColor::GetColor("#E93AAD")},
    {"green",     TColor::GetColor("#95E119")},
    {"cyan",      TColor::GetColor("#16B4CA")},
    {"yellow",    TColor::GetColor("#FEB11B")},
    {"blue",      TColor::GetColor("#1734CF")},
    {"pink",      TColor::GetColor("#F54790")},
    {"violet",    TColor::GetColor("#6B3EEA")},
  };
}

#endif
