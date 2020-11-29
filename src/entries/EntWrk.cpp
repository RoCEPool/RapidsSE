/*============================================================================*/
/* EntWrk.cpp -- Working Index Entry                                          */
/*============================================================================*/

#include "Entries.h"

/*----------------------------------------------------------------------------*/
/*-- LifeCycle ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

__aFooterFunction(EntWrk)

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
EntWrk::EntWrk(void)
: StackObject(CN_ENTWRK),
  m_xEnt(CN_ENT,sizeof(Ent))
{
  _precondition_(_MAGIC_,CN_ENTWRK);

  m_pStreamDef = nullptr;
  m_pIndexDef  = nullptr;
  
  // -- entry --
  m_xEnt.vBuffer(0);
  m_xEnt.vItem(0);
  m_xEnt.vSkipLevels(0);
  m_xEnt.vPayload(false);
  m_xEnt.vFree(false);

  m_xEnt.vElmSeqSize(0);
  
  //vPaySize(0);  CANT SET THIS YET
  
  // -- end entry --

#ifdef WITH_CHECK_INITIALISED
  MemLib::vZero(m_yaSpace,sizeof(m_yaSpace),CN_ENTWRK);
  MemLib::vZero(m_yaPaySpace,sizeof(m_yaPaySpace),CN_ENTWRK);
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
/* add element from datum to key                                              */
/*----------------------------------------------------------------------------*/
void EntWrk::vAppendDatum(Datum* pDatum,Column* pCol)
{
  _precondition_(_MAGIC_,CN_ENTWRK,_FOOTER_);
  
  i8 nElmSize = 0;

  if (pDatum->bIsNull()) {
    vAppendNull();
    return;
  }

  if (DM_VCH == pCol->eDatum()) {
    Stz* szSrc = pDatum->szStz();
    vAppendStz(szSrc);
  }
  else {
    i8 iValue = pDatum->iInt();
    vAppendInt(iValue);
  }
}

/*----------------------------------------------------------------------------*/
/* add element from row to key                                                */
/*----------------------------------------------------------------------------*/
void EntWrk::vAppendElement(RowSingle* pRow, Column* pCol)
{
  _precondition_(_MAGIC_,CN_ENTWRK,_FOOTER_);
  
  i8 iValue = pRow->iColVal(pCol->iColumnNo());
  
  if (Null::bIsNull(iValue)) {
//    byte* ayDst = (byte*)ayElmNext();
//    nElmSize = Null::nCopy(ayDst);
//    vIncElmSeqSize(nElmSize);
    vAppendNull();
    return;
  }
  
  if (DM_VCH == pCol->eDatum()) {
    Stz* szSrc = (Stz*)iValue;
//    char* aDst = (char*)ayElmNext();
//    nElmSize = StrLib::iWordFlip(aDst, aSrc);
//    vIncElmSeqSize(nElmSize);
    vAppendStz(szSrc);
  }
  else {
//    byte* ayDst = ayElmNext();
//    nElmSize = Dtm::nCopy(ayDst,iValue);
//    vIncElmSeqSize(nElmSize);
    vAppendInt(iValue);
  }
}

/*----------------------------------------------------------------------------*/
/* add element from row to key                                                */
/*----------------------------------------------------------------------------*/
void EntWrk::vAppendElement(Row* aRow, Column* pColumn)
{
  _precondition_(_MAGIC_,CN_ENTWRK,_FOOTER_);
  
  i8 i_Col = pColumn->iColumnNo();
  
  if (aRow->bIsNull_rsb(i_Col)) {
    vAppendNull();
    return;
  }
  
  if (DM_VCH == pColumn->eDatum()) {
      Stz* szSrc = aRow->szVal_rsb(i_Col);
      vAppendStz(szSrc);
  }
  else {
    vAppendInt(aRow->iVal_rsb(i_Col));
  }
}

