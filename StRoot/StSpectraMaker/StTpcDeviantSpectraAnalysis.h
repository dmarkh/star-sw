#ifndef StTpcDeviantSpectraAnalysis_hh
#define StTpcDeviantSpectraAnalysis_hh

#include "TH1.h"
#include "TH2.h"
#include "TH3.h"
class TH1D;
class TH2D;
class TH3D;

#include "StSpectraAnalysis.h"

class StEvent;

class StTpcDeviantSpectraAnalysis : public StSpectraAnalysis {

 private:

  TH3D* m2DSpectraDeviantWeighted;
  TH3D* m2DSpectraDeviantCounts;
  TH2D* m2DSpectra;
  TH2D* m2DJacobian;
  TH2D* m2DCounts;

  TH1D* mPIDDeviant;  
  TH2D* mDedxvsP;
  
 public:

  StTpcDeviantSpectraAnalysis();
  ~StTpcDeviantSpectraAnalysis();

  void bookHistograms();
  void fillHistograms(StEvent& event);
  void projectHistograms();
  void writeHistograms();
};

#endif







