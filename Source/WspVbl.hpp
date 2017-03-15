/*============================================================================*/
/* WspVbl.hpp -- variable-sized workspace                                     */
/*============================================================================*/
#pragma once

/*
 
 Description:
 
 */

namespace Magic
{
  class WspVbl
  {
  private:
    i8   m_nAreaUsed;
    // ..dataArea..
  
  public:
    WspVbl(void);

    char* aAllot(i8 nBytes);
    void  vAppend(void* aSrc, i8 nBytes);
    char* sArea(void);
    void  vFill(char cByte);
    i8    nFree(void);
    char* sLatter(void);
    i8    nLength(void);
    i8    nSize(void);
    Str*  aStr(void);
    void  vTake(i8 nBytes);
    i8    nUsed(void);
    void  vUsed(i8 nBytes);
    void  vZero(void);
  };
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
