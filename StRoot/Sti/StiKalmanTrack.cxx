//StiKalmanTrack.cxx
/*
 * $Id: StiKalmanTrack.cxx,v 2.82 2006/03/10 23:41:43 jeromel Exp $
 * $Id: StiKalmanTrack.cxx,v 2.82 2006/03/10 23:41:43 jeromel Exp $
 *
 * /author Claude Pruneau
 *
 * $Log: StiKalmanTrack.cxx,v $
 * Revision 2.82  2006/03/10 23:41:43  jeromel
 * Comment out StiDebug
 *
 * Revision 2.81  2006/03/09 22:45:12  didenko
 * get back previuos version
 *
 * Revision 2.73  2005/10/26 21:55:02  fisyak
 * get rid off dependencies from StMcEvent
 *
 * Revision 2.72  2005/08/18 02:35:23  perev
 * Cleanup
 *
 * Revision 2.71  2005/08/17 22:00:17  perev
 * getAllPointCount(...) added
 *
 * Revision 2.70  2005/08/16 20:11:10  perev
 * Typo corrected
 *
 * Revision 2.69  2005/08/14 01:10:55  perev
 * Non empty unset(). Free all nodes when track is freed
 *
 * Revision 2.68  2005/08/09 14:51:25  perev
 * Add reduce method, reducing all the nodes
 *
 * Revision 2.67  2005/08/04 03:50:31  perev
 * removeLastNode() added
 *
 * Revision 2.66  2005/07/20 17:21:44  perev
 * MultiVertex
 *
 * Revision 2.65  2005/06/09 03:12:39  perev
 * Fix typo in getNodes()
 *
 * Revision 2.64  2005/05/31 16:33:32  perev
 * Method refitL added
 *
 * Revision 2.63  2005/05/13 19:33:11  perev
 * Defence against all nodes are bad added
 *
 * Revision 2.62  2005/05/12 17:56:17  perev
 * refit tuned
 *
 * Revision 2.61  2005/04/11 17:27:59  perev
 * Error status added to fit()
 *
 * Revision 2.60  2005/03/31 18:14:00  perev
 * getMaxPointCount() fixed(thank to Jan)
 *
 * Revision 2.59  2005/03/31 17:25:57  perev
 * getMaxPointCount() fixed(thank to Jan)
 *
 * Revision 2.58  2005/03/28 05:48:49  perev
 * Reorganization of node container
 *
 * Revision 2.57  2005/03/24 17:59:38  perev
 * refit() method added
 *
 * Revision 2.56  2005/03/17 06:19:36  perev
 * Cleanup
 *
 * Revision 2.55  2005/02/25 16:36:14  perev
 * Iteration in refit added
 *
 * Revision 2.54  2005/02/18 19:02:31  fisyak
 * Add debug print out for extendToVertex
 *
 * Revision 2.53  2005/02/17 23:19:02  perev
 * NormalRefangle + Error reseting
 *
 * Revision 2.52  2005/02/17 19:58:06  fisyak
 * Add debug print out flags
 *
 * Revision 2.51  2005/02/07 18:33:42  fisyak
 * Add VMC dead material
 *
 * Revision 2.50  2005/01/17 01:31:25  perev
 * New parameter model
 *
 * Revision 2.49  2004/12/23 15:06:28  pruneau
 * use _alpha instead of getRefAngle while extending to vertex
 *
 * Revision 2.48  2004/12/11 04:31:36  perev
 * set of bus fixed
 *
 * Revision 2.47  2004/12/01 18:04:32  perev
 * test for -ve and too big track length added
 *
 * Revision 2.46  2004/12/01 03:57:08  pruneau
 * d<4
 *
 * Revision 2.45  2004/11/12 22:48:28  fisyak
 * Back to use chi2 instead DCA for Vertex fit
 *
 * Revision 2.44  2004/11/11 03:19:05  pruneau
 * implementation of extrapolation functions for Jan
 *
 * Revision 2.43  2004/11/10 21:44:26  pruneau
 * adding functions for extrapolation
 *
 * Revision 2.42  2004/11/08 15:32:50  pruneau
 * 3 sets of modifications
 * (1) Changed the StiPlacement class to hold keys to both the radial and angle placement. Propagated the use
 * of those keys in StiSvt StiTpc StiSsd and all relevant Sti classes.
 * (2) Changed the StiKalmanTrackFinder::find(StiTrack*) function's algorithm for the navigation of the
 * detector volumes. The new code uses an iterator to visit all relevant volumes. The code is now more robust and compact
 * as well as much easier to read and maintain.
 * (3) Changed the chi2 calculation in StiKalmanTrack::getChi2 and propagated the effects of this change
 * in both StiTrackingPlots and StiStEventFiller classes.
 *
 * Revision 2.41  2004/10/28 19:30:42  perev
 * Hack. Infinite Chi2 skipped in Chi2 calculation. Claude??? (VP)
 *
 * Revision 2.40  2004/10/28 04:59:18  perev
 * Fixed iterator for nodes. v3V2
 *
 * Revision 2.39  2004/10/27 03:25:49  perev
 * Version V3V
 *
 * Revision 2.38  2004/10/26 21:52:07  pruneau
 * No truncation but bad hits dropped
 *
 * Revision 2.37  2004/10/26 06:45:37  perev
 * version V2V
 *
 * Revision 2.36  2004/10/25 14:15:49  pruneau
 * various changes to improve track quality.
 *
 * Revision 2.35  2004/08/17 20:55:42  perev
 * memory cleanup heap==>stack
 *
 * Revision 2.34  2004/08/06 02:28:53  andrewar
 * Added getMaxPointCount(int detectorId)< where detectorId corresponds to the
 * StDetectorId value.
 *
 * Revision 2.33  2004/04/04 23:19:28  jeromel
 * isfinite() -> finite()
 *
 * Revision 2.32  2004/03/31 00:23:41  calderon
 * -Fixed memory leak in StiDetectorTreeBuilder::hangWhere (100 chars were lost
 *  every time this function was called)
 * -Changed algorithm to count fit points in StiKalmanTrack.  Now it is based
 *  on counting the nodes that have a chi2 < chi2Max from
 *  StiKalmanTrackFitterParameters.
 * -Which meant that I had to somehow introduce a pointer to it so that the
 *  track could know about the chi2Max used in the fitter.
 * -And I also added a method to retrieve the pointer to the fitterParams
 *  to be used in StiStEventFiller.
 * -Which was then modified to calculate the encoded fit points based on
 *  a similar algorithm (chi2<chi2Max test).
 * -Cleaned up the includes in StiKalmanTrack.h, left only the ones
 *  needed to keep the code compiling.
 * -Which required a slight modification in the include of StiKalmanTrackFinder
 * -StiTrackKalmanTrackFitter now also sets a pointer to itself in
 *  static StiKalmanTrack::setFitParameters()
 * -Removed some print outs from VectorizedFactory to reduce the size of the log
 *  files.
 *
 * Revision 2.31  2004/03/23 23:10:37  calderon
 * Check for nan's in getTrackLength() calculation.  When the argument for the
 * asin() is >1, the code instead calculates a length iteratively.
 * For these cases, the returned value is negative so that they can be inspected
 * in the gui, or filtered in the StiStEventFiller.
 *
 * Revision 2.30  2004/02/21 18:27:34  pruneau
 * Updates to comply with changes made in abstract interfaces.
 *
 * Revision 2.29  2003/09/02 17:59:41  perev
 * gcc 3.2 updates + WarnOff
 *
 * Revision 2.28  2003/08/02 08:22:43  pruneau
 * best performance so far
 *
 * Revision 2.27  2003/07/30 19:18:25  pruneau
 * sigh
 *
 * Revision 2.25  2003/05/14 21:37:59  pruneau
 * Fixed "chi2" problem. 5 first nodes on a track did not have
 * relevant errors. Fix the problem by inserting a call to calculateError()
 * inside the add(stiHit*...) method used while initializing tracks from the
 * seed finder. CP
 *
 * Revision 2.24  2003/05/06 15:33:49  mmiller
 * iCommitting changes to turn on multiple regions (StiPlacement::StiRegion -> kMidRapidity, kForwardRapidity, etc).
 * Also added a point to StiToolkit for StiMaker.  This allows for the req. GetDataSet calls in the FTPC code.
 * Not so elegant...
 *
 * Revision 2.23  2003/04/29 18:48:21  pruneau
 * *** empty log message ***
 *
 * Revision 2.22  2003/04/22 21:20:05  pruneau
 * Added hit filter
 * Tuning og finder pars
 * Tuning of KalmanTrackNode
 *
 * Revision 2.21  2003/04/10 12:02:13  pruneau
 * various changes
 *
 * Revision 2.20  2003/03/31 17:18:47  pruneau
 * various
 *
 * Revision 2.19  2003/03/17 17:45:31  pruneau
 * *** empty log message ***
 *
 * Revision 2.18  2003/03/14 20:50:29  pruneau
 * Added groupId member and accessor functions to StiDetector, StiDetectorGroup, StiDetectorBuilder,
 * and modified getNodes of the StiKalmanTrack class to use it. This eliminates explicit
 * references to Tpc and Svt within StiKalmanTrack...
 *
 * Revision 2.17  2003/03/14 19:02:20  pruneau
 * various minor updates
 *
 * Revision 2.16  2003/03/13 21:21:26  pruneau
 * getPhase() fixed. MUST inclde -helicity()*pi/2
 *
 * Revision 2.15  2003/03/13 18:59:08  pruneau
 * various updates
 *
 * Revision 2.14  2003/03/13 16:38:11  andrewar
 * Made use of X0() calls in getTrackRadLength()
 *
 * Revision 2.13  2003/03/13 15:16:41  pruneau
 * fixed getPhi, getPseudoRapdity, getPhase methods
 *
 * Revision 2.12  2003/03/12 17:57:29  pruneau
 * Elss calc updated.
 *
 * Revision 2.11  2003/03/04 15:16:22  andrewar
 * Added getTrackRadLength function to return radiation thickness along track (%).
 *
 */



