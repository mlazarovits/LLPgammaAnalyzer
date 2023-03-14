// -*- C++ -*-
//
// Package:    LLPgammaAnalyzer_MINI
// Class:      LLPgammaAnalyzer_MINI
//
/**\class LLPgammaAnalyzer_MINI LLPgammaAnalyzer_MINI.cc LLPgammaAnalyzer_MINI/plugins/LLPgammaAnalyzer_MINI.cc

 		Description: [one line class summary]

 		Implementation: [Notes on implementation]

*/
//
// Original Author:  Jack W King III
//         Created:  Wed, 27 Jan 2021 19:19:35 GMT
//
//

//----------------------------------------  cc file   --------------------------------------------------------------
//------------------------------------------------------------------------------------------------------------------

#include "LLPgammaAnalyzer_MINI.hh"
using namespace std;

//#define DEBUG true
#define DEBUG false

//
// constructors and destructor
//
LLPgammaAnalyzer_MINI::LLPgammaAnalyzer_MINI(const edm::ParameterSet& iConfig) :

// -- declare tags ----------------------------------------------------------

	// flags
	hasGenInfo (iConfig.existsAs<bool>("hasGenInfo")  ? iConfig.getParameter<bool>("hasGenInfo")  : false),

	// triggers
	//triggerResultsTag(iConfig.getParameter<edm::InputTag>("triggerResults")),
	//triggerObjectsTag(iConfig.getParameter<edm::InputTag>("triggerObjects")),

	// tracks
	tracksTag(iConfig.getParameter<edm::InputTag>("tracks")),
	
	// pfcands
	pfcandTag(iConfig.getParameter<edm::InputTag>("pfcandidates")),
	
	// vertices
	verticesTag(iConfig.getParameter<edm::InputTag>("vertices")),

	// rho
	//rhoTag(iConfig.getParameter<edm::InputTag>("rho")),

	// mets
	metsTag(iConfig.getParameter<edm::InputTag>("mets")),  

	// supercluster
	superClusterCollectionTag(iConfig.getParameter<edm::InputTag>("superClusters")),
	ootSuperClusterCollectionTag(iConfig.getParameter<edm::InputTag>("ootSuperClusters")),

	// caloclusters
	caloClusterTag(iConfig.getParameter<edm::InputTag>("caloClusters")),

	// jets
	jetsTag(iConfig.getParameter<edm::InputTag>("jets")), 

	// electrons
	electronsTag(iConfig.getParameter<edm::InputTag>("electrons")),  

	// muons
	muonsTag(iConfig.getParameter<edm::InputTag>("muons")),  

	// recHits
	recHitsEBTag(iConfig.getParameter<edm::InputTag>("recHitsEB")),  
	recHitsEETag(iConfig.getParameter<edm::InputTag>("recHitsEE")),

	// gedphotons
	gedPhotonsTag(iConfig.getParameter<edm::InputTag>("gedPhotons")),

	// ootPhotons
	ootPhotonsTag(iConfig.getParameter<edm::InputTag>("ootPhotons"))

// -- end of tag declarations ---------------------------------------
{ //<<<< LLPgammaAnalyzer_MINI::LLPgammaAnalyzer_MINI(const edm::ParameterSet& iConfig) :

	usesResource();
	usesResource("TFileService");

// -- consume tags ------------------------------------------------------------
	if( DEBUG ) std::cout << "In constructor for LLPgammaAnalyzer_MINI - tag and tokens" << std::endl;

	// Triggers
	//triggerResultsToken_ 	= consumes<edm::TriggerResults>(triggerResultsTag);
	//triggerObjectsToken_ 	= consumes<std::vector<pat::TriggerObjectStandAlone>>(triggerObjectsTag);

	// tracks 
	tracksToken_				= consumes<std::vector<reco::Track>>(tracksTag);

	// genparticles
	// genpart_token_              = consumes<std::vector<pat::PackedGenParticle>>(genpartTag);

	// pfcandidates
	pfcand_token_        		= consumes<CandidateView>(pfcandTag);

	// vertices
	verticesToken_				= consumes<std::vector<reco::Vertex>>(verticesTag);

	// rho
	//rhoToken_					= consumes<double>(rhoTag);

	// mets
	metsToken_					= consumes<std::vector<pat::MET>>(metsTag);

	// supercluster
	scToken_             		= consumes<reco::SuperClusterCollection>(superClusterCollectionTag);
	ootScToken_          		= consumes<reco::SuperClusterCollection>(ootSuperClusterCollectionTag); 

	// caloClusters
	ccToken_			     	= consumes<std::vector<reco::CaloCluster>>(caloClusterTag);	

	// jets
	jetsToken_					= consumes<std::vector<pat::Jet>>(jetsTag);
	
	// leptons
	electronsToken_				= consumes<std::vector<pat::Electron>>(electronsTag);
	muonsToken_					= consumes<std::vector<pat::Muon>>(muonsTag);

	// rechits
	recHitsEBToken_				= consumes<edm::SortedCollection<EcalRecHit,edm::StrictWeakOrdering<EcalRecHit>>>(recHitsEBTag);
	recHitsEEToken_				= consumes<edm::SortedCollection<EcalRecHit,edm::StrictWeakOrdering<EcalRecHit>>>(recHitsEETag);

	// photons
	gedPhotonsToken_ 			= consumes<std::vector<pat::Photon>>(gedPhotonsTag);
	ootPhotonsToken_ 			= consumes<std::vector<pat::Photon>>(ootPhotonsTag);

// ---------------------------------------------------------------------------------
}//>>>>LLPgammaAnalyzer_MINI::LLPgammaAnalyzer_MINI(const edm::ParameterSet& iConfig)


LLPgammaAnalyzer_MINI::~LLPgammaAnalyzer_MINI(){
	///////////////////////////////////////////////////////////////////
	// do anything here that needs to be done at desctruction time   //
	// (e.g. close files, deallocate resources etc.)                 //
	///////////////////////////////////////////////////////////////////
}//>>>>LLPgammaAnalyzer_MINI::~LLPgammaAnalyzer_MINI()


//
// member functions
//

detIdMap LLPgammaAnalyzer_MINI::SetupDetIDs(){

	detIdMap DetIDMap;
	std::string detIDConfig(ecal_config_path);
	uInt  cmsswId, dbID;
	int hashedId, iphi, ieta, absieta, Fed, SM, TT25, iTT, strip, Xtal, phiSM, etaSM;
	int side, ix, iy, SC, iSC, TTCCU, quadrant;
	TString pos;
	auto detIDConfigEB = detIDConfig + "fullinfo_detids_EB.txt";
	std::ifstream infileEB( detIDConfigEB, std::ios::in);
	if( DEBUG ) std::cout << "Setting up EB DetIDs with " << &infileEB << std::endl;
	while( infileEB >> cmsswId >> dbID >> hashedId >> iphi >> ieta >> absieta >> pos >> Fed >> SM >> TT25 >> iTT >> strip >> Xtal >> phiSM >> etaSM )
    	{ DetIDMap[cmsswId] = {iphi,ieta,TT25,ECAL::EB};}
	auto detIDConfigEE = detIDConfig + "fullinfo_detids_EE.txt";
	std::ifstream infileEE( detIDConfigEE, std::ios::in);
	if( DEBUG ) std::cout << "Setting up EE DetIDs with " << &infileEE << std::endl;
	while( infileEE >> cmsswId >> dbID >> hashedId >> side >> ix >> iy >> SC >> iSC >> Fed >> pos >> TTCCU >> strip >> Xtal >> quadrant )
    	{ DetIDMap[cmsswId] = {ix,iy,TTCCU,((side>0) ? ECAL::EP : ECAL::EM )};}

	return DetIDMap;

}//>>>>detIdMap LLPgammaAnalyzer_MINI::SetupDetIDs()


int LLPgammaAnalyzer_MINI::getPFJetID(const pat::Jet & jet){ 
	
	const auto eta  = std::abs(jet.eta());
	const auto NHF  = jet.neutralHadronEnergyFraction();
	const auto NEMF = jet.neutralEmEnergyFraction();
	const auto CHF  = jet.chargedHadronEnergyFraction();
	const auto CEMF = jet.chargedEmEnergyFraction();
	const auto NHM  = jet.neutralMultiplicity();
	const auto CHM  = jet.chargedMultiplicity();
	const auto SHM  = jet.chargedMultiplicity()+jet.neutralMultiplicity();
	const auto MUF  = jet.muonEnergyFraction();

	int tighter = 3;   
	int tightLepVeto = 0;
	int tight = 2;
	int loose = 1;

	bool nhfup  = NHF  < 0.90;
	bool nhflw  = NHF  > 0.2;
	
	bool nemfup1 = NEMF < 0.90;
	bool nemfup2 = NEMF < 0.99;
	bool nemf80 = NEMF < 0.80;
	bool nemflw = NEMF > 0.01;
	bool nemf10 = NEMF > 0.1;

	bool shm1  = SHM  > 1;
	bool muf8  = MUF  < 0.80;
	bool chf0  = CHF  > 0;
	bool chf10  = CHF  > 0.10;
	bool chm0  = CHM  > 0;
	bool cemf8 = CEMF > 0.80;
	bool nhm2  = NHM  > 1;
	bool nhm10 = NHM  > 10;

	bool eta1 = eta <= 2.6;
	bool eta2 = eta <= 2.7;
	bool eta3 = eta <= 3.0;

	if (eta1){
	 	if      (nhfup && nemfup1 && shm1 && muf8 && chf0 && chm0 && cemf8) return tightLepVeto;
	    else if (nhfup && nemf80 && shm1 && chf10 && chm0) return tighter;
	 	else if (nhfup && nemfup1 && shm1 && chf0 && chm0) return tight;
	 	else    return loose;
	} else if (!eta1 && eta2 ){ //<<>>if (eta1)
	 	if      (nhfup && nemfup2 && chm0 && muf8 && cemf8) return tightLepVeto;
	    else if (nhfup && nemf80 && chm0) return tighter;
	 	else if (nhfup && nemfup2 && chm0) return tight;
		else    return loose; 
	} else if (!eta2 && eta3){ //<<>>if (eta1) : else if
	   	if      (nemf10 && nemf80 && nhm2) return tighter;
		else if (nemflw && nemfup2 && nhm2) return tight;
	 	else    return loose; 
	} else { //<<>>if (eta1) : else if : else if
	 	if      (nhflw && nemfup1 && nhm10) return tight;
	 	else    return loose; 
	}//<<>>if (eta1) : else if : else if : else

	return -1; // should not happen

}//>>>>int LLPgammaAnalyzer_MINI::getPFJetID(const pat::Jet & jet)


rhGroup LLPgammaAnalyzer_MINI::getRHGroup( float eta, float phi, float drmin, float minenr = 0.0 ){

	rhGroup result;
	for (const auto recHit : *recHitsEB_ ){
		if( recHit.checkFlag(EcalRecHit::kHasSwitchToGain6) ) continue;
		auto enr = recHit.energy();
		if( enr <= minenr ) continue;
	   	const auto recHitId(recHit.detid());
	   	const auto recHitPos = barrelGeometry->getGeometry(recHitId)->getPosition();
	   	const auto dr = std::sqrt(reco::deltaR2(eta, phi, recHitPos.eta(), recHitPos.phi()));
	   	if( dr > drmin ) continue;
	   	result.push_back(recHit);
	}//<<>>for (const auto recHit : *recHitsEB_ )
	for (const auto recHit : *recHitsEE_ ){
	   	if( recHit.checkFlag(EcalRecHit::kHasSwitchToGain6) ) continue;
	   	auto enr = recHit.energy();
	   	if( enr <= minenr ) continue;
	   	const auto recHitId(recHit.detid());
	   	const auto recHitPos = endcapGeometry->getGeometry(recHitId)->getPosition();
	   	const auto dr = std::sqrt(reco::deltaR2(eta, phi, recHitPos.eta(), recHitPos.phi()));
	   	if( dr > drmin ) continue;
	   	result.push_back(recHit);
	}//<<>>for (const auto recHit : *recHitsEE_ )

	return result;

}//>>>>rhGroup LLPgammaAnalyzer_MINI::getRHGroup( float eta, float phi, float drmin, float minenr = 0.0 )

rhGroup LLPgammaAnalyzer_MINI::getRHGroup( uInt detid ){

	rhGroup result;
	for (const auto recHit : *recHitsEB_ ){
	   	const auto recHitId = recHit.detid();
		const auto rawId = recHitId.rawId();
	   	if( detid != rawId ) continue;
	   	result.push_back(recHit);
	}//<<>>for (const auto recHit : *recHitsEB_ )
	for (const auto recHit : *recHitsEE_ ){
	   	const auto recHitId = recHit.detid();
	   	const auto rawId = recHitId.rawId();
	   	if( detid != rawId ) continue;
	   	result.push_back(recHit);
	}//<<>>for (const auto recHit : *recHitsEE_ )

	return result;
}//>>>>rhGroup LLPgammaAnalyzer_MINI::getRHGroup( uInt detid )

rhGroup LLPgammaAnalyzer_MINI::getRHGroup(){

	rhGroup result;
	for (const auto recHit : *recHitsEB_ ) result.push_back(recHit);
	for (const auto recHit : *recHitsEE_ ) result.push_back(recHit);

	return result;

}//>>>>rhGroup LLPgammaAnalyzer_MINI::getRHGroup()


