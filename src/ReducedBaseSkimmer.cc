#include <iostream>
#include <TH1D.h>
#include <TChain.h>
#include "../interface/ReducedBaseSkimmer.hh"
#include "../interface/JetAnalyzerLeadRH.hh"
#include "../interface/JetAnalyzerBase.hh"

using std::cout;
using std::endl;

ReducedBaseSkimmer::ReducedBaseSkimmer(TChain* ch){ 
	_ch = ch;
	_base = new ReducedBase(ch);
	
	_jetHists.push_back(new TH1D("nJets","nJets",11, -0.5, 10.5));
	_jetHists.push_back(new TH1D("jetE", "jetE", 125, 0, 2500)); 
	_jetHists.push_back(new TH1D("jetPt", "jetPt", 125, 0, 2500)); 
	_jetHists.push_back(new TH1D("jetEta", "jetEta", 125, -2, 2)); 
	_jetHists.push_back(new TH1D("jetPhi", "jetPhi", 125, -3.5, 3.5)); 
	//jet PF ID - as defined in analyzer
	_jetHists.push_back(new TH1D("jetID", "jetID", 5, 0, 5)); 
	_jetHists.push_back(new TH1D("jetTime","jetTime",50, -0.5, 30.));


	_recHitHists.push_back(new TH1D("nRHs","nRHs",100,0,600));
	_recHitHists.push_back(new TH1D("rhPosX","rhPosX",100,-175,175));
	_recHitHists.push_back(new TH1D("rhPosY","rhPosY",100,-175,175));
	_recHitHists.push_back(new TH1D("rhPosZ","rhPosZ",100,-380,380));
	_recHitHists.push_back(new TH1D("rhEta","rhEta",100,-2,2));
	_recHitHists.push_back(new TH1D("rhPhi","rhPhi",100,-3.5,3.5));
	_recHitHists.push_back(new TH1D("rhEnergy","rhEnergy",200,0,800));
	_recHitHists.push_back(new TH1D("rhTime","rhTime",250,0.,1000));
	_recHitHists.push_back(new TH1D("rhTimeErr","rhTimeErr",125,-10,10));
	_recHitHists.push_back(new TH1D("rhEnergy_lead","rhEnergy_lead",200,0,200));


	_vertexHists.push_back(new TH1D("vtxX","vtxX",100,0.6,0.13));
	_vertexHists.push_back(new TH1D("vtxY","vtxY",100,0.3,0.11));
	_vertexHists.push_back(new TH1D("vtxZ","vtxZ",100,-16,18));

	_pvTimeHists.push_back(new TH1D("pvTime","pvTime",100,-20,20));
	_pvTimeHists.push_back(new TH1D("pvTimeRes","pvTimeRes",125,-50,50));
	_pvTimeHists.push_back(new TH1D("pvTimeRes_jetDphi","pvTimeRes_jetDphi",50,2.98,3.17));
	_pvTimeHists.push_back(new TH1D("pvDistToJet","pvDistToJet",100,-20,20));
/*
	_genJetHists.push_back(new TH1D("nJetsGen","nJetsGen",21,-0.5,20.5));
	_genJetHists.push_back(new TH1D("jetGenEnergy","jetGenEnergy",500,0,5000));	
	_genJetHists.push_back(new TH1D("jetGenPt","jetGenPt",500,0,5000));
	_genJetHists.push_back(new TH1D("jetGenEta","jetGenEta",700,-3.5,3.5));
	_genJetHists.push_back(new TH1D("jetGenTOF","jetGenTOF",300,0,30));
	_genJetHists.push_back(new TH1D("jetGenDrMatch","jetGenDrMatch",320,0,3.2));
	_genJetHists.push_back(new TH1D("jetGenTimeVar","jetGenTimeVar",270,-2,25));
	_genJetHists.push_back(new TH1D("jetGenNKids","jetGenNKids",100,0,100));
*/	


}




ReducedBaseSkimmer::~ReducedBaseSkimmer(){ 
	delete _base;
	_jetHists.clear();
	_genJetHists.clear();
	_vertexHists.clear();
	_recHitHists.clear();
	_pvTimeHists.clear();
}




