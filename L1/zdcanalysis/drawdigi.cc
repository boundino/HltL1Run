#include <TFile.h>
#include <TTree.h>
#include <string>
#include <vector>

#include "include/xjjmypdf.h"

#define ZDCTS(EXPAND)                           \
  EXPAND(0)                                     \
  EXPAND(1)                                     \
  EXPAND(2)                                     \
  EXPAND(3)                                     \
  EXPAND(4)                                     \
  EXPAND(5)                                     \
  EXPAND(6)                                     \
  EXPAND(7)                                     \
  EXPAND(8)                                     \
    EXPAND(9)                                   \

#define ZDCPROP(EXPAND)                         \
  EXPAND(adc)                                   \
  EXPAND(chargefC)                              \

std::map<std::string, std::string> title = { {"adc", "ADC"}, {"chargefC", "Q [fC]"}};
std::vector<int> detid = {5, 6, 14, 15};
std::vector<std::string> detname = {"M_HAD1", "M_HAD2", "P_HAD1", "P_HAD2"};
      
void sethist(TH1* h) {
  xjjroot::sethempty(h, 0, 0);
  xjjroot::setthgrstyle(h, -1, -1, -1, kBlack, 1, 1, kBlack, 1, 3354);
  h->SetMinimum(0);
}

int macro(std::string input, int istart, int iend) {
  auto te = (TTree*)(TFile::Open(input.c_str())->Get("hiEvtAnalyzer/HiTree"));
  unsigned int run; te->SetBranchAddress("run", &run);
  unsigned long long evt; te->SetBranchAddress("evt", &evt);
  
  auto t = (TTree*)(TFile::Open(input.c_str())->Get("zdcanalyzer/zdcdigi"));
  int n; t->SetBranchAddress("n", &n);

#define DECLARE_BRANCHES(q)                                             \
  int adcTs##q[50]; t->SetBranchAddress("adcTs"#q, adcTs##q);                          \
  float chargefCTs##q[50]; t->SetBranchAddress("chargefCTs"#q, chargefCTs##q); \

  ZDCTS(DECLARE_BRANCHES)

    xjjroot::setgstyle();
  xjjroot::mypdf pdf("figspdf/zdc/zdcdigi.pdf", "c", 1200, 1200);
  // std::cout<<t->GetEntries()<<std::endl; 
  int nentries = t->GetEntries();
  if (istart >= nentries) return 2;
  for (int i = istart; i < std::min(iend, nentries); i++) {
    t->GetEntry(i);
    te->GetEntry(i);

    // if (i > istart+4 && adcTs1[15] < 150 && adcTs7[15] < 150) continue;

#define MAKE_HIST(p)                                                    \
    std::vector<TH1F*> h##p = std::vector<TH1F*>(4);                    \
    for (int j=0; j<4; j++) {                                           \
      h##p[j] = new TH1F(Form("h"#p"_%s", detname[j].c_str()), Form(";TS [25 ns];%s", title[#p].c_str()), 10, 0, 10); \
      sethist(h##p[j]);                                                 \
    }                                                                   \

    ZDCPROP(MAKE_HIST)

#define FILL_ADC_HIST(q) hadc[j]->SetBinContent(q+1, adcTs##q[detid[j]]);
#define FILL_CHARGEFC_HIST(q) hchargefC[j]->SetBinContent(q+1, chargefCTs##q[detid[j]]);

      for (int j=0; j<4; j++) {
        ZDCTS(FILL_ADC_HIST)
          ZDCTS(FILL_CHARGEFC_HIST)
          }
    
#define DRAW_HIST(p)                                                    \
    pdf.prepare();                                                      \
    pdf.getc()->Divide(2, 2);                                           \
    for (int j=0; j<4; j++) {                                           \
      h##p[j]->SetMaximum(h##p[j]->GetMaximum() *1.2);                  \
      pdf.getc()->cd(j+1);                                              \
      h##p[j]->Draw("hist");                                            \
      xjjroot::drawtex(0.88, 0.85, detname[j].c_str(), 0.038, 33, 62);  \
      xjjroot::drawtex(0.25, 0.85, Form("%i (75 ns)", run), 0.038, 13); \
      xjjroot::drawtex(0.25, 0.80, "MinBias", 0.038, 13);               \
      xjjroot::drawCMS("#bf{CMS ZDC}", "2018 PbPb 5.02 TeV");           \
      if (j==2)                                                         \
        xjjroot::drawcomment(Form("run %i evt %i", run, evt));          \
    }                                                                   \
    pdf.write();                                                        \
    for (auto h : h##p) delete h;                                       \

    ZDCPROP(DRAW_HIST)

                                     }
  pdf.close();

  return 0;
   
}

int main(int argc, char* argv[]) {
  if (argc == 4)
    return macro(argv[1], atoi(argv[2]), atoi(argv[3]));

  return 1;
}
