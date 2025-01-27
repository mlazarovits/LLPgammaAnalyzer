// -*- C++ -*-
//
// Package:    LLPgammaAnalyzer_MINI
// Class:      LLPgammaAnalyzer_MINI
//
/**\class LLPgammaAnalyzer_MINI LLPgammaAnalyzer_MINI.cc LLPgammaAnalyzer_MINI/plugins/LLPgammaAnalyzer_MINI.cc

 Description: [one line class summary]

 Implementation:
     [Notes on implementation]
*/
//
// Original Author:  Jack W King III
//         Created:  Wed, 27 Jan 2021 19:19:35 GMT
//
//

//--------------------   hh file -------------------------------------------------------------
//---------------------------------------------------------------------------------------------

// system include files
#include <memory>

// basic C++ types
#include <string>
#include <iostream>
#include <fstream>
#include <sstream>
#include <cstdlib>
#include <vector>
#include <utility>
#include <map>
#include <unordered_map>
#include <cmath>
#include <limits>
#include <algorithm>
#include <tuple>
#include <random>
#include <sys/stat.h>

// user include files
#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/one/EDAnalyzer.h"

#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/MakerMacros.h"

#include "FWCore/Framework/interface/ESHandle.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "FWCore/Utilities/interface/InputTag.h"
#include "FWCore/ServiceRegistry/interface/Service.h"
#include "CommonTools/UtilAlgos/interface/TFileService.h"

// HLT + Trigger info
#include "FWCore/Common/interface/TriggerNames.h"
#include "DataFormats/Common/interface/TriggerResults.h"
#include "DataFormats/HLTReco/interface/TriggerObject.h"

// Gen Info
#include "SimDataFormats/PileupSummaryInfo/interface/PileupSummaryInfo.h"
#include "SimDataFormats/GeneratorProducts/interface/GenEventInfoProduct.h"
#include "DataFormats/HepMCCandidate/interface/GenParticle.h"
#include "DataFormats/JetReco/interface/GenJet.h"

// DataFormats
#include "DataFormats/PatCandidates/interface/PackedCandidate.h"
#include "DataFormats/ParticleFlowCandidate/interface/PFCandidate.h"
#include "DataFormats/Candidate/interface/CandidateFwd.h"

#include "DataFormats/Common/interface/ValueMap.h"
#include "DataFormats/Common/interface/View.h"
#include "DataFormats/Common/interface/Handle.h"

#include "DataFormats/TrackReco/interface/Track.h"
#include "DataFormats/TrackReco/interface/TrackFwd.h"

#include "DataFormats/VertexReco/interface/Vertex.h"

#include "DataFormats/PatCandidates/interface/Photon.h"
#include "DataFormats/PatCandidates/interface/MET.h"
#include "DataFormats/PatCandidates/interface/Jet.h"
#include "DataFormats/PatCandidates/interface/Electron.h"
#include "DataFormats/PatCandidates/interface/Muon.h"
#include "DataFormats/MuonReco/interface/MuonSelectors.h"

// DetIds 
#include "DataFormats/DetId/interface/DetId.h"
#include "DataFormats/EcalDetId/interface/EBDetId.h"
#include "DataFormats/EcalDetId/interface/EEDetId.h"
#include "DataFormats/EcalDetId/interface/EcalSubdetector.h"

// Ecal RecHits
#include "DataFormats/EcalRecHit/interface/EcalRecHit.h"
#include "DataFormats/EcalRecHit/interface/EcalRecHitCollections.h"
#include "DataFormats/EcalRecHit/interface/EcalUncalibratedRecHit.h"

// Supercluster info
#include "DataFormats/EgammaReco/interface/SuperCluster.h"
#include "DataFormats/CaloRecHit/interface/CaloCluster.h"
#include "DataFormats/EgammaReco/interface/SuperClusterFwd.h"

// EGamma Tools
#include "RecoEcal/EgammaCoreTools/interface/EcalTools.h"
#include "RecoEcal/EgammaCoreTools/interface/EcalClusterTools.h"
#include "RecoEcal/EgammaCoreTools/interface/EcalClusterLazyTools.h"

// Geometry
#include "Geometry/Records/interface/CaloGeometryRecord.h"
#include "Geometry/CaloGeometry/interface/CaloGeometry.h"
#include "Geometry/CaloGeometry/interface/CaloSubdetectorGeometry.h"
#include "Geometry/CaloGeometry/interface/CaloCellGeometry.h"
#include "DataFormats/GeometryVector/interface/GlobalPoint.h"

// ECAL Record info (Laser Constants)
#include "CalibCalorimetry/EcalLaserCorrection/interface/EcalLaserDbService.h"
#include "CalibCalorimetry/EcalLaserCorrection/interface/EcalLaserDbRecord.h"

// ECAL Record info (Intercalibration Constants)
#include "CondFormats/EcalObjects/interface/EcalIntercalibConstants.h"
#include "CondFormats/DataRecord/interface/EcalIntercalibConstantsRcd.h"

// ECAL Record info (ADCToGeV)
#include "CondFormats/EcalObjects/interface/EcalADCToGeVConstant.h"
#include "CondFormats/DataRecord/interface/EcalADCToGeVConstantRcd.h"

// ECAL Record info (ADCToGeV)
#include "CondFormats/EcalObjects/interface/EcalADCToGeVConstant.h"
#include "CondFormats/DataRecord/interface/EcalADCToGeVConstantRcd.h"

