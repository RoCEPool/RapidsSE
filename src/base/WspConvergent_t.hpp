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
    i8    m_nUsedAux;     // bytes used in the area for aux objects
    i8    m_nUsedHead;    // bytes used in the area for head objects
    i8    m_nUsedTail;    // bytes used in the area for tail items

    i8    m_i_AuxItem0;   // auxillary item 0
    i8    m_i_AuxItem1;   // auxillary item 1
  
    i8    m_nTailItems;   // number of fixed-size tail items in this wsp
    i8    m_nTailSize;    // size of each fixed-size tail item

  public:
    WspConvergent_t(void);

    TAIL* aAdjacentTail(TAIL* aPriorItem, i8 nItemBytes);
    byte* ayAllotAux0(i8 nBytes);
    byte* ayAllotAux1(i8 nBytes);
    HEAD* aAllotHead(i8 nBytes);
    TAIL* aAllotTail(i8 nBytes);
    byte* ayAux0(void);
    byte* ayAux1(void);
    void  vDecItems(void);
    void  vExtendAux0(i8 nBytes);
    byte* ayExtendAux0(i8 nBytes, i8& i_Aux);
    void  vExtendAux1(i8 nBytes);
    byte* ayExtendAux1(i8 nBytes, i8& i_Aux);
    TAIL* aFirstItem(void);
    i8    nFreeBytes(void);
    void  vGiveHead(i8 nBytes);
    void  vGiveTail(i8 nBytes);
    bool  bHasSpaceFor(i8 nBytes);
    void  vIncItems(void);
    void  vWspInit(void);
    TAIL* aItemAtOffset(i8 i_Offset);
    i8    nItems(void);
    HEAD* aLastItem(void); // ayLasHead()
    byte* ayLatterAux(void);
    byte* ayLatterHead(void);
    byte* ayLatterTail(void);
    i8    iOffsetOfItem(TAIL* ayAllotted);
    void  vResetHeads(void);
    Stz*  szStzAllot_begin(void);
    void  vStzAllot_end(i8 nSize);
    Stz*  szStzDeposit(Stz* szValue);
    TAIL* aTailItem(i8 i_Tail);
    void  vTakeAux(i8 nBytes);
    void  vTakeHead(i8 nBytes);
    void  vTakeTail(i8 nBytes);
    i8    nUsedBytes(void);
    i8    nUsedHead(void);
    i8    nUsedTail(void);
    void  vUsedHead(i8 nBytes);
    void  vUsedTail(i8 nBytes);
  
  private:
    bool  bStructurallyValid(void);
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
  
  ASSERT(2397407401047116065 == m_nUsedAux);
  m_nUsedAux   = 0;
  m_nUsedHead  = 0;
  m_nUsedTail  = 0;

  m_i_AuxItem0 = C_UNKNOWN;
  m_i_AuxItem1 = C_UNKNOWN;
  
  m_nTailItems = 0;
  m_nTailSize  = C_UNKNOWN;
  
  // !! !! !! !! !! !! !! !! !!
  // used bytes were changed
  ASSERT(bStructurallyValid());
}

