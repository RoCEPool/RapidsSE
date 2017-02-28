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
    char m_cArea[SIZE];  // ..dataArea..
    i8   m_nAreaUsed;    // bytes used in the area (must be after m_cArea)
  
  public:
    Wsp(void);

    void  vWspAppend(void* aSrc, i8 nBytes);
    char* aWspArea(void);
    i8    nWspUsed(void);
    char* aWspNext(void);
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
  m_nAreaUsed = 0;
}

/*----------------------------------------------------------------------------*/
/* append source bytes to data area                                           */
/*----------------------------------------------------------------------------*/
template<int SIZE>
void Wsp<SIZE>::vWspAppend(void* aSrc, i8 nBytes)
{
  void* aDst = aWspNext();

  ASSERT(m_nAreaUsed + nBytes <= SIZE);
  
  Memory::vCopy(aDst,aSrc,nBytes);
  
  vWspTake(nBytes);
}

/*----------------------------------------------------------------------------*/
/* get ptr to data area                                                       */
/*----------------------------------------------------------------------------*/
template<int SIZE>
char* Wsp<SIZE>::aWspArea(void)
{
  return m_cArea;
}

/*----------------------------------------------------------------------------*/
/* get number of used bytes                                                   */
/*----------------------------------------------------------------------------*/
template<int SIZE>
i8 Wsp<SIZE>::nWspUsed(void)
{
  return m_nAreaUsed;
}

/*----------------------------------------------------------------------------*/
/* get next available chunk in data area                                      */
/*----------------------------------------------------------------------------*/
template<int SIZE>
char* Wsp<SIZE>::aWspNext(void)
{
  return &m_cArea[m_nAreaUsed];
}

/*----------------------------------------------------------------------------*/
/* get size of data area                                                      */
/*----------------------------------------------------------------------------*/
template<int SIZE>
i8 Wsp<SIZE>::nWspSize(void)
{
  return SIZE;
}

/*----------------------------------------------------------------------------*/
/* take chunk in data area                                                    */
/*----------------------------------------------------------------------------*/
template<int SIZE>
void Wsp<SIZE>::vWspTake(i8 nBytes)
{
  m_nAreaUsed += nBytes;
  
  ASSERT(m_nAreaUsed <= SIZE);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
