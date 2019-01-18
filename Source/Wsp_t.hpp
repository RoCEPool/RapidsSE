/*============================================================================*/
/* Wsp_t.hpp -- fixed-sized workspace                                         */
/*============================================================================*/
#pragma once

/*
 
 Description:
 
 */

namespace Correctness
{
  template<int LENGTH>
  class Wsp_t
  {
  private:
    char m_cArea[LENGTH]; // fixed byte area
    i8   m_nAreaUsed;     // bytes used in the area (must be after m_cArea)
  
  public:
    Wsp_t(void);

    char* aAllot(i8 nBytes);
    void  vAppend(void* aSrc, i8 nBytes, eCN eClassNo);
    char* aArea(void);
    void  vFill(char cByte, eCN eClassNo);
    i8    nFree(void);
    void  vGive(i8 nBytes);
    void  vInit(void);
    char* aLatter(void);
    i8    nLength(void);
    i8    nSize(void);
    Stz*  aStz(void);
    void  vTake(i8 nBytes);
    i8    nUsed(void);
    void  vUsed(i8 nBytes);
    void  vZero(eCN eClassNo);
  };
}

using namespace Correctness;

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
template<int LENGTH>
Wsp_t<LENGTH>::Wsp_t(void)
{
  m_nAreaUsed = 0;
}

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
template<int LENGTH>
void Wsp_t<LENGTH>::vInit(void)
{
  m_nAreaUsed = 0;
}

/*----------------------------------------------------------------------------*/
/* allot bytes from byte area                                                 */
/*----------------------------------------------------------------------------*/
template<int LENGTH>
char* Wsp_t<LENGTH>::aAllot(i8 nBytes)
{
  char* aAllotted = aLatter();
  
  vTake(nBytes);
  
  return aAllotted;
}

/*----------------------------------------------------------------------------*/
/* append source bytes to byte area                                           */
/*----------------------------------------------------------------------------*/
template<int LENGTH>
void Wsp_t<LENGTH>::vAppend(void* aSrc, i8 nBytes, eCN eClassNo)
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
char* Wsp_t<LENGTH>::aArea(void)
{
  char* aArea = m_cArea;
  
  return aArea;
}

/*----------------------------------------------------------------------------*/
/* fill byte area with given byte                                             */
/*----------------------------------------------------------------------------*/
template<int LENGTH>
void Wsp_t<LENGTH>::vFill(char cByte, eCN eClassNo)
{
  void* aDst = aArea();
  
  MemLib::vSet(aDst,cByte,LENGTH,eClassNo);
}

/*----------------------------------------------------------------------------*/
/* get number of free bytes in byte area                                      */
/*----------------------------------------------------------------------------*/
template<int LENGTH>
i8 Wsp_t<LENGTH>::nFree(void)
{
  i8 nFree = LENGTH - nUsed();
  
  return nFree;
}

/*----------------------------------------------------------------------------*/
/* get length of byte area                                                    */
/*----------------------------------------------------------------------------*/
template<int LENGTH>
i8 Wsp_t<LENGTH>::nLength(void)
{
  return LENGTH;
}

/*----------------------------------------------------------------------------*/
/* get address of next available chunk in byte area                           */
/*----------------------------------------------------------------------------*/
template<int LENGTH>
char* Wsp_t<LENGTH>::aLatter(void)
{
  char* aNext = &aArea()[nUsed()];
  
  return aNext;
}

/*----------------------------------------------------------------------------*/
/* get total size of this Wsp_t                                              */
/*----------------------------------------------------------------------------*/
template<int LENGTH>
i8 Wsp_t<LENGTH>::nSize(void)
{
  return sizeof(Wsp_t<LENGTH>);
}

/*----------------------------------------------------------------------------*/
/* get address of byte area                                                   */
/*----------------------------------------------------------------------------*/
template<int LENGTH>
Stz* Wsp_t<LENGTH>::aStz(void)
{
  char* aArea = m_cArea;
  
 return reinterpret_cast<Stz*>(aArea);
}

/*----------------------------------------------------------------------------*/
/* take chunk from byte area                                                  */
/*----------------------------------------------------------------------------*/
template<int LENGTH>
void Wsp_t<LENGTH>::vTake(i8 nBytes)
{
  m_nAreaUsed += nBytes;
  
  ASSERT(m_nAreaUsed <= LENGTH);
}

/*----------------------------------------------------------------------------*/
/* give chunk back to byte area                                               */
/*----------------------------------------------------------------------------*/
template<int LENGTH>
void Wsp_t<LENGTH>::vGive(i8 nBytes)
{
  ASSERT(m_nAreaUsed <= LENGTH);

  m_nAreaUsed -= nBytes;
}

/*----------------------------------------------------------------------------*/
/* get number of used bytes                                                   */
/*----------------------------------------------------------------------------*/
template<int LENGTH>
void Wsp_t<LENGTH>::vUsed(i8 nBytes)
{
  m_nAreaUsed = nBytes;
  
  ASSERT(m_nAreaUsed <= LENGTH);
}

/*----------------------------------------------------------------------------*/
/* get number of used bytes                                                   */
/*----------------------------------------------------------------------------*/
template<int LENGTH>
i8 Wsp_t<LENGTH>::nUsed(void)
{
  return m_nAreaUsed;
}

/*----------------------------------------------------------------------------*/
/* zero fill the byte area                                                    */
/*----------------------------------------------------------------------------*/
template<int LENGTH>
void Wsp_t<LENGTH>::vZero(eCN eClassNo)
{
  void* aDst = aArea();
  
  MemLib::vZero(aDst,LENGTH,eClassNo);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