vector<TH1D*> ReducedBaseSkimmer::Skim(){
	//JetAnalyzerLeadRH* ja = new JetAnalyzerLeadRH(_base);
	int nEntries = _ch->GetEntries();
	int SKIP = 1;
	if(SKIP != 1) cout << "Choosing 1 out of every " << SKIP << " events" << endl;
	for(int e = 0; e < nEntries; e+=SKIP){
		_base->GetEntry(e);
		if((e/SKIP)%(std::max(1, int(nEntries/SKIP/10))) == 0)
	 cout << "      event " << e << " | " << nEntries << endl;
	
		//add skimmers for quantities from ntuples
		_SkimJets();
		_SkimRecHits();
		_SkimVertices();
	//	ja->Analyze_BackToBack();
//		_SkimPVTimes();
	}


	vector<TH1D*> hists;		
	for(int h = 0; h < (int)_jetHists.size(); h++) hists.push_back(_jetHists[h]); 
	for(int h = 0; h < (int)_recHitHists.size(); h++) hists.push_back(_recHitHists[h]); 
	for(int h = 0; h < (int)_vertexHists.size(); h++) hists.push_back(_vertexHists[h]); 
	for(int h = 0; h < (int)_pvTimeHists.size(); h++) hists.push_back(_pvTimeHists[h]); 
	return hists;


}


void ReducedBaseSkimmer::_SkimPVTimes(){
//	ja->Analyze_BackToBack();
//	vector<double> jet_times = ja->GetJetTimes();
//	vector<double> pv_times = ja->GetPVTimes();
//	vector<double> pv_times_diff = ja->GetPVTimeDiffs();
//	vector<double> d_jet_pv = ja->GetJetDistsToPV();
//	vector<double> d_phi = ja->GetDeltaPhi();
//
//	for(auto t : pv_times) _pvTimeHists[0]->Fill(t); 
//	for(auto t : pv_times_diff) _pvTimeHists[1]->Fill(t); 
//	for(auto t : d_phi) _pvTimeHists[2]->Fill(t); 
//	for(auto t : d_jet_pv) _pvTimeHists[3]->Fill(t); 
//	for(auto j : jet_times) _jetHists[6]->Fill(j);

}





void ReducedBaseSkimmer::_SkimJets(){
	//cout << "skim jets" << endl;
	int nJets = _base->nJets;
	_jetHists[0]->Fill(nJets);
	for(int j = 0; j < nJets; j++){
		//energy, spatial variables
		//these are vectors
		_jetHists[1]->Fill(_base->jetE->at(j));
		_jetHists[2]->Fill(_base->jetPt->at(j));
		_jetHists[3]->Fill(_base->jetEta->at(j));
		_jetHists[4]->Fill(_base->jetPhi->at(j));
		_jetHists[5]->Fill(_base->jetID->at(j));
		
	}

}


void ReducedBaseSkimmer::_SkimRecHits(){
//cout << "skimrechits" << endl;
int nRHs = _base->nRecHits;
//cout << "nrhs: " << nRHs << endl;
	_recHitHists[0]->Fill(nRHs);
	for(int r = 0; r < nRHs; r++){
		//these are vectors
		_recHitHists[1]->Fill(_base->rhPosX->at(r));
		_recHitHists[2]->Fill(_base->rhPosY->at(r));
		_recHitHists[3]->Fill(_base->rhPosZ->at(r));
		_recHitHists[4]->Fill(_base->rhPosEta->at(r));
		_recHitHists[5]->Fill(_base->rhPosPhi->at(r));
		_recHitHists[6]->Fill(_base->rhEnergy->at(r));
		_recHitHists[7]->Fill(_base->rhTime->at(r));
		_recHitHists[8]->Fill(_base->rhTimeErr->at(r));


	}


}


void ReducedBaseSkimmer::_SkimVertices(){
	//int nVtx = _base->nVtx;
	//_vertexHists[0]->Fill(nVtx);
	////only PV saved
	////these are not vectors, these are floats - position of only primary vertex
	_vertexHists[0]->Fill(_base->vtxX);
	_vertexHists[1]->Fill(_base->vtxY);
	_vertexHists[2]->Fill(_base->vtxZ);



}







/*
void ReducedBaseSkimmer::_SkimGenJets(){
	int nJets = _base->nJets;
	_genJetHists[0]->Fill(nJets);
	for(int j = 0; j < nJets; j++){
		//if a jet was not dR matched within dR = 0.3, default values are stored
		//these are vectors
		//_genJetHists[1]->Fill(_base->jetGenEnergy->at(j));	
		//_genJetHists[2]->Fill(_base->jetGenPt->at(j));
		//_genJetHists[3]->Fill(_base->jetGenEta->at(j));
		//_genJetHists[5]->Fill(_base->jetGenTOF->at(j));
		//_genJetHists[6]->Fill(_base->jetGenDrMatch->at(j));
		//_genJetHists[7]->Fill(_base->jetGenTimeVar->at(j));
		//_genJetHists[8]->Fill(_base->jetGenTimeLLP->at(j));
		//_genJetHists[9]->Fill(_base->jetGenNKids->at(j));
		


	}

}

*/