// ECAL Record info (Pedestals)
#include "CondFormats/EcalObjects/interface/EcalPedestals.h"
#include "CondFormats/DataRecord/interface/EcalPedestalsRcd.h"

// JECS
#include "CondFormats/JetMETObjects/interface/JetCorrectorParameters.h"
#include "JetMETCorrections/Objects/interface/JetCorrectionsRecord.h"
#include "CondFormats/JetMETObjects/interface/JetCorrectionUncertainty.h"

// JERs
#include "CondFormats/JetMETObjects/interface/JetResolutionObject.h"
#include "JetMETCorrections/Modules/interface/JetResolution.h"

// TOOLS
#include "DataFormats/Math/interface/deltaR.h"
#include "DataFormats/Common/interface/RefToPtr.h"

// ROOT
#include "TH1.h"
#include "TH2.h"
#include "TFormula.h"
#include "TF1.h"
#include "TTree.h"
#include "Math/PositionVector3D.h"
#include "TMatrixD.h"
#include "TVectorD.h"
#include "TMatrixDSymEigen.h"
#include "TGraph.h"
#include "TMathBase.h"

using namespace std;
using namespace edm;

//
// In class declaration :
// If the analyzer does not use TFileService, please remove
// the template argument to the base class so the class inherits
// from  edm::one::EDAnalyzer<>
// This will improve performance in multithreaded jobs.
//


using reco::TrackCollection;

//
//  constants, enums and typedefs
//

typedef edm::View<reco::Candidate> CandidateView;
typedef edm::SortedCollection<EcalRecHit,edm::StrictWeakOrdering<EcalRecHit>> recHitCol;
typedef vector<EcalRecHit> rhGroup;
typedef vector<reco::SuperCluster> scGroup;
typedef vector<reco::CaloCluster> bcGroup;
typedef unsigned int uInt;

#define nEBEEMaps 32
#define nHists 256
//const float sol = 29.9792458; // speed of light in cm/ns
#define SOL 29.9792458 // speed of light in cm/ns
#define PI 3.1415926535 // pie ...  

enum class ECAL {EB, EM, EP, EE, NONE};
#define ecal_config_path "/uscms/home/jaking/nobackup/llpa/CMSSW_10_6_20/src/LLPGamma/LLPgammaAnalyzer_MINI/macros/ecal_config/"
//#define ecal_config_path "/LLPGamma/LLPgammaAnalyzer_MINI/macros/ecal_config/"

struct DetIDStruct{
	DetIDStruct() {}
   	DetIDStruct(const int inI1, const int inI2, const int inTT, const ECAL & inEcal) : i1(inI1), i2(inI2), TT(inTT), ecal(inEcal)  {}
   	int i1; // EB: iphi, EE: ix
   	int i2; // EB: ieta, EE: iy
   	int TT; // trigger tower
   	ECAL ecal; // EB, EM, EP
};//>>>>struct DetIDStruct Def
typedef std::map<UInt_t,DetIDStruct> detIdMap;

//
//  Class Declaration
//

class LLPgammaAnalyzer_MINI : public edm::one::EDAnalyzer<edm::one::SharedResources>  {

	public:

    	explicit LLPgammaAnalyzer_MINI(const edm::ParameterSet&);
      	~LLPgammaAnalyzer_MINI();

      	static void fillDescriptions(edm::ConfigurationDescriptions& descriptions);

		detIdMap SetupDetIDs();

      	int getPFJetID(const pat::Jet & jet);

      	rhGroup getRHGroup( float eta, float phi, float drmin, float minenr );
      	rhGroup getRHGroup( const scGroup superClusterGroup, float minenr );
 		rhGroup getRHGroup( const scGroup superClusterGroup, float minenr, TH1D* rhTimeHist, TH1D* ootHist, TH1D* matchHist );
		rhGroup getRHGroup( const scGroup superClusterGroup, float minenr, vector<float> phEnergy, vector<float> phDr, float phEnMax );
        rhGroup getRHGroup( const scGroup superClusterGroup, float minenr, vector<float> phEnergy, vector<float> phDr, float phEnMax, TH1D* rhTimeHist, TH1D* ootHist, TH1D* matchHist );
      	rhGroup getRHGroup( const reco::CaloCluster basicCluster, float minenr );
      	rhGroup getRHGroup( uInt detid );
      	rhGroup getRHGroup();

      	EcalRecHit getLeadRh( rhGroup recHits );
      	vector<float> getRhTofTime( rhGroup recHits, double vtxX, double vtxY, double vtxZ );
      	vector<float> getLeadTofRhTime( rhGroup recHits, double vtxX, double vtxY, double vtxZ );
      	vector<float> getTimeDistStats( vector<float> input );
      	vector<float> getTimeDistStats( vector<float> times, vector<float> wts );
      	vector<float> getTimeDistStats( vector<float> input, rhGroup rechits );

		vector<float> getRhGrpEigen_xyz( vector<float> times, rhGroup rechits );
        vector<float> getRhGrpEigen_ep( vector<float> times, rhGroup rechits );
        vector<float> getRhGrpEigen_ieipt( vector<float> times, rhGroup rechits );
		vector<float> getRhGrpEigen_sph( vector<float> times, rhGroup rechits );
        vector<float> getRhGrpEigen( vector<float> xs, vector<float> ys, vector<float> zs, vector<float> wts );
        vector<float> getRhGrpEigen( vector<float> xs, vector<float> ys, vector<float> wts );
		vector<float> getRhGrpEigen( vector<float> xs, vector<float> wts );

