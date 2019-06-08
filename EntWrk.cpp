/*============================================================================*/
/* EntWrk.cpp -- Working Index Entry                                          */
/*============================================================================*/

#include "Data.h"
#include "StorageEngine.h"

/*----------------------------------------------------------------------------*/
/*-- LifeCycle ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
extern AttachMap* gpAttachMap;

/*----------------------------------------------------------------------------*/
/* operator new                                                               */
/*----------------------------------------------------------------------------*/
void* EntWrk::operator new(size_t)
{
  vFatal("fatal: EntWrk class cannot be new'd\n"_Stz);
  
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
: Object(CN_ENTWRK)
{
  _precondition_(_MAGIC_,CN_ENTWRK);
  
  // check initialisation of rowid
  ASSERT(0 == m_xId.iBuffer() &&
         0 == m_xId.iItem()   &&
         0 == m_xId.nLevels());
  
#ifdef WITH_CHECK_INITIALISED
  m_xWsp.vZero(CN_ENTWRK);
#endif
  
  m_xWsp.vInit();
  
  m_ayEmbedded = m_xWsp.ayArea();
  
  _checkInitialisedSize_(sizeof(EntWrk));
  
  _postcondition_(_MAGIC_,CN_ENTWRK,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
EntWrk::~EntWrk(void) noexcept(false)
{
  _precondition_(_MAGIC_,_FOOTER_);
}


/*----------------------------------------------------------------------------*/
/*-- Public ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* add element to key                                                         */
/*----------------------------------------------------------------------------*/
void EntWrk::vAddElement(Row* pRow, Column* pCol)
{
  _precondition_(_MAGIC_,CN_ENTWRK,_FOOTER_);
  
  i8 nBytes = 0;
  
  char* aDst = (char*)m_xWsp.ayLatter();
  char* aSrc = (char*)pRow->m_aiValue[pCol->iColumnNo()];
  
  if (pRow->m_aiValue[pCol->iColumnNo()] == C_DATUM_NULL) {
    nBytes = DtmLib::nCopy(pCol->eDatumType(),aDst,&aSrc);
    vTake(nBytes);
    return;
  }
  
  if (DM_VCH == pCol->eDatumType()) {
    nBytes = StrLib::iWordFlip(aDst, aSrc);
  }
  else {
    nBytes = DtmLib::nCopy(pCol->eDatumType(),aDst,&aSrc);
  }
  
  vTake(nBytes);
}

/*----------------------------------------------------------------------------*/
/* add element to key                                                         */
/*----------------------------------------------------------------------------*/
void EntWrk::vAddElement(Stz* aSrc, Column* pCol, Formatters* pFormatters)
{
  _precondition_(_MAGIC_,CN_ENTWRK,_FOOTER_);
  
  bool b;
  i8 nBytes;
  
  char* pDst = (char*)m_xWsp.ayLatter();
  char* pSrc = (char*)aSrc;
  Datum xDatum;
  
  if (pCol->eDatumType() != DM_VCH) {
    //b = Record::bExtractDatum(&xDatum,&pSrc,pCol,pFormatters);
    b = Extractor::bExtractDatum(&xDatum,&pSrc,pCol,pFormatters);
    RASSERT(b);
  }
  
  switch(pCol->eDatumType()) {
      
    case DM_BCD:
      nBytes = Bcd::nCopy((char*)pDst,(char*)&xDatum.m_iBcd);
      break;
      
    case DM_BOL:
      nBytes = Bol::nCopy((char*)pDst,(char*)&xDatum.m_iBol);
      break;
      
    case DM_DTE:
      nBytes = Dte::nCopy((char*)pDst,(char*)&xDatum.m_iDte);
      break;
      
    case DM_INT:
      nBytes = Int::nCopy(pDst,(void*)&xDatum.m_iDte);
      break;
      
    case DM_IVL:
      nBytes = Ivl::nCopy(pDst,(void*)&xDatum.m_iIvl);
      break;
      
    case DM_FLT:
      nBytes = Flt::nCopy(pDst,(void*)&xDatum.m_dFlt);
      break;
      
    case DM_TME:
      nBytes = Tme::nCopy(pDst,&xDatum.m_iTme);
      break;
      
    case DM_TSP:
      nBytes = Tsp::nCopy(pDst,&xDatum.m_iTsp);
      break;
      
    case DM_VCH:
      nBytes = StrLib::iWordFlip(pDst, pSrc);
      break;
      
    default:
      NEVER_REACH;
  }
  
  vTake(nBytes);
}

/*----------------------------------------------------------------------------*/
/* add word to key workspace                                                  */
/*----------------------------------------------------------------------------*/
void EntWrk::vAddElement(w8 wWord)
{
  _precondition_(_MAGIC_,CN_ENTWRK,_FOOTER_);
  
  char* aDst = (char*)m_xWsp.ayLatter();
  
  MemLib::vCopy(aDst, &wWord, sizeof(w8), CN_ENTWRK);
  
  vTake(sizeof(w8));
}

/*----------------------------------------------------------------------------*/
/* add min lower value to key                                                 */
/*----------------------------------------------------------------------------*/
void EntWrk::vAddMinusInfinity(void)
{
  _precondition_(_MAGIC_,CN_ENTWRK,_FOOTER_);
  
  char* aDst = (char*)m_xWsp.ayLatter();
  
  i8 iInt = C_DATUM_MINUS_INFINITY;
  Int::nCopy(aDst,(void*)&iInt);
  
  vTake(C_BYTES_PER_WORD);
}

/*----------------------------------------------------------------------------*/
/* add max upper value to key                                                 */
/*----------------------------------------------------------------------------*/
void EntWrk::vAddPlusInfinity(void)
{
  _precondition_(_MAGIC_,CN_ENTWRK,_FOOTER_);
  
  char* aDst = (char*)m_xWsp.ayLatter();
  
  i8 iInt = C_DATUM_PLUS_INFINITY;
  Int::nCopy(aDst,(void*)&iInt);
  
  vTake(C_BYTES_PER_WORD);
}

/*----------------------------------------------------------------------------*/
/* get buffer no from id                                                      */
/*----------------------------------------------------------------------------*/
i8 EntWrk::iBufferNo(void)
{
  _precondition_(_MAGIC_,_FOOTER_);
  
  return m_xId.iBuffer();
}

/*----------------------------------------------------------------------------*/
/* set buffer no in id                                                        */
/*----------------------------------------------------------------------------*/
void EntWrk::vBufferNo(i8 iBufferNo)
{
  _precondition_(_MAGIC_,_FOOTER_);
  
  m_xId.vBuffer(iBufferNo);
}

/*----------------------------------------------------------------------------*/
/* get address of embedded object                                             */
/*----------------------------------------------------------------------------*/
byte* EntWrk::ayEmbedded(void)
{  
  _precondition_(_MAGIC_,_FOOTER_);
  
  return m_ayEmbedded;
}

/*----------------------------------------------------------------------------*/
/* set address of embedded object                                             */
/*----------------------------------------------------------------------------*/
void EntWrk::vEmbedded(byte* ayEmbedded)
{
  _precondition_(_MAGIC_,_FOOTER_);
  
  m_ayEmbedded = ayEmbedded;
}

/*----------------------------------------------------------------------------*/
/* get the size of the entry required to contain the embedded object          */
/*----------------------------------------------------------------------------*/
i8 EntWrk::nEntrySize(void)
{
  _precondition_(_MAGIC_,_FOOTER_);
  
  i8 nData = nKeySize();
  
  i8 nBytes = sizeof(Ent) + nData;
  
  return nBytes;
}

/*----------------------------------------------------------------------------*/
/* extract an index key into EntWrk from this row using given index columns   */
/*----------------------------------------------------------------------------*/
void EntWrk::vExtractKey(RowBuffer* pRowBuffer, i8 i_Row, IdxColVec* pIdxColVec)
{
  // create a row
  HV_Row hRow(pRowBuffer->pColumnVec());
  
  i8 nCols = pIdxColVec->nItems();
  hRow->m_bGetAll = false;
  
  for (i8 i_Column = 0; i_Column < nCols; i_Column++) {
    
    // get the column from the index column vector
    Column* pColumn = pIdxColVec->pGet(i_Column);
    
    // mark column to be retrieved
    i8 iColumn = pColumn->iColumnNo();
    hRow->m_abGetCol[iColumn] = true;
    hRow->m_iLastCol = iColumn > hRow->m_iLastCol ? iColumn :  hRow->m_iLastCol;
  }
  
  // get the inflated compact row
  pRowBuffer->vGetRow(hRow, i_Row);
  
  for (i8 i_Column = 0; i_Column < nCols; i_Column++) {
    
    Column* pColumn = pIdxColVec->pGet(i_Column);
    
    // add the column value to the key
    vAddElement(hRow, pColumn);
    
    //vShow(pIdxColVec);
  }
  
}

/*----------------------------------------------------------------------------*/
/* compute hash of key                                                        */
/*----------------------------------------------------------------------------*/
i8 EntWrk::iHashKey(void)
{
  _precondition_(_MAGIC_,CN_ENTWRK,_FOOTER_);
  
  i8 iHash = HshLib::iHash((byte*)aKey(),nKeySize());
  
  return iHash;
}

/*----------------------------------------------------------------------------*/
/* get row id                                                                 */
/*----------------------------------------------------------------------------*/
Id& EntWrk::xId(void)
{
  _precondition_(_MAGIC_,_FOOTER_);
  
  return m_xId;
}

/*----------------------------------------------------------------------------*/
/* set row id                                                                 */
/*----------------------------------------------------------------------------*/
void EntWrk::vId(Id& xId)
{
  _precondition_(_MAGIC_,_FOOTER_);
  
  m_xId = xId;
}

/*----------------------------------------------------------------------------*/
/* get item no from id                                                        */
/*----------------------------------------------------------------------------*/
i8 EntWrk::iItemNo(void)
{
  _precondition_(_MAGIC_,_FOOTER_);
  
  return m_xId.iItem();
}

/*----------------------------------------------------------------------------*/
/* set item no in id                                                          */
/*----------------------------------------------------------------------------*/
void EntWrk::vItemNo(i8 iItemNo)
{
  _precondition_(_MAGIC_,_FOOTER_);
  
  m_xId.vItem(iItemNo);
}

/*----------------------------------------------------------------------------*/
/* get wsp area as key                                                        */
/*----------------------------------------------------------------------------*/
Key* EntWrk::aKey(void)
{
  _precondition_(_MAGIC_,CN_ENTWRK,_FOOTER_);
  
  return reinterpret_cast<Key*>(ayEmbedded());
}

/*----------------------------------------------------------------------------*/
/* get key size                                                               */
/*----------------------------------------------------------------------------*/
i8 EntWrk::nKeySize(void)
{
  _precondition_(_MAGIC_,CN_ENTWRK,_FOOTER_);
  
  i8 nSize = m_xWsp.nUsed();
  
  return nSize;
}

/*----------------------------------------------------------------------------*/
/* set key size                                                               */
/*----------------------------------------------------------------------------*/
void EntWrk::vKeySize(i8 nKeySize)
{
  _precondition_(_MAGIC_,CN_ENTWRK,_FOOTER_);
  
  m_xWsp.vUsed(nKeySize);
}

/*----------------------------------------------------------------------------*/
/* get nLevel                                                                 */
/*----------------------------------------------------------------------------*/
i8 EntWrk::nLevels(void)
{
  _precondition_(_MAGIC_,_FOOTER_);
  
  return m_xId.nLevels();
}

/*----------------------------------------------------------------------------*/
/* get offset bytes                                                           */
/*----------------------------------------------------------------------------*/
i8 EntWrk::nOffsetSize(void)
{
  _precondition_(_MAGIC_,_FOOTER_);
  
  i8 nBytes = sizeof(i4) * m_xId.nLevels();
  
  return nBytes;
}

/*----------------------------------------------------------------------------*/
/* set number of levels                                                       */
/*----------------------------------------------------------------------------*/
void EntWrk::vLevels(i8 nLevels)
{
  _precondition_(_MAGIC_,_FOOTER_);
  
  m_xId.vLevels(nLevels);
}

/*----------------------------------------------------------------------------*/
/* set a random number of levels                                              */
/*----------------------------------------------------------------------------*/
void EntWrk::vRandomLevels(void)
{
  _precondition_(_MAGIC_,_FOOTER_);
  
  Task* pTask = Task::pExecutingTask();
  
  i8 nLevels = Random::iLogLevel(3,C_FLAT_FREQ + 1,pTask->i_NextRnd()) + 1;

  m_xId.vLevels(nLevels);
}

/*----------------------------------------------------------------------------*/
/* get the entry buffer that this key references                              */
/*----------------------------------------------------------------------------*/
EntBuffer* EntWrk::pReferencedEntBuffer(void)
{
  _precondition_(_MAGIC_,CN_ENTWRK,_FOOTER_);
  
  EntBuffer* pEntBuffer = EntBuffer::pRefdEntBuffer(xId());
  
  return pEntBuffer;
}

/*----------------------------------------------------------------------------*/
/* reset the key wsp                                                          */
/*----------------------------------------------------------------------------*/
void EntWrk::vReset(void)
{
  _precondition_(_MAGIC_,CN_ENTWRK,_FOOTER_);
  
  vKeySize(0);
}

/*----------------------------------------------------------------------------*/
/* set entry using given key working entry                                    */
/*----------------------------------------------------------------------------*/
void EntWrk::vSetEntry(Ent* pEntry)
{
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // set id, transaction and offsets
  
  pEntry->vId(xId());
  pEntry->vLevels(nLevels());
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // copy key into this entry
  
  Key* aSrcKey = aKey();
  Key* aDstKey = _aKey(pEntry->ayKeyBytes());
  i8 nKeySize  = this->nKeySize();
  
  MemLib::vCopy(aDstKey,aSrcKey,nKeySize,CN_ENT);
}

/*----------------------------------------------------------------------------*/
/* set key using given working entry                                          */
/*----------------------------------------------------------------------------*/
void EntWrk::vSetKey(Ent* pEntry, i8 iBufferNo, i8 iItem, IdxColVec* pIdxColVec)
{
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // set id, transaction and offsets
  
  i8 nOffsets = pEntry->nLevels();
  vLevels(nOffsets);
  
  Id xId(iBufferNo,iItem,nOffsets);
  vId(xId);
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // copy key into this entry
  
  Key* aKeyData = _aKey(pEntry->ayKeyBytes());
  //i8 iKeySize = aKeyData->nKeySize(pIdxColVec);
  i8 iKeySize = pEntry->nSizeKey();
  RASSERT(iKeySize % 8 == 0);
  
  vEmbedded((byte*)aKeyData);
  vKeySize(iKeySize);
}

/*----------------------------------------------------------------------------*/
/* show                                                                       */
/*----------------------------------------------------------------------------*/
void EntWrk::vShow(IdxColVec* pIdxColVec)
{
  _precondition_(_MAGIC_,CN_ENTWRK,_FOOTER_);
  
  Key* aKey = this->aKey();
  aKey->vShow(pIdxColVec);
  vTrace("\n"_Stz);
}

/*----------------------------------------------------------------------------*/
/* take chunk                                                                 */
/*----------------------------------------------------------------------------*/
void EntWrk::vTake(i8 iSize)
{
  _precondition_(_MAGIC_,_FOOTER_);
  
  m_xWsp.vTake(iSize);
}


/*----------------------------------------------------------------------------*/
/*-- Footer ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
__aFooterFunction(EntWrk)

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
