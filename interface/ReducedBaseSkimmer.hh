#ifndef ReducedBaseSkimmer_HH
#define ReducedBaseSkimmer_HH
#include <TChain.h>
#include "ReducedBase.hh"

class ReducedBaseSkimmer{
	public:
		ReducedBaseSkimmer(TChain* ch);
		virtual ~ReducedBaseSkimmer();
		vector<TH1D*> Skim();

	private:
		TChain* _ch;
		ReducedBase* _base;
		vector<TH1D*> _jetHists; //PF jets
		vector<TH1D*> _genJetHists;
		vector<TH1D*> _vertexHists; //first vertex is PV
		vector<TH1D*> _recHitHists;		
		vector<TH1D*> _pvTimeHists;		
		void _SkimJets();
		void _SkimGenJets();
		void _SkimRecHits();
		void _SkimVertices();
		void _MakePVTimes();
		void _MakeJetGeo(int j);

		vector<double> _jetTimes;
		vector<double> _jetLengths;
};


#endif
