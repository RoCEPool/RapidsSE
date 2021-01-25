/*============================================================================*/
/* WspConvergent_t.hpp -- fixed-sized workspace that converges from both ends */
/*============================================================================*/
#pragma once

namespace BASE
{
  template<i8 AREA, class HEAD, class TAIL>
  class WspConvergent_t
  {
  private:
    byte  m_yaByteArea[AREA]; // allottable area
    i8    m_nUsedHead;    // bytes used in the area from the head
    i8    m_nUsedTail;    // bytes used in the area frim the tail

    byte* m_aAuxItem0;    // auxillary item 0
    byte* m_aAuxItem1;    // auxillary item 1
  
    i8    m_nTailItems;   // number of fixed-size tail items in this wsp
    i8    m_nTailSize;    // size of each fixed-size tail item

  public:
    WspConvergent_t(void);

    TAIL* aAdjacentTail(TAIL* aPriorItem, i8 nItemBytes);
    HEAD* ayAllotHead(i8 nBytes);
    TAIL* ayAllotTail(i8 nBytes);
    void  vDecItems(void);
    TAIL* aFirstItem(void); // ayFirstTail()
    i8    nFreeBytes(void);
    void  vGiveHead(i8 nBytes);
    void  vGiveTail(i8 nBytes);
    bool  bHasSpaceFor(i8 nBytes);
    void  vInit(void);
    void  vIncItems(void);
    TAIL* aItemAtOffset(i8 i_Ofs);
    i8    nItems(void);
    HEAD* aLastItem(void); // ayLasHead()
    byte* ayLatterHead(void);
    byte* ayLatterTail(void);
    i8    iOffsetOfItem(TAIL* ayAllotted);
    Stz*  szStzAllot_begin(void);
    void  vStzAllot_end(i8 nSize);
    Stz*  szStzDeposit(Stz* szValue);
    TAIL* aTailItem(i8 i_Tail);
    void  vTakeHead(i8 nBytes);
    void  vTakeTail(i8 nBytes);
    i8    nUsedBytes(void);
    i8    nUsedHead(void);
    i8    nUsedTail(void);
    void  vUsedHead(i8 nBytes);
    void  vUsedTail(i8 nBytes);
  
  private:
    byte* ayArea(void);

  };
}

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
template<i8 AREA, class HEAD, class TAIL>
WspConvergent_t<AREA,HEAD,TAIL>::WspConvergent_t(void)
{
#ifdef WITH_CHECK_INITIALISED
  MemLib::vZero(m_yaByteArea,AREA,CN_NONE);
#endif
  
  m_nUsedHead  = 0;
  m_nUsedTail  = 0;

  m_aAuxItem0  = nullptr;
  m_aAuxItem1  = nullptr;
  
  m_nTailItems = 0;
  m_nTailSize  = C_UNKNOWN;
}

/*----------------------------------------------------------------------------*/
/* init                                                                       */
/*----------------------------------------------------------------------------*/
template<i8 AREA, class HEAD, class TAIL>
void WspConvergent_t<AREA,HEAD,TAIL>::vInit(void)
{
#ifdef WITH_CHECK_INITIALISED
  MemLib::vZero(m_yaByteArea,AREA,CN_NONE);
#endif
  
  m_nUsedHead  = 0;
  m_nUsedTail  = 0;

  m_aAuxItem0  = nullptr;
  m_aAuxItem1  = nullptr;
  
  m_nTailItems = 0;
  m_nTailSize  = C_UNKNOWN;
}

/*----------------------------------------------------------------------------*/
/* get item which is memory-wise the previous item in the tail area           */
/*----------------------------------------------------------------------------*/
template<i8 AREA, class HEAD, class TAIL>
TAIL* WspConvergent_t<AREA,HEAD,TAIL>::aAdjacentTail(TAIL* aPriorItem,
                                                     i8 nSizeItem)
{
  
  ASSERT(nSizeItem == m_nTailSize);

  TAIL* aAdjacent = nullptr;
    
                                                     // -----------------
  aAdjacent = aAdd<TAIL*>(aPriorItem, - nSizeItem ); // note the -ve sign
                                                     // -----------------
  if ((char*)aAdjacent < (char*)ayLatterTail())
    return nullptr;
  
  return aAdjacent;
}

