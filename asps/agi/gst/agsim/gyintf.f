CDECK  ID>, GYINTF.
      SUBROUTINE GYINTF
************************************************************************
*                                                                      *
*       GYINTF  Intrinsic function performance               VP 920916 *
*                                                                      *
*   ARGUMENTS:                                                         *
*                                                                      *
*                                                                      *
************************************************************************
      dimension IVAL(2,2),RVAL(2,2),DVAL(2,2),JVAL(2,2)
      double precision DVAL
      complex CCA,CCB      
*               "$EXP  "  Narg = 2 : 2
          IVAL(1,0) = IVAL(1,1) ** IVAL(1,2)
          RVAL(1,0) = RVAL(1,1) ** IVAL(1,2)
          DVAL(1,0) = DVAL(1,1) ** IVAL(1,2)
          IVAL(1,0) = IVAL(1,1) ** IVAL(1,2)
          RVAL(1,0) = RVAL(1,1) ** RVAL(1,2)
          DVAL(1,0) = DVAL(1,1) ** DVAL(1,2)
*               "$CONC "  Narg = 2 : 2
*               "$???  "  Narg = 2 : 2
*               "$???  "  Narg = 2 : 2
*               "$???  "  Narg = 2 : 2
*               "$???  "  Narg = 2 : 2
*               "ABS   "  Narg = 1 : 1
        IVAL(1,0)   = IABS(IVAL(1,1))
        RVAL(1,0)   = ABS(RVAL(1,1))
        DVAL(1,0)   = DABS(DVAL(1,1))
*               "ACOS  "  Narg = 1 : 1
        RVAL(1,0) = ACOS(RVAL(1,1))
        RVAL(1,0) = ACOS(RVAL(1,1))
        DVAL(1,0) = ACOS(DVAL(1,1))
*               "ACOSD "  Narg = 1 : 1
        RVAL(1,0) = ACOSD(RVAL(1,1))
        RVAL(1,0) = ACOSD(RVAL(1,1))
        DVAL(1,0) = ACOSD(DVAL(1,1))
*               "AIMAG "  Narg = 1 : 1
*               "AINT  "  Narg = 1 : 1
        RVAL(1,0) = AINT(RVAL(1,1))
        RVAL(1,0) = AINT(RVAL(1,1))
        DVAL(1,0) = AINT(DVAL(1,1))
*               "AMAX0 "  Narg = 2 :99
*               "AMIN0 "  Narg = 2 :99
*               "ANINT "  Narg = 1 : 1
        RVAL(1,0) = ANINT(RVAL(1,1))
        RVAL(1,0) = ANINT(RVAL(1,1))
        DVAL(1,0) = ANINT(DVAL(1,1))
*               "ASIN  "  Narg = 1 : 1
        RVAL(1,0) = ASIN(RVAL(1,1))
        RVAL(1,0) = ASIN(RVAL(1,1))
        DVAL(1,0) = ASIN(DVAL(1,1))
*               "ASIND "  Narg = 1 : 1
        RVAL(1,0) = ASIND (RVAL(1,1))
        RVAL(1,0) = ASIND (RVAL(1,1))
        DVAL(1,0) = ASIND (DVAL(1,1))
*               "ATAN  "  Narg = 1 : 1
        RVAL(1,0) = ATAN(RVAL(1,1))
        RVAL(1,0) = ATAN(RVAL(1,1))
        DVAL(1,0) = ATAN(DVAL(1,1))
*               "ATAND "  Narg = 1 : 1
        RVAL(1,0) = ATAND(RVAL(1,1))
        RVAL(1,0) = ATAND(RVAL(1,1))
        DVAL(1,0) = ATAND(DVAL(1,1))
*               "ATAN2 "  Narg = 2 : 2
        RVAL(1,0) = ATAN2(RVAL(1,1),RVAL(1,2))
        RVAL(1,0) = ATAN2(RVAL(1,1),RVAL(1,2))
        DVAL(1,0) = ATAN2(DVAL(1,1),DVAL(1,2))
*               "ATAN2D"  Narg = 2 : 2
        RVAL(1,0) = ATAN2D(RVAL(1,1),RVAL(1,2))
        RVAL(1,0) = ATAN2D(RVAL(1,1),RVAL(1,2))
        DVAL(1,0) = ATAN2D(DVAL(1,1),DVAL(1,2))
