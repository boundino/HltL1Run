#include <TFile.h>
#include <TH1F.h>

#include "config.h"
#include "xjjmypdf.h"
#include "xjjcuti.h"
#include "xjjrootuti.h"
#include "xjjanauti.h"

#include "include/const.h"
#include "include/forestgo.h"

struct Input {
  forestgo* fg;
  std::string name;
  float xsec;
};

Input create_input_from_conf(std::string t) {
  auto inps = xjjc::str_divide(t, "|");
  std::cout<<"\e[2m"<<inps[0]<<"\e[0m"<<std::endl;
  auto inf = TFile::Open(inps[0].c_str());
  Input in;
  in.fg = new forestgo(inf);
  in.name = inps[1];
  in.xsec = atof(inps[2].c_str());

  return in;
}

class Output {
public:
  Output(std::string name) : _name(name) { create_hist(); }
  std::string name() { return _name; }
  TH1F *hhiHF, *hhiHFpf, *hhiHFpf__sel, *hhiHFpf__selL1;
  std::map<std::string, TH1F*> hhiHFpf__selHF;
  TH2F *hhiHF_HFpf;
  TH1F *hhiHFtowerAND, *hhiHFtowerOR;
  
  void style_hist(const xjjroot::thgrstyle& t) {
    xjjroot::setthgrstyle(hhiHF, t);
    xjjroot::setthgrstyle(hhiHFpf, t);
    xjjroot::setthgrstyle(hhiHFpf__sel, t);
    xjjroot::setthgrstyle(hhiHFpf__selL1, t);
    xjjroot::setthgrstyle(hhiHFtowerAND, t);
    xjjroot::setthgrstyle(hhiHFtowerOR, t);
    for (auto& h : hhiHFpf__selHF) {
      xjjroot::setthgrstyle(h.second, t);
    }
  }

private:
  std::string _name;
  const double _maxHF = 600;
  void set_hist(TH1* h) {
    xjjroot::sethempty(h);
    h->GetXaxis()->SetNdivisions(505);
  }
  void set_hist(TH2* h) {
    xjjroot::sethempty(h);
    h->GetXaxis()->SetNdivisions(505);
    h->GetYaxis()->SetNdivisions(505);
  }
  void create_hist() {
    hhiHF = new TH1F(Form("hhiHF__%s", _name.c_str()), ";HF E_{t}^{sum} (Tower) [GeV];", 100, 0, _maxHF);
    set_hist(hhiHF);
    hhiHFpf = new TH1F(Form("hhiHFpf__%s", _name.c_str()), ";HF E_{t}^{sum} (PF) [GeV];", 100, 0, _maxHF);
    set_hist(hhiHFpf);
    hhiHFpf__sel = new TH1F(Form("hhiHFpf__sel__%s", _name.c_str()), ";HF E_{t}^{sum} (PF) [GeV];", 100, 0, _maxHF);
    set_hist(hhiHFpf__sel);
    hhiHFpf__selL1 = new TH1F(Form("hhiHFpf__selL1__%s", _name.c_str()), ";HF E_{t}^{sum} (PF) [GeV];", 100, 0, _maxHF);
    set_hist(hhiHFpf__selL1);
    for (int i=MIN_NHFTOWER; i <= MAX_NHFTOWER; i++) {
      for (int j=MIN_HFTOWERE; j <= MAX_HFTOWERE; j++) {
        auto h = new TH1F(Form("hhiHFpf__sel%dTh%d__%s", i, j, _name.c_str()), ";HF E_{t}^{sum} (PF) [GeV];", 100, 0, _maxHF);
        set_hist(h);
        hhiHFpf__selHF[Form("%dTh%d", i, j)] = h;
      }
    }

    hhiHFtowerAND = new TH1F(Form("hhiHFtowerAND__%s", _name.c_str()), ";min(E^{tower}_{lead} HF+, E^{tower}_{lead} HF-) [GeV];", 100, 0, 100);
    set_hist(hhiHFtowerAND);
    hhiHFtowerOR = new TH1F(Form("hhiHFtowerOR__%s", _name.c_str()), ";max(E^{tower}_{lead} HF+, E^{tower}_{lead} HF-) [GeV];", 100, 0, 100);
    set_hist(hhiHFtowerOR);

    hhiHF_HFpf = new TH2F(Form("hhiHF_HFpf__%s", _name.c_str()), ";HF E_{t}^{sum} (PF) [GeV];HF E_{t}^{sum} (Tower) [GeV]", 100, 0, _maxHF, 100, 0, _maxHF);
    set_hist(hhiHF_HFpf);
  }
};

