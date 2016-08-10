/*----------------------------------------------------------------------------*/
/*  SessionSlot.hpp - Session Slot                                            */
/*  RapidsSE                                                                  */
/*                                                                            */
/*----------------------------------------------------------------------------*/
#pragma once

#include <stdio.h>
#include "Common.h"

using namespace Common;

namespace SuperStructure {
  
  class Tx {
    
  private:
    i8 m_iSlotNo;
    //eSS m_eSessionState;
    i8 m_nNextSlot;
    i8 m_nPrevSlot;
  };
  
}
