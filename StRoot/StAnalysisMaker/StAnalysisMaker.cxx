// $Id: StAnalysisMaker.cxx,v 1.4 1999/02/12 02:00:26 wenaus Exp $
// $Log: StAnalysisMaker.cxx,v $
// Revision 1.4  1999/02/12 02:00:26  wenaus
// Incorporate tag loading example
//
// Revision 1.3  1999/02/11 15:39:15  wenaus
// cleanup
//
// Revision 1.2  1999/02/10 23:59:52  wenaus
// cleanup
//
// Revision 1.1  1999/02/05 17:54:55  wenaus
// initial commit
//
//
///////////////////////////////////////////////////////////////////////////////
//
// StAnalysisMaker
//
// Description: 
//  Sample maker to access and analyze StEvent
//
// Environment:
//  Software developed for the STAR Detector at Brookhaven National Laboratory
//
// Author List: 
//  Torre Wenaus, BNL
//
// History:
//
///////////////////////////////////////////////////////////////////////////////
 * Revision for new StEvent
#include "StRoot/StEventReaderMaker/StEventReaderMaker.h"
#include "StChain/StChain.h"
#include "StEvent/StRun.hh"
#include "StEvent/StEvent.hh"

static const char rcsid[] = "$Id: StAnalysisMaker.cxx,v 1.4 1999/02/12 02:00:26 wenaus Exp $";
#include "StMessMgr.h"
void summarizeEvent(StEvent& event);
//  specific analysis tasks.
//
void summarizeEvent(StEvent& event, Int_t &nevents);
Int_t StAnalysisMaker::Make() {
  StEventReaderMaker* evMaker = (StEventReaderMaker*) gStChain->Maker("events");
  if (! evMaker->event()) return kStOK; // If no event, we're done
  StEvent& ev = *(evMaker->event());
  StRun& run = *(evMaker->run());

  // OK, we've got the event. Pass it and process it.
  summarizeEvent(ev);
  long ntk = countPrimaryTracks(ev);
  cout << "Primary tracks: " << ntk << endl;

  // Create and fill a tag
  if (theTag) delete theTag;
  theTag = new HighPtTag_st;
  tagFiller(ev,*theTag);
void tagFiller(StEvent& event, HighPtTag_st& hptTag);
  return kStOK;
    drawinit = kFALSE;
    theTag = 0;

StAnalysisMaker::StAnalysisMaker(const Char_t *name, const Char_t *title) : StMaker(name, title) {
  drawinit = kFALSE;
  theTag = 0;
}

StAnalysisMaker::~StAnalysisMaker() {
}

Int_t StAnalysisMaker::Init() {
Int_t
StAnalysisMaker::Init()
{
void StAnalysisMaker::MakeBranch() {
  StMaker::MakeBranch();
}

void StAnalysisMaker::PrintInfo() {
  printf("**************************************************************\n");
  printf("* $Id: StAnalysisMaker.cxx,v 1.4 1999/02/12 02:00:26 wenaus Exp $\n");
  printf("**************************************************************\n");
  if (gStChain->Debug()) StMaker::PrintInfo();
}

void StAnalysisMaker::Clear(Option_t *opt) {
  if (theTag) {
    delete theTag;
    theTag = 0;
  }
  SafeDelete(m_DataSet);
}

void StAnalysisMaker::SetBranch() {
  StMaker::SetBranch();
    tagFiller(ev,*theTag);

Int_t StAnalysisMaker::Finish() {
  return kStOK;
}

ClassImp(StAnalysisMaker)
    
    return kStOK;
}
