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
  std::vector<float> xaxis_2018 = {702}, yaxis_2018 = {0.316986};
  std::vector<float> xaxis_2023 = {394, 674, 1010}, yaxis_2023 = {0.270093, 0.393564, 0.493706};
  std::vector<float> xaxis_2023_1n = {394, 674, 1010}, yaxis_2023_1n = {0.0770896, 0.130832, 0.216338};

  auto gr_2018 = new TGraph(xaxis_2018.size(), xaxis_2018.data(), yaxis_2018.data());
  gr_2018->SetName("gr_2018");
  auto gr_2023 = new TGraph(xaxis_2023.size(), xaxis_2023.data(), yaxis_2023.data());
  gr_2023->SetName("gr_2023");
  auto gr_2023_1n = new TGraph(xaxis_2023_1n.size(), xaxis_2023_1n.data(), yaxis_2023_1n.data());
  gr_2023_1n->SetName("gr_2023_1n");

  xjjroot::setthgrstyle(gr_2018, xjjroot::mycolor_satmiddle["green"], 47, 2, xjjroot::mycolor_satmiddle["green"], 1, 3);
  xjjroot::setthgrstyle(gr_2023, kGray+3, 20, 1.3, kGray+3, 2, 3);
  xjjroot::setthgrstyle(gr_2023_1n, xjjroot::mycolor_satmiddle["red"], 21, 1.3, xjjroot::mycolor_satmiddle["red"], 1, 3);

  auto leg1 = new TLegend(0.25, 0.85-2*0.04, 0.45, 0.85);
  xjjroot::setleg(leg1, 0.035);
  leg1->AddEntry(gr_2018, "2018 HF", "p");
  leg1->AddEntry(gr_2023, "2023 HF", "p");
  auto leg2 = new TLegend(0.25, 0.85-2*0.04, 0.45, 0.85);
  xjjroot::setleg(leg2, 0.035);
  leg2->AddEntry(gr_2023, "HF only", "p");
  leg2->AddEntry(gr_2023_1n, "HF && ZDCor(1n)", "p");
  auto leg3 = new TLegend(0.25, 0.85-3*0.04, 0.45, 0.85);
  xjjroot::setleg(leg3, 0.035);
  leg3->AddEntry(gr_2018, "2018 (HF Only)", "p");
  leg3->AddEntry(gr_2023, "2023 (HF Only)", "p");
  leg3->AddEntry(gr_2023_1n, "2023 (HF && ZDC)", "p");

  TH2F* hempty = new TH2F("hempty", ";nBunch;Average fake rate", 10, 200, 1200, 10, 0, 0.6);
  hempty->GetXaxis()->SetNdivisions(505);
  xjjroot::sethempty(hempty);

  xjjroot::setgstyle(1);
  gStyle->SetPadRightMargin(xjjroot::margin_pad_right*2);
  xjjroot::mypdf* pdf = new xjjroot::mypdf("figspdf/combine/per.pdf", "c", 700, 600);
  pdf->prepare();
  hempty->Draw("AXIS");
  gr_2018->Draw("pl same");
  gr_2023->Draw("pl same");
  leg1->Draw();
  xjjroot::drawCMS("Preliminary", "PbPb (5.02/5.36 TeV)");
  xjjroot::drawtex(0.60, 0.20, "Similar efficiency", 0.035, 12, 62);
  pdf->write();

  pdf->prepare();
  hempty->Draw("AXIS");
  gr_2023->Draw("pl same");
  gr_2023_1n->Draw("pl same");
  leg2->Draw();
  xjjroot::drawCMS("Preliminary", "2023 PbPb (5.36 TeV)");
  xjjroot::drawtex(0.60, 0.20, "Similar efficiency", 0.035, 12, 62);
  pdf->write();

  pdf->prepare();
  hempty->Draw("AXIS");
  gr_2018->Draw("pl same");
  gr_2023->Draw("pl same");
  gr_2023_1n->Draw("pl same");
  leg3->Draw();
  xjjroot::drawCMS("Preliminary", "PbPb (5.02 TeV / 5.36 TeV)");
  xjjroot::drawtex(0.60, 0.20, "Similar efficiency", 0.035, 12, 62);
  pdf->write();

    
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
