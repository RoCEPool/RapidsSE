/*============================================================================*/
/* WspConverge_t.hpp -- fixed-sized workspace that converges from both ends   */
/*============================================================================*/
#pragma once


namespace Alignment
{
  template<int LENGTH>
  class WspConverge_t
  {
  private:
    byte m_yaByteArea[LENGTH]; // fixed byte area
    i8   m_nUsedHead;          // bytes used in the area from the head
    i8   m_nUsedTail;          // bytes used in the area frim the tail
                               //  (must be after m_yaByteArea)
  
  public:
    WspConverge_t(void);

    byte* ayAllotHead(i8 nBytes);
    byte* ayAllotTail(i8 nBytes);
    void  vAppendHead(void* aSrc, i8 nBytes, eCN eClassNo);
    void  vAppendTail(void* aSrc, i8 nBytes, eCN eClassNo);
    byte* ayArea(void);
    void  vFill(char cByte, eCN eClassNo);
    i8    nFree(void);
    void  vGiveHead(i8 nBytes);
    void  vGiveTail(i8 nBytes);
    void  vInit(void);
    byte* ayLatterHead(void);
    byte* ayLatterTail(void);
    i8    nLength(void);
    i8    iOffset(byte* ayAllotted);
    void  vReset(void);
    i8    nSize(void);
    Stz*  szStz(void);
    void  vTakeHead(i8 nBytes);
    void  vTakeTail(i8 nBytes);
    i8    nUsed(void);
    i8    nUsedHead(void);
    i8    nUsedTail(void);
    void  vUsedHead(i8 nBytes);
    void  vUsedTail(i8 nBytes);
    void  vZero(eCN eClassNo);
  };
}

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
template<int LENGTH>
WspConverge_t<LENGTH>::WspConverge_t(void)
{
  m_nUsedHead = 0;
  m_nUsedTail = 0;
}

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
template<int LENGTH>
void WspConverge_t<LENGTH>::vInit(void)
{
  m_nUsedHead = 0;
  m_nUsedTail = 0;
}

/*----------------------------------------------------------------------------*/
/* allot bytes from byte area                                                 */
/*----------------------------------------------------------------------------*/
template<int LENGTH>
byte* WspConverge_t<LENGTH>::ayAllotHead(i8 nBytes)
{
  byte* ayAllotted = ayLatterHead();
  
  vTakeHead(nBytes);
  
  return ayAllotted;
}

/*----------------------------------------------------------------------------*/
/* allot bytes from byte area                                                 */
/*----------------------------------------------------------------------------*/
template<int LENGTH>
byte* WspConverge_t<LENGTH>::ayAllotTail(i8 nBytes)
{
  byte* ayAllotted = ayLatterTail() - nBytes;
  
  vTakeTail(nBytes);
  
  return ayAllotted;
}

/*----------------------------------------------------------------------------*/
/* append source bytes to byte area at head                                   */
/*----------------------------------------------------------------------------*/
template<int LENGTH>
void WspConverge_t<LENGTH>::vAppendHead(void* aSrc, i8 nBytes, eCN eClassNo)
{
  ASSERT(nUsed() + nBytes <= LENGTH);
  
  byte* ayDst = ayLatterHead();

  MemLib::vCopy(ayDst,aSrc,nBytes,eClassNo);
  
  vTakeHead(nBytes);
}

/*----------------------------------------------------------------------------*/
/* append source bytes to byte area at tail                                   */
/*----------------------------------------------------------------------------*/
template<int LENGTH>
void WspConverge_t<LENGTH>::vAppendTail(void* aSrc, i8 nBytes, eCN eClassNo)
{
  ASSERT(nUsed() + nBytes <= LENGTH);
  
  byte* ayDst = ayLatterTail() - nBytes + 1;
  
  MemLib::vCopy(ayDst,aSrc,nBytes,eClassNo);
  
  vTakeTail(nBytes);
}

/*----------------------------------------------------------------------------*/
/* get address of byte area                                                   */
/*----------------------------------------------------------------------------*/
template<int LENGTH>
byte* WspConverge_t<LENGTH>::ayArea(void)
{
  byte* ayArea = m_yaByteArea;
  
  return ayArea;
}

/*----------------------------------------------------------------------------*/
/* fill byte area with given byte                                             */
/*----------------------------------------------------------------------------*/
template<int LENGTH>
void WspConverge_t<LENGTH>::vFill(char cByte, eCN eClassNo)
{
  byte* ayDst = ayArea();
  
  MemLib::vSet(ayDst,cByte,LENGTH,eClassNo);
}

/*----------------------------------------------------------------------------*/
/* get number of free bytes in byte area                                      */
/*----------------------------------------------------------------------------*/
template<int LENGTH>
i8 WspConverge_t<LENGTH>::nFree(void)
{
  i8 nFree = LENGTH - m_nUsedHead - m_nUsedTail;
  ASSERT(nFree >= 0);
  
  return nFree;
}

/*----------------------------------------------------------------------------*/
/* get address of next available chunk in byte areafrom head                  */
/*----------------------------------------------------------------------------*/
template<int LENGTH>
byte* WspConverge_t<LENGTH>::ayLatterHead(void)
{
  byte* ayNext = &ayArea()[nUsedHead()];
  
  return ayNext;
}