*               "BTEST "  Narg = 2 : 2
*     IVAL(1,0) = BTEST(JVAL(1,1),IVAL(1,2))
      IVAL(1,0) = JBIT(JVAL(1,1),IVAL(1,2)+1)
*               "CHAR  "  Narg = 1 : 1
*               "CMPLX "  Narg = 1 : 1
*               "CONJG "  Narg = 1 : 1
*               "COS   "  Narg = 1 : 1
        RVAL(1,0) = COS(RVAL(1,1))
        RVAL(1,0) = COS(RVAL(1,1))
        DVAL(1,0) = COS(DVAL(1,1))
*               "COSD  "  Narg = 1 : 1
        RVAL(1,0) = COSD(RVAL(1,1))
        RVAL(1,0) = COSD(RVAL(1,1))
        DVAL(1,0) = COSD(DVAL(1,1))
*               "COSH  "  Narg = 1 : 1
        RVAL(1,0) = COSH(RVAL(1,1))
        RVAL(1,0) = COSH(RVAL(1,1))
        DVAL(1,0) = COSH(DVAL(1,1))
*               "DATE  "  Narg = 1 : 1
      RVAL(1,0) = IDDATE
*               "DBLE  "  Narg = 1 : 1
      DVAL(1,0) = DVAL(1,1)
*               "DCMPLX"  Narg = 1 : 1
*               "DFLOAT"  Narg = 1 : 1
      DVAL(1,0) = DVAL(1,1)
*               "DIM   "  Narg = 2 : 2
        IVAL(1,0) = DIM(IVAL(1,1),IVAL(1,2))
        RVAL(1,0) = DIM(RVAL(1,1),RVAL(1,2))
        DVAL(1,0) = DIM(DVAL(1,1),DVAL(1,2))
*               "DIMAG "  Narg = 1 : 1
*               "DPROD "  Narg = 2 : 2
      DVAL(1,0) = DVAL(1,1)*DVAL(1,2)
*               "DREAL "  Narg = 1 : 1
*               "EXP   "  Narg = 1 : 1
        RVAL(1,0) = EXP(RVAL(1,1))
        RVAL(1,0) = EXP(RVAL(1,1))
        DVAL(1,0) = EXP(DVAL(1,1))
*               "FLOAT "  Narg = 1 : 1
      RVAL(1,0) = RVAL(1,1)
*               "IABS  "  Narg = 1 : 1
      IVAL(1,0) = IABS(IVAL(1,1))
*               "IAND  "  Narg = 2 : 2
      IVAL(1,0) = IAND(JVAL(1,1),JVAL(1,2))
*               "IBCLR "  Narg = 2 : 2
      IVAL(1,0) = IBCLR(JVAL(1,1),IVAL(1,2))
*               "IBITS "  Narg = 3 : 3
      IVAL(1,0) = IBITS(JVAL(1,1),IVAL(1,2),IVAL(1,3))
*               "IBSET "  Narg = 2 : 2
      IVAL(1,0) = IBSET(JVAL(1,1),IVAL(1,2))
*               "ICHAR "  Narg = 1 : 1
*               "IDATE "  Narg = 1 : 1
      IVAL(1,0) = IDDATE
*               "IDIM  "  Narg = 2 : 2
      IVAL(1,0) = IDIM(IVAL(1,1),IVAL(1,2))
*               "IDINT "  Narg = 1 : 1
      IVAL(1,0) = IVAL(1,1)
*               "IDNINT"  Narg = 1 : 1
      IVAL(1,0) = IDNINT(DVAL(1,1))
*               "IEOR  "  Narg = 2 : 2
      IVAL(1,0) = IEOR(JVAL(1,1),JVAL(1,2))
*               "IFIX  "  Narg = 1 : 1
      IVAL(1,0) = IVAL(1,1)
*               "INDEX "  Narg = 2 : 2
*               "INT   "  Narg = 1 : 1
      IVAL(1,0) = IVAL(1,1)
*               "IOR   "  Narg = 2 : 2
      IVAL(1,0) = IOR(JVAL(1,1),JVAL(1,2))
