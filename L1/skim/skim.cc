#include <TFile.h>
#include <TTree.h>
#include <TChain.h>

#include <fstream>
#include <iostream>
#include <iomanip>
#include <vector>
#include <string>

#include "xjjrootuti.h"
#include "xjjcuti.h"
#include "adctree.h"

const int adc_bin_min = 0, adc_bin_max = 40, adc_nbin = adc_bin_max-adc_bin_min;
const int NBIN_IETA = 101, NBIN_IPHI = 80; 

void skim(std::string inputname, std::string outputname)
{
  // TTree::SetMaxTreeSize(1LL * 1024 * 1024 * 1024 * 1024);

  TFile* inf = TFile::Open(inputname.c_str());
  TTree* l1evt = (TTree*)inf->Get("l1EventTree/L1EventTree");
  TTree* adc = (TTree*)inf->Get("HFAdcana/adc");
  dadctree::adctree* idtree = new dadctree::adctree(adc);

  TFile* outf = new TFile(outputname.c_str(), "recreate");
  TDirectory* dl1evt = outf->mkdir("l1EventTree","");
  TDirectory* dadc = outf->mkdir("HFAdcana","");
  dl1evt->cd();
  TTree* new_l1evt = l1evt->CloneTree(0);
  dadc->cd();
  TTree* new_adc = new TTree("adc", "");
  int amplmax_plus; new_adc->Branch("amplmax_plus", &amplmax_plus);
  int amplmax_minus; new_adc->Branch("amplmax_minus", &amplmax_minus);
  int longmax_plus; new_adc->Branch("longmax_plus", &longmax_plus);
  int longmax_minus; new_adc->Branch("longmax_minus", &longmax_minus);
  int shortmax_plus; new_adc->Branch("shortmax_plus", &shortmax_plus);
  int shortmax_minus; new_adc->Branch("shortmax_minus", &shortmax_minus);
  outf->cd();
  TH2F* hshortvslong = new TH2F("hshortvslong", ";HF ADC (long);HF ADC (short)", adc_nbin, adc_bin_min, adc_bin_max, adc_nbin, adc_bin_min, adc_bin_max);

  Long64_t nentries = l1evt->GetEntries();
  std::cout<<" -- Event reading"<<std::endl;
  Long64_t j=0;
  for(Long64_t i=0;i<nentries;i++)
    {
      if(i%1000==0) { xjjc::progressbar(i, nentries); }

      l1evt->GetEntry(i);
      adc->GetEntry(i);

      dl1evt->cd();
      new_l1evt->Fill();

      std::vector<int> ampleta(NBIN_IETA*100 + NBIN_IPHI, 0);
      std::vector<int> ampllong(NBIN_IETA*100 + NBIN_IPHI, 0);
      std::vector<int> amplshort(NBIN_IETA*100 + NBIN_IPHI, 0);
      amplmax_plus = 0;
      amplmax_minus = 0;
      for(int j=0; j<idtree->ampl->size(); j++)
        {
          // std::cout<<(*idtree->ieta)[j]<<" "<<(*idtree->iphi)[j]<<" "<<(*idtree->ampl)[j]<<std::endl;
          if(abs((*idtree->ieta)[j]) < 29) { std::cout<<"warning: hAdcVsGeV saves units not HF: ieta = "<<(*idtree->ieta)[j]<<std::endl; continue; }

          if((*idtree->ieta)[j] > 0 && (*idtree->ampl)[j] > amplmax_plus) { amplmax_plus = (*idtree->ampl)[j]; }
          if((*idtree->ieta)[j] < 0 && (*idtree->ampl)[j] > amplmax_minus) { amplmax_minus = (*idtree->ampl)[j]; }

          int index = ((*idtree->ieta)[j]+50) * 100 + (*idtree->iphi)[j];
          ampleta[index] = (*idtree->ieta)[j];
          if((*idtree->depth)[j]==1 || (*idtree->depth)[j]==3) { ampllong[index] += (*idtree->ampl)[j]; }
          if((*idtree->depth)[j]==2 || (*idtree->depth)[j]==4) { amplshort[index] += (*idtree->ampl)[j]; }
        }
      longmax_plus = 0;
      longmax_minus = 0;
      shortmax_plus = 0;
      shortmax_minus = 0;
      for(int index=0; index<NBIN_IETA*100 + NBIN_IPHI; index++)
        {
          if(ampleta[index] != 0) hshortvslong->Fill(ampllong[index], amplshort[index]);
          else continue;
          if(ampleta[index] > 0 && ampllong[index] > longmax_plus) { longmax_plus = ampllong[index]; }
          if(ampleta[index] < 0 && ampllong[index] > longmax_minus) { longmax_minus = ampllong[index]; }
          if(ampleta[index] > 0 && amplshort[index] > shortmax_plus) { shortmax_plus = amplshort[index]; }
          if(ampleta[index] < 0 && amplshort[index] > shortmax_minus) { shortmax_minus = amplshort[index]; }
        }
      dadc->cd();
      new_adc->Fill();

    }

  xjjc::progressbar_summary(nentries);
      
  outf->Write();
  std::cout<<" -- Writing new trees done"<<std::endl;
  // hshortvslong->Write();
  outf->Close();
      
}

int main(int argc, char* argv[])
{
  if(argc==3) { skim(argv[1], argv[2]); return 0; }
  return 1;
}


// if((*idtree->ampl)[j] < 5) continue;
          
// odtree->ampl->push_back((*idtree->ampl)[j]);
// odtree->ieta->push_back((*idtree->ieta)[j]);
// odtree->iphi->push_back((*idtree->iphi)[j]);
// odtree->depth->push_back((*idtree->depth)[j]);
// odtree->charge->push_back((*idtree->charge)[j]);
// odtree->charge_ped->push_back((*idtree->charge_ped)[j]);
// odtree->energy->push_back((*idtree->energy)[j]);
// odtree->energy_ped->push_back((*idtree->energy_ped)[j]);

// odtree->nampl++;
