//:Copyright 1996, Lawrence Berkeley National Laboratory
//:>--------------------------------------------------------------------
//:FILE:        tbrClasses.C
//:DESCRIPTION: TBR Classes
//:AUTHOR:      cet - Craig E. Tull, cetull@lbl.gov
//:BUGS:        -- STILL IN DEVELOPMENT --
//:HISTORY:     11mar96-v000a-cet,hjw- creation
//:<--------------------------------------------------------------------

//:----------------------------------------------- INCLUDES           --
#include "emlLib.h"
#include "duiLib.h"
#include "tbr_macros.h"
#include "tbrClasses.hh"

//:----------------------------------------------- MACROS             --
//:----------------------------------------------- PROTOTYPES         --
extern CC_P void writeTheVisualization(DS_DATASET_T *pDataset);
extern CC_P void tbrNewDSView(DS_DATASET_T **ppDS, long np);
extern CC_P void tbrNewTbView(DS_DATASET_T **ppDS);
extern CC_P int dsPrintDatasetSpecifier(FILE *stream
		, DS_DATASET_T *pDataset);
extern CC_P void dsPrintTableData(FILE *stream, DS_DATASET_T *table);

//:####################################################################
//:=============================================== CLASS              ==
// tbrMotifViewer

//:----------------------------------------------- CTORS & DTOR       --
tbrMotifViewer:: tbrMotifViewer(const char * name)
		: socObject(name, "tbrMotifViewer") {
   myPtr = (SOC_PTR_T)this;
}

//----------------------------------
tbrMotifViewer:: ~tbrMotifViewer() { }

//:----------------------------------------------- ATTRIBUTES         --

//:----------------------------------------------- PUB FUNCTIONS      --
STAFCV_T tbrMotifViewer:: writeVisualization ( tdmDataset * dataset ) {

   DS_DATASET_T *pDataset=NULL;
   long ptr;

   dataset->cvtDslPointer(ptr);
   pDataset = (DS_DATASET_T*)ptr;

   writeTheVisualization(pDataset);
   EML_SUCCESS(STAFCV_OK);
}
STAFCV_T tbrMotifViewer:: viewDataset ( tdmDataset * dataset) {

   DS_DATASET_T *pDataset=NULL;
   long ptr;

   dataset->cvtDslPointer(ptr);
   pDataset = (DS_DATASET_T*)ptr;

   tbrNewDSView(&pDataset,1);
   EML_SUCCESS(STAFCV_OK);
}

//----------------------------------
STAFCV_T tbrMotifViewer:: viewTable ( tdmTable * table) {

   DS_DATASET_T *pTable=NULL;
   long ptr;

   table->cvtDslPointer(ptr);
   pTable = (DS_DATASET_T*)ptr;

   tbrNewTbView(&pTable);
   EML_SUCCESS(STAFCV_OK);
}


//:----------------------------------------------- PRIV FUNCTIONS     --
// **NONE**

//:####################################################################
//:=============================================== CLASS              ==
// tbrFactory
//:----------------------------------------------- CTORS & DTOR       --
tbrFactory:: tbrFactory(const char * name)
		: socObject(name, "tbrFactory") {
   myPtr = (SOC_PTR_T)this;
}

//----------------------------------
tbrFactory:: ~tbrFactory() { }

//:----------------------------------------------- ATTRIBUTES         --
//:**NONE**
//:----------------------------------------------- PUB FUNCTIONS      --
STAFCV_T tbrFactory:: deleteMotifViewer (const char * name) {
   EML_ERROR(NOT_YET_IMPLEMENTED);
}

//----------------------------------
STAFCV_T tbrFactory:: findMotifViewer (IDREF_T id
		, tbrMotifViewer*& viewer) {
   EML_ERROR(NOT_YET_IMPLEMENTED);
}

//----------------------------------
STAFCV_T tbrFactory:: getMotifViewer (const char * name
		, tbrMotifViewer*& viewer) {
   EML_ERROR(NOT_YET_IMPLEMENTED);
}

//----------------------------------
STAFCV_T tbrFactory:: newMotifViewer (const char * name) {
   EML_ERROR(NOT_YET_IMPLEMENTED);
}

//:----------------------------------------------- PRIV FUNCTIONS     --
// **NONE**

// ---------------------------------------------------------------------

