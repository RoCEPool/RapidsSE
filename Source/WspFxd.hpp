/*============================================================================*/
/* WspFxd.hpp -- fixed-sized workspace                                           */
/*============================================================================*/

/*
 
 Description:
 
 */

#pragma once

#include "Magic.h"

namespace Magic
{
  template<int SIZE>
  class WspFxd : public WspAbs
  {
  private:
    char m_cArea[SIZE];  // ..dataArea..
    i8   m_nAreaUsed;    // bytes used in the area (must be after m_cArea)
  
  public:
    WspFxd(void);

    void  vWspAppend(void* aSrc, i8 nBytes);
    char* aWspArea(void);
    void  vWspFill(char cByte);
    i8    nWspFree(void);
    char* aWspNext(void);
    i8    nWspSize(void);
    void  vWspTake(i8 nBytes);
    i8    nWspUsed(void);
    void  vWspZero(void);
  };
}

using namespace Magic;

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
template<int SIZE>
WspFxd<SIZE>::WspFxd(void)
{
  m_nAreaUsed = 0;
}

/*----------------------------------------------------------------------------*/
/* append source bytes to data area                                           */
/*----------------------------------------------------------------------------*/
template<int SIZE>
void WspFxd<SIZE>::vWspAppend(void* aSrc, i8 nBytes)
{
  ASSERT(nWspUsed() + nBytes <= SIZE);
  
  void* aDst = aWspNext();

  Memory::vCopy(aDst,aSrc,nBytes);
  
  vWspTake(nBytes);
}

/*----------------------------------------------------------------------------*/
/* get address of data area                                                   */
/*----------------------------------------------------------------------------*/
template<int SIZE>
char* WspFxd<SIZE>::aWspArea(void)
{
  char* aArea = WspAbs::aWspArea();
  
  return aArea;
}

/*----------------------------------------------------------------------------*/
/* fill data area with given byte                                             */
/*----------------------------------------------------------------------------*/
template<int SIZE>
void WspFxd<SIZE>::vWspFill(char cByte)
{
  WspAbs::vWspFill(cByte,SIZE);
}

/*----------------------------------------------------------------------------*/
/* get number of free bytes in data area                                      */
/*----------------------------------------------------------------------------*/
template<int SIZE>
i8 WspFxd<SIZE>::nWspFree(void)
{
  i8 nFree = SIZE - nWspUsed();
  
  return nFree;
}

/*----------------------------------------------------------------------------*/
/* get address of next available chunk in data area                           */
/*----------------------------------------------------------------------------*/
template<int SIZE>
char* WspFxd<SIZE>::aWspNext(void)
{
  char* aNext = &aWspArea()[nWspUsed()];
  
  return aNext;
}

/*----------------------------------------------------------------------------*/
/* get size of data area                                                      */
/*----------------------------------------------------------------------------*/
template<int SIZE>
i8 WspFxd<SIZE>::nWspSize(void)
{
  return SIZE;
}

/*----------------------------------------------------------------------------*/
/* take chunk in data area                                                    */
/*----------------------------------------------------------------------------*/
template<int SIZE>
void WspFxd<SIZE>::vWspTake(i8 nBytes)
{
  m_nAreaUsed += nBytes;
  
  ASSERT(m_nAreaUsed <= SIZE);
}

/*----------------------------------------------------------------------------*/
/* get number of used bytes                                                   */
/*----------------------------------------------------------------------------*/
template<int SIZE>
i8 WspFxd<SIZE>::nWspUsed(void)
{
  return m_nAreaUsed;
}

/*----------------------------------------------------------------------------*/
/* zero fill data area                                                        */
/*----------------------------------------------------------------------------*/
template<int SIZE>
void WspFxd<SIZE>::vWspZero(void)
{
  WspAbs::vWspZero(SIZE);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