rhGroup LLPgammaAnalyzer_MINI::getRHGroup( const scGroup superClusterGroup, float minenr ){

	rhGroup result;
	vector<uInt> rawIds;
	for ( const auto superCluster : superClusterGroup ){
	 	auto & hitsAndFractions = superCluster.hitsAndFractions();
	 	const auto nHAF = hitsAndFractions.size();
	 	for( uInt iHAF = 0; iHAF < nHAF; iHAF++ ){
	    	const auto detId = hitsAndFractions[iHAF].first;
	    	const auto rawId = detId.rawId();
	    	if( std::find( rawIds.begin(), rawIds.end(), rawId ) == rawIds.end() ) rawIds.push_back(rawId);
	 	}//<<>>for( uInt iHAF = 0; iHAF < nHAF; iHAF++ )
	}//<<>>for ( const auto superCluster : superClusterGroup )	
	for (const auto recHit : *recHitsEB_ ){
	  	auto enr = recHit.energy();
	  	if( enr <= minenr ) continue;
	  	const auto recHitId = recHit.detid();
	  	const auto rawId = recHitId.rawId();
	  	if( std::find( rawIds.begin(), rawIds.end(), rawId ) != rawIds.end() ) result.push_back(recHit);
	}//<<>>for (const auto recHit : *recHitsEB_ )
	for (const auto recHit : *recHitsEE_ ){
	  	auto enr = recHit.energy();
	  	if( enr <= minenr ) continue;
	  	const auto recHitId = recHit.detid();
	  	const auto rawId = recHitId.rawId();
	  	if( std::find( rawIds.begin(), rawIds.end(), rawId ) != rawIds.end() ) result.push_back(recHit);
	}//<<>>for (const auto recHit : *recHitsEE_ )
			
	return result;

}//>>>>rhGroup LLPgammaAnalyzer_MINI::getRHGroup( const scGroup superClusterGroup, float minenr = 0.0 )

rhGroup LLPgammaAnalyzer_MINI::getRHGroup( const scGroup superClusterGroup, float minenr, vector<float> phEnergy, vector<float> phDr, float phEnMax ){

    rhGroup result;
    vector<uInt> rawIds;

	float dRcut = 0.05;

    int iter = -1;
    for ( const auto superCluster : superClusterGroup ){
		iter++;
		if( phDr[iter] > dRcut ) continue;
		if( phEnergy[iter] != phEnMax ) continue; 
        auto & hitsAndFractions = superCluster.hitsAndFractions();
        const auto nHAF = hitsAndFractions.size();
        for( uInt iHAF = 0; iHAF < nHAF; iHAF++ ){
            const auto detId = hitsAndFractions[iHAF].first;
            const auto rawId = detId.rawId();
            if( std::find( rawIds.begin(), rawIds.end(), rawId ) == rawIds.end() ) rawIds.push_back(rawId);
        }//<<>>for( uInt iHAF = 0; iHAF < nHAF; iHAF++ )
    }//<<>>for ( const auto superCluster : superClusterGroup )   
    for (const auto recHit : *recHitsEB_ ){
        auto enr = recHit.energy();
        if( enr <= minenr ) continue;
        const auto recHitId = recHit.detid();
        const auto rawId = recHitId.rawId();
        if( std::find( rawIds.begin(), rawIds.end(), rawId ) != rawIds.end() ) result.push_back(recHit);
    }//<<>>for (const auto recHit : *recHitsEB_ )
    for (const auto recHit : *recHitsEE_ ){
        auto enr = recHit.energy();
        if( enr <= minenr ) continue;
        const auto recHitId = recHit.detid();
        const auto rawId = recHitId.rawId();
        if( std::find( rawIds.begin(), rawIds.end(), rawId ) != rawIds.end() ) result.push_back(recHit);
    }//<<>>for (const auto recHit : *recHitsEE_ )

    return result;

}//>>>>rhGroup LLPgammaAnalyzer_MINI::getRHGroup( const scGroup superClusterGroup, float minenr, vector<float> phEnergy, vector<float> phDr, float phEnMax )

rhGroup LLPgammaAnalyzer_MINI::getRHGroup( const scGroup superClusterGroup, float minenr, vector<float> phEnergy, vector<float> phDr, float phEnMax, TH1D* rhTimeHist, TH1D* ootHist, TH1D* matchHist ){

    rhGroup result;
    vector<uInt> rawIds;

    float dRcut = 0.05;

    int iter = -1;
	int nRecHits(0);
	int matched(0);
	int nOOT(0);
    for ( const auto superCluster : superClusterGroup ){
        iter++;
        if( phDr[iter] > dRcut ) continue;
        if( phEnergy[iter] != phEnMax ) continue;
        auto & hitsAndFractions = superCluster.hitsAndFractions();
        const auto nHAF = hitsAndFractions.size();
        for( uInt iHAF = 0; iHAF < nHAF; iHAF++ ){
            const auto detId = hitsAndFractions[iHAF].first;
            const auto rawId = detId.rawId();
            if( std::find( rawIds.begin(), rawIds.end(), rawId ) == rawIds.end() ){ rawIds.push_back(rawId); nRecHits++; }
        }//<<>>for( uInt iHAF = 0; iHAF < nHAF; iHAF++ )
    }//<<>>for ( const auto superCluster : superClusterGroup )   
    for (const auto recHit : *recHitsEB_ ){
        auto enr = recHit.energy();
        if( enr <= minenr ) continue;
        const auto recHitId = recHit.detid();
        const auto rawId = recHitId.rawId();
        if( std::find( rawIds.begin(), rawIds.end(), rawId ) != rawIds.end() )
				{ result.push_back(recHit); if(recHit.checkFlag(EcalRecHit::kOutOfTime)) nOOT++; rhTimeHist->Fill(recHit.time()); matched++; }
    }//<<>>for (const auto recHit : *recHitsEB_ )
    for (const auto recHit : *recHitsEE_ ){
        auto enr = recHit.energy();
        if( enr <= minenr ) continue;
        const auto recHitId = recHit.detid();
        const auto rawId = recHitId.rawId();
        if( std::find( rawIds.begin(), rawIds.end(), rawId ) != rawIds.end() )
				{ result.push_back(recHit); if(recHit.checkFlag(EcalRecHit::kOutOfTime)) nOOT++; rhTimeHist->Fill(recHit.time()); matched++; }
    }//<<>>for (const auto recHit : *recHitsEE_ )
	if( nRecHits > 0 && matched > 0 ){
		matchHist->Fill(matched/nRecHits);
		ootHist->Fill(nOOT/matched);
	} else {
		matchHist->Fill(-0.05);
        ootHist->Fill(-0.05);
	}//<<>>if( nRecHits > 0 )

    return result;

}//>>>>rhGroup LLPgammaAnalyzer_MINI::getRHGroup( const scGroup superClusterGroup, float minenr, vector<float> phEnergy, vector<float> phDr, float phEnMax, TH1D ootHist, ... )

rhGroup LLPgammaAnalyzer_MINI::getRHGroup( const scGroup superClusterGroup, float minenr, TH1D* rhTimeHist, TH1D* ootHist, TH1D* matchHist ){

    rhGroup result;
    vector<uInt> rawIds;

    int iter = -1;
    float nRecHits(0);
    float matched(0);
    float nOOT(0);
    for ( const auto superCluster : superClusterGroup ){
        iter++;
        auto & hitsAndFractions = superCluster.hitsAndFractions();
        const auto nHAF = hitsAndFractions.size();
        for( uInt iHAF = 0; iHAF < nHAF; iHAF++ ){
            const auto detId = hitsAndFractions[iHAF].first;
            const auto rawId = detId.rawId();
            if( std::find( rawIds.begin(), rawIds.end(), rawId ) == rawIds.end() ){ rawIds.push_back(rawId); nRecHits++; }
        }//<<>>for( uInt iHAF = 0; iHAF < nHAF; iHAF++ )
    }//<<>>for ( const auto superCluster : superClusterGroup )   

//    reverse loop order ? ( loop rawids and find rechit instead of loooping rechits and finding rawids )  : think current method quicker
//    for (const auto rawid : rawIds ){
//		auto recHit = (*recHitsEB_)->find(recHitId);

    for (const auto recHit : *recHitsEB_ ){
        auto enr = recHit.energy();
        //if( enr <= minenr ) continue;
        const auto recHitId = recHit.detid();
        const auto rawId = recHitId.rawId();
        if( std::find( rawIds.begin(), rawIds.end(), rawId ) != rawIds.end() ){ 
        	matched++;
            if(recHit.checkFlag(EcalRecHit::kOutOfTime)) nOOT++;
            if( enr <= minenr ){ result.push_back(recHit); rhTimeHist->Fill(recHit.time()); }//<<>>if( enr <= minenr ) 
        }//<<>>if( std::find( rawIds.begin(), rawIds.end(), rawId ) != rawIds.end() )
    }//<<>>for (const auto recHit : *recHitsEB_ )
    for (const auto recHit : *recHitsEE_ ){
        auto enr = recHit.energy();
        //if( enr <= minenr ) continue;
        const auto recHitId = recHit.detid();
        const auto rawId = recHitId.rawId();
        if( std::find( rawIds.begin(), rawIds.end(), rawId ) != rawIds.end() ){  
            matched++;
            if(recHit.checkFlag(EcalRecHit::kOutOfTime)) nOOT++;
            if( enr <= minenr ){ result.push_back(recHit); rhTimeHist->Fill(recHit.time()); }//<<>>if( enr <= minenr ) 
        }//<<>>if( std::find( rawIds.begin(), rawIds.end(), rawId ) != rawIds.end() )
    }//<<>>for (const auto recHit : *recHitsEE_ )
	//std::cout << " --- rh cl stats : nRH " << nRecHits << " matched : " << matched << " nOOT " << nOOT << std::endl;

    if( nRecHits > 0 && matched > 0 ){
        if(nRecHits == matched ) matchHist->Fill(1.0);
		else matchHist->Fill(matched/nRecHits);
        ootHist->Fill(nOOT/matched);
 	} else if( nRecHits > 0 ){
 		matchHist->Fill(matched/nRecHits);
    } else {
        matchHist->Fill(-0.05);
        ootHist->Fill(-0.05);
    }//<<>>if( nRecHits > 0 )

    return result;

}//>>>>rhGroup LLPgammaAnalyzer_MINI::getRHGroup( const scGroup superClusterGroup, float minenr, TH1D ootHist, ... )

rhGroup LLPgammaAnalyzer_MINI::getRHGroup( const reco::CaloCluster basicCluster, float minenr = 0.0 ){

	rhGroup result;
	vector<uInt> rawIds;
	auto & hitsAndFractions = basicCluster.hitsAndFractions();
	const auto nHAF = hitsAndFractions.size();
	//std::cout << " --- getRHGroup for basic cluster with " << nHAF << " rechits " << std::endl;
	for( uInt iHAF = 0; iHAF < nHAF; iHAF++ ){
		const auto detId = hitsAndFractions[iHAF].first;
		const auto rawId = detId.rawId();
		if( std::find( rawIds.begin(), rawIds.end(), rawId ) == rawIds.end() ) rawIds.push_back(rawId);
	}//<<>>for( uInt iHAF = 0; iHAF < nHAF; iHAF++ )
	for (const auto recHit : *recHitsEB_ ){
	  	auto enr = recHit.energy();
	  	if( enr <= minenr ) continue;
	  	const auto recHitId = recHit.detid();
	  	const auto rawId = recHitId.rawId();
	  	if( std::find( rawIds.begin(), rawIds.end(), rawId ) != rawIds.end() ) result.push_back(recHit);
	}//<<>>for (const auto recHit : *recHitsEB_ )
	for (const auto recHit : *recHitsEE_ ){
	  	auto enr = recHit.energy();
	  	if( enr <= minenr ) continue;
	  	const auto recHitId = recHit.detid();
	  	const auto rawId = recHitId.rawId();
	  	if( std::find( rawIds.begin(), rawIds.end(), rawId ) != rawIds.end() ) result.push_back(recHit);
	}//<<>>for (const auto recHit : *recHitsEE_ )		

	return result;

}////rhGroup LLPgammaAnalyzer_MINI::getRHGroup( const reco::CaloCluster basicCluster, float minenr = 0.0 )

	
vector<float> LLPgammaAnalyzer_MINI::getRhTofTime( rhGroup recHits, double vtxX, double vtxY, double vtxZ ){

	vector<float> result;
	for (const auto recHit : recHits ){		
		const auto rht = recHit.time();
		//std::cout << " ----- Get TOF Time rh time: " << rht << std::endl;
	   	const auto recHitId(recHit.detid());
	   	const auto recHitPos = barrelGeometry->getGeometry(recHitId)->getPosition();
	   	const auto rhPosX = recHitPos.x();
	   	const auto rhPosY = recHitPos.y();
	   	const auto rhPosZ = recHitPos.z();
		//std::cout << " ----- Get TOF Time rh POS: " << rhPosX << " " <<	rhPosY << " " << rhPosZ << std::endl;
	   	const auto d_rh = hypo(rhPosX,rhPosY,rhPosZ);
	   	const auto d_pv = hypo(rhPosX-vtxX,rhPosY-vtxY,rhPosZ-vtxZ);
	   	const auto tof = (d_rh-d_pv)/SOL;
	   	//std::cout << " ----- Get TOF Time rh tof: " << tof << std::endl;
	   	result.push_back(rht-tof);
	}//<<>>for (const auto recHit : recHits )	

	return result;

}//>>>>vector<float>  LLPgammaAnalyzer_MINI::getRhTofTime( rhGroup recHits, double vtxX, double vtxY, double vtxZ )

