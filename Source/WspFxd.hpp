/*============================================================================*/
/* WspFxd.hpp -- fixed-sized workspace                                        */
/*============================================================================*/

/*
 
 Description:
 
 */

#pragma once

#include "Magic.h"

namespace Magic
{
  template<int LENGTH>
  class WspFxd : public Wsp
  {
  private:
    char m_cArea[LENGTH]; // byte area
    i8   m_nAreaUsed;     // bytes used in the area (must be after m_cArea)
  
  public:
    WspFxd(void);

    void  vWspAppend(void* aSrc, i8 nBytes);
    char* aWspArea(void);
    void  vWspFill(char cByte);
    i8    nWspFree(void);
    i8    nWspLength(void);
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
template<int LENGTH>
WspFxd<LENGTH>::WspFxd(void)
{
  m_nAreaUsed = 0;
}

/*----------------------------------------------------------------------------*/
/* append source bytes to byte area                                           */
/*----------------------------------------------------------------------------*/
template<int LENGTH>
void WspFxd<LENGTH>::vWspAppend(void* aSrc, i8 nBytes)
{
  ASSERT(nWspUsed() + nBytes <= LENGTH);
  
  void* aDst = aWspNext();

  Memory::vCopy(aDst,aSrc,nBytes);
  
  vWspTake(nBytes);
}

/*----------------------------------------------------------------------------*/
/* get address of byte area                                                   */
/*----------------------------------------------------------------------------*/
template<int LENGTH>
char* WspFxd<LENGTH>::aWspArea(void)
{
  char* aArea = Wsp::aWspArea();
  
  return aArea;
}

/*----------------------------------------------------------------------------*/
/* fill byte area with given byte                                             */
/*----------------------------------------------------------------------------*/
template<int LENGTH>
void WspFxd<LENGTH>::vWspFill(char cByte)
{
  Wsp::vWspFill(cByte,LENGTH);
}

/*----------------------------------------------------------------------------*/
/* get number of free bytes in byte area                                      */
/*----------------------------------------------------------------------------*/
template<int LENGTH>
i8 WspFxd<LENGTH>::nWspFree(void)
{
  i8 nFree = LENGTH - nWspUsed();
  
  return nFree;
}

/*----------------------------------------------------------------------------*/
/* get length of byte area                                                    */
/*----------------------------------------------------------------------------*/
template<int LENGTH>
i8 WspFxd<LENGTH>::nWspLength(void)
{
  return LENGTH;
}

/*----------------------------------------------------------------------------*/
/* get address of next available chunk in byte area                           */
/*----------------------------------------------------------------------------*/
template<int LENGTH>
char* WspFxd<LENGTH>::aWspNext(void)
{
  char* aNext = &aWspArea()[nWspUsed()];
  
  return aNext;
}

/*----------------------------------------------------------------------------*/
/* get total size of this WspFxd                                              */
/*----------------------------------------------------------------------------*/
template<int LENGTH>
i8 WspFxd<LENGTH>::nWspSize(void)
{
  return sizeof(WspFxd<LENGTH>);
}

/*----------------------------------------------------------------------------*/
/* take chunk from byte area                                                  */
/*----------------------------------------------------------------------------*/
template<int LENGTH>
void WspFxd<LENGTH>::vWspTake(i8 nBytes)
{
  m_nAreaUsed += nBytes;
  
  ASSERT(m_nAreaUsed <= LENGTH);
}

/*----------------------------------------------------------------------------*/
/* get number of used bytes                                                   */
/*----------------------------------------------------------------------------*/
template<int LENGTH>
i8 WspFxd<LENGTH>::nWspUsed(void)
{
  return m_nAreaUsed;
}

/*----------------------------------------------------------------------------*/
/* zero fill the byte area                                                    */
/*----------------------------------------------------------------------------*/
template<int LENGTH>
void WspFxd<LENGTH>::vWspZero(void)
{
  Wsp::vWspZero(LENGTH);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
