/*:>--------------------------------------------------------------------
**: FILE:       tfs_filt.c.template
**: HISTORY:
**:             00jan93-v000a-hpl- Created by stic Version
**:  Id: idl.y,v 1.7 1996/10/02 21:07:52 ward Exp  
**:<------------------------------------------------------------------*/
#include "tfs_filt.h"

long tfs_filt_(
  TABLE_HEAD_ST          *tphit_h,      TCL_TPHIT_ST            *tphit )
{
/*:>--------------------------------------------------------------------
**: ROUTINE:    tfs_filt_
**: DESCRIPTION: Physics Analysis Module ANSI C template.
**:             This is an ANSI C Physics Analysis Module template
**:             automatically generated by stic from tfs_filt.idl.
**:             Please edit comments and code.
**: AUTHOR:     hpl - H.P. Lovecraft, hplovecraft@cthulhu.void
**: ARGUMENTS:
**:       IN:
**:    INOUT:
**:              tphit    - PLEASE FILL IN DESCRIPTION HERE
**:             tphit_    - header Structure for tphit
**:      OUT:
**: RETURNS:    STAF Condition Value
**:>------------------------------------------------------------------*/

long i, j, count, keep, shift, last;

count = 0;
keep  = -1;
shift = -1;
for (i=0; i <= tphit_h[0].nok; i++)
    {  if (tphit[i].flag != 0 || i == tphit_h[0].nok)
         { if (count >0 && shift>-1 )
           {
           keep = keep - shift;
           last = keep+count-1;
           for (j=keep; j<= last; j++)
             {
             tphit[j].id = tphit[j+1+shift].id;
             tphit[j].flag = tphit[j+1+shift].flag;
             tphit[j].cluster = tphit[j+1+shift].cluster;
             tphit[j].id_globtrk = tphit[j+1+shift].id_globtrk;
             tphit[j].nseq = tphit[j+1+shift].nseq;
             tphit[j].row = tphit[j+1+shift].row;
             tphit[j].track = tphit[j+1+shift].track;
             tphit[j].alpha = tphit[j+1+shift].alpha; 
             tphit[j].dalpha = tphit[j+1+shift].dalpha;  
             tphit[j].dlambda =  tphit[j+1+shift].dlambda;
             tphit[j].dq = tphit[j+1+shift].dq;
             tphit[j].dx = tphit[j+1+shift].dx;
             tphit[j].dy = tphit[j+1+shift].dy;
             tphit[j].dz =  tphit[j+1+shift].dz;
             tphit[j].lambda = tphit[j+1+shift].lambda; 
             tphit[j].phi =  tphit[j+1+shift].phi;
             tphit[j].prf =  tphit[j+1+shift].prf;
             tphit[j].q =  tphit[j+1+shift].q;
             tphit[j].x =  tphit[j+1+shift].x;
             tphit[j].y =  tphit[j+1+shift].y;
             tphit[j].z = tphit[j+1+shift].z; 
             tphit[j].zrf =  tphit[j+1+shift].zrf;

             }
           }
           keep = i;
           count= 0;
           if (i < tphit_h[0].nok) shift++;          
         }
       else
         count++ ;
    }
   tphit_h[0].nok=tphit_h[0].nok-shift-1;
   return STAFCV_OK;
}
