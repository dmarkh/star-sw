// $Id: lmv.cc,v 1.1 1999/10/27 19:30:08 nystrand Exp $
// $Log: lmv.cc,v $
// Revision 1.1  1999/10/27 19:30:08  nystrand
// First Version of lmv
//
//
///////////////////////////////////////////////////////////////////////////////
//
// lmv.cc
//
// Description: 
//  Low Multiplicity Primary Vertex Finder
//  Called by StPrimaryMaker.cxx for global multiplicities < 15.
//  Finds the closest approach of each track to the beam-line (Z-axis)
//  and the uncertainty in this position due to the track extrapolation 
//  and multiple scattering. The fit is done by linearizing the tracks
//  around the point of closest approach, as described in STAR Note 89.
//
// Environment:
//  Software developed for the STAR Detector at Brookhaven National Laboratory
//
// Author List: 
//  Joakim Nystrand, LBNL  9/99
//
// History:
//
///////////////////////////////////////////////////////////////////////////////
#include <iostream.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <vector>
#include "StChain.h"
#include "SystemOfUnits.h"
#include "StThreeVectorD.hh"
#include "StHelixD.hh"
#include "StPhysicalHelixD.hh"
#include "TMath.h"
#include "tables/St_dst_track_Table.h"
#include "tables/St_dst_vertex_Table.h"
#include "StDetectorDefinitions.h"
//#include "St_base/Stypes.h"
#include "Stypes.h"
#include "math_constants.h"

#include "fortranc.h"
extern "C" {void type_of_call F77_NAME(gufld,GUFLD)(float *x, float *b);}
#define gufld F77_NAME(gufld,GUFLD)
//#include "StMagF.h"


//static const char rcsid[] = "$Id: lmv.cc,v 1.1 1999/10/27 19:30:08 nystrand Exp $";

