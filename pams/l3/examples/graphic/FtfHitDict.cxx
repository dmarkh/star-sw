//
// File generated by rootcint at Fri May 19 13:19:33 2000.
// Do NOT change. Changes will be lost next time file is generated
//
#include "FtfHitDict.h"

#include "TBuffer.h"
#include "TMemberInspector.h"
#include "TError.h"

//______________________________________________________________________________
TBuffer &operator>>(TBuffer &buf, FtfHit *&obj)
{
   // Read a pointer to an object of class FtfHit.

   ::Error("FtfHit::operator>>", "objects not inheriting from TObject need a specialized operator>> function"); if (obj) { }
   return buf;
}

//______________________________________________________________________________
void FtfHit::Streamer(TBuffer &R__b)
{
   // Stream an object of class FtfHit.

   if (R__b.IsReading()) {
      Version_t R__v = R__b.ReadVersion(); if (R__v) { }
      FtfBaseHit::Streamer(R__b);
      R__b >> id;
      R__b >> phiIndex;
      R__b >> etaIndex;
      R__b >> flags;
      R__b >> sector;
      //R__b.ReadArray(nextVolumeHit);
      //R__b.ReadArray(nextRowHit);
      R__b >> r;
      R__b >> phi;
      R__b >> dphi;
      R__b >> eta;
      R__b >> xp;
      R__b >> yp;
      R__b >> buffer1;
      R__b >> buffer2;
      R__b >> hardwareId;
   } else {
      R__b.WriteVersion(FtfHit::IsA());
      FtfBaseHit::Streamer(R__b);
      R__b << id;
      R__b << phiIndex;
      R__b << etaIndex;
      R__b << flags;
      R__b << sector;
      //R__b.WriteArray(nextVolumeHit, __COUNTER__);
      //R__b.WriteArray(nextRowHit, __COUNTER__);
      R__b << r;
      R__b << phi;
      R__b << dphi;
      R__b << eta;
      R__b << xp;
      R__b << yp;
      R__b << buffer1;
      R__b << buffer2;
      R__b << hardwareId;
   }
}

//______________________________________________________________________________
void FtfHit::ShowMembers(TMemberInspector &R__insp, char *R__parent)
{
   // Inspect the data members of an object of class FtfHit.

   TClass *R__cl  = FtfHit::IsA();
   Int_t   R__ncp = strlen(R__parent);
   if (R__ncp || R__cl || R__insp.IsA()) { }
   R__insp.Inspect(R__cl, R__parent, "id", &id);
   R__insp.Inspect(R__cl, R__parent, "phiIndex", &phiIndex);
   R__insp.Inspect(R__cl, R__parent, "etaIndex", &etaIndex);
   R__insp.Inspect(R__cl, R__parent, "flags", &flags);
   R__insp.Inspect(R__cl, R__parent, "sector", &sector);
   R__insp.Inspect(R__cl, R__parent, "*nextVolumeHit", &nextVolumeHit);
   R__insp.Inspect(R__cl, R__parent, "*nextRowHit", &nextRowHit);
   R__insp.Inspect(R__cl, R__parent, "r", &r);
   R__insp.Inspect(R__cl, R__parent, "phi", &phi);
   R__insp.Inspect(R__cl, R__parent, "dphi", &dphi);
   R__insp.Inspect(R__cl, R__parent, "eta", &eta);
   R__insp.Inspect(R__cl, R__parent, "xp", &xp);
   R__insp.Inspect(R__cl, R__parent, "yp", &yp);
   R__insp.Inspect(R__cl, R__parent, "buffer1", &buffer1);
   R__insp.Inspect(R__cl, R__parent, "buffer2", &buffer2);
   R__insp.Inspect(R__cl, R__parent, "hardwareId", &hardwareId);
   FtfBaseHit::ShowMembers(R__insp, R__parent);
}

/********************************************************
* FtfHitDict.cxx
********************************************************/

#ifdef G__MEMTEST
#undef malloc
#endif

extern "C" void G__cpp_reset_tagtableFtfHitDict();

extern "C" void G__set_cpp_environmentFtfHitDict() {
  G__add_compiledheader("TROOT.h");
  G__add_compiledheader("TMemberInspector.h");
  G__add_compiledheader("../inc/FtfHit.h");
  G__cpp_reset_tagtableFtfHitDict();
}
extern "C" int G__cpp_dllrevFtfHitDict() { return(51111); }