*               "IQINT "  Narg = 1 : 1
*               "IQNINT"  Narg = 1 : 1
*               "ISHFT "  Narg = 2 : 2
      IVAL(1,0) = ISHFT(JVAL(1,1),IVAL(1,2))
*               "ISHFTC"  Narg = 2 : 2
      IVAL(1,0) = ISHFTC(JVAL(1,1),IVAL(1,2),IVAL(1,3))
*               "ISIGN "  Narg = 2 : 2
      IVAL(1,0) = ISIGN(IVAL(1,1),IVAL(1,2))
*               "LEN   "  Narg = 1 : 1
*               "LGE   "  Narg = 2 : 2
*               "LGT   "  Narg = 2 : 2
*               "LLE   "  Narg = 2 : 2
*               "LLT   "  Narg = 2 : 2
*               "LOG   "  Narg = 1 : 1
        RVAL(1,0) = LOG (RVAL(1,1))
        RVAL(1,0) = LOG (RVAL(1,1))
        DVAL(1,0) = LOG (DVAL(1,1))
*               "LOG10 "  Narg = 1 : 1
        RVAL(1,0) = LOG10 (RVAL(1,1))
        RVAL(1,0) = LOG10 (RVAL(1,1))
        DVAL(1,0) = LOG10 (DVAL(1,1))
*               "LOG2  "  Narg = 1 : 1
        RVAL(1,0) = LOG (RVAL(1,1))/ALOG(2.)
        RVAL(1,0) = LOG (RVAL(1,1))/ALOG(2.)
        DVAL(1,0) = LOG (DVAL(1,1))/LOG(2.D0)
*               "MAX   "  Narg = 2 :99
      DVAL(1,0)   = DVAL(1,1)
        DVAL(1,0) = MAX(DVAL(1,0),DVAL(1,J))
      IVAL(1,0)   = DVAL(1,0)
      RVAL(1,0)   = DVAL(1,0)
*               "MAX0  "  Narg = 2 :99
*               "MAX1  "  Narg = 2 :99
*               "MIN   "  Narg = 2 :99
      DVAL(1,0) = DVAL(1,1)
        DVAL(1,0) = MIN(DVAL(1,0),DVAL(1,J))
      IVAL(1,0)   = DVAL(1,0)
      RVAL(1,0)   = DVAL(1,0)
*               "MIN0  "  Narg = 2 :99
*               "MIN1  "  Narg = 2 :99
*               "MOD   "  Narg = 2 : 2
        IVAL(1,0) = MOD(IVAL(1,1),IVAL(1,2))
        RVAL(1,0) = AMOD(RVAL(1,1),RVAL(1,2))
        DVAL(1,0) = DMOD(DVAL(1,1),DVAL(1,2))
*               "MVBITS"  Narg = 5 : 5
      IVAL(1,0) = IVAL(1,4)
*               "NINT  "  Narg = 1 : 1
      IVAL(1,0) = NINT(RVAL(1,1))
*               "NOT   "  Narg = 1 : 1
      IVAL(1,0) = NOT(JVAL(1,1))
*               "QEXT  "  Narg = 1 : 1
*               "QFLOAT"  Narg = 1 : 1
*               "RAN   "  Narg = 1 : 1
      RVAL(1,0) = RAN(ISEED)
*               "REAL  "  Narg = 1 : 1
*               "SIZEOF"  Narg = 1 : 1
c        IVAL(1,0) = SIZEOF(IVAL(1,1))
c        IVAL(1,0) = SIZEOF(RVAL(1,1))
c        IVAL(1,0) = SIZEOF(DVAL(1,1))
*               "SECNDS"  Narg = 1 : 1
c      RVAL(1,0) = SECNDS (RVAL(1,1))
*               "SIGN  "  Narg = 2 : 2
      DVAL(1,0) = SIGN(DVAL(1,1),DVAL(1,2))
      RVAL(1,0) = DVAL(1,0)
*               "SIN   "  Narg = 1 : 1
        RVAL(1,0) = SIN (RVAL(1,1))
        RVAL(1,0) = SIN (RVAL(1,1))
        DVAL(1,0) = SIN (DVAL(1,1))