		vector<float> kidTOFChain( std::vector<reco::CandidatePtr> kids, float cx, float cy, float cz  );

        float getRhTOF( EcalRecHit rechit, double vtxX, double vtxY, double vtxZ );
        float getLeadTofTime( rhGroup recHits, double vtxX, double vtxY, double vtxZ );
		float getSeedTofTime( reco::SuperCluster sprclstr, double vtxX, double vtxY, double vtxZ );
      	float getdt( float t1, float t2 );
      	void mrgRhGrp( rhGroup & x, rhGroup & y);
      	bool reduceRhGrps( vector<rhGroup> & x);

	private:

    	virtual void beginJob() override;
      	virtual void analyze(const edm::Event&, const edm::EventSetup&) override;
      	virtual void endJob() override;

      	// ----------member data ---------------------------

		// oputput tree
      	TTree *outTree;

		// histograms
 //     	TH1D *jetTimeHist, *jetRHTimeHist;
 //       	TH1D *hist1d[nHists];
 //     	TH2D *hist2d[nHists];

 //     	TH2D *ebeeMapSc[nEBEEMaps];
 //     	TH2D *ebeeMapBc[nEBEEMaps];
 //     	TH2D *ebeeMapDr[nEBEEMaps];

 //     	TH2D *ebeeMapT[nEBEEMaps];
 //     	TH2D *ebeeMapE[nEBEEMaps];
		
		// Flags
		const bool hasGenInfo;

      	// Event
      	unsigned long int event; // technically unsigned long long in Event.h...
      	uInt run, lumi; 

      	// Tracks
      	const edm::InputTag tracksTag;
      	edm::EDGetTokenT<TrackCollection> tracksToken_;  //used to select what tracks to read from configuration file
      	edm::Handle<std::vector<reco::Track>> tracks_;

      	// PF Candidates
      	const edm::InputTag pfcandTag;
      	edm::EDGetTokenT<edm::View<reco::Candidate>> pfcand_token_;
      	edm::Handle<edm::View<reco::Candidate>> pfcands_;

      	// vertices
      	const edm::InputTag verticesTag;
      	edm::EDGetTokenT<std::vector<reco::Vertex>> verticesToken_;
      	edm::Handle<std::vector<reco::Vertex>> vertices_;
      	int nVtx;
      	float vtxX, vtxY, vtxZ;

      	// mets
      	const edm::InputTag metsTag;
      	edm::EDGetTokenT<std::vector<pat::MET>> metsToken_;
      	edm::Handle<std::vector<pat::MET>> mets_;

	   	// supercluster
      	const edm::InputTag superClusterCollectionTag;
      	edm::EDGetTokenT<reco::SuperClusterCollection> scToken_;
      	edm::Handle<reco::SuperClusterCollection> superCluster_;

      	const edm::InputTag ootSuperClusterCollectionTag;
     	edm::EDGetTokenT<reco::SuperClusterCollection> ootScToken_;
      	edm::Handle<reco::SuperClusterCollection> ootSuperCluster_;

		// calocluster
      	const edm::InputTag caloClusterTag;
      	edm::EDGetTokenT<vector<reco::CaloCluster>> ccToken_;
      	edm::Handle<vector<reco::CaloCluster>> caloCluster_;

      	// jets
      	const edm::InputTag jetsTag;
      	edm::EDGetTokenT<std::vector<pat::Jet>> jetsToken_;
      	edm::Handle<std::vector<pat::Jet>> jets_;

      	uInt nJets;
        uInt nGoodJetEvents;
      	std::vector<float>  jetE, jetPt, jetPhi, jetEta; 
        std::vector<float>  jetEtaetaMmt, jetPhiphiMnt, jetEtaphiMnt, jetMaxD, jetConPtDis, jetConEtaPhiSprd, jetArea;
        std::vector<uInt>   jetNCarry, jetNConst; 
      	std::vector<float>  jetMuTime, jetTimeError, jetTimeRMS, jetMedTime, jetCMuTime, jetCMedTime;
      	std::vector<float>  jetSCMuTime, jetSCMedTime, jetCSCMuTime, jetCSCMedTime, jetCBCMuTime, jetCBCMedTime;
      	std::vector<float>  jetPhMuTime, jetOOTPhMuTime, jetEleMuTime;
      	std::vector<int>    jetID, njetKids, jetKidOfJet, njetSubs, njetRecHits, jetRecHitOfJet;
      	std::vector<int>    jetKidPdgID, jetKidCharge, jetKid3Charge, jetPHM, jetELM;
      	std::vector<uInt>   jetRecHitId;
      	std::vector<bool>   jetKidLLP;
      	std::vector<double> jetKidMass, jetKidVx, jetKidVy, jetKidVz;
      	std::vector<float>  jetKidE, jetKidPt, jetKidPhi, jetKidEta, jetKidTime, jetKidMedTime;
      	std::vector<float>  jetNHF, jetNEMF, jetCHF, jetCEMF, jetMUF, jetNHM, jetCHM, jetC, jetPHE, jetPHEF;
      	std::vector<float>  jetELE, jetELEF, jetMUE, jetCharge;

      	// electrons
      	const edm::InputTag electronsTag;
      	edm::EDGetTokenT<std::vector<pat::Electron> > electronsToken_;
      	edm::Handle<std::vector<pat::Electron> > electrons_;
      	std::vector<pat::Electron> electrons;
      
