//*-- Author : David Hardtke
// 
// $Id: StTpcT0Maker.cxx,v 1.8 2001/04/17 23:54:51 hardtke Exp $
// $Log: StTpcT0Maker.cxx,v $
// Revision 1.8  2001/04/17 23:54:51  hardtke
// add z Vertex contraint -- default to +-40cm
//
// Revision 1.7  2001/03/15 19:49:02  hardtke
// Add diagnostic ntuple t0hist file
//
// Revision 1.6  2001/03/09 22:44:43  hardtke
// Add vertex diagnostic histograms, create root file with these histograms by default
//
// Revision 1.5  2000/09/11 17:48:50  hardtke
// save values of trig offset, dvel, and tpc length for use in Finish()
//
// Revision 1.4  2000/09/01 21:24:28  hardtke
// Add kluge to evade infrastructure bug
//
// Revision 1.3  2000/08/28 17:42:29  hardtke
// Add new histogram
//
// Revision 1.2  2000/08/26 23:50:04  fisyak
// Make it chain
//
// Revision 1.1  2000/08/24 23:51:27  hardtke
// New package for drift velocity calibrations
//
//
//////////////////////////////////////////////////////////////////////////
//                                                                      //
// StTpcT0Maker class                                                   //
//                                                                      //
//////////////////////////////////////////////////////////////////////////

#include "StTpcT0Maker.h"
#include "StChain.h"
#include "St_DataSetIter.h"
#include "StDAQMaker/StDAQReader.h"
#include "StTpcDb/StTpcDbMaker.h"
#include "StTpcDb/StTpcDb.h"
#include "St_tcl_Maker/St_tcl_Maker.h"
#include "tables/St_dst_vertex_Table.h"
#include "tables/St_dst_L0_Trigger_Table.h"
#include "StMessMgr.h"
#include "StVertexId.h"
#include "tables/St_tpcDriftVelocity_Table.h"
#include "TSystem.h"
#include "TFile.h"

ClassImp(StTpcT0Maker)

//_____________________________________________________________________________
StTpcT0Maker::StTpcT0Maker(const char *name):StMaker(name){
  minEntries = 5;   //require 5 valid t0s for a velocity determination
  desiredEntries = 99999; // must set limit by hand. 
  maxRMS     = 0.05; //t0 should be good to 50 ns
  //  mHistOut   = kFALSE;
  mHistOut=kTRUE;
  StMaker *saveMk = cd();
  New("St_tcl_Maker","tpc_hits");
  New("St_tpt_Maker","tpc_tracks");
  New("StMatchMaker","match");
  New("StPrimaryMaker","primary");
  New("St_dst_Maker","dst");
  zVertexMax = 40.0;
  zVertexMin = -40.0;
  saveMk->cd();
}
//_____________________________________________________________________________
StTpcT0Maker::~StTpcT0Maker(){
}
//_____________________________________________________________________________
Int_t StTpcT0Maker::Init(){
  theDb = ((StTpcDbMaker*)GetMaker("tpcDB"))->tpcDbInterface();
  t0guess = 0;
  zVertexWest = -999.0;
  zVertexEast = -999.0; 
  T0HIST_MIN = 36.0;
  T0HIST_MAX = 38.0;
  t0result = new TH1F("t0result","t0result",1000,T0HIST_MIN,T0HIST_MAX);
  t0guessError = new TH1F("t0guessError","t0 measured - t0 guess",1000,-1,1);
  xVertexDiff = new TH1F("xVertexDiff","x Vertex: East - West",600,-0.3,0.3);
  yVertexDiff = new TH1F("yVertexDiff","y Vertex: East - West",600,-0.3,0.3);
  zVertexDiff = new TH1F("zVertexDiff","z Vertex: East - West",600,-0.3,0.3);
  resNtuple = new TNtuple("resNtuple","resNtuple","event:xEast:yEast:zEast:xWest:yWest:zWest:multEast:multWest");
  AddHist(t0result);
  AddHist(t0guessError);
  AddHist(xVertexDiff);
  AddHist(yVertexDiff);
  AddHist(zVertexDiff);
  date = 0;
  time = 0;
  dvel_assumed=0.0;
  trigger_assumed=0.0;
  length_assumed=0.0;
  return StMaker::Init();
}

void StTpcT0Maker::Clear(Option_t *option){
  t0guess = 0;
  zVertexWest = -999.0;
  zVertexEast = -1999.0; 
  yVertexWest = -999.0;
  yVertexEast = 999.0; 
  xVertexWest = -999.0;
  xVertexEast = 999.0; 
}

