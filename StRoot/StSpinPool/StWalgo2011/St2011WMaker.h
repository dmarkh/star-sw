// $Id: St2011WMaker.h,v 1.7 2012/07/13 20:53:16 stevens4 Exp $

#ifndef STAR_St2011WMaker
#define STAR_St2011WMaker

/*!
 *                                                                     
 * \class  St2011WMaker
 * \author Jan Balewski, MIT; 
 * \author Endcap: Justin Stevens, IUCF
 * \author JetFinder/JetReader interface: Ilya Selyuzhenkov, IUCF
 * \date   August 2009
 * \brief  muDst based extraction of W-signal from pp500 data from 2011
 *
 *
 */                                                                      

#ifndef StMaker_H
#include "StMaker.h"
#endif
#include <TChain.h>
#include <TRandom3.h>

#include "Wevent2011.h"
#include "WtpcFilter.h"

class  TObjArray;
class  TH1I;
class  TH2I;
class  StMuDstMaker;
class  StEmcGeom;
class  StEmcDecoder;
class  StBemcTables;
class  StEEmcDb;      
class  StSpinDbMaker; 
class  EEmcGeomSimple;
class  EEmcSmdGeom;
class  WeventDisplay;
class  St2011pubWanaMaker;
class  St2011pubSpinMaker;
class  St2011pubMcMaker;

class  StJetReader;
class  StJets;
class  StJet;
class  TClonesArray;
class  TTree;

class St2011WMaker : public StMaker {
 friend class WeventDisplay;
 friend class St2011pubWanaMaker;
 friend class St2011pubSpinMaker;
 friend class St2011pubMcMaker;
 friend class St2011WlumiMaker;
 friend class St2011ZMaker;
 
 private:
  StMuDstMaker* mMuDstMaker;
  StJetReader* mJetReaderMaker;
  int nJets;
  TString mJetTreeBranch,mJetTreeBranch_noEEMC;
  TClonesArray* mJets;
  Wevent2011 *wEve;
  TTree *mWtree; TString mTreeName; TFile *mTreeFile;
  WeventDisplay *wDisaply;
  WtpcFilter mTpcFilter[mxTpcSec]; //allows sector dependent filter
  WtpcFilter mTpcFilterE[mxTpcSec]; //allows sector dependent filter for endcap tracks
  int  nInpEve,nTrigEve, nAccEve; //  event counters
  int  mRunNo, nRun;
  int  isMC; //0 for real data
  int Tfirst,Tlast;

  //... internal params
  int   par_l0emulAdcThresh;
  float par_l2emulSeedThresh, par_l2emulClusterThresh;

  int   par_l2bwTrgID;
  int   parE_l2ewTrgID;
  float par_vertexZ;
  int   par_minPileupVert;

  int   par_nFitPts,parE_nFitPts;
  float par_nHitFrac, par_trackRin,  par_trackRout, par_trackPt;
  float parE_nHitFrac, parE_trackRin,  parE_trackRout, parE_trackPt;
    
  int   par_kSigPed, par_AdcThres;
  float par_maxADC, par_clustET, parE_clustET;
  float par_clustFrac24, par_nearTotEtFrac;
  float parE_clustFrac24,parE_nearTotEtFrac;
  float par_nearDeltaR, par_awayDeltaPhi;
  float par_delR3D, parE_delR3D, par_highET, parE_highET,  par_ptBalance, parE_ptBalance;
  float par_leptonEta,parE_leptonEtaLow,parE_leptonEtaHigh; //bracket acceptance 
  float parE_trackEtaMin;
  int   parE_nSmdStrip;
        
  float par_etowScale;
  float par_btowScale;

  char* gains_file;
  int   use_gains_file;
  float gains_BTOW[4801];

 public: // to overwrite default params from .C macro
  void setVertexCuts(float zm, int npv) {
    par_vertexZ=zm; par_minPileupVert=npv; }
  void setEleTrackCuts(int nfp, int hfr, float rin, float rout, float mpt) {
    par_nFitPts=nfp;  par_nHitFrac=hfr; 
    par_trackRin=rin;  par_trackRout=rout; par_trackPt=mpt;}
  void setWbosonCuts(float a, float fr2,  float bal, float eta) {
    par_highET=a; par_nearTotEtFrac=fr2;  par_ptBalance=bal;  par_leptonEta=eta;}
  void setEmcCuts(int ksp , float madc, float clet, float fr1, float dr){
    par_kSigPed=ksp; par_maxADC=madc; par_clustET=clet; 
    par_clustFrac24=fr1;}
  void setEtowScale(float x){ par_etowScale=x; }
  void setBtowScale(float x){ par_btowScale=x; }
  void setL0AdcThresh(int x){ par_l0emulAdcThresh=x; }
  void setL2ClusterThresh(float x){ par_l2emulClusterThresh=x; }
  void setL2SeedThresh(float x){ par_l2emulSeedThresh=x; }
  void setJetTreeBranch(TString jetTreeBranch, TString jetTreeBranch_noEEMC){ mJetTreeBranch = jetTreeBranch; mJetTreeBranch_noEEMC = jetTreeBranch_noEEMC; }
  
  void setGainsFile(char* x) {gains_file=x; use_gains_file=1;}
  void setTreeName(TString x) { mTreeName=x; }
 private:   

  //.... not used in the algo
  int par_DsmThres,parE_DsmThres; 
  int par_maxDisplEve;

  StBemcTables* mBarrelTables; //used to access EMC status and ped info
  StEmcGeom  *mBtowGeom, * mBSmdGeom[mxBSmd];
  int mapBtowIJ2ID[mxBTetaBin*mxBTphiBin];// vs. (iEta, iPhi)
  TVector3 positionBtow[mxBtow]; // vs. tower ID
  TVector3 positionBsmd[mxBSmd][mxBStrips]; // vs. strip ID
 
