
/** \file GEMCSCSegmentBuilder.cc
 *
 *
 */

#include <RecoLocalMuon/GEMRecHit/src/GEMCSCSegmentBuilder.h>

#include <Geometry/CSCGeometry/interface/CSCChamberSpecs.h>
//?
#include <Geometry/CSCGeometry/interface/CSCLayer.h>
//?
#include <Geometry/CSCGeometry/interface/CSCGeometry.h>
//?
#include <Geometry/GEMGeometry/interface/GEMGeometry.h>
#include <Geometry/GEMGeometry/interface/GEMEtaPartition.h>

#include <DataFormats/MuonDetId/interface/CSCDetId.h>
#include <DataFormats/MuonDetId/interface/GEMDetId.h>
#include <DataFormats/CSCRecHit/interface/CSCRecHit2D.h>
//??
#include <DataFormats/CSCRecHit/interface/CSCRangeMapAccessor.h>
//??
#include <DataFormats/CSCRecHit/interface/CSCRecHit2DCollection.h>
//??
#include <DataFormats/CSCRecHit/interface/CSCSegmentCollection.h>
//??
#include <DataFormats/CSCRecHit/interface/CSCSegment.h>
//??
#include <DataFormats/GEMRecHit/interface/GEMRecHit.h>
#include <DataFormats/GEMRecHit/interface/GEMRecHitCollection.h>


#include <RecoLocalMuon/GEMRecHit/src/GEMCSCSegmentAlgorithm.h>
#include <RecoLocalMuon/GEMRecHit/src/GEMCSCSegmentBuilderPluginFactory.h>

#include <FWCore/Utilities/interface/Exception.h>
#include <FWCore/MessageLogger/interface/MessageLogger.h> 

GEMCSCSegmentBuilder::GEMCSCSegmentBuilder(const edm::ParameterSet& ps) : geom_(0) {
	/*  //?????
	    // The algo chosen for the segment building
	    int chosenAlgo = ps.getParameter<int>("algo_type") - 1;
    
	    // Find appropriate ParameterSets for each algo type
	    std::vector<edm::ParameterSet> algoPSets = ps.getParameter<std::vector<edm::ParameterSet> >("algo_psets");
	    /// fine ??????
	*/
    // Now load the right parameter set
    // Algo name
	    //std::string algoName = algoPSets[chosenAlgo].getParameter<std::string>("algo_name");
    std::string algoName = ps.getParameter<std::string>("algo_name");
        
    LogDebug("GEMCSCSegment|GEMCSC")<< "GEMCSCSegmentBuilder algorithm name: " << algoName;

    // SegAlgo parameter set
 	   //std::vector<edm::ParameterSet segAlgoPSet = algoPSets[chosenAlgo].getParameter<std::vector<edm::ParameterSet> >("algo_psets");
    edm::ParameterSet segAlgoPSet = ps.getParameter<edm::ParameterSet>("algo_psets");
	/*////////????????????
	    // Chamber types to handle
	    std::vector<std::string> chType = algoPSets[chosenAlgo].getParameter<std::vector<std::string> >("chamber_types");
	    LogDebug("CSCSegment|CSC")<< "No. of chamber types to handle: " << chType.size();

	    // Algo to chamber type 
	    std::vector<int> algoToType = algoPSets[chosenAlgo].getParameter<std::vector<int> >("parameters_per_chamber_type");

	    // Trap if we don't have enough parameter sets or haven't assigned an algo to every type   
	    if (algoToType.size() !=  chType.size()) {
	        throw cms::Exception("ParameterSetError") << 
		  "#dim algosToType=" << algoToType.size() << ", #dim chType=" << chType.size() << std::endl;
	    }
	////////////fine ???????????
	*/    
    // Ask factory to build this algorithm, giving it appropriate ParameterSet
    algo = GEMCSCSegmentBuilderPluginFactory::get()->create(algoName, segAlgoPSet);
	 /*    
	    for (size_t j=0; j<chType.size(); ++j) {
	        algoMap[chType[j]] = CSCSegmentBuilderPluginFactory::get()->
                create(algoName, segAlgoPSet[algoToType[j]-1]);
		LogDebug("CSCSegment|CSC")<< "using algorithm #" << algoToType[j] << " for chamber type " << chType[j];
	    }*/
}

GEMCSCSegmentBuilder::~GEMCSCSegmentBuilder() {
    delete algo;
	 /* //
	  // loop on algomap and delete them
	  //
	  for (std::map<std::string, CSCSegmentAlgorithm*>::iterator it = algoMap.begin();it != algoMap.end(); it++){
	    delete ((*it).second);
	  }*/
}

void GEMCSCSegmentBuilder::build(const CSCRecHit2DCollection* recHits, CSCSegmentCollection& oc) {//da modificare
  	
  LogDebug("GEMCSCSegment|GEMCSC")<< "Total number of rechits in this event: " << recHits->size();

    std::vector<CSCDetId> chambers;
    std::vector<CSCDetId>::const_iterator chIt;
    
    for(CSCRecHit2DCollection::const_iterator it2 = recHits->begin(); it2 != recHits->end(); it2++) {
        
        bool insert = true;
        for(chIt=chambers.begin(); chIt != chambers.end(); ++chIt) 
            if (((*it2).cscDetId().chamber() == (*chIt).chamber()) &&
                ((*it2).cscDetId().station() == (*chIt).station()) &&
                ((*it2).cscDetId().ring() == (*chIt).ring()) &&
                ((*it2).cscDetId().endcap() == (*chIt).endcap()))
                insert = false;
	
        if (insert)
            chambers.push_back((*it2).cscDetId().chamberId());
    }

    for(chIt=chambers.begin(); chIt != chambers.end(); ++chIt) {

        std::vector<const CSCRecHit2D*> cscRecHits;
        const CSCChamber* chamber = geom_->chamber(*chIt);
        
        CSCRangeMapAccessor acc;
        CSCRecHit2DCollection::range range = recHits->get(acc.cscChamber(*chIt));
        
        std::vector<int> hitPerLayer(6);
        for(CSCRecHit2DCollection::const_iterator rechit = range.first; rechit != range.second; rechit++) {
            
            hitPerLayer[(*rechit).cscDetId().layer()-1]++;
            cscRecHits.push_back(&(*rechit));
        }    
        
        LogDebug("CSCSegment|CSC") << "found " << cscRecHits.size() << " rechits in chamber " << *chIt;
            
        // given the chamber select the appropriate algo... and run it
        std::vector<CSCSegment> segv = algoMap[chamber->specs()->chamberTypeName()]->run(chamber, cscRecHits);

        LogDebug("CSCSegment|CSC") << "found " << segv.size() << " segments in chamber " << *chIt;

        // Add the segments to master collection
        oc.put((*chIt), segv.begin(), segv.end());
    }
}

void GEMCSCSegmentBuilder::setGeometry(const CSCGeometry* geom) {
	geom_ = geom;
}

