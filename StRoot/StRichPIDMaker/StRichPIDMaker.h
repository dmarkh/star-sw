/**********************************************************
 * $Id: StRichPIDMaker.h,v 2.17 2001/04/25 00:31:59 lasiuk Exp $
 *
 * Description:
 *  StRrsMaker is the main module
 *  StRichRawData. It has the standard Maker functions:
 *
 *  $Log: StRichPIDMaker.h,v $
 *  Revision 2.17  2001/04/25 00:31:59  lasiuk
 *  HP changes
 *
 *  Revision 2.16  2001/02/22 21:06:05  lasiuk
 *  fill the new StEvent structures in PidTraits, and richCollection
 *  dca code now included
 *
 *  Revision 2.15  2001/02/07 15:58:31  lasiuk
 *  update for production (production version and StEvent changes)
 *  refit and momentum loss are default behavior (Nikolai's parameterization)
 *  richCollection kept as data member
 *  reprocess the traits is default behavior
 *  creation of PIDTraits is done earlier
 *
 *  Revision 2.14  2001/02/01 17:55:30  horsley
 *  set energy loss in CTB at 20 MeV (default)
 *  ifdef'd out the TrackEntryClass
 *  StRichTrack::fastEnough() has materialsDB input for wavelenght's
 *
 *  Revision 2.13  2001/01/30 16:38:44  horsley
 *  updated PID maker for next production run, included new class for TTree
 *
 *  Revision 2.12  2000/12/14 19:21:37  horsley
 *  added data member to keep track of run id, mEventRunId
 *
 *  Revision 2.11  2000/12/08 20:10:36  horsley
 *  updated monte carlo functions,data  members
 *
 *  Revision 2.10  2000/12/08 04:54:57  lasiuk
 *  hit filter changed for refit
 *  fillCorrectedNTuple
 *  energy loss
 *  modify distup for PID
 *
 *  Revision 2.9  2000/11/25 11:55:14  lasiuk
 *  add reprocess Traits
 *
 *  Revision 2.8  2000/11/21 19:50:35  lasiuk
 *  add stthreevectorf
 *
 *  Revision 2.7  2000/11/21 16:24:22  horsley
 *  Major overhaul of StRichArea, introduced monte carlo integration cross check,
 *  all possible areas, angles calculated together. StRichRingCalculator, StRichPIDMaker modified to support new StRichArea. StRichPIDMaker's hit finder
 *  typo corrected.
 *
 *  Revision 2.6  2000/11/07 14:11:43  lasiuk
 *  initCutParameters() and diagnositis print added.
 *  bins for <d> and sigma_d added.
 *  TPC hits for RICH tracks written out.
 *  (file) ptr checked before writing ntuple.
 *  check flags on Hits instead of ADC value
 *
 *  Revision 2.5  2000/11/01 17:45:24  lasiuk
 *  MAJOR. hitFilter overhaul. members reordered, padplane dimension kept as
 *  a member.  addition of initTuple.  Additional dependencies of
 *  min/max algorithms
 *
 *  Revision 2.4  2000/10/19 01:13:23  horsley
 *  added member functions to StRichPIDMaker to make cuts on hits, tracks, events.
 *  added normal distance sigma cut on hits, quartz and radiator pathlengths
 *  for individual photons, modified minimization routine to correct boundary
 *  problems
 *
 *  Revision 2.3  2000/10/02 23:06:33  horsley
 *  *** empty log message ***
 *
 *  Revision 2.2  2000/09/29 01:35:37  horsley
 *  Many changes, added StRichRingHits, StRichMcSwitch, TpcHitvecUtilities
 *  Modified the StRichCalculator, StRichTracks, StRichMCTrack, StRichRingPoint
 *
 *  Revision 1.4  2000/06/16 02:37:12  horsley
 *  many additions, added features to pad plane display (MIPS, rings, etc)
 *  along with Geant info. Added StMcRichTrack. Modified access to hit collection.
 *
 *  Revision 1.2  2000/05/19 19:06:10  horsley
 *  many revisions here, updated area calculation ring calc, ring, tracks , etc...
 *
 *  Revision 1.1  2000/04/03 19:36:08  horsley
 *  initial revision
 **********************************************************/

