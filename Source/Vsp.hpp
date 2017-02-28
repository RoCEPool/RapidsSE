/*============================================================================*/
/* Vsp.hpp -- variable-sized workspace                                        */
/*============================================================================*/

/*
 
 Description:
 
 */

#pragma once

#include "Magic.h"

namespace Magic
{
  class Vsp : public Part
  {
  private:
    i8   m_nAreaUsed;
    // ..dataArea..
  
  public:
    Vsp(void);

    void  vVspAppend(void* aSrc, i8 nBytes);
    char* aVspArea(void);
    void  vVspFill(char cByte);
    i8    nVspUsed(void);
    void  vVspUsed(i8 nBytes);
    char* aVspNext(void);
    void  vVspTake(i8 nBytes);
    void  vVspZero(void);
  };
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
