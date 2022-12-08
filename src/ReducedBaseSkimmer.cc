#include <TH1D.h>
#include "../interface/ReducedBaseSkimmer.hh"


ReducedBaseSkimmer::ReducedBaseSkimmer(ReducedBase* base){ 
	_base = base;

}



ReducedBaseSkimmer::~ReducedBaseSkimmer(){ }




void ReducedBaseSkimmer::SkimJets(){
	int nJets = _base->nJets;
	_jetHists[0]->Fill(nJets);
	for(int j = 0; j < nJets; j++){
		//energy, spatial variables
		//these are vectors
		_jetHists[1]->Fill(_base->jetE->at(j));
		_jetHists[2]->Fill(_base->jetPt->at(j));
		_jetHists[3]->Fill(_base->jetEta->at(j));
		_jetHists[4]->Fill(_base->jetPhi->at(j));
		//BC variables
		_jetHists[6]->Fill(_base->jetBcTimesCnt->at(j)); //number of basic cluster (BC) mean times
		_jetHists[7]->Fill(_base->jetBcSumRHEnr->at(j)); //total BC energy in jet
		_jetHists[8]->Fill(_base->jetBcEMFr->at(j)); //total BC energy divided by jet energy
		_jetHists[9]->Fill(_base->jetBcRhCnt->at(j));
		_jetHists[10]->Fill(_base->jetID->at(j));
			
		
	}


}

void ReducedBaseSkimmer::SkimCaloJets(){ 
	int nCJets = _base->nCaloJets;
	_caloJetHists[0]->Fill(nCJets);
	for(int j = 0; j < nCJets; j++){
		//these are vectors
	//	_caloJetHists[1]->Fill(_base->cljBcCnt->at(j));
	//	_caloJetHists[2]->Fill(_base->cljSeedTOFTime->at(j));
	//	_caloJetHists[3]->Fill(_base->cljCMeanTime->at(j));
	//	_caloJetHists[4]->Fill(_base->cljBc3dEx->at(j));
	//	_caloJetHists[5]->Fill(_base->cljBc3dEy->at(j));
	//	_caloJetHists[6]->Fill(_base->cljBc3dEz->at(j));
	//	_caloJetHists[7]->Fill(_base->cljBc3dEv->at(j));
	//	_caloJetHists[8]->Fill(_base->cljBc3dEslope->at(j));
	//	_caloJetHists[9]->Fill(_base->cljBc3dEchisp->at(j));
	//	_caloJetHists[10]->Fill(_base->cljBc2dEx->at(j));
	//	_caloJetHists[11]->Fill(_base->cljBc2dEy->at(j));
	//	_caloJetHists[12]->Fill(_base->cljBc2dEv->at(j));
	//	_caloJetHists[13]->Fill(_base->cljBc2dEslope->at(j));
	//	_caloJetHists[14]->Fill(_base->cljBc2dEchisp->at(j));
	//	_caloJetHists[15]->Fill(_base->cljCDrMeanTime->at(j));
	//	_caloJetHists[16]->Fill(_base->cljPt->at(j));
	//	_caloJetHists[17]->Fill(_base->cljEnergy->at(j));
	//	_caloJetHists[18]->Fill(_base->cljPhi->at(j));
	//	_caloJetHists[19]->Fill(_base->cljEta->at(j));
	//	_caloJetHists[20]->Fill(_base->cljPx->at(j));
	//	_caloJetHists[21]->Fill(_base->cljPy->at(j));
	//	_caloJetHists[22]->Fill(_base->cljPz->at(j));	


	}
}



void ReducedBaseSkimmer::SkimGenJets(){
	int nJets = _base->nJets;
	_genJetHists[0]->Fill(nJets);
	for(int j = 0; j < nJets; j++){
		//if a jet was not dR matched within dR = 0.3, default values are stored
		//these are vectors
		//_genJetHists[1]->Fill(_base->jetGenEnergy->at(j));	
		//_genJetHists[2]->Fill(_base->jetGenPt->at(j));
		//_genJetHists[3]->Fill(_base->jenGenEta->at(j));
		//_genJetHists[4]->Fill(_base->jetGenPhi->at(j));
		//_genJetHists[5]->Fill(_base->jetGenTOF->at(j));
		//_genJetHists[6]->Fill(_base->jetGenDrMatch->at(j));
		//_genJetHists[7]->Fill(_base->jetGenTimeVar->at(j));
		//_genJetHists[8]->Fill(_base->jetGenTimeLLP->at(j));
		//_genJetHists[9]->Fill(_base->jetGenNKids->at(j));
		


	}


}



void ReducedBaseSkimmer::SkimRecHits(){
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


void ReducedBaseSkimmer::SkimVertices(){
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




