/*----------------------------------------------------------------------------*/
/* allot a variable-size head from byte area                                  */
/*----------------------------------------------------------------------------*/
template<i8 AREA, class HEAD, class TAIL>
HEAD* WspConvergent_t<AREA,HEAD,TAIL>::ayAllotHead(i8 nBytes)
{

  ASSERT(m_nUsedHead < C_1MB);
  
  ASSERT(nBytes >= 2);

  /*
          Variable Heads (9 heads)              Fixed Tails (4 tails)
     +-------------------------------+          +-------------------+
     | : :   :    :  : :     :   :  t|//////////|h   :    :    :    |
     +-------------------------------+          +-------------------+
                          latterTail^            ^latterHead
        (last used byte in head area)            (first used byte in tail area)
   
                        allot head -->          <-- allot tails
   */
  
  byte* ayAllottedHead = ayLatterHead() + 1;
  
  vTakeHead(nBytes);
    
  return (HEAD*)ayAllottedHead;
}

/*----------------------------------------------------------------------------*/
/* allot a fixed size tail from byte area                                     */
/*----------------------------------------------------------------------------*/
template<i8 AREA, class HEAD, class TAIL>
TAIL* WspConvergent_t<AREA,HEAD,TAIL>::ayAllotTail(i8 nBytes)
{
  ASSERT(m_nUsedHead < C_1MB);
  
  ASSERT(nBytes >= 16);

  if (C_UNKNOWN == m_nTailSize) {
    m_nTailSize = nBytes;
  }
  else {
    ASSERT(nBytes == m_nTailSize);
  }

  vTakeTail(nBytes);
  
  byte* ayAllottedTail = ayLatterTail();
  
  ++ m_nTailItems;

  return (TAIL*)ayAllottedTail;
}

/*----------------------------------------------------------------------------*/
/* get address of byte area                                                   */
/*----------------------------------------------------------------------------*/
template<i8 AREA, class HEAD, class TAIL>
byte* WspConvergent_t<AREA,HEAD,TAIL>::ayArea(void)
{
  ASSERT(m_nUsedHead < C_1MB);

  byte* ayArea = m_yaByteArea;

  return ayArea;
}

/*----------------------------------------------------------------------------*/
/* dec number of items                                                        */
/*----------------------------------------------------------------------------*/
template<i8 AREA, class HEAD, class TAIL>
void WspConvergent_t<AREA,HEAD,TAIL>::vDecItems(void)
{
  ASSERT(m_nUsedHead < C_1MB);


  // I DONT THINK THIS FUNCTION IS NEEDED
  
  --m_nTailItems;
}

/*----------------------------------------------------------------------------*/
/* get address of byte area                                                   */
/*----------------------------------------------------------------------------*/
template<i8 AREA, class HEAD, class TAIL>
TAIL* WspConvergent_t<AREA,HEAD,TAIL>::aFirstItem(void)
{
  ASSERT(m_nUsedHead < C_1MB);

  if (! m_nTailItems)
    return nullptr;
  
  ASSERT(C_UNKNOWN != m_nTailSize);
  
  byte* ayFirst = &ayArea()[AREA - m_nTailSize];
  
  return (TAIL*)ayFirst;
}

/*----------------------------------------------------------------------------*/
/* get number of free bytes in byte area                                      */
/*----------------------------------------------------------------------------*/
template<i8 AREA, class HEAD, class TAIL>
i8 WspConvergent_t<AREA,HEAD,TAIL>::nFreeBytes(void)
{
  ASSERT(m_nUsedHead < C_1MB);

  i8 nFree = AREA - m_nUsedHead - m_nUsedTail;
  ASSERT(nFree >= 0);
  
  return nFree;
}

/*----------------------------------------------------------------------------*/
/* give chunk back to byte area at head                                       */
/*----------------------------------------------------------------------------*/
template<i8 AREA, class HEAD, class TAIL>
void WspConvergent_t<AREA,HEAD,TAIL>::vGiveHead(i8 nBytes)
{
  ASSERT(m_nUsedHead < C_1MB);

  // PRIVATE?

  ASSERT(m_nUsedHead  + m_nUsedTail <= AREA);

  m_nUsedHead -= nBytes;
}

/*----------------------------------------------------------------------------*/
/* give chunk back to byte area at tail                                       */
/*----------------------------------------------------------------------------*/
template<i8 AREA, class HEAD, class TAIL>
void WspConvergent_t<AREA,HEAD,TAIL>::vGiveTail(i8 nBytes)
{
  ASSERT(m_nUsedHead < C_1MB);

  // PRIVATE?
  
  ASSERT(m_nUsedHead  + m_nUsedTail <= AREA);
  
  m_nUsedTail -= nBytes;
}

