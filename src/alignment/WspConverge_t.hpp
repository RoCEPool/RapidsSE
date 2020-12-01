/*============================================================================*/
/* WspConverge_t.hpp -- fixed-sized workspace that converges from both ends   */
/*============================================================================*/
#pragma once


namespace Alignment
{
  template<int SIZE>
  class WspConverge_t
  {
  private:
    byte m_yaByteArea[SIZE]; // fixed byte area
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
template<int SIZE>
WspConverge_t<SIZE>::WspConverge_t(void)
{
  
#ifdef WITH_CHECK_INITIALISED
  MemLib::vZero(m_yaByteArea,sizeof(m_yaByteArea),CN_NONE);
#endif
  
  m_nUsedHead = 0;
  m_nUsedTail = 0;
}

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
template<int SIZE>
void WspConverge_t<SIZE>::vInit(void)
{
  m_nUsedHead = 0;
  m_nUsedTail = 0;
}

/*----------------------------------------------------------------------------*/
/* allot bytes from byte area                                                 */
/*----------------------------------------------------------------------------*/
template<int SIZE>
byte* WspConverge_t<SIZE>::ayAllotHead(i8 nBytes)
{
  byte* ayAllotted = ayLatterHead();
  
  vTakeHead(nBytes);
  
  return ayAllotted;
}

/*----------------------------------------------------------------------------*/
/* allot bytes from byte area                                                 */
/*----------------------------------------------------------------------------*/
template<int SIZE>
byte* WspConverge_t<SIZE>::ayAllotTail(i8 nBytes)
{
  byte* ayAllotted = ayLatterTail() - nBytes;
  
  vTakeTail(nBytes);
  
  return ayAllotted;
}

/*----------------------------------------------------------------------------*/
/* append source bytes to byte area at head                                   */
/*----------------------------------------------------------------------------*/
template<int SIZE>
void WspConverge_t<SIZE>::vAppendHead(void* aSrc, i8 nBytes, eCN eClassNo)
{
  ASSERT(nUsed() + nBytes <= SIZE);
  
  byte* ayDst = ayLatterHead();

  MemLib::vCopy(ayDst,aSrc,nBytes,eClassNo);
  
  vTakeHead(nBytes);
}

/*----------------------------------------------------------------------------*/
/* append source bytes to byte area at tail                                   */
/*----------------------------------------------------------------------------*/
template<int SIZE>
void WspConverge_t<SIZE>::vAppendTail(void* aSrc, i8 nBytes, eCN eClassNo)
{
  ASSERT(nUsed() + nBytes <= SIZE);
  
  byte* ayDst = ayLatterTail() - nBytes + 1;
  
  MemLib::vCopy(ayDst,aSrc,nBytes,eClassNo);
  
  vTakeTail(nBytes);
}

/*----------------------------------------------------------------------------*/
/* get address of byte area                                                   */
/*----------------------------------------------------------------------------*/
template<int SIZE>
byte* WspConverge_t<SIZE>::ayArea(void)
{
  byte* ayArea = m_yaByteArea;
  
  return ayArea;
}

/*----------------------------------------------------------------------------*/
/* fill byte area with given byte                                             */
/*----------------------------------------------------------------------------*/
template<int SIZE>
void WspConverge_t<SIZE>::vFill(char cByte, eCN eClassNo)
{
  byte* ayDst = ayArea();
  
  MemLib::vSet(ayDst,cByte,SIZE,eClassNo);
}

/*----------------------------------------------------------------------------*/
/* get number of free bytes in byte area                                      */
/*----------------------------------------------------------------------------*/
template<int SIZE>
i8 WspConverge_t<SIZE>::nFree(void)
{
  i8 nFree = SIZE - m_nUsedHead - m_nUsedTail;
  ASSERT(nFree >= 0);
  
  return nFree;
}

/*----------------------------------------------------------------------------*/
/* give chunk back to byte area at head                                       */
/*----------------------------------------------------------------------------*/
template<int SIZE>
void WspConverge_t<SIZE>::vGiveHead(i8 nBytes)
{
  ASSERT(m_nUsedHead  + m_nUsedTail <= SIZE);

  m_nUsedHead -= nBytes;
}

/*----------------------------------------------------------------------------*/
/* give chunk back to byte area at tail                                       */
/*----------------------------------------------------------------------------*/
template<int SIZE>
void WspConverge_t<SIZE>::vGiveTail(i8 nBytes)
{
  ASSERT(m_nUsedHead  + m_nUsedTail <= SIZE);
  
  m_nUsedTail -= nBytes;
}

/*----------------------------------------------------------------------------*/
/* get address of next available chunk in byte areafrom head                  */
/*----------------------------------------------------------------------------*/
template<int SIZE>
byte* WspConverge_t<SIZE>::ayLatterHead(void)
{
  byte* ayNext = &ayArea()[m_nUsedHead];
  
  return ayNext;
}

/*----------------------------------------------------------------------------*/
/* get address of next available chunk in byte areafrom head                  */
/*----------------------------------------------------------------------------*/
template<int SIZE>
byte* WspConverge_t<SIZE>::ayLatterTail(void)
{
  byte* ayNext = &ayArea()[SIZE - nUsedTail() - 1];
  
  return ayNext;
}

/*----------------------------------------------------------------------------*/
/* get length of byte area                                                    */
/*----------------------------------------------------------------------------*/
template<int SIZE>
i8 WspConverge_t<SIZE>::nLength(void)
{
  return SIZE;
}

/*----------------------------------------------------------------------------*/
/* get offset of an allotted                                                  */
/*----------------------------------------------------------------------------*/
template<int SIZE>
i8 WspConverge_t<SIZE>::iOffset(byte* ayAllotted)
{
  i8 iOfs = nDifference(ayAllotted,m_yaByteArea);
  
  return iOfs;
}

/*----------------------------------------------------------------------------*/
/* reset this wsp                                                             */
/*----------------------------------------------------------------------------*/
template<int SIZE>
void WspConverge_t<SIZE>::vReset(void)
{
  vUsedHead(0);
  vUsedTail(0);
}

/*----------------------------------------------------------------------------*/
/* get total size of this Wsp_t                                               */
/*----------------------------------------------------------------------------*/
template<int SIZE>
i8 WspConverge_t<SIZE>::nSize(void)
{
  return sizeof(WspConverge_t<SIZE>);
}

/*----------------------------------------------------------------------------*/
/* get byte area as Stz*                                                      */
/*----------------------------------------------------------------------------*/
template<int SIZE>
Stz* WspConverge_t<SIZE>::szStz(void)
{
  char* aArea = (char*)m_yaByteArea;
  
 return reinterpret_cast<Stz*>(aArea);
}

/*----------------------------------------------------------------------------*/
/* take chunk from byte area from the head                                    */
/*----------------------------------------------------------------------------*/
template<int SIZE>
void WspConverge_t<SIZE>::vTakeHead(i8 nBytes)
{
  m_nUsedHead += nBytes;
  
  if (m_nUsedHead  + m_nUsedTail > SIZE) {
    vDebugPoint();
  }
  ASSERT(m_nUsedHead + m_nUsedTail <= SIZE);
}

/*----------------------------------------------------------------------------*/
/* take chunk from byte area from the tail                                    */
/*----------------------------------------------------------------------------*/
template<int SIZE>
void WspConverge_t<SIZE>::vTakeTail(i8 nBytes)
{
  m_nUsedTail += nBytes;
  
  if (m_nUsedHead  + m_nUsedTail > SIZE) {
    vDebugPoint();
  }
  ASSERT(m_nUsedHead + m_nUsedTail <= SIZE);
}

/*----------------------------------------------------------------------------*/
/* get number of used bytes                                                   */
/*----------------------------------------------------------------------------*/
template<int SIZE>
i8 WspConverge_t<SIZE>::nUsed(void)
{
  return m_nUsedHead + m_nUsedTail;
}

/*----------------------------------------------------------------------------*/
/* get number of used bytes                                                   */
/*----------------------------------------------------------------------------*/
template<int SIZE>
i8 WspConverge_t<SIZE>::nUsedHead(void)
{
  return m_nUsedHead;
}

/*----------------------------------------------------------------------------*/
/* get number of used bytes                                                   */
/*----------------------------------------------------------------------------*/
template<int SIZE>
i8 WspConverge_t<SIZE>::nUsedTail(void)
{
  return m_nUsedTail;
}

/*----------------------------------------------------------------------------*/
/* set number of used bytes at head                                           */
/*----------------------------------------------------------------------------*/
template<int SIZE>
void WspConverge_t<SIZE>::vUsedHead(i8 nBytes)
{
  m_nUsedHead = nBytes;
  
  ASSERT(m_nUsedHead + m_nUsedTail <= SIZE);
}

/*----------------------------------------------------------------------------*/
/* set number of used bytes at tail                                           */
/*----------------------------------------------------------------------------*/
template<int SIZE>
void WspConverge_t<SIZE>::vUsedTail(i8 nBytes)
{
  m_nUsedTail = nBytes;
  
  ASSERT(m_nUsedHead + m_nUsedTail <= SIZE);
}

/*----------------------------------------------------------------------------*/
/* zero fill the byte                                                         */
/*----------------------------------------------------------------------------*/
template<int SIZE>
void WspConverge_t<SIZE>::vZero(eCN eClassNo)
{
  m_nUsedHead = 0;
  m_nUsedTail = 0;
  
  byte* ayDst = ayArea();
  
  MemLib::vZero(ayDst,SIZE,eClassNo);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
