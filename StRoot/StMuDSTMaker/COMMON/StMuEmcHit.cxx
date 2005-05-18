//###########################################################
// EMC Micro Event
// Author: Alexandre A. P. Suaide
// initial version 08/2001
//
// See README for details
//########################################################### 
#include "StMuEmcHit.h"

ClassImp(StMuEmcHit);

StMuEmcHit::StMuEmcHit()
{
}
StMuEmcHit::~StMuEmcHit()
{
}
StMuEmcHit::StMuEmcHit(StMuEmcHit &hit)
{
    mId=(short)hit.getId();
    mAdc=(short)hit.getAdc();
    mCalType=(char)hit.getCalType();
    mEnergy=hit.getEnergy();
}


