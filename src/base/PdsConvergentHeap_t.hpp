/*============================================================================*/
/* PdsConvergentHeap_t.hpp -- fixed-sized workspace that converges from both ends */
/*============================================================================*/
#pragma once

namespace BASE
{
  template<i8 AREA, class HEAD, class TAIL>
  class PdsConvergentHeap_t
  {
  public:
    i8    m_nUsedHead;    // bytes used in the area for head objects
    i8    m_nUsedTail;    // bytes used in the area for tail items

    i8    m_nHeadItems;   // number of fixed-size head items in this wsp
    i8    m_nHeadSize;    // size of each fixed-size head item

    i8    m_nTailItems;   // number of fixed-size tail items in this wsp
    i8    m_nTailSize;    // size of each fixed-size tail item

//    PadWord m__PadToCacheLine[1];

    byte  m_yaByteArea[AREA]; // allottable area

  public:
    PdsConvergentHeap_t(void);

    HEAD* aAllotHead(void);
    TAIL* aAllotTail(void);
    i8    nFreeBytes(void);
    void  vGiveHead(i8 nBytes);
    void  vGiveTail(i8 nBytes);
    bool  bHasSpaceFor(i8 nBytes);
    void  vInit(i8 nHeadSize);
    i8    nItems(void);
    void* aLastUsableByteAddr(void);
    byte* ayLatterHead(void);
    byte* ayLatterTail(void);
    i8    iTailSize(void);
    void  vTailSize(i8 nSize);
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
PdsConvergentHeap_t<AREA,HEAD,TAIL>::PdsConvergentHeap_t(void)
{
#ifdef WITH_CHECK_INITIALISED
  MemLib::vZero(m_yaByteArea,AREA,CN_NONE);
#endif

  m_nUsedHead  = 0;
  m_nUsedTail  = 0;

  m_nHeadItems = 0;
  m_nHeadSize  = C_UNKNOWN;

  m_nTailItems = 0;
  m_nTailSize  = C_UNKNOWN;

  MemLib::vZero(m_yaByteArea,AREA,CN_NONE);

  // !! !! !! !! !! !! !! !! !!
  // used bytes were changed
  ASSERT(bStructurallyValid());
}

/*----------------------------------------------------------------------------*/
/* allot a fixed-size head from byte area                                     */
/*----------------------------------------------------------------------------*/
template<i8 AREA, class HEAD, class TAIL>
HEAD* PdsConvergentHeap_t<AREA,HEAD,TAIL>::aAllotHead(void)
{
  ASSERT(bStructurallyValid());

  RASSERT(C_UNKNOWN != m_nHeadSize);

  if(! bHasSpaceFor(m_nHeadSize)) {
    return nullptr;
  }

  /*
          Fixed Heads (6 heads)                 Variable Tails (9 tails)
     +-------------------------------+          +-----------------------------+
     |    :    :    :    :    :    :|t|/////////|h| : :   :    :  : :    :  : |
     +-------------------------------+          +-----------------------------+
                          latterTail^            ^latterHead
        (last used byte in head area)            (first used byte in tail area)

                        allot head -->          <-- allot tails
   */

  byte* ayAllottedHead = ayLatterHead();

  RASSERT(DoubleWord::bOnDoubleWordBoundary(ayAllottedHead));
  vTakeHead(m_nHeadSize);

  m_nHeadItems++;

  // !! !! !! !! !! !! !! !! !!
  // used bytes were changed
  ASSERT(bStructurallyValid());

  return (HEAD*)ayAllottedHead;
}

/*----------------------------------------------------------------------------*/
/* allot a fixed size tail from byte area                                     */
/*----------------------------------------------------------------------------*/
template<i8 AREA, class HEAD, class TAIL>
TAIL* PdsConvergentHeap_t<AREA,HEAD,TAIL>::aAllotTail(void)
{
  ASSERT(bStructurallyValid());

  RASSERT(m_nTailSize != C_UNKNOWN);

  if(! bHasSpaceFor(m_nTailSize)) {
    return nullptr;
  }

  vTakeTail(m_nTailSize);

  byte* ayAllottedTail = ayLatterTail();

  ++ m_nTailItems;

  return (TAIL*)ayAllottedTail;
}

/*----------------------------------------------------------------------------*/
/* get number of free bytes in byte area                                      */
/*----------------------------------------------------------------------------*/
template<i8 AREA, class HEAD, class TAIL>
i8 PdsConvergentHeap_t<AREA,HEAD,TAIL>::nFreeBytes(void)
{
  ASSERT(bStructurallyValid());

  i8 nFree = AREA - m_nUsedHead - m_nUsedTail;
  ASSERT(nFree >= 0);

  return nFree;
}

/*----------------------------------------------------------------------------*/
/* give chunk back to byte area at head                                       */
/*----------------------------------------------------------------------------*/
template<i8 AREA, class HEAD, class TAIL>
void PdsConvergentHeap_t<AREA,HEAD,TAIL>::vGiveHead(i8 nBytes)
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
void PdsConvergentHeap_t<AREA,HEAD,TAIL>::vGiveTail(i8 nBytes)
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
bool PdsConvergentHeap_t<AREA,HEAD,TAIL>::bHasSpaceFor(i8 nSizeBytes)
{
  ASSERT(bStructurallyValid());

  if (nFreeBytes() >= nSizeBytes)
    return true;
  else {
//    RASSERT(0);
    return false;
  }
}

/*----------------------------------------------------------------------------*/
/* init                                                                       */
/*----------------------------------------------------------------------------*/
template<i8 AREA, class HEAD, class TAIL>
void PdsConvergentHeap_t<AREA,HEAD,TAIL>::vInit(i8 nHeadSize)
{
  m_nUsedHead  = 0;
  m_nUsedTail  = 0;

  m_nHeadItems = 0;
  m_nHeadSize  = nHeadSize;

  m_nTailItems = 0;
  m_nTailSize  = C_UNKNOWN;

  MemLib::vZero(m_yaByteArea,AREA,CN_NONE);

  // !! !! !! !! !! !! !! !! !!
  // used bytes were changed
  ASSERT(bStructurallyValid());
}

/*----------------------------------------------------------------------------*/
/* get number of items                                                        */
/*----------------------------------------------------------------------------*/
template<i8 AREA, class HEAD, class TAIL>
i8 PdsConvergentHeap_t<AREA,HEAD,TAIL>::nItems(void)
{
  ASSERT(bStructurallyValid());

  return m_nTailItems;
}

/*----------------------------------------------------------------------------*/
/* get the address of the last usable byte (used or free) in this Heap        */
/*----------------------------------------------------------------------------*/
template<i8 AREA, class HEAD, class TAIL>
void* PdsConvergentHeap_t<AREA,HEAD,TAIL>::aLastUsableByteAddr(void)
{
  ASSERT(bStructurallyValid());

  return &(m_yaByteArea[AREA - 1]);
}

/*----------------------------------------------------------------------------*/
/* get address of the next available byte in head area                        */
/*----------------------------------------------------------------------------*/
template<i8 AREA, class HEAD, class TAIL>
byte* PdsConvergentHeap_t<AREA,HEAD,TAIL>::ayLatterHead(void)
{
  ASSERT(bStructurallyValid());

  byte* ayBytes = &m_yaByteArea[m_nUsedHead];

  return ayBytes;
}

/*----------------------------------------------------------------------------*/
/* get address of last used byte in tail area                                 */
/*----------------------------------------------------------------------------*/
template<i8 AREA, class HEAD, class TAIL>
byte* PdsConvergentHeap_t<AREA,HEAD,TAIL>::ayLatterTail(void)
{
  ASSERT(bStructurallyValid());

  byte* ayLastByte = &m_yaByteArea[AREA - m_nUsedTail];

  return ayLastByte;
}
/*----------------------------------------------------------------------------*/
/* get address of given tail item                                             */
/*----------------------------------------------------------------------------*/
template<i8 AREA, class HEAD, class TAIL>
i8 PdsConvergentHeap_t<AREA,HEAD,TAIL>::iTailSize(void)
{
  ASSERT(bStructurallyValid());

  return m_nTailSize;
}

/*----------------------------------------------------------------------------*/
/* get address of given tail item                                             */
/*----------------------------------------------------------------------------*/
template<i8 AREA, class HEAD, class TAIL>
void PdsConvergentHeap_t<AREA,HEAD,TAIL>::vTailSize(i8 nSize)
{
  ASSERT(bStructurallyValid());

  m_nTailSize = nSize;
}

/*----------------------------------------------------------------------------*/
/* take chunk from byte area from the head                                    */
/*----------------------------------------------------------------------------*/
template<i8 AREA, class HEAD, class TAIL>
void PdsConvergentHeap_t<AREA,HEAD,TAIL>::vTakeHead(i8 nBytes)
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
void PdsConvergentHeap_t<AREA,HEAD,TAIL>::vTakeTail(i8 nBytes)
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
i8 PdsConvergentHeap_t<AREA,HEAD,TAIL>::nUsedBytes(void)
{
  return m_nUsedHead + m_nUsedTail;
}

/*----------------------------------------------------------------------------*/
/* get number of used bytes                                                   */
/*----------------------------------------------------------------------------*/
template<i8 AREA, class HEAD, class TAIL>
i8 PdsConvergentHeap_t<AREA,HEAD,TAIL>::nUsedHead(void)
{
  ASSERT(bStructurallyValid());

  return m_nUsedHead;
}

/*----------------------------------------------------------------------------*/
/* get number of used bytes                                                   */
/*----------------------------------------------------------------------------*/
template<i8 AREA, class HEAD, class TAIL>
i8 PdsConvergentHeap_t<AREA,HEAD,TAIL>::nUsedTail(void)
{
  ASSERT(bStructurallyValid());

  return m_nUsedTail;
}

/*----------------------------------------------------------------------------*/
/* set number of used bytes at head                                           */
/*----------------------------------------------------------------------------*/
template<i8 AREA, class HEAD, class TAIL>
void PdsConvergentHeap_t<AREA,HEAD,TAIL>::vUsedHead(i8 nBytes)
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
void PdsConvergentHeap_t<AREA,HEAD,TAIL>::vUsedTail(i8 nBytes)
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
bool PdsConvergentHeap_t<AREA,HEAD,TAIL>::bStructurallyValid(void)
{
  i8 nUsed = nUsedBytes();
  if (nUsed > AREA)
    return false;

  return true;
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/