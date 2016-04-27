// -*- mode: c++;-*-
// $Id: StjTreeReader.h,v 1.3.6.2 2016/04/27 17:32:52 zchang Exp $
// Copyright (C) 2008 Tai Sakuma <sakuma@bnl.gov>
#ifndef STJTREEREADER_H
#define STJTREEREADER_H

#include <TObject.h>

class TTree;

class StjTreeIndex;

class StjTreeReader : public TObject {

public:
  StjTreeReader(TTree* tree) : _tree(tree) { }
  StjTreeReader() : _tree(0) { }
  virtual ~StjTreeReader() { }

  virtual void Init();

  virtual Long64_t GetEntryWithIndex(const StjTreeIndex& idx);
  virtual Long64_t GetEntryWithIndex(Int_t big, Int_t small);

protected:

  virtual void SetBranchAddress(TTree *tree) { }

  virtual void clearEntry() = 0;
  virtual void readEntry() = 0;

  TTree* _tree;

  ClassDef(StjTreeReader, 1)

};

#endif // STJTREEREADER_H
