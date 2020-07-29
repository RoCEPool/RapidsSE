/*============================================================================*/
/* Wsp_t.hpp -- fixed-sized workspace                                         */
/*============================================================================*/
#pragma once


namespace Alignment
{
  template<int LENGTH>
  class Wsp_t
  {
  private:
    byte m_yaByteArea[LENGTH]; // fixed byte area
    i8   m_nAreaUsed;          // bytes used in the area
                               //  (must be after m_yaByteArea)
  
  public:
    Wsp_t(void);

    byte* ayAllot(i8 nBytes);
    void  vAppend(void* aSrc, i8 nBytes, eCN eClassNo);
    byte* ayArea(void);
    void  vFill(char cByte, eCN eClassNo);
    i8    nFree(void);
    void  vGive(i8 nBytes);
    void  vInit(void);
    byte* ayLatter(void);
    i8    nLength(void);
    void  vReset(void);
    i8    nSize(void);
    Stz*  szStz(void);
    void  vTake(i8 nBytes);
    i8    nUsed(void);
    void  vUsed(i8 nBytes);
    void  vZero(eCN eClassNo);
  };
}

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
template<int LENGTH>
Wsp_t<LENGTH>::Wsp_t(void)
{
#ifdef WITH_DEBUG_VERSION
  i8 nBytesNeeded =
       C_BYTES_PER_CACHE_LINE - ((i8)m_yaByteArea % C_BYTES_PER_CACHE_LINE);
  if (C_BYTES_PER_CACHE_LINE != nBytesNeeded)
    vTrace("Wst_t::m_yaByteArea %p needs %lld(B) to pad to cache line\n"_Stz,
           m_yaByteArea,nBytesNeeded);
#endif
  
  //ASSERT(CacheLine::bOnCacheLineBoundary(m_yaByteArea));
  
  m_nAreaUsed = 0;
}

/*----------------------------------------------------------------------------*/
/*-- Public ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* initialise                                                                 */
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
byte* Wsp_t<LENGTH>::ayAllot(i8 nBytes)
{
  byte* ayAllotted = ayLatter();
  
  vTake(nBytes);
  
  return ayAllotted;
}

/*----------------------------------------------------------------------------*/
/* append source bytes to byte area                                           */
/*----------------------------------------------------------------------------*/
template<int LENGTH>
void Wsp_t<LENGTH>::vAppend(void* aSrc, i8 nBytes, eCN eClassNo)
{
  ASSERT(nUsed() + nBytes <= LENGTH);
  
  byte* ayDst = ayLatter();

  MemLib::vCopy(ayDst,aSrc,nBytes,eClassNo);
  
  vTake(nBytes);
}

/*----------------------------------------------------------------------------*/
/* get address of byte area                                                   */
/*----------------------------------------------------------------------------*/
template<int LENGTH>
byte* Wsp_t<LENGTH>::ayArea(void)
{
  byte* ayArea = m_yaByteArea;
  
  return ayArea;
}

/*----------------------------------------------------------------------------*/
/* fill byte area with given byte                                             */
/*----------------------------------------------------------------------------*/
template<int LENGTH>
void Wsp_t<LENGTH>::vFill(char cByte, eCN eClassNo)
{
  byte* ayDst = ayArea();
  
  MemLib::vSet(ayDst,cByte,LENGTH,eClassNo);
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
byte* Wsp_t<LENGTH>::ayLatter(void)
{
  byte* ayNext = &ayArea()[nUsed()];
  
  return ayNext;
}

/*----------------------------------------------------------------------------*/
/* reset this wsp                                                             */
/*----------------------------------------------------------------------------*/
template<int LENGTH>
void Wsp_t<LENGTH>::vReset(void)
{
  vUsed(0);
}

/*----------------------------------------------------------------------------*/
/* get total size of this Wsp_t                                               */
/*----------------------------------------------------------------------------*/
template<int LENGTH>
i8 Wsp_t<LENGTH>::nSize(void)
{
  return sizeof(Wsp_t<LENGTH>);
}

/*----------------------------------------------------------------------------*/
/* get byte area as Stz*                                                      */
/*----------------------------------------------------------------------------*/
template<int LENGTH>
Stz* Wsp_t<LENGTH>::szStz(void)
{
  char* aArea = (char*)m_yaByteArea;
  
 return reinterpret_cast<Stz*>(aArea);
}

/*----------------------------------------------------------------------------*/
/* take chunk from byte area                                                  */
/*----------------------------------------------------------------------------*/
template<int LENGTH>
void Wsp_t<LENGTH>::vTake(i8 nBytes)
{
  m_nAreaUsed += nBytes;
  
  if (m_nAreaUsed > LENGTH) {
    vDebugPoint();
  }
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
i8 Wsp_t<LENGTH>::nUsed(void)
{
  return m_nAreaUsed;
}

/*----------------------------------------------------------------------------*/
/* set number of used bytes                                                   */
/*----------------------------------------------------------------------------*/
template<int LENGTH>
void Wsp_t<LENGTH>::vUsed(i8 nBytes)
{
  m_nAreaUsed = nBytes;
  
  ASSERT(m_nAreaUsed <= LENGTH);
}

/*----------------------------------------------------------------------------*/
/* zero fill the byte area                                                    */
/*----------------------------------------------------------------------------*/
template<int LENGTH>
void Wsp_t<LENGTH>::vZero(eCN eClassNo)
{
  m_nAreaUsed = 0;
  
  byte* ayDst = ayArea();
  
  MemLib::vZero(ayDst,LENGTH,eClassNo);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
