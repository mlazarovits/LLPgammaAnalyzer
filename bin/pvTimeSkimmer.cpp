#include <iostream>
#include <fstream>
#include <vector>
#include <TH1D.h>
#include <TChain.h>
#include <TFile.h>

#include "../interface/ReducedBaseSkimmer.hh"

using std::cout;
using std::endl;
using std::string;
using std::vector;

int main(int argc, char* argv[]){
	cout << "Look at all those chickens!" << endl;	

	string indir = "ntuples/";
	string outfilename = "root/llpgana_mc_AODSIM_GMSB_AOD_v55_pv-time-skim.root";

	string eosdir = "root://cmseos.fnal.gov//store/user/lpcsusylep/jaking/";
	
	string treeName = "tree/llpgtree";
	string infilelist = "";	
		
	TChain* ch = new TChain(treeName.c_str());
	//get trees from list of file
	if(!infilelist.empty()){
		std::ifstream infile(infilelist);
		string str;	
		int pct = 10;
		int cnt = 1;
		cout << "adding every 1 out of " << pct << " files from " << infilelist << endl;
		while(std::getline(infile, str)){
			string tfilename = eosdir + indir + str;
			if(cnt%pct == 0){
				if(cnt%(pct*100) == 0) std::cout << "Processed " << cnt << " files" << std::endl;
				ch->Add(tfilename.c_str());
			}
			cnt++;
		}
	}
	else{
		string infile = indir+"GMSB_MC_AODSIM_100TeV_cTau200to10000cm.root";
		ch->Add(infile.c_str());
	}

	//initialize chain with make class
	ReducedBaseSkimmer skim(ch);
	vector<TH1D*> hists = skim.Skim();

cout << "hists size: " << hists.size() << endl;

	TFile* outFile = new TFile(outfilename.c_str(),"RECREATE");
	outFile->cd();
	//write shit here to file
	for(int i = 0; i < int(hists.size()); i++) hists[i]->Write();
	cout << "Writing out to " << outfilename << endl;
	outFile->Close();



}
