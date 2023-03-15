#ifndef JetAnalyzerLeadRH_HH
#define JetAnalyzerLeadRH_HH

#include "JetAnalyzerBase.hh"

class JetAnalyzerLeadRH : public JetAnalyzerBase{
	public:
		JetAnalyzerLeadRH(){ };
		JetAnalyzerLeadRH(ReducedBase* base);
		virtual ~JetAnalyzerLeadRH();

		double CalcTime(vector<unsigned int> rhIDs);
		double CalcDistToPV(vector<unsigned int> rhIDs);
		






};
#endif	