//Std
#include <stdexcept>
#include <cmath>

//SCL
#include "StThreeVector.hh"
#include "StThreeVectorF.hh"
#include "StThreeVectorD.hh"
#include "StPhysicalHelixD.hh"

#include "StHit.h"

//Sti
#include "StiToolkit.h"
#include "StiTrackFinder.h"
#include "StiHit.h"
#include "StiKalmanTrackNode.h"
#include "StiKalmanTrack.h"
#include "StiDetector.h"
#include "StiPlacement.h"
#include "StiMaterial.h"
#include "StiHitErrorCalculator.h"
#include "StPhysicalHelixD.hh"
#include "StHelix.hh"
#include "StiKalmanTrackFitterParameters.h"
#include "StiKalmanTrackFinderParameters.h"
#include "StiHitContainer.h"
#include "StiTrackNodeHelper.h"
//#include "StiDebug.h"

ostream& operator<<(ostream&, const StiHit&);

Factory<StiKalmanTrackNode>* StiKalmanTrack::trackNodeFactory = 0;
StiKalmanTrackFinderParameters* StiKalmanTrack::pars = 0;
StiKalmanTrackFitterParameters* StiKalmanTrack::fitpars = 0;
int StiKalmanTrack::_debug = 0;
int debugCount=0;

/*! 
   Reset the class members to their default state.
   This method is called by the ctor of the class to initialize the
   members of the class to an "empty" or null track state. The
   method must also be called everytime an instance of this class is
   retrieved from its factory in order to set the first and last
   nodes to "null" thus guaranteeing that the track object is empty
   i.e. does not represent any track and is thus ready for a new
   search and reconstruction.  
 */
void StiKalmanTrack::reset()
{
  firstNode = 0;
  lastNode  = 0;
  mSeedHitCount = 0;
  m      = -1.;
  mFlag  = 0;

  _vDca=-1;
  _vChi2=-2;
}

 

/*! 
  Set the factory used for the creation of kalman track nodes.
  \see StiKalmanTrackNodeFactory
*/
void StiKalmanTrack::setKalmanTrackNodeFactory(Factory<StiKalmanTrackNode>* val)
{
  trackNodeFactory = val;
}