#ifndef StRichPIDMaker_HH
#define StRichPIDMaker_HH

#include "StMaker.h"
#include "TNtuple.h"
#include "TH3.h"
#include "TFile.h"

#include <vector>
#ifndef ST_NO_NAMESPACES
using std::vector;
#endif

// SCL
#include "StParticleTypes.hh"
#include "StThreeVectorD.hh"
#include "StThreeVectorF.hh"
#include "StThreeVector.hh"

// StRichPid
//#include "StRichTrackFilter.h"
#include "StRichTrackingControl.h"
#include "StRichMcSwitch.h"
//#include "TpcHitVecUtilities.h"

#ifdef RICH_WITH_L3_TRACKS
#include "StDaqLib/L3/L3.Banks.hh"
#endif

// #define myPrivateVersion 1
#ifdef myPrivateVersion	
#include "TreeEntryClasses.h"
#endif


// SCL
class StParticleDefinition;

// StRichPID
class StTrack;
class StRichTrack;
class StRichRingCalculator;
class StRichMaterialsDb;

// StRrs
class StRichGeometryDb;
class StRichCoordinateTransform;
class StRichMomentumTransform;

// StRch
class StRichSimpleHitCollection;

// StDisplay
class StRichPadMonitor;

// StEvent
class StEvent;
class StRichCollection;
class StRichPidTraits;
class StSPtrVecRichHit;
class StSPtrVecRichPixel;
class StSPtrVecRichCluster;

class StMcTrack;
class StMcEvent;
class StRichMCHit;
class StRichHit;
class StRichMCTrack;
class St_g2t_track;

class StRichPIDMaker : public StMaker {

private:
  Bool_t drawinit;
  Char_t collectionName[256];  
  vector<StRichTrack* > mListOfStRichTracks; //!
  vector<StParticleDefinition* > mListOfParticles; //!
  
  bool  mTrackRefit;
  bool  mDoGapCorrection;
  
  double mPionSaturatedArea;
  double mKaonSaturatedArea;
  double mProtonSaturatedArea;
  
  bool   mPrintThisEvent;

  int mTotalEvents;
  int mGoodEvents;

    //
    // rich collection
    //
    StRichCollection* mRichCollection;//!
    
  //
  // monte carlo event
  //
  StMcEvent* mEvent; //!
  
  //
  // Event Parameters
  //
  StThreeVectorF mVertexPos;
  float         mMagField;
  int           mEventN;
  int           mEventRunId;
  
    
  //
  // Tracks in the RICH
  //
  int mNumberOfPrimaries;
  int mNegativePrimaries;
  int mNumberOf1GeV;
  int mNumberOfRingHits;
  int mRichTracks;
  
  //
  // wavelength bounds
  //
  double mShortWave;
  double mLongWave;
  double mDefaultShortWave;
  double mDefaultLongWave;
  
  // hit filter
  double innerDistance,outerDistance,meanDistance;
  double innerAngle,outerAngle,meanAngle; 
  double ringWidth;
  
  
  //
  // pad plane dimensions
  StThreeVector<double>   mPadPlaneDimension; //!
  StThreeVector<double>   mRadiatorDimension; //!
  
  StRichPadMonitor*  mPadMonitor; //!
  StRichGeometryDb*  mGeometryDb; //!
  StRichMaterialsDb* mMaterialDb; //! 
  StRichCoordinateTransform* mCoordinateTransformation; // !  
  StRichMomentumTransform*   mMomentumTransformation; //!
  
  //
  // Cuts: parameters and types
  //
  
  // Run
    int mProductionVersion;
    
  // Event
  float mVertexWindow;
  
  // Hits
  int   mAdcCut;
  
  // Track
  float mLastHitCut;
  float mDcaCut;
  int   mFitPointsCut;
  float mEtaCut;
  float mPtCut;
  float mPathCut;
  float mPadPlaneCut;
  float mRadiatorCut;
  float mThresholdMomentum;
  
  // convergence limit for psi determination
  double mPrecision;
  