float LLPgammaAnalyzer_MINI::getRhTOF( EcalRecHit rechit, double vtxX, double vtxY, double vtxZ ){

    const auto recHitId(rechit.detid());
    const auto recHitPos = barrelGeometry->getGeometry(recHitId)->getPosition();
	const auto rhPosX = recHitPos.x();
	const auto rhPosY = recHitPos.y();
	const auto rhPosZ = recHitPos.z();
	const auto d_rh = hypo(rhPosX,rhPosY,rhPosZ);
	const auto d_pv = hypo(rhPosX-vtxX,rhPosY-vtxY,rhPosZ-vtxZ);
	const auto tof = (d_rh-d_pv)/SOL;
	return tof;	

}//>>>>>float getRhTOF( EcalRecHit rechit, double vtxX, double vtxY, double vtxZ )

EcalRecHit LLPgammaAnalyzer_MINI::getLeadRh( rhGroup recHits ){
	
	EcalRecHit result;
	float enr(0.0);
	for (const auto recHit : recHits ){
		auto rhenr = recHit.energy();
		if( rhenr < enr ) continue;
		enr = rhenr;
		result = recHit;
	}//<<>>for (const auto recHit : recHits )

	return result;

}//>>>>EcalRecHit LLPgammaAnalyzer_MINI::getLeadRh( rhGroup recHits )


vector<float> LLPgammaAnalyzer_MINI::getLeadTofRhTime( rhGroup recHits, double vtxX, double vtxY, double vtxZ ){

	vector<float> result;
	auto lrh = getLeadRh(recHits);
	const auto recHitId(lrh.detid());
	const auto recHitPos = barrelGeometry->getGeometry(recHitId)->getPosition();
	const auto rhPosX = recHitPos.x();
	const auto rhPosY = recHitPos.y();
	const auto rhPosZ = recHitPos.z();
	const auto d_rh = hypo(rhPosX,rhPosY,rhPosZ);
	const auto d_pv = hypo(rhPosX-vtxX,rhPosY-vtxY,rhPosZ-vtxZ);
	const auto tof = (d_rh-d_pv)/SOL;
	for (const auto recHit : recHits ){result.push_back(recHit.time()-tof);}
	return result;

}//>>>>vector<float>  LLPgammaAnalyzer_MINI::getLeadTofRhTime( rhGroup recHits, double vtxX, double vtxY, double vtxZ )

float LLPgammaAnalyzer_MINI::getSeedTofTime( reco::SuperCluster sprclstr, double vtxX, double vtxY, double vtxZ ){

	float rhTime(-100.0);
	const auto seedDetId = sprclstr.seed().get()->seed(); // seed detid
	const auto seedRawId = seedDetId.rawId(); // crystal number
    const auto isEB = (seedDetId.subdetId() == EcalSubdetector::EcalBarrel); // which subdet
    const auto recHits = (isEB ? *recHitsEB_ : *recHitsEE_ );
	for( const auto rechit : recHits ){
        const auto recHitId = rechit.detid();
        const auto rawId = recHitId.rawId();
        if( rawId == seedRawId ){ rhTime = rechit.time(); continue; }
    }//<<>>for (const auto recHit : *recHitsEE_ ) 

    const auto recHitPos = barrelGeometry->getGeometry(seedDetId)->getPosition();
    const auto rhPosX = recHitPos.x();
    const auto rhPosY = recHitPos.y();
    const auto rhPosZ = recHitPos.z();
    const auto d_rh = hypo(rhPosX,rhPosY,rhPosZ);
    const auto d_pv = hypo(rhPosX-vtxX,rhPosY-vtxY,rhPosZ-vtxZ);
    const auto tof = (d_rh-d_pv)/SOL;
	const auto seedTofTime = rhTime - tof;
    return seedTofTime;

}//>>>>float  LLPgammaAnalyzer_MINI::getSeedTofTime( rhGroup recHits, double vtxX, double vtxY, double vtxZ )

float LLPgammaAnalyzer_MINI::getLeadTofTime( rhGroup recHits, double vtxX, double vtxY, double vtxZ ){

    auto lrh = getLeadRh(recHits);
    const auto recHitId(lrh.detid());
    const auto recHitPos = barrelGeometry->getGeometry(recHitId)->getPosition();
    const auto rhPosX = recHitPos.x();
    const auto rhPosY = recHitPos.y();
    const auto rhPosZ = recHitPos.z();
    const auto d_rh = hypo(rhPosX,rhPosY,rhPosZ);
    const auto d_pv = hypo(rhPosX-vtxX,rhPosY-vtxY,rhPosZ-vtxZ);
    const auto tof = (d_rh-d_pv)/SOL;
    return lrh.time()-tof;

}//>>>>float  LLPgammaAnalyzer_MINI::getSeedTofTime( rhGroup recHits, double vtxX, double vtxY, double vtxZ )

vector<float> LLPgammaAnalyzer_MINI::getTimeDistStats( vector<float> times ){

	vector<float> results;
	int size = times.size();
	
	if( size == 0 ){		
	   	std::cout << "Time group is empty" << std::endl; 
	   	results.push_back(-29.25);//0
	   	results.push_back(99.9);//1
	   	results.push_back(-29.25);//2
	   	results.push_back(99.9);//3
	   	results.push_back(99.9);//4
	   	results.push_back(9.9);//5
	   	results.push_back(-29.25);//6
	   	results.push_back(99.9);//7
	   	results.push_back(99.9);//8
	   	results.push_back(99.9);//9
	   	results.push_back(-29.25);//10
	   	results.push_back(99.9);//11
	   	return results;
	}//<<>>if( size == 0 )

	TH1D timeDist( "temp", "temp", 500, -25, 25 );
	for( const auto time : times ) timeDist.Fill(time);
	auto nEntries = timeDist.GetEntries();
	double medtime(0.0);
	if( nEntries > 0 ){	
	   	double quant(0.5);
	   	timeDist.ComputeIntegral();
	   	timeDist.GetQuantiles(1, &medtime, &quant);
	} else medtime = -9.9;

	auto error = timeDist.GetMeanError();
	results.push_back(timeDist.GetMean());//0
	results.push_back(error);//1
	results.push_back(medtime);//2
	results.push_back(1.2533*error);//3 valid for noraml distibutions
	results.push_back(timeDist.GetRMS());//4
	results.push_back(timeDist.GetSkewness());//5

	auto rmu = mean(times);
	results.push_back(rmu);//6 mean
	auto rsd = stdev(times, rmu);
	results.push_back(rsd);//7 stdev
	results.push_back(rms(times));//8 rms
	auto err = rsd/std::sqrt(size);
	results.push_back(err);//9 error of mean

	std::sort(times.begin(),times.end());
	if( size%2 == 0 ) results.push_back((times[(size/2)-1] + times[(size/2)])/2);//10
	else results.push_back(times[size/2]);//10 median
	results.push_back(1.2533*err);//11 error of median
	
	return results;
}//>>>> vector<float> LLPgammaAnalyzer_MINI::getTimeDistStats( vector<float> times )


vector<float> LLPgammaAnalyzer_MINI::getTimeDistStats( vector<float> times, vector<float> wts ){

	vector<float> results;
	int size = times.size();	
	int wtsize = wts.size();
	if( size != wtsize || size == 0 ){ 
		std::cout << "Time & rechit groups not same or empty" << std::endl; 
		results.push_back(-29.25);//0 mu
	   	results.push_back(99.9);//1
	   	results.push_back(-29.25);//2
	   	results.push_back(99.9);//3
	   	results.push_back(99.9);//4
	   	results.push_back(9.9);//5
	   	results.push_back(-29.25);//6
	   	results.push_back(99.9);//7
	   	results.push_back(99.9);//8
	   	results.push_back(99.9);//9
		results.push_back(-29.25);//10
	   	results.push_back(9.9);//11
	   	results.push_back(-10.0);//12
		return results; 
	}//<<>>if( size != rechits.size() || size == 0 )
			
	TH1D timeDist( "temp", "temp", 500, -10, 10 );
	vector<float> wtimes;
	float wtot(0.0);
	for( int it(0); it < size; it++ ){
		timeDist.Fill(times[it], wts[it]);
		wtimes.push_back(times[it]*wts[it]);
		wtot += wts[it];
	}//<<>>for( uInt it(0); it < size; it++ )
   	//auto nEntries = timeDist.GetEntries();
	double hmedtime(9.9);
	//if( nEntries > 0 ){
	//	double quant(0.5);
	// 	timeDist.ComputeIntegral();
	// 	timeDist.GetQuantiles(1, &hmedtime, &quant);
	//} else hmedtime = -9.9; 
	
	auto herror = timeDist.GetMeanError();
	auto hmutime = timeDist.GetMean();
	auto hrms = timeDist.GetRMS();
	results.push_back(hmutime);//0
	results.push_back(herror);//1
	results.push_back(hmedtime);//2
	results.push_back(1.2533*herror);//3 - valid for noraml distibutions
	results.push_back(hrms);//4
	results.push_back(timeDist.GetSkewness());//5

	auto rmu = mean(wtimes,wtot);
	results.push_back(rmu);//6 mean
	auto rsd = stdev(times, rmu, wts, wtot);
	results.push_back(rsd);//7 stdev
	results.push_back(rms(wtimes));//8 rms
	auto err = rsd/std::sqrt(size);
	results.push_back(err);//9 error of mean

	std::sort(times.begin(),times.end());
	if( size%2 == 0 ) results.push_back((times[(size/2)-1]+times[(size/2)])/2);//10
	else results.push_back(times[size/2]);//10 median
	results.push_back(1.2533*err);//11 error of median	
	results.push_back(wtot);//12 tot e of all rechits
	results[2] = results[6] - results[10];

	return results;
}//>>>>vector<float> LLPgammaAnalyzer_MINI::getTimeDistStats( vector<float> times, vector<float> weights )

vector<float> LLPgammaAnalyzer_MINI::getTimeDistStats( vector<float> times, rhGroup rechits ){

   	// N 3.64, C 0.3000  s^2 = (N/(rhe))^2 + 2C^2

   	float N(3.64);
   	float C(0.3000);

   	vector<float> wts;
   	for( uInt it(0); it < rechits.size(); it++ ){
	//auto wt = 1/std::sqrt(sq2(N/rechits[it].energy())+2*sq2(C)); 
      	auto wt = 1/(sq2(N/rechits[it].energy())+2*sq2(C));
		wts.push_back(wt);
   	}//<<>>for( uInt it(0); it < rechits.size(); it++ )

   	return getTimeDistStats( times, wts );
}//>>>>vector<float> LLPgammaAnalyzer_MINI::getTimeDistStats( vector<float> times, rhGroup rechits ){

vector<float> LLPgammaAnalyzer_MINI::getRhGrpEigen( vector<float> xs, vector<float> wts ){
//spherical

    vector<float> results;

    auto ts2 = wsin2( xs, wts );
    auto tc2 = wcos2( xs, wts );
    auto tsc = wsincos( xs, wts );
	//std::cout << "Sin2 : " << ts2 << " Cos2 : " << tc2 << " SinCos : " << tsc << std::endl;
    //std::cout << "Varencies and Means Calculated" << std::endl;

    //TMatrixDSym mat(3,3);
    double array[] = { ts2, tsc, tsc, tc2 };
    //for( auto thing : array ) std::cout << thing;
    //cout << std::endl;
    //TArrayD data(9,array);
    //mat.SetMatrixArray(data.GetArray());
    //mat.SetMatrixArray(array);
    TMatrixDSym mat(2,array);
    //std::cout << "Input matrix created" << std::endl;
    //mat.Print();

    TMatrixDSymEigen eigen(mat);
    const TVectorD& eginVal = eigen.GetEigenValues();
    const TMatrixD& eginVec = eigen.GetEigenVectors();
    TVectorD evaules(eginVal);
    //evaules.ResizeTo(3);
    //std::cout << "Eigin Values and Vectors Found " << std::endl;
    //eginVal.Print();
    //evaules.Print();
    //eginVec.Print();

    int index(1);
    if( (eginVal(0) >= eginVal(1)) ){
        index = 0;
        //if( (eginVal(0) == eginVal(1)) ) std::cout << " -- rhGrp is Spherical" << std::endl;
    }//<<>>if( (eginVal(0) >= eginVal(1)) )
    //std::cout << "Largest Eigin Value Found" << std::endl;

    auto ex = eginVec(index,0);
    auto ey = eginVec(index,1);
    auto ev = eginVal(index);
    //std::cout << "Eigin Angles Found" << std::endl;

    results.push_back(ex);
    results.push_back(ey);
    results.push_back(ev);

    return results;
}//<<>>vector<float> LLPgammaAnalyzer_MINI::getRhGrpEigen2D( vector<float> xs, vector<float> ys, vector<float> zs, vector<float> wts )

