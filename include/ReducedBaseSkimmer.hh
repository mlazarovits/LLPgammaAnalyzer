#ifndef ReducedBaseSkimmer_HH
#define ReducedBaseSkimmer_HH

#include <vector>
#include "ReducedBase.hh"

using std::vector;

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
		vector<TH1D*> _jetHists; //PF jets
		vector<TH1D*> _genJetHists;
		vector<TH1D*> _caloJetHists; //really only need either PF jets or calo jets (not both)
		vector<TH1D*> _vertexHists; //first vertex is PV
		vector<TH1D*> _recHitHists;		
		

};


#endif
