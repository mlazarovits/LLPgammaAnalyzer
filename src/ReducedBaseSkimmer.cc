#include <iostream>
#include <TH1D.h>
#include <TChain.h>
#include "../interface/ReducedBaseSkimmer.hh"

using std::cout;
using std::endl;

ReducedBaseSkimmer::ReducedBaseSkimmer(TChain* ch){ 
	_ch = ch;
	_base = new ReducedBase(ch);
	_jetHists.push_back(new TH1D("nJets","nJets",21, -0.5, 20.5));
	_jetHists.push_back(new TH1D("jetE", "jetE", 500, 0, 5000)); 
	_jetHists.push_back(new TH1D("jetPt", "jetPt", 500, 0, 5000)); 
	_jetHists.push_back(new TH1D("jetEta", "jetEta", 700, -3.5, 3.5)); 
	_jetHists.push_back(new TH1D("jetPhi", "jetPhi", 700, -3.5, 3.5)); 
	_jetHists.push_back(new TH1D("jetID", "jetID", 5, 0, 5)); 
	_jetHists.push_back(new TH1D("jetRecHitId", "jetRecHitId", 101, 0, 9191000000.00)); 


	_genJetHists.push_back(new TH1D("nJetsGen","nJetsGen",21,-0.5,20.5));
	_genJetHists.push_back(new TH1D("jetGenEnergy","jetGenEnergy",500,0,5000));	
	_genJetHists.push_back(new TH1D("jetGenPt","jetGenPt",500,0,5000));
	_genJetHists.push_back(new TH1D("jetGenEta","jetGenEta",700,-3.5,3.5));
	_genJetHists.push_back(new TH1D("jetGenTOF","jetGenTOF",300,0,30));
	_genJetHists.push_back(new TH1D("jetGenDrMatch","jetGenDrMatch",320,0,3.2));
	_genJetHists.push_back(new TH1D("jetGenTimeVar","jetGenTimeVar",270,-2,25));
	_genJetHists.push_back(new TH1D("jetGenNKids","jetGenNKids",100,0,100));
	


}



ReducedBaseSkimmer::~ReducedBaseSkimmer(){ 
	//delete[] _jetHists; ?
}




vector<TH1D*> ReducedBaseSkimmer::Skim(){
	int nEntries = _ch->GetEntries();
	int SKIP = 10;
	if(SKIP != 1) cout << "Choosing 1 out of every " << SKIP << " events" << endl;
	for(int e = 0; e < nEntries; e+=SKIP){
		_base->GetEntry(e);
		if((e/SKIP)%(std::max(1, int(nEntries/SKIP/10))) == 0)
	 cout << "      event " << e << " | " << nEntries << endl;
		_SkimJets();
	}
	vector<TH1D*> hists;		
	for(int h = 0; h < 7; h++) hists.push_back(_jetHists[h]); 
	return hists;


}

void ReducedBaseSkimmer::_SkimJets(){
	//only fill jets that we dR matched to rec hits
	int nJets = _base->nGoodDrJets;
	_jetHists[0]->Fill(nJets);
	for(int j = 0; j < nJets; j++){
		//energy, spatial variables
		//these are vectors
		_jetHists[1]->Fill(_base->jetE->at(j));
		_jetHists[2]->Fill(_base->jetPt->at(j));
		_jetHists[3]->Fill(_base->jetEta->at(j));
		_jetHists[4]->Fill(_base->jetPhi->at(j));
		_jetHists[5]->Fill(_base->jetID->at(j));
		int nRHs = _base->jetDrRhCnt->at(j);
		for(int rh = 0; rh < nRHs; rh++){
			_jetHists[6]->Fill(_base->jetDrRhIds->at(j).at(rh));
		}	
		
	}


}


//make function to match dR jets to rechits


void ReducedBaseSkimmer::_SkimGenJets(){
	int nJets = _base->nJets;
	_genJetHists[0]->Fill(nJets);
	for(int j = 0; j < nJets; j++){
		//if a jet was not dR matched within dR = 0.3, default values are stored
		//these are vectors
		_genJetHists[1]->Fill(_base->jetGenEnergy->at(j));	
		_genJetHists[2]->Fill(_base->jetGenPt->at(j));
		_genJetHists[3]->Fill(_base->jetGenEta->at(j));
		_genJetHists[5]->Fill(_base->jetGenTOF->at(j));
		_genJetHists[6]->Fill(_base->jetGenDrMatch->at(j));
		_genJetHists[7]->Fill(_base->jetGenTimeVar->at(j));
		_genJetHists[8]->Fill(_base->jetGenTimeLLP->at(j));
		_genJetHists[9]->Fill(_base->jetGenNKids->at(j));
		


	}


}



void ReducedBaseSkimmer::_SkimRecHits(){
	int nRHs = _base->nRecHits;
	_recHitHists[0]->Fill(nRHs);
	for(int r = 0; r < nRHs; r++){
		//these are vectors
		//_recHitHists[1]->Fill(_base->rhPosX[r]);
		//_recHitHists[2]->Fill(_base->rhPosY[r]);
		//_recHitHists[3]->Fill(_base->rhPosZ[r]);
		//_recHitHists[4]->Fill(_base->rhPosEta[r]);
		//_recHitHists[5]->Fill(_base->rhPosPhi[r]);
		//_recHitHists[6]->Fill(_base->rhEnergy[r]);
		//_recHitHists[7]->Fill(_base->rhTime[r]);
		//_recHitHists[8]->Fill(_base->rhTimeErr[r]);
		//_recHitHists[9]->Fill(_base->rhTOF[r]);
		





	}


}


void ReducedBaseSkimmer::_SkimVertices(){
	int nVtx = _base->nVtx;
	_vertexHists[1]->Fill(nVtx);
	for(int v = 0; v < nVtx; v++){
		//first vertex is primary vertex
		//these are not vectors, these are floats
		//_vertexHists[1]->Fill(_base->vtxX[v]);
		//_vertexHists[2]->Fill(_base->vtxY[v]);
		//_vertexHists[3]->Fill(_base->vtxZ[v]);
	}



}




















