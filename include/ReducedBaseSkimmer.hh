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

	private:
		ReducedBase* _base;



};


#endif
