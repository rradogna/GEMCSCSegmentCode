#include "RecoMuon/MuonSeedGenerator/src/MuonSeedVPatternRecognition.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"

MuonSeedVPatternRecognition::MuonSeedVPatternRecognition(const edm::ParameterSet & pset)
: enableDTMeasurement(pset.getParameter<bool>("EnableDTMeasurement")),
  enableCSCMeasurement(pset.getParameter<bool>("EnableCSCMeasurement")),
  enableGEMMeasurement(pset.getParameter<bool>("EnableGEMMeasurement"))

{
  if(enableDTMeasurement)
    // the name of the DT rec hits collection
    theDTRecSegmentLabel = pset.getParameter<edm::InputTag>("DTRecSegmentLabel");

  if(enableCSCMeasurement)
    // the name of the CSC rec hits collection
    theCSCRecSegmentLabel = pset.getParameter<edm::InputTag>("CSCRecSegmentLabel");

  if(enableGEMMeasurement)
    // the name of the CSC rec hits collection
    theGEMRecSegmentLabel = pset.getParameter<edm::InputTag>("GEMRecSegmentLabel");

}