  StEEmcDb        *mDbE; // access to EEMC database  
  StSpinDbMaker   *spinDb; // access spin information  
  EEmcGeomSimple  *geomE;// access to EEMC geometry 
  EEmcSmdGeom     *geoSmd;// access to ESMD geometry
  TVector3 positionEtow[mxEtowSec*mxEtowSub][mxEtowEta];  
  
  int   accessBarrelTrig();
  int   accessEndcapTrig();
  int   accessVertex();
  void  fillTowHit(bool vert);
  void  fillNorm();
  int   accessTracks();
  int   accessBTOW();
  void  accessBSMD();
  int   accessETOW();  
  void  accessEPRS();
  void  accessESMD();
  void  analyzeESMD();
  void  analyzeEPRS();

  bool  passes_L0();
  bool  passes_L2();
  void  find_W_boson();
  void  findEndcap_W_boson();
  void  tag_Z_boson();
  int   extendTrack2Barrel();
  int   matchTrack2BtowCluster();
  int   extendTrack2Endcap();
  int   matchTrack2EtowCluster();
  void  findNearJet();
  void  findAwayJet();
  void  findPtBalance();
  void  esmdAnalysis();
  
  // jets
  StJet* getJet(int i){return (StJet*)mJets->At(i);}
  TClonesArray* getJets(TString branchName);
  StJets* getStJetsCopy(TString branchName); 
  TClonesArray* getJetsTreeAnalysis(TString branchName);

  // tools
  float sumTpcCone( int vertID, TVector3 refAxis, int flag,int pointTowId);
  float sumBtowCone( float zVert,  TVector3 refAxis, int flag);
  float sumEtowCone(float zVert, TVector3 refAxis,int flag);
  float sumTpcConeFromTree( int vertID, TVector3 refAxis, int flag,int pointTowId); //uses track vector saved in tree
  WeveCluster maxBtow2x2(int iEta, int iPhi, float zVert);
  WeveCluster sumBtowPatch(int iEta, int iPhi, int Leta,int  Lphi,float zVert);
  WeveCluster maxEtow2x1(int iEta, int iPhi, float zVert);
  WeveCluster maxEtow2x2(int iEta, int iPhi, float zVert);
  WeveCluster sumEtowPatch(int iEta, int iPhi, int Leta,int  Lphi,float zVert);
  void patchToEtaPhi(int patch, int*eta, int*phi);
  

  // histograms
  TObjArray *HList; TObjArray *HListTpc;
  enum {mxHA=300}; TH1 * hA[mxHA];
  enum {mxHE=300}; TH1 * hE[mxHE];
  TH1 *hbxIdeal;    

  void initHistos(); void initEHistos();
  void initGeom();
  int L2algoEtaPhi2IJ(float etaF,float phiF,int &kEta, int &kPhi);
  
 public: 
  St2011WMaker(const char *name="2011Walgo");
  virtual  ~St2011WMaker(){};
  virtual Int_t Init();
  virtual Int_t Make();
  virtual Int_t Finish();

  virtual Int_t InitRun  (int runumber);
  virtual void Clear(const Option_t* = "");
  virtual Int_t FinishRun(int runumber);

  void setTrigID(  int l2bw, int l2ew) { par_l2bwTrgID=l2bw; parE_l2ewTrgID=l2ew; }

  void setHList(TObjArray * x){HList=x;}
  void setHListTpc(TObjArray * x){HListTpc=x;}
  void setMC(int x){isMC=x;}
  void setMaxDisplayEve(int n) { par_maxDisplEve=n;}
  void attachSpinDb(StSpinDbMaker *mk){ spinDb=mk;}
 
  //tree analysis
  void chainFile( const Char_t *name );
  void chainJetFile( const Char_t *name );
  Int_t getNumberOfEvents(){ return mTreeChain->GetEntries(); }
  Int_t getEvent(Int_t event, Int_t eventJet);

 protected:
  Int_t index,indexJet;
  TChain *mTreeChain;
  TChain *mJetTreeChain;

  /// Displayed on session exit, leave it as-is please ...
  virtual const char *GetCVS() const {
    static const char cvs[]="Tag $Name:  $ $Id: St2011WMaker.h,v 1.7 2012/07/13 20:53:16 stevens4 Exp $ built "__DATE__" "__TIME__ ; 
    return cvs;
  }

  ClassDef(St2011WMaker,0)   //StAF chain virtual base class for Makers
};

#endif


// $Log: St2011WMaker.h,v $
// Revision 1.7  2012/07/13 20:53:16  stevens4
// Add filling of empty events in W tree
// Minor modifications to histograms
//
// Revision 1.6  2012/07/13 16:11:44  balewski
// minor clenup, prevent crash in Finish if zero input events, now it runs on M-C events as well
//
// Revision 1.5  2012/07/12 20:49:21  balewski
// added spin info(star: bx48, bx7, spin4) and maxHtDSM & BTOW to Wtree
// removed dependence of spinSortingMaker from muDst
// Now Wtree can be spin-sorted w/o DB
// rdMu.C & readWtree.C macros modified
// tested so far on real data run 11
// lot of misc. code shuffling
//
// Revision 1.4  2012/06/25 20:53:17  stevens4
// algo and histo cleanup
//
// Revision 1.3  2012/06/18 18:28:00  stevens4
// Updates for Run 9+11+12 AL analysis
//
// Revision 1.2  2011/02/25 06:03:35  stevens4
// addes some histos and enabled running on MC
//
// Revision 1.1  2011/02/10 20:33:22  balewski
// start
//