vector<float> LLPgammaAnalyzer_MINI::getRhGrpEigen( vector<float> xs, vector<float> ys, vector<float> wts ){

    vector<float> results;

    auto mean_x = mean( xs, wts );
    auto mean_y = mean( ys, wts );
    auto swts = vfsum( wts );
    auto var_x = var( xs, mean_x, wts, swts );
    auto var_y = var( ys, mean_y, wts, swts );
    auto var_xy = cvar( xs, mean_x, ys, mean_y, wts, swts );
    //std::cout << "Varencies and Means Calculated" << std::endl;

    //TMatrixDSym mat(3,3);
    double array[] = { var_x, var_xy, var_xy, var_y };
    //for( auto thing : array ) std::cout << thing;
    //cout << std::endl;
    //TArrayD data(9,array);
    //mat.SetMatrixArray(data.GetArray());
    //mat.SetMatrixArray(array);
    TMatrixDSym mat(2,array);
    //std::cout << "Input matrix created" << std::endl;
    //mat.Print();

    TMatrixDSymEigen eigen(mat);
    const TVectorD& eginVal = eigen.GetEigenValues();
    const TMatrixD& eginVec = eigen.GetEigenVectors();
    TVectorD evaules(eginVal);
    //evaules.ResizeTo(3);
    //std::cout << "Eigin Values and Vectors Found " << std::endl;
    //eginVal.Print();
    //evaules.Print();
    //eginVec.Print();

    int index(1);
    if( (eginVal(0) >= eginVal(1)) ){
        index = 0;
        //if( (eginVal(0) == eginVal(1)) ) std::cout << " -- rhGrp is Spherical" << std::endl;
    }//<<>>if( (eginVal(0) >= eginVal(1)) )
    //std::cout << "Largest Eigin Value Found" << std::endl;

    auto ex = eginVec(index,0);
    auto ey = eginVec(index,1);
    auto ev = eginVal(index);
    //std::cout << "Eigin Angles Found" << std::endl;

    results.push_back(ex);
    results.push_back(ey);
    results.push_back(ev);

    return results;
}//<<>>vector<float> LLPgammaAnalyzer_MINI::getRhGrpEigen2D( vector<float> xs, vector<float> ys, vector<float> zs, vector<float> wts )

vector<float> LLPgammaAnalyzer_MINI::getRhGrpEigen( vector<float> xs, vector<float> ys, vector<float> zs, vector<float> wts ){
// ieipt

	vector<float> results;

	auto mean_x = mean( xs, wts );
    auto mean_y = mean( ys, wts );
    auto mean_z = mean( zs, wts );
    auto swts = vfsum( wts );
    auto var_x = var( xs, mean_x, wts, swts );
    auto var_y = var( ys, mean_y, wts, swts );
    auto var_z = var( zs, mean_z, wts, swts );
    auto var_xy = cvar( xs, mean_x, ys, mean_y, wts, swts );
    auto var_xz = cvar( xs, mean_x, zs, mean_z, wts, swts );
    auto var_yz = cvar( ys, mean_y, zs, mean_z, wts, swts );
	//std::cout << "Varencies and Means Calculated" << std::endl;
    //std::cout << " xx- " << var_x  << " xy- " << var_xy << " xz- " << var_xz << std::endl; 
    //std::cout << " xy- " << var_xy << " yy- " << var_y  << " yz- " << var_yz << std::endl;
    //std::cout << " xz- " << var_xz << " yz- " << var_yz << " zz- " << var_z  << std::endl;

	//TMatrixDSym mat(3,3);
	double array[] = { var_x, var_xy, var_xz, var_xy, var_y, var_yz, var_xz, var_yz, var_z };
    //for( auto thing : array ) std::cout << thing;
	//cout << std::endl;
   	//TArrayD data(9,array);
	//mat.SetMatrixArray(data.GetArray());
    //mat.SetMatrixArray(array);
    TMatrixDSym mat(3,array);
    //std::cout << "Input matrix created" << std::endl;
	//mat.Print();

	TMatrixDSymEigen eigen(mat);
	const TVectorD& eginVal = eigen.GetEigenValues();
	const TMatrixD& eginVec = eigen.GetEigenVectors();
	TVectorD evaules(eginVal);	
    //evaules.ResizeTo(3);
    //std::cout << "Eigin Values and Vectors Found " << std::endl;
    //eginVal.Print();
    //evaules.Print();
    //eginVec.Print();

	int index(2);
	int index2(2);
	if( (eginVal(0) >= eginVal(1)) && (eginVal(0) >= eginVal(2)) ){ 
		index = 0;
		if( eginVal(1) >= eginVal(2) ) index2 = 1; 
		if( (eginVal(0) == eginVal(1)) && (eginVal(0) == eginVal(2)) ) std::cout << " -- rhGrp is Spherical" << std::endl;
    } else if( eginVal(1) >= eginVal(2) ) {
		index = 1;
		if( eginVal(0) >= eginVal(2) ) index2 = 0;
		if( eginVal(1) == eginVal(2) ) std::cout << " -- rhGrp is a Flatend Sphere" << std::endl;
	} else { if( eginVal(0) >= eginVal(1) ) index2 = 0; else index2 = 1; } //<<>>if( (eginVal(0) >= eginVal(1)) && (eginVal(0) >= eginVal(2)) )
    //std::cout << "Largest Eigin Value Found" << std::endl;

	auto index3 = 3 - index - index2;
	auto ev23hypo = hypo(eginVal[index2], eginVal[index3]);
	auto ev1Shypo = hypo(eginVal[index], ev23hypo);
	auto speginval = sq2(eginVal[index]/ev1Shypo);

	auto ex = eginVec(index,0);
    auto ey = eginVec(index,1);
    auto ez = eginVec(index,2);
    auto ev = speginval; //eginVal(index);
    //std::cout << "Eigin Angles Found" << std::endl;

	results.push_back(ex);//0
    results.push_back(ey);//1
    results.push_back(ez);//2
    results.push_back(ev);//3

	return results;
}//<<>>vector<float> LLPgammaAnalyzer_MINI::getRhGrpEigen3D( vector<float> xs, vector<float> ys, vector<float> zs, vector<float> wts )

vector<float> LLPgammaAnalyzer_MINI::getRhGrpEigen_ieipt( vector<float> times, rhGroup rechits ){

    // N 3.64, C 0.3000  s^2 = (N/(rhe))^2 + 2C^2

    float N(3.64);
    float C(0.3000);
    float tdis(2.2/SOL);

    vector<float> wts;
    vector<float> etas;
    vector<float> phis;
    vector<float> ebtimes;
    vector<float> emepReturn(4,-999);

	float totWts(0);
    //bool ebp = false;
    for( uInt it(0); it < rechits.size(); it++ ){

		const auto recHitID = getRawID(rechits[it]);
		const auto & idinfo = DetIDMap[recHitID];
		if( idinfo.ecal == ECAL::EB ){
    		//auto wt = 1/std::sqrt(sq2(N/rechits[it].energy())+2*sq2(C)); 
        	auto resolution = 1/(sq2(N/rechits[it].energy())+2*sq2(C));
        	wts.push_back(resolution);
			totWts += resolution;
        	//const auto recHitPos = barrelGeometry->getGeometry(recHitId)->getPosition();
        	const auto rhEtaPos = tdis*(idinfo.i2);//recHitPos.eta();
			//if( rhEtaPos > 0 ) ebp = true;
        	etas.push_back(rhEtaPos);
           // hist1d[66]->Fill(idinfo.i2);
        	//const auto rhPhiPos = tdis*idinfo.i1;//recHitPos.phi();
        	phis.push_back(idinfo.i1);
          //  hist1d[67]->Fill(idinfo.i1);
			ebtimes.push_back(times[it]);
           // hist1d[68]->Fill(times[it]);
		} else { return emepReturn; //std::cout << "In getRhGrpEigen_sph : NOT EB !!!!!!" << std::endl; }
		}//<<>>if( idinfo.ecal == ECAL::EB )

    }//<<>>for( uInt it(0); it < rechits.size(); it++ )

	vector<float> adjphis;
	auto mphi = ceil(meanPhi(phis));
	for( uInt it(0); it < phis.size(); it++ ){ 
		auto dstphi = dPhi(phis[it],mphi);
		adjphis.push_back(tdis*(mphi+dstphi));
	}//<<>>for( auto it : phis )

    auto eigens = getRhGrpEigen( etas, adjphis, ebtimes, wts );//0 x, 1 y, 2 z, 3 values

    vector<float> angles;
    auto meta = mean(etas,wts);
    auto adjmphi = mean(adjphis,wts);
    auto mtime = mean(ebtimes,wts);
    //for( uInt it(0); it < ebtimes.size(); it++ ){
    //    float leta = etas[it] - meta;
    //    float lphi = adjphis[it] - amphi;
    //    if( leta == 0 && lphi == 0 ) continue;
    //    //float ltim = ebtimes[it]-mtime;
    //    float angle = getAngle( leta, lphi );
    //    angles.push_back(angle);
    //}//<<>>for( uInt it(0); it < ebtimes.size(); it++ )

    auto eprotangle = getAngle(eigens[0], eigens[1]);
	//auto ephypo = hypo(eigens[0], eigens[1]);
    //auto etrotangle = getAngle(ephypo, eigens[2]);
    //if( not ebp ){ rotangle += PI; if( rotangle > TWOPI ) rotangle -= TWOPI; }
    float epeignsin = std::sin(eprotangle);
    float epeigncos = std::cos(eprotangle);
    //float eteignsin = std::sin(etrotangle);
    //float eteigncos = std::cos(etrotangle);
    vector<float> xs;
    float xsum(0.0);
    for( uInt it(0); it < ebtimes.size(); it++ ){

        float leta = etas[it] - meta;
        float lphi = adjphis[it] - adjmphi;
        float ltime = ebtimes[it] - mtime;
		//if( not ebp ) leta *= -1;
       // hist1d[75]->Fill(leta);
       // hist1d[76]->Fill(lphi);
       // hist1d[77]->Fill(ltime);
        auto epxcor = epeigncos*(leta) - epeignsin*(lphi);
        //auto epycor = epeignsin*(leta) + epeigncos*(lphi);
        //auto etxcor = eteigncos*(epxcor) - eteignsin*(ltime);
        //auto etycor = eteignsin*(epxcor) + eteigncos*(ltime);
        //if( not ebp ) xcor *= -1;
        //std::cout << "In getRhGrpEigen_sph w/2 leta " << leta << " : lphi " << lphi << " : xcor " << xcor << " : ycor " << ycor << " : dt " << wts[it] << std::endl;
        //if( abs(wts[it]) < 8 )
        //auto fill = ltime*wts[it];
		//if( std::abs(fill) < 1 ){
       // hist2d[77]->Fill(epxcor,epycor,fill);
       // hist2d[78]->Fill(epxcor,epycor,wts[it]);
       // //}//<<>>if( std::abs(fill) < 20 )
       // hist2d[79]->Fill(etxcor,etycor,fill);
       // hist2d[84]->Fill(etxcor,etycor,wts[it]);
       // hist2d[80]->Fill(epxcor,ltime,fill);
       // hist2d[85]->Fill(epxcor,ltime,wts[it]);
		auto sl = (ltime)/(epxcor);
		xsum += sl*wts[it];
        xs.push_back(sl);

    }//<<>>for( uInt it(0); it < wts.size(); it++ )

	auto slope = xsum/totWts;
	auto nWts = wts.size();
    //float schi2(0); for( uInt it(0); it < nWts; it++ ){ schi2 += sq2(xs[it]-slope)*wts[it]/abs(slope);}
    //auto chi2 = schi2/totWts/nWts;
    //auto chi2 = schi2/totWts;
    //auto chi2pf = chi2/(nWts-1);
    //auto chi2pf = TMath::Prob(chi2, nWts);
    auto varsl = var(xs,slope,wts,totWts);
    auto chi2 = chisq(xs,slope,varsl);
	auto chi2pf = 1 - TMath::Prob(chi2, nWts);
    eigens.push_back(slope);//4
    eigens.push_back(chi2pf);//5
   // hist1d[114]->Fill(slope);
   // hist1d[115]->Fill(chi2pf);
   // hist1d[104]->Fill(1000/slope);

    return eigens;
}//>>>>vector<float> LLPgammaAnalyzer_MINI::getRhGrpEigen( vector<float> times, rhGroup rechits ){

vector<float> LLPgammaAnalyzer_MINI::getRhGrpEigen_xyz( vector<float> times, rhGroup rechits ){

    // N 3.64, C 0.3000  s^2 = (N/(rhe))^2 + 2C^2

    float N(3.64);
    float C(0.3000);

    vector<float> wts;
    vector<float> etas;
    vector<float> phis;
    for( uInt it(0); it < rechits.size(); it++ ){
    //auto wt = 1/std::sqrt(sq2(N/rechits[it].energy())+2*sq2(C)); 
        auto wt = 1/(sq2(N/rechits[it].energy())+2*sq2(C));
        wts.push_back(wt);
        const auto recHitId(rechits[it].detid());
        const auto recHitPos = barrelGeometry->getGeometry(recHitId)->getPosition();
        const auto rhEtaPos = recHitPos.eta();
        etas.push_back(rhEtaPos);
        const auto rhPhiPos = recHitPos.phi();
        phis.push_back(rhPhiPos);
    }//<<>>for( uInt it(0); it < rechits.size(); it++ )

    return getRhGrpEigen( etas, phis, times, wts );
}//>>>>vector<float> LLPgammaAnalyzer_MINI::getRhGrpEigen( vector<float> times, rhGroup rechits ){

