////////////////////////////////////////////////////////////////////////////
//
// $Id: StFlowConstants.cxx,v 1.6 2002/03/12 02:33:17 posk Exp $
//
// Author: Art Poskanzer and Raimond Snellings, LBNL, Oct 1999
//
////////////////////////////////////////////////////////////////////////////
//
// Description: constants for the flow makers
//
////////////////////////////////////////////////////////////////////////////
#include "StFlowConstants.h"

ClassImp(Flow)
  
  // Commit with these values:
  Float_t Flow::etaMin = -4.5;
  Float_t Flow::etaMax =  4.5;
  //Float_t Flow::etaMin = -1.5;
  //Float_t Flow::etaMax =  1.5;

  Float_t Flow::ptMin     = 0.;
  Float_t Flow::ptMax     = 2.;
  Float_t Flow::ptMaxPart = 6.;


//////////////////////////////////////////////////////////////////////
//
// $Log: StFlowConstants.cxx,v $
// Revision 1.6  2002/03/12 02:33:17  posk
// Now makes pico files in SL02c.
//
// Revision 1.5  2001/12/18 19:21:53  posk
// "proton" and "antiproton" changed to "pr+" and "pr-".
// Compiles on Solaris.
//
// Revision 1.4  2001/12/11 21:33:40  posk
// Went from one to four sets of histograms for making the event plane isotropic.
// StFlowEvent::PhiWeight() has changed arguments and return value.
// The ptWgt saturates above 2 GeV/c.
//
// Revision 1.3  2001/11/13 22:43:46  posk
// Documentation updated.
//
// Revision 1.2  2001/11/10 01:08:03  posk
// Moved some constants into StFlowConstants.
//
//
//////////////////////////////////////////////////////////////////////
