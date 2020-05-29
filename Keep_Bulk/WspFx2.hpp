/*============================================================================*/
/* WspFx2.hpp -- fixed-sized workspace                                        */
/*============================================================================*/
#pragma once

/*
 
 Description:
 
 */

namespace CorrectnessAlgebra
{
  template<int LENGTH>
  class WspFx2 : public Wsp
  {
  private:
    i8   m_nAreaUsed;     // bytes used in the area (must be after m_cArea)
    char m_cArea[LENGTH]; // fixed byte area
  
  public:
    WspFx2(void);

    char* aAllot(i8 nBytes);
    void  vAppend(void* aSrc, i8 nBytes, eCN eClassNo);
    char* aArea(void);
    void  vFill(char cByte);
    i8    nFree(void);
    void  vGive(i8 nBytes);
    void  vInit(void);
    char* aLatter(void);
    i8    nLength(void);
    i8    nSize(void);
    void  vTake(i8 nBytes);
    i8    nUsed(void);
    void  vUsed(i8 nBytes);
    void  vZero(eCN eClassNo);
  };
}

using namespace CorrectnessAlgebra;

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
template<int LENGTH>
WspFx2<LENGTH>::WspFx2(void)
{
  m_nAreaUsed = 0;
  
  vZero();
}

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
template<int LENGTH>
void WspFx2<LENGTH>::vInit(void)
{
  m_nAreaUsed = 0;
  
  vZero();
}

/*----------------------------------------------------------------------------*/
/* allot bytes from byte area                                                 */
/*----------------------------------------------------------------------------*/
template<int LENGTH>
char* WspFx2<LENGTH>::aAllot(i8 nBytes)
{
  char* aAllotted = aLatter();
  
  vTake(nBytes);
  
  return aAllotted;
}

/*----------------------------------------------------------------------------*/
/* append source bytes to byte area                                           */
/*----------------------------------------------------------------------------*/
template<int LENGTH>
void WspFx2<LENGTH>::vAppend(void* aSrc, i8 nBytes, eCN eClassNo)
{
  ASSERT(nUsed() + nBytes <= LENGTH);
  
  void* aDst = aLatter();

  MemLib::vCopy(aDst,aSrc,nBytes,eClassNo);
  
  vTake(nBytes);
}

/*----------------------------------------------------------------------------*/
/* get address of byte area                                                   */
/*----------------------------------------------------------------------------*/
template<int LENGTH>
char* WspFx2<LENGTH>::aArea(void)
{
  char* aArea = (char*)&m_cArea;
  
  return aArea;
}

/*----------------------------------------------------------------------------*/
/* fill byte area with given byte                                             */
/*----------------------------------------------------------------------------*/
template<int LENGTH>
void WspFx2<LENGTH>::vFill(char cByte)
{
  MemLib::vSet(aArea(),cByte,LENGTH);
}

/*----------------------------------------------------------------------------*/
/* get number of free bytes in byte area                                      */
/*----------------------------------------------------------------------------*/
template<int LENGTH>
i8 WspFx2<LENGTH>::nFree(void)
{
  i8 nFree = LENGTH - nUsed();
  
  return nFree;
}

/*----------------------------------------------------------------------------*/
/* get length of byte area                                                    */
/*----------------------------------------------------------------------------*/
template<int LENGTH>
i8 WspFx2<LENGTH>::nLength(void)
{
  return LENGTH;
}

/*----------------------------------------------------------------------------*/
/* get address of next available chunk in byte area                           */
/*----------------------------------------------------------------------------*/
template<int LENGTH>
char* WspFx2<LENGTH>::aLatter(void)
{
  char* aNext = &aArea()[nUsed()];
  
  return aNext;
}

/*----------------------------------------------------------------------------*/
/* get total size of this WspFx2                                              */
/*----------------------------------------------------------------------------*/
template<int LENGTH>
i8 WspFx2<LENGTH>::nSize(void)
{
  return sizeof(WspFx2<LENGTH>);
}

/*----------------------------------------------------------------------------*/
/* take chunk from byte area                                                  */
/*----------------------------------------------------------------------------*/
template<int LENGTH>
void WspFx2<LENGTH>::vTake(i8 nBytes)
{
  m_nAreaUsed += nBytes;
  
  ASSERT(m_nAreaUsed <= LENGTH);
}

/*----------------------------------------------------------------------------*/
/* give chunk back to byte area                                               */
/*----------------------------------------------------------------------------*/
template<int LENGTH>
void WspFx2<LENGTH>::vGive(i8 nBytes)
{
  ASSERT(m_nAreaUsed <= LENGTH);

  m_nAreaUsed -= nBytes;
}

/*----------------------------------------------------------------------------*/
/* get number of used bytes                                                   */
/*----------------------------------------------------------------------------*/
template<int LENGTH>
void WspFx2<LENGTH>::vUsed(i8 nBytes)
{
  m_nAreaUsed = nBytes;
  
  ASSERT(m_nAreaUsed <= LENGTH);
}

/*----------------------------------------------------------------------------*/
/* get number of used bytes                                                   */
/*----------------------------------------------------------------------------*/
template<int LENGTH>
i8 WspFx2<LENGTH>::nUsed(void)
{
  return m_nAreaUsed;
}

/*----------------------------------------------------------------------------*/
/* zero fill the byte area                                                    */
/*----------------------------------------------------------------------------*/
template<int LENGTH>
void WspFx2<LENGTH>::vZero(eCN eClassNo)
{
  _memZero_(aArea(),LENGTH,eClassNo);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