/*********************************************************
* Member function Interface Method
*********************************************************/

/* FtfHit */
static int G__FtfHit_printLinks_0_0(G__value *result7,G__CONST char *funcname,struct G__param *libp,int hash) {
      G__setnull(result7);
      ((FtfHit*)(G__getstructoffset()))->printLinks();
   return(1 || funcname || hash || result7 || libp) ;
}

static int G__FtfHit_printLinks_1_0(G__value *result7,G__CONST char *funcname,struct G__param *libp,int hash) {
      G__setnull(result7);
      ((FtfHit*)(G__getstructoffset()))->printLinks((int)G__int(libp->para[0]));
   return(1 || funcname || hash || result7 || libp) ;
}

static int G__FtfHit_setStatus_2_0(G__value *result7,G__CONST char *funcname,struct G__param *libp,int hash) {
      G__setnull(result7);
      ((FtfHit*)(G__getstructoffset()))->setStatus((FtfTrack*)G__int(libp->para[0]));
   return(1 || funcname || hash || result7 || libp) ;
}

static int G__FtfHit_DeclFileName_3_0(G__value *result7,G__CONST char *funcname,struct G__param *libp,int hash) {
      G__letint(result7,67,(long)((FtfHit*)(G__getstructoffset()))->DeclFileName());
   return(1 || funcname || hash || result7 || libp) ;
}

static int G__FtfHit_DeclFileLine_4_0(G__value *result7,G__CONST char *funcname,struct G__param *libp,int hash) {
      G__letint(result7,105,(long)((FtfHit*)(G__getstructoffset()))->DeclFileLine());
   return(1 || funcname || hash || result7 || libp) ;
}

static int G__FtfHit_ImplFileName_5_0(G__value *result7,G__CONST char *funcname,struct G__param *libp,int hash) {
      G__letint(result7,67,(long)((FtfHit*)(G__getstructoffset()))->ImplFileName());
   return(1 || funcname || hash || result7 || libp) ;
}

static int G__FtfHit_ImplFileLine_6_0(G__value *result7,G__CONST char *funcname,struct G__param *libp,int hash) {
      G__letint(result7,105,(long)((FtfHit*)(G__getstructoffset()))->ImplFileLine());
   return(1 || funcname || hash || result7 || libp) ;
}

static int G__FtfHit_Class_Version_7_0(G__value *result7,G__CONST char *funcname,struct G__param *libp,int hash) {
      G__letint(result7,115,(long)((FtfHit*)(G__getstructoffset()))->Class_Version());
   return(1 || funcname || hash || result7 || libp) ;
}

static int G__FtfHit_Class_8_0(G__value *result7,G__CONST char *funcname,struct G__param *libp,int hash) {
      G__letint(result7,85,(long)((FtfHit*)(G__getstructoffset()))->Class());
   return(1 || funcname || hash || result7 || libp) ;
}

static int G__FtfHit_IsA_9_0(G__value *result7,G__CONST char *funcname,struct G__param *libp,int hash) {
      G__letint(result7,85,(long)((const FtfHit*)(G__getstructoffset()))->IsA());
   return(1 || funcname || hash || result7 || libp) ;
}

static int G__FtfHit_ShowMembers_0_1(G__value *result7,G__CONST char *funcname,struct G__param *libp,int hash) {
      G__setnull(result7);
      ((FtfHit*)(G__getstructoffset()))->ShowMembers(*(TMemberInspector*)libp->para[0].ref,(char*)G__int(libp->para[1]));
   return(1 || funcname || hash || result7 || libp) ;
}

static int G__FtfHit_Streamer_1_1(G__value *result7,G__CONST char *funcname,struct G__param *libp,int hash) {
      G__setnull(result7);
      ((FtfHit*)(G__getstructoffset()))->Streamer(*(TBuffer*)libp->para[0].ref);
   return(1 || funcname || hash || result7 || libp) ;
}

static int G__FtfHit_Dictionary_2_1(G__value *result7,G__CONST char *funcname,struct G__param *libp,int hash) {
      G__setnull(result7);
      ((FtfHit*)(G__getstructoffset()))->Dictionary();
   return(1 || funcname || hash || result7 || libp) ;
}

