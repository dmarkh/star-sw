// *-- Author : Jan Balewski
// 
// $Id: StEEtowerExampleMaker.cxx,v 1.1 2004/06/06 04:54:08 balewski Exp $

#include <TFile.h>
#include <TH2.h>

#include "StEEtowerExampleMaker.h"

#include "StMuDSTMaker/COMMON/StMuEvent.h"
#include "StMuDSTMaker/COMMON/StMuDst.h"
#include "StMuDSTMaker/COMMON/StMuDstMaker.h"

#include <StMessMgr.h>

#include "StEEmcDbMaker/EEmcDbItem.h"
#include "StEEmcDbMaker/StEEmcDbMaker.h"


ClassImp(StEEtowerExampleMaker)

//________________________________________________
//________________________________________________
StEEtowerExampleMaker::StEEtowerExampleMaker(const char* self ,const char* muDstMakerName) : StMaker(self){
  mMuDstMaker = (StMuDstMaker*)GetMaker(muDstMakerName);
  assert(mMuDstMaker);
}


//___________________ _____________________________
//________________________________________________
StEEtowerExampleMaker::~StEEtowerExampleMaker(){
}

 
//________________________________________________
//________________________________________________
Int_t StEEtowerExampleMaker::Init(){
  eeDb=(EEDB*)GetMaker("eemcDb");
  EEtower::init();
  return StMaker::Init();
}

//________________________________________________
//________________________________________________
Int_t StEEtowerExampleMaker::Finish(){
  finish();
  return kStOK;
}
 
 
 

//________________________________________________
//________________________________________________
Int_t StEEtowerExampleMaker::Make(){

  EEtower::clear();

  // ............. acuire EEMC data 
  if(unpackMuDst()<0)   return kStOK;

  //print();
  task1(); // do real analysis

  return kStOK;
}


//________________________________________________
//________________________________________________
Int_t StEEtowerExampleMaker::unpackMuDst(){
  nInpEve++;
  gMessMgr->Message("","D") <<GetName()<<"::::getAdc() is called "<<endm;

  // Access to muDst .......................
  StMuEmcCollection* emc = mMuDstMaker->muDst()->emcCollection();
  if (!emc) {
    gMessMgr->Message("","W") <<"No EMC data for this event"<<endm;    return kStOK;
  }
    
  int i, n1=0;

  int sec,eta,sub,adc;
  for (i=0; i< emc->getNEndcapTowerADC(); i++) {
    emc->getEndcapTowerADC(i,adc,sec,sub,eta);
    assert(sec>0);// total corruption of muDst
    
    const EEmcDbItem *x=eeDb->getTile(sec,'A'+sub-1,eta,'T');
    if(x==0) continue; // it should never happened for muDst
    if(x->fail ) continue; // drop broken channels
    if(adc <x->thr) continue;// value must be >ped+ N*ped_sig
    if(x->gain<=0 ) continue; // gains not avaliable

    n1++;
    float value=(adc-x->ped)/x->gain;
    int iphi=(x->sec-1)*5+(x->sub-'A');
    int ieta=x->eta-1;
    assert(iphi>=0 && iphi<MaxPhiBins);
    assert(ieta>=0 && ieta<MaxEtaBins);
    
    towerE[ieta][iphi]=value;
    // if(value>0) printf(" %d %d %f\n",ieta,iphi,value);
  }

  gMessMgr->Message("","I") <<GetName()<<"::::getAdc()  found "<<n1<<" ADC>thres "<<endm;
  return n1;
}


// $Log: StEEtowerExampleMaker.cxx,v $
// Revision 1.1  2004/06/06 04:54:08  balewski
// dual analyzis
//
