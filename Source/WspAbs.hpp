/*============================================================================*/
/* WspAbs.hpp -- fixed-sized workspace (abstract)                             */
/*============================================================================*/

/*
 
 Description:
 
 */

#pragma once

#include "Magic.h"

namespace Magic
{
  class WspAbs : public Part
  {
  private:
  
  public:
    WspAbs(void);

    void  vWspAppend(void* aSrc, i8 nBytes);
    char* aWspArea(void);
    void  vWspFill(char cByte);
    void  vWspFill(char cByte, i8 nBytes);
    i8    nWspFree(void);
    char* aWspNext(void);
    i8    nWspSize(void);
    void  vWspTake(i8 nBytes);
    i8    nWspUsed(void);
    void  vWspZero(void);
    void  vWspZero(i8 nBytes);
  };
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
