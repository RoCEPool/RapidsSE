/*----------------------------------------------------------------------------*/
/*  Node.hpp - Node                                                           */
/*  RapidsSE                                                                  */
/*                                                                            */
/*----------------------------------------------------------------------------*/
#pragma once

#include <stdio.h>
#include "Common.h"

using namespace Common;

namespace SuperStructure {
  
  enum eNS // Node State
  {
    /* 0 */ NS_NONE=0,
    /* 1 */ NS_ACTIVE,
    /* 2 */ NS_INACTIVE
  };

  static const char *pNS[] {
    /* 0 */ "NONE",
    /* 1 */ "ACTIVE",
    /* 2 */ "INACTIVE"
  };
  
  class Node {

  private:
    i8  m_iTnxId;
    i8  m_iNodeNo;
    i8  m_iPort;
    eNS m_eState;
    i8  m_nNodeMgr;
  };

}