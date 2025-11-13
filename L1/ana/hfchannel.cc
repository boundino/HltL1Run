#include <TFile.h>
#include <TTree.h>
#include <TH1F.h>
#include <TH2F.h>
#include <string>
#include "xjjcuti.h"
#include "xjjrootuti.h"
#include "xjjanauti.h"
#include "xjjmypdf.h"
#include "config.h"

#ifndef LOOPBRANCHES
#define LOOPBRANCHES(EXPAND)                    \
  EXPAND(mMaxL1HFAdcPlus)                       \
  EXPAND(mMaxietaPlus)                          \
  EXPAND(mMaxiphiPlus)                          \
  EXPAND(mMaxdepthPlus)                         \
  EXPAND(mMaxL1HFAdcMinus)                      \
  EXPAND(mMaxietaMinus)                         \
  EXPAND(mMaxiphiMinus)                         \
  EXPAND(mMaxdepthMinus)                        \

#endif

struct hfmax {
#define DECLAREST(t) int t;
  LOOPBRANCHES(DECLAREST);
};

void setbranch(TTree* t, hfmax& hf) {
  t->SetBranchStatus("*", false);

#ifndef SETBRANCH
#define SETBRANCH(b)                            \
  t->SetBranchStatus(#b, true);                 \
  t->SetBranchAddress(#b, &hf.b);
#endif

  LOOPBRANCHES(SETBRANCH);
}

auto cc = TColor::GetColor("#005b96");

void draw(std::vector<TH2F*>& pieta, std::vector<TH2F*>& pphi, xjjroot::mypdf* pdf, std::string tag,
          std::vector<std::string> comment = {}) {
  pdf->getc()->Divide(4, 2);
  auto pps = {pieta, pphi};
  int count = 1;
  for (auto &p : pps) {
    for (int i=0; i<4; i++) {
      pdf->getc()->cd(count);
      xjjroot::sethempty(p[i]);
      p[i]->SetMarkerStyle(21);
      p[i]->SetMarkerSize(0.2);
      p[i]->SetMarkerColor(cc);
      p[i]->Draw("scat=0.5");
      xjjroot::drawCMS(xjjroot::CMS::internal, tag.c_str());
      float y = 0.89, dy = 0.04;
      for (auto str : comment) {
        xjjroot::drawtex(0.25, y-=dy, str.c_str(), 0.035);
      }
      xjjroot::drawtex(0.88, 0.85, Form("depth = %d", i+1), 0.035, 32);
      count++;
    }
  }
}

int macro(std::string param, long long int nevtmax = 1.e4)
{
  xjjc::config conf(param);
  std::string inputname = conf["Input"], outputdir = conf["Output"];
  auto tag = conf["Tag"];

  hfmax hf;
  
  auto inf = TFile::Open(inputname.c_str());
  if (!inf) {
    inputname = conf["Input_adcToGeV"];
    inf = TFile::Open(inputname.c_str());
  }
  TTree* adc = 0;
  for (const std::string& tname : {"HFAdcana/adc", "mbnt"})
    if (xjjana::tree_exist(inf, tname.c_str())) {
      adc = (TTree*)inf->Get(tname.c_str());
      std::cout<<"\e[32m(o) "<<tname<<"\e[0m"<<std::endl;
      break;
    } else {
      std::cout<<"\e[31m(x) "<<tname<<"\e[0m"<<std::endl;
    }
  if (!adc) { std::cout<<"error: bad tree."<<std::endl; return 1; }
  setbranch(adc, hf);

  std::vector<TH2F*> pietaPlus(4, 0), pietaMinus(4, 0),
    piphiPlus(4, 0), piphiMinus(4, 0),
    pPlus(4, 0), pMinus(4, 0);
  for (int i=0; i<4; i++) {
    pietaPlus[i] = new TH2F(Form("pietaPlus%d", i), ";ieta;Max ADC", 13, 29, 42, 19, 5, 24);
    piphiPlus[i] = new TH2F(Form("piphiPlus%d", i), ";iphi;Max ADC", 73, 0, 73, 19, 5, 24);
    pPlus[i] = new TH2F(Form("pPlus%d", i), ";ieta;iphi", 13, 29, 42, 73, 0, 73);
    pietaMinus[i] = new TH2F(Form("pietaMinus%d", i), ";ieta;Max ADC", 13, -41, -28, 19, 5, 24);
    piphiMinus[i] = new TH2F(Form("piphiMinus%d", i), ";iphi;Max ADC", 73, 0, 73, 19, 5, 24);
    pMinus[i] = new TH2F(Form("pMinus%d", i), ";ieta;iphi", 13, -41, -28, 73, 0, 73);
  }

  auto nentries = std::min(adc->GetEntries(), nevtmax);
  for (int i=0; i<nentries; i++) {
    xjjc::progressslide(i, nentries, 1000);
    adc->GetEntry(i);
    pietaPlus[hf.mMaxdepthPlus-1]->Fill(hf.mMaxietaPlus, hf.mMaxL1HFAdcPlus);
    piphiPlus[hf.mMaxdepthPlus-1]->Fill(hf.mMaxiphiPlus, hf.mMaxL1HFAdcPlus);
    // pPlus[hf.mMaxdepthPlus-1]->Fill(hf.mMaxietaPlus, hf.mMaxiphiPlus, hf.mMaxL1HFAdcPlus);
    pPlus[hf.mMaxdepthPlus-1]->Fill(hf.mMaxietaPlus, hf.mMaxiphiPlus);
    pietaMinus[hf.mMaxdepthMinus-1]->Fill(hf.mMaxietaMinus, hf.mMaxL1HFAdcMinus);
    piphiMinus[hf.mMaxdepthMinus-1]->Fill(hf.mMaxiphiMinus, hf.mMaxL1HFAdcMinus);
    // pMinus[hf.mMaxdepthMinus-1]->Fill(hf.mMaxietaMinus, hf.mMaxiphiMinus, hf.mMaxL1HFAdcMinus);
    pMinus[hf.mMaxdepthMinus-1]->Fill(hf.mMaxietaMinus, hf.mMaxiphiMinus);
  }
  xjjc::progressbar_summary(nentries);

  xjjroot::setgstyle();
  gStyle->SetLineScalePS(2);
  auto pdf = new xjjroot::mypdf("figspdf/"+outputdir+"/hfchannel.pdf", "c", 2400, 1200);
  pdf->prepare();
  draw(pPlus, pMinus, pdf, tag);
  pdf->write();
  pdf->prepare();
  draw(pietaPlus, piphiPlus, pdf, tag);
  pdf->write();
  pdf->prepare();
  draw(pietaMinus, piphiMinus, pdf, tag);
  pdf->write();

  pdf->close();

  return 0;
}

int main(int argc, char* argv[]) {
  if (argc == 2)
    return macro(argv[1]);
  return 1;
}
