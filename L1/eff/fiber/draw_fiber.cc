#include <TFile.h>
#include <TH2F.h>
#include <TCanvas.h>
#include <string>

#include "xjjrootuti.h"
#include "xjjcuti.h"

void draw_fiber(std::string inputname, std::string tag)
{
  std::string tagname = xjjc::str_replaceallspecial(tag);
  TFile* inf = TFile::Open(inputname.c_str());
  TH2F* hshortvslong = (TH2F*)inf->Get("hshortvslong");
  xjjroot::sethempty(hshortvslong);

  xjjroot::setgstyle(1);
  TCanvas* c = new TCanvas("c", "", 600, 600);
  hshortvslong->Draw();
  xjjroot::drawCMSleft();
  xjjroot::drawCMSright(tag.c_str());
  std::string outputname = "plots/"+tagname+"/chshortvslong.pdf";
  xjjroot::mkdir(outputname);
  c->SaveAs(outputname.c_str());
}

int main(int argc, char* argv[])
{
  if(argc==3) { draw_fiber(argv[1], argv[2]); return 0; }
  return 1;
}
