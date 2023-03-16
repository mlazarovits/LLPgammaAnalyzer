#include <iostream>
#include <TH1D.h>
#include <TChain.h>
#include "../interface/ReducedBaseSkimmer.hh"

using std::cout;
using std::endl;

ReducedBaseSkimmer::ReducedBaseSkimmer(ReducedBase* base){ 
	_base = base;
	
	_jetHists.push_back(new TH1D("nJets","nJets",11, -0.5, 10.5));
	_jetHists.push_back(new TH1D("jetE", "jetE", 125, 0, 2500)); 
	_jetHists.push_back(new TH1D("jetPt", "jetPt", 125, 0, 2500)); 
	_jetHists.push_back(new TH1D("jetEta", "jetEta", 125, -2, 2)); 
	_jetHists.push_back(new TH1D("jetPhi", "jetPhi", 125, -3.5, 3.5)); 
	//jet PF ID - as defined in analyzer
	_jetHists.push_back(new TH1D("jetID", "jetID", 5, 0, 5)); 
	_jetHists.push_back(new TH1D("jetTime","jetTime",50, -0.5, 30.));
	_jetHists.push_back(new TH1D("jetDistToPV","jetDistToPV",100,100,350));


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
	_pvTimeHists.push_back(new TH1D("pvTimeRes","pvTimeRes",125,-20,20));
	_pvTimeHists.push_back(new TH1D("pvTimeRes_jetDphi","pvTimeRes_jetDphi",50,2.98,3.17));


}




ReducedBaseSkimmer::~ReducedBaseSkimmer(){ 
	_jetHists.clear();
	_vertexHists.clear();
	_recHitHists.clear();
	_pvTimeHists.clear();
	//_genJetHists.clear();
}




vector<TH1D*> ReducedBaseSkimmer::Skim(){
	cout << "Skim" << endl;
	int nEntries = _base->fChain->GetEntries();
	cout << "nEntries: " << nEntries << " " << _base->fChain->GetEntries() << endl;
	int SKIP = 1;
	if(SKIP != 1) cout << "Choosing 1 out of every " << SKIP << " events" << endl;
	for(int e = 0; e < nEntries; e+=SKIP){
		_base->GetEntry(e);
		if((e/SKIP)%(std::max(1, int(nEntries/SKIP/10))) == 0)
	 cout << "      event " << e << " | " << nEntries << endl;
	
		//add skimmers for quantities from ntuples
		_SkimJet();
		_SkimRecHits();
		_SkimVertices();
		_SkimPVTimes();
	}


	vector<TH1D*> hists;		
	for(int h = 0; h < (int)_jetHists.size(); h++) hists.push_back(_jetHists[h]); 
	for(int h = 0; h < (int)_recHitHists.size(); h++) hists.push_back(_recHitHists[h]); 
	for(int h = 0; h < (int)_vertexHists.size(); h++) hists.push_back(_vertexHists[h]); 
	for(int h = 0; h < (int)_pvTimeHists.size(); h++) hists.push_back(_pvTimeHists[h]); 
	return hists;

	cout << "Skim - end" << endl;

}



void ReducedBaseSkimmer::_SkimPVTimes(){
	vector<unsigned int> rhIDs1;
        vector<unsigned int> rhIDs2;
        double dphi_dn = 3;
        double dphi = -999;
        double t_jet1 = -999;
        double t_jet2 = -999;
        double d_pv1 = -999;
        double d_pv2 = -999;
        for(int j1 = 0; j1 < (int)_base->nJets; j1++){
                for(int j2 = 0; j2 < (int)_base->nJets; j2++){
                        if(j1 <= j2) continue;
                        dphi = _deltaPhi(_base->jetPhi->at(j1), _base->jetPhi->at(j2));
                        if(fabs(dphi) < dphi_dn) continue;
			_pvTimeHists[2]->Fill(dphi);
			rhIDs1 = GetRhIDs(j1);
                        rhIDs2 = GetRhIDs(j2);
                        t_jet1 = MakeJetTime_LeadRH(rhIDs1);
                        t_jet2 = MakeJetTime_LeadRH(rhIDs2);
                        d_pv1 = MakeJetdPV_LeadRH(rhIDs1);
                        d_pv2 = MakeJetdPV_LeadRH(rhIDs2);
			_jetHists[6]->Fill(t_jet1); 
			_jetHists[6]->Fill(t_jet2); 
			_jetHists[7]->Fill(d_pv1); 
			_jetHists[7]->Fill(d_pv2);

			_pvTimeHists[0]->Fill(t_jet1 - d_pv1/_c); 
			_pvTimeHists[0]->Fill(t_jet2 - d_pv2/_c); 
			_pvTimeHists[1]->Fill((t_jet1 - d_pv1/_c) - (t_jet2 - d_pv2/_c)); 
                        rhIDs1.clear();
                        rhIDs2.clear();

                }

	}
}





double ReducedBaseSkimmer::MakeJetTime_LeadRH(vector<unsigned int> rhIDs){
	double enr = -999.;
        double jet_time = -999.;
        int idx = -999;
	for(int i = 0; i < (int)rhIDs.size(); i++){
                for(int rh = 0; rh < _base->nRecHits; rh++){
                        if(_base->rhID->at(rh) == rhIDs[i]){
                                if(_base->rhEnergy->at(rh) > enr){
                                        enr = _base->rhEnergy->at(rh);
                                        idx = rh;
                                }
                                break;
                        }
                }
        } 
        if(idx != -999){
                jet_time = _base->rhTime->at(idx);
		_recHitHists[9]->Fill(enr);
	}
        else { return -999.; }
        return jet_time;
}
double ReducedBaseSkimmer::MakeJetdPV_LeadRH(vector<unsigned int> rhIDs){
	double enr = -999.;
        int idx = -999;
       	double d_pv = -999;
	for(int i = 0; i < (int)rhIDs.size(); i++){
                for(int rh = 0; rh < _base->nRecHits; rh++){
                        if(_base->rhID->at(rh) == rhIDs[i]){
                                if(_base->rhEnergy->at(rh) > enr){
                                        enr = _base->rhEnergy->at(rh);
                                        idx = rh;
                                }
                                break;
                        }
                }
        } 
        if(idx != -999){
        	d_pv = _base->rh_distToPV->at(idx);
	}
        else { return -999.; }
        return d_pv;
}



vector<unsigned int> ReducedBaseSkimmer::GetRhIDs(int j){
        vector<unsigned int> rhIDs;
        for(int i = 0; i < (int)_base->jetRecHitOfJet->size(); i++){
                if(_base->jetRecHitOfJet->at(i) == j) rhIDs.push_back(_base->jetRecHitId->at(i));
        }
        return rhIDs;
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



















