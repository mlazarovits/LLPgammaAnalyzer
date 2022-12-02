#include <iostream>
#include <fstream>
#include "ReducedBase.hh"


using std::cout;
using std::endl;
using std::string;


int main(int argc, char* argv[]){
	cout << "Look at all these chickens!" << endl;	

	string indir = "LLPGamma/llpga_GMSB_AOD_v55/";
	string infilename = "llpgana_mc_AODSIM_GMSB_AOD_v55_Full.txt";
	string outfilename = "llpgana_mc_AODSIM_GMSB_AOD_v55_pv-time-skim.root";

	string eosdir = "root://cmseos.fnal.gov//store/user/lpcsusylep/jaking/";
	
	string treeName = "tree/llpgana";
	string infilelist = "llpgana_mc_AODSIM_GMSB_AOD_v55_Full.txt";	
	std::ifstream infile(infilelist);
	
	TChain* ch = new TChain(treeName.c_str());
	//get trees from files
	string str;	
	int pct = 10;
	int cnt = 1;
	cout << "adding every 1 out of " << pct << " files from " << eosdir+indir+infilelist << endl;
	while(std::getline(infile, str)){
		string tfilename = eosdir + indir + str;
		if(cnt%pct == 0){
			std::cout << "--  adding file: " << tfilename << std::endl;
			ch->Add(tfilename.c_str());
		}
		cnt++;
	}

	//initialize chain with make class
	ReducedBase* base = new ReducedBase(ch);	
	//loop through events
	int nEntries = ch->GetEntries();
	cout << nEntries << endl;
	for(int i = 0; i < nEntries; i++){
		if (i % 10000 == 0) cout << "Processed " << i << " of " << nEntries << endl;
			
	}




}
