//////////////////////////////////////////////////////////////////////
//
// $Id: StJets.cxx,v 1.8 2004/05/06 22:55:27 thenry Exp $
// $Log: StJets.cxx,v $
// Revision 1.8  2004/05/06 22:55:27  thenry
// This works better.
//
// Revision 1.7  2003/09/02 17:59:01  perev
// gcc 3.2 updates + WarnOff
//
// Revision 1.6  2003/05/20 20:46:04  thenry
// Accidentally left debug output line in last commit.
//
// Revision 1.5  2003/05/20 20:22:44  thenry
// Moved body of jetTrackIndices to cxx.
//
// Revision 1.4  2003/05/20 19:17:42  thenry
// Fixed problem with jet value accessor functions (always returned -999. fixed),
// now return useful values.
//
// Revision 1.3  2003/05/15 17:48:26  thenry
// Previous versions of StJets expected only primary TPC tracks to be used by
// the jet maker.  That changed with the introduction of EMC points.
// Therefore, a bug existed in jetParticles, because this function
// assumed that all the TrackToJetIndices were valid primary TPC track indices.
// This bug has been fixed, so that if the TrackToJetIndex is greater than
// the number of primary tracks, that index is skipped in the construction
// of the StJets::TrackVec.  Therefore, the StJets::jetParticles function NOW
// does exactly what it did before, completely ignoring EMC Points, even when
// they contribute to the jet.
//
// In addition, a new function was added: jetTrackIndices(), which returns a
// vector of integers corresponding to TPC track indices with the addition of
// (EMC Point index + number TPC primary tracks)).  This function then allows
// us to determine which tracks and which points (their indexes at least) are
// part of each jet, even if we do not have a correctly filled StppEvent*.
//
// Revision 1.2  2003/05/09 19:28:13  thenry
// No changes.
//
// Revision 1.1  2002/12/04 20:28:07  thenry
// StppuDstMaker was modified to allow multiple jet analysis modules to be
// run simultaneosly with various parameters while the Maker loads the events
// and analyses them.  Four different jet analyzers exist:
//
// Konstanin's Analyzers:
//     Kt type: StppKonstKtJetAnalyzer
//     Cone type: StppKonstConeJetAnalyzer
//
// Mike's Analyzers:
//     Kt type: StppMikeKtJetAnalyzer
//     Cone type: StppMikeConeJetAnalyzer
//
// These modules all require the StJetFinder modules.
//
//
// Revision 1.0  2002/09/05 Thomas Henry adapted from Akio Ogawa
//
//////////////////////////////////////////////////////////////////////

//std
#include "Stiostream.h"

//StJetFinder
#include "StJetFinder/StProtoJet.h"

//StSpinMaker
#include "StppEvent.h"
#include "StMuTrackFourVec.h"
#include "StJet.h"
#include "StJets.h"

ClassImp(StJets)
ClassImp(TrackToJetIndex)

int* global_index;

StJets::StJets()
    : mJets( new TClonesArray("StJet",100)), mTrackToJetIndices( new TClonesArray("TrackToJetIndex",200)) 
{
}

StJets::~StJets()
{
    mJets->Delete();
    delete mJets;
    mJets = 0;

    mTrackToJetIndices->Delete();
    delete mTrackToJetIndices;
    mTrackToJetIndices = 0;
} 

void StJets::Clear(bool clearAll)
{
    mJets->Clear();
    mTrackToJetIndices->Clear();
}

void StJets::addProtoJet(StProtoJet& pj)
{
    //jetIndex == number of jets + 1, i.e., where to insert
    int jetIndex = mJets->GetLast()+1;
    
    StProtoJet::FourVecList &trackList = pj.list(); // Get the tracks too.
    
    // We need to add up the charged tracks to get charge and nCell
    int nCell = 0;
    int charge = 0;
    
    for(StProtoJet::FourVecList::iterator it2=trackList.begin(); it2!=trackList.end(); ++it2)  {
	StMuTrackFourVec *track = dynamic_cast<StMuTrackFourVec*>(*it2);
	if (!track) {
	    cout <<"StJets::addProtoJet(). ERROR:\tcast to StMuTrackFourVecFailed.  no action"<<endl;
	    return;
	}
	int muTrackIndex = track->getIndex();
	//cout << "muTrackIndex: " << track->getIndex();
	if (muTrackIndex >=0) {
	    //add to trackToJetIndices
	    int addAt = mTrackToJetIndices->GetLast()+1;
	    new ( (*mTrackToJetIndices)[addAt]) TrackToJetIndex( jetIndex, muTrackIndex);
	    ((TrackToJetIndex*)(*mTrackToJetIndices)[addAt])->setTrackIndex(muTrackIndex);
	    ((TrackToJetIndex*)(*mTrackToJetIndices)[addAt])->setJetIndex(jetIndex);
	}
	if(track->particle())
	    if( track->charge() ) {  // If charge != 0, increment the number of cp
                nCell++;
	}
	if(track->particle())
	    charge += track->particle()->charge();
    }
    //add in the jet container
    new((*mJets)[jetIndex]) StJet( pj.e(), pj.px(), pj.py(), pj.pz(), nCell, charge );
}

