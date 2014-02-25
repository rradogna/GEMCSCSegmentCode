#ifndef GEMRecHit_GEMCSCSegment_h
#define GEMRecHit_GEMCSCSegment_h

/** \class GEMCSCSegment
 *  Describes a reconstructed track segment in the GEM + CSC chambers. 
 *  This is 4-dimensional since it has an origin (x,y) and a direction (x,y)
 *  in the local coordinate system of the chamber.
 *
 *  $Date:  $
 *  \author R. Radogna
 */

#include <DataFormats/TrackingRecHit/interface/RecSegment.h>

#include <DataFormats/GEMRecHit/interface/GEMRecHitCollection.h>

#include <DataFormats/CSCRecHit/interface/CSCRecHit2DCollection.h>
#include <DataFormats/CSCRecHit/interface/CSCSegmentCollection.h>
#include <DataFormats/CSCRecHit/interface/CSCSegment.h>

#include <iosfwd>

class CSCDetId; //????????????

class GEMCSCSegment GCC11_FINAL : public RecSegment {

public:

    /// Default constructor
    GEMCSCSegment() : theChi2(0.) {}
	
    /// Constructor
    GEMCSCSegment(const std::vector<const CSCRecHit2D*>& proto_segment, LocalPoint origin, 
        	LocalVector direction, AlgebraicSymMatrix errors, double chi2);
  
    /// Destructor
    virtual ~GEMCSCSegment();

    //--- Base class interface
    GEMCSCSegment* clone() const { return new GEMCSCSegment(*this); }

    LocalPoint localPosition() const { return theOrigin; }
    LocalError localPositionError() const ;
	
    LocalVector localDirection() const { return theLocalDirection; }
    LocalError localDirectionError() const ;

    /// Parameters of the segment, for the track fit in the order (dx/dz, dy/dz, x, y )
    AlgebraicVector parameters() const;

    /// Covariance matrix of parameters()
    AlgebraicSymMatrix parametersError() const { return theCovMatrix; }

    /// The projection matrix relates the trajectory state parameters to the segment parameters().
    virtual AlgebraicMatrix projectionMatrix() const;

    virtual std::vector<const TrackingRecHit*> recHits() const;

    virtual std::vector<TrackingRecHit*> recHits();

    double chi2() const { return theChi2; };

    virtual int dimension() const { return 4; }

    virtual int degreesOfFreedom() const { return 2*nRecHits() - 4;}	 

    //--- Extension of the interface
        
    const std::vector<CSCRecHit2D>& specificRecHits() const { return theCSCRecHits; } // da modificare

    int nRecHits() const { return theCSCRecHits.size(); }        

    CSCDetId cscDetId() const { return  geographicalId(); }

    /*void setDuplicateSegments(std::vector<CSCSegment*>& duplicates);

    bool isME11a_duplicate() const { return (theDuplicateSegments.size() > 0 ? true : false); }
    // a copy of the duplicated segments (ME1/1a only) 
    const std::vector< CSCSegment> & duplicateSegments() const { return theDuplicateSegments; } 
    
    bool testSharesAllInSpecificRecHits( const std::vector<CSCRecHit2D>& specificRecHits_1,
					 const std::vector<CSCRecHit2D>& specificRecHits_2,
					 CSCRecHit2D::SharedInputType) const;
    
    //bool sharesRecHits(CSCSegment  & anotherSegment, CSCRecHit2D::SharedInputType);
    // checks if ALL the rechits share the specific input (allWires, allStrips or all)
    bool sharesRecHits(const CSCSegment  & anotherSegment, CSCRecHit2D::SharedInputType sharesInput) const;
    // checks if ALL the rechits share SOME wire AND SOME strip input
    bool sharesRecHits(const CSCSegment  & anotherSegment) const;*/

    float time() const;
    
    void print() const;		
    
 private:
    
    std::vector<CSCRecHit2D> theCSCRecHits;
    LocalPoint theOrigin;   // in chamber frame - the GeomDet local coordinate system
    LocalVector theLocalDirection; // in chamber frame - the GeomDet local coordinate system
    AlgebraicSymMatrix theCovMatrix; // the covariance matrix
    double theChi2;

};

std::ostream& operator<<(std::ostream& os, const GEMCSCSegment& seg);

#endif // CSCRecHit_CSCSegment_h
