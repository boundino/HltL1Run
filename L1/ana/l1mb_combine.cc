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

int yeardiv = 1;

int macro(std::vector<std::string> params)
{
  std::vector<int> thres = {15};
  auto yaxis = xjjc::array2d<std::vector<float>>(l1trigger::nNeus, thres.size());
  std::vector<float> xaxis(params.size(), 0);
  // std::vector<float> xaxis_2018(yeardiv, 0), xaxis_2023(params.size() - yeardiv, 0);

  for (int j=0; j<params.size(); j++) {
    xjjc::config conf(params[j]);
    conf.print();
    std::string outputdir = conf["Output"];
    float xvar = conf.vf("nBunch");
    xaxis[j] = xvar;
    // if (j<yeardiv)
    //   xaxis_2018[j] = xvar;
    // else
    //   xaxis_2023[j-yeardiv] = xvar;
    TFile* inf = TFile::Open(Form("rootfiles/%s/savehist.root", outputdir.c_str()));
    for(int k=0; k<l1trigger::nNeus; k++) {
      auto hfake = (TH1F*)inf->Get(Form("hfake_And_ZDCOr_%dn", k));
      for (int i=0; i<thres.size(); i++) {
        yaxis[k][i].push_back(hfake->GetBinContent(thres[i]+1));
        std::cout<<xaxis[j]<<", "<<hfake->GetBinContent(thres[i]+1)<<std::endl;
      }
      delete hfake;
    }
  }

  return 1;
  
  auto gr = xjjc::array2d<TGraph*>(l1trigger::nNeus, thres.size());
  auto leg = new TLegend(0.70, 0.85-0.04*(thres.size()+1), 0.85, 0.85);
  xjjroot::setleg(leg, 0.035);
  leg->AddEntry((TObject*)0, "HF thre", NULL);
  for(int k=0; k<l1trigger::nNeus; k++) {
    for (int i=0; i<thres.size(); i++) {
      gr[k][i] = new TGraph(params.size(), xaxis.data(), yaxis[k][i].data());
      gr[k][i]->SetName(Form("gr%d%d", k, i));
      xjjroot::setthgrstyle(gr[k][i], xjjroot::colorlist_middle[i], 21, 1, xjjroot::colorlist_middle[i], 1, 2);
      if (!k) {
        leg->AddEntry(gr[k][i], Form("%d", thres[i]), "p");
      }
    }
  }

  TH2F* hempty = new TH2F("hempty", ";nBunch;Average fake rate", 10, 200, 1500, 10, 0, 0.8);
  xjjroot::sethempty(hempty);

  xjjroot::setgstyle(1);
  gStyle->SetPadRightMargin(xjjroot::margin_pad_right*2);
  xjjroot::mypdf* pdf = new xjjroot::mypdf("figspdf/combine/per.pdf", "c", 700, 600);
  for (int k=0; k<l1trigger::nNeus; k++) {
    pdf->prepare();
    hempty->Draw("AXIS");
    for (int i=0; i<thres.size(); i++) {
      gr[k][i]->Draw("pl same");
    }
    leg->Draw();
    xjjroot::drawCMS("Preliminary", "2023 PbPb 5.36 TeV");
    xjjroot::drawtex(0.23, 0.83, k?Form("HF && ZDCOR (%dn)", k):"HF only", 0.035, 12, 62);
    pdf->write();
  }
  pdf->close();

  return 0;
}

int main(int argc, char* argv[]) {
  std::vector<std::string> confs = {
                                    "configs/run327327.conf",
                                    "configs/run374596.conf",
                                    "configs/run374778.conf",
                                    "configs/run374803.conf",
  };
  return macro(confs);
}
