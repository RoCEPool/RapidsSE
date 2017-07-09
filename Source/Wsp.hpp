/*============================================================================*/
/* Wsp.hpp -- abstract workspace                                              */
/*============================================================================*/
#pragma once

/*
 
 Description:
 
 */

namespace Magic
{
  class Wsp
  {
  private:
  
  public:
    Wsp(void);

    char* aAllot(i8 nBytes);
    void  vAppend(void* aSrc, i8 nBytes);
    char* aArea(void);
    void  vFill(char cByte);
    void  vFill(char cByte, i8 nBytes, eCN eClassNo);
    i8    nFree(void);
    char* aLatter(void);
    i8    nLength(void);
    i8    nSize(void);
    Str*  aStr(void);
    void  vTake(i8 nBytes);
    i8    nUsed(void);
    void  vUsed(i8 nBytes);
    void  vZero(void);
    void  vZero(i8 nBytes, eCN eClassNo);
  };
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
