/**************************************************************************
 * Copyright(c) 2001, STAR  Experiment at BNL, All rights reserved.       *
 *                                                                        *
 * Author: STAR Integrated Track Task Force                               *
 * Contributors are mentioned in the code where appropriate.              *
 *                                                                        *
 * Permission to use, copy, modify and distribute this software and its   *
 * documentation strictly for non-commercial purposes is hereby granted   *
 * without fee, provided that the above copyright notice appears in all   *
 * copies and that both the copyright notice and this permission notice   *
 * appear in the supporting documentation. The authors make no claims     *
 * about the suitability of this software for any purpose. It is          *
 * provided "as is" without express or implied warranty.                  *
 *                                                                        *
 **************************************************************************/

/**************************************************************************
 *                                                                        *
 * StiFactory	                                                          *				   
 *                                                                        *
 * Author:  Claude Pruneau, Wayne State University                        *
 * Created: March 2001                                                    *
 *                                                                        *
 * Description: Base class used to define factories.                      *
 * It is assumed factories will have a name and shall be known as         *
 * factories but should otherwise have no other apriori attribute.        *
 *                                                                        *
 **************************************************************************/
#include "StiFactory.h"

ClassImp(StiFactory) 


StiFactory::StiFactory(const char * newName)
{
  name = 0;
  setName(newName);
}

StiFactory::~StiFactory()
{
  delete name;
}

const char * StiFactory::getName()
{
  return name;
}
 

void  StiFactory::setName(const char * newName)
{
  if (name!=0 && name!=newName)
    delete name;
  int size = 1+strlen(newName);
  name = malloc(size);
  strcpy(newName,name);
}

