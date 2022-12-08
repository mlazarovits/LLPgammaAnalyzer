#ifndef ReducedBaseSkimmer_HH
#define ReducedBaseSkimmer_HH

#include "ReducedBase.hh"

class ReducedBaseSkimmer{
	public:
		ReducedBaseSkimmer(ReducedBase* base);
		virtual ~ReducedBaseSkimmer();
		void SkimJets();
		void SkimCaloJets();
		void SkimGenJets();
		void SkimRecHits();
		void SkimVertices();


	private:
		ReducedBase* _base;
		TH1D* _jetHists[100]; //PF jets
		TH1D* _genJetHists[100];
		TH1D* _caloJetHists[23]; //really only need either PF jets or calo jets (not both)
		TH1D* _vertexHists[4]; //first vertex is PV
		TH1D* _recHitHists[10];		
		

};


#endif
