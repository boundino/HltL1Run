#include <TH2F.h>

#include "xjjrootuti.h"
#include "xjjmypdf.h"
#include "stat.h"

int main() {
  int cc_nevts = kBlack, cc_lumi = xjjroot::mycolor_satmiddle["azure"], cc_stroke = xjjroot::mycolor_satmiddle["red"];
  float max_nevts = 14, max_lumi = 2, scale = max_nevts / max_lumi;
  auto n = datasets.size();
  std::vector<float> v_nevts, v_x, v_lumi;
  std::vector<std::string> v_xlabel;
  for (auto& i : datasets) {
    v_xlabel.push_back(i.label);
    v_nevts.push_back(i.nevts / 1.e3);
    v_lumi.push_back(i.lumi / 1.e3 * scale);
  }

  auto hlumi = new TH1F("hlumi", "", n, 0, n);
  auto hempty = new TH2F("hempty", ";;Number of MB events / Billion", n, 0, n, 10, 0, max_nevts);
  xjjroot::sethempty(hempty, 0, -0.2);
  for (int i=0; i<n; i++) {
    hempty->GetXaxis()->SetBinLabel(i+1, v_xlabel[i].c_str());
    hempty->GetXaxis()->SetLabelSize(0.065);
    hempty->GetYaxis()->SetTitleFont(62);
    v_x.push_back(i+0.5);
    hlumi->SetBinContent(i+1, v_lumi[i]);
  }
  auto gnevts = new TGraph(n, v_x.data(), v_nevts.data());
  xjjroot::setthgrstyle(gnevts, cc_nevts, 21, 0.2, cc_nevts, 1, 3);
  auto glumi = new TGraph(n, v_x.data(), v_lumi.data());
  xjjroot::setthgrstyle(glumi, cc_lumi, 21, 0.2, cc_lumi, 1, 3);
  xjjroot::setthgrstyle(hlumi, cc_lumi, 21, 0.2, kBlack, 1, 2, cc_lumi, 0.15, 1001, 0.2);

  xjjroot::setgstyle();
  gStyle->SetPadLeftMargin(xjjroot::margin_pad_right * 3);
  gStyle->SetPadRightMargin(xjjroot::margin_pad_right * 3);
  gStyle->SetPadBottomMargin(xjjroot::margin_pad_bottom * 0.7);
  auto pdf = new xjjroot::mypdf("figspdf/combine/stat.pdf", "c", 800, 600);
  pdf->prepare();
  hempty->Draw("axis");
  hlumi->Draw("hist same");
  // glumi->Draw("l same");
  gnevts->Draw("pl same");
  auto axis_lumi = xjjroot::drawaxis((float)n, 0, (float)n, max_nevts, 0, max_lumi,
                                     cc_lumi, 1, gStyle->GetLineWidth(), "+L", hempty->GetYaxis()->GetLabelSize()*0.9);
  axis_lumi->SetTitle("CMS total recorded luminosity / nb^{-1}");
  axis_lumi->CenterTitle();
  xjjroot::setaxisstyle(hempty->GetYaxis(), axis_lumi);

  for (auto i : {2, 4, 5}) {
    float tp = 3.5;
    xjjroot::drawline(i, 0, i, tp, cc_stroke, 2, 2);
    auto lines = xjjc::str_divide(datasets[i-1].run, "\n");
    if (i != 5)
      xjjroot::drawarrow(i, tp-0.1, i-0.3, tp-0.1, cc_stroke, 1, 2);
    for (auto t : lines) {
      xjjroot::drawtexnum(i-0.1, tp-=0.8, t.c_str(), 0.04, 33, 42, cc_stroke);
    }
  }

  xjjroot::drawCMSleft(xjjroot::CMS::pre, 0.17-gStyle->GetPadLeftMargin(), -0.1);
  xjjroot::drawCMSright("(PbPb datasets)");
  xjjroot::drawtex(0.17, 0.78, "MinimumBias triggered events", 0.04);
  xjjroot::drawtex(0.17, 0.78-0.045, "(exclude contamination)", 0.04);

  
  pdf->write();
  pdf->close();

  return 0;
}

