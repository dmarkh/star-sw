//******************************************************************************

/*! \class StEmcDBHandler
    \author Marcia Maria de Moura
		
		  2003/01/02
		  Class to retrieve Emc database tables
*/		
//******************************************************************************

#include "StEmcDbHandler.h"
#include "TUnixTime.h"
#include "StMessMgr.h"

#include "TFile.h"
#include "TTable.h"
#include "TSystem.h"
#include "TKey.h"

ClassImp(StEmcDbHandler)

//------------------------------------------------------------------------------
/*! Class constructor */
StEmcDbHandler::StEmcDbHandler()
{
  // Default table node is Calibrations_emc (see .h file) 
  setTableNode();
  setTimeStamp();
  setMaxEntryTime();
  setFlavor();
}
//------------------------------------------------------------------------------
/*! Class destructor */
StEmcDbHandler::~StEmcDbHandler()
{
}
//------------------------------------------------------------------------------
/*! Retrieving of specified database Emc Table */
StDbTable* StEmcDbHandler::getDbTable()
{
  StDbManager* dbMngr = StDbManager::Instance();
  StDbTable* table = (dbMngr->initConfig(mTableNode.c_str()))->addDbTable(mTableName.c_str());
  
  //set beginTime
  dbMngr->setRequestTime(mTimeStamp.c_str());

  //set flavor
  table->setFlavor(mFlavor.c_str());

  //set maxEntryTime
  UInt_t unixEntryTime = (dbMngr->findDb(mTableNode.c_str()))->getUnixTime(mMaxTime.c_str());
  table->setProdTime(unixEntryTime);

  dbMngr->fetchDbTable(table);

  mTimeStamp = "";//getSqlTime(table->getBeginDateTime()); 
  mTable = table;
  
  return mTable;  
}
//------------------------------------------------------------------------------
/*! Return time stamp list of an EMC table within specified limits*/
std::vector<std::string> StEmcDbHandler::getTimeStampList(const char * beginTime, const char * endTime)
{
    mTimeStampList.clear();
    
    StDbManager* dbMngr = StDbManager::Instance();
    StDbTable* table = (dbMngr->initConfig(mTableNode.c_str()))->addDbTable(mTableName.c_str());
    dbMngr->setStoreTime(mMaxTime.c_str());
    unsigned int unixStoreTime = dbMngr->getUnixStoreTime();
    StDataBaseI* db=dbMngr->findDb(mTableNode.c_str());
    
    char whereClause[256];    
    sprintf(whereClause, " where flavor='%s' and beginTime>='%s' and beginTime<='%s' and entryTime<='%s' and (deactive=0 or deactive>=%d) order by beginTime", 
            mFlavor.c_str(), beginTime, endTime, mMaxTime.c_str(), unixStoreTime);
    LOG_INFO <<"::getTimeStampList() using query = "<<whereClause<<endm;
    
    UInt_t* timeStampList;
    timeStampList = db->QueryDbTimes(table,whereClause,1);  // opt=1 means don't get any data!
    
    int nRows=table->GetNRows();
    int nRows2;
    table->getElementID(nRows2);
    if (nRows != nRows2) {
        LOG_WARN<<"problem:  nRows!=nRows2"<<endm;
    }
    
    for (int i = 0; i < nRows ; i++)
    {
      dbMngr->setRequestTime(timeStampList[i]); //let db translate to Human Readable
      mTimeStampList.push_back(""/*getSqlTime(dbMngr->getDateRequestTime())*/);
    }
    
    delete [] timeStampList;
    
    return mTimeStampList;
}

//------------------------------------------------------------------------------

std::string & StEmcDbHandler::timeToSqlTime(const char* apiTime) {
    TString string=apiTime;
    TString dateString=string(0,4)+ "-" +string(4,2)+ "-" + string(6,2);
    TString timeString=string(8,2)+ ":" +string(10,2)+ ":" + string(12,2);
    TString datetime = dateString+ " " +timeString;
    //cout << " datetime.c_str() = " << datetime.c_str() << endl;
    std::string * s = new std::string(datetime.Data());
    return *s;
}

void StEmcDbHandler::writeToDb(char* data) {
    StDbManager *dbMngr = StDbManager::Instance();
    StDbTable* table = (dbMngr->initConfig(mTableNode.c_str()))->addDbTable(mTableName.c_str());
    table->setFlavor(mFlavor.c_str());
    table->SetTable(data,1);
    dbMngr->setStoreTime(mTimeStamp.c_str());
    dbMngr->storeDbTable(table);
}
