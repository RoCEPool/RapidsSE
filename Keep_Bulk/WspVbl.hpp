/*============================================================================*/
/* WspVbl.hpp -- variable-sized workspace                                     */
/*============================================================================*/
#pragma once

/*
 
 Description:
 
 */

namespace Values
{
  class StrVal;
}

namespace CorrectnessAlgebra
{
  class WspVbl
  {
  friend class Values::StrVal;
  
  private:
    i8   m_nAreaUsed;
    // ..dataArea..
  
  public:
    WspVbl(void);

    char* aAllot(i8 nBytes);
    void  vAppend(void* aSrc, i8 nBytes, eCN eClassNo);
    char* aArea(void);
    void  vFill(char cByte, eCN eClassNo);
    i8    nFree(void);
    char* aLatter(void);
    i8    nLength(void);
    i8    nSize(void);
    void  vTake(i8 nBytes);
    i8    nUsed(void);
    void  vUsed(i8 nBytes);
    char* aVblData(void);
    void  vZero(eCN eClassNo);
  };
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
