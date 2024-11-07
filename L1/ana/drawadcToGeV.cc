#include <vector>

#include <TFile.h>
#include <TH2F.h>
#include <TString.h>
#include <TCanvas.h>
#include <TProfile.h>
#include <TLegend.h>
#include <TF1.h>

#include "xjjrootuti.h"
#include "config.h"
#include "xjjmypdf.h"

class ffleg
{    
public:
  TF1* f1;
  TF1* f2;
  std::string leg;
  ffleg(std::string strf1, std::string strf2, std::string strleg) : leg(strleg)
  {
    f1 = new TF1("f1", strf1.c_str(), 0, 30);
    f2 = new TF1("f2", strf2.c_str(), 0, 30);
    xjjroot::setlinestyle(f1, kAzure-3, 2, 3);
    xjjroot::setlinestyle(f2, kAzure-3, 2, 3);
  }
  ~ffleg();
  void drawf12() { f1->Draw("same"); f2->Draw("same"); }
};

void drawadcToGeV(std::string param)
{
  xjjc::config conf(param);
  conf.print();
  std::string outputdir = conf["Output"];
  auto tag = conf["Tag"], subtag = conf["SubTag"];

  TFile* inf = TFile::Open(Form("rootfiles/%s/adcToGeV.root", outputdir.c_str()));
  std::vector<TH2F*> vhist;
  TH2F* hcorr_ADC_Eped = (TH2F*)inf->Get("hcorr_ADC_Eped");
  vhist.push_back(hcorr_ADC_Eped);
  TH2F* hcorr_ADC_Epedplus = (TH2F*)inf->Get("hcorr_ADC_Epedplus");
  vhist.push_back(hcorr_ADC_Epedplus);
  TH2F* hcorr_ADC_Epedminus = (TH2F*)inf->Get("hcorr_ADC_Epedminus");
  vhist.push_back(hcorr_ADC_Epedminus);
  TH2F* hcorr_ADC_E = (TH2F*)inf->Get("hcorr_ADC_E");
  vhist.push_back(hcorr_ADC_E);
  TH2F* hcorr_Eped_E = (TH2F*)inf->Get("hcorr_Eped_E");
  vhist.push_back(hcorr_Eped_E);
  TH2F* hcorr_fCped_Eped = (TH2F*)inf->Get("hcorr_fCped_Eped");
  vhist.push_back(hcorr_fCped_Eped);
  TH2F* hcorr_fC_E = (TH2F*)inf->Get("hcorr_fC_E");
  vhist.push_back(hcorr_fC_E);
  // TH2F* hcorrtower_ADC_Eoffline = (TH2F*)inf->Get("hcorrtower_ADC_Eoffline");
  // vhist.push_back(hcorrtower_ADC_Eoffline);
  // TH2F* hcorrtower_E_Eoffline = (TH2F*)inf->Get("hcorrtower_E_Eoffline");
  // vhist.push_back(hcorrtower_E_Eoffline);
  // TH2F* hcorrtower_ADCsum_Eoffline = (TH2F*)inf->Get("hcorrtower_ADCsum_Eoffline");
  // vhist.push_back(hcorrtower_ADCsum_Eoffline);
  // TH2F* hcorrtower_Esum_Eoffline = (TH2F*)inf->Get("hcorrtower_Esum_Eoffline");
  // vhist.push_back(hcorrtower_Esum_Eoffline);
  
  ffleg* f = new ffleg("(x-4)*0.5", "(x-4)*0.8", "0.5-0.8 GeV/ADC (4 ADC shift)");
  ffleg* f_ped = new ffleg("x*0.5", "x*0.8", "0.5-0.8 GeV/ADC");

  xjjroot::setgstyle(1);
  gStyle->SetLineWidth(2);
  xjjroot::mypdf* pdf = new xjjroot::mypdf("figspdf/" + outputdir + "/adcToGeV.pdf", "c", 700, 600);

  for ( auto& i : vhist )
    {
      int nline = TString(i->GetName()).Contains("ADC")?2:1;
      ffleg* ff = TString(i->GetName()).Contains("ped")?f_ped:f;
      xjjroot::sethempty(i, 0., 0.);
      TProfile* p = i->ProfileX("_pfx", 0, i->GetNbinsY()+1);
      p->SetLineWidth(3);
      p->SetLineColor(kRed+1);

      pdf->prepare();
      // c->SetLogz();
      i->Draw("scat=0.005");
      p->Draw("le same");
      if(nline==2) 
        { 
          ff->drawf12();
          // xjjroot::drawline(15, 0, 15, 20, kOrange+6, 3, 3);
          // xjjroot::drawline(12, 0, 12, 20, kOrange+6, 3, 3);
          // xjjroot::drawline(17, 0, 17, 20, kOrange+6, 3, 3);
          // xjjroot::drawline(0, p->GetBinContent(15+1), 15,  p->GetBinContent(15+1), kOrange+6, 3, 3);
        }
      xjjroot::drawCMS(xjjroot::CMS::internal, tag.c_str());
      xjjroot::drawtex(0, 0.01, subtag.c_str(), 0.035, 11, 52, kGray+1);
      TLegend* leg = new TLegend(0.21, 0.79-0.045*nline, 0.55, 0.79);
      xjjroot::setleg(leg);
      leg->AddEntry(p, "ProfileX", "l");
      if(nline==2) { leg->AddEntry(ff->f1, ff->leg.c_str(), "l"); }
      leg->Draw();

      pdf->write();
      delete leg;
      delete p;
    }
  pdf->close();
}

int main(int argc, char* argv[])
{
  if(argc==2) { drawadcToGeV(argv[1]); return 0; }
  return 1;
}
