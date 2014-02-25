/** \file GEMCSCSegmentProducer.cc
 *
 */

#include <RecoLocalMuon/GEMRecHit/src/GEMCSCSegmentProducer.h>
#include <RecoLocalMuon/GEMRecHit/src/GEMCSCSegmentBuilder.h>

#include <DataFormats/Common/interface/Handle.h>
#include <FWCore/Framework/interface/ESHandle.h>
#include <FWCore/MessageLogger/interface/MessageLogger.h> 

#include <Geometry/Records/interface/MuonGeometryRecord.h>

#include <DataFormats/GEMRecHit/interface/GEMRecHitCollection.h>
#include <DataFormats/GEMRecHit/interface/GEMCSCSegmentCollection.h>
#include <DataFormats/GEMRecHit/interface/GEMCSCSegment.h>

#include <DataFormats/CSCRecHit/interface/CSCRecHit2DCollection.h>
#include <DataFormats/CSCRecHit/interface/CSCSegmentCollection.h>
#include <DataFormats/CSCRecHit/interface/CSCSegment.h>

GEMCSCSegmentProducer::GEMCSCSegmentProducer(const edm::ParameterSet& pas) : iev(0) {
	
    inputObjectsTag = pas.getParameter<edm::InputTag>("inputObjects");
    segmentBuilder_ = new GEMCSCSegmentBuilder(pas); // pass on the PS

  	// register what this produces
    produces<GEMCSCSegmentCollection>();
}

GEMCSCSegmentProducer::~GEMCSCSegmentProducer() {

    LogDebug("GEMCSCSegment|GEMCSC") << "deleting GEMCSCSegmentBuilder after " << iev << " events w/gem and csc data.";
    delete segmentBuilder_;
}

void GEMCSCSegmentProducer::produce(edm::Event& ev, const edm::EventSetup& setup) {

    LogDebug("GEMCSCSegment|GEMCSC") << "start producing segments for " << ++iev << "th event with gem and csc data";
	
    // find the geometry (& conditions?) for this event & cache it in the builder
  
  
  ///////?????????????????? due geometrie
    //edm::ESHandle<CSCGeometry> h;
    //setup.get<MuonGeometryRecord>().get(h);
    //const CSCGeometry* pgeom = &*h;
    //segmentBuilder_->setGeometry(pgeom);
	
    // get the collection of CSCSegment and GEMRecHits
    edm::Handle<GEMRecHitCollection> gemRecHits;
    ev.getByLabel(inputObjectsTag, gemRecHits);  
    edm::Handle<CSCSegmentCollection> cscSegment;
    ev.getByLabel(inputObjectsTag, cscSegment); 
    edm::Handle<CSCRecHit2DCollection> cscRecHits;
    ev.getByLabel(inputObjectsTag, cscRecHits);


    // create empty collection of Segments
    std::auto_ptr<GEMCSCSegmentCollection> oc( new GEMCSCSegmentCollection );
//to be modified
  	// fill the collection
    segmentBuilder_->build(gemRecHits.product(), *oc); //@@ FILL oc

    // put collection in event
    ev.put(oc);
}
