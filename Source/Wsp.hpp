/*============================================================================*/
/* Wsp.hpp -- fixed-sized workspace                                           */
/*============================================================================*/

/*
 
 Description:
 
 */

#pragma once

#include "Values.h"

namespace Values
{
  template<int SIZE>
  class Wsp : public Part
  {
  private:
    char m_mByteArea[SIZE];
    i8   m_nBytesUsed;
  
  public:
    Wsp(void);

    void  vWspAppend(void* pSrc, i8 nBytes);
    char* pWspArea(void);
    i8    nWspUsed(void);
    char* pWspNext(void);
    i8    nWspSize(void);
    void  vWspTake(i8 nBytes);
  };
}

using namespace Values;

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
template<int SIZE>
Wsp<SIZE>::Wsp(void)
{
  m_nBytesUsed = 0;
}

/*----------------------------------------------------------------------------*/
/* get next available chunk in data area                                      */
/*----------------------------------------------------------------------------*/
template<int SIZE>
void Wsp<SIZE>::vWspAppend(void* pSrc, i8 nBytes)
{
  void* pDst = pWspNext();

  ASSERT(m_nBytesUsed + nBytes <= SIZE);
  
  Memory::vCopy(pDst,pSrc,nBytes);
  
  vWspTake(nBytes);
}

/*----------------------------------------------------------------------------*/
/* get ptr to byte area                                                       */
/*----------------------------------------------------------------------------*/
template<int SIZE>
char* Wsp<SIZE>::pWspArea(void)
{
  return m_mByteArea;
}

/*----------------------------------------------------------------------------*/
/* get number of used bytes                                                   */
/*----------------------------------------------------------------------------*/
template<int SIZE>
i8 Wsp<SIZE>::nWspUsed(void)
{
  return m_nBytesUsed;
}

/*----------------------------------------------------------------------------*/
/* get next available chunk in data area                                      */
/*----------------------------------------------------------------------------*/
template<int SIZE>
char* Wsp<SIZE>::pWspNext(void)
{
  return &m_mByteArea[m_nBytesUsed];
}

/*----------------------------------------------------------------------------*/
/* get size of byte area                                                      */
/*----------------------------------------------------------------------------*/
template<int SIZE>
i8 Wsp<SIZE>::nWspSize(void)
{
  return SIZE;
}

/*----------------------------------------------------------------------------*/
/* get next available chunk in data area                                      */
/*----------------------------------------------------------------------------*/
template<int SIZE>
void Wsp<SIZE>::vWspTake(i8 nBytes)
{
  m_nBytesUsed += nBytes;
  
  ASSERT(m_nBytesUsed <= SIZE);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
