#include <iostream>
#include <fstream>
#include "ReducedBase.hh"


using std::cout;
using std::endl;
using std::string;


int main(int argc, char* argv[]){
	cout << "Look at all those chickens!" << endl;	

	string indir = "LLPGamma/llpga_GMSB_AOD_v55/";
	string outfilename = "root/llpgana_mc_AODSIM_GMSB_AOD_v55_pv-time-skim.root";

	string eosdir = "root://cmseos.fnal.gov//store/user/lpcsusylep/jaking/";
	
	string treeName = "tree/llpgtree";
	string infilelist = "fileLists/llpgana_mc_AODSIM_GMSB_AOD_v55_Full.txt";	
	std::ifstream infile(infilelist);
	
	TChain* ch = new TChain(treeName.c_str());
	//get trees from files
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

	//initialize chain with make class
	ReducedBase* base = new ReducedBase(ch);	
	//loop through events
	int nEntries = ch->GetEntries();
	for(int i = 0; i < nEntries; i++){
		if (i % 100000 == 0) cout << "Processed " << i << " of " << nEntries << endl;
			
	}



	TFile* outFile = new TFile(outfilename.c_str(),"RECREATE");
	outFile->cd();
	//write shit here to file
	cout << "Writing out to " << outfilename << endl;
	outFile->Close();




}
