// $Id: StTGeoHelper.h,v 1.3 2009/08/29 21:23:12 perev Exp $
//
//
// Class StTGeoHelper
// ------------------


#ifndef ST_TGEOHELPER_H
#define ST_TGEOHELPER_H

#include "TString.h"
#include "TObject.h"
class TObjArray;
class TGeoNavigator;
class TGeoNode;
class TGeoVolume;
class TGeoMedium;
class TGeoMaterial;
class StVoluExt;
class StTGeoHitShape;
class StGenHitPlane;

class StTGeoHelper : public TObject
{
         StTGeoHelper();
virtual ~StTGeoHelper();

public:
static	StTGeoHelper *Instance(); 

public:
	void  Print(const char *tit=0) const;
	void  SetModule  (const char *voluName);
	void  SetModule  (int   moduLevl=2) {fModuLev = moduLevl;}
	 int  IsModule   (const TGeoVolume *volu) 	const;
	 int  IsModule   (const TGeoNode   *node) 	const;
	 int  IsHidule (const TGeoVolume *volu) 	const;
	 int  IsHitPlane (const TGeoVolume *volu) 	const;
	 int  IsHitPlane (const TGeoNode   *node) 	const;
	 int  MayHitPlane(const TGeoVolume *volu)       const;
	 int  MakeHitPlane(const TGeoVolume *volu)           ;
     TString  FullPath() const;     
static	 int  IsSensitive(const TGeoVolume *volu) 	     ;

        void  ShootZR(double z,double rxy);

const StTGeoHitShape* GetHitShape() const {return fHitShape;}


	void  SetExt     (StVoluExt *ext);
   StVoluExt *GetExt     (int idx) 			const;
	void  SetPlane   (StGenHitPlane *pla);
const TGeoVolume *GetModule() const;
       void Init();
static void Test();
private:
char fBeg[1];
int fModuLev;
TObjArray      *fExtArr;
TObjArray      *fPlaArr;
StTGeoHitShape *fHitShape;
char fEnd[1];
ClassDef(StTGeoHelper,0) //
};

class StVoluExt : public TObject
{
enum E_StVoluExt {
      kIsModule      = BIT(15),   // The volume is a module, like TPCE,SVTT
      kHitPlane      = BIT(16),   // The volume is a Hit plane
      kHidule      = BIT(17)    // The volume is a Module with Hits 
};
public:
enum E_Kind { kVoluExt,kBaseHitPlane };

   	 StVoluExt(int voluNumber)	{SetUniqueID(voluNumber)  ;}
virtual ~StVoluExt(){;}
	int IsModule  ()   	const 	{return TestBit(kIsModule);}
	int IsHitPlane() 	const 	{return TestBit(kHitPlane);}
	int IsHidule() 	const 	{return TestBit(kHidule);}
       void SetModule   (int s=1)       {SetBit(kIsModule,s)      ;}
       void SetHitPlane (int s=1)     	{SetBit(kHitPlane,s)      ;}
       void SetHidule (int s=1)     	{SetBit(kHidule,s)      ;}
        int GetNumber() const 		{return GetUniqueID()     ;}
const  TGeoVolume* GetVolu() const; 
virtual int Kind() const		{return kVoluExt;}
public:
ClassDef(StVoluExt,0) //
};



class StGenHitPlane : public StVoluExt
{
friend class StTGeoHelper;
public:
      StGenHitPlane(int volId);	
virtual ~StGenHitPlane(){;}
void  operator=(const StVoluExt& ext){*((StVoluExt*)this)=ext;}
      int Kind() const		{return kBaseHitPlane;}
const double *GetDir() const	{return fDir;}
const double *GetOrg() const	{return fOrg;}

protected:
double fOrg[3];
double fDir[3];
};



class StTGeoIterator
{
public:
                  StTGeoIterator();
                 ~StTGeoIterator();
StTGeoIterator   &operator++();
const TGeoVolume *operator*() const {return fVolu;}
StTGeoIterator   &Next();
StTGeoIterator   &Down();
StTGeoIterator   &Upp();

const TGeoNode   *GetNode(int idx=0) const;
const TGeoVolume *GetVolu(int idx=0) const;
             int  GetLev() const {return fLev;}
             int  GetIdx() const {return (fLev)? fStk[fLev-1]:0;}
const        int *GetStk() const {return fStk;}
             int  IsFirst()const {return fNow ==1;}
             int  IsLast ()const {return fNow ==2;}
             int  IsEnd()  const {return fVolu==0;}
             int  State()  const {return fNow;}
            void  LocalToMaster(const double* local, double* master) const;
            void  Print(const char *tit="") const;
         TString  FullPath() const {return FullPath(fNavi);}     
static TString FullPath(const TGeoNavigator *nav);     
private:

private:
	int  fNow; // 1=first visit,2=last visit,3=first and last visit        	
	int  fKase;
TGeoVolume    *fVolu;
TGeoNavigator *fNavi;
	int  fLev;
	int  fStk[100];
    
};
class StTGeoHitShape
{
public:
     StTGeoHitShape(double zMin,double zMax);
void Update(double z1, double z2, double rxy);
int  Inside(double z,double rxy) const;
private:
enum {kNZ=100};
double fZMin;
double fZMax;
double fRxy[kNZ];
};
#endif //ST_TGEOHELPER_H   
   