/*----------------------------------------------------------------------------*/
/* get address of next available chunk in byte areafrom head                  */
/*----------------------------------------------------------------------------*/
template<int LENGTH>
byte* WspConverge_t<LENGTH>::ayLatterTail(void)
{
  byte* ayNext = &ayArea()[LENGTH - nUsedTail() - 1];
  
  return ayNext;
}

/*----------------------------------------------------------------------------*/
/* get length of byte area                                                    */
/*----------------------------------------------------------------------------*/
template<int LENGTH>
i8 WspConverge_t<LENGTH>::nLength(void)
{
  return LENGTH;
}

/*----------------------------------------------------------------------------*/
/* get offset of an allotted                                                  */
/*----------------------------------------------------------------------------*/
template<int LENGTH>
i8 WspConverge_t<LENGTH>::iOffset(byte* ayAllotted)
{
  i8 iOfs = nDifference(ayAllotted,m_yaByteArea);
  
  return iOfs;
}

/*----------------------------------------------------------------------------*/
/* reset this wsp                                                             */
/*----------------------------------------------------------------------------*/
template<int LENGTH>
void WspConverge_t<LENGTH>::vReset(void)
{
  vUsedHead(0);
  vUsedTail(0);
}

/*----------------------------------------------------------------------------*/
/* get total size of this Wsp_t                                               */
/*----------------------------------------------------------------------------*/
template<int LENGTH>
i8 WspConverge_t<LENGTH>::nSize(void)
{
  return sizeof(WspConverge_t<LENGTH>);
}

/*----------------------------------------------------------------------------*/
/* get byte area as Stz*                                                      */
/*----------------------------------------------------------------------------*/
template<int LENGTH>
Stz* WspConverge_t<LENGTH>::szStz(void)
{
  char* aArea = (char*)m_yaByteArea;
  
 return reinterpret_cast<Stz*>(aArea);
}

/*----------------------------------------------------------------------------*/
/* take chunk from byte area from the head                                    */
/*----------------------------------------------------------------------------*/
template<int LENGTH>
void WspConverge_t<LENGTH>::vTakeHead(i8 nBytes)
{
  m_nUsedHead += nBytes;
  
  if (m_nUsedHead  + m_nUsedTail > LENGTH) {
    vDebugPoint();
  }
  ASSERT(m_nUsedHead + m_nUsedTail <= LENGTH);
}

/*----------------------------------------------------------------------------*/
/* take chunk from byte area from the tail                                    */
/*----------------------------------------------------------------------------*/
template<int LENGTH>
void WspConverge_t<LENGTH>::vTakeTail(i8 nBytes)
{
  m_nUsedTail += nBytes;
  
  if (m_nUsedHead  + m_nUsedTail > LENGTH) {
    vDebugPoint();
  }
  ASSERT(m_nUsedHead + m_nUsedTail <= LENGTH);
}

/*----------------------------------------------------------------------------*/
/* give chunk back to byte area at head                                       */
/*----------------------------------------------------------------------------*/
template<int LENGTH>
void WspConverge_t<LENGTH>::vGiveHead(i8 nBytes)
{
  ASSERT(m_nUsedHead  + m_nUsedTail <= LENGTH);

  m_nUsedHead -= nBytes;
}

/*----------------------------------------------------------------------------*/
/* give chunk back to byte area at tail                                       */
/*----------------------------------------------------------------------------*/
template<int LENGTH>
void WspConverge_t<LENGTH>::vGiveTail(i8 nBytes)
{
  ASSERT(m_nUsedHead  + m_nUsedTail <= LENGTH);
  
  m_nUsedTail -= nBytes;
}

/*----------------------------------------------------------------------------*/
/* get number of used bytes                                                   */
/*----------------------------------------------------------------------------*/
template<int LENGTH>
i8 WspConverge_t<LENGTH>::nUsed(void)
{
  return m_nUsedHead + m_nUsedTail;
}

/*----------------------------------------------------------------------------*/
/* get number of used bytes                                                   */
/*----------------------------------------------------------------------------*/
template<int LENGTH>
i8 WspConverge_t<LENGTH>::nUsedHead(void)
{
  return m_nUsedHead;
}

/*----------------------------------------------------------------------------*/
/* get number of used bytes                                                   */
/*----------------------------------------------------------------------------*/
template<int LENGTH>
i8 WspConverge_t<LENGTH>::nUsedTail(void)
{
  return m_nUsedTail;
}

/*----------------------------------------------------------------------------*/
/* set number of used bytes at head                                           */
/*----------------------------------------------------------------------------*/
template<int LENGTH>
void WspConverge_t<LENGTH>::vUsedHead(i8 nBytes)
{
  m_nUsedHead = nBytes;
  
  ASSERT(m_nUsedHead + m_nUsedTail <= LENGTH);
}

/*----------------------------------------------------------------------------*/
/* set number of used bytes at tail                                           */
/*----------------------------------------------------------------------------*/
template<int LENGTH>
void WspConverge_t<LENGTH>::vUsedTail(i8 nBytes)
{
  m_nUsedTail = nBytes;
  
  ASSERT(m_nUsedHead + m_nUsedTail <= LENGTH);
}

/*----------------------------------------------------------------------------*/
/* zero fill the byte                                                         */
/*----------------------------------------------------------------------------*/
template<int LENGTH>
void WspConverge_t<LENGTH>::vZero(eCN eClassNo)
{
  m_nUsedHead = 0;
  m_nUsedTail = 0;
  
  byte* ayDst = ayArea();
  
  MemLib::vZero(ayDst,LENGTH,eClassNo);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
