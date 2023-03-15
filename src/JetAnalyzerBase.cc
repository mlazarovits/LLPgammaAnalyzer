#include <iostream>
#include <vector>
#include <TChain.h>
#include "../interface/JetAnalyzerBase.hh"
#include "../interface/ReducedBase.hh"

using std::cout;
using std::endl;
using std::vector;

JetAnalyzerBase::JetAnalyzerBase(ReducedBase* base){ 
	_base = base;
	_nJets = _base->nJets;

}

JetAnalyzerBase::~JetAnalyzerBase(){ 
	_jetTimes.clear();
	_pvTimes.clear();
	_jetDistsToPV.clear();
	_pvTimeDiffs.clear();
	_dphi.clear();	
}

//looks at times for backtoback jets
void JetAnalyzerBase::Analyze_BackToBack(){
	vector<unsigned int> rhIDs1;
	vector<unsigned int> rhIDs2;
	double dphi_dn = 3;
	double dphi = -999;
	double t_pv1 = -999;
	double t_pv2 = -999;
	for(int j1 = 0; j1 < _nJets; j1++){
		for(int j2 = 0; j2 < _nJets; j2++){	
			if(j1 <= j2) continue;
			dphi = _deltaPhi(_base->jetPhi->at(j1), _base->jetPhi->at(j2));
			if(fabs(dphi) < dphi_dn) continue;
			_dphi.push_back(dphi);
		
			rhIDs1 = GetRhIDs(j1);
			rhIDs2 = GetRhIDs(j2);

			t_pv1 = CalcPVTime(rhIDs1);
			t_pv2 = CalcPVTime(rhIDs2);
			
			_pvTimeDiffs.push_back(t_pv1 - t_pv2);
			_pvTimes.push_back(t_pv1);
			_pvTimes.push_back(t_pv2);
			
			rhIDs1.clear();
			rhIDs2.clear();
			
		}
	}
	
}


vector<unsigned int> JetAnalyzerBase::GetRhIDs(int j){
	vector<unsigned int> rhIDs;
	for(int i = 0; i < (int)_base->jetRecHitOfJet->size(); i++){
		if(_base->jetRecHitOfJet->at(i) == j) rhIDs.push_back(_base->jetRecHitId->at(i));
	}
	return rhIDs;
}


double JetAnalyzerBase::CalcPVTime(vector<unsigned int> rhIDs){
	double t = CalcTime(rhIDs);
	double d_pv = CalcDistToPV(rhIDs);
	_jetTimes.push_back(t);
	_jetDistsToPV.push_back(d_pv);
	return t - d_pv/_c;
}
