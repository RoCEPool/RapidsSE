/*============================================================================*/
/* WspConvergent_t.hpp -- fixed-sized workspace that converges from both ends */
/*============================================================================*/
#pragma once


namespace Alignment
{
  template<int LENGTH>
  class WspConvergent_t : Object
  {
  private:
    byte m_yaByteArea[LENGTH]; // fixed byte area
    i8   m_nUsedHead;          // bytes used in the area from the head
    i8   m_nUsedTail;          // bytes used in the area frim the tail
                               //  (must be after m_yaByteArea)
    i8   m_nItems;             // number of items in this wsp

  public:
    WspConvergent_t(void);

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
    i8    nItems(void);
    void  vItemsDec(void);
    void  vItemsInc(void);
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
WspConvergent_t<LENGTH>::WspConvergent_t(void)
: Object(CN_WSPCONVERGENT_t)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_WSPCONVERGENT_t);
  
  m_nUsedHead = 0;
  m_nUsedTail = 0;
  m_nItems    = 0;
  
  _postcondition_(_ADDRESS_,_MAGIC_,CN_WSPCONVERGENT_t);
}

/*----------------------------------------------------------------------------*/
/* init                                                                       */
/*----------------------------------------------------------------------------*/
template<int LENGTH>
void WspConvergent_t<LENGTH>::vInit(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_WSPCONVERGENT_t);

  m_nUsedHead = 0;
  m_nUsedTail = 0;
}

/*----------------------------------------------------------------------------*/
/* allot bytes from byte area                                                 */
/*----------------------------------------------------------------------------*/
template<int LENGTH>
byte* WspConvergent_t<LENGTH>::ayAllotHead(i8 nBytes)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_WSPCONVERGENT_t);

  byte* ayAllotted = ayLatterHead();
  
  vTakeHead(nBytes);
  
  return ayAllotted;
}

/*----------------------------------------------------------------------------*/
/* allot bytes from byte area                                                 */
/*----------------------------------------------------------------------------*/
template<int LENGTH>
byte* WspConvergent_t<LENGTH>::ayAllotTail(i8 nBytes)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_WSPCONVERGENT_t);

  byte* ayAllotted = ayLatterTail() - nBytes;
  
  vTakeTail(nBytes);
  
  return ayAllotted;
}

/*----------------------------------------------------------------------------*/
/* append source bytes to byte area at head                                   */
/*----------------------------------------------------------------------------*/
template<int LENGTH>
void WspConvergent_t<LENGTH>::vAppendHead(void* aSrc, i8 nBytes, eCN eClassNo)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_WSPCONVERGENT_t);

  ASSERT(nUsed() + nBytes <= LENGTH);
  
  byte* ayDst = ayLatterHead();

  MemLib::vCopy(ayDst,aSrc,nBytes,eClassNo);
  
  vTakeHead(nBytes);
}

/*----------------------------------------------------------------------------*/
/* append source bytes to byte area at tail                                   */
/*----------------------------------------------------------------------------*/
template<int LENGTH>
void WspConvergent_t<LENGTH>::vAppendTail(void* aSrc, i8 nBytes, eCN eClassNo)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_WSPCONVERGENT_t);

  ASSERT(nUsed() + nBytes <= LENGTH);
  
  byte* ayDst = ayLatterTail() - nBytes + 1;
  
  MemLib::vCopy(ayDst,aSrc,nBytes,eClassNo);
  
  vTakeTail(nBytes);
}

/*----------------------------------------------------------------------------*/
/* get address of byte area                                                   */
/*----------------------------------------------------------------------------*/
template<int LENGTH>
byte* WspConvergent_t<LENGTH>::ayArea(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_WSPCONVERGENT_t);

  byte* ayArea = m_yaByteArea;
  
  return ayArea;
}

/*----------------------------------------------------------------------------*/
/* fill byte area with given byte                                             */
/*----------------------------------------------------------------------------*/
template<int LENGTH>
void WspConvergent_t<LENGTH>::vFill(char cByte, eCN eClassNo)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_WSPCONVERGENT_t);

  byte* ayDst = ayArea();
  
  MemLib::vSet(ayDst,cByte,LENGTH,eClassNo);
}

/*----------------------------------------------------------------------------*/
/* get number of free bytes in byte area                                      */
/*----------------------------------------------------------------------------*/
template<int LENGTH>
i8 WspConvergent_t<LENGTH>::nFree(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_WSPCONVERGENT_t);

  i8 nFree = LENGTH - m_nUsedHead - m_nUsedTail;
  ASSERT(nFree >= 0);
  
  return nFree;
}

/*----------------------------------------------------------------------------*/
/* give chunk back to byte area at head                                       */
/*----------------------------------------------------------------------------*/
template<int LENGTH>
void WspConvergent_t<LENGTH>::vGiveHead(i8 nBytes)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_WSPCONVERGENT_t);

  ASSERT(m_nUsedHead  + m_nUsedTail <= LENGTH);

  m_nUsedHead -= nBytes;
}

/*----------------------------------------------------------------------------*/
/* give chunk back to byte area at tail                                       */
/*----------------------------------------------------------------------------*/
template<int LENGTH>
void WspConvergent_t<LENGTH>::vGiveTail(i8 nBytes)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_WSPCONVERGENT_t);

  ASSERT(m_nUsedHead  + m_nUsedTail <= LENGTH);
  
  m_nUsedTail -= nBytes;
}

/*----------------------------------------------------------------------------*/
/* get number of items                                                        */
/*----------------------------------------------------------------------------*/
template<int LENGTH>
i8 WspConvergent_t<LENGTH>::nItems(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_WSPCONVERGENT_t);

  return m_nItems;
}