float nh_width(TH1* h) {
  return h->Integral(0, h->GetNbinsX()+1, "width");
}

struct Effpurity {
  std::vector<TH1F*> vh;
  float tot;
  float eff;
  float pur;
};

TGraph* roc(std::map<std::string, Effpurity>& effpur, std::string name,
            std::vector<std::string>* tags = 0) {
  std::vector<float> eff, pur;
  for (auto& r : effpur) {
    if (tags)
      (*tags).push_back(r.first);
    eff.push_back(r.second.eff);
    pur.push_back(r.second.pur);
  }
  // std::sort (eff.begin(), eff.end());
  // std::sort (pur.begin(), pur.end());
  auto g = new TGraph(eff.size(), eff.data(), pur.data());
  g->SetName(Form("g%s", name.c_str()));
  return g;
}

void g_draw(xjjc::config& conFig, std::string title = "") {
  xjjroot::drawCMS((conFig.vi("MC")? xjjroot::CMS::simulation:xjjroot::CMS::preliminary), conFig["COLLISION"]);
  xjjroot::drawtex(0.25, 0.85, title.c_str(), 0.04, 13);
}
void g_draw_eff(float eff, float pur) {
  xjjroot::drawtex(0.4, 0.3, Form("Efficiency = %.1f%s", eff*100., "%"), 0.04);
  xjjroot::drawtex(0.4, 0.3-0.045, Form("Purity = %.1f%s", pur*100., "%"), 0.04);
}
const std::vector<Style_t> g_fstyle = { 1001, 3354, 3345, 3305, 3395 };