/*!
  Add a hit to this track.
  <p>
  If the current lastNode is non null, 
  <ol>
  <li>Insert the given hit in a StiKalmanTrackNode instance.</li>
  <li>Add the new node as a child to the current last node.</li>
  <li>Make the new node the last node of this track.</li>
  </ol>
  else
  <ol>
  <li>Insert the given hit in a StiKalmanTrackNode instance.</li>
  </ol>
*/
StiKalmanTrackNode * StiKalmanTrack::add(StiHit *h,double alpha, double eta, double curvature, double tanl,int direction)
{
  TRACKMESSENGER << "StiKalmanTrack::add(...) -I- Started"<<endl;
  StiKalmanTrackNode * n = trackNodeFactory->getInstance();
  n->initialize(h,alpha,eta,curvature,tanl);
  
  add(n,direction);
  TRACKMESSENGER << "StiKalmanTrack::add(...) -I- Done"<<endl;
  return n;
}



/*! Initialization of this kalman track from external parameters.
  <p>
  This track object is initialized on the basis of parameters determined externally. The
  parameters consist of the track curvature, the tangent of pitch angle, the origin of 
  the helix, and a vector of hits already associated with the track.
  
  <h3>Arguments:</h3>
  <TABLE BORDER="0" CELLPADDING="2" CELLSPACING="0" WIDTH="100%">
  <TR> <TD WIDTH="10%">curvature</TD> <TD WIDTH="90%">1/radius of the tack.</TD>  </TR>
  <TR> <TD WIDTH="10%">tanl</TD>      <TD WIDTH="90%">tan(pitch angle)</TD> </TR>
  <TR> <TD WIDTH="10%">origin</TD>    <TD WIDTH="90%">origin of the track in global coordinates.</TD> </TR>
  <TR> <TD WIDTH="10%">v</TD>         <TD WIDTH="90%">vector of hits associated with this track.</TD> </TR>
</TABLE>
<h3>Algorithm:</h3>
<ol>
<li>Verify that a valid node factory exists.</li>
<LI>Use local arrays state and error to add and set all nodes of this track.</LI>
<LI>Use the same curvature, and tanl for all nodes as supplied in argument list.</li>
<li>Use Unit matrix for error matrix.</li>
<li>Loop over all hits of the input hit vector and create a track node for each.</LI>
<li>Paramters of the track node are set according to the y,z of the hits added.</LI>
<li>Hits given are transformed in the local coordinates of their detector.
</ol>
<h3>Notes:</h3>
<OL>
<LI>Throws a logic_error exception if no track node factory is available.</li>
<LI>Throws a logic_error exception if the factory
  is not a castable to a factory of StiKalmanTrackNode.</li>
<li>Throws a logic error exception if hits do not have a valid pointer to a detector object.</li>
</OL>
*/
void StiKalmanTrack::initialize(double curvature,
				double tanl,
				const StThreeVectorD& origin,
				const vector<StiHit*> & hits)
{
  //cout << "StiKalmanTrack::initialize() -I- Started"<<endl;
  reset();
  vector<StiHit*>::const_iterator it;
  //StiKalmanTrackNode * node  = 0;
  double eta   =-99999.;
  double alphaP=-99999.;
  double alpha;
  StThreeVectorD temp;
  const StiDetector* detector;
  for (it=hits.begin(); it!=hits.end(); ++it)
    {
      detector = (*it)->detector();
      if (!detector) 
	{
	  cout <<"StiKalmanTrack::initialize() -F- detector==0"<<endl;
	  throw logic_error("StiKalmanTrack::initialize() - FATAL - Hit has null detector.");
	}
      // if alpha is same, avoid recalculating eta
      alpha = detector->getPlacement()->getNormalRefAngle();
      if (alphaP!=alpha)
	{
	  temp = origin;
	  temp.rotateZ(-alpha);
	  eta = curvature*temp.x();
	  alphaP=alpha;
	}
      add((*it),alpha,eta,curvature,tanl,kOutsideIn);
    }
   //cout << "StiKalmanTrack::initialize() -I- Done"<<endl;
}


StThreeVector<double> StiKalmanTrack::getMomentumAtOrigin() const
{
  double px,py,pz;
  px=py=pz=0;

  StiKalmanTrackNode * inner = getInnerMostNode();

  if (inner==0)throw logic_error("StiKalmanTrack::getMomentumAtOrigin() - ERROR - No node");
  inner->propagate(0.,0,kOutsideIn);
  double p[3];
  double e[6];
  inner->getMomentum(p,e);
  StThreeVector<double> p3(p[0],p[1],p[2]);
  p3.rotateZ(inner->getAlpha());
  return p3;
}

/*! Return the track sign
   <h3>Notes</h3> 
   <ol>
   <li>Use the last node and the field.</li>
   </ol>
*/
int StiKalmanTrack::getCharge() const
{
  StiKalmanTrackNode *node = getInnerMostNode();
  if (!node) return 0;
  return  node->getCharge();
}

/// Return the track chi2 per dof
/// <p>
/// The track chi2 is calculated from the incremental chi2 of all nodes carrying a hit that contributed to the fit of the track. 
/// Note that a hit is not counted as contributing to the fit if its chi2 exceeds "fitpars->getMaxChi2()"
/// Note that this function returns "-1" if the number of fit points is smaller than 6
double  StiKalmanTrack::getChi2() const
{
  double fitHits   = 0;
  double trackChi2 = 0;
  double nodeChi2  = 0;
  double maxChi2   = fitpars->getMaxChi2();
  double theChi2 = 1.e+60;
  if (!firstNode) return theChi2;
  theChi2 = 0;
  StiKTNBidirectionalIterator it;
  for (it=begin();it!=end();it++)  {
    StiKalmanTrackNode *node = &(*it);
    if (!node->isValid()) continue;
    if (!node->getHit() ) continue;
    nodeChi2 = node->getChi2();
    if (nodeChi2>maxChi2) continue;
    trackChi2 += nodeChi2;
    ++fitHits;
  }
  return (fitHits>5)?trackChi2/(2.*fitHits-5.):1e30;
}


/*! 
	Calculate and return the number of hits on this track. 
   <h3>Notes</h3> 
   <ol>
   <li>Iterate through all nodes of this track.</li>
   <li>Count number of hits.</li>
   </ol>
	 \return number of hits.
*/
int StiKalmanTrack::getPointCount(int detectorId) const
{
  const StiDetector *detector=0;   
  int nPts = 0;
  StiKTNBidirectionalIterator it;
  for (it=begin();it!=end();it++) {
    StiKalmanTrackNode *node = &(*it);
    if (!node->isValid()) 	continue;
    if (!node->getHit())	continue;
    detector = node->getDetector();  
    if (!detector) 		continue;
    if (detectorId && detector->getGroupId() != detectorId) 	continue;
    nPts++;
  }
  return nPts;
}