/*----------------------------------------------------------------------------*/
/* get item which is memory-wise the previous item in the tail area           */
/*----------------------------------------------------------------------------*/
template<i8 AREA, class HEAD, class TAIL>
TAIL* WspConvergent_t<AREA,HEAD,TAIL>::aAdjacentTail(TAIL* aPriorItem,
                                                     i8 nSizeItem)
{
  ASSERT(bStructurallyValid());
  
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
/* allot variable-size auxillary item 0 from byte area                        */
/*----------------------------------------------------------------------------*/
template<i8 AREA, class HEAD, class TAIL>
byte* WspConvergent_t<AREA,HEAD,TAIL>::ayAllotAux0(i8 nBytes)
{
  ASSERT(bStructurallyValid());
  ASSERT(! m_nUsedHead);
  ASSERT(C_UNKNOWN == m_i_AuxItem0);
  ASSERT(C_UNKNOWN == m_i_AuxItem1);
  
  m_i_AuxItem0 = m_nUsedAux;
  
  vTakeAux(nBytes);
    
  return & m_yaByteArea[m_i_AuxItem0];
}

/*----------------------------------------------------------------------------*/
/* allot variable-size auxillary item 1 from byte area                        */
/*----------------------------------------------------------------------------*/
template<i8 AREA, class HEAD, class TAIL>
byte* WspConvergent_t<AREA,HEAD,TAIL>::ayAllotAux1(i8 nBytes)
{
  ASSERT(bStructurallyValid());
  ASSERT(! m_nUsedHead);
  ASSERT(C_UNKNOWN != m_i_AuxItem0);
  ASSERT(C_UNKNOWN == m_i_AuxItem1);

  m_i_AuxItem1 =  m_nUsedAux;
  
  vTakeAux(nBytes);

  return & m_yaByteArea[m_i_AuxItem1];
}

/*----------------------------------------------------------------------------*/
/* allot a variable-size head from byte area                                  */
/*----------------------------------------------------------------------------*/
template<i8 AREA, class HEAD, class TAIL>
HEAD* WspConvergent_t<AREA,HEAD,TAIL>::aAllotHead(i8 nBytes)
{
  ASSERT(bStructurallyValid());
  
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
    
  // !! !! !! !! !! !! !! !! !!
  // used bytes were changed
  ASSERT(bStructurallyValid());

  return (HEAD*)ayAllottedHead;
}

/*----------------------------------------------------------------------------*/
/* allot a fixed size tail from byte area                                     */
/*----------------------------------------------------------------------------*/
template<i8 AREA, class HEAD, class TAIL>
TAIL* WspConvergent_t<AREA,HEAD,TAIL>::aAllotTail(i8 nBytes)
{
  ASSERT(bStructurallyValid());
  
  ASSERT(nBytes >= 16);

  if (C_UNKNOWN == m_nTailSize) {
    m_nTailSize = nBytes;
  }
  //else {
  //  ASSERT(nBytes == m_nTailSize);
  //}

  vTakeTail(nBytes);
  
  byte* ayAllottedTail = ayLatterTail();
  
  ++ m_nTailItems;

  return (TAIL*)ayAllottedTail;
}

/*----------------------------------------------------------------------------*/
/* get auxilliary 0                                                           */
/*----------------------------------------------------------------------------*/
template<i8 AREA, class HEAD, class TAIL>
byte* WspConvergent_t<AREA,HEAD,TAIL>::ayAux0(void)
{
  ASSERT(bStructurallyValid());

  if (C_UNKNOWN == m_i_AuxItem0)
    return nullptr;
  
  byte* ayAux0 = & m_yaByteArea[m_i_AuxItem0];

  return ayAux0;
}

/*----------------------------------------------------------------------------*/
/* get auxilliary 1                                                           */
/*----------------------------------------------------------------------------*/
template<i8 AREA, class HEAD, class TAIL>
byte* WspConvergent_t<AREA,HEAD,TAIL>::ayAux1(void)
{
  ASSERT(bStructurallyValid());

  if (C_UNKNOWN == m_i_AuxItem1)
    return nullptr;

  byte* ayAux1 = & m_yaByteArea[m_i_AuxItem1];

  return ayAux1;
}

//*----------------------------------------------------------------------------*/
/* dec number of items                                                        */
/*----------------------------------------------------------------------------*/
template<i8 AREA, class HEAD, class TAIL>
void WspConvergent_t<AREA,HEAD,TAIL>::vDecItems(void)
{
  ASSERT(bStructurallyValid());
  
  --m_nTailItems;
}

/*----------------------------------------------------------------------------*/
/* extend aux 0                                                               */
/*----------------------------------------------------------------------------*/
template<i8 AREA, class HEAD, class TAIL>
byte* WspConvergent_t<AREA,HEAD,TAIL>::ayExtendAux0(i8 nBytes, i8& i_Aux)
{
  ASSERT(bStructurallyValid());
  ASSERT(! m_nUsedHead);
  ASSERT(C_UNKNOWN != m_i_AuxItem0);
  ASSERT(C_UNKNOWN == m_i_AuxItem1);
    
  i_Aux = m_nUsedAux;
  byte* ayExtension = & m_yaByteArea[m_nUsedAux];
  
  vTakeAux(nBytes);
  
  return ayExtension;
}

/*----------------------------------------------------------------------------*/
/* extend aux 0                                                               */
/*----------------------------------------------------------------------------*/
template<i8 AREA, class HEAD, class TAIL>
void WspConvergent_t<AREA,HEAD,TAIL>::vExtendAux0(i8 nBytes)
{
  ASSERT(bStructurallyValid());
  ASSERT(! m_nUsedHead);
  ASSERT(C_UNKNOWN != m_i_AuxItem0);
  ASSERT(C_UNKNOWN == m_i_AuxItem1);
    
  vTakeAux(nBytes);
}

/*----------------------------------------------------------------------------*/
/* extend aux 1                                                               */
/*----------------------------------------------------------------------------*/
template<i8 AREA, class HEAD, class TAIL>
byte* WspConvergent_t<AREA,HEAD,TAIL>::ayExtendAux1(i8 nBytes, i8& i_Aux)
{
  ASSERT(bStructurallyValid());
  ASSERT(! m_nUsedHead);
  ASSERT(C_UNKNOWN != m_i_AuxItem0);
  ASSERT(C_UNKNOWN != m_i_AuxItem1);
    
  i_Aux = m_nUsedAux;
  byte* ayExtension = & m_yaByteArea[m_nUsedAux];

  vTakeAux(nBytes);
  
  return ayExtension;
}

/*----------------------------------------------------------------------------*/
/* extend aux 1                                                               */
/*----------------------------------------------------------------------------*/
template<i8 AREA, class HEAD, class TAIL>
void WspConvergent_t<AREA,HEAD,TAIL>::vExtendAux1(i8 nBytes)
{
  ASSERT(bStructurallyValid());
  ASSERT(! m_nUsedHead);
  ASSERT(C_UNKNOWN != m_i_AuxItem0);
  ASSERT(C_UNKNOWN != m_i_AuxItem1);
    
  vTakeAux(nBytes);
}

/*----------------------------------------------------------------------------*/
/* get address of byte area                                                   */
/*----------------------------------------------------------------------------*/
template<i8 AREA, class HEAD, class TAIL>
TAIL* WspConvergent_t<AREA,HEAD,TAIL>::aFirstItem(void)
{
  ASSERT(bStructurallyValid());

  if (! m_nTailItems)
    return nullptr;
  
  TAIL* aTail = aTailItem(0);
  
  return aTail;
}

/*----------------------------------------------------------------------------*/
/* get number of free bytes in byte area                                      */
/*----------------------------------------------------------------------------*/
template<i8 AREA, class HEAD, class TAIL>
i8 WspConvergent_t<AREA,HEAD,TAIL>::nFreeBytes(void)
{
  ASSERT(bStructurallyValid());

  i8 nFree = AREA - m_nUsedAux - m_nUsedHead - m_nUsedTail;
  ASSERT(nFree >= 0);
  
  return nFree;
}

/*----------------------------------------------------------------------------*/
/* give chunk back to byte area at head                                       */
/*----------------------------------------------------------------------------*/
template<i8 AREA, class HEAD, class TAIL>
void WspConvergent_t<AREA,HEAD,TAIL>::vGiveHead(i8 nBytes)
{
  ASSERT(bStructurallyValid());

  m_nUsedHead -= nBytes;
  
  // !! !! !! !! !! !! !! !! !!
  // used bytes were changed
  ASSERT(bStructurallyValid());
}

/*----------------------------------------------------------------------------*/
/* give chunk back to byte area at tail                                       */
/*----------------------------------------------------------------------------*/
template<i8 AREA, class HEAD, class TAIL>
void WspConvergent_t<AREA,HEAD,TAIL>::vGiveTail(i8 nBytes)
{
  ASSERT(bStructurallyValid());
  
  ASSERT(m_nUsedHead  + m_nUsedTail <= AREA);
  
  m_nUsedTail -= nBytes;
  
  // !! !! !! !! !! !! !! !! !!
  // used bytes were changed
  ASSERT(bStructurallyValid());
}

/*----------------------------------------------------------------------------*/
/* does this workspace have enough free space for given size                  */
/*----------------------------------------------------------------------------*/
template<i8 AREA, class HEAD, class TAIL>
bool WspConvergent_t<AREA,HEAD,TAIL>::bHasSpaceFor(i8 nSizeBytes)
{
  ASSERT(bStructurallyValid());

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
  ASSERT(bStructurallyValid());

  ++m_nTailItems;
}

/*----------------------------------------------------------------------------*/
/* init                                                                       */
/*----------------------------------------------------------------------------*/
template<i8 AREA, class HEAD, class TAIL>
void WspConvergent_t<AREA,HEAD,TAIL>::vWspInit(void)
{
//#ifdef WITH_CHECK_INITIALISED
//  MemLib::vZero(m_yaByteArea,AREA,CN_NONE);
//#endif
  
  //m_nUsedAux   = 0;
  m_nUsedHead  = 0;
  m_nUsedTail  = 0;

  //m_i_AuxItem0 = C_UNKNOWN;
  //m_i_AuxItem1 = C_UNKNOWN;
  
  m_nTailItems = 0;
  m_nTailSize  = C_UNKNOWN;
  
  // !! !! !! !! !! !! !! !! !!
  // used bytes were changed
  ASSERT(bStructurallyValid());
}

/*----------------------------------------------------------------------------*/
/* get tail item at given offset                                              */
/*----------------------------------------------------------------------------*/
template<i8 AREA, class HEAD, class TAIL>
TAIL* WspConvergent_t<AREA,HEAD,TAIL>::aItemAtOffset(i8 i_Offset)
{
  ASSERT(bStructurallyValid());

  TAIL* aTail = (TAIL*) & m_yaByteArea[i_Offset];

  return aTail;
}

/*----------------------------------------------------------------------------*/
/* get number of items                                                        */
/*----------------------------------------------------------------------------*/
template<i8 AREA, class HEAD, class TAIL>
i8 WspConvergent_t<AREA,HEAD,TAIL>::nItems(void)
{
  ASSERT(bStructurallyValid());

  return m_nTailItems;
}

/*----------------------------------------------------------------------------*/
/* get address of last used byte in aux area                                  */
/*----------------------------------------------------------------------------*/
template<i8 AREA, class HEAD, class TAIL>
byte* WspConvergent_t<AREA,HEAD,TAIL>::ayLatterAux(void)
{
  ASSERT(bStructurallyValid());

  byte* ayLastByte = &m_yaByteArea[m_nUsedAux - 1];
  
  return ayLastByte;
}

/*----------------------------------------------------------------------------*/
/* get address of last used byte in head area                                 */
/*----------------------------------------------------------------------------*/
template<i8 AREA, class HEAD, class TAIL>
byte* WspConvergent_t<AREA,HEAD,TAIL>::ayLatterHead(void)
{
  ASSERT(bStructurallyValid());

  byte* ayLastByte = &m_yaByteArea[m_nUsedAux + m_nUsedHead - 1];
  
  return ayLastByte;
}

/*----------------------------------------------------------------------------*/
/* get address of last used byte in tail area                                 */
/*----------------------------------------------------------------------------*/
template<i8 AREA, class HEAD, class TAIL>
byte* WspConvergent_t<AREA,HEAD,TAIL>::ayLatterTail(void)
{
  ASSERT(bStructurallyValid());

  byte* ayLastByte = &m_yaByteArea[AREA - m_nUsedTail];
  
  return ayLastByte;
}

/*----------------------------------------------------------------------------*/
/* get offset of an allotted                                                  */
/*----------------------------------------------------------------------------*/
template<i8 AREA, class HEAD, class TAIL>
i8 WspConvergent_t<AREA,HEAD,TAIL>::iOffsetOfItem(TAIL* ayAllotted)
{
  ASSERT(bStructurallyValid());

  i8 i_Ofs = nDifference(ayAllotted,m_yaByteArea);
  
  return i_Ofs;
}

/*----------------------------------------------------------------------------*/
/* reset                                                                      */
/*----------------------------------------------------------------------------*/
template<i8 AREA, class HEAD, class TAIL>
void WspConvergent_t<AREA,HEAD,TAIL>::vResetHeads(void)
{
  ASSERT (m_nTailItems <= 1);
  
  m_nUsedHead  = 0;
  
  // !! !! !! !! !! !! !! !! !!
  // used bytes were changed
  ASSERT(bStructurallyValid());
}

/*----------------------------------------------------------------------------*/
/* begin allot of stz (of unknown size) in head                               */
/*----------------------------------------------------------------------------*/
template<i8 AREA, class HEAD, class TAIL>
Stz* WspConvergent_t<AREA,HEAD,TAIL>::szStzAllot_begin(void)
{
  ASSERT(bStructurallyValid());

  Stz* szStz = (Stz*)ayLatterHead() + 1;
  
  return szStz;
}

/*----------------------------------------------------------------------------*/
/* end allot of stz in head (size now known)                                  */
/*----------------------------------------------------------------------------*/
template<i8 AREA, class HEAD, class TAIL>
void WspConvergent_t<AREA,HEAD,TAIL>::vStzAllot_end(i8 nSize)
{
  ASSERT(bStructurallyValid());

  vTakeHead(nSize);
}

/*----------------------------------------------------------------------------*/
/* deposit given stz                                                          */
/*----------------------------------------------------------------------------*/
template<i8 AREA, class HEAD, class TAIL>
Stz* WspConvergent_t<AREA,HEAD,TAIL>::szStzDeposit(Stz* szValue)
{
  ASSERT(bStructurallyValid());

  i8 nSize = szValue->nSize();
  
  Stz* szAllottedStz = szStzAllot_begin();
  szAllottedStz->vCopy(szValue);
  vStzAllot_end(nSize);
  
  // !! !! !! !! !! !! !! !! !!
  // string copy
  ASSERT(bStructurallyValid());
  
  return szAllottedStz;
}

/*----------------------------------------------------------------------------*/
/* get address of given tail item                                             */
/*----------------------------------------------------------------------------*/
template<i8 AREA, class HEAD, class TAIL>
TAIL* WspConvergent_t<AREA,HEAD,TAIL>::aTailItem(i8 i_Tail)
{
  ASSERT(bStructurallyValid());

  if (! m_nTailItems)
    return nullptr;
  
  ASSERT(C_UNKNOWN != m_nTailSize);
  
  i8 i_Tail_area = (i_Tail + 1) * m_nTailSize;
  byte* ayItem = &m_yaByteArea[AREA - i_Tail_area];
  
  return (TAIL*)ayItem;
}

/*----------------------------------------------------------------------------*/
/* take chunk from byte area from the head                                    */
/*----------------------------------------------------------------------------*/
template<i8 AREA, class HEAD, class TAIL>
void WspConvergent_t<AREA,HEAD,TAIL>::vTakeAux(i8 nBytes)
{
  // PRIVATE?
  ASSERT(bStructurallyValid());

  m_nUsedAux += nBytes;
    
  // !! !! !! !! !! !! !! !! !!
  // used bytes were changed
  ASSERT(bStructurallyValid());
}

/*----------------------------------------------------------------------------*/
/* take chunk from byte area from the head                                    */
/*----------------------------------------------------------------------------*/
template<i8 AREA, class HEAD, class TAIL>
void WspConvergent_t<AREA,HEAD,TAIL>::vTakeHead(i8 nBytes)
{
  // PRIVATE?
  ASSERT(bStructurallyValid());

  m_nUsedHead += nBytes;
    
  // !! !! !! !! !! !! !! !! !!
  // used bytes were changed
  ASSERT(bStructurallyValid());
}

/*----------------------------------------------------------------------------*/
/* take chunk from byte area from the tail                                    */
/*----------------------------------------------------------------------------*/
template<i8 AREA, class HEAD, class TAIL>
void WspConvergent_t<AREA,HEAD,TAIL>::vTakeTail(i8 nBytes)
{
  ASSERT(bStructurallyValid());

  m_nUsedTail += nBytes;
  
  // !! !! !! !! !! !! !! !! !!
  // used bytes were changed
  ASSERT(bStructurallyValid());
}

/*----------------------------------------------------------------------------*/
/* get number of used bytes                                                   */
/*----------------------------------------------------------------------------*/
template<i8 AREA, class HEAD, class TAIL>
i8 WspConvergent_t<AREA,HEAD,TAIL>::nUsedBytes(void)
{
  return m_nUsedAux + m_nUsedHead + m_nUsedTail;
}

/*----------------------------------------------------------------------------*/
/* get number of used bytes                                                   */
/*----------------------------------------------------------------------------*/
template<i8 AREA, class HEAD, class TAIL>
i8 WspConvergent_t<AREA,HEAD,TAIL>::nUsedHead(void)
{
  ASSERT(bStructurallyValid());

  return m_nUsedHead;
}

/*----------------------------------------------------------------------------*/
/* get number of used bytes                                                   */
/*----------------------------------------------------------------------------*/
template<i8 AREA, class HEAD, class TAIL>
i8 WspConvergent_t<AREA,HEAD,TAIL>::nUsedTail(void)
{
  ASSERT(bStructurallyValid());

  return m_nUsedTail;
}

/*----------------------------------------------------------------------------*/
/* set number of used bytes at head                                           */
/*----------------------------------------------------------------------------*/
template<i8 AREA, class HEAD, class TAIL>
void WspConvergent_t<AREA,HEAD,TAIL>::vUsedHead(i8 nBytes)
{
  ASSERT(bStructurallyValid());

  m_nUsedHead = nBytes;
  
  // !! !! !! !! !! !! !! !! !!
  // used bytes were changed
  ASSERT(bStructurallyValid());
}

/*----------------------------------------------------------------------------*/
/* set number of used bytes at tail                                           */
/*----------------------------------------------------------------------------*/
template<i8 AREA, class HEAD, class TAIL>
void WspConvergent_t<AREA,HEAD,TAIL>::vUsedTail(i8 nBytes)
{
  ASSERT(bStructurallyValid());

  m_nUsedTail = nBytes;
  
  // !! !! !! !! !! !! !! !! !!
  // used bytes were changed
  ASSERT(bStructurallyValid());
}

/*----------------------------------------------------------------------------*/
/*-- private -----------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* set number of used bytes at tail                                           */
/*----------------------------------------------------------------------------*/
template<i8 AREA, class HEAD, class TAIL>
bool WspConvergent_t<AREA,HEAD,TAIL>::bStructurallyValid(void)
{
  i8 nUsed = nUsedBytes();
  if (nUsed > AREA)
    return false;
  
  return true;
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
