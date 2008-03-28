/*!\class StEmcDecoder
\author Alexandre A. P. Suaide
 
This class makes the decodification from EMC daq
and electronics scheme to software scheme. This class
has methods to decode the numbers in both directions and
it works for SMD and towers.<br><br>
 
The current id's definitions are:<br>
      - for Towers
            - 0 <= RDO (daq_id)            <= 4799
            - 1 <= Crate                   <= 30
            - 0 <= Crate_sequency          <= 159
            - 1 <= crate board             <= 5
            - 0 <= position on board       <= 31
            - 0 <= TDC                     <= 29
            - 0 <= TDC channels            <= 29
            - 0 <= tdc sequency on channel <= 159
            - 1 <= Tower software id       <= 4800       
      - for SMD
            - 0 <= RDO             <= 7
            - 0 <= index           <= 4799
            
*/
#include <Stiostream.h>
#include "Stiostream.h"

#ifndef StEmcDecoder_HH
#define StEmcDecoder_HH

class StEmcDecoder
{
protected:

    int       Init_Crate[30];
    int       TDC_Crate[30];
    int       Crate_TDC[30];
    int       ReverseOrder[4800];
    int       TriggerPatch[30];
    int       TriggerSequence[10];
    int       TowerBugFixIndex[4800];
    int       PreshowerBugFixIndex[4800];
    bool      fixTowerMap;
    bool      fixPreshowerMap;

    int       JetPatchFromTriggerPatch[300];
    int       JetPatchSeqFromTriggerPatch[300];
    int       TriggerPatchFromJetPatchAndSeq[300];


    int       SmdModules[8][15];
    int       FEE1[4],FEE2[4],FEE3[4];
    int       connector1[20],connector2[20],connector3[20];
    int       SmdeRDO[120][150],SmdpRDO[120][10][15];
    int       SmdeIndex[120][150],SmdpIndex[120][10][15];

    int       PsdModules[4][15];
    int       PsdOffset[40];
    int       PsdStart[60];
    int       PsdRDO[4800];
    int       PsdIndex[4800];

    int       Getjose_towerWest(int,int) const;///<Get Software Id for West size for towers
    int       Getjose_towerEast(int,int) const;///<Get Software Id for East side for towers
    int       getSmdModule(int,int,int&) const;///<Get SMD module in a given position in RDO
    int       checkDummy(int) const;///<Check dummy positions on SMD crate
    int       getSmdPin(int,int,int,int&) const;///<Get SMD pin number
    int       getSmdpStrip(int,int&,int&) const;///<Get SMDP strip
    void      fixTowerBugIndexes();///<fixes the array in order to correct the tower bug
    void      fixPreshowerBugIndexes();///<fixes the array in order to correct the preshower mapping

    void      Init(unsigned int,unsigned int);///< Init method

public:
    StEmcDecoder(unsigned int date=20300101,unsigned int time=000000, bool TowerMapBug = false);///< StEmcDecoder constructor
    virtual   ~StEmcDecoder();///< StEmcDecoder destructor

    void      SetDateTime(unsigned int date, unsigned int time);///< Sets the timestamp to be used
    bool      GetFixTowerMapBug(void) const;///< Shows if the tower map bug is being fixed on the fly
    void      SetFixTowerMapBug(bool fix);///< Sets the tower map bug fix

    int       GetTowerBugCorrectionShift(int id_original, int &shift) const;///<Returns the index shift for the tower in original map
    int       GetPreshowerBugCorrectionShift(int id_original, int &shift) const;///<Returns the index shift for the preshower in original map

	//methods to get another basis from softId
    int       GetTowerBin(int softId,int &m,int &e,int &s) const;///<Convert from softId to m,e,s for towers only
    int       GetDaqIdFromTowerId(int softId,int &RDO) const;///< Get Daq Id from Software Id for towers
	int       GetCrateFromTowerId(int softId, int &crate, int &sequence) const;///<Get crate number and position in crate for Software Id
	int       GetTDCFromTowerId(int softId, int &TDC) const;///<Get TDC channel from Software Id
	int       GetTriggerPatchFromTowerId(int softId, int &patchId) const;///<Get trigger patch that contains software Id
	int       GetJetPatchFromTowerId(int softId, int &jetPatch) const;///<Get jet patch that contains software Id
	