//_____________________________________________________________________________
/*! Calculate and return the maximum possible number of hits on this track. 
  <h3>Notes</h3> 
   <ol>
   <li>Iterate through all nodes of this track.</li>
   <li>Count active layers.</li>
   <li>Use the (y,z) position of the node to determine whether point is on
       active region of the detector i.e. RDO were functional.</li>
   </ol>
	 \return maximum number of points
*/
int StiKalmanTrack::getMaxPointCount(int detectorId) const
{
  int nPts = 0;
  StiKTNBidirectionalIterator it;

  for (it=begin();it!=end();it++){
    const StiKalmanTrackNode *node = &(*it);
    if (!node->isValid()) 					continue;
    const StiDetector *detector = node->getDetector();
    if (!detector)						continue;
    StiHit* h = node->getHit();
    if (!h && !detector->isActive(node->getY(),node->getZ()))	continue;
    if (detectorId && detector->getGroupId() != detectorId) 	continue;
    nPts++;
  }
  return nPts;
}


//_____________________________________________________________________________
/*! Return the number of gaps (active layers with no hits) along this track.
  <h3>Notes</h3> 
  <ol>
  <li>A gap consists of one or multiple contiguous active layers through which this track
  passes.</li>
  <li>There can be gaps on the inside or the outside of the track if no hits are found there.</li>
  </ol>
  \returns number of gaps.
*/
int    StiKalmanTrack::getGapCount()    const  
{
  int gaps = 0;
  if (firstNode)
    {
      StiKTNBidirectionalIterator it;
      bool inGap = false;
      for (it=begin();it!=end();it++)
	{
	  const StiDetector * detector = (*it).getDetector();
	  if (detector && detector->isActive())
	    {
	      if ((*it).getHit())
		{
		  if (inGap) 
		    inGap = false;
		}
	      else
		{
		  if (!inGap)
		    {
		      inGap = true;
		      gaps++;
		    }										
		}
	    }
	}
    }
  return gaps;
}

//_____________________________________________________________________________
/*! Return the number of hits (points) used in the fit of this track.
  <h3>Notes</h3> 
  <ol>
  <li>Currently no difference is made between points on the track and fit points 
  on the track.</li>
  <li>Call "getPointCount()" to get the count.</li>
  </ol>
  \return number of hits on this track.
*/
///Get number of fit points in given detector
int StiKalmanTrack::getFitPointCount(int detectorId)    const  
{
  int fitPointCount  = 0;
  //yf  double chi2Max = fitpars->getMaxChi2();
  StiKTNBidirectionalIterator it;
  for (it=begin();it!=end();it++)  {
    StiKalmanTrackNode* node = &(*it); 
    if(!node->isValid())		continue;
    StiHit* hit = node->getHit();
    if (!hit)				continue;
    if (!node->isFitted())		continue;
    const StiDetector *det = hit->detector();
    if (!det)				continue;  
    if (detectorId && detectorId!=det->getGroupId())continue;
    fitPointCount++;
  }
  return fitPointCount;
}
//_____________________________________________________________________________
void StiKalmanTrack::getAllPointCount(int count[1][3],int maxDetId) const
{
//  output array actually is count[maxDetId+1][3] 
//  count[0] all detectors
//  count[detId] for particular detector
//  count[detId][0] == number of possible points
//  count[detId][1] == number of measured points
//  count[detId][2] == number of fitted   points
enum {kPP=0,kMP=1,kFP=2};

  memset(count[0],0,(maxDetId+1)*3*sizeof(int));
  StiKTNBidirectionalIterator it;

  for (it=begin();it!=end();it++){
    const StiKalmanTrackNode *node = &(*it);
    if (!node->isValid()) 	continue;
    const StiDetector *detector = node->getDetector();
    if (!detector)		continue;
    int detId = detector->getGroupId();
    StiHit* h = node->getHit();

//fill possible points
    if (h || detector->isActive(node->getY(),node->getZ())) {
       count[0][kPP]++; count[detId][kPP]++;
    }
    
    if (!h ) 			continue;
//fill measured points
    count[0][kMP]++; count[detId][kMP]++;
    if (!node->isFitted()) 	continue;
    count[0][kFP]++; count[detId][kFP]++;
  }
}

//_____________________________________________________________________________
/*! Calculate and return the track length.
  <h3>Notes</h3> 
   <ol>
   <li>Using helix track model in global reference frame.</li>
   <li>Using only inner most and outer most hits associated with this track.</li>
   </ol>
   \return tracklength
   \throws runtime_error
*/
double StiKalmanTrack::getTrackLength() const
{
  StiKalmanTrackNode * inNode = getInnerMostHitNode(3);
  if (!inNode) 					return 0;
  StThreeVectorD in(inNode->x_g(),inNode->y_g(),inNode->z_g());
  StiKalmanTrackNode * otNode = getOuterMostHitNode(3);
  if (!otNode) 					return 0;
  if (inNode==otNode) 				return 0;
  if (otNode->getX()-inNode->getX()<1e-3) 	return 0;
  StThreeVectorD ot(otNode->x_g(),otNode->y_g(),otNode->z_g());
  StPhysicalHelixD hlx(fabs(inNode->getCurvature()),
		            inNode->getDipAngle(),
		            inNode->getPhase(),
		            in,
		            inNode->getHelicity());
  double per = hlx.period();
  double len = hlx.pathLength(ot);
//  StHelix can return negative length if -ve path is shorter then +ve one
//  period ia added in this case;
  while(len<0  ) {len+=per;}
//  StHelix can return length > period id dip is almost zero and by chance
//  this length is more suitable for Z distance. It is wrong solution even
//  it has better distance to end point. Subtruct period in this case
  while(len>per) {len-=per;}

  return len;
}


/*! Calculate and return the track segment length between two nodes.
  <h3>Notes</h3>
  <ol>
  <li>This is basically a wrapper for the StiKalmanTrackNode::pathLToNode function.</li>
  </ol>
  \return length
 */
//  inline double StiKalmanTrack::calculateTrackSegmentLength(const StiKalmanTrackNode &p1, const StiKalmanTrackNode &p2) const
//  {
//    return p1.pathLToNode(&p2);
//  }

