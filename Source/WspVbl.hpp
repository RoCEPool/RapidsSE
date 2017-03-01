/*============================================================================*/
/* WspVbl.hpp -- variable-sized workspace                                     */
/*============================================================================*/

/*
 
 Description:
 
 */

#pragma once

#include "Magic.h"

namespace Magic
{
  class WspVbl : public Part
  {
  private:
    i8   m_nAreaUsed;
    // ..dataArea..
  
  public:
    WspVbl(void);

    void  vWspAppend(void* aSrc, i8 nBytes);
    char* aWspArea(void);
    void  vWspFill(char cByte);
    i8    nWspUsed(void);
    void  vWspUsed(i8 nBytes);
    char* aWspNext(void);
    void  vWspTake(i8 nBytes);
    void  vWspZero(void);
  };
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