	//methods to get softId from another basis
	int       GetTowerIdFromBin(int m, int e, int s, int &softId) const;///<Get Software Id from (m,e,s) space
	int       GetTowerIdFromDaqId(int RDO, int &softId) const;///<Get Sofwtare Id from Daq Id for towers
    int       GetTowerIdFromCrate(int crate, int sequence, int &softId) const;///<Get Software Id from Crate number and position in crate for towers
    int       GetTowerIdFromTDC(int TDC, int sequence, int &softId) const;///<Get Software Id from TDC channel number and position in TDC for towers
	
	//additional RDO <-> TDC <-> crate translations
    int       GetTowerCrateFromDaqId(int RDO,int &crate, int &sequence) const;///< Get crate number from Daq Id for towers
    int       GetTowerCrateFromTDC(int TDC, int &crate) const;///<Get crate number from TDC channel for towers
    int       GetTowerTDCFromCrate(int crate, int &TDC) const;///< Get TDC channel from crate number for towers
    int       GetTowerTDCFromDaqId(int RDO, int &TDC) const;///< Get TDC channel from Daq Id for towers

	//additional trigger patch <-> jet patch <-> crate translations
    int       GetTriggerPatchFromCrate(int crate, int sequence, int &patchId) const; ///< returns the trigger patch from crate and sequence in the crate
    int       GetCrateAndSequenceFromTriggerPatch(int patchId, int &crate, int &sequence) const; ///< returns the crate number and start point for a given trigger patch
    int       GetTriggerPatchFromJetPatch(int jetPatch, int sequence, int &patchId) const; ///< returns the trigger patch from big jet patch and the sequence in it
    int       GetJetPatchAndSequenceFromTriggerPatch(int patchId, int &jetPatch, int &sequence) const; ///< return the big jet patch and sequence number within it from the
    
    /// dsmModule is set to the DSM module containing this trigger patch (0-299)
    int       GetDSMFromTriggerPatch(int patchId, int &dsmModule) const;
    
    /// triggerPatches is an int[10]; contents will be set to the TPs of the supplied DSM #
    int       GetTriggerPatchesFromDSM(int dsmModule, int *triggerPatches) const;

	//smd methods
    int       GetSmdCoord(int RDO, int posInFiber, int &det, int &m, int &e, int &s, bool print=false) const;///<Get SMD detector (3==SMDE, 4==SMDP), m, e, s from RDO and position for SMD
    int       GetSmdCoord(int RDO, int posInFiber, int &det, int &m, int &e, int &s, int &wire, int &A_value, bool print=false) const;///<Get SMD detector (3==SMDE, 4==SMDP), m, e, s from RDO and position for SMD. Also returns smd-wire and A_value
    int       GetSmdRDO(int detector, int m, int e, int s, int &RDO, int &posInFiber) const;///<Get SMD fiber and position from detector number (3==SMDE, 4==SMDP), m, e, s

	//psd methods
    int       GetPsdId(int RDO, int posInFiber, int &softId,bool print=false) const;///<Get PSD id
    int       GetPsdId(int RDO, int posInFiber, int &softId, int &PMTbox, int &wire, int &A_value, bool print=false) const;///<Get PSD id. Also returns PMTBox,  wire number and FEE value
    int       GetPsdRDO(int softId, int &RDO, int &posInFiber) const;///<Get PSD fiber and position from id

    void      PrintTowerMap(ofstream *out) const;///<Print Tower MAP
    void      PrintSmdMap(ofstream *out) const;///<Print SMD MAP
    void      PrintPsdMap(ofstream *out) const;///<Print SMD MAP
};
#endif

// $Id: StEmcDecoder.h,v 2.17 2007/10/09 18:02:24 kocolosk Exp $
//
// $Log: StEmcDecoder.h,v $
// Revision 2.17  2007/10/09 18:02:24  kocolosk
// two extra support functions for TP <=> DSM module mapping
//
// Revision 2.16  2007/09/11 02:41:37  kocolosk
// added code to fix preshower swaps in 2006 and beyond
//
// Revision 2.15  2007/04/04 17:35:12  kocolosk
// Added methods GetCrateFromTowerId, GetTDCFromTowerId, GetTDCFromTowerId, GetTriggerPatchFromTowerId, GetJetPatchFromTowerId, and GetTowerIdFromBin.  Also implemented const-correctness and used meaningful argument names in method declarations to improve readability.
//