/*! Calculates the radiation length of material crossed by the track.

 */
double StiKalmanTrack::getTrackRadLength() const
{
  double x1, x2, x3;  //lengths in different media
  double totalR=0.;
  //Are we going in or out? Makes a difference which material to call

  StiKTNBidirectionalIterator tNode = begin();

  //set initial conditions for tNode, the 'current' node;
  //will also need 'nextNode', ie node which is next 
  StiKalmanTrackNode *thisNode = &(*tNode);

  x1=thisNode->pathlength()/2.; 
  x3=0.;


  //  while ((++tNode)!=end() && (*tNode).getDetector())
  while ((tNode++)!=end() && (*tNode).getDetector())
    {

      StiKalmanTrackNode *nextNode = &(*(tNode)); //incrimented tNode
    

      x2=thisNode->pathLToNode(nextNode); 
      x3=nextNode->pathlength()/2.;

      if(x3==-1.) continue;  
      //if there is an error with "next" node, proceed along track
      //without updating current node. This should provide
      //function thisNode=thisNode, nextNode=new node

      if (x2> (x1+x3)) x2 = x2 - x1 - x3; //x2 is now the gap distance
      else x2=0.;

      TRACKMESSENGER
	   <<"getTrackRadLength:"
	   <<"\n\tIn Detector: "<<thisNode->getDetector()->getName()
	   <<"\n\t\tMaterial: "<<thisNode->getDetector()->getMaterial()
	   <<"\n\t\tLength: "<<x1
	   <<"\t\tGap Length: "<<x2
           <<"\n\tNext Detector: "<<nextNode->getDetector()->getName()
	   <<"\n\t\tMaterial: "<<nextNode->getDetector()->getMaterial()
	   <<"\n\t\tLength: "<<x3
           << endl;
	{
	  if (thisNode->getX0()>0)    totalR += x1/thisNode->getX0();
	  if (nextNode->getGasX0()>0) totalR += x2/nextNode->getGasX0();
	  if (nextNode->getX0()>0)    totalR += x3/nextNode->getX0();
	}
      //cache nextNode for next iteration...
      thisNode = nextNode;
      x1       = x3;
    }
  if (totalR>200.)
    cout <<"StiKalmanTrack::getTrackRadLength() -W- Total Rad Length Error: "<<totalR;
  return totalR;
}
//_____________________________________________________________________________
double StiKalmanTrack::getNearBeam(StThreeVectorD *pnt,StThreeVectorD *dir) const
{
  StiKalmanTrackNode * inNode = lastNode;
  StThreeVectorD in(inNode->x_g(),inNode->y_g(),inNode->z_g());

  StPhysicalHelixD hlx(fabs(inNode->getCurvature()),
		            inNode->getDipAngle(),
		            inNode->getPhase(),
		            in,
		            inNode->getHelicity());
  double per = hlx.period();
  double len = hlx.pathLength(0.,0.);
//  StHelix can return negative length if -ve path is shorter then +ve one
//  period ia added in this case;
  if (fabs(len) > fabs(len+per)) len+=per;
  if (fabs(len) > fabs(len-per)) len-=per;

  hlx.moveOrigin(len);
  if (pnt) (*pnt) = hlx.at(0);

  if (dir) {
    double phase = hlx.phase();
    double dip   = hlx.dipAngle();
    int h        = hlx.h();

    (*dir)[0]= -sin(phase)*cos(dip)*h;	
    (*dir)[1]=  cos(phase)*cos(dip)*h;
    (*dir)[2]=             sin(dip)*h;}

  return fabs(len);
}

//_____________________________________________________________________________
/*! Return the inner most hit associated with this track.
   <h3>Notes</h3>
   <ol>
   <li>Throws logic_error exception if firstNode or lastNode are not defined, or if track has no hit.</li>
   <li>Loop through all nodes from end() to begin() (or vice versa if tracking 
       direction is outside-in) and search for node with hit. Return first hit found.</li>
   </ol>
	 \return inner most hit node on this track
	 \throws logic_error
*/
StiKalmanTrackNode * StiKalmanTrack::getInnOutMostNode(int inot,int qua)  const
{
  if (firstNode==0 || lastNode==0)
 {
  //cout << "StiKalmanTrack::getInnOutMostNode() -E- firstNode||lastNode==0" << endl;
  throw runtime_error("StiKalmanTrack::getInnOutMostNode() -E- firstNode||lastNode==0");
 }
  
  StiKalmanTrackNode *node;
  StiKTNBidirectionalIterator it =(inot) ? begin():rbegin();
  for (;(node=it());it++){
    if (!node->isValid()) 				continue;
    StiHit *hit = node->getHit();
    if ((qua&1) && !hit) 				continue;
    if ((qua&2) && hit && node->getChi2()>10000.)	continue;
    return node;
  }
  cout << "StiKalmanTrack::getInnOutMostNode() -E- No requested nodes " << endl;
  //throw runtime_error("StiKalmanTrack::getInnOutMostNode() -E- No requested nodes");
  return 0;
}
StiKalmanTrackNode * StiKalmanTrack::getOuterMostHitNode(int qua)  const
{
  return getInnOutMostNode(1,qua|1);
}


/*! Return the inner most hit associated with this track.
   <h3>Notes</h3>
   <ol>
   <li>Throws logic_error exception if firstNode or lastNode are not defined, or if track has no hit.</li>
   <li>Loop through all nodes from begin() to end() (or vice versa if tracking 
       direction is outside-in) and search for node with hit. Return first hit found.</li>
   </ol>
	 \return outer most hit node on this track
*/

StiKalmanTrackNode * StiKalmanTrack::getInnerMostHitNode(int qua)   const
{
  return getInnOutMostNode(0,qua|1);
}
//_____________________________________________________________________________
int StiKalmanTrack::getNNodes(int qua)  const
{
  StiKalmanTrackNode *node;
  StiKTNBidirectionalIterator it = begin();
  int nn=0;
  for (;(node=it());it++){
    if (!node->isValid()) 				continue;
    StiHit *hit = node->getHit();
    if ((qua&1) && !hit) 				continue;
    if ((qua&2) && hit && node->getChi2()>10000.)	continue;
    nn++;
  }
  return nn;
}

