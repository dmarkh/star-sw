// -*- mode: c++;-*-
// $Id: AbstractFourVec.h,v 1.2 2008/04/19 02:09:52 tai Exp $
#ifndef ABSTRACTFOURVEC_H
#define ABSTRACTFOURVEC_H

#include <iostream>
#include <cmath>

/*!
  \class AbstractFourVec
  \author M.L. Miller (Yale Software)
  Abstract base class to define required interface of a four vector to be fed to a derived instance
  of StJetFinder
 */
class AbstractFourVec {

public:
  AbstractFourVec() {};
  virtual ~AbstractFourVec() {};
	
  //access
	
  //momenta
  virtual double px() const = 0;
  virtual double py() const = 0;
  virtual double pz() const = 0;
	
  //angles
  virtual double phi() const = 0;
  virtual double eta() const = 0;
	
  //4-th component
  virtual double eT() const = 0;
  virtual double e() const = 0;
  virtual double mass() const = 0;
  	
  //charge
  virtual double charge() const = 0;
	
private:

};


#endif // ABSTRACTFOURVEC_H
