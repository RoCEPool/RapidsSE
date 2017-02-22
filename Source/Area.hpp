/*============================================================================*/
/* Area.hpp -- data work area                                                 */
/*============================================================================*/

/*
 
 Description:
 
 */

#pragma once

#include "Values.h"

namespace Values
{
  template<int SIZE>
  class Area : public Part
  {
  private:
    i8   m_nBytesUsed;
    char m_mByteArea[SIZE];
  
  public:
    Area(void);

    void  vAppendBytes(void* pSrc, i8 nBytes);
    i8    nAreaSize(void);
    char* pByteArea(void);
    i8    nBytesUsed(void);
    char* pNextChunk(void);
    void  vTakeBytes(i8 nBytes);
  };
}

using namespace Values;

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
template<int SIZE>
Area<SIZE>::Area(void)
{
  m_nBytesUsed = 0;
}

/*----------------------------------------------------------------------------*/
/* get next available chunk in data area                                      */
/*----------------------------------------------------------------------------*/
template<int SIZE>
void Area<SIZE>::vAppendBytes(void* pSrc, i8 nBytes)
{
  void* pDst = pNextChunk();

  ASSERT(m_nBytesUsed + nBytes <= SIZE);
  
  Memory::vCopy(pDst,pSrc,nBytes);
  
  vTakeBytes(nBytes);
}

/*----------------------------------------------------------------------------*/
/* get size of byte area                                                      */
/*----------------------------------------------------------------------------*/
template<int SIZE>
i8 Area<SIZE>::nAreaSize(void)
{
  return SIZE;
}

/*----------------------------------------------------------------------------*/
/* get ptr to byte area                                                       */
/*----------------------------------------------------------------------------*/
template<int SIZE>
char* Area<SIZE>::pByteArea(void)
{
  return m_mByteArea;
}

/*----------------------------------------------------------------------------*/
/* get number of used bytes                                                   */
/*----------------------------------------------------------------------------*/
template<int SIZE>
i8 Area<SIZE>::nBytesUsed(void)
{
  return m_nBytesUsed;
}

/*----------------------------------------------------------------------------*/
/* get next available chunk in data area                                      */
/*----------------------------------------------------------------------------*/
template<int SIZE>
char* Area<SIZE>::pNextChunk(void)
{
  return &m_mByteArea[m_nBytesUsed];
}

/*----------------------------------------------------------------------------*/
/* get next available chunk in data area                                      */
/*----------------------------------------------------------------------------*/
template<int SIZE>
void Area<SIZE>::vTakeBytes(i8 nBytes)
{
  m_nBytesUsed += nBytes;
  
  ASSERT(m_nBytesUsed <= SIZE);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