/*----------------------------------------------------------------------------*/
/* dec number of items                                                        */
/*----------------------------------------------------------------------------*/
template<int LENGTH>
void WspConvergent_t<LENGTH>::vItemsDec(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_WSPCONVERGENT_t);

  --m_nItems;
}

/*----------------------------------------------------------------------------*/
/* inc number of items                                                        */
/*----------------------------------------------------------------------------*/
template<int LENGTH>
void WspConvergent_t<LENGTH>::vItemsInc(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_WSPCONVERGENT_t);

  ++m_nItems;
}

/*----------------------------------------------------------------------------*/
/* get address of next available chunk in byte areafrom head                  */
/*----------------------------------------------------------------------------*/
template<int LENGTH>
byte* WspConvergent_t<LENGTH>::ayLatterHead(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_WSPCONVERGENT_t);

  byte* ayNext = &ayArea()[m_nUsedHead];
  
  return ayNext;
}

/*----------------------------------------------------------------------------*/
/* get address of next available chunk in byte areafrom head                  */
/*----------------------------------------------------------------------------*/
template<int LENGTH>
byte* WspConvergent_t<LENGTH>::ayLatterTail(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_WSPCONVERGENT_t);

  byte* ayNext = &ayArea()[LENGTH - nUsedTail() - 1];
  
  return ayNext;
}

/*----------------------------------------------------------------------------*/
/* get length of byte area                                                    */
/*----------------------------------------------------------------------------*/
template<int LENGTH>
i8 WspConvergent_t<LENGTH>::nLength(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_WSPCONVERGENT_t);

  return LENGTH;
}

/*----------------------------------------------------------------------------*/
/* get offset of an allotted                                                  */
/*----------------------------------------------------------------------------*/
template<int LENGTH>
i8 WspConvergent_t<LENGTH>::iOffset(byte* ayAllotted)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_WSPCONVERGENT_t);

  i8 iOfs = nDifference(ayAllotted,m_yaByteArea);
  
  return iOfs;
}

/*----------------------------------------------------------------------------*/
/* reset this wsp                                                             */
/*----------------------------------------------------------------------------*/
template<int LENGTH>
void WspConvergent_t<LENGTH>::vReset(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_WSPCONVERGENT_t);

  vUsedHead(0);
  vUsedTail(0);
}

/*----------------------------------------------------------------------------*/
/* get total size of this Wsp_t                                               */
/*----------------------------------------------------------------------------*/
template<int LENGTH>
i8 WspConvergent_t<LENGTH>::nSize(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_WSPCONVERGENT_t);

  return sizeof(WspConvergent_t<LENGTH>);
}

/*----------------------------------------------------------------------------*/
/* get byte area as Stz*                                                      */
/*----------------------------------------------------------------------------*/
template<int LENGTH>
Stz* WspConvergent_t<LENGTH>::szStz(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_WSPCONVERGENT_t);

  char* aArea = (char*)m_yaByteArea;
  
 return reinterpret_cast<Stz*>(aArea);
}

/*----------------------------------------------------------------------------*/
/* take chunk from byte area from the head                                    */
/*----------------------------------------------------------------------------*/
template<int LENGTH>
void WspConvergent_t<LENGTH>::vTakeHead(i8 nBytes)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_WSPCONVERGENT_t);

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
void WspConvergent_t<LENGTH>::vTakeTail(i8 nBytes)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_WSPCONVERGENT_t);

  m_nUsedTail += nBytes;
  
  if (m_nUsedHead  + m_nUsedTail > LENGTH) {
    vDebugPoint();
  }
  ASSERT(m_nUsedHead + m_nUsedTail <= LENGTH);
}

/*----------------------------------------------------------------------------*/
/* get number of used bytes                                                   */
/*----------------------------------------------------------------------------*/
template<int LENGTH>
i8 WspConvergent_t<LENGTH>::nUsed(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_WSPCONVERGENT_t);

  return m_nUsedHead + m_nUsedTail;
}

/*----------------------------------------------------------------------------*/
/* get number of used bytes                                                   */
/*----------------------------------------------------------------------------*/
template<int LENGTH>
i8 WspConvergent_t<LENGTH>::nUsedHead(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_WSPCONVERGENT_t);

  return m_nUsedHead;
}

/*----------------------------------------------------------------------------*/
/* get number of used bytes                                                   */
/*----------------------------------------------------------------------------*/
template<int LENGTH>
i8 WspConvergent_t<LENGTH>::nUsedTail(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_WSPCONVERGENT_t);

  return m_nUsedTail;
}

/*----------------------------------------------------------------------------*/
/* set number of used bytes at head                                           */
/*----------------------------------------------------------------------------*/
template<int LENGTH>
void WspConvergent_t<LENGTH>::vUsedHead(i8 nBytes)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_WSPCONVERGENT_t);

  m_nUsedHead = nBytes;
  
  ASSERT(m_nUsedHead + m_nUsedTail <= LENGTH);
}

/*----------------------------------------------------------------------------*/
/* set number of used bytes at tail                                           */
/*----------------------------------------------------------------------------*/
template<int LENGTH>
void WspConvergent_t<LENGTH>::vUsedTail(i8 nBytes)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_WSPCONVERGENT_t);

  m_nUsedTail = nBytes;
  
  ASSERT(m_nUsedHead + m_nUsedTail <= LENGTH);
}

/*----------------------------------------------------------------------------*/
/* zero fill the byte                                                         */
/*----------------------------------------------------------------------------*/
template<int LENGTH>
void WspConvergent_t<LENGTH>::vZero(eCN eClassNo)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_WSPCONVERGENT_t);

  m_nUsedHead = 0;
  m_nUsedTail = 0;
  
  byte* ayDst = ayArea();
  
  MemLib::vZero(ayDst,LENGTH,eClassNo);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
