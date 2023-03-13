//////////////////////////////////////////////////////////
// This class has been automatically generated on
// Thu Mar  9 14:06:43 2023 by ROOT version 6.14/09
// from TTree ReducedBase/ReducedBase
// found on file: ntuples/llpgana_mc_MINIAODSIM_test_JetHT_2018A.root
//////////////////////////////////////////////////////////

#ifndef ReducedBase_h
#define ReducedBase_h

#include <TROOT.h>
#include <TChain.h>
#include <TFile.h>

// Header file for the classes stored in the TTree if any.
#include "vector"

using namespace std;

class ReducedBase {
public :
   TTree          *fChain;   //!pointer to the analyzed TTree or TChain
   Int_t           fCurrent; //!current Tree number in a TChain

// Fixed size dimensions of array or collections stored in the TTree if any.

   // Declaration of leaf types
   UInt_t          run;
   UInt_t          lumi;
   ULong64_t       event;
   Float_t         vtxX;
   Float_t         vtxY;
   Float_t         vtxZ;
   UInt_t          nJets;
   vector<float>   *jetE;
   vector<float>   *jetPt;
   vector<float>   *jetEta;
   vector<float>   *jetPhi;
   vector<int>     *jetID;
   vector<unsigned int> *jetRecHitId;
   vector<int>  *jetRecHitOfJet;
   Int_t           nRecHits;
   vector<float>   *rhPosX;
   vector<float>   *rhPosY;
   vector<float>   *rhPosZ;
   vector<float>   *rhPosEta;
   vector<float>   *rhPosPhi;
   vector<float>   *rhEnergy;
   vector<float>   *rhTime;
   vector<float>   *rhTimeErr;
   vector<unsigned int> *rhID;

   // List of branches
   TBranch        *b_run;   //!
   TBranch        *b_lumi;   //!
   TBranch        *b_event;   //!
   TBranch        *b_vtxX;   //!
   TBranch        *b_vtxY;   //!
   TBranch        *b_vtxZ;   //!
   TBranch        *b_nJets;   //!
   TBranch        *b_jetE;   //!
   TBranch        *b_jetPt;   //!
   TBranch        *b_jetEta;   //!
   TBranch        *b_jetPhi;   //!
   TBranch        *b_jetID;   //!
   TBranch        *b_jetRecHitId;   //!
   TBranch        *b_jetRecHitOfJet;   //!
   TBranch        *b_nRecHits;   //!
   TBranch        *b_rhPosX;   //!
   TBranch        *b_rhPosY;   //!
   TBranch        *b_rhPosZ;   //!
   TBranch        *b_rhPosEta;   //!
   TBranch        *b_rhPosPhi;   //!
   TBranch        *b_rhEnergy;   //!
   TBranch        *b_rhTime;   //!
   TBranch        *b_rhTimeErr;   //!
   TBranch        *b_rhID;   //!

   ReducedBase(TTree *tree=0);
   virtual ~ReducedBase();
   virtual Int_t    Cut(Long64_t entry);
   virtual Int_t    GetEntry(Long64_t entry);
   virtual Long64_t LoadTree(Long64_t entry);
   virtual void     Init(TTree *tree);
//   virtual void     Loop();
   virtual Bool_t   Notify();
   virtual void     Show(Long64_t entry = -1);
};

#endif

inline ReducedBase::ReducedBase(TTree *tree) : fChain(0) 
{
// if parameter tree is not specified (or zero), connect the file
// used to generate this class and read the Tree.
   if (tree == 0) {
      TFile *f = (TFile*)gROOT->GetListOfFiles()->FindObject("ntuples/llpgana_mc_MINIAODSIM_test_JetHT_2018A.root");
      if (!f || !f->IsOpen()) {
         f = new TFile("ntuples/llpgana_mc_MINIAODSIM_test_JetHT_2018A.root");
      }
      TDirectory * dir = (TDirectory*)f->Get("ntuples/llpgana_mc_MINIAODSIM_test_JetHT_2018A.root:/tree");
      dir->GetObject("ReducedBase",tree);

   }
   Init(tree);
}

inline ReducedBase::~ReducedBase()
{
   if (!fChain) return;
   delete fChain->GetCurrentFile();
}

inline Int_t ReducedBase::GetEntry(Long64_t entry)
{
// Read contents of entry.
   if (!fChain) return 0;
   return fChain->GetEntry(entry);
}

