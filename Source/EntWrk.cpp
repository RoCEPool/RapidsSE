/*============================================================================*/
/* EntWrk.cpp -- Working Index Entry                                          */
/*============================================================================*/

#include "Entries.h"
#include "AM.hpp"

/*----------------------------------------------------------------------------*/
/*-- LifeCycle ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* operator new                                                               */
/*----------------------------------------------------------------------------*/
void* EntWrk::operator new(size_t)
{
  vThrow(FK_FATAL,VL_FATAL,__func__,__FILE__,__LINE__,
         "fatal: EntWrk class cannot be new'd\n"_Stz);
  
  NEVER_REACH;
}

/*----------------------------------------------------------------------------*/
/* operator new                                                               */
/*----------------------------------------------------------------------------*/
void* EntWrk::operator new(size_t, EntWrk* pEntWrk)
{
  return pEntWrk;
}

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
EntWrk::EntWrk(void)
: Object(CN_ENTWRK),
  m_xEnt(CN_ENT,sizeof(Ent))
{
  _precondition_(_MAGIC_,CN_ENTWRK);

  m_pStreamDef = nullptr;
  m_pIdxColVec = nullptr;
  
  // -- entry --
  m_xEnt.vBuffer(0);
  m_xEnt.vItem(0);
  m_xEnt.vOffsets(0);
  
  m_xEnt.vElmSeqSize(0);
  // -- end entry --

#ifdef WITH_CHECK_INITIALISED
  MemLib::vZero(m_yaSpace,sizeof(m_yaSpace),CN_ENTWRK);
#else
  m_yaSpace[0] = '\0';
#endif
  
  _checkInitialisedSize_(sizeof(EntWrk));
  
  _postcondition_(_MAGIC_,CN_ENTWRK,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
EntWrk::~EntWrk(void) noexcept(false)
{
  _precondition_(_MAGIC_,CN_ENTWRK,_FOOTER_);
}


/*----------------------------------------------------------------------------*/
/*-- Static ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* get next element from key                                                  */
/*----------------------------------------------------------------------------*/
byte* EntWrk::ayEachElm(Column* pColumn, i8& nBytes, byte** aayCursor)
{
  byte* ayElm = pColumn->ayValue(nBytes,aayCursor);
  
  return ayElm;
}


/*----------------------------------------------------------------------------*/
/*-- Public ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* add element to key                                                         */
/*----------------------------------------------------------------------------*/
void EntWrk::vAppendElement(Row* pRow, Column* pCol)
{
  _precondition_(_MAGIC_,CN_ENTWRK,_FOOTER_);
  
  i8 nElmSize = 0;
  
  i8 iValue = pRow->m_waColVal[pCol->iColumnNo()].m_iWrd;
  
  if (Null::bIsNull(iValue)) {
    byte* ayDst = (byte*)ayElmNext();
    nElmSize = Null::nCopy(ayDst);
    vIncElmSeqSize(nElmSize);
    return;
  }
  
  if (DM_VCH == pCol->eDatum()) {
    char* aDst = (char*)ayElmNext();
    char* aSrc = (char*)iValue;
    nElmSize = StrLib::iWordFlip(aDst, aSrc);
    vIncElmSeqSize(nElmSize);
  }
  else {
//    byte* ayDst = ayElmNext();
//    nElmSize = Dtm::nCopy(ayDst,iValue);
//    vIncElmSeqSize(nElmSize);
    vAppendInt(iValue);
  }
}

/*----------------------------------------------------------------------------*/
/* add int element to key                                                     */
/*----------------------------------------------------------------------------*/
void EntWrk::vAppendInt(i8 iArg)
{
  _precondition_(_MAGIC_,CN_ENTWRK,_FOOTER_);
  
  byte* ayDst = ayElmNext();
  
  i8 nElmSize = Dtm::nCopy(ayDst,iArg);
  
  vIncElmSeqSize(nElmSize);
}

/*----------------------------------------------------------------------------*/
/* add min lower value to key                                                 */
/*----------------------------------------------------------------------------*/
void EntWrk::vAppendMinusInfinity(void)
{
  _precondition_(_MAGIC_,CN_ENTWRK,_FOOTER_);
  
  byte* ayDst = ayElmNext();
  Dtm::vCopy(ayDst,(i8)C_DATUM_MINUS_INFINITY);

  vIncElmSeqSize(C_BYTES_PER_WORD);
}

/*----------------------------------------------------------------------------*/
/* add max upper value to key                                                 */
/*----------------------------------------------------------------------------*/
void EntWrk::vAppendPlusInfinity(void)
{
  _precondition_(_MAGIC_,CN_ENTWRK,_FOOTER_);
   
  byte* ayDst = ayElmNext();
  Dtm::vCopy(ayDst,(i8)C_DATUM_PLUS_INFINITY);
  
  vIncElmSeqSize(C_BYTES_PER_WORD);
}

/*----------------------------------------------------------------------------*/
/* get buffer no from ref                                                     */
/*----------------------------------------------------------------------------*/
i8 EntWrk::iBuffer(void)
{
  _precondition_(_MAGIC_,CN_ENTWRK,_FOOTER_);
  
  i8 iBuffer = m_xEnt.iBuffer();
  
  return iBuffer;
}

/*----------------------------------------------------------------------------*/
/* set buffer no in re                                                        */
/*----------------------------------------------------------------------------*/
void EntWrk::vBuffer(i8 iBuffer)
{
  _precondition_(_MAGIC_,CN_ENTWRK,_FOOTER_);
  
  m_xEnt.vBuffer(iBuffer);
}

/*----------------------------------------------------------------------------*/
/* get address where next element would be added                              */
/*----------------------------------------------------------------------------*/
byte* EntWrk::ayElmNext(void)
{
  _precondition_(_MAGIC_,CN_ENTWRK,_FOOTER_);
  
  i8 nElmSeqSize = m_xEnt.nElmSeqSize();

  byte* ayNextElm = aAdd<byte*>(ayElmSeq(),nElmSeqSize);

  return ayNextElm;
}

/*----------------------------------------------------------------------------*/
/* set element sequence size                                                  */
/*----------------------------------------------------------------------------*/
void EntWrk::vElmSeqSize(i8 nElmSeqSize)
{
  _precondition_(_MAGIC_,CN_ENTWRK,_FOOTER_);
  
  m_xEnt.vElmSeqSize(nElmSeqSize);
}

/*----------------------------------------------------------------------------*/
/* get byte sequence of key                                                   */
/*----------------------------------------------------------------------------*/
byte* EntWrk::ayElmSeq(void)
{
  _precondition_(_MAGIC_,CN_ENTWRK,_FOOTER_);
  
  return m_xEnt.ayElmSeq();
}

/*----------------------------------------------------------------------------*/
/* get entry pointer                                                          */
/*----------------------------------------------------------------------------*/
Ent* EntWrk::aEnt(void)
{
  _precondition_(_MAGIC_,CN_ENTWRK,_FOOTER_);
  
  return &m_xEnt;
}

/*----------------------------------------------------------------------------*/
/* compute hash of key byte sequence                                          */
/*----------------------------------------------------------------------------*/
i8 EntWrk::iHashKey(void)
{
  _precondition_(_MAGIC_,CN_ENTWRK,_FOOTER_);
  
  i8 iHash = HshLib::iHash(ayElmSeq(),m_xEnt.nElmSeqSize());
  
  return iHash;
}

/*----------------------------------------------------------------------------*/
/* get index column vector                                                    */
/*----------------------------------------------------------------------------*/
IdxColVec* EntWrk::pIdxColVec(void)
{
  _precondition_(_MAGIC_,CN_ENTWRK,_FOOTER_);
  
  return m_pIdxColVec;
}

/*----------------------------------------------------------------------------*/
/* set index column vector                                                    */
/*----------------------------------------------------------------------------*/
void EntWrk::vIdxColVec(IdxColVec* pIdxColVec)
{
  _precondition_(_MAGIC_,CN_ENTWRK,_FOOTER_);
  
  m_pIdxColVec = pIdxColVec;
}

/*----------------------------------------------------------------------------*/
/* inc total element sequence size by the size of element                     */
/*----------------------------------------------------------------------------*/
void EntWrk::vIncElmSeqSize(i8 nElementSize)
{
  _precondition_(_MAGIC_,CN_ENTWRK,_FOOTER_);
  
  m_xEnt.aKey()->vIncElmSeqSize(nElementSize);
}

/*----------------------------------------------------------------------------*/
/* get item no from entry reference                                           */
/*----------------------------------------------------------------------------*/
i8 EntWrk::iItem(void)
{
  _precondition_(_MAGIC_,CN_ENTWRK,_FOOTER_);
  
  return m_xEnt.iItem();
}

/*----------------------------------------------------------------------------*/
/* set item no in entry reference                                             */
/*----------------------------------------------------------------------------*/
void EntWrk::vItem(i8 iItem)
{
  _precondition_(_MAGIC_,CN_ENTWRK,_FOOTER_);
  
  m_xEnt.vItem(iItem);
}

/*----------------------------------------------------------------------------*/
/* set element sequence size and element sequence of key                      */
/*----------------------------------------------------------------------------*/
void EntWrk::vElmSeq(i8 nElmSeqSize, byte* ayElmSeq)
{
  _precondition_(_MAGIC_,CN_ENTWRK,_FOOTER_);
  
  m_xEnt.vElmSeqSize(nElmSeqSize);
  
  MemLib::vCopy(this->ayElmSeq(),ayElmSeq,nElmSeqSize,CN_ENTWRK);
}

/*----------------------------------------------------------------------------*/
/* get number of levels                                                       */
/*----------------------------------------------------------------------------*/
i8 EntWrk::nOffsets(void)
{
  _precondition_(_MAGIC_,CN_ENTWRK,_FOOTER_);
  
  return m_xEnt.nOffsets();
}

/*----------------------------------------------------------------------------*/
/* set number of levels                                                       */
/*----------------------------------------------------------------------------*/
void EntWrk::vOffsets(i8 nLevels)
{
  _precondition_(_MAGIC_,CN_ENTWRK,_FOOTER_);
  
  m_xEnt.vOffsets(nLevels);
}

/*----------------------------------------------------------------------------*/
/* get ref (sub-ref to child entBuffer or row-ref at H0 leaf buffers to row)  */
/*----------------------------------------------------------------------------*/
Ref& EntWrk::xRef(void)
{
  _precondition_(_MAGIC_,CN_ENTWRK,_FOOTER_);
  
  return m_xEnt.xRef();
}

/*----------------------------------------------------------------------------*/
/* set ref (sub-ref to child entBuffer or row-ref at H0 leaf buffers to row)  */
/*----------------------------------------------------------------------------*/
void EntWrk::vRef(Ref& xRef)
{
  _precondition_(_MAGIC_,CN_ENTWRK,_FOOTER_);
  
  m_xEnt.vRef(xRef);
}

/*----------------------------------------------------------------------------*/
/* set this entwrk using given details                                        */
/*----------------------------------------------------------------------------*/
void EntWrk::vSetEnt(i8 iBufferNo, i8 i_Item, Ent* pEntry)
{
  _precondition_(_MAGIC_,CN_ENTWRK,_FOOTER_);
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // set id, transaction and offsets
  
  i8 nLevels = pEntry->nOffsets();
  vOffsets(nLevels);
  
  Ref xRef(iBufferNo,i_Item,nLevels);
  vRef(xRef);
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // copy key byte sequence into this entry
  
  byte* ayByteSeq = pEntry->ayElmSeq();
  i8 nKeyBytes = pEntry->nElmSeqSize();
  RASSERT(nKeyBytes % 8 == 0);
  
  vElmSeq(nKeyBytes,ayByteSeq);
}

/*----------------------------------------------------------------------------*/
/* show                                                                       */
/*----------------------------------------------------------------------------*/
void EntWrk::vShow(IdxColVec* pIdxColVec)
{
  _precondition_(_MAGIC_,CN_ENTWRK,_FOOTER_);
  
  //Key* aKey = reinterpret_cast<Key*>(ayByteSeq());
  Ent* aEnt = this->aEnt();
  Key* aKey = aEnt->aKey();
  
  //aKey->vShow(pIdxColVec);
  //vTrace("\n"_Stz);
  
}

/*----------------------------------------------------------------------------*/
/* get the size this (Ent+Key+Neo)                                            */
/*----------------------------------------------------------------------------*/
i8 EntWrk::nSize_EntKeyNeo(void)
{
  _precondition_(_MAGIC_,CN_ENTWRK,_FOOTER_);
  
  i8 nSizeEnt    = sizeof(Ent);
  i8 nSizeElmSeq = m_xEnt.nElmSeqSize();
  i8 nSizeNeo    = m_xEnt.nNEOsize();
  
  i8 nSize = nSizeEnt + nSizeElmSeq + nSizeNeo;
  
  return nSize;
}

/*----------------------------------------------------------------------------*/
/* get stream definition                                                      */
/*----------------------------------------------------------------------------*/
StreamDef* EntWrk::pStreamDef(void)
{
  _precondition_(_MAGIC_,CN_ENTWRK,_FOOTER_);
  
  return m_pStreamDef;
}

/*----------------------------------------------------------------------------*/
/* set stream definition                                                      */
/*----------------------------------------------------------------------------*/
void EntWrk::vStreamDef(StreamDef* pStreamDef)
{
  _precondition_(_MAGIC_,CN_ENTWRK,_FOOTER_);
  
  m_pStreamDef = pStreamDef;
}


/*----------------------------------------------------------------------------*/
/*-- Private -----------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/


/*----------------------------------------------------------------------------*/
/*-- Footer ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
__aFooterFunction(EntWrk)

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