long lmv(St_dst_track *track, St_dst_vertex *vertex)
{

  // Constants
  //  double kappa  = 0.299792458; // Constant Units (GeV T**-1 m**-1)
  double RIfc   = 49.3;        // From Roy Bossingham
  double RBPipe = 3.95;        // From $STAR/pams/geometry/pipegeo/pipegeo.g
  double X_X0_Ifc   = 0.0052;  // From Jim Thomas Web Page
  double X_X0_BPipe = 0.00283; // Based on 1 mm of Be
  double X0_N2      = 32609;   // This is in cm. Calc from PDG.

  // Parameters
  double Rmincut      = 4.0;
  long   MinTrkPoints = 10;
  double DVtxMax      = 4.0;

  // Get BField from gufld(,) 
  cout<<"Trying to Get the BField the old way..."<<endl;
  float x[3] = {0,0,0};
  float b[3];
  gufld(x,b);
  double bfield = 0.1*b[2]; //This is now Tesla.
  //cout<<"bfield = "<<bfield<<endl;

  //	  cout<<"Trying to Get the BField the new way..."<<endl;
  //          float y[3] = {0,0,0};
  //          float c[3];
  //          StMagF *mymag = new StMagF();
  //          mymag->Field(y,c);
  //          cout<<"New field in KGauss: "<<c[2]<<endl;
          

  //  St_dst_track *glb_pointer = (St_tpt_track *) tpc_iterator("tptrack");

#ifdef ST_NO_TEMPLATE_DEF_ARGS
  vector<long,allocator<long> > index;
  vector<StPhysicalHelixD,allocator<StPhysicalHelixD> > clvec,helices;
  vector<double,allocator<double> > TrkLength,sigma;
#else
  vector<long > index;
  vector<StPhysicalHelixD > clvec,helices;
  vector<double > TrkLength,sigma;
#endif
  index.clear();
  double spath,h;
  double x0,y0,z0;
  double ptinv,psi,tanl;
  double px,py,pz;
  StThreeVectorD XVertex=(0.0,0.0,0.0);
  double chi2, chi2pdof;

  long Ntrk = track->GetNRows();
  if( Ntrk <= 1 ){
    cout<<"lmv: Event contains "<<Ntrk<<" global tracks. ";
    cout<<"No vertex can be reconstructed."<<endl;
    return kStOK; 
  }
  cout<<"lmv: Low Multiplicity Vertex Finder. Number of global tracks: "<<Ntrk<<endl;

  long i_non_tpc=0;
  dst_track_st *glb_track_pointer = track->GetTable();
  dst_track_st *sec_pointer;
  sec_pointer = new dst_track_st;
  sec_pointer = glb_track_pointer;
  for (long l=0; l<Ntrk; l++){

    // First point on Helix
    x0 = glb_track_pointer->r0*cos(C_RAD_PER_DEG*glb_track_pointer->phi0);
    y0 = glb_track_pointer->r0*sin(C_RAD_PER_DEG*glb_track_pointer->phi0);
    z0 = glb_track_pointer->z0;
    StThreeVectorD origin(x0*centimeter, y0*centimeter, z0*centimeter);

    // Helicity / Sense of Curvatutre
    h  = 1.0;  if( bfield*glb_track_pointer->icharge > 0.0 )h=-1.0;
    double qtrk = 1.0; if( h*bfield > 0.0)qtrk=-1.0;

    // Track direction at first point
    ptinv  = glb_track_pointer->invpt;
    tanl   = glb_track_pointer->tanl;
    psi    = (C_PI/180.0)*glb_track_pointer->psi; if(psi<0.0){psi=psi+2.*C_PI;}

    px   = (1./ptinv)*cos(psi);
    py   = (1./ptinv)*sin(psi);
    pz   = (1./ptinv)*tanl;
    StThreeVectorD MomFstPt(px*GeV, py*GeV, pz*GeV);
    
    StPhysicalHelixD *TrkHlxPtr; 
    TrkHlxPtr = new StPhysicalHelixD(MomFstPt, origin, bfield*tesla, qtrk);
    StPhysicalHelixD TrkHlx = *TrkHlxPtr;

    long NPoints = glb_track_pointer->n_point;
    if(NPoints > MinTrkPoints){
      helices.push_back(TrkHlx);
      double ltk = glb_track_pointer->length;
      TrkLength.push_back(ltk);
      long trk_id = glb_track_pointer->id;
      index.push_back(trk_id);
      if( glb_track_pointer->det_id != kTpcIdentifier )i_non_tpc=1;
    }

    glb_track_pointer++;

  }

  //Currently, use only pure tpc tracks
  if( i_non_tpc == 1 ){
    cout<<"This event contains non-tpc tracks - lmv currently only works for pure tpc tracks"<<endl;
    return kStWarn;
  }

#ifdef ST_NO_TEMPLATE_DEF_ARGS
  vector<StPhysicalHelixD,allocator<StPhysicalHelixD> >::iterator ihlx=helices.begin();
  vector<double,allocator<double> >::iterator ihelp=TrkLength.begin();
  vector<long,allocator<long> >::iterator i_index=index.begin();
#else
  vector<StPhysicalHelixD >::iterator ihlx=helices.begin();
  vector<double >::iterator ihelp=TrkLength.begin();
  vector<long >::iterator i_index=index.begin();
#endif  
  while( ihlx != helices.end()){
    StPhysicalHelixD trk = *ihlx;
    double xorigin = 0.0; double yorigin = 0.0;
    spath = trk.pathLength(xorigin, yorigin);
    StThreeVectorD XMinVec = trk.at(spath);
    //    cout<<"Min Position: "<<XMinVec<<endl;
    double x_m = XMinVec.x(), y_m = XMinVec.y();
    double dmin = sqrt(x_m*x_m + y_m*y_m);
    if( dmin > Rmincut ){
      helices.erase(ihlx);
      TrkLength.erase(ihelp);
      index.erase(i_index);
    }
    else{
      ihlx++; ihelp++; i_index++;
    }
  }

  // Do the Multiple Scattering
  for(unsigned int jj=0; jj < helices.size(); jj++){

    double xo = 0.0; double yo = 0.0;
    spath = helices[jj].pathLength(xo, yo);

    // Find Coordinates of Intersect with IFC
    double ifcpath=0.0;
    pairD  SIfc; 
    SIfc = helices[jj].pathLength(RIfc);
    if(SIfc.first > 0.0)SIfc.first=SIfc.first-helices[jj].period();
    if(SIfc.second > 0.0)SIfc.second=SIfc.second-helices[jj].period();
    if( SIfc.second < SIfc.first){
      ifcpath = SIfc.first;
    }
    else{
      ifcpath = SIfc.second;
    }

    StThreeVectorD xpos;
    xpos = helices[jj].at(ifcpath);
    double xhat = xpos.x()/xpos.mag();
    double yhat = xpos.y()/xpos.mag();
    double zhat = xpos.z()/xpos.mag();

    // Find momentum at this point
    StThreeVectorD pmom;
    pmom = helices[jj].momentumAt(ifcpath, bfield*tesla);
    double pxhat = pmom.x()/pmom.mag();
    double pyhat = pmom.y()/pmom.mag();
    double pzhat = pmom.z()/pmom.mag();

    double incang = acos( xhat*pxhat + yhat*pyhat );
    double lpath_ifc = X_X0_Ifc/cos(incang); // 0.52% is dr/Lrad for IFC

    // Find Coordinates of Intersect with Beam Pipe
    double bpipepath=0.0;
    pairD  SBPipe; 
    SBPipe = helices[jj].pathLength(RBPipe);
    if(SBPipe.first > 0.0)SBPipe.first=SBPipe.first-helices[jj].period();
    if(SBPipe.second > 0.0)SBPipe.second=SBPipe.second-helices[jj].period();
    if( SBPipe.second < SBPipe.first){
      bpipepath = SBPipe.first;
    }
    else{
      bpipepath = SBPipe.second;
    }

    //    StThreeVectorD xpos;
    xpos = helices[jj].at(bpipepath);
    xhat = xpos.x()/xpos.mag();
    yhat = xpos.y()/xpos.mag();
    zhat = xpos.z()/xpos.mag();

    // Find momentum at this point
    //    StThreeVectorD pmom;
    pmom = helices[jj].momentumAt(ifcpath, bfield*tesla);
    pxhat = pmom.x()/pmom.mag();
    pyhat = pmom.y()/pmom.mag();
    pzhat = pmom.z()/pmom.mag();

    incang = acos( xhat*pxhat + yhat*pyhat );
    double lpath_bp = X_X0_BPipe/cos(incang); 

    // Do the rescattering in the gas
    double lpath_gas = fabs(spath-ifcpath)/X0_N2;

    // From Particle Data Booklet
    double beta = pmom.mag()/sqrt(pmom.mag()*pmom.mag()+0.139*0.139); //Assume pion

    //
    //    double dth1 = (0.0136/(beta*pmom.mag()))*sqrt(lpath_ifc)*(1+0.038*log(lpath_ifc));
    //           dth1 = C_SQRT2*dth1;
    //    cout<<"Error in angle    : "<<dth<<endl;

    //    double dth2 = (0.0136/(beta*pmom.mag()))*sqrt(lpath_bp)*(1+0.038*log(lpath_bp));
    //           dth2 = C_SQRT2*dth2;

    //    double dth3 = (0.0136/(beta*pmom.mag()))*sqrt(lpath_gas)*(1+0.038*log(lpath_gas));
    //           dth3 = C_SQRT2*dth3;

    double lpath_tot = lpath_ifc + lpath_bp + lpath_gas;
    double dth4 = (0.0136/(beta*pmom.mag()))*sqrt(lpath_tot)*(1+0.038*log(lpath_tot));
           dth4 = C_SQRT2*dth4;

    //    double dr1 = fabs(dth1*(spath-ifcpath));
    //    double dr2 = fabs(dth2*(spath-bpipepath));
    //    double dr3 = fabs(dth3*(spath-ifcpath));
    double dr4 = fabs(dth4*(spath-ifcpath));
    sigma.push_back(dr4);

  }  

  // Do the extrapolation
  // This is entirely phenomenological now since there is no complete error matrix. 
  if( helices.size() != TrkLength.size() )cout<<"This is a problem!"<<endl;
  for(unsigned int kk=0; kk < helices.size(); kk++){

    double xoo = 0.0; double yoo = 0.0;
    spath = helices[kk].pathLength(xoo, yoo);
    double D_fst_point = 0.030; // Estimated track resolution at first point

    double dr_ext = 0.0;
    if( TrkLength[kk] > 1.0 ){
      dr_ext = D_fst_point + D_fst_point*fabs(spath/TrkLength[kk]);
    }

  }

  //Do the actual vertex fitting, continue until good
  double A11=0.0,A12=0.0,A13=0.0,A21=0.0,A22=0.0,A23=0.0;
  double A31=0.0,A32=0.0,A33=0.0; // Matrix Elements
  double C11=0.0,C12=0.0,C13=0.0,C21=0.0,C22=0.0,C23=0.0;
  double C31=0.0,C32=0.0,C33=0.0; // C = A^-1
  int done = 0;
  while( done != 1 ){

    // Check that there at least are 2 tracks
    if( helices.size() <= 1 ){
      cout<<"lmv: Fewer than 2 track remains. No vertex found."<<endl;
      return kStOK;
    }

    // Begin by doing a fit
    A11=0.0,A12=0.0,A13=0.0,A21=0.0,A22=0.0,A23=0.0;
    A31=0.0,A32=0.0,A33=0.0; // Matrix Elements
    C11=0.0,C12=0.0,C13=0.0,C21=0.0,C22=0.0,C23=0.0;
    C31=0.0,C32=0.0,C33=0.0; // C = A^-1
    double b1=0.0,b2=0.0,b3=0.0;
    // Compute matrix A and vector b
    for(unsigned int itr=0; itr < helices.size(); itr++){ 
      //      sigma[itr]=1.0;
      double xo=0.0,yo=0.0;
      spath = helices[itr].pathLength(xo,yo);
      StThreeVectorD XClosest = helices[itr].at(spath);
      StThreeVectorD XMomAtClosest = helices[itr].momentumAt(spath,bfield*tesla);
      double xp   = XClosest.x(); double yp= XClosest.y(); double zp= XClosest.z();  
      double xhat = XMomAtClosest.x()/XMomAtClosest.mag();
      double yhat = XMomAtClosest.y()/XMomAtClosest.mag();
      double zhat = XMomAtClosest.z()/XMomAtClosest.mag();
      A11=A11+(yhat*yhat+zhat*zhat)/sigma[itr];
      A12=A12-(xhat*yhat)/sigma[itr];
      A13=A13-(xhat*zhat)/sigma[itr];
      A22=A22+(xhat*xhat+zhat*zhat)/sigma[itr];
      A23=A23-(yhat*zhat)/sigma[itr];
      A33=A33+(xhat*xhat+yhat*yhat)/sigma[itr];
      b1=b1 + ( (yhat*yhat+zhat*zhat)*xp - xhat*yhat*yp - xhat*zhat*zp )/sigma[itr];
      b2=b2 + ( (xhat*xhat+zhat*zhat)*yp - xhat*yhat*xp - yhat*zhat*zp )/sigma[itr];
      b3=b3 + ( (xhat*xhat+yhat*yhat)*zp - xhat*zhat*xp - yhat*zhat*yp )/sigma[itr];
    }
    A21 = A12; A31=A13; A32=A23;

    // Invert A
    double detA =   A11*A22*A33 + A12*A23*A31 + A13*A21*A32;
    detA = detA   - A31*A22*A13 - A32*A23*A11 - A33*A21*A12;
    //    cout<<"Determinant= "<<detA<<endl;
    //    cout<<"A11,A12,A13: "<<A11<<" "<<A12<<" "<<A13<<endl;
    //    cout<<"A21,A22,A23: "<<A21<<" "<<A22<<" "<<A23<<endl;
    //    cout<<"A31,A32,A33: "<<A31<<" "<<A32<<" "<<A33<<endl;
    //    cout<<"b1,b2,b3 "<<b1<<" "<<b2<<" "<<b3<<endl;
    C11=(A22*A33-A23*A32)/detA; C12=(A13*A32-A12*A33)/detA; C13=(A12*A23-A13*A22)/detA;
    C21=C12;                    C22=(A11*A33-A13*A31)/detA; C23=(A13*A21-A11*A23)/detA;
    C31=C13;                    C32=C23;                    C33=(A11*A22-A12*A21)/detA;

    // Find Vertex Position
    double Xv = C11*b1 + C12*b2 + C13*b3;
    double Yv = C21*b1 + C22*b2 + C23*b3;
    double Zv = C31*b1 + C32*b2 + C33*b3;
    XVertex.setX(Xv); XVertex.setY(Yv); XVertex.setZ(Zv);
    //    cout<<"Vertex Position   : "<<XVertex.x()<<" "<<XVertex.y()<<" "<<XVertex.z()<<endl;
    //    cout<<"Error in Position : "<<sqrt(C11)<<" "<<sqrt(C22)<<" "<<sqrt(C33)<<endl;
    

    // Check if the fit is any good

    // Loop over tracks again to get Chi2 and check each track's deviation
    double dmax=0.0;
#ifdef ST_NO_TEMPLATE_DEF_ARGS
    vector<StPhysicalHelixD,allocator<StPhysicalHelixD> >::iterator itehlx=helices.begin(), i1keep;
    vector<double,allocator<double> >::iterator itesig=sigma.begin(),i2keep;
    vector<long,allocator<long> >::iterator iteind=index.begin(),i3keep;
#else
    vector<StPhysicalHelixD >::iterator itehlx=helices.begin(), i1keep;
    vector<double >::iterator itesig=sigma.begin(),i2keep;
    vector<long >::iterator iteind=index.begin(),i3keep;
#endif
    while( itehlx != helices.end()){
      //      sigma[itr]=1.0;
      StPhysicalHelixD hlx = *itehlx;
      double sig = *itesig;
      spath = hlx.pathLength(XVertex); 
      StThreeVectorD XHel = hlx.at(spath);
      double d=(XHel.x()-XVertex.x())*(XHel.x()-XVertex.x());
         d = d+(XHel.y()-XVertex.y())*(XHel.y()-XVertex.y());
         d = d+(XHel.z()-XVertex.z())*(XHel.z()-XVertex.z());
         d = sqrt(d);
      chi2 = chi2 + (d*d)/(sig*sig);
      double drel = d/sig;
      if( drel > dmax ){
	// Save the track that deviates the most from vertex
        dmax = drel;
        i1keep = itehlx;
        i2keep = itesig;
        i3keep = iteind;
      }
      //
      itehlx++; itesig++; iteind++;
    }

    if( dmax > DVtxMax ){
      cout<<"Removing a track! dmax= "<<dmax<<endl;
      helices.erase(i1keep);
      sigma.erase(i2keep);
      index.erase(i3keep);
      done=0;
    }
    else{
      done=1;
    }
  } // End While Loop


  chi2pdof = chi2/(helices.size()-1);

  // Mark the vertex tracks in the global_trk table
  for (long ll=0; ll<Ntrk; ll++){
    long idt = sec_pointer->id;
    long icheck;
    icheck = 0;
    for(unsigned int ine=0; ine < index.size(); ine++){
      if( idt == index[ine] )icheck=1;
    }  
    if( icheck == 1 ){
      // sec_pointer->id_start_vertex = 1;
      sec_pointer->id_start_vertex = 7;
    }
    else{
      sec_pointer->id_start_vertex = 99;
    } 
    sec_pointer++;
  }

  // Fill the dst_vertex table
  long Nvtx = vertex->GetNRows();
  if( Nvtx != 1 ){cout<<"lvm: Problem, NPrimaryVertices>1 = "<<Nvtx<<endl;}
  dst_vertex_st *dst_vertex_pointer = vertex->GetTable();
  dst_vertex_pointer->vtx_id      = 1;
  dst_vertex_pointer->n_daughters = helices.size();
  dst_vertex_pointer->id          = 1;
  dst_vertex_pointer->iflag       = 1;
  dst_vertex_pointer->det_id      = kTpcIdentifier; //TPC track by definition
  dst_vertex_pointer->id_aux_ent  = 0;
  dst_vertex_pointer->x           = XVertex.x();
  dst_vertex_pointer->y           = XVertex.y();
  dst_vertex_pointer->z           = XVertex.z();
  dst_vertex_pointer->covar[0]    = sqrt(C11);
  dst_vertex_pointer->covar[1]    = sqrt(C12);
  dst_vertex_pointer->covar[2]    = sqrt(C22);
  dst_vertex_pointer->covar[3]    = sqrt(C13);
  dst_vertex_pointer->covar[4]    = sqrt(C23);
  dst_vertex_pointer->covar[5]    = sqrt(C33);
  dst_vertex_pointer->chisq[0]    = chi2pdof;
  dst_vertex_pointer->chisq[1]    = 1.0; // Need to find the prob func in Root

  return kStOK;
}





