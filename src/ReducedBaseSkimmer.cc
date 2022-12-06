#include "ReducedBaseSkimmer.hh"




ReducedBaseSkimmer::ReducedBaseSkimmer(ReducedBase* base){ 
	_base = base;

}



ReducedBaseSkimmer::~ReducedBaseSkimmer(){ }




void ReducedBaseSkimmer::SkimJets(){
	int nJets = _base->nJets;
	_jetHists[0]->Fill(nJets);
	for(int j = 0; j < nJets; j++){
		//energy, spatial variables
		_jetHists[1]->Fill(_base->jetE[j]);
		_jetHists[2]->Fill(_base->jetPt[j]);
		_jetHists[3]->Fill(_base->jetEta[j]);
		_jetHists[4]->Fill(_base->jetPhi[j]);
		//BC variables
		_jetHists[6]->Fill(_base->jetBcTimesCnt[j]); //number of basic cluster (BC) mean times
		_jetHists[7]->Fill(_base->jetBcSumRHEnr[j]); //total BC energy in jet
		_jetHists[8]->Fill(_base->jetBcEMFr[j]); //total BC energy divided by jet energy
		_jetHists[9]->Fill(_base->jetBcRhCnt[j]);
		_jetHists[10]->Fill(_base->jetID[j]);
			
		
	}


}

void ReducedBaseSkimmer::SkimCaloJets(){ 
	int nCJets = base->nCaloJets;
	_caloJetHists[0]->Fill(nCJets);
	for(int j = 0; j < nCJets; j++){
		_caloJetHists[1]->Fill(_base->cljBcCnt[j]);
		_caloJetHists[2]->Fill(_base->cljSeedTOFTime[j]);
		_caloJetHists[3]->Fill(_base->cljCMeanTime[j]);
		_caloJetHists[4]->Fill(_base->cljBc3dEx[j]);
		_caloJetHists[5]->Fill(_base->cljBc3dEy[j]);
		_caloJetHists[6]->Fill(_base->cljBc3dEz[j]);
		_caloJetHists[7]->Fill(_base->cljBc3dEv[j]);
		_caloJetHists[8]->Fill(_base->cljBc3dEslope[j]);
		_caloJetHists[9]->Fill(_base->cljBc3dEchisp[j]);
		_caloJetHists[10]->Fill(_base->cljBc2dEx[j]);
		_caloJetHists[11]->Fill(_base->cljBc2dEy[j]);
		_caloJetHists[12]->Fill(_base->cljBc2dEv[j]);
		_caloJetHists[13]->Fill(_base->cljBc2dEslope[j]);
		_caloJetHists[14]->Fill(_base->cljBc2dEchisp[j]);
		_caloJetHists[15]->Fill(_base->cljCDrMeanTime[j]);
		_caloJetHists[16]->Fill(_base->cljPt[j]);
		_caloJetHists[17]->Fill(_base->cljEnergy[j]);
		_caloJetHists[18]->Fill(_base->cljPhi[j]);
		_caloJetHists[19]->Fill(_base->cljEta[j]);
		_caloJetHists[20]->Fill(_base->cljPx[j]);
		_caloJetHists[21]->Fill(_base->cljPy[j]);
		_caloJetHists[22]->Fill(_base->cljPz[j]);	


	}
}



void ReducedBaseSkimmer::SkimGenJets(){
	int nJets = _base->nJets;
	_genJetHists[0]->Fill(nJets);
	for(int j = 0; j < nJets; j++){
		//if a jet was not dR matched within dR = 0.3, default values are stored
		_genJetHists[1]->Fill(_base->jetGenEnergy[j]);	
		_genJetHists[2]->Fill(_base->jetGenPt[j]);
		_genJetHists[3]->Fill(_base->jenGenEta[j]);
		_genJetHists[4]->Fill(_base->jetGenPhi[j]);
		_genJetHists[5]->Fill(_base->jetGenTOF[j]);
		_genJetHists[6]->Fill(_base->jetGenDrMatch[j]);
		_genJetHists[7]->Fill(_base->jetGenTimeVar[j]);
		_genJetHists[8]->Fill(_base->jetGenTimeLLP[j]);
		_genJetHists[9]->Fill(_base->jetGenNKids[j]);
		


	}


}



void ReducedBaseSkimmer::SkimRecHits(){
	int nRHs = _base->nRecHits;
	_recHitHists[0]->Fill(nRHs);
	for(int r = 0; r < nRHs; r++){
		_recHitHists[1]->Fill(_base->rhPosX[r]);
		_recHitHists[2]->Fill(_base->rhPosY[r]);
		_recHitHists[3]->Fill(_base->rhPosZ[r]);
		_recHitHists[4]->Fill(_base->rhPosEta[r]);
		_recHitHists[5]->Fill(_base->rhPosPhi[r]);
		_recHitHists[6]->Fill(_base->rhEnergy[r]);
		_recHitHists[7]->Fill(_base->rhTime[r]);
		_recHitHists[8]->Fill(_base->rhTimeErr[r]);
		_recHitHists[9]->Fill(_base->rhTOF[r]);
		








}


void ReducedBaseSkimmer::SkimVertices(){
	int nVtx = _base->nVtx;
	_vertexHists[1]->Fill(nVtx);
	for(int v = 0; v < nVtx; v++){
		//first vertex is primary vertex
		_vertexHists[1]->Fill(_base->vtxX[v]);
		_vertexHists[2]->Fill(_base->vtxY[v]);
		_vertexHists[3]->Fill(_base->vtxZ[v]);
	}



}




