      	// muons
      	const edm::InputTag muonsTag;
      	edm::EDGetTokenT<std::vector<pat::Muon> > muonsToken_;
      	edm::Handle<std::vector<pat::Muon> > muons_;
      	std::vector<pat::Muon> muons;
      
      	// RecHits
      	const edm::InputTag recHitsEBTag;
      	edm::EDGetTokenT<edm::SortedCollection<EcalRecHit,edm::StrictWeakOrdering<EcalRecHit> > > recHitsEBToken_;
      	edm::Handle<edm::SortedCollection<EcalRecHit,edm::StrictWeakOrdering<EcalRecHit> > > recHitsEB_;
      	const edm::SortedCollection<EcalRecHit,edm::StrictWeakOrdering<EcalRecHit> > * recHitsEB;

      	const edm::InputTag recHitsEETag;
      	edm::EDGetTokenT<edm::SortedCollection<EcalRecHit,edm::StrictWeakOrdering<EcalRecHit> > > recHitsEEToken_;
      	edm::Handle<edm::SortedCollection<EcalRecHit,edm::StrictWeakOrdering<EcalRecHit> > > recHitsEE_;
      	const edm::SortedCollection<EcalRecHit,edm::StrictWeakOrdering<EcalRecHit> > * recHitsEE;

        detIdMap DetIDMap;
      	int nRecHits;
      	std::vector<float> rhPosX, rhPosY, rhPosZ, rhEnergy, rhTime, rhTimeErr, rhTOF, rhPosEta, rhPosPhi, rh_distToPV;
      	std::vector<uInt> rhID;

      	// gedPhotons
      	const edm::InputTag gedPhotonsTag;
      	edm::EDGetTokenT<std::vector<pat::Photon> > gedPhotonsToken_;
      	edm::Handle<std::vector<pat::Photon> > gedPhotons_;

      	// ootPhotons
      	const edm::InputTag ootPhotonsTag;
      	edm::EDGetTokenT<std::vector<pat::Photon> > ootPhotonsToken_;
      	edm::Handle<std::vector<pat::Photon> > ootPhotons_;

     
      	// geometry CaloSubdetectorGeometry
      	edm::ESHandle<CaloGeometry> caloGeo_;
      	const CaloSubdetectorGeometry * barrelGeometry;
      	const CaloSubdetectorGeometry * endcapGeometry;  

};//<<>>class LLPgammaAnalyzer_MINI : public edm::one::EDAnalyzer<edm::one::SharedResources>  {

//
// Helper functions ( single line function defs, mostly )
//

//
// The "crystalball" function for ROOT 5.x (mimics ROOT 6.x).
//
// Create the "crystalball" TF1 somewhere in your source code using:
// double xmin = 3., xmax = 8.; // whatever you need
// TF1 *crystalball = new TF1("crystalball", crystalball_function, xmin, xmax, 5);
// crystalball->SetParNames("Constant", "Mean", "Sigma", "Alpha", "N");
// crystalball->SetTitle("crystalball"); // not strictly necessary
//

// #include "TMath.h"
#include <cmath>

// see math/mathcore/src/PdfFuncMathCore.cxx in ROOT 6.x
double crystalball_function(double x, double alpha, double n, double sigma, double mean) {

  	// evaluate the crystal ball function
  	if (sigma < 0.)     return 0.;
  	double z = (x - mean)/sigma; 
  	if (alpha < 0) z = -z; 
  	double abs_alpha = std::abs(alpha);
  	// double C = n/abs_alpha * 1./(n-1.) * std::exp(-alpha*alpha/2.);
  	// double D = std::sqrt(M_PI/2.)*(1.+ROOT::Math::erf(abs_alpha/std::sqrt(2.)));
  	// double N = 1./(sigma*(C+D));
  	if (z  > - abs_alpha) return std::exp(- 0.5 * z * z);
  	else {
    	//double A = std::pow(n/abs_alpha,n) * std::exp(-0.5*abs_alpha*abs_alpha);
    	double nDivAlpha = n/abs_alpha;
    	double AA =  std::exp(-0.5*abs_alpha*abs_alpha);
    	double B = nDivAlpha -abs_alpha;
    	double arg = nDivAlpha/(B-z);
    	return AA * std::pow(arg,n);
  	}//<<>> if (z  > - abs_alpha)

}//<<>>double crystalball_function(double x, double alpha, double n, double sigma, double mean)

double crystalball_function(const double *x, const double *p) {

  	// if ((!x) || (!p)) return 0.; // just a precaution
  	// [Constant] * ROOT::Math::crystalball_function(x, [Alpha], [N], [Sigma], [Mean])
  	return (p[0] * crystalball_function(x[0], p[3], p[4], p[2], p[1]));

}//<<>>double crystalball_function(const double *x, const double *p)

