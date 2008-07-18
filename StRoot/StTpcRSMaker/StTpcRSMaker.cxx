/// \author Y.Fisyak, fisyak@bnl.gov
/// \date
// $Id: StTpcRSMaker.cxx,v 1.4 2008/07/18 16:21:16 fisyak Exp $
// doxygen info here
/*
 */
#include "StTpcRSMaker.h"
#include "Stiostream.h"
// SCL
#include "StGlobals.hh"
#include "StThreeVectorD.hh"
#include "StPhysicalHelixD.hh"
// ROOT
#include "TDataSetIter.h"
#include "TTableSorter.h"
#include "TRandom.h"
#include "TSystem.h"
#include "TFile.h"
#include "TBenchmark.h"
#include "TProfile2D.h"
// Chain
#include "StBFChain.h"
// Dave's Header file
#include "StDbUtilities/StTpcCoordinateTransform.hh"
#include "StDbUtilities/StCoordinates.hh" 
#include "StDbUtilities/StMagUtilities.h"
#include "StDaqLib/TPC/trans_table.hh"
#include "StDetectorDbMaker/St_TpcAltroParametersC.h"
#include "StDetectorDbMaker/St_asic_thresholdsC.h"
#include "StDetectorDbMaker/St_tss_tssparC.h"
#include "StTpcRawData.h"
#include "Altro.h"
#define PrPP(A,B) cout << "StTpcRSMaker::" << (#A) << "\t" << (#B) << " = \t" << (B) << endl;
static const char rcsid[] = "$Id: StTpcRSMaker.cxx,v 1.4 2008/07/18 16:21:16 fisyak Exp $";
static  Gccuts_t *ccuts = 0;

