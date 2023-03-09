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

	_recHitHists.push_back(new TH1D("nRHs","nRHs",102,0,4794));
	_recHitHists.push_back(new TH1D("rhPosX","rhPosX",100,-175,175));
	_recHitHists.push_back(new TH1D("rhPosY","rhPosY",100,-175,175));
	_recHitHists.push_back(new TH1D("rhPosZ","rhPosZ",100,-380,380));
	_recHitHists.push_back(new TH1D("rhEta","rhEta",100,-3.5,3.5));
	_recHitHists.push_back(new TH1D("rhPhi","rhPhi",100,-3.5,3.5));
	_recHitHists.push_back(new TH1D("rhEnergy","rhEnergy",1000,0,1000));
	_recHitHists.push_back(new TH1D("rhTime","rhTime",500,-50,50));
	_recHitHists.push_back(new TH1D("rhTimeErr","rhTimeErr",2000,-50,50));


	_vertexHists.push_back(new TH1D("vtxX","vtxX",100,-0.10,0.04));
	_vertexHists.push_back(new TH1D("vtxY","vtxY",100,0.3,0.11));
	_vertexHists.push_back(new TH1D("vtxZ","vtxZ",100,-16,18));

	_pvTimeHists.push_back(new TH1D("pvTime","pvTime",500,-50,50));
	_pvTimeHists.push_back(new TH1D("pvTimeRes","pvTimeRes",500,-50,50));
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
	int nEntries = _ch->GetEntries();
	int SKIP = 10;
	if(SKIP != 1) cout << "Choosing 1 out of every " << SKIP << " events" << endl;
	for(int e = 0; e < nEntries; e+=SKIP){
		_base->GetEntry(e);
		if((e/SKIP)%(std::max(1, int(nEntries/SKIP/10))) == 0)
	 cout << "      event " << e << " | " << nEntries << endl;
		//add skimmers for objects here
		_SkimJets();
		_SkimRecHits();
		_SkimVertices();
		_MakePVTimes();
	}
	vector<TH1D*> hists;		
	for(int h = 0; h < (int)_jetHists.size(); h++) hists.push_back(_jetHists[h]); 
	for(int h = 0; h < (int)_recHitHists.size(); h++) hists.push_back(_recHitHists[h]); 
	for(int h = 0; h < (int)_vertexHists.size(); h++) hists.push_back(_vertexHists[h]); 
	for(int h = 0; h < (int)_pvTimeHists.size(); h++) hists.push_back(_pvTimeHists[h]); 
	return hists;


}

void ReducedBaseSkimmer::_SkimJets(){
cout << "makejets" << endl;	
//only fill jets that we dR matched to rec hits
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


//make function to match dR jets to rechits - don't need this - ntuple has rechitIDs + jetRecHitOfJet and can match that way
/*

void ReducedBaseSkimer::_getRHGroup(int jetIdx){
	//get ids of all rechits in a jet (given jet idx)
	int idx = ( std::find(_base->jetRecHitOfJet()->begin(),_base->jetRecHitOfJet()->end(),jetIdx) - _base->jetRecHitOfJet()->begin() );

	unsigned int rhID = _base->rhID->at(idx);
	double e = 0.;
	



}

*/
void ReducedBaseSkimmer::_MakePVTimes(){
cout << "makepvtimes" << endl;
//loop through jets - find pairs that are back to back (dphi ~ pi or something)
	int nJets = _base->nJets;
	double dphi;
	double dphi_up = 3.28;
	double dphi_dn = 3.;
	//double ptUp = 200.;
	//double fracPtDn = 0.8; //min fractional pt
	//double fracHtDn = 0.8; //min fractional ht
	
	for(int j1 = 0; j1 < nJets; j1++){
		for(int j2 = 0; j2 < nJets; j2++){
			if(j1 <= j2) continue;
			//calculate dphi 
			dphi = _base->jetPhi->at(j1) - _base->jetPhi->at(j2);
			//make sure dphi is around pi
			if(dphi < dphi_dn || dphi > dphi_up) continue;
			
			//calculate jet time for each jet - save in hist
			//save difference in histogram			
		}
	}



//for each pair:
//	take difference of t_pv_diff = t_pv_jet1 - t_pv_jet2
//	put t_pv_diff in histogram pvTimeRes




}


//make jet time for jet # i in event
double ReducedBaseSkimmer::_MakeJetTime(int j){
cout << "makejettime" << endl;
//within each jet in pair:
//	find pos of lead rh, compute distance from (0,0,0)
//	compute distance bw lead rh and pv
//	take difference, divide by SOL = t_pv
//	put t_pv in histogram pvTime
	vector<unsigned int> rhIDs = _base->jetRecHitId->at(j);
	
//	find lead rh of jet
	double enr = 0.;
	unsigned int lead_id = -999;
	for(int i = 0; i < (int)rhIDs.size(); i++){
		for(int rh = 0; rh < _base->nRecHits; rh++){
			//get ith rechit in jet j
			if(_base->rhID->at(rh) == rhIDs[i]){
				//update energy
				if(_base->rhEnergy->at(rh) > enr){
					enr = _base->rhEnergy->at(rh);
					lead_id = rhIDs[i];
				}
				break;
			}
		}
	}
	
	//get positions of lead rh
	double rhX = -999;
	double rhY = -999;
	double rhZ = -999;
	for(int rh = 0; rh < _base->nRecHits; rh++){
		if(_base->rhID->at(rh) == lead_id){
				rhX = _base->rhPosX->at(rh);
				rhY = _base->rhPosY->at(rh);
				rhZ = _base->rhPosZ->at(rh);
			}
				break;
	}
	
	//double vtxX = _base->vtxX;
			//temp
	double tmp = rhX + rhY + rhZ;
	return tmp;


}



void ReducedBaseSkimmer::_SkimGenJets(){
/*
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
*/

}



void ReducedBaseSkimmer::_SkimRecHits(){
cout << "skimrh" << endl;	
int nRHs = _base->nRecHits;
cout << "nrhs: " << nRHs << endl;
cout << "x: " << _base->rhPosX->size() << endl;
cout << "y: " << _base->rhPosY->size() << endl;
cout << "z: " << _base->rhPosZ->size() << endl;
cout << "Eta: " << _base->rhPosEta->size() << endl;
cout << "Phi: " << _base->rhPosPhi->size() << endl;
cout << "energy: " << _base->rhEnergy->size() << endl;
cout << "time: " << _base->rhTime->size() << endl;
cout << "time err: " << _base->rhTimeErr->size() << endl;
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

cout << "skimrh - end" << endl;	

}


void ReducedBaseSkimmer::_SkimVertices(){
	cout << "skimvertices" << endl;
	//int nVtx = _base->nVtx;
	//_vertexHists[0]->Fill(nVtx);
	////only PV saved
	////these are not vectors, these are floats - position of only primary vertex
	_vertexHists[0]->Fill(_base->vtxX);
	_vertexHists[1]->Fill(_base->vtxY);
	_vertexHists[2]->Fill(_base->vtxZ);



}




