//_____________________________________________________________________________
/*! Return true if inner most hit associated with this track is main vertex.
   <h3>Algorithm</h3>
   <ol>
   <li>Find the inner most hit node associated with this tracks.</li>
   <li>Return true if "x" of inner most hit is less than 2 cm.
   </ol>
	 \return true if "x" of inner most hit is less than 2 cm.
*/
bool  StiKalmanTrack::isPrimary() const
{
  StiKalmanTrackNode * node = getInnerMostHitNode(3);
  if (node->getDetector()) return 0;
  return (fabs(node->getX())<2.);
}


//_____________________________________________________________________________
///return vector of nodes with hits
vector<StiKalmanTrackNode*> StiKalmanTrack::getNodes(int detectorId) const
{
  StiKTNBidirectionalIterator it;
  vector<StiKalmanTrackNode*> nodeVec;
  for (it=begin();it!=end();++it) {
          StiKalmanTrackNode* node = &(*it);
    const StiHit* hit = node->getHit();
    if(!hit) 				continue;
    const StiDetector *det = hit->detector();
    if (!det) 				continue;
    if (node->getDedx()<=0.)		continue;   
    if (detectorId!=det->getGroupId())	continue;
    nodeVec.push_back(node);
  }
  return nodeVec;
}

//_____________________________________________________________________________
///return hits;
vector<const StMeasuredPoint*> StiKalmanTrack::stHits() const
{
  StiKTNBidirectionalIterator it;
  vector<const StMeasuredPoint*> hits;
  for (it=begin();it!=end();++it) {
    const StiKalmanTrackNode* node = &(*it);
    if (!node->isValid()) 	continue;
    if (node->getChi2()>10000.) continue;
    const StiHit* hit = node->getHit();
    if (!hit) 			continue;
    if (!hit->detector())	continue;
    const StMeasuredPoint *stHit = hit->stHit();
    if (!stHit) 		continue;
    hits.push_back(stHit);
  }
  return hits;
}



/*! Declare hits associated with given track as used.
  <p>
  Declare hits on the track ending at "node" as used. 
  This method starts with the last node and seeks the
  parent of each node recursively. The hit associated with each
  node (when there is a hit) is set to "used".
*/	
void StiKalmanTrack::reserveHits()
{
  StiKTNForwardIterator it(lastNode);
  for_each( it, it.end(), SetHitUsed() );
}

/*! Extend track to the given vertex.
  <p>
  Attempt an extension of the track  the given vertex. 
  <p>
  <ol>
  <li>Get node from node factory.</li>
  <li>Reset node.</li>
  <li>Propagate the node from given parent node "sNode", to the given vertex using a 
  call to "propagate".</li>
  <li>Evaluate the chi2 of the extrapolated if the vertex is added to the track. Done
  using a call to "evaluateChi2".</li>
  <li>If chi2 is less than max allowed "maxChi2ForSelection", update track parameters
  using the vertex as a measurement and add the vertex to the track as the last node.</li>
  </ol>
  <h3>Notes</h3>
  <ul>
  <li>Throws logic_error if no node can be obtained from the node factory.</li>
  <li>The methods "propagate", "evaluateChi2", and "updateNode" may throw 
  runtime_error exceptions which are NOT caught here...</li>
  </ul>
*/
bool StiKalmanTrack::extendToVertex(StiHit *vertex, const StiDetector * alternate)
{
  if (!alternate)
    return extendToVertex(vertex);
  cout << " alternate propagate =========="<<endl;
  cout << *alternate<<endl;
  cout << "lastNode:"<<*lastNode<<endl;
  StiKalmanTrackNode *lastGoodNode = getInnOutMostNode(0,0);
  StiKalmanTrackNode * tNode = trackNodeFactory->getInstance();
  tNode->reset();
  int status = tNode->propagate(lastGoodNode,alternate,kOutsideIn);
  cout << "propagate status:"<<status<<endl;
  return false;
}


StiTrackNode *StiKalmanTrack::extendToVertex(StiHit* vertex)
{
  double chi2;
  StiKalmanTrackNode * sNode=0;
  StiKalmanTrackNode * tNode=0;
  bool trackExtended = false;

  StiKalmanTrackNode * innerMostHitNode = getInnerMostHitNode();
  if (!innerMostHitNode) 		return 0;
  // track with hits in the outer portion of the TPC only are not considered
  if (innerMostHitNode->getX()>100.) 	return 0;
		
  StiHit localVertex = *vertex;
  sNode = getInnerMostNode();

  localVertex.rotate(sNode->getAlpha());
  tNode = trackNodeFactory->getInstance();
  StiHit *myHit;
  //cout << "SKT::extendToVertex() -I- x,y,z:"<< localVertex.x() 
  //     << " " <<  localVertex.y() << " " << localVertex.z() << endl;
  //cout << "SKT::extendToVertex() -I- sNode->getX():"<<sNode->getX()<<endl;
  //cout << "SKT::extendToVertex() -I-0 tNode->getX():"<< tNode->getX()<<endl;
  if (tNode->propagate(sNode, &localVertex,kOutsideIn))
    { 
      //cout << " on vertex plane:";
      chi2 = tNode->evaluateChi2(&localVertex); 
      
      double dx,dy,dz,d;
      dx=tNode->getX()- localVertex.x();
      dy=tNode->getY()- localVertex.y();
      dz=tNode->getZ()- localVertex.z();
      d= ::sqrt(dx*dx+dy*dy+dz*dz);
	_dca = ::sqrt(dy*dy+dz*dz);
#ifdef Sti_DEBUG      
	int npoints[2] = {0,0};
	vector<StMeasuredPoint*> hitVec = stHits();
	for (vector<StMeasuredPoint*>::iterator point = hitVec.begin(); point!=hitVec.end();++point) {
	  StHit * hit = dynamic_cast<StHit *>(*point);
	  if (hit) {
	    StDetectorId detId = hit->detector();
	    if (detId == kTpcId) ++npoints[0];
	    if (detId == kSvtId) ++npoints[1];
	  }
	}
	cout << "StiKalmanTrack::extendToVertex: localVertex: " << localVertex << endl;
	cout << "StiKalmanTrack::extendToVertex: chi2 @ vtx: " << chi2 
	     << " dx:"<< dx
	     << " dy:"<< dy
	     << " dz:"<< dz
	     << " d: "<< d
	     << " dca: " << _dca << " npoints tpc/svt: " << npoints[0] << "/" << npoints[1] << endl;
	cout << "StiKalmanTrack::extendToVertex: TrackBefore:" << *this << endl;
#endif

      _vDca = d;
      _vChi2= chi2;
//    if (chi2<pars->maxChi2Vertex  && d<4.)
      if (                             d<4.)
	{
	  //_dca = ::sqrt(dy*dy+dz*dz);
	  myHit = StiToolkit::instance()->getHitFactory()->getInstance();
	  *myHit = localVertex;
	  tNode->setHit(myHit);
	  tNode->setChi2(chi2);
	  tNode->setDetector(0);
          trackExtended = (tNode->updateNode()==0);
          
#ifdef Sti_DEBUG      
cout << "StiKalmanTrack::extendToVertex: TrackAfter:" << *this << endl;
#endif
if (debug()) cout << "extendToVertex:: " << StiKalmanTrackNode::Comment() << endl;

	  if (trackExtended) return tNode;
          trackNodeFactory->free(tNode);             
	}
    }
  //else
  //  cout <<" TRACK NOT REACHING THE VERTEX PLANE!"<<endl;
  return 0;
}