//_____________________________________________________________________________
Int_t StTpcT0Maker::Make(){
  if (date==0) {date = GetDate();cout << "date = " << date << endl;}
  if (time==0) {time = GetTime();cout << "time = " << time << endl;}
  if (dvel_assumed==0.0) {dvel_assumed = 1e-6*theDb->DriftVelocity();
  gMessMgr->Info() << "StTpcT0Maker::Drift Velocity = " << dvel_assumed << endm;} 
  if (trigger_assumed==0.0){ trigger_assumed = theDb->triggerTimeOffset()*1e6;
  gMessMgr->Info() << "StTpcT0Maker::Trig Offset  = " << trigger_assumed << endm;} 
  if (length_assumed==0.0){ length_assumed =  theDb->Dimensions()->outerEffectiveDriftDistance();
  gMessMgr->Info() << "StTpcT0Maker::TPC Length = " << length_assumed << endm;} 
//   if (dvel_assumed!=(1e-6*theDb->DriftVelocity())||trigger_assumed!=(theDb->triggerTimeOffset()*1e6)||length_assumed!=(theDb->Dimensions()->outerEffectiveDriftDistance())) {
//     gMessMgr->Error() << "StTpcT0Maker::t0 assumed has changed, calibration ceasing" << endm;
//      return kStEOF;
//   }
  t0guess = length_assumed/dvel_assumed - trigger_assumed;
  gMessMgr->Info() << "StTpcT0Maker::t0 guess = " << t0guess << " micro seconds" << endm; 
  if (!GetMaker("tpc_hits")||!GetMaker("tpc_tracks")||!GetMaker("match")||!GetMaker("primary")||!GetMaker("dst")) {
    gMessMgr->Error() << "StTpcT0Maker::Missing Another Maker, check chain options " << endm;
      return kStFatal;
  }
  St_tcl_Maker* tcl = (St_tcl_Maker*)GetMaker("tpc_hits");
  tcl->Clear();
  //  GetMaker("tpc_tracks")->Clear();
  // GetMaker("primary")->Clear();
  //  GetMaker("dst")->Clear();
  tcl->AllOn();
  tcl->EastOff();
  tcl->Make();  
  GetMaker("tpc_tracks")->Make();
  //  GetMaker("PreVtx")->Make();
  GetMaker("match")->Make();
  GetMaker("primary")->Make();
  GetMaker("dst")->Make();
    TDataSet *ds=GetDataSet("dst/vertex");
    if(!ds) return kStErr;
    TDataSetIter dsiter(ds);
    St_dst_vertex *vert = (St_dst_vertex *) dsiter.Find("vertex");
    if (!vert)  return kStErr;
    dst_vertex_st *sth = vert->GetTable();
    for (int ij=0;ij<vert->GetNRows();ij++,sth++){
     if(sth->iflag==1&&sth->vtx_id==kEventVtxId){
       zVertexWest = sth->z;
       yVertexWest = sth->y;
       xVertexWest = sth->x;
       multWest = (float)(sth->n_daughters);
       break;    // found primary vertex
     }    
    }

  tcl->Clear();
  GetMaker("tpc_tracks")->Clear();
  //  GetMaker("PreVtx")->Clear();
  GetMaker("match")->Clear();
  GetMaker("primary")->Clear();
  GetMaker("dst")->Clear();
  tcl->AllOn();
  //check to see if event is OK
  if (zVertexWest<zVertexMin||zVertexWest>zVertexMax){
    gMessMgr->Info() << "StTpcT0Maker::Vertex out of range, skip event" << endm;
    return kStOK;
  }

  tcl->WestOff();
  tcl->Make();  
  GetMaker("tpc_tracks")->Make();
  // GetMaker("PreVtx")->Make();
  GetMaker("match")->Make();
  GetMaker("primary")->Make();
  GetMaker("dst")->Make();
    TDataSet *ds1=GetDataSet("dst/vertex");
    if (!ds1) return kStErr;
    TDataSetIter dsiter1(ds1);
    St_dst_vertex *vert1 = (St_dst_vertex *) dsiter1.Find("vertex");
    if (!vert1)  return kStErr;
    dst_vertex_st *sth1 = vert1->GetTable();
    for (int ij=0;ij<vert1->GetNRows();ij++,sth1++){
     if(sth1->iflag==1&&sth1->vtx_id==kEventVtxId){
       zVertexEast = sth1->z;
       yVertexEast = sth1->y;
       xVertexEast = sth1->x;
       multEast = (float)(sth1->n_daughters);
       break;    // found primary vertex
     }    
    }

    if (zVertexEast>-999&&zVertexWest>-999&&zVertexEast>zVertexMin&&zVertexEast<zVertexMax){
      t0current = (zVertexEast-zVertexWest)/(2*dvel_assumed) + t0guess;
      gMessMgr->Info() << "StTpcT0Maker::zVertexWest = " << zVertexWest << endm;
      gMessMgr->Info() << "StTpcT0Maker::zVertexEast = " << zVertexEast << endm;
      gMessMgr->Info() << "StTpcT0Maker::t0 = " << t0current << endm;
      t0result->Fill(t0current);
      t0guessError->Fill(t0current-t0guess);
      xVertexDiff->Fill(xVertexEast-xVertexWest);
      yVertexDiff->Fill(yVertexEast-yVertexWest);
      zVertexDiff->Fill(zVertexEast-zVertexWest);
      eventNumber = (float)GetEventNumber();
      resNtuple->Fill(eventNumber,xVertexEast,yVertexEast,zVertexEast,xVertexWest,yVertexWest,zVertexWest,multEast,multWest);
      if (t0current<T0HIST_MIN||t0current>T0HIST_MAX){
         gMessMgr->Info() << "StTpcT0Maker::t0 out of defined range for histogram"<< endm;
      }
    }
  tcl->Clear();
  GetMaker("tpc_tracks")->Clear();
  //  GetMaker("PreVtx")->Clear();
  GetMaker("match")->Clear();
  GetMaker("primary")->Clear();
  GetMaker("dst")->Clear();
  tcl->AllOn();
  if (t0result->GetEntries()>=desiredEntries){
    gMessMgr->Info() << "StTpcT0Maker::Sufficient Statistics, Ending Chain" << endm;
//   if (t0result->GetEntries()>minEntries){
//     if (t0result->GetRMS()<maxRMS){
//      WriteTableToFile();
//     }
//     else{
//       gMessMgr->Error() << "StTpcT0Maker::t0 unstable. RMS = " << t0result->GetRMS() << " micro-seconds. No table will be written" << endm;
//     }
//   }
//   else {
//    gMessMgr->Error() << "StTpcT0Maker::Insufficient statistics for velocity determination.  Only " << t0result->GetEntries() << " entries. No table will be written" << endm;
//   }
//   minEntries = 9999;
    return kStEOF;
  }
  return kStOK;
}


