/***************************************************************************
 *
 * StMcFtpcHit.cc
 *
 **************************************************************************/
#include "StMcEvent/StMcFtpcHit.hh"
#include "StMcEvent/StMcTrack.hh"

static const char rcsid[] = "$Id: StMcFtpcHit.cc,v 1.1.1.1 1999/07/13 18:17:15 uid2620 Exp $";

StMcFtpcHit::StMcFtpcHit(const StThreeVector<float>& p,
			 const float de, const float ds,
			 StMcTrack* parent)  : StMcHit(p, de, ds, parent)
{ /* noop */ }

StMcFtpcHit::StMcFtpcHit(g2t_ftp_hit_st* pt)
{
  mdE = pt->de;
  mdS = pt->ds;
  // Decode position.
  mPosition.setX(pt->x[0]); 
  mPosition.setY(pt->x[1]);
  mPosition.setZ(pt->x[2]);
  // For parent track, the g2t_hits table only gives the id of 
  // the parent track :  pt->track_p  .  We need to decode this and assign
  // mParentTrack to the pointer to the parent track.
}



