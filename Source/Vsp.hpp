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
    i8   m_nBytesUsed;
    // ..dataArea..
  
  public:
    Vsp(void);

    void  vVspAppend(void* pSrc, i8 nBytes);
    char* pVspArea(void);
    i8    nVspUsed(void);
    void  vVspUsed(i8 nBytes);
    char* pVspNext(void);
    void  vVspTake(i8 nBytes);
  };
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
