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
	_jetHists.push_back(new TH1D("jetTime_LeadRH","jetTime_LeadRH",50, -0.5, 15.));
	_jetHists.push_back(new TH1D("jetDistToPV_LeadRH","jetDistToPV_LeadRH",100,100,350));
	_jetHists.push_back(new TH1D("jetTime_EnrGeo","jetTime_EnrGeo",50, -0.5, 15.));
	_jetHists.push_back(new TH1D("jetDistToPV_Geo","jetDistToPV_Geo",100,100,350));


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
	_recHitHists.push_back(new TH1D("rhEnergy_avg","rhEnergy_avg",200,0,200));
	_recHitHists.push_back(new TH1D("rhPos_distToCentroid","rhPos_distToCentroid",200,0,60));


	_vertexHists.push_back(new TH1D("vtxX","vtxX",100,0.6,0.13));
	_vertexHists.push_back(new TH1D("vtxY","vtxY",100,0.3,0.11));
	_vertexHists.push_back(new TH1D("vtxZ","vtxZ",100,-16,18));

	_pvTimeHists.push_back(new TH1D("pvTimeRes_jetDphi","pvTimeRes_jetDphi",50,2.98,3.17));
	_pvTimeHists.push_back(new TH1D("pvTime_LeadRH","pvTime_LeadRH",100,-1.,1.));
	_pvTimeHists.push_back(new TH1D("pvTimeRes_LeadRH","pvTimeRes_LeadRH",125,-1.,1.));
	_pvTimeHists.push_back(new TH1D("pvTime_EnrGeo","pvTime_EnrGeo",100,-1.,1.));
	_pvTimeHists.push_back(new TH1D("pvTimeRes_EnrGeo","pvTimeRes_EnrGeo",125,-1.,1.));


}




ReducedBaseSkimmer::~ReducedBaseSkimmer(){ 
	_jetHists.clear();
	_vertexHists.clear();
	_recHitHists.clear();
	_pvTimeHists.clear();
	//_genJetHists.clear();
}