Int_t StTpcT0Maker::Finish() {
  if (t0result->GetEntries()>=minEntries){
    if (t0result->GetRMS()<maxRMS){
     WriteTableToFile();
    }
    else{
      gMessMgr->Error() << "StTpcT0Maker::t0 unstable. RMS = " << t0result->GetRMS() << " micro-seconds. No table will be written" << endm;
    }
  }
  else {
   gMessMgr->Error() << "StTpcT0Maker::Insufficient statistics for velocity determination.  Only " << t0result->GetEntries() << " entries. No table will be written" << endm;
  }
  if (mHistOut){
    WriteHistFile();
  }  
    return StMaker::Finish();
}

void StTpcT0Maker::PrintInfo() {
  printf("**************************************************************\n");
  printf("* $Id: StTpcT0Maker.cxx,v 1.8 2001/04/17 23:54:51 hardtke Exp $\n");
  printf("**************************************************************\n");

  if (Debug()) StMaker::PrintInfo();
}

void StTpcT0Maker::WriteTableToFile(){
  char filename[80]; 
  sprintf(filename,"./StarDb/Calibrations/tpc/tpcDriftVelocity.%08d.%06d.C",date,time);
  TString dirname = gSystem->DirName(filename);
  if (gSystem->OpenDirectory(dirname.Data())==0) { 
    if (gSystem->mkdir(dirname.Data())) {
      cout << "Directory " << dirname << " creation failed" << endl;
      cout << "Putting tpcDriftVelocity.C in current directory" << endl;
      for (int i=0;i<80;i++){filename[i]=0;}
      sprintf(filename,"tpcDriftVelocity.%08d.%06d.C",date,time);
    }
  }
  ofstream *out = new ofstream(filename);
  driftTable()->SavePrimitive(*out,"");
  return;
}

 St_tpcDriftVelocity* StTpcT0Maker::driftTable(){
   float velocity = length_assumed/(AverageT0() + trigger_assumed);
  St_tpcDriftVelocity* table = new St_tpcDriftVelocity("tpcDriftVelocity",1);
  tpcDriftVelocity_st* row = table->GetTable();
  row->cathodeDriftVelocityEast = velocity;
  row->cathodeDriftVelocityWest = velocity;
  row->laserDriftVelocityEast = 0.0;
  row->laserDriftVelocityWest = 0.0;
  table->SetNRows(1);
  return table;
 }

void StTpcT0Maker::WriteHistFile(){
  char filename[80]; 
  sprintf(filename,"t0hist.%08d.%06d.root",date,time);
  TFile out(filename,"RECREATE");
  GetHistList()->Write();
  resNtuple->Write();
  out.Close();
}

float StTpcT0Maker::AverageT0(){float mean = t0result->GetMean();return mean;}
void StTpcT0Maker::SetMinEntries(int entries){minEntries = entries; }
void StTpcT0Maker::SetDesiredEntries(int entries){desiredEntries = entries;
if (minEntries>desiredEntries) minEntries = desiredEntries; }
void StTpcT0Maker::SetMaxRMS(float rms){maxRMS = rms;}
int  StTpcT0Maker::GetValidityDate(){return date;}
int  StTpcT0Maker::GetValidityTime(){return time;}
void StTpcT0Maker::HistFileByDefault(){mHistOut = kTRUE;} 
void StTpcT0Maker::SetVertexZmax(float zmax){zVertexMax = zmax;}
void StTpcT0Maker::SetVertexZmin(float zmin){zVertexMin = zmin;}

