/*!
 * \class StMtdCollection 
 */
/***************************************************************************
 *
 * $Id: StMtdCollection.h,v 2.1 2011/04/25 21:24:02 ullrich Exp $
 *
 * Author: Frank Geurts, April 25, 2011
 ***************************************************************************
 *
 * Description:
 *
 * Persistent data which is written into StEvent
 * directly from the reco chain.
 *
 ***************************************************************************
 *
 * $Log: StMtdCollection.h,v $
 * Revision 2.1  2011/04/25 21:24:02  ullrich
 * Initial Revision.
 *
 *
 **************************************************************************/
#ifndef StMtdCollection_hh
#define StMtdCollection_hh

#include "StObject.h"
#include "StContainers.h"
#include "StEnumerations.h"
#include "StMtdHeader.h"
#include "StMtdHit.h"
#include "StMtdRawHit.h"

class StMtdCollection : public StObject {
public: 
    StMtdCollection();
    ~StMtdCollection();

    const StMtdHeader*         mtdHeader() const;
    StMtdHeader*               mtdHeader();

    const StSPtrVecMtdHit&     mtdHits() const;
    StSPtrVecMtdHit&           mtdHits();

    const StSPtrVecMtdRawHit&  mtdRawHits() const;
    StSPtrVecMtdRawHit&        mtdRawHits();

    void setHeader(StMtdHeader*);

    void addHit(const StMtdHit*);
    void addRawHit(const StMtdRawHit*);

    bool hitsPresent()     const;
    bool rawHitsPresent()  const;
    
private:
    StMtdHeader*               mMtdHeader;

    StSPtrVecMtdHit            mMtdHits;
    StSPtrVecMtdRawHit         mMtdRawHits;
  
    ClassDef(StMtdCollection, 1)
};

#endif
