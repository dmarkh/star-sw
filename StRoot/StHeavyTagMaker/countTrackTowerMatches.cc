//
// countTrackTowerMatches.cc
//
// Author: Manuel Calderon de la Barca Sanchez
// For use in the creation of the Heavy Flavor Tags
// Track cuts are:
// flag>0
// tpc fit points >=15
// |eta|<1.5
// momentum > 2 GeV
// Note: the momentum cut here is slightly larger than
// the one used for the invariant mass tag, because this is for
// a single track.  The invariant mass tag should be helpful
// for J/Psi in pp and dAu, so one needs to go lower than half of
// the J/Psi mass in momentum for that tag.


#include "StEvent.h"
#include "StPrimaryVertex.h"
#include "StContainers.h"
#include "StPrimaryTrack.h"
#include "StTrackGeometry.h"
#include "StTrackFitTraits.h"
#include "StEventSummary.h"
#include "StEmcCollection.h"
#include "StEmcDetector.h"
#include "StEmcModule.h"
#include "StEmcRawHit.h"


#include "StThreeVectorD.hh"

#include "StEmcUtil/geometry/StEmcGeom.h"
#include "StEmcUtil/projection/StEmcPosition.h"

int countTrackTowerMatches(StEvent* event) {

    // first, protect against funny business..
    if (!event) return -9999;
    if (!(event->primaryVertex())) return -9999;
    if (!(event->emcCollection())) return -9999;
    StDetectorId stBEMCDetId = static_cast<StDetectorId>(0 + kBarrelEmcTowerId);
    if (!(event->emcCollection()->detector(stBEMCDetId))) return -9999;
    
    // Set up the parameters we'll need
    // Use the bemc radius ("bemc", or det=1 in call to getEmcGeom
    // look in StRoot/StEmcUtil/geometry/StEmcGeom.cxx for implementation.
    // Initialize counters
    StEmcGeom* bemcGeom = StEmcGeom::getEmcGeom("bemc");
    const double Radius = bemcGeom->Radius(); 
    int trackTowerPairs = 0;

    // Get primary track container and BEMC detector pointer from StEvent
    const StSPtrVecPrimaryTrack& trackArray = event->primaryVertex()->daughters();    
    StEmcDetector* stBEMCDetector= event->emcCollection()->detector(stBEMCDetId);

    // Loop over primary tracks
    // find towers matching this track inside the loop
    // 
    for (unsigned int ipr1=0; ipr1<trackArray.size(); ++ipr1) {
	StPrimaryTrack* const ptrack1 = trackArray[ipr1];

	// check track 1 for cuts:
	if (!ptrack1) continue; // valid pointer
	if (ptrack1->flag()<=0) continue; // valid flag
	if (ptrack1->fitTraits().numberOfFitPoints(kTpcId)<15) continue; // enough fit points
	StThreeVectorF mom1 = ptrack1->geometry()->momentum();
	if (mom1.mag()<2.) continue; //use tracks with p>2 (NOT THE SAME AS FOR INV MASS TAG)
	if (fabs(mom1.pseudoRapidity())>1.5) continue; // use tracks with |eta|<1.5

	// at this point, track passed cuts,
	// try the track extrapolation

	StThreeVectorD trackMomentum, trackPosition;
	StEmcPosition emcPos;
	bool tok = emcPos.trackOnEmc(&trackPosition, &trackMomentum, ptrack1,
				     event->summary()->magneticField(), Radius);
	// if it doesn't extrapolate to the BEMC, go on to the next track.
	if (!tok) continue;
	
	// At this point, it extrapolates, so now look to see if the
	// adc value is 416 which corresponds to roughly 3 GeV.
	// Note 1: if the calibration above doesn't hold, then this tag is suspect
	// Note 2: one must also take into account pedestal subtraction (roughly 30 ADC)

	int moduleH, etaH, subH, id;
	bemcGeom->getBin(trackPosition.phi(), trackPosition.pseudoRapidity(), moduleH, etaH, subH);
	bemcGeom->getId(moduleH, etaH, subH, id);
	// can I find the tower by Id? That would be faster
	// Looks like I have to loop over all the towers in the module...
	StEmcModule* module = stBEMCDetector->module(moduleH);
	const StSPtrVecEmcRawHit& modHits = module->hits();
	for (size_t i=0; i<modHits.size();++i) {
	    StEmcRawHit* hit = modHits[i];
	    if (hit->eta()==static_cast<unsigned int>(etaH) && hit->sub()==subH) {
		// This is the hit that the track extrapolates to.
		// Check it's adc value, if it's greater
		// than 416 AFTER pedestal subtraction,
		// we have a match
		int adc = static_cast<int>(hit->adc());
		if (adc-30>416) ++trackTowerPairs;
	    } // found the hit that track extrapolates to
	}// hits in module loop
	
	
    }// track loop
    return trackTowerPairs;
}