// automatic default constructor
static int G__FtfHit_FtfHit_3_1(G__value *result7,G__CONST char *funcname,struct G__param *libp,int hash) {
   FtfHit *p;
   if(0!=libp->paran) ;
   if(G__getaryconstruct()) p=new FtfHit[G__getaryconstruct()];
   else                    p=new FtfHit;
   result7->obj.i = (long)p;
   result7->ref = (long)p;
   result7->type = 'u';
   result7->tagnum = G__get_linked_tagnum(&G__FtfHitDictLN_FtfHit);
   return(1 || funcname || hash || result7 || libp) ;
}

// automatic copy constructor
static int G__FtfHit_FtfHit_4_1(G__value *result7,G__CONST char *funcname,struct G__param *libp,int hash)
{
   FtfHit *p;
   p=new FtfHit(*(FtfHit*)G__int(libp->para[0]));
   result7->obj.i = (long)p;
   result7->ref = (long)p;
   result7->type = 'u';
   result7->tagnum = G__get_linked_tagnum(&G__FtfHitDictLN_FtfHit);
   return(1 || funcname || hash || result7 || libp) ;
}

// automatic destructor
static int G__FtfHit_wAFtfHit_5_1(G__value *result7,G__CONST char *funcname,struct G__param *libp,int hash) {
   if(G__getaryconstruct())
     if(G__PVOID==G__getgvp())
       delete[] (FtfHit *)(G__getstructoffset());
     else
       for(int i=G__getaryconstruct()-1;i>=0;i--)
         delete (FtfHit *)((G__getstructoffset())+sizeof(FtfHit)*i);
   else  delete (FtfHit *)(G__getstructoffset());
      G__setnull(result7);
   return(1 || funcname || hash || result7 || libp) ;
}


/* Setting up global function */

/*********************************************************
* Member function Stub
*********************************************************/

/* FtfHit */

/*********************************************************
* Global function Stub
*********************************************************/

/*********************************************************
* Get size of pointer to member function
*********************************************************/
class G__Sizep2memfuncFtfHitDict {
 public:
  G__Sizep2memfuncFtfHitDict() {p=&G__Sizep2memfuncFtfHitDict::sizep2memfunc;}
    size_t sizep2memfunc() { return(sizeof(p)); }
  private:
    size_t (G__Sizep2memfuncFtfHitDict::*p)();
};

size_t G__get_sizep2memfuncFtfHitDict()
{
  G__Sizep2memfuncFtfHitDict a;
  G__setsizep2memfunc((int)a.sizep2memfunc());
  return((size_t)a.sizep2memfunc());
}


/*********************************************************
* virtual base class offset calculation interface
*********************************************************/

   /* Setting up class inheritance */

/*********************************************************
* Inheritance information setup/
*********************************************************/
extern "C" void G__cpp_setup_inheritanceFtfHitDict() {

   /* Setting up class inheritance */
   if(0==G__getnumbaseclass(G__get_linked_tagnum(&G__FtfHitDictLN_FtfHit))) {
     FtfHit *G__Lderived;
     G__Lderived=(FtfHit*)0x1000;
     {
       FtfBaseHit *G__Lpbase=(FtfBaseHit*)G__Lderived;
       G__inheritance_setup(G__get_linked_tagnum(&G__FtfHitDictLN_FtfHit),G__get_linked_tagnum(&G__FtfHitDictLN_FtfBaseHit),(long)G__Lpbase-(long)G__Lderived,1,1);
     }
   }
}

