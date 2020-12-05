/*============================================================================*/
/* WspConvergent_t.hpp -- fixed-sized workspace that converges from both ends */
/*============================================================================*/
#pragma once

namespace Alignment
{
  template<i8 SIZE, class HEAD, class TAIL>
  class WspConvergent_t
  {
  private:
    byte m_yaByteArea[SIZE]; // allottable area
  
    i8   m_nUsedHead;    // bytes used in the area from the head
  
    i8   m_nTailItems;   // number of fixed-size items in this wsp
    i8   m_nUsedTail;    // bytes used in the area frim the tail
    i8   m_nTailSize;    // size of each fixed-size tail item

  public:
    WspConvergent_t(void);

    TAIL* aAdjacentItem(TAIL* aPriorItem, i8 nItemBytes); // ayNextHead()
    TAIL* aAdjacentItem(TAIL* aPriorItem); // ayNextHead()
    HEAD* ayAllotHead(i8 nBytes);
    TAIL* ayAllotTail(i8 nBytes);
    byte* ayArea(void);
    void  vDecItems(void);
    void  vFill(char cByte, eCN eClassNo);
    TAIL* aFirstItem(void); // ayFirstTail()
    i8    nFree(void);
    i8    nFreeBytes(void);
    void  vGiveHead(i8 nBytes);
    void  vGiveTail(i8 nBytes);
    bool  bHasSpaceFor(i8 nBytes);
    void  vInit(void);
    void  vIncItems(void);
    i8    nItems(void);
    HEAD* aLastItem(void); // ayLasHead()
    byte* ayLatterHead(void);
    byte* ayLatterTail(void);
    i8    nLength(void);
    void  vReset(void);
    i8    nSize(void);
    Stz*  szStzAllot_begin(void);
    void  vStzAllot_end(i8 nSize);
    Stz*  szStzArea(void);
    Stz*  szStzDeposit(Stz* szValue);
    TAIL* aTailItem(i8 i_Tail);
    void  vTakeHead(i8 nBytes);
    void  vTakeTail(i8 nBytes);
    i8    iOffsetInWsp(byte* ayAllotted);
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
template<i8 SIZE, class HEAD, class TAIL>
WspConvergent_t<SIZE,HEAD,TAIL>::WspConvergent_t(void)
{
  m_nUsedHead  = 0;
  
  m_nTailItems = 0;
  m_nUsedTail  = 0;
  m_nTailSize  = C_UNKNOWN;
  
#ifdef WITH_CHECK_INITIALISED
  MemLib::vZero(m_yaByteArea,SIZE,CN_NONE);
#endif
}

/*----------------------------------------------------------------------------*/
/* init                                                                       */
/*----------------------------------------------------------------------------*/
template<i8 SIZE, class HEAD, class TAIL>
void WspConvergent_t<SIZE,HEAD,TAIL>::vInit(void)
{
  m_nUsedHead  = 0;
  
  m_nTailItems = 0;
  m_nUsedTail  = 0;
  m_nTailSize  = C_UNKNOWN;
  
#ifdef WITH_CHECK_INITIALISED
  MemLib::vZero(m_yaByteArea,SIZE,CN_NONE);
#endif
}



/*----------------------------------------------------------------------------*/
/* get item which is memory-wise the previous item in the tail area           */
/*----------------------------------------------------------------------------*/
template<i8 SIZE, class HEAD, class TAIL>
TAIL* WspConvergent_t<SIZE,HEAD,TAIL>::aAdjacentItem(TAIL* aPriorItem,
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
template<i8 SIZE, class HEAD, class TAIL>
HEAD* WspConvergent_t<SIZE,HEAD,TAIL>::ayAllotHead(i8 nBytes)
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
template<i8 SIZE, class HEAD, class TAIL>
TAIL* WspConvergent_t<SIZE,HEAD,TAIL>::ayAllotTail(i8 nBytes)
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
template<i8 SIZE, class HEAD, class TAIL>
byte* WspConvergent_t<SIZE,HEAD,TAIL>::ayArea(void)
{
  ASSERT(m_nUsedHead < C_1MB);

  byte* ayArea = m_yaByteArea;
  
  return ayArea;
}

/*----------------------------------------------------------------------------*/
/* dec number of items                                                        */
/*----------------------------------------------------------------------------*/
template<i8 SIZE, class HEAD, class TAIL>
void WspConvergent_t<SIZE,HEAD,TAIL>::vDecItems(void)
{
  ASSERT(m_nUsedHead < C_1MB);


  // I DONT THINK THIS FUNCTION IS NEEDED
  
  --m_nTailItems;
}

/*----------------------------------------------------------------------------*/
/* fill byte area with given byte                                             */
/*----------------------------------------------------------------------------*/
template<i8 SIZE, class HEAD, class TAIL>
void WspConvergent_t<SIZE,HEAD,TAIL>::vFill(char cByte, eCN eClassNo)
{
  ASSERT(m_nUsedHead < C_1MB);


  // I DONT THINK THIS FUNCTION IS NEEDED

  byte* ayDst = ayArea();
  
  MemLib::vSet(ayDst,cByte,SIZE,eClassNo);
}

/*----------------------------------------------------------------------------*/
/* get address of byte area                                                   */
/*----------------------------------------------------------------------------*/
template<i8 SIZE, class HEAD, class TAIL>
TAIL* WspConvergent_t<SIZE,HEAD,TAIL>::aFirstItem(void)
{
  ASSERT(m_nUsedHead < C_1MB);

  if (! m_nTailItems)
    return nullptr;
  
  ASSERT(C_UNKNOWN != m_nTailSize);
  
  byte* ayFirst = &ayArea()[SIZE - m_nTailSize];
  
  return (TAIL*)ayFirst;
}

/*----------------------------------------------------------------------------*/
/* get number of free bytes in byte area                                      */
/*----------------------------------------------------------------------------*/
template<i8 SIZE, class HEAD, class TAIL>
i8 WspConvergent_t<SIZE,HEAD,TAIL>::nFree(void)
{
  ASSERT(m_nUsedHead < C_1MB);

  i8 nFree = SIZE - m_nUsedHead - m_nUsedTail;
  ASSERT(nFree >= 0);
  
  return nFree;
}

/*----------------------------------------------------------------------------*/
/* get number of free bytes in byte area                                      */
/*----------------------------------------------------------------------------*/
template<i8 SIZE, class HEAD, class TAIL>
i8 WspConvergent_t<SIZE,HEAD,TAIL>::nFreeBytes(void)
{
  ASSERT(m_nUsedHead < C_1MB);

  i8 nFree = SIZE - m_nUsedHead - m_nUsedTail;
  ASSERT(nFree >= 0);
  
  return nFree;
}

/*----------------------------------------------------------------------------*/
/* give chunk back to byte area at head                                       */
/*----------------------------------------------------------------------------*/
template<i8 SIZE, class HEAD, class TAIL>
void WspConvergent_t<SIZE,HEAD,TAIL>::vGiveHead(i8 nBytes)
{
  ASSERT(m_nUsedHead < C_1MB);

  // PRIVATE?

  ASSERT(m_nUsedHead  + m_nUsedTail <= SIZE);

  m_nUsedHead -= nBytes;
}

/*----------------------------------------------------------------------------*/
/* give chunk back to byte area at tail                                       */
/*----------------------------------------------------------------------------*/
template<i8 SIZE, class HEAD, class TAIL>
void WspConvergent_t<SIZE,HEAD,TAIL>::vGiveTail(i8 nBytes)
{
  ASSERT(m_nUsedHead < C_1MB);

  // PRIVATE?
  
  ASSERT(m_nUsedHead  + m_nUsedTail <= SIZE);
  
  m_nUsedTail -= nBytes;
}

/*----------------------------------------------------------------------------*/
/* does this workspace have enough free space for given size                  */
/*----------------------------------------------------------------------------*/
template<i8 SIZE, class HEAD, class TAIL>
bool WspConvergent_t<SIZE,HEAD,TAIL>::bHasSpaceFor(i8 nSizeBytes)
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
template<i8 SIZE, class HEAD, class TAIL>
void WspConvergent_t<SIZE,HEAD,TAIL>::vIncItems(void)
{
  ASSERT(m_nUsedHead < C_1MB);

  // I DONT THINK THIS FUNCTION IS NEEDED

  ++m_nTailItems;
}

/*----------------------------------------------------------------------------*/
/* get number of items                                                        */
/*----------------------------------------------------------------------------*/
template<i8 SIZE, class HEAD, class TAIL>
i8 WspConvergent_t<SIZE,HEAD,TAIL>::nItems(void)
{
  ASSERT(m_nUsedHead < C_1MB);

  return m_nTailItems;
}

/*----------------------------------------------------------------------------*/
/* get address of last used byte in head area                                 */
/*----------------------------------------------------------------------------*/
template<i8 SIZE, class HEAD, class TAIL>
byte* WspConvergent_t<SIZE,HEAD,TAIL>::ayLatterHead(void)
{
  ASSERT(m_nUsedHead < C_1MB);

  byte* ayNext = &ayArea()[m_nUsedHead - 1];
  
  return ayNext;
}

/*----------------------------------------------------------------------------*/
/* get address of first used byte in head area                                */
/*----------------------------------------------------------------------------*/
template<i8 SIZE, class HEAD, class TAIL>
byte* WspConvergent_t<SIZE,HEAD,TAIL>::ayLatterTail(void)
{
  ASSERT(m_nUsedHead < C_1MB);

  byte* ayNext = &ayArea()[SIZE - nUsedTail()];
  
  return ayNext;
}

/*----------------------------------------------------------------------------*/
/* get length of byte area                                                    */
/*----------------------------------------------------------------------------*/
template<i8 SIZE, class HEAD, class TAIL>
i8 WspConvergent_t<SIZE,HEAD,TAIL>::nLength(void)
{
  ASSERT(m_nUsedHead < C_1MB);

  // I THINK THIS SHOULD BE nSize()
  
  return SIZE;
}

/*----------------------------------------------------------------------------*/
/* reset this wsp                                                             */
/*----------------------------------------------------------------------------*/
template<i8 SIZE, class HEAD, class TAIL>
void WspConvergent_t<SIZE,HEAD,TAIL>::vReset(void)
{
  ASSERT(m_nUsedHead < C_1MB);

  // I DONT THINK THIS FUNCTION IS NEEDED
  
  vUsedHead(0);
  vUsedTail(0);
  
  m_nTailItems = 0;
}

/*----------------------------------------------------------------------------*/
/* get total size of this Wsp_t                                               */
/*----------------------------------------------------------------------------*/
template<i8 SIZE, class HEAD, class TAIL>
i8 WspConvergent_t<SIZE,HEAD,TAIL>::nSize(void)
{
  ASSERT(m_nUsedHead < C_1MB);

  return sizeof(WspConvergent_t<SIZE,HEAD,TAIL>);
}

/*----------------------------------------------------------------------------*/
/* get byte area as Stz*                                                      */
/*----------------------------------------------------------------------------*/
template<i8 SIZE, class HEAD, class TAIL>
Stz* WspConvergent_t<SIZE,HEAD,TAIL>::szStzArea(void)
{
  ASSERT(m_nUsedHead < C_1MB);

  char* aArea = (char*)m_yaByteArea;
  
 return reinterpret_cast<Stz*>(aArea);
}

/*----------------------------------------------------------------------------*/
/* begin allot of stz (of unknown size) in head                               */
/*----------------------------------------------------------------------------*/
template<i8 SIZE, class HEAD, class TAIL>
Stz* WspConvergent_t<SIZE,HEAD,TAIL>::szStzAllot_begin(void)
{
  ASSERT(m_nUsedHead < C_1MB);

  Stz* szStz = (Stz*)ayLatterHead();
  
  return szStz;
}

/*----------------------------------------------------------------------------*/
/* end allot of stz in head (size now known)                                  */
/*----------------------------------------------------------------------------*/
template<i8 SIZE, class HEAD, class TAIL>
void WspConvergent_t<SIZE,HEAD,TAIL>::vStzAllot_end(i8 nSize)
{
  ASSERT(m_nUsedHead < C_1MB);

  vTakeHead(nSize);
}

/*----------------------------------------------------------------------------*/
/* deposit given stz                                                          */
/*----------------------------------------------------------------------------*/
template<i8 SIZE, class HEAD, class TAIL>
Stz* WspConvergent_t<SIZE,HEAD,TAIL>::szStzDeposit(Stz* szValue)
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
template<i8 SIZE, class HEAD, class TAIL>
TAIL* WspConvergent_t<SIZE,HEAD,TAIL>::aTailItem(i8 i_Tail)
{
  ASSERT(m_nUsedHead < C_1MB);

  if (! m_nTailItems)
    return nullptr;
  
  ASSERT(C_UNKNOWN != m_nTailSize);
  
  i8 i_Area_right = (i_Tail + 1) * m_nTailSize;
  byte* ayItem = &ayArea()[SIZE - i_Area_right];
  
  return (TAIL*)ayItem;
}

/*----------------------------------------------------------------------------*/
/* take chunk from byte area from the head                                    */
/*----------------------------------------------------------------------------*/
template<i8 SIZE, class HEAD, class TAIL>
void WspConvergent_t<SIZE,HEAD,TAIL>::vTakeHead(i8 nBytes)
{
  // PRIVATE?
  ASSERT(m_nUsedHead < C_1MB);


  m_nUsedHead += nBytes;
  
  if (m_nUsedHead  + m_nUsedTail > SIZE) {
    vDebugPoint();
  }
  ASSERT(m_nUsedHead + m_nUsedTail <= SIZE);
}

/*----------------------------------------------------------------------------*/
/* take chunk from byte area from the tail                                    */
/*----------------------------------------------------------------------------*/
template<i8 SIZE, class HEAD, class TAIL>
void WspConvergent_t<SIZE,HEAD,TAIL>::vTakeTail(i8 nBytes)
{
  ASSERT(m_nUsedHead < C_1MB);

  // PRIVATE?

  m_nUsedTail += nBytes;
  
  if (m_nUsedHead  + m_nUsedTail > SIZE) {
    vDebugPoint();
  }
  ASSERT(m_nUsedHead + m_nUsedTail <= SIZE);
}

/*----------------------------------------------------------------------------*/
/* get offset of an allotted                                                  */
/*----------------------------------------------------------------------------*/
template<i8 SIZE, class HEAD, class TAIL>
i8 WspConvergent_t<SIZE,HEAD,TAIL>::iOffsetInWsp(byte* ayAllotted)
{
  ASSERT(m_nUsedHead < C_1MB);

  // I THINK THIS SHOULD BE i_Offset()

  i8 i_Ofs = nDifference(ayAllotted,m_yaByteArea);
  
  return i_Ofs;
}

/*----------------------------------------------------------------------------*/
/* get number of used bytes                                                   */
/*----------------------------------------------------------------------------*/
template<i8 SIZE, class HEAD, class TAIL>
i8 WspConvergent_t<SIZE,HEAD,TAIL>::nUsed(void)
{
  ASSERT(m_nUsedHead < C_1MB);

  return m_nUsedHead + m_nUsedTail;
}

/*----------------------------------------------------------------------------*/
/* get number of used bytes                                                   */
/*----------------------------------------------------------------------------*/
template<i8 SIZE, class HEAD, class TAIL>
i8 WspConvergent_t<SIZE,HEAD,TAIL>::nUsedHead(void)
{
  ASSERT(m_nUsedHead < C_1MB);

  return m_nUsedHead;
}

/*----------------------------------------------------------------------------*/
/* get number of used bytes                                                   */
/*----------------------------------------------------------------------------*/
template<i8 SIZE, class HEAD, class TAIL>
i8 WspConvergent_t<SIZE,HEAD,TAIL>::nUsedTail(void)
{
  ASSERT(m_nUsedHead < C_1MB);

  return m_nUsedTail;
}

/*----------------------------------------------------------------------------*/
/* set number of used bytes at head                                           */
/*----------------------------------------------------------------------------*/
template<i8 SIZE, class HEAD, class TAIL>
void WspConvergent_t<SIZE,HEAD,TAIL>::vUsedHead(i8 nBytes)
{
  ASSERT(m_nUsedHead < C_1MB);

  m_nUsedHead = nBytes;
  
  ASSERT(m_nUsedHead + m_nUsedTail <= SIZE);
}

/*----------------------------------------------------------------------------*/
/* set number of used bytes at tail                                           */
/*----------------------------------------------------------------------------*/
template<i8 SIZE, class HEAD, class TAIL>
void WspConvergent_t<SIZE,HEAD,TAIL>::vUsedTail(i8 nBytes)
{
  ASSERT(m_nUsedHead < C_1MB);

  m_nUsedTail = nBytes;
  
  ASSERT(m_nUsedHead + m_nUsedTail <= SIZE);
}

/*----------------------------------------------------------------------------*/
/* zero fill the byte                                                         */
/*----------------------------------------------------------------------------*/
template<i8 SIZE, class HEAD, class TAIL>
void WspConvergent_t<SIZE,HEAD,TAIL>::vZero(eCN eClassNo)
{
  ASSERT(m_nUsedHead < C_1MB);

  m_nUsedHead  = 0;
  
  m_nTailItems = 0;
  m_nUsedTail  = 0;
  m_nTailSize  = C_UNKNOWN;

  byte* ayDst = ayArea();
  
  MemLib::vZero(ayDst,SIZE,eClassNo);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
