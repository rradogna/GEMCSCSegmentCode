#ifndef GEMRecHit_GEMCSCSegmentAlgorithm_h
#define GEMRecHit_GEMCSCSegmentAlgorithm_h

/** \class GEMCSCSegmentAlgo
 * An abstract base class for algorithmic classes used to
 * build segments in one pair of GEM-CSC detector.
 *
 * Implementation notes: <BR>
 * For example, GEMCSCSegmAlgoRR inherits from this class,

 *
 * $Date:  $
 * $Revision: 1.7 $
 * \author Raffaella Radogna
 *
 */

#include <DataFormats/CSCRecHit/interface/CSCRecHit2DCollection.h>
#include <DataFormats/CSCRecHit/interface/CSCSegment.h>
#include <Geometry/CSCGeometry/interface/CSCChamber.h>

#include <FWCore/Framework/interface/Frameworkfwd.h>
#include <map>
#include <vector>

class GEMCSCSegmentAlgorithm {
public:
    /// Constructor
    explicit GEMCSCSegmentAlgorithm(const edm::ParameterSet&) {};
    /// Destructor
    virtual ~GEMCSCSegmentAlgorithm() {};

    /** Run the algorithm = build the segments in this chamber
    */
    virtual std::vector<CSCSegment> run(const CSCChamber* chamber, const std::vector<const CSCRecHit2D*>& rechits) = 0;  //////DA MODIFICARE

    private:
};

#endif