double twosided_crystalball_function(double x, double alphalow, double alphahigh, double nlow, double nhigh, double sigma, double mean) {

  	// evaluate the crystal ball function
  	if(sigma < 0.) return 0.;
  	auto z = (x - mean)/sigma;
  	auto abs_alphalow = std::abs(alphalow);
  	auto abs_alphahigh = std::abs(alphahigh);
  	if( z  > abs_alphalow and z  < abs_alphahigh ) return std::exp(- 0.5 * z * z);
  	else if( z  > abs_alphahigh ){
    	double nDivAlpha = nhigh/abs_alphahigh;
    	double AA =  std::exp(-0.5*abs_alphahigh*abs_alphahigh);
    	double B = nDivAlpha -abs_alphahigh;
    	double arg = nDivAlpha/(B-z);
    	return AA * std::pow(arg,nhigh);
  	} else {
    	double nDivAlpha = nlow/abs_alphalow;
    	double AA =  std::exp(-0.5*abs_alphalow*abs_alphalow);
    	double B = nDivAlpha -abs_alphalow;
    	double arg = nDivAlpha/(B-z);
    	return AA * std::pow(arg,nlow);
  	}//<<>>if( z  > abs_alphalow and z  < abs_alphahigh )

}//<<>>double double_crystalball_function(double x, double alphalow, double alphahigh, double nlow, double nhigh, double sigma, double mean)

double twosided_crystalball_function(const double *x, const double *p) {

	return (p[0] * twosided_crystalball_function(x[0], p[3], p[4], p[5], p[6], p[2], p[1]));

}//<<>>double double_crystalball_function(const double *x, const double *p)

void fillTH1( float val, TH1F* hist ){

   	auto nBins = hist->GetNbinsX();
   	auto low = hist->GetBinCenter(1);
   	auto high = hist->GetBinCenter(nBins);
   	if( val < low ) hist->Fill( low );
   	else if ( val > high ) hist->Fill( high );
   	else hist->Fill( val );

}//<<>>void fillTH1( float val, TH1F* hist )

void fillTH1( float val, TH1D* hist ){

   	auto nBins = hist->GetNbinsX();
   	auto low = hist->GetBinCenter(1);
   	auto high = hist->GetBinCenter(nBins);
   	if( val < low ) hist->Fill( low );
   	else if ( val > high ) hist->Fill( high );
   	else hist->Fill( val );

}//<<>>void fillTH1( float val, TH1D* hist )

void normTH2D(TH2D* hist){

    std::cout << "Normilizing " << " hist: " << hist->GetName() << std::endl;

	const auto nXbins = hist->GetNbinsX();
    const auto nYbins = hist->GetNbinsY();

    for (auto ibinX = 1; ibinX <= nXbins; ibinX++){

        const auto norm = hist->Integral(ibinX,ibinX,1,nYbins);
        if( norm == 0.0 ) continue;
        for (auto ibinY = 1; ibinY <= nYbins; ibinY++){

            // get content/error
            auto content = hist->GetBinContent(ibinX,ibinY);
            auto error   = hist->GetBinError  (ibinX,ibinY);
            // set new contents
            content /= norm;
            error /= norm;
            hist->SetBinContent(ibinX,ibinY,content);
            hist->SetBinError  (ibinX,ibinY,error);

        }//<<>>for (auto ibinY = 1; ibinY <= nXbins; ibinY++){
	}//<<>>for (auto ibinX = 1; ibinX <+ nYbins; ibinX++){

}//<<>>void NormTH2D(TH2D* hist){

void normTH1D(TH1D* hist){

    std::cout << "Normilizing " << " hist: " << hist->GetName() << std::endl;

    const auto nBins = hist->GetNbinsX();
    const auto norm = hist->Integral();
    for (auto ibinX = 1; ibinX <= nBins; ibinX++){

        if( norm == 0.0 ) continue;
		// get content/error
		auto content = hist->GetBinContent(ibinX);
		auto error   = hist->GetBinError(ibinX);
		// set new contents
        content /= norm;
		error /= norm;
		hist->SetBinContent(ibinX,content);
		hist->SetBinError  (ibinX,error);

	}//<<>>for (auto ibinX = 1; ibinX <= nBins; ibinX++)

}//<<>>void NormTH1D(TH1D* hist)