bool StiKalmanTrack::find(int direction)
{
static int nCall=0; nCall++;
//StiDebug::Break(nCall);
  bool trackExtended=false;  
  bool trackExtendedOut=false;
  int status = 0;
  setFlag(0);
  // invoke tracker to find or extend this track
  //cout <<"StiKalmanTrack::find(int) -I- Outside-in"<<endl;
  try 
    {
      if (getNNodes(3)<4) return false;
      status = fit(kOutsideIn);
      if (getNNodes(3)<4) return false;
      if (debug()) cout << "StiKalmanTrack::find seed " << *((StiTrack *) this);
      double radSvt= 25.;
      if (trackFinder->find(this,kOutsideIn,radSvt)) {
          status = refit(); if(status) return false;
	  trackExtended = getNNodes(3)>5;
      }	

      if (trackFinder->find(this,kOutsideIn,0.)) {
          status = refit(); if(status) return false;
	  trackExtended = trackExtended || getNNodes(3)>5;
      }	
    }
  catch (runtime_error & error)
    {
      cout << "SKT:find(int dir) -W- ERROR:" << error.what()<<endl;
    }
  // decide if an outward pass is needed.
  const StiKalmanTrackNode * outerMostNode = getOuterMostNode(2);
  if (!outerMostNode)
    {
      setFlag(-1);
      return false;
    }
  if (outerMostNode->getX()<185. )
    {
      try
	{
	  if (debug()) cout << "StiKalmanTrack::find swap " << *((StiTrack *) this);
	  trackExtendedOut= trackFinder->find(this,kInsideOut);
          if (trackExtendedOut) { 
             status = refit(); if(status) return false;
	     trackExtendedOut = getNNodes(3)>5;
          }
	  if (debug()) cout << "StiKalmanTrack::find find(this,kInsideOut)" << *((StiTrack *) this);
	}
      catch (...)
	{
	  cout << "StiKalmanTrack::find(int direction) -W- Exception while in insideOut find"<<endl;
	}
      //cout<<"StiKalmanTrack::find(int) -I- Swap back track"<<endl;
    }
  if (getNNodes(3)>=5) refitL();
  setFlag(1);
  //cout << " find track done" << endl;
  return trackExtended||trackExtendedOut;
}

void StiKalmanTrack::setParameters(StiKalmanTrackFinderParameters *parameters)
{
  pars = parameters;
}

void StiKalmanTrack::setFitParameters(StiKalmanTrackFitterParameters *parameters)
{
  fitpars = parameters;
}

///Return all the hits associated with this track, including those with a large incremental
///chi2 that may not contribute to the fit.
vector<StiHit*> StiKalmanTrack::getHits()
{
  vector<StiHit*> hits;
  StiKalmanTrackNode* leaf = getLastNode();
  StiKTNForwardIterator it(leaf);
  StiKTNForwardIterator end = it.end();
  for (;it!=end;++it) 
    {
      const StiKalmanTrackNode& node = *it;
      if (!node.isValid())		continue;
      if (node.getChi2()>10000.) 	continue;
      StiHit* hit = node.getHit();
      if (!hit) 			continue;
      hits.push_back(hit);
    }
  return hits;
}

//_____________________________________________________________________________
/// Return global dca of the track relative to given vertex or point.
double  StiKalmanTrack::getDca(const StiHit * vertex)    const
{
  StiKalmanTrackNode*	node;

  node = getInnerMostHitNode(2); 
  StThreeVectorD originD(node->x_g(),node->y_g(),node->z_g());
  StThreeVectorD vxDD(vertex->x_g(), vertex->y_g(),vertex->z_g());
  StPhysicalHelixD physicalHelix(0.,0.,0.,originD,-1);
  physicalHelix.setParameters(fabs(node->getCurvature()),
			       node->getDipAngle(),
			       node->getPhase(),
			       originD,
			       node->getHelicity());
  double dca = physicalHelix.distance(vxDD);
  return dca;
}
//_____________________________________________________________________________
ostream& operator<<(ostream& os, const StiKalmanTrack& track)
{
  try 
    {
      os << *((StiTrack *) &track);
      os <<"List of nodes" << endl;
      StiKTNBidirectionalIterator tNode = track.begin();
      StiKTNBidirectionalIterator eNode = track.end();
      //set initial conditions for tNode, the 'current' node;
      //will also need 'nextNode', ie node which is next 
      while (tNode != eNode) {
	StiKalmanTrackNode *thisNode = &(*tNode);
	if (thisNode) os << *thisNode;
	tNode++;
      }
    }
  catch (runtime_error & rte)
    {
      os << " Run-time Error while accessing track parameters: " << rte.what() << endl;
    }
  catch (logic_error & le)
    {
      os << " Logic Error while accessing track parameters: " << le.what() << endl;
    }
  return os;
}

