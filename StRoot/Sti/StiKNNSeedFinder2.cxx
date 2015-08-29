int gMyEta =0;
int myKont = 0;
int gMyNextSeed=0;

///\file StiKNNSeedFinder.cxx 
///\author Victor Perev (BNL Software) 11/2012
#include <math.h>
#include "StarRoot/THelixTrack.h"
#include "StiHit.h"
#include "StiHitContainer.h"
#include "StiKNNSeedFinder2.h"
#include "StiToolkit.h"
#include "StiDetectorContainer.h"
#include "StiSortedHitIterator.h"
#include "StiKalmanTrack.h"
#include "StarRoot/StMultiKeyMap.h"
#include "StiUtilities/StiDebug.h"
#include "StEvent/StEnumerations.h"

#define __NOSTV__

#define StvHit_HH 1
#define ST_TGEOHELPER_H

#include "Stv/StvStl.h"

typedef  StiDetector StHitPlane;
class StvHit: private StiHit
{
public:
  int timesUsed() const 		{ return StiHit::isUsed();}
  void setTimesUsed(int i) 		{ StiHit::setTimesUsed(i);}
  const StiDetector* detector() const 	{ return StiHit::detector();}
  const float *x() const		{ return &_xg            ;}
};


#define StvSeedFinder_HH 1
class StvSeedFinder
{
public:
   StvSeedFinder(const char *name=0)	{if (name){};}
   void Clear() 			{fSeedHits.clear();}
const THelixTrack* Approx() 		{return (THelixTrack*)(-1);}
const StvHits *GetHits() const 		{return &fSeedHits;}
void  FeedBack(int)     {};
   float fXi2[2];
protected:
   StvHits  fSeedHits;
   
};
#define __STVDEBUG_h_
class StvDebug
{
public:
static void Count(const char *name,double x)		{StiDebug::Count(name,x);}
static void Count(const char *name,double x,double y)	{StiDebug::Count(name,x,y);}
static int  Flag (const char*)				{return 0;}
static int  iFlag (const char *flagName, int dflt=0)	{return StiDebug::iFlag(flagName,dflt);}
};


#ifdef KNNGONE
#include "StvSeed/StvGoneRejector.cxx"
#endif
#ifndef KNNGONE
#include "StvSeed/StvConeRejector.cxx"
#endif
#include "StvSeed/StvKNSeedSelector.h"
#include "StvSeed/StvKNSeedFinder.h"

class myStvKNSeedFinder: public StvKNSeedFinder
//______________________________________________________________________________
{
  public:
  void Reset();
  void Clear();
};


//______________________________________________________________________________
StiKNNSeedFinder::StiKNNSeedFinder()
{
  fRxyMin=0;
  fStvKNSeedFinder = new myStvKNSeedFinder;
}

//______________________________________________________________________________
StiKNNSeedFinder::~StiKNNSeedFinder()
{
  delete fStvKNSeedFinder;
}

//______________________________________________________________________________
/// Produce the next track seed 
/// Loop through available hits and attempt to form a track seed
/// Only use hits that have not been already used in fully formed
/// tracks. 
//______________________________________________________________________________
StiTrack* StiKNNSeedFinder::findTrack(double rMin)
{
static StiToolkit *kit = StiToolkit::instance();
  fRxyMin = rMin;
  while(1) {
    fTrack = 0;
    const THelixTrack *hel = fStvKNSeedFinder->NextSeed();
gMyNextSeed++;
    if (!hel) 		return 0;
    const std::vector<StiHit*> *hits = (const std::vector<StiHit*>*)fStvKNSeedFinder->GetHits();
    if (!hits)		continue;
    if (hits->size()<5)	continue;

    fTrack = kit->getTrackFactory()->getInstance();
    fEta=0;
    int ifail = ((StiKalmanTrack*)fTrack)->initialize(*hits);
    if (ifail) 		continue;
    fEta = fTrack->getPseudoRapidity();

    gMyEta = 0;
    if (fabs(fEta) < 0.1 ) {
      StiKalmanTrackNode *node=fTrack->getInnerMostHitNode();
      if (fabs(node->z_g())<20) {
       gMyEta = ++myKont;
    } }



    return  fTrack;
  }
}

//______________________________________________________________________________
void myStvKNSeedFinder::Reset()
{
static StiToolkit *kit = StiToolkit::instance();  
assert(!f1stHitMap->size()); 

  StiHitContainer      *hitContainer     = kit->getHitContainer();
  const HitMapToVectorAndEndType& mymap = hitContainer->hits();
  for (HitMapToVectorAndEndType::const_iterator it=mymap.begin()
      ;it != mymap.end()
      ;++it) {
    const vector<StiHit*> &vh = (*it).second.hits();
    for (int ih=0;ih<(int)vh.size();ih++) {
      StvHit *hit = (StvHit*)vh[ih];
      if (hit->timesUsed()) continue;
      const float *x = hit->x();
      double qwe = x[0]*x[0]+x[1]*x[1]+x[2]*x[2];
      f1stHitMap->insert(std::pair<float,StvHit*>(-qwe, hit));
      float xx[6] = {x[0],x[1],x[2],x[0]-x[1],x[1]-x[2],x[2]-x[0]};
      fMultiHits->Add(hit,xx);
  } }

  fMultiHits->MakeTree();
  *f1stHitMapIter = f1stHitMap->begin();

}
//______________________________________________________________________________
void StiKNNSeedFinder::reset()
{
fStvKNSeedFinder->Reset();
}
//______________________________________________________________________________
void StiKNNSeedFinder::clear()
{
fStvKNSeedFinder->Clear();
}
//______________________________________________________________________________
void myStvKNSeedFinder::Clear()
{
fMultiHits->Clear();
f1stHitMap->clear();
}
//______________________________________________________________________________
void StiKNNSeedFinder::FeedBack(int badGood)
{
  if (!fEta) return;
  if (badGood<=0) {
    StiDebug::Count("BadEta",fEta);
  } else {
    fEta = fTrack->getPseudoRapidity();
    int nHits = fTrack->getPointCount(kPxlId);
    nHits    += fTrack->getPointCount(kIstId);
    nHits    += fTrack->getPointCount(kSstId);
    StiDebug::Count("GoodEta",fEta);

    if (gMyEta) {
      StiKalmanTrackNode *node=fTrack->getInnerMostHitNode();
      if (fabs(node->z_g())<10) {
    printf("UUUUUUU=%d\n",gMyEta);
    }}

    if (nHits>=2) StiDebug::Count("HftEta",fEta);
  }
}
//______________________________________________________________________________
//______________________________________________________________________________
//______________________________________________________________________________
#include "StvSeed/StvSeedConst.h"
#include "Stv/StvStl.cxx"
#define Cop CopRej

#undef Cop
#define Cop CopSel
#include "StvSeed/StvKNSeedSelector.cxx"

#include "TCernLib.h"
#include "TVector3.h"
#include "StMultiKeyMap.h"
#include "THelixTrack.h"
#undef Cop
#include "StvSeed/StvKNSeedFinder.cxx"