void profileTH2D(TH2D* nhist, TH1D* prof, TH1D* fithist){

    std::cout << "Profile " << " hist: " << nhist->GetName() << std::endl;

    const auto nXBins = nhist->GetNbinsX();
    //const auto nYBins = nhist->GetNbinsY();
    for (auto ibinX = 1; ibinX <= nXBins; ibinX++){

		auto phist = (TH1F*)nhist->ProjectionY("temp",ibinX,ibinX);
		//double error;
        //auto content = hist->IntegralAndError(ibinX,ibinX,1,nYBins,error);

        auto mean = phist->GetMean();
        //auto mean = 0.f;
        auto stdv = phist->GetStdDev();
		//auto error = phist->GetMeanError();
		auto norm = phist->GetBinContent(phist->GetMaximumBin());
		auto high = mean + 0.2*stdv;
        //auto high = 0.2;
        auto low = mean - 0.2*stdv;
        //auto low = -0.2;
		//std::cout << " - Profile: m " << mean << " s " << stdv << " h " << high << " l " << low << " n " << norm << std::endl;
		if( abs(stdv) > 0.01 && abs(norm) > 1 ){
			auto tmp_form = new TFormula("tmp_formula","[0]*exp(-0.5*((x-[1])/[2])**2)");
			auto tmp_fit  = new TF1("tmp_fit",tmp_form->GetName(),low,high);
            //auto tmp_fit  = new TF1("tmp_fit","crystalball",low,high);
			//auto tmp_fit = new TF1("crystalball", twosided_crystalball_function, low, high, 7);
            //auto tmp_fit  = new TF1("tmp_fit","gaus",high,low);
    		tmp_fit->SetParameter(0,norm); //tmp_fit->SetParLimits(0,norm/2,norm*2);
    		tmp_fit->SetParameter(1,mean); //tmp_fit->SetParLimits(1,-2,2);
    		tmp_fit->SetParameter(2,stdv); //tmp_fit->SetParLimits(2,0,10);
            //tmp_fit->SetParameter(3,0.25);
            //tmp_fit->SetParameter(4,0.25);
            //tmp_fit->SetParameter(5,1);
            //tmp_fit->SetParameter(6,1);
			phist->Fit(tmp_fit->GetName(),"RBQ0");
            //phist->Fit(tmp_fit->GetName(),"R");
			//auto fnorm = tmp_fit->GetParameter(0);
			auto fmean = tmp_fit->GetParameter(1);
            //auto fstd = tmp_fit->GetParameter(2);
			auto error = tmp_fit->GetParError(1);
            //auto fChi2 = tmp_fit->GetChisquare();
            auto fNdf = tmp_fit->GetNDF();
            auto fProb = tmp_fit->GetProb();
			//auto error = fstd/std::sqrt(fnorm);
			//std::cout << " - Profile: fm " << fmean << " fChi2 " << fProb  << " e " << error << " fNdf " << fNdf << std::endl;
	
        	// set new contents
        	if( fNdf > 0 && fProb > 0.05 && error < 1.0 ){
				//auto fChi2Ndf = fChi2/fNdf;
				fithist->SetBinContent( ibinX, fProb );
                fithist->SetBinError( ibinX, 0 );
				prof->SetBinContent( ibinX, fmean );
        		prof->SetBinError( ibinX, error );
			}//<<>>if( fmean < 1 && error < 0.1 )

			//delete tmp_form;
        	delete tmp_fit;
		}//<<>>if( stdv > 0.01 )

    }//<<>>for (auto ibinX = 1; ibinX <= nBins; ibinX++)

}//<<>>void profileTH2D(TH2D* hist, TH1D* prof)

void thresDivTH2D(TH2D* numi, TH2D* denom, float thres){

    std::cout << "Threshold Division - " << " hist: " << numi->GetName() << std::endl;

    const auto nXbins = numi->GetNbinsX();
    const auto nYbins = numi->GetNbinsY();

    for (auto ibinX = 1; ibinX <= nXbins; ibinX++){
        for (auto ibinY = 1; ibinY <= nYbins; ibinY++){
 
            // get content/error
            auto ncontent = numi->GetBinContent(ibinX,ibinY);
            auto nerror   = numi->GetBinError  (ibinX,ibinY);
            auto dcontent = denom->GetBinContent(ibinX,ibinY);
            auto derror   = denom->GetBinError  (ibinX,ibinY);
            // set new contents
            auto content(0.0);
            auto error(0.0);
			if( dcontent > thres ){ content = ncontent/dcontent; error = nerror/derror; } 
            numi->SetBinContent(ibinX,ibinY,content);
            numi->SetBinError  (ibinX,ibinY,error);
 
        }//<<>>for (auto ibinY = 1; ibinY <= nXbins; ibinY++){
    }//<<>>for (auto ibinX = 1; ibinX <+ nYbins; ibinX++){
 
}//<<>>void thresDivTH2D(TH2D* numi, TH2D* denom, float thres){

const float getMyAngle ( const float x, const float y){

	if( x == 0 && y == 0 ) return 6.39; //-999.0;
	if( x == 0 ){ if( y > 0 ) return PI/2; else return 3*PI/2; } 
    if( y == 0 ){ if( x > 0 ) return 0; else return PI; }
	float m = std::sqrt(x*x+y*y); 
	float a = std::asin(abs(y/m)); 
	if( x < 0 && y < 0 ) return PI+a; 
	if( x < 0 ) return PI-a; 
	if( y < 0 ) return 2*PI-a; 
	return a;

}//<<>> const float getAngle (CFlt x, CFlt y)

const float getAngle ( const float x, const float y){

	if( x == 0 && y == 0) return 6.39;
	auto a = std::atan2(y,x);
	if( a < 0 ) a = 2*PI+a;
	return a;

}//<<>> const float getAngle (CFlt x, CFlt y) with atan2

const float getATan2 ( const float x, const float y){

    if( x == 0 && y == 0) return 6.39;
    else return std::atan2(y,x);

}//<<>> const float getAngle (CFlt x, CFlt y) with atan2

//  --------------------------------------------------------------------------
//  ---------   move all chase functions into class   !!!!!!!!!!!!!!!!!!!!!!!!
//  --------------------------------------------------------------------------

string bigKidChase( std::vector<reco::CandidatePtr> kids, float vx ){

	std::string result("");
	float maxpt(0.0);
	const reco::Candidate* bigKid(NULL);
	for( auto kid : kids ){
		
		if( kid->pt() > maxpt ){
			maxpt = kid->pt();
			bigKid = kid.get();
		}

	}//<<>>for( auto kid : kids )
	
	result += std::string(" -- BigKid : pdgID : ") + bigKid->pdgId() + std::string(" pt : ") + bigKid->pt();
	result += std::string(" vtx : ") + bigKid->mother(0)->vx() + std::string(" / ") + vx ;
	return result;

}//<<>>string bigKidChase( std::vector<reco::CandidatePtr> kids, float vx )