#define Laserino 170
#define Chasrino 171
static const Int_t NoOfSectors  =  24;
static const Int_t NoOfRows     =  45;
static const Int_t NoOfInnerRows=  13;
static const Int_t NoOfPads           = 182;
static const Int_t NoOfTimeBins = 512;
static const Double_t tauIntegraton        =  74.6e-9; // secs
static const Double_t tauF                 = 394.0e-9; 
static const Double_t tauP                 = 775.0e-9;
//                                    Inner        Outer
static       Double_t t0IO[2]   = {1.20868e-9, 1.43615e-9}; // recalculated in InducedCharge
static const Double_t tauC[2]   = {999.655e-9, 919.183e-9}; 
TF1*     StTpcRSMaker::fgTimeShape3[2]    = {0, 0};
TF1*     StTpcRSMaker::fgTimeShape0[2]    = {0, 0};
//________________________________________________________________________________
static const Int_t nx[2] = {200,250};
static const Double_t xmin[2] =  {-10., -6};
static const Double_t xmax[2] =  { 10., 19};
static const Int_t nz = 42;
static const Double_t zmin = -210;
static const Double_t zmax = -zmin;
struct Name_t {
  Char_t *Name;
  Char_t *Title;
};
static const Name_t InOut[4] = {
  {"Inner","Inner old electronics"},
  {"Outer","Outer old electronics"},
  {"InnerX","Inner new electronics"},
  {"OuterX","Outer new electronics"}
};
static const Name_t PadTime[2] = {
  {"Pad","Pad"},
  {"Time","Time"}
};
//                      io pt
static TProfile2D *hist[4][2];
//________________________________________________________________________________
ClassImp(StTpcRSMaker);
//________________________________________________________________________________
StTpcRSMaker::StTpcRSMaker(const char *name): 
  StMaker(name), fTree(0), mGeant(0), 
  m_SignalSum(0),
  mdNdx(0), mdNdE(0),
  mChargeFractionInner(0), mPadResponseFunctionInner(0), 
  mChargeFractionOuter(0), mPadResponseFunctionOuter(0), mPolya(0),
  mAlignSector(kTRUE), mMagUtilitiesMask(0), mPAI(0), mLaserScale(1), mClusterLength(0),
  mTau(0),   mTimeBinWidth(0),
  I0(13.1),
  mCluster(3.2), tauGlobalOffSet(0), 
  OmegaTauC(2.96), //OmegaTauC(3.58), 
  transverseDiffusionConstant(0.0640), //transverseDiffusionConstant(0.0623), 
  longitudinalDiffusionConstant(0.0370), 
  OmegaTau(OmegaTauC), 
  Inner_wire_to_plane_couplingScale(5.8985e-01*1.43), // comparision wtih data
  Outer_wire_to_plane_couplingScale(5.0718e-01*1.43), //  -"-
  FanoFactor(0.3),
  K3IP(0.68),    // K3 from E.Mathieson, Fig. 5.3b (pads) for a/s = 2.5e-3 and h/s = 0.5
  K3IR(0.89),    // K3 from E.Mathieson, Fig. 5.3a (row)  for a/s = 2.5e-3 and h/s = 0.5
  K3OP(0.55),    // K3 from E.Mathieson, Fig. 5.3b (pads) for a/s = 2.5e-3 and h/s = 1.0
  K3OR(0.61),    // K3 from E.Mathieson, Fig. 5.3a (row)  for a/s = 2.5e-3 and h/s = 1.0
  mAveragePedestal(1.24102e+02), 
  mAveragePedestalRMS(1.95684e+01),
  mPedestalRMS(0.493),
  minSignal(1e-3),
  LorenzAngle(8), // degrees for P10 at B = 4 kG and E = 1.5 kV/cm
  InnerAlphaVariation(0),
  OuterAlphaVariation(0),
  innerSectorAnodeVoltage(1170),
  outerSectorAnodeVoltage(1390),
  ElectronRange(0.0055), // Electron Range(.055mm)
  ElectronRangeEnergy(3000), // eV
  ElectronRangePower(1.78), // sigma =  ElectronRange*(eEnery/ElectronRangeEnergy)**ElectronRangePower
  CrossTalkInner    (4e-3), //0.15),
  CrossTalkOuter    (4e-3)  //0.025)
{
  memset (mShaperResponses, 0, sizeof(mShaperResponses));
  SETBIT(m_Mode,kPAI); 
  //  SETBIT(m_Mode,kGAIN); 
  SETBIT(m_Mode,kGAINOAtALL);
  //  SETBIT(m_Mode,kGAINO);  // disactivated at all
  //  SETBIT(m_Mode,kPedestal);
  SETBIT(m_Mode,kNONOISE);
  //  SETBIT(m_Mode,kdEdxCorr);
  //  SETBIT(m_Mode,kTree);
  TanLorenzAngle = TMath::Tan(LorenzAngle/180.*TMath::Pi())*5./4.; // for 5 kG
}
//________________________________________________________________________________
StTpcRSMaker::~StTpcRSMaker() {
  Finish();
}
//________________________________________________________________________________
Int_t StTpcRSMaker::Finish() {
  //  SafeDelete(fTree);
  free(m_SignalSum); m_SignalSum = 0;
  SafeDelete(mdNdx);
  SafeDelete(mdNdE);
  for (Int_t i = 0; i < 2; i++) 
    for (Int_t j = 0; j < 24; j++) {
      if (!TestBit(kNotDeleted)) {SafeDelete(mShaperResponses[i][j]);}
    }
  SafeDelete(mChargeFractionInner);
  SafeDelete(mPadResponseFunctionInner);
  SafeDelete(mChargeFractionOuter);
  SafeDelete(mPadResponseFunctionOuter);
  SafeDelete(mPolya);
  SafeDelete(m_TpcdEdxCorrection);
  SafeDelete(mPAI);
  return StMaker::Finish();
}
//________________________________________________________________________________
Int_t StTpcRSMaker::InitRun(Int_t runnumberOf) {
  if (!gStTpcDb) {
    cout << "Database Missing! Can't initialize TRS" << endl;
    return kStFatal;
  }
  if (! gMC) {
    cout << "TGiant3 has not been instantiated" << endl;
    return kStFatal;
  }
  mGeant = (TGiant3 *) gMC;
  ccuts  = (Gccuts_t *) mGeant->Gccuts();
  if (TESTBIT(m_Mode, kPAI)) {
    mPAI = PAI::Instance(); 
    gMessMgr->Warning() << "StTpcRSMaker:: use PAI model for dE/dx simulation" << endm;
  }
  else if (TESTBIT(m_Mode, kBICHSEL)) {
    gMessMgr->Warning() << "StTpcRSMaker:: use H.Bichsel model for dE/dx simulation" << endm;
    if (! mdNdE || ! mdNdx) {
      Char_t *path  = ".:./StarDb/dEdxModel:./StarDb/global/dEdx"
	":./StRoot/StBichsel:$STAR/StarDb/dEdxModel:$STAR/StarDb/global/dEdx:$STAR/StRoot/StBichsel";
      Char_t *Files[2] = {"dNdE_Bichsel.root","dNdx_Bichsel.root"};
      for (Int_t i = 0; i < 2; i++) {
	Char_t *file = gSystem->Which(path,Files[i],kReadPermission);
	if (! file) Fatal("StTpcRSMaker::Init","File %s has not been found in path %s",Files[i],path);
	else        Warning("StTpcRSMaker::Init","File %s has been found as %s",Files[i],file);
	TFile       *pFile = new TFile(file);
	if (i == 0) {mdNdE = (TH1D *) pFile->Get("dNdEI"); assert(mdNdE);   mdNdE->SetDirectory(0);}
	if (i == 1) {mdNdx = (TH1D *) pFile->Get("dNdx"); assert(mdNdx);   mdNdx->SetDirectory(0);}
	delete pFile;
      }
    }
  }
  else gMessMgr->Warning() << "StTpcRSMaker:: use GEANT321 model for dE/dx simulation" << endm;
  // Distortions
  StBFChain *chain = dynamic_cast<StBFChain*>(GetChain());
  if (! chain) {
    gMessMgr->Error() << "Cannot run without StBFChain" << endl;
    return kStFatal;
  }
  if (!chain->GetOption("AlignSectors")) {
    gMessMgr->Warning() << "StTpcRSMaker:: use AlignSectors" << endm;
    AlignHits(kFALSE);
  }
  if (TESTBIT(m_Mode,kdEdxCorr)) {
    gMessMgr->Warning() << "StTpcRSMaker:: use Tpc dE/dx correction from calibaration" << endm;
    Int_t Mask = -1;
#if 0
    SETBIT(Mask,StTpcdEdxCorrection::ktpcPressure); 
    //  SETBIT(Mask,StTpcdEdxCorrection::kAdcCorrection); 
    SETBIT(Mask,StTpcdEdxCorrection::kTpcSecRow); 
    SETBIT(Mask,StTpcdEdxCorrection::kDrift);
    SETBIT(Mask,StTpcdEdxCorrection::kzCorrection);
    SETBIT(Mask,StTpcdEdxCorrection::kdXCorrection);
    //  SETBIT(Mask,StTpcdEdxCorrection::kTpcdEdxCor);
    //  SETBIT(Mask,StTpcdEdxCorrection::kTpcLengthCorrection);
#endif
    m_TpcdEdxCorrection = new StTpcdEdxCorrection(Mask);
  }
  if (Debug() && gStTpcDb->PadResponse()) gStTpcDb->PadResponse()->Table()->Print(0,1);
  longitudinalDiffusionConstant = gStTpcDb->PadResponse()->longitudinalDiffusionConstant();
  //  transverseDiffusionConstant   = gStTpcDb->PadResponse()->transverseDiffusionConstant();
  //  firstOuterSectorAnodeWire  = gStTpcDb->WirePlaneGeometry()->firstOuterSectorAnodeWire();
  innerSectorzOffSet = gStTpcDb->Dimensions()->zInnerOffset(); 
  samplingFrequency = 1.e6*gStTpcDb->Electronics()->samplingFrequency(); // Hz
  mTimeBinWidth     = 1./samplingFrequency;
  //  static Double_t Gain;
  //  Gain             =  gStTpcDb->Electronics()->nominalGain();
  outerSectorzOffSet = gStTpcDb->Dimensions()->zOuterOffset();
  numberOfInnerSectorAnodeWires  = gStTpcDb->WirePlaneGeometry()->numberOfInnerSectorAnodeWires ();
  firstInnerSectorAnodeWire = gStTpcDb->WirePlaneGeometry()->firstInnerSectorAnodeWire();
  lastInnerSectorAnodeWire  = gStTpcDb->WirePlaneGeometry()->lastInnerSectorAnodeWire ();
  numberOfOuterSectorAnodeWires  = gStTpcDb->WirePlaneGeometry()->numberOfOuterSectorAnodeWires ();
  firstOuterSectorAnodeWire = gStTpcDb->WirePlaneGeometry()->firstOuterSectorAnodeWire();
  lastOuterSectorAnodeWire  = gStTpcDb->WirePlaneGeometry()->lastOuterSectorAnodeWire ();
  anodeWirePitch            = gStTpcDb->WirePlaneGeometry()->anodeWirePitch           ();
  anodeWireRadius           = gStTpcDb->WirePlaneGeometry()->anodeWireRadius(); 
  Float_t BFieldG[3]; 
  Float_t xyz[3] = {0,0,0};
  StMagF::Agufld(xyz,BFieldG);
  //  OmegaTau = 2.34*BFieldG[2]/5.0; // signed !
  OmegaTau = OmegaTauC*BFieldG[2]/5.0;// from diffusion 586 um / 106 um at B = 0/ 5kG
  // Shapers
  Double_t timeBinMin = -0.5;
  Double_t timeBinMax = 24.5;
  const Char_t *Names[2] = {"I","O"};
  for (Int_t io = 0; io < 2; io++) {// In/Out
    if (io == 0) {
      cout << "Inner Sector ======================" << endl;
      InnerAlphaVariation = InducedCharge(anodeWirePitch,
					  gStTpcDb->PadPlaneGeometry()->innerSectorPadWidth(),
					  anodeWireRadius,
					  innerSectorAnodeVoltage, t0IO[io]);
    }
    else {
      cout << "Outer Sector ======================" << endl;
      OuterAlphaVariation = InducedCharge(anodeWirePitch,
					  gStTpcDb->PadPlaneGeometry()->outerSectorPadWidth(),
					  anodeWireRadius,
					  outerSectorAnodeVoltage, t0IO[io]);
    }
    Double_t params3[7] = {t0IO[io], tauF, tauP, tauIntegraton, mTimeBinWidth, 0, io};
    Double_t params0[5] = {t0IO[io],             tauIntegraton, mTimeBinWidth, 0, io};
    if (! fgTimeShape3[io]) {// old electronics, intergation + shaper alltogether
      fgTimeShape3[io] = new TF1(Form("TimeShape3%s",Names[io]),
				 shapeEI3,timeBinMin*mTimeBinWidth,timeBinMax*mTimeBinWidth,7);
      fgTimeShape3[io]->SetParNames("t0","tauF","tauP", "tauI");
      fgTimeShape3[io]->SetParameters(params3);
      params3[5] = fgTimeShape3[io]->Integral(timeBinMin*mTimeBinWidth,timeBinMax*mTimeBinWidth);
    }
    if (! fgTimeShape0[io]) {// new electronics only integration
      fgTimeShape0[io] = new TF1(Form("TimeShape%s",Names[io]),
			    shapeEI,timeBinMin*mTimeBinWidth,timeBinMax*mTimeBinWidth,5);
      fgTimeShape0[io]->SetParNames("t0","tauI");
      fgTimeShape0[io]->SetParameters(params0);
      params0[3] = fgTimeShape0[io]->Integral(0,timeBinMax*mTimeBinWidth);
    }
    
    for (Int_t sector = 1; sector <= 24; sector++) {
      if (St_TpcAltroParametersC::instance()->N(sector-1) < 0) {// old TPC
	// check if the function has been created
	for (Int_t sec = 1; sec < sector; sec++) {
	  if (St_TpcAltroParametersC::instance()->N(sec-1) < 0 &&
	      ! mShaperResponses[io][sec-1]) {
	    mShaperResponses[io][sector-1] = mShaperResponses[io][sec-1];
	    break;
	  }
	}
	if (! mShaperResponses[io][sector-1]) {
	  mShaperResponses[io][sector-1] = new TF1(Form("ShaperFunc_%s_S%02i",Names[io],sector),
						    StTpcRSMaker::shapeEI3_I,timeBinMin,timeBinMax,7);  
	  mShaperResponses[io][sector-1]->SetParameters(params3);
	  mShaperResponses[io][sector-1]->SetParNames("t0","tauF","tauP", "tauI", "width","norm","io");
	  mShaperResponses[io][sector-1]->SetNpx((Int_t) (10*(timeBinMax-timeBinMin)));
	  mShaperResponses[io][sector-1]->Save(timeBinMin,timeBinMax,0,0,0,0);
	}
	continue;
      }
      //Altro
      for (Int_t sec = 1; sec < sector; sec++) {
	if (St_TpcAltroParametersC::instance()->N(sec-1) >= 0 &&
	    ! mShaperResponses[io][sec-1] ) {
	  mShaperResponses[io][sector-1] = mShaperResponses[io][sec-1];
	  break;
	}
      }
      if (! mShaperResponses[io][sector-1]) {
	mShaperResponses[io][sector-1] = new TF1(Form("ShaperFunc_%s_S%02i",Names[io],sector),
						  StTpcRSMaker::shapeEI_I,timeBinMin,timeBinMax,5);  
	mShaperResponses[io][sector-1]->SetParameters(params0);
	mShaperResponses[io][sector-1]->SetParNames("t0","tauI", "width","norm","io");
	mShaperResponses[io][sector-1]->SetNpx((Int_t) (10*(timeBinMax-timeBinMin)));
	mShaperResponses[io][sector-1]->Save(timeBinMin,timeBinMax,0,0,0,0);
      }
    }
  }
  //                             w       h         s      a       l   i
  //  Double_t paramsI[6] = {0.2850, 0.2000,  0.4000, 0.0010, 1.1500, 0};
  //  Double_t paramsO[6] = {0.6200, 0.4000,  0.4000, 0.0010, 1.1500, 0};
  if (! mPadResponseFunctionInner ||
      ! mPadResponseFunctionOuter ||
      ! mChargeFractionInner ||
      ! mChargeFractionOuter) {
    Double_t params[5];
    params[0] = gStTpcDb->PadPlaneGeometry()->innerSectorPadWidth();                     // w = width of pad       
    params[1] = gStTpcDb->WirePlaneGeometry()->innerSectorAnodeWirePadPlaneSeparation(); // h = Anode-Cathode gap   
    params[2] = gStTpcDb->WirePlaneGeometry()->anodeWirePitch();                         // s = wire spacing       
    //    params[3] = anodeWireRadius;                                                         // a = Anode wire radius  
    params[4] = 0;
    params[5] = 0;
    Double_t xminP = 0; 
    Double_t xmaxP = 2.75;//4.5*gStTpcDb->PadPlaneGeometry()->innerSectorPadWidth();// 4.5 
    if (! mPadResponseFunctionInner) {
      mPadResponseFunctionInner = new TF1("PadResponseFunctionInner",
					  StTpcRSMaker::PadResponseFunc,xminP,xmaxP,6); 
      params[3] =  K3IP;
      params[4] = CrossTalkInner;
      params[5] = gStTpcDb->PadPlaneGeometry()->innerSectorPadPitch();
      mPadResponseFunctionInner->SetParameters(params);
      mPadResponseFunctionInner->Save(xminP,xmaxP,0,0,0,0);
    }
    if (! mChargeFractionInner) {
      xmaxP = 1.5;//5*gStTpcDb->PadPlaneGeometry()->innerSectorPadLength(); // 1.42
      mChargeFractionInner = new TF1("ChargeFractionInner",
				     StTpcRSMaker::PadResponseFunc,xminP,xmaxP,6);
      params[0] = gStTpcDb->PadPlaneGeometry()->innerSectorPadLength();
      params[3] = K3IR;
      params[4] = 0;
      params[5] = 1.; 
      mChargeFractionInner->SetParameters(params);
      mChargeFractionInner->Save(xminP,xmaxP,0,0,0,0);
    }
    if (! mPadResponseFunctionOuter) {
      xmaxP = 2.75;//5.*gStTpcDb->PadPlaneGeometry()->outerSectorPadWidth(); // 3.
      mPadResponseFunctionOuter = new TF1("PadResponseFunctionOuter",
					  StTpcRSMaker::PadResponseFunc,xminP,xmaxP,6); 
      params[0] = gStTpcDb->PadPlaneGeometry()->outerSectorPadWidth();                    // w = width of pad       
      params[1] = gStTpcDb->WirePlaneGeometry()->outerSectorAnodeWirePadPlaneSeparation(); // h = Anode-Cathode gap   
      params[2] = gStTpcDb->WirePlaneGeometry()->anodeWirePitch();                         // s = wire spacing       
      //      params[3] = gStTpcDb->WirePlaneGeometry()->anodeWireRadius();                        // a = Anode wire radius  
      params[3] = K3OP;
      params[4] = CrossTalkOuter;
      params[5] = gStTpcDb->PadPlaneGeometry()->outerSectorPadPitch();
      mPadResponseFunctionOuter->SetParameters(params);
      mPadResponseFunctionOuter->Save(xminP,xmaxP,0,0,0,0);
    }
    if (! mChargeFractionOuter) {
      xmaxP = 2.5;//5*gStTpcDb->PadPlaneGeometry()->outerSectorPadLength(); // 1.26
      mChargeFractionOuter = new TF1("ChargeFractionOuter",
				     StTpcRSMaker::PadResponseFunc,xminP,xmaxP,6);
      params[0] = gStTpcDb->PadPlaneGeometry()->outerSectorPadLength();
      params[3] = K3OR; 
      params[4] = 0;
      params[5] = 1.; 
      mChargeFractionOuter->SetParameters(params);
      mChargeFractionOuter->Save(xminP,xmaxP,0,0,0,0);
    }
  }
  //  TF1 *func = new TF1("funcP","x*sqrt(x)/exp(2.5*x)",0,10);
  // see http://www4.rcf.bnl.gov/~lebedev/tec/polya.html
  // Gain fluctuation in proportional counters follows Polya distribution. 
  // x = G/G_0
  // P(m) = m(m(x)**(m-1)*exp(-m*x)/Gamma(m);
  // original Polya  m = 1.5 (R.Bellazzini and M.A.Spezziga, INFN PI/AE-94/02). 
  // Valeri Cherniatin (cherniat@bnlarm.bnl.gov) recomends m=1.38
  // Trs uses x**1.5/exp(x)
  // tss used x**0.5/exp(1.5*x)
  //  mPolya = new TF1("Polya","sqrt(x)/exp(1.5*x)",0,10); // original Polya 
  mPolya = new TF1("Polya","pow(x,0.38)*exp(-1.38*x)",0,10); //  Valeri Cherniatin
  //   mPoly = new TH1D("Poly","polyaAvalanche",100,0,10);
  //   mPoly->FillRandom("funcP",100000);
  //   delete func;
  // tss
  
  W = St_tss_tssparC::instance()->ave_ion_pot()*GeV; // eV
  InnerSectorGasGain = St_tss_tssparC::instance()->gain_in();
  OuterSectorGasGain = St_tss_tssparC::instance()->gain_out();
  Inner_wire_to_plane_coupling = St_tss_tssparC::instance()->wire_coupling_in() ;//0.90914;//0.81;//avr for 2 highest points// 0.774; // average for 3 points
  Outer_wire_to_plane_coupling = St_tss_tssparC::instance()->wire_coupling_out();//1.01459;//1.02;//                        // 0.972;
  numberOfElectronsPerADCcount = St_tss_tssparC::instance()->scale();
  
  if (Debug()) Print();
  memset (hist, 0, sizeof(hist));
  if (GetTFile()) {
    GetTFile()->cd();
    Int_t color = 1;
    for (Int_t io = 0; io < 4; io++) {
      for (Int_t pt = 0; pt < 2; pt++) {
	TString Name(InOut[io].Name); Name += PadTime[pt].Name; Name += "Mc";
	TString Title(InOut[io].Title); Title += PadTime[pt].Title; Title += "Mc";
	hist[io][pt] = (TProfile2D *) gDirectory->Get(Name);
	if (! hist[io][pt]) {
	  hist[io][pt] = new TProfile2D(Name,Title,nx[pt],xmin[pt],xmax[pt],nz,zmin,zmax,""); 
	  hist[io][pt]->SetMarkerStyle(20);
	  hist[io][pt]->SetMarkerColor(color++);
	}
      }
    }
  }
  return kStOK;
}
//________________________________________________________________________________
Int_t StTpcRSMaker::Make(){  //  PrintInfo();
  // constants
  static StTpcCoordinateTransform transform(gStTpcDb);
  Int_t Ndebug = 0, Idebug; // debug printout depth
  if (Debug()%10) {
    if (Debug()%10 > 1) Ndebug = 10;
    gBenchmark->Reset();
    gBenchmark->Start("TrsMinMake");
    cout << "\n -- Begin TrsMini Processing -- \n";
  }
  TDataSetIter geant(GetDataSet("geant"));
  St_g2t_tpc_hit *g2t_tpc_hit = (St_g2t_tpc_hit *) geant("g2t_tpc_hit");
  if (!g2t_tpc_hit) return kStWarn;
  Int_t no_tpc_hits       = g2t_tpc_hit->GetNRows();               if (no_tpc_hits<1) return kStOK;
  if (Debug() > 1) g2t_tpc_hit->Print(0,10);
  St_g2t_track *g2t_track = (St_g2t_track *) geant("g2t_track");   if (!g2t_track)    return kStWarn;
  g2t_track_st *tpc_track = g2t_track->GetTable();
  St_g2t_vertex  *g2t_ver = (St_g2t_vertex *) geant("g2t_vertex"); if (!g2t_ver)      return kStWarn;
  g2t_vertex_st     *gver = g2t_ver->GetTable();
  g2t_tpc_hit_st *tpc_hit_begin = g2t_tpc_hit->GetTable();
  g2t_tpc_hit_st *tpc_hit = tpc_hit_begin;
  // sort 
  TTableSorter sorter(g2t_tpc_hit,&SearchT,&CompareT);//, 0, no_tpc_hits);
  //Float_t amass, charge, tlife; 
  Int_t sortedIndex = 0;
  tpc_hit = tpc_hit_begin;
  for (Int_t sector = 1; sector <= NoOfSectors; sector++) {
    //    offset=transform.zFromTB(0);
    Double_t driftVelocity = gStTpcDb->DriftVelocity(sector);
    //    tauGlobalOffSet = (3*(mTau - 55e-9) + 22.7e-9)*driftVelocity; // offset due to new mTau
    Idebug = 0;
    Int_t NoHitsInTheSector = 0;
    SignalSum_t *SignalSum = ResetSignalSum();
    //    for (; i<=no_tpc_hits; i++,tpc_hit++){// it is assumed that hit are oreder by sector
    while (sortedIndex < no_tpc_hits) {
      Int_t idnx = sorter.GetIndex(sortedIndex);
      if (idnx < 0) break;
      tpc_hit = tpc_hit_begin + idnx;
      Int_t volId = tpc_hit->volume_id%100000;
      Int_t iSector = volId/100;
      if (iSector != sector) {
	if (! ( iSector > sector ) )
	  cout << "StTrsMinMaker::Make: g2t_tpc_hit table has not been ordered by sector no. !" << endl;
	assert( iSector > sector );
	break;
      }
 #if 0
      while (tpc_track[Id-1].next_parent_p && ipart == 3) { // delta electrons ?
	Id = tpc_track[Id-1].next_parent_p;
	ipart      = tpc_track[Id-1].ge_pid;
      }
#endif
      for (Int_t s_index = 0; s_index < 1; s_index++, sortedIndex++) {
	Int_t isDet  = tpc_hit->volume_id/100000;
	if (isDet) continue; // skip pseudo padrow
	Int_t iPadrow = volId%100;
	Int_t Id         = tpc_hit->track_p;
	Int_t TrackId    = Id;
	Int_t id3        = tpc_track[Id-1].start_vertex_p;
	Int_t ipart      = tpc_track[Id-1].ge_pid;
	Float_t charge = tpc_track[Id-1].charge;
	if (ipart == Laserino || ipart == Chasrino) charge = 0;
	Int_t io = 0;
	if (iPadrow > 13) io = 1;
	Int_t ioH = io;
	if (St_TpcAltroParametersC::instance()->N(sector-1) >= 0) ioH += 2;
	Double_t TotalSignal  = 0;
	Double_t tof = gver[id3-1].ge_tof;
	if (! TESTBIT(m_Mode, kNoToflight)) tof += tpc_hit->tof;
	Double_t zToFoffset = tof*driftVelocity;
	Double_t lgam = tpc_hit->lgam;
	Double_t gamma = TMath::Power(10.,lgam) + 1;
	Double_t betaGamma = TMath::Sqrt(gamma*gamma - 1.);
	// move up, calculate field at center of TPC
	static Float_t BFieldG[3]; 
	StMagF::Agufld(tpc_hit->x,BFieldG);
	// distortion and misalignment 
	StGlobalCoordinate   xyzG(tpc_hit->x[0],tpc_hit->x[1],tpc_hit->x[2]);
	// replace pxy => direction and try linear extrapolation
	StThreeVectorD       pxyzG(tpc_hit->p[0],tpc_hit->p[1],tpc_hit->p[2]);

	StGlobalDirection    dirG(pxyzG.unit());
	StGlobalDirection    BG(BFieldG[0],BFieldG[1],BFieldG[2]);
	static StTpcLocalCoordinate xyzL;
	transform(xyzG,xyzL,sector,iPadrow);
	static StTpcLocalDirection  dirL;
	transform(dirG,dirL,sector,iPadrow);
	static StTpcLocalDirection  BL;
	transform(  BG,  BL,sector,iPadrow);
	// Generate signal 
	Double_t padlength = gStTpcDb->PadPlaneGeometry()->innerSectorPadLength();
	Double_t wire_to_plane_coupling = Inner_wire_to_plane_coupling*Inner_wire_to_plane_couplingScale;
	Double_t Gain = InnerSectorGasGain;
	Int_t rowMin = iPadrow;
	Int_t rowMax = iPadrow;
	TF1 *PadResponseFunction = mPadResponseFunctionInner;
	TF1 *ChargeFraction      = mChargeFractionInner;
	Double_t PadPitch        = gStTpcDb->PadPlaneGeometry()->innerSectorPadPitch();
	if(iPadrow > NoOfInnerRows) { // Outer
	  rowMin                 = max(iPadrow - 1, NoOfInnerRows);
	  rowMax                 = min(iPadrow + 1, NoOfRows);
	  PadResponseFunction    = mPadResponseFunctionOuter;
	  ChargeFraction         = mChargeFractionOuter;
	  wire_to_plane_coupling = Outer_wire_to_plane_coupling*Outer_wire_to_plane_couplingScale;
	  Gain                   = OuterSectorGasGain;
	  PadPitch               = gStTpcDb->PadPlaneGeometry()->outerSectorPadPitch();
	  padlength              = gStTpcDb->PadPlaneGeometry()->outerSectorPadLength();
	}
#if 0
	// Distortions 
	if (StMagUtilities::Instance()) {
	  Float_t pos[3] = {xyzL.position().x(), xyzL.position().y(), xyzL.position().z()};
	  Float_t posMoved[3];
	  StMagUtilities::Instance()->DoDistortion(pos,posMoved);   // input pos[], returns posMoved[]
	  StThreeVector<double> postion(posMoved[0],posMoved[1],posMoved[2]);
	  xyzL.setPosition(postion);
	}
#endif
	static StTpcLocalSectorAlignedCoordinate xyzA;
	transform(xyzL,xyzA);
	static StTpcLocalSectorCoordinate xyzLocalSector;
	transform(xyzA,xyzLocalSector);
	static StTpcPadCoordinate Pad;
	transform(xyzLocalSector,Pad,kFALSE,kFALSE);
	Double_t padH = Pad.pad();        Int_t pad0 = TMath::Nint(padH + xmin[0]);
	Double_t tbkH = Pad.timeBucket(); Int_t tbk0 = TMath::Nint(tbkH + xmin[1]);
	static StTpcLocalSectorAlignedDirection  dirA;
	transform(dirL,dirA);
	static StTpcLocalSectorDirection dirLocalSector;
	transform(dirA,dirLocalSector);
	static StTpcLocalSectorAlignedDirection  BA;
	transform(BL,BA);
	static StTpcLocalSectorDirection BLocalSector;
	transform(BA,BLocalSector);
	Float_t BField[3] = {BLocalSector.position().x(), BLocalSector.position().y(), BLocalSector.position().z()};
	//	OmegaTau = 2.34*BField[2]/5.0; // signed !
	OmegaTau = OmegaTauC*BField[2]/5.0;// from diffusion 586 um / 106 um at B = 0/ 5kG
	Double_t tangLorenzAngle = TanLorenzAngle*BField[2]/5.;
	StPhysicalHelixD track(dirLocalSector.position(),
			       xyzLocalSector.position(),
			       BField[2]*kilogauss,charge);  if (Debug() % 10 > 1) PrPP(Make,track);
	Double_t NP = TMath::Abs(tpc_hit->de/tpc_hit->ds)/(W*mCluster); // from GEANT
	Double_t dStep =  tpc_hit->ds;
	Double_t s_low = - TMath::Abs(dStep)/2;
	Double_t s_upper = - s_low;
	Double_t dSD    = s_upper - s_low;
	Double_t driftLength = xyzLocalSector.position().z(); 
	if (driftLength <= 0) continue; 
	Double_t D = 1. + OmegaTau*OmegaTau;
	Double_t SigmaT = transverseDiffusionConstant*TMath::Sqrt(driftLength/D);
	Double_t SigmaL = longitudinalDiffusionConstant*TMath::Sqrt(driftLength);
	if (Debug()%10 > 1) { 	
	  cout << "--> tpc_hit: " << sortedIndex << "\t"
	       << tpc_hit->volume_id   << "\t"
	       << tpc_hit->de          << "\t"
	       << tpc_hit->ds          << "\t" << endl;
	  PrPP(Make,xyzG); PrPP(Make,xyzL); PrPP(Make,xyzA); PrPP(Make,xyzLocalSector);
	  PrPP(Make,dirG); PrPP(Make,dirL); PrPP(Make,dirA); PrPP(Make,dirLocalSector);
	  PrPP(Make,BG); PrPP(Make,BL); PrPP(Make,BA); PrPP(Make,BLocalSector);
	  PrPP(Make,Pad);
	  PrPP(Make,dirLocalSector); PrPP(Make,dirLocalSector.position()); PrPP(Make,betaGamma);
	  cout << "s_low/s_upper/dSD\t" << s_low << "/\t" << s_upper << "/\t" << dSD << endl;
	}
	// dE/dx correction
	Double_t dEdxCor = 1;
	if (m_TpcdEdxCorrection) {
	  dEdxY2_t CdEdx;
	  memset (&CdEdx, 0, sizeof(dEdxY2_t));
	  CdEdx.sector = Pad.sector(); 
	  CdEdx.row    = Pad.row();
	  CdEdx.pad    = TMath::Nint(Pad.pad());
	  CdEdx.dE     = 1;
	  CdEdx.dx     = dSD;
	  CdEdx.xyz[0] = xyzG.position().x();
	  CdEdx.xyz[1] = xyzG.position().y();
	  CdEdx.xyz[2] = xyzG.position().z();
	  CdEdx.ZdriftDistance = driftLength;
	  St_tpcGas *tpcGas = m_TpcdEdxCorrection->tpcGas();
	  if (tpcGas)
	    CdEdx.ZdriftDistanceO2 = CdEdx.ZdriftDistance*(*tpcGas)[0].ppmOxygenIn;
	  if (m_TpcdEdxCorrection->dEdxCorrection(CdEdx)) continue; 
	  dEdxCor = CdEdx.dE;
	  if (dEdxCor <= 0.) continue;
	}
	Double_t GainLocal = Gain/dEdxCor*wire_to_plane_coupling/numberOfElectronsPerADCcount; // Account dE/dx calibration
	// end of dE/dx correction
	// generate electrons: No. of primary clusters per cm
	if (TESTBIT(m_Mode, kBICHSEL)) 
	  NP = GetNoPrimaryClusters(betaGamma); // per cm
	Int_t nP = 0; 
	Int_t WireIndex;
	Double_t xOnWire, yOnWire, zOnWire;
	Double_t newPosition = s_low;
	Double_t dESum = 0;
	Double_t dSSum = 0;
	Double_t padsdE[32]; memset (padsdE, 0, sizeof(padsdE));
	Double_t tbksdE[64]; memset (tbksdE,  0, sizeof(tbksdE));
	do {// Clusters
	  Float_t dS = 0;
	  Float_t dE = 0;
	  if (charge) {
	    if (TESTBIT(m_Mode, kPAI)) {
	      mPAI->xNext(betaGamma,dS,dE);
	    }
	    else {
	      dS = - TMath::Log(gRandom->Rndm())/NP;
	      if (TESTBIT(m_Mode, kBICHSEL)) dE = mdNdE->GetRandom();
	      else                           dE = W/eV*gRandom->Poisson(mCluster);
	    }
	  }
	  else { // charge == 0 geantino
	    // for Laserino assume dE/dx = 25 keV/cm;
	    dE = 10; // eV
	    dS = dE*eV/(TMath::Abs(mLaserScale*tpc_hit->de/tpc_hit->ds));
	  }	  
	  dESum += dE;
	  dSSum += dS;
	  Double_t sigmaR = ElectronRange*TMath::Power(dE/ElectronRangeEnergy,ElectronRangePower);
	  Double_t xRange = gRandom->Gaus(0,sigmaR);
	  Double_t E = dE*eV;
	  if (E > ccuts->cutele) continue; // account delta-electrons
	  nP++;
	  newPosition += dS;
	  if (newPosition > s_upper) break;
	  StThreeVectorD xyzC = track.at(newPosition);
#if 0
	  StThreeVectorD xyzCP = xyzLocalSector.position() + newPosition*dirLocalSector.position();
	  StThreeVectorD diff   = xyzC - xyzCP;
	  if (diff.mag() > 1e-3) {
	    cout << "helix approximation: " << xyzC << endl;
	    cout << "straight line appro: " << xyzCP << " are different" << endl;
	  }
	  //	if (Debug()) {PrPP(Make,xyzC);}
#endif
	  Int_t n0 = TMath::Nint((E - I0*eV)/W/(1. - FanoFactor));
	  Int_t Nt = gRandom->Binomial(n0, 1. - FanoFactor) + 1;
	  Double_t sigmaT = SigmaT;
	  Double_t sigmaL = SigmaL;
	  Double_t QAv = GainLocal*mPolya->GetRandom();
	  if (mClusterLength > 0) {// propagate cluster as whole
	    Double_t dNt = Nt;
	    sigmaT = SigmaT/TMath::Sqrt(dNt);
	    sigmaL = SigmaL/TMath::Sqrt(dNt);
	    for (Int_t i = 0; i < Nt - 1; i++) QAv += GainLocal*mPolya->GetRandom();
	    Nt = 1;
	  }
	  for (Int_t ie = 0; ie < Nt; ie++) {
	    if (ie)  QAv = GainLocal*mPolya->GetRandom();
	    // transport to wire
	    StTpcLocalSectorCoordinate xyzE(xyzC.x()+xRange+gRandom->Gaus(0,sigmaT),
					    xyzC.y()+gRandom->Gaus(0,sigmaT),
					    xyzC.z()+gRandom->Gaus(0,sigmaL), sector, iPadrow);
	    Double_t y = xyzE.position().y();
	    Double_t zoffset = innerSectorzOffSet;
	    Double_t alphaVariation = InnerAlphaVariation;
	    Double_t CrossTalk = CrossTalkInner;
	    // Transport to wire
	    if (y < firstInnerSectorAnodeWire || y >  lastOuterSectorAnodeWire) continue;
	    if (y >  lastInnerSectorAnodeWire && y < firstOuterSectorAnodeWire) continue;
	    if (y < lastInnerSectorAnodeWire) {
	      WireIndex = (int) ((y - firstInnerSectorAnodeWire)/anodeWirePitch + 0.5);
	      yOnWire = firstInnerSectorAnodeWire + WireIndex*anodeWirePitch;
	    }
	    else {
	      WireIndex = (int) ((y - firstOuterSectorAnodeWire)/anodeWirePitch + 0.5);
	      yOnWire = firstOuterSectorAnodeWire + WireIndex*anodeWirePitch;
	      WireIndex += (int) ((lastInnerSectorAnodeWire - firstInnerSectorAnodeWire)/anodeWirePitch) + 1;
	      zoffset = outerSectorzOffSet;
	      alphaVariation = OuterAlphaVariation;
	      CrossTalk = CrossTalkOuter;
	    }
	    Double_t distanceToWire = y - yOnWire; // Calculated effective distance to wire affected by Lorentz shift 
	    xOnWire = xyzE.position().x();
	    zOnWire = xyzE.position().z() + zToFoffset + zoffset; 
	    // Grid plane (1 mm spacing) focusing effect + Lorentz angle in drift volume 
	    Int_t iGridWire = (Int_t ) TMath::Abs(10.*distanceToWire);
	    Double_t dist2Grid = TMath::Sign(0.05 + 0.1*iGridWire, distanceToWire);
	    Double_t yShift   = dist2Grid - distanceToWire;
	    xOnWire -= yShift*OmegaTau;
	    zOnWire += TMath::Abs(yShift);
	    // Ground plane (1 mm spacing) focusing effect
	    Int_t iGroundWire = (Int_t ) TMath::Abs(10.*dist2Grid);
	    Double_t distFocused = TMath::Sign(0.05 + 0.1*iGroundWire, dist2Grid);
	    xOnWire += distFocused*tangLorenzAngle; // Lorentz shift
	    zOnWire += TMath::Abs(distFocused);
	    if (! iGroundWire ) QAv *= TMath::Exp( alphaVariation);
	    else                QAv *= TMath::Exp(-alphaVariation);
	    Float_t bin = transform.tBFromZ(zOnWire,sector);
	    Int_t binT = (Int_t) bin;
	    if (binT < 0 || binT >= NoOfTimeBins) continue;
	    Double_t dT = bin -  binT;
	    for(Int_t row = rowMin; row <= rowMax; row++) {              
	      Double_t dely           = TMath::Abs(transform.yFromRow(row)-yOnWire);            
	      Double_t localYDirectionCoupling = ChargeFraction->GetSave(&dely);
	      if(localYDirectionCoupling < minSignal) continue;
	      Float_t padX = transform.padFromX(xOnWire,row);
	      Int_t CentralPad = TMath::Nint(padX);
	      if (CentralPad < 1) continue;
	      Int_t PadsAtRow = gStTpcDb->PadPlaneGeometry()->numberOfPadsAtRow(row);
	      if(CentralPad > PadsAtRow) continue;
	      Int_t DeltaPad = TMath::Nint(PadResponseFunction->GetXmax() + 1);
	      Int_t padMin = TMath::Max(CentralPad - DeltaPad ,1);
	      Int_t padMax = TMath::Min(CentralPad + DeltaPad ,PadsAtRow);
	      Double_t xPad = padMin - padX;
	      TF1 *mShaperResponse = mShaperResponses[io][sector-1];
	      for(Int_t pad = padMin; pad <= padMax; pad++, xPad++) {
		Double_t dt = dT;
#if 0
		if (! TESTBIT(m_Mode, kGAINOAtALL)) { 
		  Double_t gain   = gStTpcDb->tpcGain()->Gain(sector,row,pad);
		  if (gain <= 0.0) continue;
		  dt -= gStTpcDb->T0(sector)->getT0(row,pad);
		}
#endif
		Double_t xpad[1] = {TMath::Abs(xPad)};
		Double_t localXDirectionCoupling = PadResponseFunction->GetSave(xpad);
		if (localXDirectionCoupling < minSignal) continue;
		Double_t XYcoupling = localYDirectionCoupling*localXDirectionCoupling;
		if(XYcoupling < minSignal)  continue;
		Int_t bin_low  = TMath::Max(0             ,binT + (int)(dt+mShaperResponse->GetXmin()-0.5));
		Int_t bin_high = TMath::Min(NoOfTimeBins-1,binT + (int)(dt+mShaperResponse->GetXmax()+0.5));
		Int_t index = NoOfTimeBins*((row-1)*NoOfPads+pad-1)+bin_low;
		for(Int_t itbin=bin_low;itbin<=bin_high;itbin++, index++){
		  Double_t t = -dt + (Double_t)(itbin - binT);
		  Double_t signal = XYcoupling*QAv*mShaperResponse->GetSave(&t);
		  //		  if (TMath::Abs(signal) < minSignal) continue; // Remember tail could be negative
		  TotalSignal += signal;
		  SignalSum[index].Sum += signal;
		  if (pad >= pad0 && pad < pad0 + 32 && 
		      itbin >= tbk0 &&  itbin < tbk0 + 64) {
		    padsdE[pad-pad0]   += signal;
		    tbksdE[itbin-tbk0] += signal;
		  }
		  if ( TrackId ) {
		    if (! SignalSum[index].TrackId ) SignalSum[index].TrackId = TrackId;
		    else  // switch TrackId, works only for 2 tracks, more tracks ?
		      if ( SignalSum[index].TrackId != TrackId && SignalSum[index].Sum < 2*signal) 
			SignalSum[index].TrackId = TrackId;
		  }
#if 0
		  if (Debug() > 1) {
		    cout << "simu R/P/T/I = " << row << " /\t" << pad << " /\t" << itbin << " /\t" << index 
			 << "\tSum/Adc/TrackId = " << SignalSum[index].Sum << " /\t" 
			 << SignalSum[index].Adc << " /\t" << SignalSum[index].TrackId << endl;
		  }
#endif
		} // time 
	      } // pad limits
	    } // row limits
	  }  // electrons in Cluster
	} while (kTRUE); // Clusters
	tpc_hit->lgam = -99;
	if (dESum > 0 && dSSum) {
	  tpc_hit->de = dESum*eV; tpc_hit->ds = dSSum; tpc_hit->lgam = TotalSignal;
	  if (hist[ioH][0]) {
	    for (Int_t p = 0; p < 32; p++) hist[ioH][0]->Fill((p+pad0)-padH,xyzL.position().z(),padsdE[p]/dESum);
	  }								                          
	  if (hist[ioH][1]) {						                          
	    for (Int_t t = 0; t < 64; t++) hist[ioH][1]->Fill((t+tbk0+0.5)-tbkH,xyzL.position().z(),tbksdE[t]/dESum);
	  }
	}
	NoHitsInTheSector++;
      }
    } // hit in the sector
    if (NoHitsInTheSector) {
      DigitizeSector(sector);   
      if (Debug()) cout << "StTpcRSMaker: Done with sector\t" << sector << " total no. of hit = " << NoHitsInTheSector << endl;
    }
  } // sector
  if (Debug()%10) gBenchmark->Show("TrsMinMake");
  return kStOK;
}
//________________________________________________________________________________
Double_t StTpcRSMaker::GetNoPrimaryClusters(Double_t betaGamma) {
  if (! mdNdx) return 0;
  Int_t bin = mdNdx->FindBin(betaGamma);
  return mdNdx->GetBinContent(bin);
}
//________________________________________________________________________________
Double_t StTpcRSMaker::ShaperFunc(Double_t *x, Double_t *par) {
  Double_t tau = par[0];
  Double_t width = par[1];
  Double_t p = par[2];
  Double_t t = x[0]*width/tau;
  Double_t Delta = width/tau;
  Double_t t1 = t - Delta/2.;
  Double_t t2 = t1 + Delta;
  Double_t val = TMath::Gamma(p,t2) - TMath::Gamma(p,t1);
  return val;
}
//________________________________________________________________________________
Double_t StTpcRSMaker::PadResponseFunc(Double_t *x, Double_t *par) {
  Double_t CrossTalk = par[4];
  Double_t Value = 0;
  Double_t X = par[5]*x[0];
  if (CrossTalk > 0) {
    for (Int_t i = -1; i <= 1; i++) {
      Double_t xx = X + par[5]*i;
      if (i == 0) Value += (1. - 2.*CrossTalk)*Gatti(&xx,par);
      else        Value +=          CrossTalk *Gatti(&xx,par);
    }
  } else   Value = Gatti(&X,par);
  return Value;
}
//________________________________________________________________________________
Double_t StTpcRSMaker::Gatti(Double_t *x, Double_t *par) {
  /************************************************************************
   *  Function    : generates the cathode signal using                    *
   *                the single-parameter Gatti formula:                   *
   *                              1 - tanh(K2 * lambda)**2                *
   *     GFunc(lambda) = K1 * -------------------------------             *
   *                           1 + K3 * tanh (K2 *lambda)**2              *
   *     lambda = x/h, h is anode cathode spacing                         *
   *                                                                      *
   *     K2 = pi/2*(1 - 0.5*sqrt(K3))                                     *
   *                                                                      *
   *              K2*sqrt(K3)                                             *
   *     K1 = -------------------                                         *
   *            4 * atan(sqrt(K3))                                        *
   *                                                                      *
   *  References  : E.Gatti, A.Longoni, NIM 163 (1979) 82-93.             *
   *  Authors : V.Balagura,V.Cherniatin,A.Chikanian                       *
   ************************************************************************/
  Double_t y = x[0];   // distance to center of strip
  Double_t w = par[0]; // w = width of pad       
  Double_t h = par[1]; // h = Anode-Cathode gap  
  Double_t K3  = par[3];
  Double_t lambda = TMath::Abs(y/h);
  Double_t K2 = TMath::PiOver2()*(1. - 0.5*TMath::Sqrt(K3));  
  //  Double_t K1 = K2*TMath::Sqrt(K3)/(2*TMath::ATan(TMath::Sqrt(K3)));
  Double_t sqK3 = TMath::Sqrt(K3);
  Double_t ATsqK3 = 0.5/TMath::ATan(sqK3);
  Double_t Y1 = lambda - w/h/2;
  Double_t Y2 = Y1 + w/h;
  Double_t X1 = K2*Y1;
  Double_t X2 = K2*Y2;
  Double_t Z1 = sqK3*TMath::TanH(X1);
  Double_t Z2 = sqK3*TMath::TanH(X2);
  Double_t val = ATsqK3*(TMath::ATan(Z2) - TMath::ATan(Z1));
  return val;
}
//________________________________________________________________________________
void  StTpcRSMaker::Print(Option_t *option) const {
  PrPP(Print, NoOfSectors);
  PrPP(Print, NoOfRows);
  PrPP(Print, NoOfInnerRows);
  PrPP(Print, NoOfPads);
  PrPP(Print, W);// = 26.2);//*eV
  PrPP(Print, mCluster);
  PrPP(Print, longitudinalDiffusionConstant);
  PrPP(Print, transverseDiffusionConstant);
  PrPP(Print, samplingFrequency);
  PrPP(Print, tauGlobalOffSet);
  //  PrPP(Print, Gain);
  PrPP(Print, NoOfTimeBins);
  PrPP(Print, numberOfInnerSectorAnodeWires); 
  PrPP(Print, firstInnerSectorAnodeWire);
  PrPP(Print, lastInnerSectorAnodeWire);
  PrPP(Print, numberOfOuterSectorAnodeWires);
  PrPP(Print, firstOuterSectorAnodeWire);
  PrPP(Print, lastOuterSectorAnodeWire);
  PrPP(Print, anodeWirePitch);
  PrPP(Print, OmegaTau); // tan of Lorentz angle
  PrPP(Print, InnerSectorGasGain);
  PrPP(Print, OuterSectorGasGain);
  PrPP(Print, Inner_wire_to_plane_coupling);
  PrPP(Print, Inner_wire_to_plane_couplingScale);
  PrPP(Print, Outer_wire_to_plane_coupling);
  PrPP(Print, Outer_wire_to_plane_couplingScale);
  PrPP(Print, numberOfElectronsPerADCcount);
  PrPP(Print, anodeWireRadius);
  PrPP(Print, innerSectorzOffSet);
  PrPP(Print, outerSectorzOffSet);
  //  PrPP(Print, offset);
  if (TESTBIT(m_Mode, kAVERAGEPEDESTAL)) {
    PrPP(Print, mAveragePedestal);
    PrPP(Print, mAveragePedestalRMS);
    PrPP(Print, mPedestalRMS);
  }
  PrPP(Print, FanoFactor);
  PrPP(Print, TanLorenzAngle);
  PrPP(Print, innerSectorAnodeVoltage);
  PrPP(Print, outerSectorAnodeVoltage);
  PrPP(Print, K3IP);
  PrPP(Print, K3IR);
  PrPP(Print, K3OP);
  PrPP(Print, K3OR);
  PrPP(Print, CrossTalkInner);
  PrPP(Print, CrossTalkOuter);
}
//________________________________________________________________________________
void  StTpcRSMaker::DigitizeSector(Int_t sector){
  static Altro * altro = 0;
  //  static Int_t PedestalMem[512];
  TDataSet *event = GetData("Event");
  StTpcRawData *data = 0;
  if (! event) {
    data = new StTpcRawData(24);
    event = new TObjectSet("Event", data);
    AddData(event);
  } else data = (StTpcRawData *) event->GetObject();
  assert(data);
  SignalSum_t *SignalSum = GetSignalSum();
  Double_t ped    = 0; 
  Double_t pedRMS = 0; 
  Int_t adc = 0;
  Int_t index = 0;
  Double_t gain = 1;
  Int_t row, pad, bin;
  Int_t Sector = TMath::Abs(sector);
  StTpcDigitalSector *digitalSector = data->GetSector(Sector);
  if (! digitalSector) {
    digitalSector = new StTpcDigitalSector();
    data->setSector(Sector,digitalSector);
  } else 
    digitalSector->clear();
  for (row = 1;  row <= NoOfRows; row++) {
    Int_t NoOfPadsAtRow = StTpcDigitalSector::numberOfPadsAtRow(row);
    for (pad = 1; pad <= NoOfPadsAtRow; pad++) {
      gain = 1;
#if 0
      if (! TESTBIT(m_Mode, kGAINOAtALL)) { 
	gain   = gStTpcDb->tpcGain()->Gain(Sector,row,pad);
	if (gain <= 0.0) continue;
      }
#endif
      ped    = mAveragePedestal;
      pedRMS = 0; 
#if 0
      if (! TESTBIT(m_Mode, kNONOISE)) {
	if (TESTBIT(m_Mode, kAVERAGEPEDESTAL)) {
	  ped = gRandom->Gaus(mAveragePedestal,mAveragePedestalRMS);
	  pedRMS = mPedestalRMS;
	}
	else {// if (TESTBIT(m_Mode, kPedestal)) {
	  ped    = gStTpcDb->Pedestal()->Pedestal(sector,row,pad);
	  pedRMS = gStTpcDb->Pedestal()->Rms(sector,row,pad);
	}
      }
#endif
      if (pedRMS > 5.0) continue; // noisy pads
      static  Short_t ADCs[512];
      static UShort_t IDTs[512];
      memset(ADCs, 0, sizeof(ADCs));
      memset(IDTs, 0, sizeof(IDTs));
      if (St_TpcAltroParametersC::instance()->N(sector-1) > 0 && ! altro) {
	/* Tonk 06/25/08
First, there is no BSL1 (which is the pedestal subtraction) since
you're data is already subtracted.

Second, there is no BSL2 in dAu.

For the zero-suppression thresholds I don't remember for
sure, they should be in the RC database (Jeff?) but I think
they were:

pre & post samples = 0
threshold & min-above threshold = 2

so try them.

Jeff,what were the "ASIC" parameters for TPX in dAu?
Can you get it from the RC databases?
	*/
	altro = new Altro(512,ADCs);
	//from ~/public/sources/Altro++/AltroConfigs/AltroConfig.globaltcf.v1.data
	// ConfigAltro(int ONBaselineCorrection1, int ONTailcancellation, int ONBaselineCorrection2, int ONClipping, int ONZerosuppression)
	//	altro->ConfigAltro(                    1,                      1,                         1,              1,                     0);
	altro->ConfigAltro(                    0,                      1,                         0,              1,                     0); // Tonko 06/25/08
	//     ConfigBaselineCorrection_1(int mode, int ValuePeDestal, int *PedestalMem, int polarity)
	//	altro->ConfigBaselineCorrection_1(4, 0, PedestalMem, 0);  // Tonko 06/25/08
	Int_t *altro_reg = St_TpcAltroParametersC::instance()->altro_reg(sector-1);
	altro->ConfigTailCancellationFilter(altro_reg[0],altro_reg[1],altro_reg[2], // K1-3
					    altro_reg[3],altro_reg[4],altro_reg[5]);// L1-3
	//     ConfigBaselineCorrection_2(int HighThreshold, int LowThreshold, int Offset, int Presamples, int Postsamples);
	//altro->ConfigBaselineCorrection_2(                3,                3,          0,              0,               0); // Tonko 06/25/08
	//void ConfigZerosuppression(int Threshold, int MinSamplesaboveThreshold, int Presamples, int Postsamples);
	//	altro->ConfigZerosuppression(            3,                            3,              2,               3);
	altro->ConfigZerosuppression(            2,                            2,              0,               0); // Tonko 06/25/08
	altro->PrintParameters();
      }
      Int_t NoTB = 0;
      index = NoOfTimeBins*((row-1)*NoOfPads+pad-1);
      for (bin = 0; bin < NoOfTimeBins; bin++,index++) {
	//	Int_t index= NoOfTimeBins*((row-1)*NoOfPads+pad-1)+bin;
	// Digits : gain + ped
	if (pedRMS > 0) adc = (Int_t) (SignalSum[index].Sum/gain + gRandom->Gaus(ped,pedRMS));
	else            adc = (Int_t) (SignalSum[index].Sum/gain);
	if (adc > 1023) adc = 1023;
	if (pedRMS > 0) adc = adc - (int) ped;
	if (adc <= 0) continue;
	SignalSum[index].Adc = adc;
	NoTB++;
	ADCs[bin] = adc;
	IDTs[bin] = SignalSum[index].TrackId;
#if 0
	if (Debug() > 1) {
	  cout << "digi R/P/T/I = " << row << " /\t" << pad << " /\t" << bin << " /\t" << index 
	       << "\tSum/Adc/TrackId = " << SignalSum[index].Sum << " /\t" 
	       << SignalSum[index].Adc << " /\t" << SignalSum[index].TrackId << endl;
	}
#endif
      }
      if (! NoTB) continue;
      if (altro) altro->RunEmulation();
      else {
	if (St_TpcAltroParametersC::instance()->N(sector-1) < 0) NoTB = AsicThresholds(ADCs);
      }
      if (NoTB && digitalSector) {
	digitalSector->putTimeAdc(row,pad,ADCs,IDTs);
      }
    }
  }
  SafeDelete(altro);
}
//________________________________________________________________________________
Int_t StTpcRSMaker::AsicThresholds(Short_t ADCs[512]) {
  Int_t t1 = 0;
  Int_t nSeqLo = 0;
  Int_t nSeqHi = 0;
  Int_t noTbleft = 0;
  for (UInt_t tb = 0; tb < 512; tb++) {
    if (ADCs[tb] <= St_asic_thresholdsC::instance()->thresh_lo()) {
      if (! t1) ADCs[tb] = 0;
      else {
	if (nSeqLo <= St_asic_thresholdsC::instance()->n_seq_lo() ||
	    nSeqHi <= St_asic_thresholdsC::instance()->n_seq_hi()) 
	  for (UInt_t t = t1; t <= tb; t++) ADCs[t] = 0;
	else noTbleft += nSeqLo;
      }
      t1 = nSeqLo = nSeqHi = 0;
    }
    nSeqLo++; 
    if (! t1) t1 = tb;
    if (ADCs[tb] > St_asic_thresholdsC::instance()->thresh_hi()) {nSeqHi++;}
  }
  return noTbleft;
}
//________________________________________________________________________________
Double_t StTpcRSMaker::InducedCharge(Double_t s, Double_t h, Double_t ra, Double_t Va, Double_t &t0) {
  // Calculate variation of induced charge due to different arrived angles 
  // alpha = -26 and -70 degrees
  cout << "wire spacing = " << s << " cm"
       << "\tcathode anode gap = " << h << " cm"
       << "\tanode wire radius = " << ra << " cm"
       << "\tpotential on anode wire = " << Va << " V" << endl;
  const Double_t B  = 30e-3; // 1/V
  const Double_t E0 = 20e3; // V/cm
  const Double_t mu = 2.26; // cm**2/V/sec CH4+ mobility 
  // const Double_t mu = 1.87; // cm**2/V/sec Ar+ mobility 
  Double_t alpha[2] = {-26., -70.};
  Double_t pi = TMath::Pi();
  // E.Mathieson (3.2b), V.Chernyatin said that it should be used this (Weber ) approximation 07/09/08
  Double_t rc = s/(2*pi)*TMath::Exp(pi*h/s); cout << "rc(Cylinder approx) = " << rc << " cm" << endl; 
  //  Double_t rc = 4*h/pi; cout << "rc = " << rc << " cm" << endl;   // E.Mathieson (4.3), no valid for our case
  Double_t C  = 1./(2*TMath::Log(rc/ra)); cout << "C = " << C << endl;
  Double_t E  = 2*pi*C*Va/s; cout << "E = " << E << " V/cm" << endl;
  // Gain variation: M = M0*(1 - k*cos(2*alpha))
  Double_t k = 2*B/3.*TMath::Power((pi/E0/s),2)*TMath::Power(C*Va,3); cout << "k = " << k << endl;
  // Induced charge variation
  t0 = ra*ra/(4*mu*C*Va); cout << "t0 = " << 1e9*t0 << " ns" << endl;                                     // E.Mathieson (2.10)
  Double_t Tav = t0*h/s/(2*pi*C);  cout << "Tav = " << 1e9*Tav << " ns" << endl;
  //  Double_t t = 5*55e-9;             cout << "t = " << 1e9*t << " ns" << endl;
  Double_t t = 180e-9;             cout << "t = " << 1e9*t << " ns" << endl; 
  Double_t rp = TMath::Sqrt(1. + t/t0); cout << "r' = " << rp << endl;
  // qc = rp*ra*sin(alpha)/(2*h) + C/2*log(1 + t/t0) = A*sin(alpha) + B
  Double_t Aconstant = rp*ra/(2*h);        cout << "Aconstant = " << Aconstant << endl;
  Double_t Bconstant = C/2*TMath::Log(1 + t/t0); cout << "Bconstant = " << Bconstant << endl;
  Double_t Gains[2];
  for (Int_t i = 0; i < 2; i++) {
    Gains[i] = Aconstant*TMath::Sin(pi/180*alpha[i]) + Bconstant; 
    cout << "Gain = " << Gains[i] << " at alpha = " << alpha[i] << " degree" << endl;
  }
  Double_t GainsAv = TMath::Sqrt(Gains[0]*Gains[1]);
  Double_t r = 0;
  for (Int_t i = 0; i < 2; i++) {
    r = TMath::Log(Gains[i]/GainsAv); cout << "Relative gain " << r << " at alpha = " << alpha[i] << endl;
  }
  return r;
}
//________________________________________________________________________________
Int_t StTpcRSMaker::SearchT(const void *elem1, const void **elem2) { 
  g2t_tpc_hit_st *value1 = (g2t_tpc_hit_st *) elem1;    
  g2t_tpc_hit_st *value2 = (g2t_tpc_hit_st *) *elem2;   
  if (value1->volume_id%100000 != value2->volume_id%100000) return value1->volume_id%100000 - value2->volume_id%100000;
  else {
    if (value1->track_p != value2->track_p) return value1->track_p - value2->track_p;
    else {
      return (Int_t) ((value1->x[2] - value2->x[2])/4.); // within 4 cm
    }
  }
}
//________________________________________________________________________________
Int_t StTpcRSMaker::CompareT(const void **elem1, const void **elem2) {
  return SearchT(*elem1, elem2);
}                                                     
#if 0
//________________________________________________________________________________
Double_t StTpcRSMaker::DriftLength(Double_t x, Double_t y) {
  static const Double_t Step = 5e-2;
  Double_t r = TMath::Sqrt(x*x + y*y);
  if (r < 0.25) return r;
  x = TMath::Abs(x);
  y = TMath::Abs(y);
  Int_t Nstep = 0;
  while (x > Step || y > Step) {
    Double_t Slope = TMath:SinH(TMath::Pi()*y/s)/TMath:Sin(TMath::Pi()*x/s);
    Double_t Co2 = 1./(1. + Slope*Slope);
    Double_t Si  = TMath::Sqrt(1. - Co2);
    Double_t Co  = TMath::Sqrt(Co2);
    x = TMath::Abs(x - Step*Co);
    y = TMath::Abs(y - Step*Si);
    NStep++
  }
  return NStep*Step;
}
#endif
//______________ EXPONENTIAL INTEGRALS En __________________________________________________________________
// define: E_n(x) = \int_1^infty{exp(-xt)/t^n}dt, x>0, n=0,1,...
Double_t StTpcRSMaker::expint(Int_t n, Double_t x) {
  // based on Numerical Recipes in C
  const Double_t euler = 0.57721566; // Euler's constant, gamma
  const Int_t    maxit = 100;        // max. no. of iterations allowed
  const Double_t fpmin = 1.0e-30;    // close to smallest floating-point   number
  const Double_t eps = 6.0e-8;       // relative error, or absolute error near
  // the zero of Ei at x=0.3725
  
  Int_t i, ii, nm1;
  Double_t a,b,c,d,del,fact,h,psi,ans;
  
  nm1=n-1;
  if(n<0 || x<0 || (x==0 && (n==0 || n==1))) {
    cout << "Bad argument for expint(n,x)" << endl; return -1;
  }
  else {
    if(n==0) ans=exp(-x)/x;
    else {
      if(x==0) ans=1.0/nm1;
      else {
	if(x>1) {
	  b=x+n;
	  c=1.0/fpmin;
	  d=1.0/b;
	  h=d;
	  for(i=1; i<maxit; i++) {
	    a = -i*(nm1+i);
	    b += 2.0;
	    d=1.0/(a*d+b);
	    c=b+a/c;
	    del=c*d;
	    h *= del;
	    if(fabs(del-1.0)<eps) {
	      ans=h*exp(-x);
	      return ans;
	    }
	  }
	  cout << "***continued fraction failed in expint(n,x)!!!" << endl;
	  return -1;
	} else {
	  ans = (nm1!=0 ? 1.0/nm1 : -log(x)-euler);
	  fact=1;
	  for(i=1; i<=maxit; i++) {
	    fact *= -x/i;
	    if(i!=nm1) del = -fact/(i-nm1);
	    else {
	      psi = -euler;
	      for(ii=1; ii<=nm1; ii++) psi += 1.0/ii;
	      del = fact*(-log(x)+psi);
	    }
	    ans += del;
	    if(fabs(del)<fabs(ans)*eps) return ans;
	  }
	  cout << "***series failed in expint!!!" << endl;
	  return -1;
	}
      }
    }
  }
  
  return ans;
}
//______________ EXPONENTIAL INTEGRAL Ei __________________________________________________________________
// define: ei(x) = -\int_{-x}^{\infty}{exp(-t)/t}dt,  for x>0
// power series: ei(x) = eulerconst + ln(x) + x/(1*1!) + x^2/(2*2!) + ...
Double_t StTpcRSMaker::ei(Double_t x)
{ // taken from Numerical Recipes in C
  const Double_t euler = 0.57721566; // Euler's constant, gamma
  const Int_t maxit = 100;           // max. no. of iterations allowed
  const Double_t fpmin = 1.e-7; //1.0e-40;    // close to smallest floating-point number
  const Double_t eps = 1.e-7; //1.0e-30;       // relative error, or absolute error  near
                                    // the zero of Ei at x=0.3725
  //  I actually changed fpmin and eps into smaller values than in NR
  
  Int_t k;
  Double_t fact, prev, sum, term;
  
  // special case
  if(x < 0) return -expint(1,-x);
  
  if(x == 0.0) { cout << "Bad argument for ei(x)" << endl; return -1; }
  if(x < fpmin) return log(x)+euler;
  if(x <= -log(eps)) {
    sum = 0;
    fact = 1;
    for(k=1; k<=maxit; k++) {
      fact *= x/k;
      term = fact/k;
      sum += term;
      if(term < eps*sum) break;
    }
    if(k>maxit) { cout << "Series failed in ei(x)" << endl; return -1; }
    return sum+log(x)+euler;
  } else {
    sum = 0;
    term = 1;
    for(k=1; k<=maxit; k++) {
      prev = term;
      term *= k/x;
      if(term<eps) break;
      if(term<prev) sum+=term;
      else {
	sum -= prev;
	break;
      }
    }
    return exp(x)*(1.0+sum)/x;
  }
}
//________________________________________________________________________________
Double_t StTpcRSMaker::shapeEI(Double_t *x, Double_t *par) {// does not work. It is needed to 1/s
  Double_t t0    = par[0];
  Double_t tau_I = par[1];
#if 0
  Double_t a = - 1./tau_I;
  Double_t t  = x[0];
  Double_t value = 0;
  if (t <= 0) return value;
  value = TMath::Exp(a*(t+t0))*(ei(-a*(t+t0))-ei(-a*t0));
#else
  Int_t io = (Int_t) par[4];
  Double_t a[2] = {- 1./tau_I, - 1./tauC[io]};
  Double_t A[2] = {a[0]/(a[0]-a[1]), a[1]/(a[1]-a[0])};
  Double_t t  = x[0];
  Double_t value = 0;
  if (t <= 0) return value;
  for (Int_t i = 0; i < 2; i++) {
    value += A[i]*TMath::Exp(a[i]*(t+t0))*(ei(-a[i]*(t+t0))-ei(-a[i]*t0));
  }
#endif
  return value;
}
//________________________________________________________________________________
Double_t StTpcRSMaker::shapeEI_I(Double_t *x, Double_t *par) { //Integral of shape over time bin
  static Double_t sqrt2 = TMath::Sqrt(2.);
  Double_t TimeBinWidth = par[2];
  Double_t norm = par[3];
  Double_t t1 = TimeBinWidth*(x[0] - 0.5);
  Double_t t2 = t1 + TimeBinWidth;
  Int_t io = (Int_t) par[4];
  assert(io >= 0 && io <= 1);
  return sqrt2*fgTimeShape0[io]->Integral(t1,t2)/norm;
}
//________________________________________________________________________________
Double_t StTpcRSMaker::shapeEI3(Double_t *x, Double_t *par) {// does not work. It is needed to 1/s
  Double_t t0    = par[0];
  Double_t tau_F = par[1];
  Double_t tau_P = par[2];
  Double_t tau_I = par[3];
  Double_t d =   1./tau_P;
#if 0
  Double_t a[2] = {- 1./tau_I, - 1./tau_F};
  Double_t A[2] = {(a[0]+d)/(a[0]-a[1]), (a[1]+d)/(a[1]-a[0])};
  Double_t t  = x[0];
  Double_t value = 0;
  if (t <= 0) return value;
  for (Int_t i = 0; i < 2; i++) {
    value += A[i]*TMath::Exp(a[i]*(t+t0))*(ei(-a[i]*(t+t0))-ei(-a[i]*t0));
  }
#else
  Int_t io = (Int_t) par[6];
  Double_t a[3] = {- 1./tau_I, - 1./tau_F, - 1./tauC[io]};
  Double_t A[3] = {(a[0] + d)/a[0]/(a[0] - a[1])/(a[0] - a[2]),
		   (a[1] + d)/a[1]/(a[1] - a[0])/(a[1] - a[2]),
		   (a[2] + d)/a[2]/(a[2] - a[0])/(a[2] - a[1])}; 
  Double_t t  = x[0];
  Double_t value = - d/(a[0]*a[1]*a[2])/(t + t0);
  for (Int_t i = 0; i < 3; i++) {
    value += A[i]*TMath::Exp(a[i]*(t+t0))*(ei(-a[i]*(t+t0))-ei(-a[i]*t0));
  }
#endif
  return value;
}
//________________________________________________________________________________
Double_t StTpcRSMaker::shapeEI3_I(Double_t *x, Double_t *par) { //Integral of shape over time bin
  static Double_t sqrt2 = TMath::Sqrt(2.);
  Double_t TimeBinWidth = par[4];
  Double_t norm = par[5];
  Double_t t1 = TimeBinWidth*(x[0] - 0.5);
  Double_t t2 = t1 + TimeBinWidth;
  Int_t io = (Int_t) par[6];
  assert(io >= 0 && io <= 1);
  return sqrt2*fgTimeShape3[io]->Integral(t1,t2)/norm;
}
//________________________________________________________________________________
SignalSum_t  *StTpcRSMaker::GetSignalSum() {
  if (! m_SignalSum) 
    m_SignalSum = (SignalSum_t  *) malloc(NoOfRows*NoOfPads*NoOfTimeBins*sizeof(SignalSum_t)); 
  return m_SignalSum;
}
//________________________________________________________________________________
SignalSum_t  *StTpcRSMaker::ResetSignalSum() {
  GetSignalSum();
  memset (m_SignalSum, 0, NoOfRows*NoOfPads*NoOfTimeBins*sizeof(SignalSum_t));
  return m_SignalSum;
}