vector<float> LLPgammaAnalyzer_MINI::getRhGrpEigen_sph( vector<float> times, rhGroup rechits ){

    // N 3.64, C 0.3000  s^2 = (N/(rhe))^2 + 2C^2

    float N(3.64);
    float C(0.3000);
    //float twoPI(2*PI);

    vector<float> wts;
    vector<float> etas;
    vector<float> phis;
    vector<float> ebtimes;
    vector<float> angles;
    //vector<float> energies;
    vector<float> resolutions;
    //float sumenergy = 0;
    vector<float> emepReturn(4,-999);

	//bool ebp = true;
    //bool ebn = true;
	auto nRecHits = rechits.size();
    for( uInt it(0); it < nRecHits; it++ ){

        const auto recHitID = getRawID(rechits[it]);
        const auto & idinfo = DetIDMap[recHitID];
        if( idinfo.ecal == ECAL::EB ){ 
        	//auto wt = 1/std::sqrt(sq2(N/rechits[it].energy())+2*sq2(C)); 
        	//auto wt = 1/(sq2(N/rechits[it].energy())+2*sq2(C));
        	//wts.push_back(wt);
        	//const auto recHitPos = barrelGeometry->getGeometry(recHitId)->getPosition();
        	const auto rhEtaPos = idinfo.i2;//recHitPos.eta();
			//if( rhEtaPos < 10 ) ebp = false;
            //if( rhEtaPos > 10 ) ebn = false;
        	etas.push_back(rhEtaPos);
        	//hist1d[69]->Fill(rhEtaPos);
        	const auto rhPhiPos = idinfo.i1;//recHitPos.phi();
        	//hist1d[70]->Fill(rhPhiPos);
        	phis.push_back(rhPhiPos);
        	ebtimes.push_back(times[it]);	
			auto rhenergy = rechits[it].energy();
			auto resolution = 1/(sq2(N/rhenergy)+2*C*C);
			//energies.push_back(sq2(rhenergy));
            resolutions.push_back(resolution);
			//sumenergy += rhenergy;
			//std::cout << "In getRhGrpEigen_sph w/ rheta " << rhEtaPos << " : rhphi " << rhPhiPos << " : rht " << times[it] << std::endl;		
		} else { return emepReturn; //std::cout << "In getRhGrpEigen_sph : NOT EB !!!!!!" << std::endl; }
		}//<<>>if( idinfo.ecal != ECAL::EB )
    }//<<>>for( uInt it(0); it < rechits.size(); it++ )

	//auto meta = std::ceil(mean(etas,resolution));
    auto meta = mean(etas,resolutions);
    //hist1d[84]->Fill(meta);
    //auto mphi = std::ceil(meanPhi(phis));
	//auto mphi = std::ceil(meanPhi(phis,resolution));
    auto mphi = meanPhi(phis,resolutions);
	//vector<float> phiTest{25,50,255,240};
//	hist1d[83]->Fill(mphi);      //meanPhi(phiTest));
	auto mtime = mean(ebtimes,resolutions);
  //  hist1d[85]->Fill(mtime);

	//std::cout << "In getRhGrpEigen_sph w/ meta " << meta << " : mphi " << mphi << " : mt " << mtime << std::endl;

	for( uInt it(0); it < ebtimes.size(); it++ ){

		float leta = etas[it]-meta;
        float lphi = dPhi(phis[it],mphi);
		if( leta == 0 && lphi == 0 ) continue;
        float ltim = ebtimes[it]-mtime;
		float angle = getAngle( leta, lphi );
        //if( meta < 0 ) angle += PI;
		//if( ltim < 0 ) angle += PI;
		//if( angle > twoPI ) angle -= twoPI;
		angles.push_back(angle);
	//std::cout << "In getRhGrpEigen_sph w/1 leta " << leta << " : lphi " << lphi << " : ltim " << ltim << " : mag " << resolutions[it] << " : angle " << angle << std::endl;
		//wts.push_back(ltim*energies[it]/sumenergy);
        wts.push_back(ltim*ltim*resolutions[it]);
        //hist1d[69]->Fill(leta);
        //hist1d[70]->Fill(lphi);
        //hist1d[71]->Fill(ltim);
        //hist1d[72]->Fill(angle);
	}//<<>>for( uInt it(0); it < ebtimes.size(); it++ )

	//for( int ia = -50; ia < 51; ia++ ){ for( int ja = -50; ja < 51; ja++ ){ auto a=getAngle(ia,ja); hist1d[74]->Fill(a); hist1d[80]->Fill(std::sin(a));}}
    //for( int ia = 1; ia < 360; ia++ ){ for( int ja = 1; ja < 360; ja++ ){ auto a=dPhi(ia,ja); hist1d[82]->Fill(a);}}

    auto eigens =  getRhGrpEigen( angles, wts );//0 x, 1 y, 2 values

	//float eigncot = eigens[0]/eigens[1];
    //float eignsin = 1/(std::sqrt(1+eigncot*eigncot));
	//std::cout << "In getRhGrpEigen_sph eigen0 " << eigens[0] << " : eigen1 " << eigens[1] << " sin : " << eignsin << std::endl;
    //float eigncos = std::qrt(1.0 - eignsin*eignsin);
    auto rotangle = getAngle(eigens[0], eigens[1]);
    //hist1d[73]->Fill(rotangle);
	//hist1d[78]->Fill(eigens[0]);
	//hist1d[79]->Fill(eigens[1]);
    //if( not ebp ){ rotangle += PI; if( rotangle > TWOPI ) rotangle -= TWOPI; }
	//hist1d[74]->Fill(rotangle);
	float eignsin = std::sin(rotangle);
	float eigncos = std::cos(rotangle);
	auto orgsin = eignsin;
	auto orgcos = eigncos;

	// ----------------------
	// aligning eigen to point same direction
	// ----------------------
    float totRes(0.0);
	float ltsum(0.0);
	for( uInt it(0); it < wts.size(); it++ ){

        float leta = etas[it] - meta;
        float lphi = dPhi(phis[it],mphi);
		float ltime = ebtimes[it]-mtime;
		auto xcor = eigncos*(leta) - eignsin*(lphi);
		//if( xcor > 0 ) ltsum += ltime*resolutions[it];
		ltsum += ltime*resolutions[it]/xcor;	
        totRes += resolutions[it];	

	}//<<>>for( uInt it(0); it < wts.size(); it++ )

	//auto oreignsin = eignsin;
    //auto oreigncos = eigncos;
	if( ltsum/totRes < 0 ){ 

		//eigens[0] *= -1; 
		//eigens[1] *= -1;
    	rotangle = getAngle(-1*eigens[0], -1*eigens[1]);
		eignsin = std::sin(rotangle);
		eigncos = std::cos(rotangle);

	}//if( ebp && ebn && ltsum < 0 )

	//if( not ebn && ebp ){
    //    eigens[0] *= -1;
    //    eigens[1] *= -1;
    //    rotangle = getAngle(eigens[0], eigens[1]);
    //    eignsin = std::sin(rotangle);
    //    eigncos = std::cos(rotangle);
    //}//if( not ebp && ebn ) 

	//hist1d[73]->Fill(rotangle);
	//hist1d[78]->Fill(eigens[0]);
	//hist1d[79]->Fill(eigens[1]);

	// -----------------------------------------
	// finding nemo ( slope )
	// -----------------------------------------
    auto nWts = resolutions.size();
	//auto nWts = wts.size();
	//bool nwtcut = nWts > 10;
    //bool pfcut = true;
	//bool pfcut = eigens[2] < 0.95 && eigens[2] > 0.6;
    //bool pfcut = eigens[2] > 0.6 && nwtcut && eigens[2] < 0.95;

	vector<float> xs;
    vector<float> xs2;
	float xsum(0.0);
    float xsum2(0.0);
    for( uInt it(0); it < nWts; it++ ){

        float leta = etas[it] - meta;
        float lphi = dPhi(phis[it],mphi);
        float ltime = ebtimes[it]-mtime;
        auto sxcor = eigncos*(leta*2.2) - eignsin*(lphi*2.2);
        auto sycor = eignsin*(leta*2.2) + eigncos*(lphi*2.2);
        auto xcor = orgcos*(leta*2.2) - orgsin*(lphi*2.2);
        auto ycor = orgsin*(leta*2.2) + orgcos*(lphi*2.2);
        //if( not ebp ) xcor *= -1;
        if( false ) std::cout << "In getRhGrpEigen_sph w/2 leta " << leta << " : lphi " << lphi << " : xcor " << xcor << " : ycor " << ycor << " : dt " << wts[it] << std::endl;
        if( false ) std::cout << "In getRhGrpEigen_sph w/2 leta " << leta << " : lphi " << lphi << " : sxcor " << sxcor << " : sycor " << sycor << " : dt " << wts[it] << std::endl;
        //if( abs(wts[it]) < 8 )
        //auto fill = ltime*resolutions[it];
        //if( nwtcut ){
        //if( pfcut ){
        //	hist2d[73]->Fill(sxcor,sycor,fill);
        //	hist2d[74]->Fill(sxcor,sycor,resolutions[it]);
        //	hist2d[75]->Fill(leta,lphi,fill);
        //	hist2d[76]->Fill(leta,lphi,resolutions[it]);
		//}//<<>>if( std::abs(fill) < x )
		//if( pfcut ){
//			hist2d[86]->Fill(sycor,ltime,resolutions[it]);
//			hist2d[87]->Fill(sxcor,ltime,resolutions[it]);
        //}//<<>>if( pfcut )
//			if( sxcor > -1 && sxcor <= 0 ) hist1d[94]->Fill(ltime,resolutions[it]);
//        	if( sxcor < 1 && sxcor >= 0 ) hist1d[95]->Fill(ltime,resolutions[it]);
//        	if( sxcor > -2 && sxcor <= -1 ) hist1d[96]->Fill(ltime,resolutions[it]);
//        	if( sxcor < 2 && sxcor >= 1 ) hist1d[97]->Fill(ltime,resolutions[it]);
//        	if( sxcor > -3 && sxcor <= -2 ) hist1d[98]->Fill(ltime,resolutions[it]);
//        	if( sxcor < 3 && sxcor >= 2 ) hist1d[99]->Fill(ltime,resolutions[it]);
        //}//<<>>if( pfcut )
		//if( xcor < minx ) { minx = xcor; miny = ltime; }
        //if( xcor > maxx ) { maxx = xcor; maxy = ltime; }
		//xs.push_back(ltime/dxcor);
        //if( ltime/leta  > 0 ){
        	auto sl2 = (ltime*1000)/(sxcor);
			auto sl = (ltime*1000)/(leta*2.2);// changed to eta to look at slope in eta direction
			xs.push_back(sl);
            xs2.push_back(sl2);
			//hist1d[100]->Fill(sl*resolutions[it]);
            xsum += sl*resolutions[it];
			xsum2 += sl2*resolutions[it]; 
		//}//<<>>if( std::abs(leta) > 1 )
		//xs[it] = xcor;
		//ys.push_back(fill);
		//ys[it] = ltime;
		//graph->AddPoint(xcor,fill);
    }//<<>>for( uInt it(0); it < wts.size(); it++ )

	//xs[0] = minx; ys[0] = miny;
    //xs[1] = maxx; ys[1] = maxy;
	//auto graph = new TGraph( nWts, xs, ys );
	//graph->Fit("pol1","Q");
	//auto pol1 = graph->GetFunction("pol1");
	//auto slope = pol1->GetParameter(1);
	//float xsum(0); for( uInt it(0); it < nWts; it++ ){ xsum += xs[it]*resolutions[it]; hist1d[100]->Fill(xs[it]);}
	//auto nXSum = xs.size();
	auto slope = xsum/totRes;
    auto slope2 = xsum2/totRes;
	auto varsl = var(xs,slope,resolutions,totRes);
    //auto varsl2 = var(xs2,slope2,resolutions,totRes);
	auto chi2 = chisq(xs,slope,varsl);
    auto chi22 = chisq(xs,slope,varsl);
    auto chi2pf = 1 - TMath::Prob(chi2, nWts);
    auto chi2pf2 = 1 - TMath::Prob(chi22, nWts);
	eigens.push_back(slope);//3
    eigens.push_back(chi2pf);//4
    eigens.push_back(slope2);//5
    eigens.push_back(chi2pf2);//6

	//auto slope_cut = ( eigens[2] < 0.95 ) && ( eigens[2] > 0.6 )  && ( chi2pf > 0.95 ) && ( nXSum > 5 ) ;
    //if( slope_cut ){
//    	hist1d[91]->Fill(slope);
 //   	hist1d[92]->Fill(chi2pf);
 //  	 	hist1d[103]->Fill(std::abs(1000/slope));
 //   	hist2d[91]->Fill(slope,chi2pf);
 //   	hist2d[92]->Fill(slope,eigens[2]);
 //   	hist2d[93]->Fill(slope,rotangle);
 //   	hist2d[94]->Fill(slope,nXSum);
 //       hist2d[95]->Fill(chi2pf,eigens[2]);
 //       hist2d[97]->Fill(chi2pf,nXSum);
 //       	hist2d[119]->Fill(slope,slope2);
 //       hist2d[120]->Fill(slope,slope-slope2);
	//}//<<>>if( slope_cut )

	return eigens;
}//>>>>vector<float> LLPgammaAnalyzer_MINI::getRhGrpEigen_sph( vector<float> times, rhGroup rechits ){