int macro(std::string conFig) {
  xjjc::config conf(conFig);
  std::vector<Input> inputs;
  std::cout<<std::endl;
  for (auto i : conf.vv("INPUT")) {
    auto in = create_input_from_conf(i);
    inputs.push_back(in);
  }
  auto n = inputs.size();
  std::cout<<std::endl;
  std::vector<Output> hists;
  auto leg = new TLegend(0.55, 0.86-(n+1)*0.046, 0.82, 0.86-0.046);
  xjjroot::setleg(leg, 0.04);

  for (int ih=0; ih < n; ih++) {
    auto in = inputs[ih];
    Output h(in.name);
    const auto fg = in.fg;
    
    for (int i=0; i<fg->GetEntries(); i++) {
      xjjc::progressslide(i, fg->GetEntries(), 10000);
      fg->GetEntry(i);

      h.hhiHF->Fill(fg->hiHF);
      h.hhiHFpf->Fill(fg->hiHF_pf);
      h.hhiHF_HFpf->Fill(fg->hiHF_pf, fg->hiHF);

      if (fg->pprimaryVertexFilter && fg->pclusterCompatibilityFilter) {
        h.hhiHFpf__sel->Fill(fg->hiHF_pf);
        for (int k=MIN_NHFTOWER; k <= MAX_NHFTOWER; k++) {
          for (int j=MIN_HFTOWERE; j <= MAX_HFTOWERE; j++) {
            auto key = Form("%dTh%d", k, j);
            if (fg->pphfCoincFilterPF[key]) {
              h.hhiHFpf__selHF[key]->Fill(fg->hiHF_pf);
            }
          }
        }
      }
      if (fg->mMaxL1HFAdcPlus > 12 || fg->mMaxL1HFAdcMinus > 12) {
        h.hhiHFpf__selL1->Fill(fg->hiHF_pf);
      }

      float HFmaxPlus = 0, HFmaxMinus = 0;
      for (int j=0; j<fg->nPF; j++) {
        if ((*fg->pfId)[j] != 6 && (*fg->pfId)[j] != 7) continue;
        const bool eta_plus = ((*fg->pfEta)[j] > 3.0) && ((*fg->pfEta)[j] < 6.0);
        const bool eta_minus = ((*fg->pfEta)[j] < -3.0) && ((*fg->pfEta)[j] > -6.0);
        // if (pfcand.et() < 0.0) continue;
        if (!eta_plus && !eta_minus) continue;
        if (eta_plus) {
          HFmaxPlus = std::max(HFmaxPlus, (*fg->pfE)[j]);
        }
        if (eta_minus) {
          HFmaxMinus = std::max(HFmaxMinus, (*fg->pfE)[j]);
        }
      }
      h.hhiHFtowerAND->Fill(std::min(HFmaxPlus, HFmaxMinus));
      h.hhiHFtowerOR->Fill(std::max(HFmaxPlus, HFmaxMinus));
    }
    xjjc::progressbar_summary(fg->GetEntries());

    h.hhiHF->Scale(in.xsec / fg->GetEntries(), "width");
    h.hhiHFpf->Scale(in.xsec / fg->GetEntries(), "width");
    h.hhiHFpf__sel->Scale(in.xsec / fg->GetEntries(), "width");
    h.hhiHFpf__selL1->Scale(in.xsec / fg->GetEntries(), "width");
    for (auto& h : h.hhiHFpf__selHF) {
      h.second->Scale(in.xsec / fg->GetEntries(), "width");
    }
    h.hhiHFtowerAND->Scale(in.xsec / fg->GetEntries(), "width");
    h.hhiHFtowerOR->Scale(in.xsec / fg->GetEntries(), "width");

    const xjjroot::thgrstyle t = { .lcolor = (Color_t)(ih?xjjroot::colorlist_middle[ih]:kBlack),
                                   .lstyle = (Style_t)(ih?2:1),
                                   .lwidth = 2,
                                   .fcolor = (Color_t)(ih?xjjroot::colorlist_middle[ih]:0),
                                   .fstyle = g_fstyle[ih]
    };
    h.style_hist(t);
    leg->AddEntry(h.hhiHF, Form("%s#scale[0.3]{ }#scale[0.9]{(%s b)}", plot::leg(in.name).c_str(), xjjc::number_remove_zero(in.xsec).c_str()), "f");

    hists.push_back(h);
  }

#define SETHMAX(s)                                                      \
  Effpurity r##s = { .tot = 0 };                                        \
  for (auto h : hists) {                                                \
    r##s.vh.push_back(h.h##s);                                          \
    r##s.tot += (nh_width(h.h##s));                                     \
  }                                                                     \
  xjjana::sethsmax<TH1F*>(r##s.vh, 1.2);                                \
  r##s.pur = nh_width(hists.front().h##s)*1./r##s.tot;                  \
  r##s.eff = nh_width(hists.front().h##s)*1./nh_width(hists.front().hhiHFpf); \
 
  SETHMAX(hiHF)
    SETHMAX(hiHFpf)
    SETHMAX(hiHFpf__sel)
    SETHMAX(hiHFpf__selL1)
    SETHMAX(hiHFtowerAND)
    SETHMAX(hiHFtowerOR)

    std::map<std::string, Effpurity> rhiHFpf__selHF;
  for (int i=MIN_NHFTOWER; i <= MAX_NHFTOWER; i++) {
    for (int j=MIN_HFTOWERE; j <= MAX_HFTOWERE; j++) {
      auto key = Form("%dTh%d", i, j);
      rhiHFpf__selHF[key].tot = 0;
      for (auto& h : hists) {
        rhiHFpf__selHF[key].vh.push_back(h.hhiHFpf__selHF[key]);
        rhiHFpf__selHF[key].tot += nh_width(h.hhiHFpf__selHF[key]);
      }
      xjjana::sethsmax<TH1F*>(rhiHFpf__selHF[key].vh, 1.2);
      rhiHFpf__selHF[key].pur = nh_width(hists.front().hhiHFpf__selHF[key])*1./rhiHFpf__selHF[key].tot;
      rhiHFpf__selHF[key].eff = nh_width(hists.front().hhiHFpf__selHF[key])*1./nh_width(hists.front().hhiHFpf);
    }
  }
  auto ghiHFpf__selHF = roc(rhiHFpf__selHF, "hiHFpf__selHF");
  xjjroot::setthgrstyle(ghiHFpf__selHF, kBlack, 20, 1.2, kBlack, 1, 1);
  auto hempty__roc = new TH2F("hempty__roc", ";Efficiency;Purity", 10, 0.6, 1, 10, 0.6, 1.2);
  xjjroot::sethempty(hempty__roc);
  
  xjjroot::setgstyle(1);
  xjjroot::mypdf pdf(Form("figspdf/%s.pdf", conf["TAG"].c_str()));

  pdf.prepare();
  hempty__roc->Draw("AXIS");
  xjjroot::drawline(hempty__roc->GetXaxis()->GetXmin(), 1, hempty__roc->GetXaxis()->GetXmax(), 1, kBlack, 2, 1);
  ghiHFpf__selHF->Draw("p same");
  g_draw(conf, "PV & clusComp & HFCoinc");
  pdf.write();  
  
  pdf.getc()->SetLogy();

  pdf.prepare();
  hists.front().hhiHF->Draw("AXIS");
  for (auto h : hists) {
    h.hhiHF->Draw("hist same");
  }
  leg->Draw();
  g_draw(conf, "No selection");
  g_draw_eff(rhiHF.eff, rhiHF.pur);
  pdf.write();
  
  pdf.prepare();
  hists.front().hhiHFpf->Draw("AXIS");
  for (auto h : hists) {
    h.hhiHFpf->Draw("hist same");
  }
  leg->Draw();
  g_draw(conf, "No selection");
  g_draw_eff(rhiHFpf.eff, rhiHFpf.pur);
  pdf.write();

  pdf.prepare();
  hists.front().hhiHFpf__sel->Draw("AXIS");
  for (auto h : hists) {
    h.hhiHFpf__sel->Draw("hist same");
  }
  leg->Draw();
  g_draw(conf, "PV & clusComp");
  g_draw_eff(rhiHFpf__sel.eff, rhiHFpf__sel.pur);
  pdf.write();

  for (int i=MIN_NHFTOWER; i <= MAX_NHFTOWER; i++) {
    for (int j=MIN_HFTOWERE; j <= MAX_HFTOWERE; j++) {
      auto key = Form("%dTh%d", i, j);
    
      pdf.prepare();
      hists.front().hhiHFpf__selHF[key]->Draw("AXIS");
      for (auto h : hists) {
        h.hhiHFpf__selHF[key]->Draw("hist same");
      }
      leg->Draw();
      g_draw(conf, Form("PV & clusComp & HFCoinc%s", key));
      g_draw_eff(rhiHFpf__selHF[key].eff, rhiHFpf__selHF[key].pur);
      pdf.write();
    }
  }

  pdf.prepare();
  hists.front().hhiHFpf__selL1->Draw("AXIS");
  for (auto h : hists) {
    h.hhiHFpf__selL1->Draw("hist same");
  }
  leg->Draw();
  g_draw(conf, "L1_HFOR12");
  g_draw_eff(rhiHFpf__selL1.eff, rhiHFpf__selL1.pur);
  pdf.write();

  pdf.prepare();
  hists.front().hhiHFtowerAND->Draw("AXIS");
  for (auto h : hists) {
    h.hhiHFtowerAND->Draw("hist same");
  }
  leg->Draw();
  g_draw(conf, "No selection");
  pdf.write();
  
  pdf.prepare();
  hists.front().hhiHFtowerOR->Draw("AXIS");
  for (auto h : hists) {
    h.hhiHFtowerOR->Draw("hist same");
  }
  leg->Draw();
  g_draw(conf, "No selection");
  pdf.write();
  
  pdf.getc()->SetLogy(0);

  for (auto h : hists) {
    pdf.prepare();
    h.hhiHF_HFpf->Draw("scat");
    g_draw(conf, plot::leg(h.name()));
    pdf.write();
  }

  pdf.close();

  
  
  return 0;

}

int main(int argc, char* argv[]) {
  if (argc == 2) {
    return macro(argv[1]);
  }
  return 1;
}