//________________________________________________________________________________
// $Log: StTpcRSMaker.cxx,v $
// Revision 1.4  2008/07/18 16:21:16  fisyak
// Remove TF1F
//
// Revision 1.3  2008/06/25 20:02:32  fisyak
// The first set of parametrs for Altro, Remove gains for the moment
//
// Revision 1.2  2008/06/19 22:45:43  fisyak
// Freeze problem with TPX parameterization
//
// Revision 1.1.1.1  2008/04/28 14:39:47  fisyak
// Start new Tpc Response Simulator
//
// Revision 1.20  2008/04/24 10:42:03  fisyak
// Fix binning issues
//
// Revision 1.19  2008/04/04 15:00:11  fisyak
// Freeze before shaper modifications
//
// Revision 1.18  2005/02/07 21:40:31  fisyak
// rename antique TGeant3 to TGiant3
//
// Revision 1.17  2005/01/26 23:28:38  fisyak
// Check boundary for sorted tpc_hit array
//
// Revision 1.16  2005/01/26 21:45:31  fisyak
// Freeze correction made in June
//
// Revision 1.14  2004/06/04 17:09:02  fisyak
// Change tau in Chaper and OmegaTau for gas
//
// Revision 1.13  2004/05/29 21:16:27  fisyak
// Fix pad direction, add sorting for ADC/cluster nonlinearity, replace product by sum of logs
//
// Revision 1.12  2004/05/17 19:45:08  fisyak
// Clean up, add pseudo padrows
//
// Revision 1.11  2004/05/05 17:41:52  fisyak
// Take K3 from E.Mathieson book
//
// Revision 1.10  2004/05/04 13:39:06  fisyak
// Add TF1
//
// Revision 1.9  2004/05/02 20:54:18  fisyak
// fix t0 offset
//
// Revision 1.8  2004/04/22 01:05:03  fisyak
// Freeze the version before modification parametrization for K3
//
// Revision 1.7  2004/04/12 14:30:07  fisyak
// Propagate cluster as a whole
//
// Revision 1.6  2004/04/06 01:50:13  fisyak
// Switch from Double_t to Float_t for sum
//
// Revision 1.5  2004/03/30 19:30:04  fisyak
// Add Laser
//
// Revision 1.4  2004/03/21 19:00:43  fisyak
// switch to GEANT step length
//
// Revision 1.3  2004/03/20 17:57:15  fisyak
// Freeze the version of PAI model
//
// Revision 1.2  2004/03/17 20:47:43  fisyak
// Add version with TrsCluster TTree
//
// Revision 1.1.1.1  2004/03/05 20:51:25  fisyak
// replacement for Trs
//