vector<float> LLPgammaAnalyzer_MINI::getRhGrpEigen_ep( vector<float> times, rhGroup rechits ){

    // N 3.64, C 0.3000  s^2 = (N/(rhe))^2 + 2C^2

    float N(3.64);
    float C(0.3000);
    float tdis(2.2/SOL);

    vector<float> wts;
    vector<float> etas;
    vector<float> phis;
    vector<float> ebtimes;

    for( uInt it(0); it < rechits.size(); it++ ){

        const auto recHitID = getRawID(rechits[it]);
        const auto & idinfo = DetIDMap[recHitID];
        if( idinfo.ecal == ECAL::EB ){
            //auto wt = 1/std::sqrt(sq2(N/rechits[it].energy())+2*sq2(C)); 
            auto wt = 1/(sq2(N/rechits[it].energy())+2*sq2(C));
            wts.push_back(wt);
            //const auto recHitPos = barrelGeometry->getGeometry(recHitId)->getPosition();
            const auto rhEtaPos = tdis*idinfo.i2;//recHitPos.eta();
            etas.push_back(rhEtaPos);
//			hist1d[69]->Fill(rhEtaPos);
            const auto rhPhiPos = tdis*idinfo.i1;//recHitPos.phi();
  //          hist1d[70]->Fill(rhPhiPos);
            phis.push_back(rhPhiPos);
            ebtimes.push_back(times[it]);
        }//<<>>if( idinfo.ecal == ECAL::EB )
    }//<<>>for( uInt it(0); it < rechits.size(); it++ )

    return getRhGrpEigen( etas, phis, wts );
}//>>>>vector<float> LLPgammaAnalyzer_MINI::getRhGrpEigen_ep( vector<float> times, rhGroup rechits ){

float LLPgammaAnalyzer_MINI::getdt( float t1, float t2 ){
if(DEBUG) cout << "getdt" << endl;
if(DEBUG) cout << " t1: " << t1 << " t2: " << t2 << endl;	
	auto dt = t1 - t2;
if(DEBUG) cout << "dt: " << dt << endl;	
	if( dt == 0.0 )	dt = -5.5;// to be commented out in final version
	if( t1 == 0.0 || t2 == 0.0 ) dt = -5.0;// to be copmmented out in final version
	if( t1 < -28.9 || t2 < -28.9 ) dt = -3.75;
	return dt;
}//>>>>float LLPgammaAnalyzer_MINI::getdt( float t1, float t2 )


void LLPgammaAnalyzer_MINI::mrgRhGrp( rhGroup & x, rhGroup & y ){

	//std::cout << " --- In mrgRhGrp " << std::endl;
	bool matched(false);
	for(const auto rhy : y ){ 
		for(const auto rhx : x ){ if( rhMatch(rhx,rhy) ) matched = true; }
		if( not matched ) x.push_back(rhy);
	}//<<>>for(const auto rhx : y )
   //std::cout << " ---- Finished mrgRhGrp " << std::endl;

	return;
}//>>>>rhGroup LLPgammaAnalyzer_MINI::mrgRhGrp(const rhGroup x, const rhGroup y)


bool LLPgammaAnalyzer_MINI::reduceRhGrps( vector<rhGroup> & x ){

	//std::cout << " --- In reduceRhGrps " << std::endl;
	uInt s = x.size();
	uInt a(0), b(0);
	bool match(false);
   	for( uInt ita(0); ita < s; ita++ ){ 
		for( auto itb = ita+1; itb < s; itb++ ){ 
			if( dupRhFnd(x[ita], x[itb]) ){ a = ita; b = itb; match = true; } 
			if(match) break;
   	}//<<>>for( int itb = ita+1; itb < s; itb++ ) 
		if(match) break;
	}//<<>>for( int ita = 0; ita < nBcRhGroups; ita++ )
	if(match){
		mrgRhGrp( x[a], x[b] );
		x.erase(x.begin()+b);
	} //<<>>if(match) : else
	//std::cout << " ---- Finished reduceRhGrps " << std::endl;

	return match;
}//>>>>vector<rhGroup> LLPgammaAnalyzer_MINI::reduceRhGrps(const vector<rhGroup> x)

vector<float> LLPgammaAnalyzer_MINI::kidTOFChain( std::vector<reco::CandidatePtr> kids, float cx, float cy, float cz  ){

    vector<float> result;
    vector<float> kidtime;
    vector<float> kide;
	vector<float> impact;
	bool llpcut(false);
	float nextBX(0.0);
	float llpkid(0.0);
    for( auto kid : kids ){

        bool done(false);
        bool first(true);
        bool llp(false);
        bool bquark(false);
        auto mom = kid->mother();
        vector<double> stepp{kid->p()};
        vector<double> stepe{kid->energy()};
        vector<double> orignvx;
        vector<double> orignvy;
        vector<double> orignvz;
        bool top(true);
		int steps(0);
        while( not done ){
			auto pdgid = std::abs(mom->pdgId());
			if( pdgid == 6000113 ) llp = true;
			if( pdgid == 5 ) bquark = true;
			if( std::floor(pdgid/100) == 5 ) bquark = true;
            if( mom->pt() == 0 ) done = true;
            else {
                auto gmom = mom->mother();
                if( top ){
                    if( first ) first = false;
                    else {
                        stepp.push_back(mom->p());
                        stepe.push_back(mom->energy());
                    }//<<>>if( first )
					steps++;
					//if( llp || bquark ) llpstep.push_back(1.0); else llpstep.push_back(0.0);
                    orignvx.push_back(mom->vx());
                    orignvy.push_back(mom->vy());
                    orignvz.push_back(mom->vz());
                }//<<>>if( top )
                if( gmom->pt() == 0 ) done = true;
                else {
                    if( mom->vx() == gmom->vx() ) top = false;
                    else top = true;
                    mom = gmom;
                }//<<>>if( gmom->pt() == 0 )
            }//<<>>if( mom->pt() == 0 )                 
        }//<<>>while( not done )
        first = true;

        //kide.push_back(stepe[0]);
        if( DEBUG && false ) std::cout << " ---- jetGenTime Calc Steps : " << steps << std::endl;
		bool stepcut( steps == 0 );
		//llpcut = llp || bquark;
		if( DEBUG && false ) std::cout << " ---- jetGenTime Calc llpcut : " << llpcut << std::endl;
        //auto gencut = stepcut && not llpcut;
		//auto gencut = stepcut || not llpcut;
        //if( stepcut ){ 
		//	kidtime.push_back(999); 
		//	impact.push_back(999);
		//	//std::cout << "Steps are Zero" << std::endl; 
		//} else {
		if( not stepcut ){
			float maxe(0.0);
            float totaltime(0.0);
            auto destvx(cx);
            auto destvy(cy);
            auto destvz(cz);
            for( int it(0); it < steps; it++ ){
				if( stepe[it] > maxe ) maxe = stepe[it];
                auto beta = stepp[it]/stepe[it];
                auto legtime = hypo( (destvx-orignvx[it]), (destvy-orignvy[it]), (destvz-orignvz[it]) )/(SOL*beta);
				totaltime += legtime;
				//std::cout << "Sum path legs beta: " << beta << " x: " << (destvx-orignvx[it]) << " y: " << (destvy-orignvy[it]);
				//std::cout << " z: " << (destvz-orignvz[it]) << " legtime: " << legtime << " totaltime: " << totaltime << std::endl;
                //std::cout << " -- dest : " << destvx << ", " << destvy << ", " << destvz; 
                //std::cout << " orig : " << orignvx[it] << ", " << orignvy[it] << ", " << orignvz[it] << std::endl;
                destvx = orignvx[it];
                destvy = orignvy[it];
                destvz = orignvz[it];
            }//<<>>for( int it(0); it < steps; it++ )
			auto r = hypo((cx-orignvx[0]), (cy-orignvy[0]));
			auto iangle = getAngle((cz-orignvz[0]),r);	
			//std::cout << "getAngle: " << iangle << " r: " << r << " z: " << cz-orignvz[0] << " x: " << cx-orignvx[0];
			//std::cout << " y: " << cy-orignvy[0] << std::endl; 
			if( totaltime > 25.0 ) {
				nextBX = 1.0;
			} else {
				impact.push_back(iangle);
            	kidtime.push_back(totaltime);
				kide.push_back(maxe);
				llpcut = llpcut || llp || bquark;
				if( llp || bquark ) llpkid++;
			}//<<>>if( totaltime > 25.0 )
        }//<<>>if( steps == 0 )

    }//<<>>for( auto kid : kids )

	auto mtime(-30.0);
	auto mangle(7.0);
	auto tvar(-2.0);
	auto ktsize = kidtime.size();
	//if( llpcut && ( ktsize > 0 ) ){
    if( ktsize > 0 ){
		if( ktsize > 1 ){
			mtime = mean(kidtime,kide);
			mangle = mean(impact,kide);
			tvar = var(kidtime,mtime,kide);
		} else {
			mtime = kidtime[0];
			mangle = impact[0];
			tvar = -1.0;
		}//<<>>if( kidtime.size() > 1 )
	}//<<>>if( llpcut )
	result.push_back(mtime);//0
	result.push_back(mangle);//1
	result.push_back(tvar);//2
	result.push_back(nextBX);//3
	if( llpcut ) result.push_back(1.0); else result.push_back(0.0);//4
	result.push_back(llpkid/ktsize);//5
    result.push_back(ktsize);//6
    return result;

}//>>>>vector<float> LLPgammaAnalyzer_MINI::kidTOFChain( std::vector<reco::CandidatePtr> kids, float cx, float cy, float cz  )

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

