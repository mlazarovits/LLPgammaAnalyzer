#include <iostream>
//#include "../interface/JetAnalyzerBase.hh"
#include "../interface/JetAnalyzerLeadRH.hh"
#include "../interface/ReducedBase.hh"

using std::cout;
using std::endl;

JetAnalyzerLeadRH::JetAnalyzerLeadRH(ReducedBase* base){
	_base = base;
	_nJets = _base->nJets;
}

JetAnalyzerLeadRH::~JetAnalyzerLeadRH(){ 
}

double JetAnalyzerLeadRH::CalcTime(vector<unsigned int> rhIDs){
	double enr = -999.;
        double time = -999.;
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
                time = _base->rhTime->at(idx);
        }
        else { return -999.; }
	return time;



}

double JetAnalyzerLeadRH::CalcDistToPV(vector<unsigned int> rhIDs){
	double enr = -999.;
        double d_pv = -999;
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
                d_pv = _base->rh_distToPV->at(idx);
        }
        else { return -999.; }
	return d_pv;

}