  char* fileName; //!
  char* mySaveDirectory; //!
  
  // particles
  StPionMinus*  pion;   //!
  StKaonMinus*  kaon;   //!
  StAntiProton* proton; //!
  
  TFile*   file;          //!
  TNtuple* geantTrackNtuple;   //!
  TNtuple* geantPhotonNtuple;   //!
  TNtuple* geantPixelNtuple;   //!
  TNtuple* geantCloseHitNtuple;   //!

#ifdef myPrivateVersion	
  TTree* myTree; 
  TrackEntry* m_Track; 
#endif

  //
  // brian
  //
  TNtuple* distup; //!
  
  //TpcHitVecUtilities* util; //!
  bool     kWriteNtuple;    //!
  
      
public:
  
    StRichPIDMaker(const Char_t *name="RICHPID", bool writeNtuple=false);
    virtual ~StRichPIDMaker();
  
    Int_t Init();
    Int_t Make();
    Int_t Finish();

    void initCutParameters();
    void initNtuples();

    //
    // hit operations
    //
    void hitFilter(const StSPtrVecRichHit*, StRichRingCalculator*);
    void hitFilter(StRichRingCalculator* , StThreeVectorF&, float&, float&);
    
    float getHitSigma(float); 
    void  tagMips(StEvent*, StSPtrVecRichHit*);

    //
    // track operations
    //
    Int_t fillTrackList(StEvent*, const StSPtrVecRichHit*);
    Float_t calculateSignedDca(const StTrack*, double*);
    
    //
    // functions to apply cuts
    // and set cut parameters
    //
    bool checkEvent(StEvent*);
    bool checkHit(StRichHit*);
    bool checkTrack(StRichTrack*);
    bool checkTrack(StTrack*);
    bool checkTrackMomentum(float);

    //
    // set/Print cut parameters at the macro Level
    //

    void printCutParameters(ostream& os=cout) const;
    void setProductionVersion(int no);
    int  productionVersion() const;
    
    // Event Level
    void setVertexWindow(float);

    // Hit Level
    void setAdcCut(int);

    // Track Level
    void setLastHitCut(float);
    void setEtaCut(float);
    void setDcaCut(float);
    void setPtCut(float);
    void setFitPointsCut(int);
    void setPathLengthCut(float);
    void setPadPlaneCut(float);
    void setRadiatorCut(float);

    void Clear(Option_t *option="");
    void DefineSaveDirectory(char*);

    //
    // analysis parameters and flags
    //
    void setFileName(char *);
    void setWavelengthRange(double, double);
    void setTrackRefit(bool);
    void drawPadPlane(StEvent*, bool );
  


    // StEvent
    void fillPIDTraits(StRichRingCalculator*);
    bool reprocessTheTraits(StRichPidTraits*);
    void fillRichSoftwareMonitor(StEvent*);

#ifdef  myRICH_WITH_MC
    void fillMcTrackNtuple(const StSPtrVecRichCluster*);
    void fillMcPhotonNtuple(StMcEvent*, const StSPtrVecRichCluster*, const StSPtrVecRichHit*);
    void fillMcPixelNtuple(const StSPtrVecRichPixel*);
    void fillGeantHitNtuple();

    void getGeantPhotonInfo(StRichMCTrack* richTrack, StMcTrack* photon, float& wave, float& gpsi, float& z, float& gtheta, float& gphottheta);
  
    bool makeTrackAssociations(StMcEvent*, const StSPtrVecRichHit* hits); 

    StMcTrack*     getStMcTrack(StRichMCHit* , StMcEvent* , St_g2t_track*);
    StThreeVectorF getTheGeantHitOnPadPlane(StMcTrack*, StThreeVectorF&);
#endif

    void clearTrackList();
  
  
#ifdef RICH_WITH_L3_TRACKS
  double findL3ZVertex(globalTrack *,int);
#endif    
  
  ClassDef(StRichPIDMaker,1)
    };

inline void StRichPIDMaker::setProductionVersion(int no) {mProductionVersion = no;}
inline int  StRichPIDMaker::productionVersion() const {return mProductionVersion;}
#endif
