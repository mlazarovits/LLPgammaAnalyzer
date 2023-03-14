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
		void _SkimRecHits();
		void _SkimVertices();
		//void _SkimGenJets();
		void _MakePVTimes();
		double _MakeJetTime(int j);
		double _MakeJetCentroid(int j);
	
		//wraps phi around pi	
		double _deltaPhi(double phi1, double phi2){
			double x = phi1 - phi2;
			double o2pi = 1./(2*_pi);
			if(fabs(x) <= _pi)
				return x;
			double n = std::round(x * o2pi);
			return x - n * double(2.* _pi);
		};

		double _pi = 3.14159265358979323846;
		double _c = 29.9792458; //speed of light in cm/ns
};


#endif
