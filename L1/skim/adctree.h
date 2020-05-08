#include "xjjrootuti.h"
#include "xjjcuti.h"
#include <TTree.h>
#include <TFile.h>
#include <string>
#include <vector>
#include <TH2D.h>

namespace dadctree
{
  class adctree
  {
  public:
    adctree(TTree* nt) : fnt(nt) { setup(); setbranchesaddress(); }
    adctree(TFile* inf) { build(); }
    // branches
    int nampl;
    std::vector<int>* ampl;
    std::vector<int>* ieta;
    std::vector<int>* iphi;
    std::vector<int>* depth;
    std::vector<double>* charge;
    std::vector<double>* charge_ped;
    std::vector<double>* energy;
    std::vector<double>* energy_ped;

    TTree* nt() { return fnt; }
    void clear();

  private:
    TTree* fnt;
    void setbranchesaddress();
    void setup();
    void build();
  };
}

void dadctree::adctree::setup()
{
  nampl = 0;
  ampl = 0;
  ieta = 0;
  iphi = 0;
  depth = 0;
  charge = 0;
  charge_ped = 0;
  energy = 0;
  energy_ped = 0;
}

void dadctree::adctree::clear()
{
  nampl = 0;
  ampl->clear();
  ieta->clear();
  iphi->clear();
  depth->clear();
  charge->clear();
  charge_ped->clear();
  energy->clear();
  energy_ped->clear();
}

void dadctree::adctree::setbranchesaddress()
{
  fnt->SetBranchAddress("nampl", &nampl);
  fnt->SetBranchAddress("ampl", &ampl);
  fnt->SetBranchAddress("ieta", &ieta);
  fnt->SetBranchAddress("iphi", &iphi);
  fnt->SetBranchAddress("depth", &depth);
  fnt->SetBranchAddress("charge", &charge);
  fnt->SetBranchAddress("charge_ped", &charge_ped);
  fnt->SetBranchAddress("energy", &energy);
  fnt->SetBranchAddress("energy_ped", &energy_ped);
}

void dadctree::adctree::build()
{
  fnt = new TTree("adc", "");

  ampl = new std::vector<int>();
  ieta = new std::vector<int>();
  iphi = new std::vector<int>();
  depth = new std::vector<int>();
  charge = new std::vector<double>();
  charge_ped = new std::vector<double>();
  energy = new std::vector<double>();
  energy_ped = new std::vector<double>();

  fnt->Branch("nampl", &nampl);
  fnt->Branch("ampl", ampl);
  fnt->Branch("ieta", ieta);
  fnt->Branch("iphi", iphi);
  fnt->Branch("depth", depth);
  fnt->Branch("charge", charge);
  fnt->Branch("charge_ped", charge_ped);
  fnt->Branch("energy", energy);
  fnt->Branch("energy_ped", energy_ped);
}

