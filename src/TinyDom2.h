#ifndef x_TinyDom2_h
#define x_TinyDom2_h

#include "DetectorDescription/DDRegressionTest/src/TagName.h"
#include "DetectorDescription/DDCore/interface/adjgraph.h"
#include "DetectorDescription/DDCore/interface/graphwalker.h"
#include <string>
#include <map>
#include <iostream>

using std::string;
using std::map;
using std::ostream;
using std::pair;

class AnotherDummy2 {};

  typedef std::map<TagName, TagName> AttList2;
  typedef std::pair<TagName, AttList2> Node2;
  typedef graph<Node2, AnotherDummy2> TinyDom2;
  typedef graphwalker<Node2, AnotherDummy2> TinyDom2Walker;
  
  void TinyDom2PrettyPrint(ostream &, const TinyDom2 &);

#endif