/*********************************************************
* typedef information setup/
*********************************************************/
extern "C" void G__cpp_setup_typetableFtfHitDict() {

   /* Setting up typedef entry */
   G__search_typename2("Char_t",99,-1,0,
-1);
   G__setnewtype(-1,"Signed Character 1 byte",0);
   G__search_typename2("UChar_t",98,-1,0,
-1);
   G__setnewtype(-1,"Unsigned Character 1 byte",0);
   G__search_typename2("Short_t",115,-1,0,
-1);
   G__setnewtype(-1,"Signed Short integer 2 bytes",0);
   G__search_typename2("UShort_t",114,-1,0,
-1);
   G__setnewtype(-1,"Unsigned Short integer 2 bytes",0);
   G__search_typename2("Int_t",105,-1,0,
-1);
   G__setnewtype(-1,"Signed integer 4 bytes",0);
   G__search_typename2("UInt_t",104,-1,0,
-1);
   G__setnewtype(-1,"Unsigned integer 4 bytes",0);
   G__search_typename2("Seek_t",105,-1,0,
-1);
   G__setnewtype(-1,"File pointer",0);
   G__search_typename2("Long_t",108,-1,0,
-1);
   G__setnewtype(-1,"Signed long integer 8 bytes",0);
   G__search_typename2("ULong_t",107,-1,0,
-1);
   G__setnewtype(-1,"Unsigned long integer 8 bytes",0);
   G__search_typename2("Float_t",102,-1,0,
-1);
   G__setnewtype(-1,"Float 4 bytes",0);
   G__search_typename2("Double_t",100,-1,0,
-1);
   G__setnewtype(-1,"Float 8 bytes",0);
   G__search_typename2("Text_t",99,-1,0,
-1);
   G__setnewtype(-1,"General string",0);
   G__search_typename2("Bool_t",98,-1,0,
-1);
   G__setnewtype(-1,"Boolean (0=false, 1=true)",0);
   G__search_typename2("Byte_t",98,-1,0,
-1);
   G__setnewtype(-1,"Byte (8 bits)",0);
   G__search_typename2("Version_t",115,-1,0,
-1);
   G__setnewtype(-1,"Class version identifier",0);
   G__search_typename2("Option_t",99,-1,0,
-1);
   G__setnewtype(-1,"Option string",0);
   G__search_typename2("Ssiz_t",105,-1,0,
-1);
   G__setnewtype(-1,"String size",0);
   G__search_typename2("Real_t",102,-1,0,
-1);
   G__setnewtype(-1,"TVector and TMatrix element type",0);
   G__search_typename2("VoidFuncPtr_t",89,-1,0,
-1);
   G__setnewtype(-1,"pointer to void function",0);
   G__search_typename2("FreeHookFun_t",89,-1,0,
-1);
   G__setnewtype(-1,NULL,0);
   G__search_typename2("ReAllocFun_t",81,-1,0,
-1);
   G__setnewtype(-1,NULL,0);
   G__search_typename2("ReAllocCFun_t",81,-1,0,
-1);
   G__setnewtype(-1,NULL,0);
   G__search_typename2("Axis_t",102,-1,0,
-1);
   G__setnewtype(-1,"Axis values type",0);
   G__search_typename2("Stat_t",100,-1,0,
-1);
   G__setnewtype(-1,"Statistics type",0);
}

/*********************************************************
* Data Member information setup/
*********************************************************/

   /* Setting up class,struct,union tag member variable */

   /* FtfHit */
static void G__setup_memvarFtfHit(void) {
   G__tag_memvar_setup(G__get_linked_tagnum(&G__FtfHitDictLN_FtfHit));
   { FtfHit *p; p=(FtfHit*)0x1000; if (p) { }
   G__memvar_setup((void*)((long)(&p->id)-(long)(p)),108,0,0,-1,-1,-1,1,"id=",0,(char*)NULL);
   G__memvar_setup((void*)((long)(&p->phiIndex)-(long)(p)),115,0,0,-1,-1,-1,1,"phiIndex=",0,"Phi index    ");
   G__memvar_setup((void*)((long)(&p->etaIndex)-(long)(p)),115,0,0,-1,-1,-1,1,"etaIndex=",0,"Eta index    ");
   G__memvar_setup((void*)((long)(&p->flags)-(long)(p)),115,0,0,-1,-1,-1,1,"flags=",0,"various flags      ");
   G__memvar_setup((void*)((long)(&p->sector)-(long)(p)),115,0,0,-1,-1,-1,1,"sector=",0,"various flags      ");
   G__memvar_setup((void*)((long)(&p->nextVolumeHit)-(long)(p)),89,0,0,-1,-1,-1,1,"nextVolumeHit=",0,"Next volume hit            ");
   G__memvar_setup((void*)((long)(&p->nextRowHit)-(long)(p)),89,0,0,-1,-1,-1,1,"nextRowHit=",0,"Next row hit               ");
   G__memvar_setup((void*)((long)(&p->r)-(long)(p)),102,0,0,-1,-1,-1,1,"r=",0,"radius                     ");
   G__memvar_setup((void*)((long)(&p->phi)-(long)(p)),102,0,0,-1,-1,-1,1,"phi=",0,"azimuthal angle            ");
   G__memvar_setup((void*)((long)(&p->dphi)-(long)(p)),102,0,0,-1,-1,-1,1,"dphi=",0,"Error in phi               ");
   G__memvar_setup((void*)((long)(&p->eta)-(long)(p)),102,0,0,-1,-1,-1,1,"eta=",0,"hit pseudorapidity         ");
   G__memvar_setup((void*)((long)(&p->xp)-(long)(p)),102,0,0,-1,-1,-1,1,"xp=",0,"x conformal coordinate ");
   G__memvar_setup((void*)((long)(&p->yp)-(long)(p)),102,0,0,-1,-1,-1,1,"yp=",0,"y conformal coordinate ");
   G__memvar_setup((void*)((long)(&p->buffer1)-(long)(p)),115,0,0,-1,-1,-1,1,"buffer1=",0,(char*)NULL);
   G__memvar_setup((void*)((long)(&p->buffer2)-(long)(p)),115,0,0,-1,-1,-1,1,"buffer2=",0,(char*)NULL);
   G__memvar_setup((void*)((long)(&p->hardwareId)-(long)(p)),114,0,0,-1,-1,-1,1,"hardwareId=",0,(char*)NULL);
   G__memvar_setup((void*)NULL,85,0,0,G__get_linked_tagnum(&G__FtfHitDictLN_TClass),-1,-2,4,"fgIsA=",0,(char*)NULL);
   }
   G__tag_memvar_reset();
}

