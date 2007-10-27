//

#include <iostream>
#include <fstream>
#include <sstream>
#include <cstring>
#include <string>
#include <vector>
#include <memory>

#include <boost/shared_ptr.hpp>

#include <TClass.h>

#include "FWCore/Utilities/interface/Exception.h"
#include "FWCore/Framework/interface/ESProducer.h"
#include "FWCore/Framework/interface/EventSetupRecordIntervalFinder.h"
#include "FWCore/Framework/interface/MakerMacros.h"
#include "FWCore/Framework/interface/SourceFactory.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "FWCore/ParameterSet/interface/FileInPath.h"
#include "FWCore/MessageLogger/interface/MessageLogger.h"

#include "CondFormats/DataRecord/interface/BeamSpotObjectsRcd.h"
#include "CondFormats/BeamSpotObjects/interface/BeamSpotObjects.h"

class BeamSpotFakeConditions : public edm::ESProducer, public edm::EventSetupRecordIntervalFinder {
public:
	typedef boost::shared_ptr<BeamSpotObjects> ReturnType;
	BeamSpotFakeConditions(const edm::ParameterSet &params);
	virtual ~BeamSpotFakeConditions();
	ReturnType produce(const BeamSpotObjectsRcd &record);
private:
	void setIntervalFor(const edm::eventsetup::EventSetupRecordKey &key,const edm::IOVSyncValue &syncValue,edm::ValidityInterval &oValidity);
	edm::FileInPath xmlCalibration;
	bool usedummy;
	
};

BeamSpotFakeConditions::BeamSpotFakeConditions(const edm::ParameterSet &params) :
	
	//xmlCalibration(params.getParameter<edm::FileInPath>("xmlCalibration") ), //disable until xml writer is fixed
	usedummy(params.getParameter<bool>("UseDummy") ) {
	
	setWhatProduced(this);
	findingRecord<BeamSpotObjectsRcd>();
}

BeamSpotFakeConditions::~BeamSpotFakeConditions(){}

BeamSpotFakeConditions::ReturnType
BeamSpotFakeConditions::produce(const BeamSpotObjectsRcd &record){


	if ( ! usedummy ) {
	  //TBufferXML code removed from here...		
	}
	else {

		BeamSpotObjects *adummy = new BeamSpotObjects();
		adummy->SetPosition(0.,0.,0.);
		adummy->SetSigmaZ(7.55);
		adummy->Setdxdz(0.);
		adummy->Setdydz(0.);
		adummy->SetBeamWidth(15.e-4);
		return ReturnType(adummy);
	}
  
	
}

void BeamSpotFakeConditions::setIntervalFor(const edm::eventsetup::EventSetupRecordKey &key,
						  const edm::IOVSyncValue &syncValue,
						  edm::ValidityInterval &oValidity){
  oValidity = edm::ValidityInterval(edm::IOVSyncValue::beginOfTime(),
				    edm::IOVSyncValue::endOfTime());
}

DEFINE_ANOTHER_FWK_EVENTSETUP_SOURCE(BeamSpotFakeConditions);