void motherChase( const reco::Candidate* kid, string & depth ){

	depth += "-";
	for( long unsigned int gmit(0); gmit < kid->numberOfMothers(); gmit++ ){
        std::cout <<  depth  << " gMomID : " << kid->mother(gmit)->pdgId() << " pt : " << kid->mother(gmit)->pt(); 
		std::cout << " Vertix (" << kid->mother(gmit)->vx() << "," << kid->mother(gmit)->vy() << "," << kid->mother(gmit)->vz() << ")";
		std::cout << " nGMothers " << kid->mother(gmit)->numberOfMothers() << std::endl; 
		//if( kid->mother(gmit)->numberOfMothers() > 0 ) motherChase( kid->mother(gmit), depth );
	}//<<>>for( long unsigned int gmit(0); gmit < nKMother; gmit++ )
	if( not depth.empty() ) depth = depth.substr (0,depth.length()-1);

}//<<>> void MotherChase( Candidate* kid, string depth  )

bool llpChase( const reco::Candidate* kid ){

	bool result(false);
    for( long unsigned int gmit(0); gmit < kid->numberOfMothers(); gmit++ ){
		if( kid->pdgId() == 6000113 ) return true; 
        if( kid->mother(gmit)->numberOfMothers() > 0 ) result = llpChase( kid->mother(gmit) );
		if( result == true ) return true;
    }//<<>>for( long unsigned int gmit(0); gmit < nKMother; gmit++ )
	return result;

}//<<>> void llpChase( Candidate* kid )

void kidChase( std::vector<reco::CandidatePtr> kids, float vx, float vy, float vz ){

    for( auto kid : kids ){

		bool done(false);
		auto mom = kid->mother();
		//std::cout << " -- Kid pdgID : " << kid->pdgId() << " pt : " << kid->pt() << " PV : (" << vx << "," << vy << "," << vz << ")" << std::endl;
        std::cout << " -- Kid pdgID : " << kid->pdgId() << " p : " << kid->p() << " e : " << kid->energy() << std::endl;
		bool top(true);
		while( not done ){
			if( mom->pt() == 0 ) done = true;
			else {
 				auto gmom = mom->mother();
                if( top ){
                    std::cout << " ---- KidStep pdgID : " << mom->pdgId() << " p : " << mom->p() << " e : " << mom->energy();
                    std::cout << " vert : (" << mom->vx() << "," << mom->vy() << "," << mom->vz() << ")" << std::endl;
                }//<<>>if( top )
				if( gmom->pt() == 0 ) done = true;
				else { 
					if( mom->vx() == gmom->vx() ) top = false;
					else top = true; 		
					mom = gmom;
				}//<<>>if( gmom->pt() == 0 )
			}//<<>>if( mom->pt() == 0 )					
		}//<<>>while( not done )

		//if( llpChase( kid.get() ) ){
		//	auto gmom = mom->mother();
		//	std::cout << " ---- LLP found : " << gmom->pdgId() << " pt : " << gmom->pt();
		//	std::cout << " vert : (" << gmom->vx() << "," << gmom->vy() << "," << gmom->vz() << ")" << std::endl;
		//}//<<>>if( llpChase( kid.get() ) )

    }//<<>>for( auto kid : kids )

}//<<>>string kidChase( std::vector<reco::CandidatePtr> kids, float vx )

// sort functions

#define CAuto const auto
#define CFlt  const float
#define CDbl  const double
#define CVFlt const vector<float>
#define CVDbl const vector<double>

CAuto sortByPt = [](CAuto & obj1, CAuto & obj2) {return obj1.pt() > obj2.pt();};

// math functions
CAuto sq2			(CFlt x){return x*x;}
CAuto sq2           (CDbl x){return x*x;}
CAuto rad2  		(CFlt x, CFlt y, CFlt z = 0.f){return x*x+y*y+z*z;}
CAuto hypo  		(CFlt x, CFlt y, CFlt z = 0.f){return std::sqrt(rad2(x,y,z));}
CAuto phi   		(CFlt x, CFlt y){return std::atan2(y,x);}
CAuto theta 		(CFlt r, CFlt z){return std::atan2(r,z);}
CAuto eta   		(CFlt x, CFlt y, CFlt z){return -1.0f*std::log(std::tan(theta(hypo(x,y),z)/2.f));}
CAuto effMean   	(CFlt x, CFlt y){return (x*y)/sqrt(x*x+y*y);}
CAuto dPhi			(CFlt x, CFlt y){auto dp(x-y); if( dp > 180 ){dp-=360.0;} else if( dp < -180 ){ dp+=360.0;} return dp;}
CAuto vfsum         (CVFlt x){return std::accumulate(x.begin(),x.end(),0.0f);}
CAuto max           (CVFlt x){float m(x[0]); for(auto ix : x ){ if( ix > m ) m = ix; } return m;}

// stats functions
CAuto mean			(CVFlt x){return std::accumulate(x.begin(),x.end(),0.0f)/x.size();}
CAuto mean  		(CVFlt x, CFlt w){return std::accumulate(x.begin(),x.end(),0.0f)/w;}
CAuto mean			(CVFlt x, CVFlt wv){float sum(0.0), wt(0.0); int it(0); for( auto ix : x ){ sum+=ix*wv[it]; wt+=wv[it]; it++; } return sum/wt;}
CAuto wnum			(CFlt it, CFlt w){return (((it-1)*w)/it);}
CAuto stdev			(CVFlt x, CFlt m){float sum(0.0); for( auto ix : x ){ sum += sq2(ix-m); } return std::sqrt(sum/(x.size()-1));}	
CAuto var           (CVFlt x, CFlt m){float sum(0.0); for( auto ix : x ){ sum += sq2(ix-m); } return sum/(x.size()-1);}