// ------------ method called for each event	------------
void LLPgammaAnalyzer_MINI::analyze(const edm::Event& iEvent, const edm::EventSetup& iSetup){

	using namespace edm;

// -- Consume Tokens --------------------------------------------
	if( DEBUG ) std::cout << "Consume Tokens -------------------------------------------- " << std::endl;

	// TRIGGER
	//iEvent.getByToken(triggerResultsToken_,triggerResults_);
	//iEvent.getByToken(triggerObjectsToken_,triggerObjects_);

	// TRACKS
	iEvent.getByToken(tracksToken_, tracks_);

	// PFCANDIDATES
	iEvent.getByToken(pfcand_token_, pfcands_);

    //GEN PARTICLES
	//if( hasGenInfo ) iEvent.getByToken(genpart_token_, genpart_);

	// VERTICES
	iEvent.getByToken(verticesToken_, vertices_);

	// RHO
	//iEvent.getByToken(rhoToken_, rho_);

	// METS
	//iEvent.getByToken(metsToken_, mets_);

	// SUPERCLUSTERS
	iEvent.getByToken(scToken_, superCluster_);  
	iEvent.getByToken(ootScToken_, ootSuperCluster_);

	// CALOCLUSTERS
	iEvent.getByToken(ccToken_, caloCluster_);

	// JETS
	iEvent.getByToken(jetsToken_, jets_);

	// LEPTONS & PHOTONS
	iEvent.getByToken(electronsToken_, electrons_);
	iEvent.getByToken(muonsToken_, muons_);

	// PHOTONS
	iEvent.getByToken(gedPhotonsToken_, gedPhotons_);
	iEvent.getByToken(ootPhotonsToken_, ootPhotons_);

	// ECAL RECHITS
	iEvent.getByToken(recHitsEBToken_, recHitsEB_);
	iEvent.getByToken(recHitsEEToken_, recHitsEE_);

	// GEOMETRY : https://gitlab.cern.ch/shervin/ECALELF
	iSetup.get<CaloGeometryRecord>().get(caloGeo_); 
	barrelGeometry = caloGeo_->getSubdetectorGeometry(DetId::Ecal, EcalSubdetector::EcalBarrel);
	endcapGeometry = caloGeo_->getSubdetectorGeometry(DetId::Ecal, EcalSubdetector::EcalEndcap); 

// -- Process Objects ------------------------------------------

// -- event information 

   	const auto run   = iEvent.id().run();
   	const auto lumi  = iEvent.luminosityBlock();
   	const auto event = iEvent.id().event();
    if( DEBUG ) std::cout << "******************************************************************************************************" << std::endl;
	if( DEBUG ) std::cout << "Processing event: " << event << " in run: " << run << " and lumiblock: " << lumi << std::endl;

// -- Process Prime Vertix
	const auto & primevtx = vertices_->front();
	
	vtxX = primevtx.position().x();
	vtxY = primevtx.position().y();
	vtxZ = primevtx.position().z();
	
// ---- process jets --------------------------

// ** extracted from disphoana : starting point **** not all functios/varibles defined ***************
// ** for example only -- convert to nano?, use ewkino varibles for output, find rechit information ** 

	std::vector<pat::Jet> fjets;
   	std::vector<reco::CaloCluster> fbclusts;

	//jets.clear(); 
	//jets.reserve(jets_->size());

	// Selection criteria declartation
	// deltaRmin from ecalelf SC Associtor : 0.2
	float	deltaRminJet	= 0.3;//0.4
	//float deltaRminKid	= 0.15;//0.2
	//float 	jetPTmin		= 200.0;
    float   jetPTmin        = 100.0;// for energy/time comp
	int 	jetIDmin		= 2; //3;
    //int     jetIDmin        = 1; //3; for energy/time comp
	float 	jetETAmax		= 1.5; //1.5;
	uInt 	minRHcnt		= 5; //32;
	float 	minRHenr		= 2.0;
   	//float 	bcMinEnergy		= 0.667;
   	//uInt 	bcMinRHGrpSize	= 3;
	float 	minEmf			= 0.0;//0.2
    //bool    useJetSC        = true; // for cluster eigen calcs
    //bool    useJetSC		= false; // for cluster eigen calcs use photon instead

	if( DEBUG ) std::cout << "Filter Jets" << std::endl;
	for(const auto& jet : *jets_ ){ // Filters jet collection & sorts by pt
		
		if (jet.pt() < jetPTmin) continue;
	  	if (std::abs(jet.eta()) > jetETAmax) continue;
	  
	  	const auto jetID = getPFJetID(jet);
	  	if (jetID < jetIDmin) continue;

	  	// save the jets, and then store the ID
	  	fjets.emplace_back(jet);
	  	fjets.back().addUserInt("jetID",jetID);
	  
	  	std::sort(fjets.begin(),fjets.end(),sortByPt);
	}//<<>>for(const auto& jet : *jets_ )
	
	if( DEBUG ) std::cout << "Collecting Calo Clusters" << std::endl;
	for(const auto& bclust : *caloCluster_ ) fbclusts.push_back(bclust);

	if( DEBUG ) std::cout << "Processing RecHits" << std::endl;
	nRecHits = 0;
	rhPosX.clear();
	rhPosY.clear();
	rhPosZ.clear();
	rhPosEta.clear();
	rhPosPhi.clear();
	rhEnergy.clear();
	rhTime.clear();
	rh_distToPV.clear();
	rhTimeErr.clear();
	rhTOF.clear();
	rhID.clear();



if(DEBUG)std::cout << "-----------------------------------------------------------" << std::endl;
if(DEBUG)std::cout << "Starting RecHit loop"  << std::endl; 
	std::vector<EcalRecHit> _rechits;
	for(const auto rechit : *recHitsEB_) _rechits.push_back(rechit);
	nRecHits = (int)_rechits.size();
	for(const auto rechit : _rechits){
		const auto rhid = getRawID(rechit);
		auto Rh_pos = barrelGeometry->getGeometry(rechit.detid())->getPosition();
	
		auto Rh_x = Rh_pos.x();
		auto Rh_y = Rh_pos.y();
		auto Rh_z = Rh_pos.z();
		auto Rh_eta = Rh_pos.eta();
		auto Rh_phi = Rh_pos.phi();

		//do central time correction - add time it took to fly to detector face so all RHs are in same time frame
		double ctc = hypo(Rh_x,Rh_y,Rh_z);
		rhTime.push_back(rechit.time() + ctc/SOL);
		//distance from RH to PV
		double d_pv = hypo(Rh_x-vtxX,Rh_y-vtxY,Rh_z-vtxZ);
		rh_distToPV.push_back(d_pv);

		rhID.push_back(rhid);	
		rhPosX.push_back(Rh_x);	
		rhPosY.push_back(Rh_y);	
		rhPosZ.push_back(Rh_z);	
		rhPosEta.push_back(Rh_eta);
		rhPosPhi.push_back(Rh_phi);
		
		rhTimeErr.push_back(rechit.timeError());
		rhEnergy.push_back(rechit.energy());
}	

    if( DEBUG ) std::cout << "Init for Jet Loop with " << nJets << " jets"<< std::endl;

	//auto nUnJets = (*jets_).size();
	nJets = fjets.size();
	uInt nGoodDrJets(0);
   	//uInt nGoodScJets(0);
   	//uInt nGoodBcJets(0);
	//set the number of leading jets to skim ( = nJets for all )
	//auto stJets = nJets; 

	jetE.clear();
	jetPt.clear(); 
	jetPhi.clear(); 
	jetEta.clear(); 
	jetEtaetaMmt.clear();
	jetPhiphiMnt.clear();
	jetEtaphiMnt.clear();
	jetMaxD.clear();
	jetConPtDis.clear();
	jetConEtaPhiSprd.clear();
	jetArea.clear();
	jetNCarry.clear();
	jetNConst.clear();

	jetID.clear();
	jetNHF.clear();
	jetNEMF.clear();	
	jetCHF.clear();
	jetCEMF.clear();
	jetMUF.clear();
	jetNHM.clear();
	jetCHM.clear();

	jetMuTime.clear();
	jetTimeError.clear();
	jetTimeRMS.clear();
	jetMedTime.clear();
	jetSCMuTime.clear();
	jetSCMedTime.clear();
	jetCMuTime.clear();
	jetCMedTime.clear();
	jetCSCMuTime.clear();
	jetCSCMedTime.clear();
	jetCBCMuTime.clear();
	jetCBCMedTime.clear();
    jetPhMuTime.clear();
    jetOOTPhMuTime.clear();
    jetEleMuTime.clear();

	jetPHM.clear();
	jetELM.clear();
	jetC.clear();
	jetPHE.clear();
	jetPHEF.clear();
	jetELE.clear();
	jetELEF.clear();
	jetMUE.clear();
	jetCharge.clear();

	njetRecHits.clear();
	jetRecHitOfJet.clear();
	jetRecHitId.clear();

	njetKids.clear();
	jetKidOfJet.clear();
	jetKidE.clear();
	jetKidPt.clear();
	jetKidPhi.clear();
	jetKidEta.clear();
	jetKidPdgID.clear();
	jetKidCharge.clear();
	jetKid3Charge.clear();
	jetKidLLP.clear();
	jetKidMass.clear();
	jetKidVx.clear();
	jetKidVy.clear();
	jetKidVz.clear();

	njetSubs.clear();
	
	float jetHt(0.0);
	
	
//////////  reffrence information
// **** const reco::TrackRefVector& associatedTracks() const;
// **** const reco::PixelClusterTagInfo* tagInfoPixelCluster(const std::string& label = "") const; // from b tagging info methods
// **** reco::PFCandidateFwdPtrVector const& pfCandidatesFwdPtr() const { return pfCandidatesFwdPtr_; }

//	hist1d[18]->Fill(nJets);
	bool goodJetEvent(false);
if(DEBUG)std::cout << "-----------------------------------------------------------" << std::endl;
if(DEBUG)std::cout << "Starting Jet Loop for " << nJets << " jets " << std::endl; 
	for ( uInt ijet(0); ijet < nJets; ijet++ ){ 
		//std::cout << "Processing with jet " << ijet << std::endl;
	   	const auto & jet = fjets[ijet];
//moved place - only dealing with jets dR = 0.3 matched to rhGroup with >5 rechits
	   	if( DEBUG ) std::cout << "Getting jet dR rechit group" << std::endl; 
		auto jetDrRhGroup = getRHGroup( jet.eta(), jet.phi(), deltaRminJet, minRHenr ); 
		auto rhCount = jetDrRhGroup.size();

	   	//std::cout << "rhCount is " << rhCount << std::endl;
	   	auto sumdrrhe = getRhGrpEnr( jetDrRhGroup );
		auto dremf = sumdrrhe/jet.energy();
		//float jetDrTime(-99.9);
//cout << "1 ijet: " << ijet << " nJets: " << nJets << " nGoodDrJets: " << nGoodDrJets << endl;
		if( rhCount >= minRHcnt && dremf > minEmf ){
		// places jet info in output tree

	   	// jetID in jet.h ?
		nGoodDrJets++;
	//cout << "jet #: " << ijet << " ngoodjets: " << nGoodDrJets << " out of "  << nJets << " total" << endl;
	   	jetHt += jet.pt();

	   	jetE.push_back(jet.energy());
	   	jetPt.push_back(jet.pt());
	   	jetPhi.push_back(jet.phi());
	   	jetEta.push_back(jet.eta());
		jetEtaetaMmt.push_back(jet.etaetaMoment());
		jetPhiphiMnt.push_back(jet.phiphiMoment());
		jetEtaphiMnt.push_back(jet.etaphiMoment());
		jetMaxD.push_back(jet.maxDistance());
		jetConPtDis.push_back(jet.constituentPtDistribution());
		jetConEtaPhiSprd.push_back(jet.constituentEtaPhiSpread());
		jetArea.push_back(jet.jetArea());
		jetNCarry.push_back(jet.nCarrying(0.1));
		jetNConst.push_back(jet.nConstituents());

	   	jetID.push_back(jet.userInt("jetID"));
	   	jetNHF.push_back(jet.neutralHadronEnergyFraction());
	   	jetNEMF.push_back(jet.neutralEmEnergyFraction());
	   	jetCHF.push_back(jet.chargedHadronEnergyFraction());
	   	jetCEMF.push_back(jet.chargedEmEnergyFraction());
	   	jetMUF.push_back(jet.muonEnergyFraction());
	   	jetNHM.push_back(jet.neutralMultiplicity());
		jetCHM.push_back(jet.chargedMultiplicity());
		jetCharge.push_back(jet.jetCharge());

		jetPHE.push_back(jet.photonEnergy());
		jetPHEF.push_back(jet.photonEnergyFraction()); 
		jetELE.push_back(jet.electronEnergy());
		jetELEF.push_back(jet.electronEnergyFraction());
		jetMUE.push_back(jet.muonEnergy());
		jetPHM.push_back(jet.photonMultiplicity());
		jetELM.push_back(jet.electronMultiplicity());

   
	   	if( DEBUG ) std::cout << "Fill jet pt/phi/eta Histograms" << std::endl;
 		
      	//const auto jetepafrac 	= jet.photonEnergyFraction() + jet.electronEnergyFraction();
      	//const auto jetepe 		= jet.photonEnergy() + jet.electronEnergy();
	//	const auto jeteme 		= jet.chargedEmEnergy() + jet.neutralEmEnergy();
      	//const auto jetemfrac 	= jeteme/jet.energy();
//      	const auto jetepfrac 	= jetepe/jet.energy();


        // dR matched RH group  -----------------------------------------------
		// --------------------------------------------------------------------
//original place
			if( DEBUG ) std::cout << " - get jetDRtofTimes " << std::endl;
	   		auto tofTimes = getLeadTofRhTime( jetDrRhGroup, vtxX, vtxY, vtxZ );
	   		//auto leadJetRh = getLeadRh( jetDrRhGroup );

			

/*
			//do pvTimes stuff
			auto leadRh_ID = leadJetRh.detid();
			auto leadRh_pos = barrelGeometry->getGeometry(leadRh_ID)->getPosition();
		
			auto leadRh_x = leadRh_pos.x();
			auto leadRh_y = leadRh_pos.y();
			auto leadRh_z = leadRh_pos.z();
			
			const auto d_rh = hypo(leadRh_x, leadRh_y, leadRh_z);
			const auto d_pv = hypo(leadRh_x-vtxX, leadRh_y-vtxY, leadRh_z-vtxZ);
	   		
			float dist = d_rh - d_pv;
			pvTimes.push_back(dist/SOL);
			*/
		if(DEBUG)std::cout << "Starting RecHit Loop" << std::endl;
		if(DEBUG)cout << "jet #: " << ijet << " has " << rhCount << " recHits" << endl;
			for ( uInt irhg = 0; irhg < rhCount; irhg++){
			
				//std::cout << " -- irhg: " << irhg << " rhCount: " << rhCount << std::endl;
				jetRecHitOfJet.push_back(nGoodDrJets-1);
				auto detid = (jetDrRhGroup[irhg]).detid();
				//std::cout << " -- (jetDrRhGroup[irhg]).detid(): " << detid.rawId() << std::endl;
		      	jetRecHitId.push_back(detid.rawId());	
			//	auto rhtime = tofTimes[irhg];
			//std::cout << " -- tofTimes[irhg]: " << rhtime << std::endl;
		      	//fillTH1(rhtime,hist1d[0]);//hist1d[0]->Fill(rhtime);
				//auto rhe = (jetDrRhGroup[irhg]).energy();
		      	//std::cout << " -- jetDrRhGroup[irhg]).energy(): " << rhe << std::endl;
//		      	hist2d[38]->Fill(rhtime, rhe);
		   	}//<<>>for ( uInt irhg = 0; irhg < rhCount; irhg++)
			if(DEBUG) std::cout << "evt: " << event << " nrechits in jet: " << rhCount << endl;
		   	//const auto leadJetRhId = leadJetRh.detid();
		   	//const auto leadJetRhIdPos = barrelGeometry->getGeometry(leadJetRhId)->getPosition();
		   //	auto sc_eta = leadJetRhIdPos.eta();
		   //	auto sc_phi = leadJetRhIdPos.phi();
		   //	auto sc_enr = leadJetRh.energy();
		   	//std::cout << "Lead Jet dR RH Group E: " << sc_enr << " eta: " << sc_eta << " phi: " << sc_phi << std::endl;

	
			//  make jettime varible
//	      	for( auto t : tofTimes ) hist1d[5]->Fill(t);
			auto jetTimeStats = getTimeDistStats( tofTimes, jetDrRhGroup ); 
			auto jmutime = jetTimeStats[0];
		   	auto jterr = jetTimeStats[1];
		   	auto jtrms = jetTimeStats[4];
			auto jmedtime = jetTimeStats[2];
	      	//auto mederr = jetTimeStats[3];
			auto jcmutime = jetTimeStats[6];
	      	auto jcmedtime = jetTimeStats[10];

			//jetDrTime = jcmutime;
		   	njetRecHits.push_back(rhCount);
		   	jetMuTime.push_back(jmutime);
		   	jetTimeError.push_back(jterr);
		   	jetTimeRMS.push_back(jtrms);
		   	jetMedTime.push_back(jmedtime);
	      	jetCMuTime.push_back(jcmutime);	
	      	jetCMedTime.push_back(jcmedtime);

			//good dr jets are those matched to rh groups
			//if( jcmutime > -28.9 ) nGoodDrJets++;			
		}else{ //<<>>if( rhCount > minRHcnt && dremf > minEmf )
			njetRecHits.push_back(0);
	      		jetMuTime.push_back(-29.25);
	      		jetTimeError.push_back(99.9);
	      		jetTimeRMS.push_back(99.9);
	      		jetMedTime.push_back(-29.25);
	      		jetCMuTime.push_back(-29.25);
	      		jetCMedTime.push_back(-29.25);
		}	
		

		// GenJet Info for MC  -------------------------------------------------------------------
		// ---------------------------------------------------------------------------------------
/*

        if( DEBUG ) std::cout << "Getting jetGenParton Information" << std::endl;
        //const reco::GenParticle * jetGenParton(0);
        float jetGenImpactAngle(7.0);
        float jetGenTime(-99.9);
        float jetGenPt(-1.0);
        //float jetGenEta(10.0);
        float jetGenEnergy(-1.0);
        float jetGenEMFrac(-1.0);
        //float jetGenDrMatch(-1.0);
	//	float jetGenTimeVar(-1.0);
      // float jetGenTimeLLP(0.0);
      // float jetGenLLPPurity(-1.0);
      // float jetGenNextBX(-1.0);
      // float jetGenNKids(-1.0);

        if( hasGenInfo ){
            if( DEBUG ) std::cout << " -- Pulling jet gen info " << std::endl;
            auto jetGenParton = jet.genParton();
            auto jetGenJet = jet.genJet();
			if( rhCount > 0 ){
			if( jetGenJet ){
            	if( DEBUG ) std::cout << " ---- jetGenParton : " << jetGenParton << " genJet : " << jetGenJet << std::endl;
            	// size_t numberOfDaughters() const override;
            	// size_t numberOfMothers() const override;
            	// const Candidate * daughter(size_type) const override;
            	// const Candidate * mother(size_type = 0) const override;
            	// Candidates :
            	//  virtual double vx() const  = 0;
            	//  virtual double vy() const  = 0;
            	//  virtual double vz() const  = 0;
            	//  virtual int pdgId() const  = 0;
            	//  + mass/energy/momentum info ....

            	//auto nMother = jetGenJet->numberOfMothers();
            	//auto nDaughter = jetGenJet->numberOfDaughters();
            	auto nSources = jetGenJet->numberOfSourceCandidatePtrs();
            	if( DEBUG ) std::cout << " ---------------------------------------------------- " << std::endl;
            	//std::cout << " - jetGenJet mothers : " << nMother << " daughters : " << nDaughter << " sources : " << nSources << std::endl;
            	if( DEBUG ) std::cout << " - jetGenJet srcs : " << nSources << " PV (" << vtxX << "," << vtxY << "," << vtxZ << ")" << std::endl;
            	auto kids = jetGenJet->daughterPtrVector();
            	//std::cout << bigKidChase( kids, vtxX ) << std::endl;
            	//kidChase( kids, vtxX, vtxY, vtxZ );
				auto leadJetRh = getLeadRh( jetDrRhGroup );
            	auto leadJetRhId = leadJetRh.detid();
            	auto leadJetRhIdPos = barrelGeometry->getGeometry(leadJetRhId)->getPosition();
				auto cx = leadJetRhIdPos.x();
				auto cy = leadJetRhIdPos.y();
				auto cz = leadJetRhIdPos.z();
				auto tofcor = hypo( cx, cy, cz )/SOL;
				//if( DEBUG ) kidChase( kids, vtxX, vtxY, vtxZ );
            	auto genTime = kidTOFChain( kids, cx, cy, cz );
				//jetGenEta = jetGenJet->eta();
				if( genTime[0] > 25.0 ) jetGenTime = -28.0;
				else if( genTime[0] > -25.0 ) jetGenTime = genTime[0]-tofcor;
				else jetGenTime = -27.0;
				jetGenImpactAngle = genTime[1];
				jetGenPt = jetGenJet->pt();
                jetGenEnergy = jetGenJet->energy();
				jetGenEMFrac = (jetGenJet->chargedEmEnergy() + jetGenJet->neutralEmEnergy())/jetGenEnergy;
				//jetGenDrMatch = std::sqrt(reco::deltaR2(jet.eta(), jet.phi(), jetGenJet->eta(), jetGenJet->phi()));
				//jetGenTimeVar = genTime[2];
                //jetGenNextBX = genTime[3];
                //jetGenTimeLLP = genTime[4];
                //jetGenLLPPurity = genTime[5];
		//		jetGenNKids = genTime[6];
				if( DEBUG ){ 
					std::cout << " - jetGenJet GenTime : " << jetGenTime << " rhPos: " << cx << "," << cy << "," << cz; 
					std::cout << " Angle: " << jetGenImpactAngle << std::endl;
					std::cout << " -- Energy : " << jetGenEnergy << " Pt : " << jetGenPt << " EMfrac : " << jetGenEMFrac << std::endl;
				}//<<>>if( DEBUG )
			}//<<>>if( jetGenJet )
			else { if( DEBUG ) std::cout << " - jetGenJet GenTime : jetGenJet == 0 " << std::endl; jetGenTime = -50.0; }
			}//<<>>if( rhCount >= minRHcnt )

    
			else if( DEBUG ) std::cout << " - jetGenJet GenTime : rhCount == 0 " << std::endl;
			if( DEBUG ) std::cout << " ---------------------------------------------------- " << std::endl;

        }//<<>>if( hasGenInfo )
*/
	}//<<<<for ( uInt ijet(0); ijet < nJets; ijet++ )
	/*
	//-----------------------------------------------------------------------------------------------------
	// ***************************** d jetTime for back-to-back high pt jets	*****************************
	//auto dijetIdCut = 1;
	auto dijetPtMin = 200.0;
	auto difPtLmt = 0.8;
	auto htPctLmt = 0.8;
	auto dPhiLmt = 2.8;

	if( DEBUG ) std::cout << "Finding jet dt pairs" << std::endl;
	for ( uInt q = 0; q < nJets; q++ ){
		for ( uInt p = q+1; p < nJets; p++ ){
	
			if( DEBUG ) std::cout << " - filter jet pairs" << std::endl;
	      	const auto & qjet = fjets[q];
	      	const auto & pjet = fjets[p];
	      	if( qjet.pt() < dijetPtMin ) continue;
	      	auto diffPt = pjet.pt()/qjet.pt();
	//      	hist1d[24]->Fill(diffPt);
	      	if( diffPt < difPtLmt ) continue;
	      	auto htPct= (qjet.pt()+pjet.pt())/jetHt;
//			hist1d[25]->Fill(htPct);
	      	if( htPct < htPctLmt ) continue;
	      	auto dPhi = reco::deltaPhi(qjet.phi(),pjet.phi());
//	      	hist1d[26]->Fill(dPhi);
	      	if( dPhi < dPhiLmt ) continue;

			if( DEBUG ) std::cout << " - get jet pair dt" << std::endl;


		}//<<>>for ( uInt p = q+1; p < nJets; p++ )
	}//<<>>for ( uInt q = 0; q < nJets; q++ )
	//-------------------------------------------------------------------------------
*/
if(nJets < nGoodDrJets) cout << "2 nJets: " << nJets << " nGoodDrJets: " << nGoodDrJets << endl;
	nJets = nGoodDrJets;
	if( goodJetEvent ) nGoodJetEvents++;

	// -- Fill output trees ------------------------------------------
	if( DEBUG ) std::cout << "---------- Next Event -----" << std::endl;
	outTree->Fill();

	// -- EOFun ------------------------------------------------------
	//#ifdef THIS_IS_AN_EVENTSETUP_EXAMPLE
	//	 ESHandle<SetupData> pSetup;
	//	 iSetup.get<SetupRecord>().get(pSetup);
	//#endif
}//>>>>void LLPgammaAnalyzer_MINI::analyze(const edm::Event& iEvent, const edm::EventSetup& iSetup)


