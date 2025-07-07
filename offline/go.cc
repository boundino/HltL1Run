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
  std::map<std::string, TH1F*> hhiHF;
  // TH1F *hhiHF, *hhiHFpf, *hhiHFpf__sel, *hhiHFpf__selL1;
  // std::map<std::string, TH1F*> hhiHFpf__selHF;
  TH2F *hhiHF_HFpf;
  std::map<std::string, TH1F*> hhiHFtower;
  // TH1F *hhiHFtowerAND, *hhiHFtowerOR;
  
  void style_hist(const xjjroot::thgrstyle& t) {
    for (auto& h : hhiHF) {
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
  std::vector<std::string> _sel = {"", "pf", "pf__sel", "pf__selL1" };
  void create_hist() {
    for (auto& t : _sel) {
      hhiHF[t] = new TH1F(Form("hiHF%s__%s", t.c_str(), _name.c_str()), ";HF E_{t}^{sum} (Tower) [GeV];", 100, 0, _maxHF);
      set_hist(hhiHF[t]);
    }
    for (int i=MIN_NHFTOWER; i <= MAX_NHFTOWER; i++) {
      for (int j=MIN_HFTOWERE; j <= MAX_HFTOWERE; j++) {
        auto str_hf = Form("pf__selHF%dTh%d", i, j);
        hhiHF[str_hf] = new TH1F(Form("hiHF%s__%s", str_hf, _name.c_str()), ";HF E_{t}^{sum} (PF) [GeV];", 100, 0, _maxHF);
        set_hist(hhiHF[str_hf]);
      }
    }

    hhiHFtower["Plus"] = new TH1F(Form("hhiHFtowerPlus__%s", _name.c_str()), ";E^{tower}_{lead} HF+ [GeV];", 100, 0, 100);
    set_hist(hhiHFtower["Plus"]);
    hhiHFtower["Minus"] = new TH1F(Form("hhiHFtowerMinus__%s", _name.c_str()), ";E^{tower}_{lead} HF- [GeV];", 100, 0, 100);
    set_hist(hhiHFtower["Minus"]);
    hhiHFtower["AND"] = new TH1F(Form("hhiHFtowerAND__%s", _name.c_str()), ";min(E^{tower}_{lead} HF+, E^{tower}_{lead} HF-) [GeV];", 100, 0, 100);
    set_hist(hhiHFtower["AND"]);
    hhiHFtower["OR"] = new TH1F(Form("hhiHFtowerOR__%s", _name.c_str()), ";max(E^{tower}_{lead} HF+, E^{tower}_{lead} HF-) [GeV];", 100, 0, 100);
    set_hist(hhiHFtower["OR"]);

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
void g_draw_eff(Effpurity& r) {
  g_draw_eff(r.eff, r.pur);
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
  std::vector<Output> outputs;
  auto leg = new TLegend(0.54, 0.86-(n+1)*0.046, 0.80, 0.86-0.046);
  xjjroot::setleg(leg, 0.04);

  for (int ih=0; ih < n; ih++) {
    auto in = inputs[ih];
    Output o(in.name);
    const auto fg = in.fg;

    auto nentries = fg->GetEntries();
    for (int i=0; i<nentries; i++) {
      xjjc::progressslide(i, fg->GetEntries(), 1000);
      fg->GetEntry(i);

      continue;
      
      o.hhiHF[""]->Fill(fg->hiHF);
      o.hhiHF["pf"]->Fill(fg->hiHF_pf);
      o.hhiHF_HFpf->Fill(fg->hiHF_pf, fg->hiHF);

      if (fg->pprimaryVertexFilter && fg->pclusterCompatibilityFilter
          && fabs(fg->vz) < 15) {
        o.hhiHF["pf__sel"]->Fill(fg->hiHF_pf);
        for (int k=MIN_NHFTOWER; k <= MAX_NHFTOWER; k++) {
          for (int j=MIN_HFTOWERE; j <= MAX_HFTOWERE; j++) {
            auto key = Form("%dTh%d", k, j);
            if (fg->pphfCoincFilterPF[key]) {
              o.hhiHF[Form("pf__selHF%s", key)]->Fill(fg->hiHF_pf);
            }
          }
        }
      }
      if (fg->mMaxL1HFAdcPlus > 12 || fg->mMaxL1HFAdcMinus > 12) {
        o.hhiHF["pf__selL1"]->Fill(fg->hiHF_pf);
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
      o.hhiHFtower["Plus"]->Fill(HFmaxPlus);
      o.hhiHFtower["Minus"]->Fill(HFmaxMinus);
      o.hhiHFtower["AND"]->Fill(std::min(HFmaxPlus, HFmaxMinus));
      o.hhiHFtower["OR"]->Fill(std::max(HFmaxPlus, HFmaxMinus));
    }
    xjjc::progressbar_summary(fg->GetEntries());

    for (auto& h : o.hhiHF) {
      h.second->Scale(in.xsec / fg->GetEntries(), "width");
    }
    for (auto& h : o.hhiHFtower) {
      h.second->Scale(in.xsec / fg->GetEntries(), "width");
    }

    const xjjroot::thgrstyle t = { .lcolor = (Color_t)(ih?xjjroot::colorlist_middle[ih]:kBlack),
                                   .lstyle = (Style_t)(ih?2:1),
                                   .lwidth = 2,
                                   .fcolor = (Color_t)(ih?xjjroot::colorlist_middle[ih]:0),
                                   .fstyle = g_fstyle[ih]
    };
    o.style_hist(t);
    leg->AddEntry(o.hhiHF[""], Form("%s#scale[0.3]{ }#scale[0.9]{(%s b)}", plot::leg(in.name).c_str(), xjjc::number_remove_zero(in.xsec).c_str()), "f");

    outputs.push_back(o);
  }

  // #define SETHMAX(s)                                                      \
  //   Effpurity r##s = { .tot = 0 };                                        \
  //   for (const auto& o : outputs) {                                                \
  //     r##s.vh.push_back(h.h##s);                                          \
  //     r##s.tot += (nh_width(h.h##s));                                     \
  //   }                                                                     \
  //   xjjana::sethsmax<TH1F*>(r##s.vh, 1.2);                                \
  //   r##s.pur = nh_width(outputs.front().h##s)*1./r##s.tot;                  \
  //   r##s.eff = nh_width(outputs.front().h##s)*1./nh_width(outputs.front().hhiHF["pf"]); \
 
  //   SETHMAX(hiHF)
  //     SETHMAX(hiHFpf)
  //     SETHMAX(hiHFpf__sel)
  //     SETHMAX(hiHFpf__selL1)
  //     SETHMAX(hiHFtower["AND"])
  //     SETHMAX(hiHFtower["OR"])

  std::map<std::string, Effpurity> rhiHF;
  for (const auto &[key, value] : outputs.front().hhiHF) {
    rhiHF[key].tot = 0;
    for (auto& o : outputs) {
      rhiHF[key].vh.push_back(o.hhiHF[key]);
      rhiHF[key].tot += nh_width(o.hhiHF[key]);
    }
    xjjana::sethsmax<TH1F*>(rhiHF[key].vh, 1.2);
    rhiHF[key].pur = nh_width(outputs.front().hhiHF[key])*1./rhiHF[key].tot;
    rhiHF[key].eff = nh_width(outputs.front().hhiHF[key])*1./nh_width(outputs.front().hhiHF["pf"]);
  }
  
  auto ghiHF = roc(rhiHF, "hiHF");
  xjjroot::setthgrstyle(ghiHF, kBlack, 20, 1.2, kBlack, 1, 1);
  auto hempty__roc = new TH2F("hempty__roc", ";Efficiency;Purity", 10, 0.6, 1, 10, 0.8, 1.05);
  xjjroot::sethempty(hempty__roc);
  
  xjjroot::setgstyle(1);
  xjjroot::mypdf pdf(Form("figspdf/%s.pdf", conf["TAG"].c_str()));

  pdf.prepare();
  hempty__roc->Draw("AXIS");
  xjjroot::drawline(hempty__roc->GetXaxis()->GetXmin(), 1, hempty__roc->GetXaxis()->GetXmax(), 1, kBlack, 2, 1);
  ghiHF->Draw("p same");
  float xx = 0.3, yy = 0.5;
  for(auto& in : inputs) {
    xjjroot::drawtex(xx, yy-=0.04, Form("%s#scale[0.3]{ }#scale[0.9]{(%s b)}", plot::leg(in.name).c_str(), xjjc::number_remove_zero(in.xsec).c_str()), 0.038);
  }
  g_draw(conf, "|v_{z}| < 15 & clusComp & HFCoinc");
  pdf.write();  
  
  pdf.getc()->SetLogy();

  pdf.prepare();
  outputs.front().hhiHF["pf"]->Draw("AXIS");
  for (auto& o : outputs) {
    o.hhiHF["pf"]->Draw("hist same");
  }
  leg->Draw();
  g_draw(conf, "No selection");
  g_draw_eff(rhiHF["pf"]);
  pdf.write();

  pdf.prepare();
  outputs.front().hhiHF["pf__sel"]->Draw("AXIS");
  for (auto& o : outputs) {
    o.hhiHF["pf__sel"]->Draw("hist same");
  }
  leg->Draw();
  g_draw(conf, "|v_{z}| < 15 & clusComp");
  g_draw_eff(rhiHF["pf__sel"]);
  pdf.write();

  for (int i=MIN_NHFTOWER; i <= MAX_NHFTOWER; i++) {
    for (int j=MIN_HFTOWERE; j <= MAX_HFTOWERE; j++) {
      auto tag = Form("%dTh%d", i, j), key = Form("pf__selHF%s", tag);
      pdf.prepare();
      outputs.front().hhiHF[key]->Draw("AXIS");
      for (auto& o : outputs) {
        o.hhiHF[key]->Draw("hist same");
      }
      leg->Draw();
      g_draw(conf, Form("|v_{z}| < 15 & clusComp & HFCoinc%s", tag));
      g_draw_eff(rhiHF[key]);
      pdf.write();
    }
  }

  pdf.prepare();
  outputs.front().hhiHF["pf__selL1"]->Draw("AXIS");
  for (auto& o : outputs) {
    o.hhiHF["pf__selL1"]->Draw("hist same");
  }
  leg->Draw();
  g_draw(conf, "L1_HFOR12");
  g_draw_eff(rhiHF["pf__selL1"]);
  pdf.write();

  pdf.prepare();
  outputs.front().hhiHFtower["AND"]->Draw("AXIS");
  for (auto& o : outputs) {
    o.hhiHFtower["AND"]->Draw("hist same");
  }
  leg->Draw();
  g_draw(conf, "No selection");
  pdf.write();
  
  pdf.prepare();
  outputs.front().hhiHFtower["OR"]->Draw("AXIS");
  for (auto& o : outputs) {
    o.hhiHFtower["OR"]->Draw("hist same");
  }
  leg->Draw();
  g_draw(conf, "No selection");
  pdf.write();
  
  pdf.prepare();
  outputs.front().hhiHF[""]->Draw("AXIS");
  for (auto& o : outputs) {
    o.hhiHF[""]->Draw("hist same");
  }
  leg->Draw();
  g_draw(conf, "No selection");
  g_draw_eff(rhiHF[""]);
  pdf.write();
  
  pdf.getc()->SetLogy(0);

  for (auto& o : outputs) {
    pdf.prepare();
    o.hhiHF_HFpf->Draw("scat");
    g_draw(conf, plot::leg(o.name()));
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