CAuto stdev			(CVFlt x, CFlt m, CVFlt wv, CFlt w){
						float sum(0.0); int it(0); 
						for(auto ix : x ){ sum += wv[it]*sq2(ix-m); it++; } 
						return std::sqrt(sum/wnum(it,w));
					}//CAuto stdev

CAuto var           (CVFlt x, CFlt m, CVFlt wv, CFlt w){float sum(0.0); int it(0); for(auto ix : x ){ sum += wv[it]*sq2(ix-m); it++; } return sum/wnum(it,w);}
CAuto var           (CVFlt x, CFlt m, CVFlt wv){float sum(0.0); int it(0); for(auto ix : x ){ sum += wv[it]*sq2(ix-m); it++; } return sum/wnum(it,vfsum(wv));}

CAuto cvar          (CVFlt x, CFlt mx, CVFlt y, CFlt my, CVFlt wv, CFlt w){
						float sum(0.0); int it(0); 
						for(auto ix : x ){ sum += wv[it]*(ix-mx)*(y[it]-my); it++; } 
						return sum/wnum(it,w);
					}//<<>> CAuto cvar(CVFlt x, CFlt mx, CVFlt y, CFlt my, CVFlt wv, CFlt w)

CAuto cvar          (CVFlt x, CFlt mx, CVFlt y, CFlt my){
						float sum(0.0); int it(0); 
						for( auto ix : x ){ sum += (ix-mx)*(y[it]-my); it++; } 
						return sum/(x.size()-1);
					}//CAuto cvar

CAuto rms			(CVFlt x){float sum(0.0); for(auto ix : x ){ sum += sq2(ix); } return std::sqrt(sum/x.size());}
CAuto chisq			(CVFlt x, CFlt m, CFlt v){ float chi(0); for(auto ix : x ){ chi += sq2(ix-m)/v; } return chi; }

CAuto meanPhi		(CVFlt x){
						float sum(0.0);
                        auto maxphi = max(x); 
						for(auto ix : x ){ if( (maxphi-ix) > 180 ) sum+=(ix+360); else sum+=ix; } 
						auto rslt = sum/x.size(); 
						if( rslt > 360 ) rslt-=360; 
						return rslt;
					}//<<>> CAuto meanPhi(CVFlt x)

CAuto meanPhi       (CVFlt x, CVFlt wv){
						float wt(0.0), sum(0.0); int it(0); 
						auto maxphi = max(x); 
						for(auto ix : x ){ if( (maxphi-ix) > 180 ) sum+=(ix+360)*wv[it]; else sum+=ix*wv[it]; wt+=wv[it]; it++; }
                        auto rslt = sum/wt; 
						if( rslt > 360 ) rslt-=360; 
						return rslt;
					 }//<<>> CAuto meanPhi(CVFlt x, CVFlt wv)

CAuto wsin2         (CVFlt x, CVFlt wv){
						double sum(0.0), wt(0.0); int it(0); 
						for(auto ix : x ){ 
							sum += wv[it]*sq2(sin(ix)); 
							wt += wv[it];
							//std::cout << " ---- wsin2 : " << it << " x: " << ix << " sin^2: " << sq2(sin(ix)) << " sum: " << sum << " wt: " << wt << std::endl;
							it++;
						}//for(auto ix : x )
						return sum/wt;
					}//CAuto wsin2(CVFlt x, CVFlt wv)

CAuto wcos2         (CVFlt x, CVFlt wv){
						double sum(0.0), wt(0.0); int it(0); 
						for(auto ix : x ){ sum += wv[it]*sq2(cos(ix)); wt += wv[it]; it++;} 
						return sum/wt;
					}//CAuto wcos2

CAuto wsincos       (CVFlt x, CVFlt wv){
						double sum(0.0), wt(0.0); int it(0); 
						for(auto ix : x ){ sum += wv[it]*sin(ix)*cos(ix); wt += wv[it]; it++;} 
						return sum/wt;
					}//CAuto wsincos

// rh group functions
CAuto getRawID		(const EcalRecHit recHit){ auto recHitId = recHit.detid(); return recHitId.rawId();}
CAuto rhMatch		(const EcalRecHit rhx, const EcalRecHit rhy){ return getRawID(rhx) == getRawID(rhy);}
CAuto dupRhFnd		(const rhGroup x, const rhGroup y){for(CAuto rhx : x ){for(CAuto rhy : y ){if(rhMatch(rhx,rhy)){ return true;}}} return false;}
CAuto isRhGrpEx 	(const rhGroup x){int s=x.size();for( int i=0;i<s;i++){for( int j=i+1;j<s;j++){if(rhMatch(x[i],x[j])) return false;}} return true;}
CAuto getRhGrpEnr	(const rhGroup x){float e(0.0);for( auto ix : x ){e+=ix.energy();} return e;}
CAuto getDupCnt		(const vector<rhGroup> x){int c=0; int s=x.size();for( int a=0;a<s;a++){for( int b=a+1;b<s;b++){if(dupRhFnd(x[a],x[b]))c++;}} return c;}

//
// static data member definitions
//


//-------------------------------------------------------------------------------------------------------------------
