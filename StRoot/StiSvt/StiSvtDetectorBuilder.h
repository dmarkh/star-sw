#ifndef StiSvtDetectorBuilder_H
#define StiSvtDetectorBuilder_H
#include "StSvtClassLibrary/StSvtConfig.hh"
#include "StSvtClassLibrary/StSvtGeometry.hh"
#include "StThreeVector.hh"
#include "Sti/StiDetectorBuilder.h"
#include "Sti/StiHitErrorCalculator.h"

class StiSvtDetectorBuilder : public StiDetectorBuilder
{
 public:
	StiSvtDetectorBuilder(bool active, const string & inputFile);
	virtual ~StiSvtDetectorBuilder(); 
	virtual void loadDS(TDataSet&);
  virtual void loadFS(ifstream& inFile);

	virtual void buildDetectors(StMaker& source);	
	virtual void setDefaults();

 protected:
	StiMaterial    * _gasMat;
	StiMaterial    * _siMat;
	StiMaterial    * _hybridMat;
	StiPlanarShape * _waferShape[6];
	StiPlanarShape * _hybridShape[6];
	StSvtConfig    * _config;
	StSvtGeometry  * _geometry;
	StiDefaultHitErrorCalculator  _calc;
};
#endif 