//_____________________________________________________________________________
///Extrapolate this track to the beam axis (x==0) to provide an estimate of the
///track location at the beam axis.
///Returns a null pointer is the operation cannot be completed i.e. the track does not reach
///the beam axis plane.
StiKalmanTrackNode * StiKalmanTrack::extrapolateToBeam()
{
  StiKalmanTrackNode * innerMostNode = getInnerMostNode();
  //return null if there is no node to extrapolate from.
  if (!innerMostNode) return 0;
  StiKalmanTrackNode * n = trackNodeFactory->getInstance();
  if (n->propagateToBeam(innerMostNode,kOutsideIn)) return n;
  trackNodeFactory->free(n);
  return 0;
}

//_____________________________________________________________________________
StiKalmanTrackNode * StiKalmanTrack::extrapolateToRadius(double radius)
{
  StiKalmanTrackNode * outerMostNode = getOuterMostNode();
  //return null if there is no node to extrapolate from.
  if (!outerMostNode) return 0;
  StiKalmanTrackNode *n = trackNodeFactory->getInstance();
  if (n->propagateToRadius(outerMostNode,radius,kOutsideIn)) return n;
  trackNodeFactory->free(n);
  return 0;
}

//_____________________________________________________________________________
void StiKalmanTrack::add(StiTrackNode * node,int direction)
{
   StiKalmanTrackNode *Node = (StiKalmanTrackNode*)node;
   if (lastNode==0) {
     lastNode = firstNode = Node; return;
   }
   if (direction==0) {
     lastNode->add(Node,direction);
     lastNode = Node;
  } else {
     firstNode->add(Node,direction);
     firstNode = Node;
  }
}
//_____________________________________________________________________________
void StiKalmanTrack::setFirstLastNode(StiKalmanTrackNode * node)
{
 firstNode = (StiKalmanTrackNode*)node->getFirstNode();
  lastNode = (StiKalmanTrackNode*)node->getLastNode ();
}
//_____________________________________________________________________________
void StiKalmanTrack::removeLastNode()
{
  StiKalmanTrackNode *node = lastNode;
  lastNode = (StiKalmanTrackNode*)node->disconnect();
  BFactory::Free(node);
}
//_____________________________________________________________________________
int StiKalmanTrack::refit() 
{
static int nCall=0; nCall++;
  enum {kMaxIter=30};
static double convFactor=1.;
  double errFactor=1;
  int nNBeg = getNNodes(3);
  if (nNBeg<3) return -1;
  StiKalmanTrackNode *inn,*out;
  inn = getInnerMostNode(3);
  int fail = 1,status;
  double pars[kNPars]; memcpy(pars,inn->getPars(),sizeof(pars));
  double est;
  int iter;
  for (iter=0;iter<kMaxIter;iter++) {
    est = -1.;
    inn = getInnerMostNode(0);
    inn->resetError(convFactor);
    StiKalmanTrackNode::setErrFactor(errFactor);
    status = fit(kInsideOut);  
    errFactor += 1./convFactor;
    StiKalmanTrackNode::setErrFactor(1.);
    if (status) 	break;
    out = getOuterMostNode(0);
    out->resetError(convFactor);
    StiKalmanTrackNode::setErrFactor(errFactor);
    status = fit(kOutsideIn);  
    errFactor += 1./convFactor;
    StiKalmanTrackNode::setErrFactor(1.);
    if (status) 	break;
    inn = getInnerMostNode(3);
    if (!inn)		break;
    est=0;
    for (int i=1;i<kNPars;i++) {
	est += pow(pars[i]-inn->getPars()[i],2)/inn->getDiag(i);} 

    memcpy(pars,inn->getPars(),sizeof(pars));
      if (est<0.05) { fail=0; break;}
  }
  StiKalmanTrackNode::setErrFactor(1.);

  StiKTNBidirectionalIterator first = begin();
  StiKTNBidirectionalIterator last  = end();
  StiKTNBidirectionalIterator source;
  double chi2;
  double maxChi2 = fitpars->getMaxChi2();
  
  
  for (source=first;source!=last;source++) {
    StiKalmanTrackNode *node = &(*source);
    node->resetError(errFactor);
    if (!node->isValid())	continue;
    StiHit *hit = node->getHit();
    if (!hit) 			continue;
    chi2 = node->getChi2();
    if (chi2>1e3) 		continue;
    chi2 = node->evaluateChi2(hit);
    if (chi2>maxChi2) chi2=1e55;
    node->setChi2(chi2);
  }

//  int nNEnd = getNNodes(3);
//  if (nNBeg!=nNEnd) {printf("iter=%d est=%g nNodes %d %d nCall=%d\n",iter,est,nNBeg,nNEnd,nCall);}
  return fail;
}
//_____________________________________________________________________________
int StiKalmanTrack::refitL() 
{
static int nCall=0;nCall++;
// StiDebug::Break(nCall);

  StiKTNBidirectionalIterator source;
  StiKalmanTrackNode *pNode = 0,*targetNode;
  int iNode=0, status = 0;
  StiTrackNodeHelper tnh(fitpars->getMaxChi2());
  for (source=begin();source!=end();source++) {
    iNode++;
    targetNode = &(*source);
    if (!targetNode->isValid()) 	continue;
    tnh.setNodes(pNode,targetNode);
    status = tnh.fake1Fit();
    pNode = targetNode;
  }//end for of nodes

    pNode = 0; iNode=0;
    for (source=rbegin();source!=rend();source++) {
    iNode++;
    targetNode = &(*source);
    if (!targetNode->isValid()) 	continue;
    if (!pNode) {
      if (!targetNode->getHit() ) 	continue;
      if ( targetNode->getChi2()>1e3)	continue;
    }
    tnh.setNodes(pNode,targetNode);
    status = tnh.fake2Fit();
    pNode = targetNode;
  }//end for of nodes
  return 0;
}
//_____________________________________________________________________________
void StiKalmanTrack::reduce() 
{
  StiKTNBidirectionalIterator source;
  for (source=begin();source!=end();source++) {(*source).reduce();}
}
//_____________________________________________________________________________
void StiKalmanTrack::unset() 
{
  if (!lastNode) return;
  StiKTNBidirectionalIterator source;
  for (source=begin();source!=end();source++) {BFactory::Free(&(*source));}
  lastNode=0; firstNode=0;
}
//_____________________________________________________________________________
void StiKalmanTrack::print(const char *opt) const
{
  printf("\n Track %p\n",(void*)this);

  StiKTNBidirectionalIterator it;
  int n=0;
  for (it=begin();it!=end();++it) {
    n++;
    printf("%3d - ",n);
    (*it).print(opt);
  }
}