inline Long64_t ReducedBase::LoadTree(Long64_t entry)
{
// Set the environment to read one entry
   if (!fChain) return -5;
   Long64_t centry = fChain->LoadTree(entry);
   if (centry < 0) return centry;
   if (fChain->GetTreeNumber() != fCurrent) {
      fCurrent = fChain->GetTreeNumber();
      Notify();
   }
   return centry;
}

inline void ReducedBase::Init(TTree *tree)
{
   // The Init() function is called when the selector needs to initialize
   // a new tree or chain. Typically here the branch addresses and branch
   // pointers of the tree will be set.
   // It is normally not necessary to make changes to the generated
   // code, but the routine can be extended by the user if needed.
   // Init() will be called many times when running on PROOF
   // (once per file to be processed).

   // Set object pointer
   jetE = 0;
   jetPt = 0;
   jetEta = 0;
   jetPhi = 0;
   jetID = 0;
   jetRecHitId = 0;
   jetRecHitOfJet = 0;
   rhPosX = 0;
   rhPosY = 0;
   rhPosZ = 0;
   rhPosEta = 0;
   rhPosPhi = 0;
   rhEnergy = 0;
   rhTime = 0;
   rhTimeErr = 0;
   // Set branch addresses and branch pointers
   if (!tree) return;
   fChain = tree;
   fCurrent = -1;
   fChain->SetMakeClass(1);

   fChain->SetBranchAddress("run", &run, &b_run);
   fChain->SetBranchAddress("lumi", &lumi, &b_lumi);
   fChain->SetBranchAddress("event", &event, &b_event);
   fChain->SetBranchAddress("vtxX", &vtxX, &b_vtxX);
   fChain->SetBranchAddress("vtxY", &vtxY, &b_vtxY);
   fChain->SetBranchAddress("vtxZ", &vtxZ, &b_vtxZ);
   fChain->SetBranchAddress("nJets", &nJets, &b_nJets);
   fChain->SetBranchAddress("jetE", &jetE, &b_jetE);
   fChain->SetBranchAddress("jetPt", &jetPt, &b_jetPt);
   fChain->SetBranchAddress("jetEta", &jetEta, &b_jetEta);
   fChain->SetBranchAddress("jetPhi", &jetPhi, &b_jetPhi);
   fChain->SetBranchAddress("jetID", &jetID, &b_jetID);
   fChain->SetBranchAddress("jetRecHitId", &jetRecHitId, &b_jetRecHitId);
   fChain->SetBranchAddress("jetRecHitOfJet", &jetRecHitOfJet, &b_jetRecHitOfJet);
   fChain->SetBranchAddress("nRecHits", &nRecHits, &b_nRecHits);
   fChain->SetBranchAddress("rhPosX", &rhPosX, &b_rhPosX);
   fChain->SetBranchAddress("rhPosY", &rhPosY, &b_rhPosY);
   fChain->SetBranchAddress("rhPosZ", &rhPosZ, &b_rhPosZ);
   fChain->SetBranchAddress("rhPosEta", &rhPosEta, &b_rhPosEta);
   fChain->SetBranchAddress("rhPosPhi", &rhPosPhi, &b_rhPosPhi);
   fChain->SetBranchAddress("rhEnergy", &rhEnergy, &b_rhEnergy);
   fChain->SetBranchAddress("rhTime", &rhTime, &b_rhTime);
   fChain->SetBranchAddress("rhTimeErr", &rhTimeErr, &b_rhTimeErr);
   fChain->SetBranchAddress("rhID", &rhID, &b_rhID);
   Notify();
}

inline Bool_t ReducedBase::Notify()
{
   // The Notify() function is called when a new file is opened. This
   // can be either for a new TTree in a TChain or when when a new TTree
   // is started when using PROOF. It is normally not necessary to make changes
   // to the generated code, but the routine can be extended by the
   // user if needed. The return value is currently not used.

   return kTRUE;
}

inline void ReducedBase::Show(Long64_t entry)
{
// Print contents of entry.
// If entry is not specified, print current entry
   if (!fChain) return;
   fChain->Show(entry);
}

inline Int_t ReducedBase::Cut(Long64_t entry)
{
// This function may be called from Loop.
// returns  1 if entry is accepted.
// returns -1 otherwise.
   return 1;
}