// ------------ method called once each job just before starting event loop	------------
void LLPgammaAnalyzer_MINI::beginJob(){
	// Global Varibles
	nGoodJetEvents = 0;

   	// Set up DetIdMap
   	DetIDMap = SetupDetIDs();

	// Book output files and trees
	edm::Service<TFileService> fs;
	outTree = fs->make<TTree>("llpgtree","llpgtree");

	// Book histograms
	
	//int jtdiv(400);
	//float jtran(8);
//    int jtdiv(625);
//    float jtran(25);
//	int jdtdiv(200);
//	float jdtran(4);
//   	int jztdiv(100);
//   	float jztran(2);
//	int rhcnt(80);

	// Create output Tree branches -----------------------------

	// Run, Lumi, Event info
	outTree->Branch("run", &run);
	outTree->Branch("lumi", &lumi);
	outTree->Branch("event", &event, "event/l");

	//PV info
	outTree->Branch("vtxX",&vtxX);
	outTree->Branch("vtxY",&vtxY);
	outTree->Branch("vtxZ",&vtxZ);

	// Jet info
	outTree->Branch("nJets", &nJets);
	outTree->Branch("jetE", &jetE);
	outTree->Branch("jetPt", &jetPt);
	outTree->Branch("jetEta", &jetEta);
	outTree->Branch("jetPhi", &jetPhi);
	outTree->Branch("jetID", &jetID);
	outTree->Branch("jetRecHitId",&jetRecHitId);
	outTree->Branch("jetRecHitOfJet",&jetRecHitOfJet);

	//rh info
	outTree->Branch("nRecHits",&nRecHits);
	outTree->Branch("rhPosX",&rhPosX);
	outTree->Branch("rhPosY",&rhPosY);
	outTree->Branch("rhPosZ",&rhPosZ);
	outTree->Branch("rhPosEta",&rhPosEta);
	outTree->Branch("rhPosPhi",&rhPosPhi);
	outTree->Branch("rhEnergy",&rhEnergy);
	outTree->Branch("rhTime",&rhTime);
	outTree->Branch("rh_distToPV",&rh_distToPV);
	outTree->Branch("rhTimeErr",&rhTimeErr);
	outTree->Branch("rhID",&rhID);

}//>>>>void LLPgammaAnalyzer_MINI::beginJob()


// ------------ method called once each job just after ending the event loop	------------
void LLPgammaAnalyzer_MINI::endJob(){

/*
    normTH1D(hist1d[15]);
    normTH1D(hist1d[16]);
    normTH1D(hist1d[39]);
    normTH1D(hist1d[40]);

	normTH1D(hist1d[48]);
    normTH1D(hist1d[49]);
    normTH1D(hist1d[41]);
    normTH1D(hist1d[42]);

    normTH1D(hist1d[27]);
    normTH1D(hist1d[28]);

    normTH1D(hist1d[59]);
    normTH1D(hist1d[60]);

	//hist2d[73]->Divide(hist2d[74]);
	thresDivTH2D( hist2d[73], hist2d[74], 100 );
    //hist2d[75]->Divide(hist2d[76]);
    thresDivTH2D( hist2d[75], hist2d[76], 100 );
    //hist2d[77]->Divide(hist2d[78]);
    thresDivTH2D( hist2d[77], hist2d[78], 100 );
    thresDivTH2D( hist2d[79], hist2d[84], 100 );
    thresDivTH2D( hist2d[80], hist2d[85], 100 );
	profileTH2D( hist2d[87], hist1d[87], hist1d[101] );
    profileTH2D( hist2d[85], hist1d[88], hist1d[102] );
*/
}//>>>>void LLPgammaAnalyzer_MINI::endJob()


// ------------ method fills 'descriptions' with the allowed parameters for the module	------------
void LLPgammaAnalyzer_MINI::fillDescriptions(edm::ConfigurationDescriptions& descriptions) {
	//The following says we do not know what parameters are allowed so do no validation
	// Please change this to state exactly what you do use, even if it is no parameters
	edm::ParameterSetDescription desc;
	desc.setUnknown();
	descriptions.addDefault(desc);

	//Specify that only 'tracks' is allowed
	//To use, remove the default given above and uncomment below
	//ParameterSetDescription desc;
	//desc.addUntracked<edm::InputTag>("tracks","ctfWithMaterialTracks");
	//descriptions.addDefault(desc);
}//>>>>void LLPgammaAnalyzer_MINI::fillDescriptions(edm::ConfigurationDescriptions& descriptions)


//define this as a plug-in
DEFINE_FWK_MODULE(LLPgammaAnalyzer_MINI);
