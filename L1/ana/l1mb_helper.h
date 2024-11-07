
std::string tag_, subtag_;

TGraph* roc(TH1F* hx, TH1F* hy, std::string name="")
{
  int n = hx->GetXaxis()->GetNbins();
  std::vector<float> x(n), y(n);
  for(int i=0; i<n; i++)
    {
      x[i] = hx->GetBinContent(i+1);
      y[i] = hy->GetBinContent(i+1);
    }
  TGraph* gr = new TGraph(n, x.data(), y.data());
  gr->SetName(name.c_str());
  return gr;
}

template<class T>
void drawshadow(T* hempty, Color_t cc = kGray, float guide = 15)
{
  hempty->Draw("AXIS");
  xjjroot::drawCMS(xjjroot::CMS::internal, tag_.c_str());
  xjjroot::drawtex(0, 0.01, subtag_.c_str(), 0.035, 11, 52, kGray+1);
  if(cc > 0)
    {
      float prescls[] = {1, 1.5};
      for(auto& pp : prescls)
        {
          xjjroot::drawline(pp*guide, hempty->GetYaxis()->GetXmin(), pp*guide, hempty->GetYaxis()->GetXmax()<2?1:hempty->GetYaxis()->GetXmax(), cc, 5, 2);
          xjjroot::drawtexnum(pp*guide-0.5, hempty->GetYaxis()->GetXmin() + (hempty->GetYaxis()->GetXmax()-hempty->GetYaxis()->GetXmin())/20.,
                              Form("Prescl = %s", xjjc::number_remove_zero(pp).c_str()), 0.032, 32, 62, cc);
        }
    }
}

void drawgrval(TGraph* gr, TH2* hempty, bool val = true)
{
  gr->Draw("plY0 same");
  if(val)
    for(int i=0; i<gr->GetN(); i++)
      {
        double x = gr->GetPointX(i), y = gr->GetPointY(i);
        if((x>16 && x<32))
          xjjroot::drawtexnum(x, y+(hempty->GetYaxis()->GetXmax()-hempty->GetYaxis()->GetXmin())/20., Form("%.2f", y), 0.03, 23, 42, gr->GetMarkerColor());
      }
}

void drawhfth(TGraph* gr, TH2* hempty)
{
  for(int i=0; i<gr->GetN(); i++)
    {
      double x = gr->GetPointX(i), y = gr->GetPointY(i);
      if (x >= hempty->GetXaxis()->GetXmin() && x <= hempty->GetXaxis()->GetXmax())
        xjjroot::drawtexnum(x, y+(hempty->GetYaxis()->GetXmax()-hempty->GetYaxis()->GetXmin())/20., Form("%d", i), 0.03, 23, 42, gr->GetMarkerColor());
    }
}

int nearest(TH1F* h, float frate)
{
  int iresult = -1;
  float dev = 1.e+10;
  for(int i=0; i<h->GetXaxis()->GetNbins(); i++) {
    float delta = h->GetBinContent(i+1) - frate;
    // if (delta > 0) continue;
    if (fabs(delta) > dev) {
      if (delta > 0)
        iresult = iresult - 1;
      break;
    }
    dev = delta;
    iresult = i;
  }
  std::cout<<__FUNCTION__<<": \e[1m"<<frate<<"kHz\e[0m: \e[1m("<<h->GetName()<<")\e[0m => \e[1m"<<Form("%.2f kHz", h->GetBinContent(iresult+1))<<" (ADC > "<<iresult<<")\e[0m"<<std::endl;
  return iresult;
}

float hist_to_value(TFile* inf, std::string histname) {
  auto h = (TH1F*)inf->Get(histname.c_str());
  float value = h->GetBinContent(1);
  return value;
}

int printrate(std::vector<TH1F*> vh)
{
  xjjc::prt_divider();
  std::cout<<"HF thre, ";
  for (auto& h : vh) {
    std::cout<<std::setw(9)<<Form("%s,", h->GetName());
  }
  std::cout<<std::endl;
  for(int i=0; i<vh[0]->GetXaxis()->GetNbins(); i++) {
    std::cout<<std::left<<std::setw(9)<<Form("%d,", i);
    for (auto& h : vh) {
      std::cout<<std::setw(9)<<Form("%f,", h->GetBinContent(i+1));
    }
    std::cout<<std::endl;
  }
  return 0;
}
