/*----------------------------------------------------------------------------*/
/*  Session.hpp - Session                                                     */
/*  RapidsSE                                                                  */
/*                                                                            */
/*----------------------------------------------------------------------------*/
#pragma once

#include <stdio.h>

#include "Common.h"

using namespace Common;

namespace SuperStructure {
    
  enum eSS // Session State
  {
    /* 0 */ SS_NONE=0,
    /* 1 */ SS_CONNECTED,
    /* 2 */ SS_CONNECTING,
    /* 2 */ SS_DISCONNECTED
  };

  static const char *pSS[] {
    /* 0 */ "NONE",
    /* 1 */ "CONNECTED",
    /* 2 */ "CONNECTING",
    /* 2 */ "DISCONNECTED"
  };
  
  class Session {
    
  private:
    i8  m_iTnxId;
    i8  m_iSsnId;
    eSS m_eState;
    i8  m_nSsnMgr;
  };
    
}

