#include <iostream>
#include <fstream>
#include <vector>
#include <TH1D.h>
#include <TChain.h>
#include <TFile.h>
#include <TSystem.h>

#include "../interface/ReducedBaseSkimmer.hh"

using std::cout;
using std::endl;
using std::string;
using std::vector;

int main(int argc, char* argv[]){
	cout << "Look at all those chickens!" << endl;	

	string indir = "ntuples/";
	string infileName = "llpgana_mc_MINIAODSIM_test_JetHT_2018A";
	string outfilename = "pv-time-skim.root";
	string outfileDir = "skims/"+infileName+"/";
	outfilename = outfileDir+outfilename;

	//if out dir doesn't exist, make it
	if(gSystem->AccessPathName(outfileDir.c_str())){
		gSystem->mkdir(outfileDir.c_str());
	}	

	//string eosdir = "root://cmseos.fnal.gov//store/user/lpcsusylep/jaking/";
	
	
	string treeName = "tree/llpgtree";
	string infilelist = "/uscms/home/mlazarov/nobackup/CMSSW_10_6_5/src/PVTiming/LLPgammaAnalyzer/fileLists/llpga_JetHT_Run2018A-UL2018_MiniAODv2-v1_MINIAOD_user.txt";	
		
	TChain* ch = new TChain(treeName.c_str());
	//get trees from list of file
	if(!infilelist.empty()){
		std::ifstream infile(infilelist);
		int pct = 10;
		int cnt = 1;
		string str;	

    
		cout << "adding every 1 out of " << pct << " files from " << infilelist << endl;
		while(std::getline(infile, str)){
			//string tfilename = eosdir + indir + str;
			string tfilename =  str;
			//if((e/SKIP)%(std::max(1, int(nEntries/SKIP/10))) == 0)
         		//cout << "      event " << e << " | " << nEntries << endl;
			if(cnt%pct == 0){
				if(cnt%(pct*100) == 0) std::cout << "Processed " << cnt << " files" << std::endl;
				ch->Add(tfilename.c_str());
			}
			cnt++;
		}
	}
	else{
		string infile = indir+infileName+".root";
		ch->Add(infile.c_str());
	}
ReducedBase* base = new ReducedBase(ch);
//int nEntries = base->fChain->GetEntries();
//int SKIP = 1;
//cout << "total entries: " << nEntries << endl;
//for(int i = 0; i < nEntries; i+=SKIP){
//	base->GetEntry(i);
//	if((i/SKIP)%(std::max(1, int(nEntries/SKIP/10))) == 0)
//	cout << "      event " << i << " | " << nEntries << endl;
//
//}

	//initialize chain with make class
	ReducedBaseSkimmer skim(base);
	vector<TH1D*> hists = skim.Skim();

	TFile* outFile = new TFile(outfilename.c_str(),"RECREATE");
	outFile->cd();
	//write shit here to file
	for(int i = 0; i < int(hists.size()); i++) hists[i]->Write();
	cout << "Writing out to " << outfilename << endl;
	outFile->Close();
	hists.clear();
}
