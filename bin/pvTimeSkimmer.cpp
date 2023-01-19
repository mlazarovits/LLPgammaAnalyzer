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
	string infileName = "GMSB_MC_AODSIM_100TeV_cTau200to10000cm";
	string outfilename = "pv-time-skim.root";
	string outfileDir = "skims/"+infileName+"/";
	outfilename = outfileDir+outfilename;

	//if out dir doesn't exist, make it
	if(gSystem->AccessPathName(outfileDir.c_str())){
		gSystem->mkdir(outfileDir.c_str());
	}	

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
		string infile = indir+infileName+".root";
		ch->Add(infile.c_str());
	}

	//initialize chain with make class
	ReducedBaseSkimmer skim(ch);
	vector<TH1D*> hists = skim.Skim();


	TFile* outFile = new TFile(outfilename.c_str(),"RECREATE");
	outFile->cd();
	//write shit here to file
	for(int i = 0; i < int(hists.size()); i++) hists[i]->Write();
	cout << "Writing out to " << outfilename << endl;
	outFile->Close();



}