/*----------------------------------------------------------------------------*/
/* add element from row to key                                                */
/*----------------------------------------------------------------------------*/
void EntWrk::vAppendElement(Row* aRow, StreamPix* pStreamPix, i8 i_Col)
{
  _precondition_(_MAGIC_,CN_ENTWRK,_FOOTER_);
  
  eDM eDatum = (eDM)pStreamPix->m_xaColPix[i_Col].m_uDatum;
    
  if (aRow->bIsNull_rsb(i_Col)) {
    vAppendNull();
    return;
  }
  
  if (DM_VCH == eDatum) {
    Stz* szSrc = aRow->szVal_rsb(i_Col);
    vAppendStz(szSrc);
  }
  else {
    vAppendInt(aRow->iVal_rsb(i_Col));
  }
}

/*----------------------------------------------------------------------------*/
/* add element from row to key                                                */
/*----------------------------------------------------------------------------*/
void EntWrk::vAppendElementInt(Row* aRow, i8 iInt)
{
  _precondition_(_MAGIC_,CN_ENTWRK,_FOOTER_);
  
  vAppendInt(iInt);
}

/*----------------------------------------------------------------------------*/
/* add element from row to key                                                */
/*----------------------------------------------------------------------------*/
void EntWrk::vAppendElementStz(Row* aRow, Stz* szStr)
{
  _precondition_(_MAGIC_,CN_ENTWRK,_FOOTER_);
  
  vAppendStz(szStr);
}