vector<TH1D*> ReducedBaseSkimmer::Skim(){
	int nEntries = _base->fChain->GetEntries();
	int SKIP = 10;
	if(SKIP != 1) cout << "Choosing 1 out of every " << SKIP << " events" << endl;
	for(int e = 0; e < nEntries; e+=SKIP){
		_base->GetEntry(e);
		if((e/SKIP)%(std::max(1, int(nEntries/SKIP/10))) == 0)
	 cout << "      event " << e << " | " << nEntries << endl;
	
		//add skimmers for quantities from ntuples
		_SkimJets();
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


}



void ReducedBaseSkimmer::_SkimPVTimes(){
	vector<unsigned int> rhIDs1;
        vector<unsigned int> rhIDs2;
        double dphi_dn = 3;
        double dphi = -999;
        double t_jet1_LeadRH = -999;
        double t_jet2_LeadRH = -999;
        double t_jet1_egeo = -999;
        double t_jet2_egeo = -999;
        double d_pv1_LeadRH = -999;
        double d_pv2_LeadRH = -999;
        double d_pv1_geo = -999;
        double d_pv2_geo = -999;
        for(int j1 = 0; j1 < (int)_base->nJets; j1++){
                for(int j2 = 0; j2 < (int)_base->nJets; j2++){
                        if(j1 <= j2) continue;
                        dphi = _deltaPhi(_base->jetPhi->at(j1), _base->jetPhi->at(j2));
                        if(fabs(dphi) < dphi_dn) continue;
			_pvTimeHists[0]->Fill(dphi);
			rhIDs1 = GetRhIDs(j1);
                        rhIDs2 = GetRhIDs(j2);
                        t_jet1_LeadRH = MakeJetTime_LeadRH(rhIDs1);
                        t_jet2_LeadRH = MakeJetTime_LeadRH(rhIDs2);
                        d_pv1_LeadRH = MakeJetdPV_LeadRH(rhIDs1);
                        d_pv2_LeadRH = MakeJetdPV_LeadRH(rhIDs2);
			
			_jetHists[6]->Fill(t_jet1_LeadRH); 
			_jetHists[6]->Fill(t_jet2_LeadRH); 
			_jetHists[7]->Fill(d_pv1_LeadRH); 
			_jetHists[7]->Fill(d_pv2_LeadRH);

			_pvTimeHists[1]->Fill(t_jet1_LeadRH - d_pv1_LeadRH/_c); 
			_pvTimeHists[1]->Fill(t_jet2_LeadRH - d_pv2_LeadRH/_c); 
			_pvTimeHists[2]->Fill((t_jet1_LeadRH - d_pv1_LeadRH/_c) - (t_jet2_LeadRH - d_pv2_LeadRH/_c)); 
                        
			t_jet1_egeo = MakeJetTime_EnrGeo(rhIDs1);
                        t_jet2_egeo = MakeJetTime_EnrGeo(rhIDs2);
                        d_pv1_geo = MakeJetdPV_Geo(rhIDs1);
                        d_pv2_geo = MakeJetdPV_Geo(rhIDs2);
			
			_jetHists[8]->Fill(t_jet1_egeo); 
			_jetHists[8]->Fill(t_jet2_egeo); 
			_jetHists[9]->Fill(d_pv1_geo); 
			_jetHists[9]->Fill(d_pv2_geo);
			
			_pvTimeHists[3]->Fill(t_jet1_egeo - d_pv1_geo/_c); 
			_pvTimeHists[3]->Fill(t_jet2_egeo - d_pv2_geo/_c); 
			_pvTimeHists[4]->Fill((t_jet1_egeo - d_pv1_geo/_c) - (t_jet2_egeo - d_pv2_geo/_c)); 
                        
			rhIDs1.clear();
                        rhIDs2.clear();

                }

	}
}

//fractional energy and inverse distance to centroid weighted
double ReducedBaseSkimmer::MakeJetTime_EnrGeo(vector<unsigned int> rhIDs){
	double jet_time = 0.;
	double enr_tot = 0.;
        vector<double> rh_times;
	vector<double> rh_enr;
	vector<double> rh_x;
	vector<double> rh_y;
	vector<double> rh_z;
	int nRHs = (int)rhIDs.size();
	for(int i = 0; i < nRHs; i++){
                for(int rh = 0; rh < _base->nRecHits; rh++){
                        if(_base->rhID->at(rh) == rhIDs[i]){
                                enr_tot += _base->rhEnergy->at(rh);
                                rh_times.push_back(_base->rhTime->at(rh));
				rh_enr.push_back(_base->rhEnergy->at(rh));	
				rh_x.push_back(_base->rhPosX->at(rh));
				rh_y.push_back(_base->rhPosY->at(rh));
				rh_z.push_back(_base->rhPosZ->at(rh));
				break;
                        }
                }
        }
if(rh_x.size() == 0 || rh_y.size() == 0 || rh_z.size() == 0){  return -999.;}
	vector<double> centroid = _centroid(rh_x, rh_y, rh_z);
	vector<double> wts;
	double dist = -999;
	double norm = 0;
	double wt = 0;
	for(int i = 0; i < nRHs; i++){
		dist = hypo(rh_x[i] - centroid[0], rh_y[i] - centroid[1], rh_z[i] - centroid[2]);
		if(dist < 1.1){
			wt = 1.;
		}
		if(dist < 0) cout << "negative distance to centroid: " << dist << endl;
		//fill hist with distance from rh to center
		_recHitHists[11]->Fill(dist); 
		//consider just doing energy (not fractional)
		//convert distance to time
		//dist /= _c;
		wt = (rh_enr[i]/enr_tot)/dist;
	//	wt = 1./dist;
		wts.push_back(wt);
		norm += wt;
	}
	for(int i = 0; i < nRHs; i++) jet_time += (rh_times[i]*wts[i])/norm;
	return jet_time;
}

double ReducedBaseSkimmer::MakeJetdPV_Geo(vector<unsigned int> rhIDs){
	vector<double> rh_x;
	vector<double> rh_y;
	vector<double> rh_z;
	int nRHs = (int)rhIDs.size();
	for(int i = 0; i < nRHs; i++){
                for(int rh = 0; rh < _base->nRecHits; rh++){
                        if(_base->rhID->at(rh) == rhIDs[i]){
				rh_x.push_back(_base->rhPosX->at(rh));
				rh_y.push_back(_base->rhPosY->at(rh));
				rh_z.push_back(_base->rhPosZ->at(rh));
				break;
                        }
                }
        }
if(rh_x.size() == 0 || rh_y.size() == 0 || rh_z.size() == 0){  return -999.;}
	vector<double> centroid = _centroid(rh_x, rh_y, rh_z);
        double d_pv = hypo(centroid[0] - _base->vtxX, centroid[1] - _base->vtxY, centroid[2] - _base->vtxZ);
        return d_pv;
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



