/*----------------------------------------------------------------------------*/
/* does this workspace have enough free space for given size                  */
/*----------------------------------------------------------------------------*/
template<i8 AREA, class HEAD, class TAIL>
bool WspConvergent_t<AREA,HEAD,TAIL>::bHasSpaceFor(i8 nSizeBytes)
{
  ASSERT(m_nUsedHead < C_1MB);

  if (nFreeBytes() >= nSizeBytes)
    return true;
  else
    return false;
}

/*----------------------------------------------------------------------------*/
/* inc number of items                                                        */
/*----------------------------------------------------------------------------*/
template<i8 AREA, class HEAD, class TAIL>
void WspConvergent_t<AREA,HEAD,TAIL>::vIncItems(void)
{
  ASSERT(m_nUsedHead < C_1MB);

  // I DONT THINK THIS FUNCTION IS NEEDED

  ++m_nTailItems;
}

/*----------------------------------------------------------------------------*/
/* get tail item at given offset                                              */
/*----------------------------------------------------------------------------*/
template<i8 AREA, class HEAD, class TAIL>
TAIL* WspConvergent_t<AREA,HEAD,TAIL>::aItemAtOffset(i8 i_Ofs)
{
  ASSERT(m_nUsedHead < C_1MB);

  TAIL* aTail = aAdd<TAIL*>(WspConvergent_t<AREA,Stz,Row>::ayArea(),i_Ofs);

  return aTail;
}

/*----------------------------------------------------------------------------*/
/* get number of items                                                        */
/*----------------------------------------------------------------------------*/
template<i8 AREA, class HEAD, class TAIL>
i8 WspConvergent_t<AREA,HEAD,TAIL>::nItems(void)
{
  ASSERT(m_nUsedHead < C_1MB);

  return m_nTailItems;
}

/*----------------------------------------------------------------------------*/
/* get address of last used byte in head area                                 */
/*----------------------------------------------------------------------------*/
template<i8 AREA, class HEAD, class TAIL>
byte* WspConvergent_t<AREA,HEAD,TAIL>::ayLatterHead(void)
{
  ASSERT(m_nUsedHead < C_1MB);

  byte* ayNext = &ayArea()[m_nUsedHead - 1];
  
  return ayNext;
}

/*----------------------------------------------------------------------------*/
/* get address of last used byte in tail area                                 */
/*----------------------------------------------------------------------------*/
template<i8 AREA, class HEAD, class TAIL>
byte* WspConvergent_t<AREA,HEAD,TAIL>::ayLatterTail(void)
{
  ASSERT(m_nUsedHead < C_1MB);

  byte* ayNext = &ayArea()[AREA - nUsedTail()];
  
  return ayNext;
}

/*----------------------------------------------------------------------------*/
/* get offset of an allotted                                                  */
/*----------------------------------------------------------------------------*/
template<i8 AREA, class HEAD, class TAIL>
i8 WspConvergent_t<AREA,HEAD,TAIL>::iOffsetOfItem(TAIL* ayAllotted)
{
  ASSERT(m_nUsedHead < C_1MB);

  // I THINK THIS SHOULD BE i_Offset()

  i8 i_Ofs = nDifference(ayAllotted,m_yaByteArea);
  
  return i_Ofs;
}

/*----------------------------------------------------------------------------*/
/* begin allot of stz (of unknown size) in head                               */
/*----------------------------------------------------------------------------*/
template<i8 AREA, class HEAD, class TAIL>
Stz* WspConvergent_t<AREA,HEAD,TAIL>::szStzAllot_begin(void)
{
  ASSERT(m_nUsedHead < C_1MB);

  Stz* szStz = (Stz*)ayLatterHead();
  
  return szStz;
}

/*----------------------------------------------------------------------------*/
/* end allot of stz in head (size now known)                                  */
/*----------------------------------------------------------------------------*/
template<i8 AREA, class HEAD, class TAIL>
void WspConvergent_t<AREA,HEAD,TAIL>::vStzAllot_end(i8 nSize)
{
  ASSERT(m_nUsedHead < C_1MB);

  vTakeHead(nSize);
}

