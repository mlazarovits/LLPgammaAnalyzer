#ifndef JetAnalyzerBase_HH
#define JetAnalyzerBase_HH
#include <cmath>
#include <vector>

using std::vector;
class ReducedBase;

//needs to be wrapped in skimmer class - this is for one event
class JetAnalyzerBase{
	public:
		JetAnalyzerBase(){ };
		JetAnalyzerBase(ReducedBase* base);
		virtual ~JetAnalyzerBase();

		void Analyze_BackToBack();

		//time calculation methods
		virtual double CalcTime(vector<unsigned int> rhIDs) = 0;

		//centroid calculation methods
		virtual double CalcDistToPV(vector<unsigned int> rhIDs) = 0;
	
		double CalcPVTime(vector<unsigned int> rhIDs);	
		//get RecHit ids for given jet via jet number
		vector<unsigned int> GetRhIDs(int j);
		vector<double> GetJetTimes(){ return _jetTimes; }
		vector<double> GetPVTimes(){ return _pvTimes; }
		vector<double> GetPVTimeDiffs(){ return _pvTimeDiffs; }
		vector<double> GetJetDistsToPV(){ return _jetDistsToPV; }
		vector<double> GetDeltaPhi(){ return _dphi; }
		//wraps phi around pi
		double _deltaPhi(double phi1, double phi2){
			double x = phi1 - phi2;
			double o2pi = 1./(2*_pi);
			if(fabs(x) <= _pi)
				return x;
			double n = std::round(x * o2pi);
			return x - n * double(2.* _pi);

		};
	protected:
		ReducedBase* _base;
		int _nJets;
		vector<double> _jetTimes;
		vector<double> _pvTimes;
		vector<double> _jetDistsToPV;
		vector<double> _pvTimeDiffs;
		vector<double> _dphi;
		double _c = 29.9792458; //speed of light in cm/ns
		double _pi = 3.14159265358979323846;


};
#endif