extern "C" void G__cpp_setup_memvarFtfHitDict() {
}
/***********************************************************
************************************************************
************************************************************
************************************************************
************************************************************
************************************************************
************************************************************
***********************************************************/

/*********************************************************
* Member function information setup for each class
*********************************************************/
static void G__setup_memfuncFtfHit(void) {
   /* FtfHit */
   G__tag_memfunc_setup(G__get_linked_tagnum(&G__FtfHitDictLN_FtfHit));
   G__memfunc_setup("printLinks",1070,G__FtfHit_printLinks_0_0,121,-1,-1,0,0,1,1,0,"",(char*)NULL,(void*)NULL,0);
   G__memfunc_setup("printLinks",1070,G__FtfHit_printLinks_1_0,121,-1,-1,0,1,1,1,0,"i - - 0 - level",(char*)NULL,(void*)NULL,0);
   G__memfunc_setup("setStatus",976,G__FtfHit_setStatus_2_0,121,-1,-1,0,1,1,1,0,"U 'FtfTrack' - 0 - this_track",(char*)NULL,(void*)NULL,0);
   G__memfunc_setup("DeclFileName",1145,G__FtfHit_DeclFileName_3_0,67,-1,-1,0,0,1,1,1,"",(char*)NULL,(void*)NULL,0);
   G__memfunc_setup("DeclFileLine",1152,G__FtfHit_DeclFileLine_4_0,105,-1,-1,0,0,1,1,0,"",(char*)NULL,(void*)NULL,0);
   G__memfunc_setup("ImplFileName",1171,G__FtfHit_ImplFileName_5_0,67,-1,-1,0,0,1,1,1,"",(char*)NULL,(void*)NULL,0);
   G__memfunc_setup("ImplFileLine",1178,G__FtfHit_ImplFileLine_6_0,105,-1,-1,0,0,1,1,0,"",(char*)NULL,(void*)NULL,0);
   G__memfunc_setup("Class_Version",1339,G__FtfHit_Class_Version_7_0,115,-1,G__defined_typename("Version_t"),0,0,1,1,0,"",(char*)NULL,(void*)NULL,0);
   G__memfunc_setup("Class",502,G__FtfHit_Class_8_0,85,G__get_linked_tagnum(&G__FtfHitDictLN_TClass),-1,0,0,1,1,0,"",(char*)NULL,(void*)NULL,0);
   G__memfunc_setup("IsA",253,G__FtfHit_IsA_9_0,85,G__get_linked_tagnum(&G__FtfHitDictLN_TClass),-1,0,0,1,1,8,"",(char*)NULL,(void*)NULL,1);
   G__memfunc_setup("ShowMembers",1132,G__FtfHit_ShowMembers_0_1,121,-1,-1,0,2,1,1,0,
"u 'TMemberInspector' - 1 - insp C - - 0 - parent",(char*)NULL,(void*)NULL,1);
   G__memfunc_setup("Streamer",835,G__FtfHit_Streamer_1_1,121,-1,-1,0,1,1,1,0,"u 'TBuffer' - 1 - b",(char*)NULL,(void*)NULL,1);
   G__memfunc_setup("Dictionary",1046,G__FtfHit_Dictionary_2_1,121,-1,-1,0,0,1,1,0,"",(char*)NULL,(void*)NULL,0);
   // automatic default constructor
   G__memfunc_setup("FtfHit",581,G__FtfHit_FtfHit_3_1,(int)('i'),G__get_linked_tagnum(&G__FtfHitDictLN_FtfHit),-1,0,0,1,1,0,"",(char*)NULL,(void*)NULL,0);
   // automatic copy constructor
   G__memfunc_setup("FtfHit",581,G__FtfHit_FtfHit_4_1,(int)('i'),G__get_linked_tagnum(&G__FtfHitDictLN_FtfHit),-1,0,1,1,1,0,"u 'FtfHit' - 1 - -",(char*)NULL,(void*)NULL,0);
   // automatic destructor
   G__memfunc_setup("~FtfHit",707,G__FtfHit_wAFtfHit_5_1,(int)('y'),-1,-1,0,0,1,1,0,"",(char*)NULL,(void*)NULL,0);
   G__tag_memfunc_reset();
}