*               "SIND  "  Narg = 1 : 1
        RVAL(1,0) = SIND (RVAL(1,1))
        RVAL(1,0) = SIND (RVAL(1,1))
        DVAL(1,0) = SIND (DVAL(1,1))
*               "SINH  "  Narg = 1 : 1
        RVAL(1,0) = SINH (RVAL(1,1))
        RVAL(1,0) = SINH (RVAL(1,1))
        DVAL(1,0) = SINH (DVAL(1,1))
*               "SQRT  "  Narg = 1 : 1
        RVAL(1,0) = SQRT (RVAL(1,1))
        RVAL(1,0) = SQRT (RVAL(1,1))
        DVAL(1,0) = SQRT (DVAL(1,1))
*               "TAN   "  Narg = 1 : 1
        RVAL(1,0) = TAN (RVAL(1,1))
        RVAL(1,0) = TAN (RVAL(1,1))
        DVAL(1,0) = TAN (DVAL(1,1))
*               "TAND  "  Narg = 1 : 1
        RVAL(1,0) = TAND (RVAL(1,1))
        RVAL(1,0) = TAND (RVAL(1,1))
        DVAL(1,0) = TAND (DVAL(1,1))
*               "TANH  "  Narg = 1 : 1
        RVAL(1,0) = TANH (RVAL(1,1))
        RVAL(1,0) = TANH (RVAL(1,1))
        DVAL(1,0) = TANH (DVAL(1,1))

*	bessel

        RVAL(1,1) = besj0(RVAL(1,1))
        RVAL(1,1) = besj1(RVAL(1,1))
        RVAL(1,1) = besjn(RVAL(1,1))
        RVAL(1,1) = besy0(RVAL(1,1))
        RVAL(1,1) = besy1(RVAL(1,1))
        RVAL(1,1) = besyn(RVAL(1,1))
        DVAL(1,1) = dbesj0(DVAL(1,1))
        DVAL(1,1) = dbesj1(DVAL(1,1))
        DVAL(1,1) = dbesjn(DVAL(1,1))
        DVAL(1,1) = dbesy0(DVAL(1,1))
        DVAL(1,1) = dbesy1(DVAL(1,1))
        DVAL(1,1) = dbesyn(DVAL(1,1))


*		CERN Bessel

	DVAL(1,1) = dbesi0(DVAL(1,1))
	DVAL(1,1) = dbesi1(DVAL(1,1))
	DVAL(1,1) = dbesk0(DVAL(1,1))
	DVAL(1,1) = dbesk1(DVAL(1,1))
	DVAL(1,1) = dbesj0(DVAL(1,1))
	DVAL(1,1) = dbesy0(DVAL(1,1))
	DVAL(1,1) = dbesj1(DVAL(1,1))
	DVAL(1,1) = dbesy1(DVAL(1,1))
	RVAL(1,1) = besi0(RVAL(1,1))
	RVAL(1,1) = besi1(RVAL(1,1))
	RVAL(1,1) = ebesi0(RVAL(1,1))
	RVAL(1,1) = ebesi1(RVAL(1,1))
	RVAL(1,1) = besk0(RVAL(1,1))
	RVAL(1,1) = besk1(RVAL(1,1))
	RVAL(1,1) = ebesk0(RVAL(1,1))
	RVAL(1,1) = ebesk1(RVAL(1,1))
	RVAL(1,1) = besj0(RVAL(1,1))
	RVAL(1,1) = besj1(RVAL(1,1))
	RVAL(1,1) = besy0(RVAL(1,1))
	RVAL(1,1) = besy1(RVAL(1,1))
	RVAL(1,1) = wclbes(RVAL(1,1))

      open(unit=1998,file='dummy',status='old',err=999)
      read (1998,*,end=999) i,r,DVAL(1,1)
      write(*,*)     'AAAAA',i,r,DVAL(1,1)
      close(1998)
      
      i = mod(i,j)
      C=(-1)**MOD(KFA,2)    

      CCA = CCA*CCB/(CCA+CCB)
      CCA = CCA/RRR
      CCA = exp(cca) + log(cca) +cos(cca) + sin(cca)

 999  return
      END