/*----------------------------------------------------------------------------*/
/* add int element to key                                                     */
/*----------------------------------------------------------------------------*/
void EntWrk::vAppendInt(i8 iArg)
{
  _precondition_(_MAGIC_,CN_ENTWRK,_FOOTER_);
  
  if (KE_GENERAL == m_xEnt.eKind()) {
    byte* ayDst = ayElmNext();
  
    i8 nElmSize = Dtm::nCopy(ayDst,iArg);
  
    vIncElmSeqSize(nElmSize);
  }
  else {
    ASSERT(KE_INT == m_xEnt.eKind());
    m_xEnt.vIntElm0(iArg);
  }
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
/* add null element to key                                                    */
/*----------------------------------------------------------------------------*/
void EntWrk::vAppendNull(void)
{
  _precondition_(_MAGIC_,CN_ENTWRK,_FOOTER_);
  
  byte* ayDst = (byte*)ayElmNext();
  
  i8 nElmSize = Null::nCopy(ayDst);
  
  vIncElmSeqSize(nElmSize);
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
/* add ref element to key                                                     */
/*----------------------------------------------------------------------------*/
void EntWrk::vAppendRef(Rrf xwArg)
{
  _precondition_(_MAGIC_,CN_ENTWRK,_FOOTER_);
  
  byte* ayDst = ayElmNext();
  
  MemLib::vCopy(ayDst,&xwArg,8,CN_ENTWRK);
  
  i8 nElmSize = 8;
  vIncElmSeqSize(nElmSize);
}

/*----------------------------------------------------------------------------*/
/* add stz element to key                                                     */
/*----------------------------------------------------------------------------*/
void EntWrk::vAppendStz(Stz* szArg)
{
  _precondition_(_MAGIC_,CN_ENTWRK,_FOOTER_);
  
  char* aDst = (char*)ayElmNext();
  
  i8 nElmSize = StrLib::iWordFlip(aDst, szArg->azChar());
  
  vIncElmSeqSize(nElmSize);
}

/*----------------------------------------------------------------------------*/
/* get buffer no from ref                                                     */
/*----------------------------------------------------------------------------*/
i8 EntWrk::iBuffer(void)
{
  _precondition_(_MAGIC_,CN_ENTWRK,_FOOTER_);
  
  i8 i_Buffer = m_xEnt.iBuffer();
  
  return i_Buffer;
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
/* get entry address                                                          */
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
/* get key address                                                            */
/*----------------------------------------------------------------------------*/
Key* EntWrk::aKey(void)
{
  _precondition_(_MAGIC_,CN_ENTWRK,_FOOTER_);
  
  return m_xEnt.aKey();
}

/*----------------------------------------------------------------------------*/
/* get index column vector                                                    */
/*----------------------------------------------------------------------------*/
IndexDef* EntWrk::pIndexDef(void)
{
  _precondition_(_MAGIC_,CN_ENTWRK,_FOOTER_);
  
  return m_pIndexDef;
}

/*----------------------------------------------------------------------------*/
/* set index column vector                                                    */
/*----------------------------------------------------------------------------*/
void EntWrk::vIndexDef(IndexDef* pIndexDef)
{
  _precondition_(_MAGIC_,CN_ENTWRK,_FOOTER_);
  
  m_pIndexDef = pIndexDef;
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
  
  return m_xEnt.nSkipLevels();
}

/*----------------------------------------------------------------------------*/
/* set number of levels                                                       */
/*----------------------------------------------------------------------------*/
void EntWrk::vOffsets(i8 nLevels)
{
  _precondition_(_MAGIC_,CN_ENTWRK,_FOOTER_);
  
  m_xEnt.vSkipLevels(nLevels);
}

/*----------------------------------------------------------------------------*/
/* get pay address                                                            */
/*----------------------------------------------------------------------------*/
Pay* EntWrk::aPay(void)
{
  _precondition_(_MAGIC_,CN_ENTWRK,_FOOTER_);
    
  Pay* aPay = m_xEnt.aPay();
  
  return aPay;
}

/*----------------------------------------------------------------------------*/
/* is there a payload?                                                        */
/*----------------------------------------------------------------------------*/
bool EntWrk::bPayload(void)
{
  _precondition_(_MAGIC_,CN_ENTWRK,_FOOTER_);
  
  bool bPayLoad = m_xEnt.bPayload();
  
  return bPayLoad;
}

/*----------------------------------------------------------------------------*/
/* get payload at cell                                                        */
/*----------------------------------------------------------------------------*/
i8* EntWrk::aiPayCell(i8 iCell)
{
  _precondition_(_MAGIC_,CN_ENTWRK,_FOOTER_);
  
  Pay* aPay = m_xEnt.aPay();
  i8* aiPay = (i8*)&aPay[iCell];
  
  return aiPay;
}

/*----------------------------------------------------------------------------*/
/* get payload                                                                */
/*----------------------------------------------------------------------------*/
byte* EntWrk::ayPayload(void)
{
  _precondition_(_MAGIC_,CN_ENTWRK,_FOOTER_);
  
  Pay* aPay = m_xEnt.aPay();
  
  return aPay->ayaPayload();
}

/*----------------------------------------------------------------------------*/
/* set is there a payload                                                     */
/*----------------------------------------------------------------------------*/
void EntWrk::vPayload(bool bPayload)
{
  _precondition_(_MAGIC_,CN_ENTWRK,_FOOTER_);
    
  m_xEnt.vPayload(bPayload);
}

/*----------------------------------------------------------------------------*/
/* get payload size                                                           */
/*----------------------------------------------------------------------------*/
i8 EntWrk::nPayloadBytes(void)
{
  _precondition_(_MAGIC_,CN_ENTWRK,_FOOTER_);
  
  Pay* aPay = m_xEnt.aPay();

  i8 nPayloadBytes = aPay->nPayloadBytes();
  
  return nPayloadBytes;
}

/*----------------------------------------------------------------------------*/
/* set payload size                                                           */
/*----------------------------------------------------------------------------*/
void EntWrk::vPayloadBytes(i8 nPayloadBytes)
{
  _precondition_(_MAGIC_,CN_ENTWRK,_FOOTER_);
  
  Pay* aPay = m_xEnt.aPay();

  aPay->vPayloadBytes(nPayloadBytes);
  MemLib::vZero(&aPay[1],nPayloadBytes,CN_ENTWRK);
}

/*----------------------------------------------------------------------------*/
/* generate random entry                                                      */
/*----------------------------------------------------------------------------*/
void EntWrk::vRandomEnt(IndexDef* pIndexDef)
{
  _precondition_(_MAGIC_,CN_ENTWRK,_FOOTER_);

  Datum oDatum;
  
  i8 nIdxCol = pIndexDef->nColumns();
  for (i8 i_IdxCol = 0; i_IdxCol < nIdxCol; i_IdxCol++) {
    
    Column* pCol = pIndexDef->pGetColumn(i_IdxCol);
    
    DatumRandom::vRndDtm(&oDatum,pCol);
  
    vAppendDatum(&oDatum,pCol);
  }
  
}

/*----------------------------------------------------------------------------*/
/* get ref (sub-ref to child entBuffer or row-ref at H0 leaf buffers to row)  */
/*----------------------------------------------------------------------------*/
Rrf& EntWrk::rxRef(void)
{
  _precondition_(_MAGIC_,CN_ENTWRK,_FOOTER_);
  
  return m_xEnt.rxRef();
}

/*----------------------------------------------------------------------------*/
/* set ref (sub-ref to child entBuffer or row-ref at H0 leaf buffers to row)  */
/*----------------------------------------------------------------------------*/
void EntWrk::vRef(Rrf& xRef)
{
  _precondition_(_MAGIC_,CN_ENTWRK,_FOOTER_);
  
  m_xEnt.vRef(xRef);
}

/*----------------------------------------------------------------------------*/
/* reset this entwrk                                                          */
/*----------------------------------------------------------------------------*/
void EntWrk::vReset(void)
{
  _precondition_(_MAGIC_,CN_ENTWRK,_FOOTER_);
  
  m_xEnt.vReset();
}

/*----------------------------------------------------------------------------*/
/* set this entwrk using given details                                        */
/*----------------------------------------------------------------------------*/
void EntWrk::vSetEnt(i8 iBufferNo, i8 i_Item, Ent* pEntry)
{
  _precondition_(_MAGIC_,CN_ENTWRK,_FOOTER_);
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // set id, transaction and offsets
  
  i8 nLevels = pEntry->nSkipLevels();
  vOffsets(nLevels);
  
  Rrf xRef(iBufferNo,i_Item,nLevels);
  
  eKE eEntKind = pEntry->eKind();
  xRef.vKind(eEntKind);
  
  vRef(xRef);
  
  
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // copy key byte sequence into this entry
  
  if (KE_INT == eEntKind) {
    i8 iVal = pEntry->iIntElm0();
    *(i8*)aKey() = iVal;
    return;
  }
  
  byte* ayByteSeq = pEntry->ayElmSeq();
  i8 nKeyBytes = pEntry->nElmSeqSize();
  RASSERT(nKeyBytes % 8 == 0);
  
  vElmSeq(nKeyBytes,ayByteSeq);
}

/*----------------------------------------------------------------------------*/
/* show                                                                       */
/*----------------------------------------------------------------------------*/
void EntWrk::vShow(IndexDef* pIndexDef)
{
  _precondition_(_MAGIC_,CN_ENTWRK,_FOOTER_);
  
  //Key* aKey = reinterpret_cast<Key*>(ayByteSeq());
  Ent* aEnt = this->aEnt();
  Key* aKey = aEnt->aKey();
  
  aKey->vShow(pIndexDef);
  vTrace(" Buffer %lld Item %lld\n"_Stz,iBuffer(),iItem());
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
/* get the total size of this (Ent+Key)                                       */
/*----------------------------------------------------------------------------*/
i8 EntWrk::nTotalSize(void)
{
  _precondition_(_MAGIC_,CN_ENTWRK,_FOOTER_);
    
  i8 nSize = m_xEnt.nSizeBytes();
  
  return nSize;
}
/*----------------------------------------------------------------------------*/
//i8 EntWrk::nTotalSize(void)
//{
//  _precondition_(_MAGIC_,CN_ENTWRK,_FOOTER_);
//
//  i8 nSizeEntProper = sizeof(Ent);
//  i8 nSizeElmSeq    = m_xEnt.nElmSeqSize();
//
//  i8 nSize = nSizeEntProper + nSizeElmSeq;
//
//  return nSize;
//}


/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