/*----------------------------------------------------------------------------*/
/* deposit given stz                                                          */
/*----------------------------------------------------------------------------*/
template<i8 AREA, class HEAD, class TAIL>
Stz* WspConvergent_t<AREA,HEAD,TAIL>::szStzDeposit(Stz* szValue)
{
  ASSERT(m_nUsedHead < C_1MB);

  i8 nSize = szValue->nSize();
  
  Stz* szAllottedStz = szStzAllot_begin();
  szAllottedStz->vCopy(szValue);
  vStzAllot_end(nSize);
  
  return szAllottedStz;
}

/*----------------------------------------------------------------------------*/
/* get address of byte area                                                   */
/*----------------------------------------------------------------------------*/
template<i8 AREA, class HEAD, class TAIL>
TAIL* WspConvergent_t<AREA,HEAD,TAIL>::aTailItem(i8 i_Tail)
{
  ASSERT(m_nUsedHead < C_1MB);

  if (! m_nTailItems)
    return nullptr;
  
  ASSERT(C_UNKNOWN != m_nTailSize);
  
  i8 i_Area_right = (i_Tail + 1) * m_nTailSize;
  byte* ayItem = &ayArea()[AREA - i_Area_right];
  
  return (TAIL*)ayItem;
}

/*----------------------------------------------------------------------------*/
/* take chunk from byte area from the head                                    */
/*----------------------------------------------------------------------------*/
template<i8 AREA, class HEAD, class TAIL>
void WspConvergent_t<AREA,HEAD,TAIL>::vTakeHead(i8 nBytes)
{
  // PRIVATE?
  ASSERT(m_nUsedHead < C_1MB);


  m_nUsedHead += nBytes;
  
  if (m_nUsedHead  + m_nUsedTail > AREA) {
    vDebugPoint();
  }
  ASSERT(m_nUsedHead + m_nUsedTail <= AREA);
}

/*----------------------------------------------------------------------------*/
/* take chunk from byte area from the tail                                    */
/*----------------------------------------------------------------------------*/
template<i8 AREA, class HEAD, class TAIL>
void WspConvergent_t<AREA,HEAD,TAIL>::vTakeTail(i8 nBytes)
{
  ASSERT(m_nUsedHead < C_1MB);

  // PRIVATE?

  m_nUsedTail += nBytes;
  
  if (m_nUsedHead  + m_nUsedTail > AREA) {
    vDebugPoint();
  }
  ASSERT(m_nUsedHead + m_nUsedTail <= AREA);
}

/*----------------------------------------------------------------------------*/
/* get number of used bytes                                                   */
/*----------------------------------------------------------------------------*/
template<i8 AREA, class HEAD, class TAIL>
i8 WspConvergent_t<AREA,HEAD,TAIL>::nUsedBytes(void)
{
  ASSERT(m_nUsedHead < C_1MB);

  return m_nUsedHead + m_nUsedTail;
}

/*----------------------------------------------------------------------------*/
/* get number of used bytes                                                   */
/*----------------------------------------------------------------------------*/
template<i8 AREA, class HEAD, class TAIL>
i8 WspConvergent_t<AREA,HEAD,TAIL>::nUsedHead(void)
{
  ASSERT(m_nUsedHead < C_1MB);

  return m_nUsedHead;
}

/*----------------------------------------------------------------------------*/
/* get number of used bytes                                                   */
/*----------------------------------------------------------------------------*/
template<i8 AREA, class HEAD, class TAIL>
i8 WspConvergent_t<AREA,HEAD,TAIL>::nUsedTail(void)
{
  ASSERT(m_nUsedHead < C_1MB);

  return m_nUsedTail;
}

/*----------------------------------------------------------------------------*/
/* set number of used bytes at head                                           */
/*----------------------------------------------------------------------------*/
template<i8 AREA, class HEAD, class TAIL>
void WspConvergent_t<AREA,HEAD,TAIL>::vUsedHead(i8 nBytes)
{
  ASSERT(m_nUsedHead < C_1MB);

  m_nUsedHead = nBytes;
  
  ASSERT(m_nUsedHead + m_nUsedTail <= AREA);
}

/*----------------------------------------------------------------------------*/
/* set number of used bytes at tail                                           */
/*----------------------------------------------------------------------------*/
template<i8 AREA, class HEAD, class TAIL>
void WspConvergent_t<AREA,HEAD,TAIL>::vUsedTail(i8 nBytes)
{
  ASSERT(m_nUsedHead < C_1MB);

  m_nUsedTail = nBytes;
  
  ASSERT(m_nUsedHead + m_nUsedTail <= AREA);
}

/*----------------------------------------------------------------------------*/
/*-- private -----------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