void StJets::print()
{
    /*
      for(Int_t i = 0; i < numJets(); i++)
      printf("Jet#%d: Et=%6.3f  Eta=%6.3f  Phi=%6.3f  P=%6.3f  Pt=%6.3f  E=%6.3f  nCell= %4d\n",
      i,et(i),eta(i),phi(i),p(i),pt(i),e(i),nCell(i));
    */
}

//right now it's a linear search, even though the JetsToTrackIndices is ordered by jetIndex
StJets::TrackVec StJets::jetParticles(StppEvent* event, int jetIndex)
{
    TrackVec vec;
    int size = mTrackToJetIndices->GetLast()+1;
    
    TClonesArray& tracks = *(event->tracks);
    Int_t maxNumTracks = event->nPrimTrack;   

    for (int i=0; i<size; ++i) {
	TrackToJetIndex* id = static_cast<TrackToJetIndex*>( (*mTrackToJetIndices)[i] );
	int trackIndex = id->trackIndex();
        if (trackIndex >= maxNumTracks) continue;  // Probably an emc point
	if (id->jetIndex() == jetIndex ) {
	    StMuTrack* track = static_cast<StMuTrack*>( tracks[trackIndex] );
	    vec.push_back( track );
	}
    }
    
    return vec;
}

vector<int> StJets::jetTrackIndices(int jetIndex)
{
  vector<int> vec;
  int size = mTrackToJetIndices->GetLast()+1;
  
  for (int i=0; i<size; ++i) {
    TrackToJetIndex* id = dynamic_cast<TrackToJetIndex*>
      ( mTrackToJetIndices->UncheckedAt(i) );
    if(id) {
      int trackIndex = id->trackIndex();
      if(id->jetIndex() == jetIndex)
	vec.push_back( trackIndex ); }
  }
  
  return vec;
}


bool StJets::inBounds(int i)
{
    return (i>0 && i<nJets());
}

StJet* StJets::jet(int i) 
{
    //this is readable, but fast, optimized compiler will take care of it
    TClonesArray& tj = *mJets;
    TObject* temp = tj[i];
    return ( inBounds(i)==true ) ? static_cast<StJet*>( temp ) : 0;
}

double StJets::e(int i) 
{
    StJet* j = dynamic_cast<StJet*>(mJets->UncheckedAt(i));
    return (j) ? j->E() : -999.;
}

double StJets::et(int i) 
{
    StJet* j = dynamic_cast<StJet*>(mJets->UncheckedAt(i));
    return (j) ? j->et() : -999.;
}

double StJets::p(int i) 
{
    StJet* j = dynamic_cast<StJet*>(mJets->UncheckedAt(i));
    return (j) ? j->P() : -999.;
}

double StJets::pt(int i) 
{
    StJet* j = dynamic_cast<StJet*>(mJets->UncheckedAt(i));
    return (j) ? j->Pt() : -999.;
}

double StJets::phi(int i)
{
    StJet* j = dynamic_cast<StJet*>(mJets->UncheckedAt(i));
    return (j) ? j->Phi() : -999.;
}

double StJets::eta(int i) 
{
    StJet* j = dynamic_cast<StJet*>(mJets->UncheckedAt(i));
    return (j) ? j->Eta() : -999.;
}

int StJets::nCell(int i) 
{
    StJet* j = dynamic_cast<StJet*>(mJets->UncheckedAt(i));
    return (j) ? j->nCell : -999;
}

int StJets::charge(int i) 
{
    StJet* j = dynamic_cast<StJet*>(mJets->UncheckedAt(i));
    return (j) ? j->charge : -999;
}
