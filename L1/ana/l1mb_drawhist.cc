#include <TFile.h>
#include <TH1F.h>
#include <TH2F.h>
#include <TLegend.h>
#include <string>
#include "xjjcuti.h"
#include "xjjrootuti.h"
#include "xjjmypdf.h"
#include "config.h"

#include "l1mb_constant.h"
#include "l1mb_helper.h"

float frate = 50; // 50
void draw_cover(std::vector<std::string> title, xjjroot::mypdf* pdf);
int macro(std::string param)
{
  xjjc::config conf(param);
  con.init(conf);
  if (!con.valid()) return 2;
  std::string outputdir = con.label;
  
  auto inf = TFile::Open(Form("rootfiles/%s/savehist.root", outputdir.c_str()));

  float hlt_roc = hist_to_value(inf, "hcent_hlt_rate"),
    hlt_fake = hist_to_value(inf, "hcent_hlt_fake");

  auto hcent = (TH1F*)inf->Get("hcent");
  xjjroot::sethempty(hcent);
  xjjroot::setthgrstyle(hcent, kBlack, 20, 0.2, kBlack, 1, 2);
  auto hcent_hlt_eff = (TH1F*)inf->Get("hcent_hlt_eff");

  // hist
  read_hist(inf);
  
  // gr
  make_gr();

  // leg
  auto leg_cent = new TLegend(0.22, 0.55-0.0375*l1trigger::ncent, 0.42, 0.55);
  xjjroot::setleg(leg_cent, 0.037);
  for (int l=0; l<l1trigger::ncent; l++)
    leg_cent->AddEntry(gr_heff_And_HFOnly[l], Form("Cent. %d-%d%s", l1trigger::cent[l]/2, l1trigger::cent[l+1]/2, "%"), "p");
  auto leg_HLT = new TLegend(0.66, 0.33, 0.85, 0.38);
  xjjroot::setleg(leg_HLT, 0.037);
  xjjroot::addentrybystyle(leg_HLT, "Online HLT", "p", l1style::colors[con.Ineu], 47, 2.);
  // hempty
  auto hemptyroc = new TH2F("hemptyroc", ";L1 Rate [kHz];Trigger Efficiency", 10, con.rate_min, con.rate_max, 10, 0, 1.35);
  xjjroot::sethempty(hemptyroc);
  auto hemptyfake = new TH2F("hemptyfake", ";Fake fraction (Inclusive centrality);Trigger Efficiency", 10, 0, 0.4, 10, 0, 1.35);
  xjjroot::sethempty(hemptyfake);
  auto hemptyeffcent = new TH2F("hemptyeffcent", ";Centrality [%];Trigger Efficiency", 10, 0, 100, 10, 0, 1.35);
  xjjroot::sethempty(hemptyeffcent);
  auto hemptyeffadc = new TH2F("hemptyeffadc", ";HF Threshold [ADC];Trigger Efficiency", 10, 0, l1trigger::nadc, 10, 0, 1.35);
  xjjroot::sethempty(hemptyeffadc);

  xjjroot::setgstyle(1);
  gStyle->SetPadRightMargin(xjjroot::margin_pad_right*2);
  auto pdf = new xjjroot::mypdf("figspdf/" + outputdir + "/per.pdf", "c", 700, 600);
  if (conf.has("Comment")) { 
    auto cover_title = conf.get_vec("Comment");
    if (conf.has("Tag")) {
      cover_title.insert(cover_title.begin(), conf.get("Tag"));
    }
    for (auto& t : cover_title) std::cout<<t<<std::endl;
    draw_cover(cover_title, pdf);
  }

  
  /********************
  / Efficiency vs ADC /
  ********************/
  
  pdf->prepare();
  drawshadow(hemptyeffadc, 0);
  xjjroot::drawline(con.Iadc, 0, con.Iadc, 1, kGray, 2, 2);
  xjjroot::drawline(0, 1, l1trigger::nadc, 1, kBlack, 2, 2);
  for (int l=0; l<l1trigger::ncent; l++) {
    gr_heff_And_HFOnly[l]->Draw("plXe same");
    drawyvalue(gr_heff_And_HFOnly[l], hemptyeffadc, con.Iadc);
  }
  leg_cent->Draw();
  l1style::drawleg(l1style::And, l1style::No);
  pdf->getc()->RedrawAxis();
  pdf->write();

  pdf->prepare();
  drawshadow(hemptyeffadc, 0);
  xjjroot::drawline(con.Iadc, 0, con.Iadc, 1, kGray, 2, 2);
  xjjroot::drawline(0, 1, l1trigger::nadc, 1, kBlack, 2, 2);
  for (int l=0; l<l1trigger::ncent; l++) {
    gr_heff_Or_HFOnly[l]->Draw("plXe same");
    drawyvalue(gr_heff_Or_HFOnly[l], hemptyeffadc, con.Iadc);
  }
  leg_cent->Draw();
  l1style::drawleg(l1style::Or, l1style::No);
  pdf->getc()->RedrawAxis();
  pdf->write();

  /********************
  / Int + Interest    / 
  ********************/

#define DRAW_INCL(h, z, t)                                              \
  pdf->prepare();                                                       \
  drawshadow(hemptyeffadc, 0);                                          \
  xjjroot::drawline(con.Iadc, 0, con.Iadc, gr_heff_##h##_ZDC##z##_##t[con.hasZDC() ? con.Ineu : 0]->GetPointY(con.Iadc), kGray, 2, 2); \
  xjjroot::drawline(0, 1, l1trigger::nadc, 1, kBlack, 2, 2);            \
  gr_heff_##h##_ZDCAnd_##t[0]->Draw("plXe same");                       \
  if (con.hasZDC()) {                                                   \
    for (int k=l1trigger::nNeus-1; k>=1; k--) {                         \
      gr_heff_##h##_ZDC##z##_##t[k]->Draw("plXe same");                 \
    }                                                                   \
    l1style::drawleg(l1style::h, l1style::z, gr_heff_##h##_ZDC##z##_##t[0]); \
  } else {                                                              \
    l1style::drawleg(l1style::h, l1style::No, gr_heff_##h##_ZDC##z##_##t[0]); \
  }                                                                     \
  xjjroot::drawtex(0.23, 0.4, Form("Cent. 0 - %d%s", l1trigger::cent[l1trigger::l_##t]/2, "%"), 0.038, 12); \
  drawyvalue(gr_heff_##h##_ZDC##z##_##t[con.hasZDC() ? con.Ineu : 0], hemptyeffadc, con.Iadc); \
  pdf->getc()->RedrawAxis();                                            \
  pdf->write();                                                         \

  draw_cover({ "Efficiency vs HF threshold", "Inclusive centrality 0 - 100%" }, pdf);
  if (con.hasZDC()) {
    COMBINE3D(DRAW_INCL, incl);
  } else {
    COMBINE3D_ZAND(DRAW_INCL, incl);
  }

  if (l1trigger::l_interest < l1trigger::l_incl) {
    draw_cover({ "Efficiency vs HF threshold", Form("Inclusive centrality 0 - %d%s", l1trigger::cent[l1trigger::l_interest]/2, "%") }, pdf);
    if (con.hasZDC()) {
      COMBINE3D(DRAW_INCL, interest);
    } else {
      COMBINE3D_ZAND(DRAW_INCL, interest);
    }
  }
  
  /********************
  / ZDC distribution  / 
  ********************/
  
  if (con.hasZDC()) {
    draw_cover({ "ZDC distributions" }, pdf);
  
    pdf->getc()->SetLogy();
    for (int j=0; j<l1trigger::nDirs; j++) {
      pdf->prepare();
      drawshadow(hZDCdisGeV[j], 0);
      hZDCdisGeV[j]->Draw("pe same");
      for (int k=1; k<l1trigger::nNeus; k++) {
        float biny = hZDCdisGeV[j]->GetBinContent(hZDCdisGeV[j]->FindBin(con.ZDCthre[j][k]));
        xjjroot::drawline(con.ZDCthre[j][k], hZDCdisGeV[j]->GetMinimum()/2., 
                          con.ZDCthre[j][k], biny,
                          l1style::colors[k], 7, 4);
        xjjroot::drawtexnum(con.ZDCthre[j][k], biny*5, Form("(%dn)", k), 0.038, 22, 62, l1style::colors[k]);
      }
      // xjjroot::drawtex(0.80, 0.80, "Mimic trigger (Ts2 - 0.45#timesTs1)", 0.038, 32);
      pdf->write();
    }
    pdf->getc()->SetLogy(0);
  }

  /***************************
  / Centrality distribution  / 
  ***************************/
  
  draw_cover({ "Centrality distributions", "(ZeroBias + offline event selections)" }, pdf);
  
  pdf->getc()->SetLogy(0);
  pdf->prepare();
  hcent->SetMinimum(0);
  hcent->SetMaximum(hcent->GetMaximum()*1.8);
  drawshadow(hcent, 0);
  // xjjroot::drawbox(85, hcent->GetMinimum(), 100, hcent->GetMaximum());
  hcent->Draw("hist e same");
  xjjroot::drawtex(0.24, 0.85, "Applied Offline Event Selection", 0.038, 13);
  pdf->getc()->RedrawAxis();
  pdf->write();

  /***************************
  / Centrality dependence    / 
  ***************************/

  if (l1trigger::ncent > 1) {
    draw_cover({ "Efficiency vs centrality" }, pdf);

    ////////////////////////////
    /* Fixed HF treshold      */ 
    ////////////////////////////

    int thisIneu = 1;
    for (int a : { 16 }) {
      pdf->prepare();
      drawshadow(hemptyeffcent, 0);
      geffcent_And_ZDCAnd[0][a]->Draw("same pl");    
      xjjroot::drawtex(0.23, 0.35, Form("Same HF threshold = %d", a), 0.035, 12, 62);
      xjjroot::drawtex(0.23, 0.30, Form("Rate [HF only] = %.2f kHz", hrate_And_ZDCAnd[0]->GetBinContent(a+1)), 0.038, 12);
      if (con.hasZDC()) {
        geffcent_And_ZDCOr[thisIneu][a]->Draw("same pl");
        xjjroot::drawtex(0.23, 0.25, Form("Rate [ZDC Or %dn] = %.2f kHz", thisIneu, hrate_And_ZDCOr[thisIneu]->GetBinContent(a+1)), 0.038, 12, 42, l1style::colors[thisIneu]);
      }
      pdf->getc()->RedrawAxis();
      pdf->write();
    }

    if (con.hasZDC()) {
      
      ////////////////////////////
      /* ZDC only               */ 
      ////////////////////////////

      pdf->prepare();
      drawshadow(hemptyeffcent, 0);
      for (int k=0; k<l1trigger::nNeus; k++) {
        int aAnd = 0, aOr = 0;
        geffcent_And_ZDCAnd[k][aAnd]->Draw("same pl");
        geffcent_And_ZDCOr[k][aOr]->Draw("same pl");
      }
      l1style::drawleg(l1style::No, l1style::And, geffcent_And_ZDCAnd[0][0], l1style::Up);
      l1style::drawleg(l1style::No, l1style::Or, geffcent_And_ZDCOr[0][0], l1style::Down);    
      xjjroot::drawtex(0.23, 0.35, "ZDC only", 0.035, 12, 62);
      pdf->getc()->RedrawAxis();
      pdf->write();
    }

    if (frate > 0) {

      ////////////////////////////
      /* Fixed rate             */ 
      ////////////////////////////

      for (float prescale : {1.}) {
        pdf->prepare();
        drawshadow(hemptyeffcent, 0);
      
        for (int k=0; k<l1trigger::nNeus; k++) {
          if (!con.hasZDC() && k) continue; 
          int aAnd = nearest(hrate_And_ZDCAnd[k], frate*prescale),
            aOr = nearest(hrate_And_ZDCOr[k], frate*prescale);
          geffcent_And_ZDCAnd[k][aAnd]->Draw("same pl");
          geffcent_And_ZDCOr[k][aOr]->Draw("same pl");
          if (!con.hasZDC()) {
            aOr = nearest(hrate_Or_ZDCOr[k], frate*prescale);
            geffcent_Or_ZDCOr[k][aOr]->Draw("same pl");
          }
        }
        xjjroot::drawtex(0.23, 0.30, Form("L1 MB rate #approx %.0f kHz", frate*prescale), 0.038, 12);
        if (con.hasZDC()) {
          l1style::drawleg(l1style::And, l1style::And, geffcent_And_ZDCAnd[0][0], l1style::Up);
          l1style::drawleg(l1style::And, l1style::Or, geffcent_And_ZDCOr[0][0], l1style::Down);
          xjjroot::drawtex(0.23, 0.35, "Fix rate", 0.035, 12, 62);
        } else {
          l1style::drawleg(l1style::And, l1style::No, geffcent_And_ZDCAnd[0][0], l1style::Up);
          l1style::drawleg(l1style::Or, l1style::No, geffcent_Or_ZDCOr[0][0], l1style::Down);
        }
        pdf->getc()->RedrawAxis();
        pdf->write();
      }
    }
  } // <-- if (l1trigger::ncent > 1) {

  /***************************
  / ROC: rate + fake, HF only/ 
  ***************************/

#define DRAWROCHF(t)                                                    \
  draw_cover({ Form("Efficiency vs %s", hempty##t->GetXaxis()->GetTitle()), "HF only" }, pdf); \
  for (int l=0; l<l1trigger::ncent; l++) {                              \
    pdf->prepare();                                                     \
    drawshadow(hempty##t, 0);                                           \
                                                                        \
    g##t##_And_ZDCAnd[0][l]->Draw("same pl");                           \
    drawhfth(g##t##_And_ZDCAnd[0][l], hempty##t, +1./40);               \
    g##t##_Or_ZDCOr[0][l]->Draw("same pl");                             \
    drawhfth(g##t##_Or_ZDCOr[0][l], hempty##t, -1./40);                 \
    l1style::drawleg(l1style::And, l1style::No, g##t##_And_ZDCAnd[0][l], l1style::Up); \
    l1style::drawleg(l1style::Or, l1style::No, g##t##_Or_ZDCOr[0][l], l1style::Down); \
    xjjroot::drawtex(0.88, 0.30, Form("Centrality %d-%d%s", l1trigger::cent[l]/2, l1trigger::cent[l+1]/2, "%"), 0.038, 32); \
    if (con.drawHLT() && con.Ineu == 0) {                               \
      xjjroot::drawpoint(hlt_##t, hcent_hlt_eff->GetBinContent(l+1), l1style::colors[con.Ineu], 47, 2.0); \
      leg_HLT->Draw();                                                  \
    }                                                                   \
    pdf->getc()->RedrawAxis();                                          \
    pdf->write();                                                       \
  }                                                                     \

  DRAWROCHF(roc);
  DRAWROCHF(fake);

  /***************************
  / ROC: rate + fake, w. ZDC  / 
  ***************************/
  
#define DRAWROC(h, z, t)                                                \
  draw_cover({ Form("Efficiency vs %s", hempty##t->GetXaxis()->GetTitle()), "HF#bf{" #h "} AND ZDC#bf{" #z "}"}, pdf); \
  for (int l=0; l<l1trigger::ncent; l++) {                              \
    pdf->prepare();                                                     \
    drawshadow(hempty##t, 0);                                           \
                                                                        \
    g##t##_##h##_ZDCAnd[0][l]->Draw("same pl");                         \
    for (int k=l1trigger::nNeus-1; k>=0; k--) {                         \
      g##t##_##h##_ZDC##z[k][l]->Draw("same pl");                       \
      drawhfth(g##t##_##h##_ZDC##z[k][l], hempty##t);                   \
    }                                                                   \
    l1style::drawleg(l1style::h, l1style::z, g##t##_##h##_ZDC##z[0][l]); \
    if (con.drawHLT()) {                                                \
      xjjroot::drawpoint(hlt_##t, hcent_hlt_eff->GetBinContent(l+1), l1style::colors[con.Ineu], 47, 2.0); \
      leg_HLT->Draw();                                                  \
    }                                                                   \
    xjjroot::drawtex(0.88, 0.30, Form("Centrality %d-%d%s", l1trigger::cent[l]/2, l1trigger::cent[l+1]/2, "%"), 0.038, 32); \
    pdf->getc()->RedrawAxis();                                          \
    pdf->write();                                                       \
  }

  if (con.hasZDC()) {
    COMBINE3D(DRAWROC, roc);
    COMBINE3D(DRAWROC, fake);
  }
  
  // --> Individuals (only HF and 2n)
  // auto leg_ind1 = new TLegend(0.24, 0.80, 0.87, 0.85);
  // leg_ind1->SetNColumns(2);
  // xjjroot::setleg(leg_ind1, 0.038);
  // leg_ind1->AddEntry(groc_And_ZDCAnd[0][0], "HF Only", "pl");
  // leg_ind1->AddEntry(groc_And_ZDCOr[con.Ineu][0], Form("HF + ZDC_OR (%dn)", con.Ineu), "pl");

  // // >>
  // for (int l=0; l<l1trigger::ncent; l++)
  //   {
  //     pdf->prepare();
  //     drawshadow(hempty, 0);
  //     // drawgrval(groc_And_ZDCAnd[0][l], hempty);
  //     // drawgrval(groc_And_ZDCOr[con.Ineu][l], hempty);
  //     groc_And_ZDCAnd[0][l]->Draw("pl same");
  //     groc_And_ZDCOr[con.Ineu][l]->Draw("pl same");
  //     drawhfth(groc_And_ZDCOr[0][l], hempty);
  //     drawhfth(groc_And_ZDCOr[con.Ineu][l], hempty);
  //     leg_ind1->Draw();
  //     xjjroot::drawtex(0.88, 0.30, Form("Centrality %d-%d%s", l1trigger::cent[l]/2, l1trigger::cent[l+1]/2, "%"), 0.038, 32);
  //     pdf->write();
  //   }

  // // >>
  // for (auto& h : groc_And_ZDCOr[con.Ineu]) h->SetLineStyle(1);
  // auto leg_ind = new TLegend(0.24, 0.76, 0.87, 0.86);
  // leg_ind->SetNColumns(2);
  // xjjroot::setleg(leg_ind, 0.038);
  // leg_ind->AddEntry(groc_And_ZDCAnd[0][0], "HF Only", "pl");
  // leg_ind->AddEntry(groc_And_ZDCOr[con.Ineu][0], Form("HF + ZDC_OR (%dn)", con.Ineu), "pl");
  // for (int l=0; l<l1trigger::ncent; l++)
  //   {
  //     pdf->prepare(); 
  //     drawshadow(hempty, 0);
  //     drawgrval(groc_And_ZDCAnd[0][l], hempty);
  //     drawgrval(groc_And_ZDCOr[con.Ineu][l], hempty);
  //     leg_ind->Draw();
  //     xjjroot::drawtex(0.88, 0.30, Form("Centrality %d-%d%s", l1trigger::cent[l]/2, l1trigger::cent[l+1]/2, "%"), 0.038, 32);
  //     pdf->write();
  //   }


  // publish
  // auto hemptyfake_zoom = new TH2F("hemptyfake_zoom", ";Fake fraction (Inclusive centrality);Efficiency", 10, 0, 0.6, 10, 0, 1.15);
  // xjjroot::sethempty(hemptyfake_zoom);

  // auto *leg_And_HF = new TLegend(0.25, 0.80, 0.38, 0.86);
  // xjjroot::setleg(leg_And_HF, 0.037);
  // leg_And_HF->AddEntry(groc_And_ZDCAnd[0][0], "HF Only", "p");
  // auto *leg_And_ZDC = new TLegend(0.66, 0.80, 0.86, 0.86);
  // leg_And_ZDC->SetNColumns(2);
  // xjjroot::setleg(leg_And_ZDC, 0.037);
  // leg_And_ZDC->AddEntry(groc_And_ZDCOr[1][0], "(1n)", "p");
  // leg_And_ZDC->AddEntry(groc_And_ZDCOr[2][0], "(2n)", "p");
  // auto t_And_ZDC = xjjroot::drawtex(0.64, 0.83, "HF & ZDC_#bf{OR}", 0.037, 32, 42, kBlack, 0, false);
  
  // for (int l=0; l<l1trigger::ncent; l++) {
  //   pdf->prepare();
  //   drawshadow(hemptyfake_zoom, 0);

  //   int aa = 15;
  //   for (int aa : {15, 16}) {
  //     xjjroot::drawbox(gfake_And_ZDCOr[2][l]->GetPointX(aa), gfake_And_ZDCOr[2][l]->GetPointY(aa),
  //                      gfake_And_ZDCOr[0][l]->GetPointX(aa), gfake_And_ZDCOr[0][l]->GetPointY(aa));
  //   }
    
  //   gfake_And_ZDCAnd[0][l]->Draw("same pl");
  //   gfake_And_ZDCOr[1][l]->Draw("same pl");
  //   gfake_And_ZDCOr[2][l]->Draw("same pl");

  //   leg_And_HF->Draw();
  //   leg_And_ZDC->Draw();
  //   t_And_ZDC->Draw();
  //   xjjroot::drawtex(0.88, 0.30, Form("Centrality %d-%d%s", l1trigger::cent[l]/2, l1trigger::cent[l+1]/2, "%"), 0.038, 32);
  //   pdf->getc()->RedrawAxis();
  //   pdf->write();
  // }
  
  pdf->close();

  // std::vector<TH1F*> vh = {hrate_And_ZDCAnd[0], hrate_And_ZDCOr[1], hrate_And_ZDCOr[2]};
  // printrate(vh);

  return 0;
}

int main(int argc, char* argv[])
{
  if (argc==2) return macro(argv[1]);
  return 1;
}

void draw_cover(std::vector<std::string> title, xjjroot::mypdf* pdf) {
  
  if (title.empty()) return;
  title[0] = "#bf{" + title[0] + "}";
  pdf->prepare();
  xjjroot::drawtexgroup(0.5, 0.5, title, 0.05, 22, 42, 1.15);
  pdf->write();
}