/*********************************************************
* Member function information setup
*********************************************************/
extern "C" void G__cpp_setup_memfuncFtfHitDict() {
}

/*********************************************************
* Global variable information setup for each class
*********************************************************/
extern "C" void G__cpp_setup_globalFtfHitDict() {

   /* Setting up global variables */
   G__resetplocal();


   G__resetglobalenv();
}

/*********************************************************
* Global function information setup for each class
*********************************************************/
extern "C" void G__cpp_setup_funcFtfHitDict() {
   G__lastifuncposition();


   G__resetifuncposition();
}

/*********************************************************
* Class,struct,union,enum tag information setup
*********************************************************/
/* Setup class/struct taginfo */
G__linked_taginfo G__FtfHitDictLN_TClass = { "TClass" , 99 , -1 };
G__linked_taginfo G__FtfHitDictLN_FtfBaseHit = { "FtfBaseHit" , 99 , -1 };
G__linked_taginfo G__FtfHitDictLN_FtfHit = { "FtfHit" , 99 , -1 };

/* Reset class/struct taginfo */
extern "C" void G__cpp_reset_tagtableFtfHitDict() {
  G__FtfHitDictLN_TClass.tagnum = -1 ;
  G__FtfHitDictLN_FtfBaseHit.tagnum = -1 ;
  G__FtfHitDictLN_FtfHit.tagnum = -1 ;
}

extern "C" void G__cpp_setup_tagtableFtfHitDict() {

   /* Setting up class,struct,union tag entry */
   G__tagtable_setup(G__get_linked_tagnum(&G__FtfHitDictLN_FtfHit),sizeof(FtfHit),-1,0,(char*)NULL,G__setup_memvarFtfHit,G__setup_memfuncFtfHit);
}
extern "C" void G__cpp_setupFtfHitDict() {
  G__check_setup_version(51111,"G__cpp_setupFtfHitDict()");
  G__set_cpp_environmentFtfHitDict();
  G__cpp_setup_tagtableFtfHitDict();

  G__cpp_setup_inheritanceFtfHitDict();

  G__cpp_setup_typetableFtfHitDict();

  G__cpp_setup_memvarFtfHitDict();

  G__cpp_setup_memfuncFtfHitDict();
  G__cpp_setup_globalFtfHitDict();
  G__cpp_setup_funcFtfHitDict();

   if(0==G__getsizep2memfunc()) G__get_sizep2memfuncFtfHitDict();
  return;
}
class G__cpp_setup_initFtfHitDict {
  public:
    G__cpp_setup_initFtfHitDict() { G__add_setup_func("FtfHitDict",&G__cpp_setupFtfHitDict); }
   ~G__cpp_setup_initFtfHitDict() { G__remove_setup_func("FtfHitDict"); }
};
G__cpp_setup_initFtfHitDict G__cpp_setup_initializerFtfHitDict;

