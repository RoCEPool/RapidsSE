/*============================================================================*/
/* KeyWrk.cpp -- Working Key                                                  */
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
void* KeyWrk::operator new(size_t)
{
  vFatal("fatal: KeyWrk class cannot be new'd\n"_Stz);
  
  NEVER_REACH;
}

/*----------------------------------------------------------------------------*/
/* operator new                                                               */
/*----------------------------------------------------------------------------*/
void* KeyWrk::operator new(size_t, KeyWrk* pKeyWrk)
{
  return pKeyWrk;
}

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
KeyWrk::KeyWrk(void)
: Object(CN_KEYWRK)
{
  _precondition_(_MAGIC_,CN_KEYWRK);
  
  // check initialisation of rowid
  ASSERT(0 == m_xId.iBuffer() &&
         0 == m_xId.iItem()   &&
         0 == m_xId.nLevels());
  
#ifdef WITH_CHECK_INITIALISED
  m_xWsp.vZero(CN_KEYWRK);
#endif
  
  m_xWsp.vInit();
  
  m_ayEmbedded = m_xWsp.ayArea();
  
  _checkInitialisedSize_(sizeof(KeyWrk));
  
  _postcondition_(_MAGIC_,CN_KEYWRK,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
KeyWrk::~KeyWrk(void) noexcept(false)
{
  _precondition_(_MAGIC_,_FOOTER_);
}


/*----------------------------------------------------------------------------*/
/*-- Public ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* add element to key                                                         */
/*----------------------------------------------------------------------------*/
void KeyWrk::vAddDatum(Stz* aSrc, Column* pCol, Formatters* pFormatters)
{
  _precondition_(_MAGIC_,CN_KEYWRK,_FOOTER_);
  
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
/* add element to key                                                         */
/*----------------------------------------------------------------------------*/
void KeyWrk::vAddDatum(Datum& oDatum, Column* pCol)
{
  _precondition_(_MAGIC_,CN_KEYWRK,_FOOTER_);
  
  i8 nBytes;
  
  char* aDst = (char*)m_xWsp.ayLatter();
  char* aSrc;
  
  if (DM_VCH == pCol->eDatumType()) {
    aSrc = (char*)(oDatum.m_sVch.aStz());
    nBytes = StrLib::iWordFlip(aDst, aSrc);
  }
  else {
    aSrc = (char*)(&oDatum.m_iInt);
    nBytes = DtmLib::nCopy(pCol->eDatumType(),aDst,aSrc);
  }
  
  vTake(nBytes);
}

/*----------------------------------------------------------------------------*/
/* add element to key                                                         */
/*----------------------------------------------------------------------------*/
void KeyWrk::vAddElement(Row* pRow, Column* pCol)
{
  _precondition_(_MAGIC_,CN_KEYWRK,_FOOTER_);
  
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
/* add min lower value to key                                                 */
/*----------------------------------------------------------------------------*/
void KeyWrk::vAddMinusInfinity(void)
{
  _precondition_(_MAGIC_,CN_KEYWRK,_FOOTER_);
  
  char* aDst = (char*)m_xWsp.ayLatter();
  
  i8 iInt = C_DATUM_MINUS_INFINITY;
  Int::nCopy(aDst,(void*)&iInt);
  
  vTake(C_BYTES_PER_WORD);
}

/*----------------------------------------------------------------------------*/
/* add max upper value to key                                                 */
/*----------------------------------------------------------------------------*/
void KeyWrk::vAddPlusInfinity(void)
{
  _precondition_(_MAGIC_,CN_KEYWRK,_FOOTER_);
  
  char* aDst = (char*)m_xWsp.ayLatter();
  
  i8 iInt = C_DATUM_PLUS_INFINITY;
  Int::nCopy(aDst,(void*)&iInt);
  
  vTake(C_BYTES_PER_WORD);
}

/*----------------------------------------------------------------------------*/
/* add word to key workspace                                                  */
/*----------------------------------------------------------------------------*/
void KeyWrk::vAddWord(w8 wWord)
{
  _precondition_(_MAGIC_,CN_KEYWRK,_FOOTER_);
  
  char* aDst = (char*)m_xWsp.ayLatter();
  
  MemLib::vCopy(aDst, &wWord, sizeof(w8), CN_KEYWRK);
  
  vTake(sizeof(w8));
}

/*----------------------------------------------------------------------------*/
/* get buffer no from id                                                      */
/*----------------------------------------------------------------------------*/
i8 KeyWrk::iBufferNo(void)
{
  _precondition_(_MAGIC_,_FOOTER_);
  
  return m_xId.iBuffer();
}

/*----------------------------------------------------------------------------*/
/* set buffer no in id                                                        */
/*----------------------------------------------------------------------------*/
void KeyWrk::vBufferNo(i8 iBufferNo)
{
  _precondition_(_MAGIC_,_FOOTER_);
  
  m_xId.vBuffer(iBufferNo);
}

/*----------------------------------------------------------------------------*/
/* get address of embedded object                                             */
/*----------------------------------------------------------------------------*/
byte* KeyWrk::ayEmbedded(void)
{  
  _precondition_(_MAGIC_,_FOOTER_);
  
  return m_ayEmbedded;
}

/*----------------------------------------------------------------------------*/
/* set address of embedded object                                             */
/*----------------------------------------------------------------------------*/
void KeyWrk::vEmbedded(byte* ayEmbedded)
{
  _precondition_(_MAGIC_,_FOOTER_);
  
  m_ayEmbedded = ayEmbedded;
}

/*----------------------------------------------------------------------------*/
/* get the size of the entry required to contain the embedded object          */
/*----------------------------------------------------------------------------*/
i8 KeyWrk::nEntrySize(void)
{
  _precondition_(_MAGIC_,_FOOTER_);
  
  i8 nData = nDataSize();
  
  i8 nBytes = sizeof(Ent) + nData;
  
  return nBytes;
}

/*----------------------------------------------------------------------------*/
/* extract an index key into KeyWrk from this row using given index columns   */
/*----------------------------------------------------------------------------*/
void KeyWrk::vExtractKey(RowBuffer* pRowBuffer, i8 i_Row, IdxColVec* pIdxColVec)
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
i8 KeyWrk::iHashKey(void)
{
  _precondition_(_MAGIC_,CN_KEYWRK,_FOOTER_);
  
  i8 iHash = HshLib::iHash((byte*)aKey(),nKeySize());
  
  return iHash;
}

/*----------------------------------------------------------------------------*/
/* get row id                                                                 */
/*----------------------------------------------------------------------------*/
Id& KeyWrk::xId(void)
{
  _precondition_(_MAGIC_,_FOOTER_);
  
  return m_xId;
}

/*----------------------------------------------------------------------------*/
/* set row id                                                                 */
/*----------------------------------------------------------------------------*/
void KeyWrk::vId(Id& xId)
{
  _precondition_(_MAGIC_,_FOOTER_);
  
  m_xId = xId;
}

/*----------------------------------------------------------------------------*/
/* get item no from id                                                        */
/*----------------------------------------------------------------------------*/
i8 KeyWrk::iItemNo(void)
{
  _precondition_(_MAGIC_,_FOOTER_);
  
  return m_xId.iItem();
}

/*----------------------------------------------------------------------------*/
/* set item no in id                                                          */
/*----------------------------------------------------------------------------*/
void KeyWrk::vItemNo(i8 iItemNo)
{
  _precondition_(_MAGIC_,_FOOTER_);
  
  m_xId.vItem(iItemNo);
}

/*----------------------------------------------------------------------------*/
/* get wsp area as key                                                        */
/*----------------------------------------------------------------------------*/
Key* KeyWrk::aKey(void)
{
  _precondition_(_MAGIC_,CN_KEYWRK,_FOOTER_);
  
  return reinterpret_cast<Key*>(ayEmbedded());
}

/*----------------------------------------------------------------------------*/
/* get key size                                                               */
/*----------------------------------------------------------------------------*/
i8 KeyWrk::nKeySize(void)
{
  _precondition_(_MAGIC_,CN_KEYWRK,_FOOTER_);
  
  return nDataSize();
}

/*----------------------------------------------------------------------------*/
/* set key size                                                               */
/*----------------------------------------------------------------------------*/
void KeyWrk::vKeySize(i8 nKeySize)
{
  _precondition_(_MAGIC_,CN_KEYWRK,_FOOTER_);
  
  vDataSize(nKeySize);
}

/*----------------------------------------------------------------------------*/
/* get nLevel                                                                 */
/*----------------------------------------------------------------------------*/
i8 KeyWrk::nLevels(void)
{
  _precondition_(_MAGIC_,_FOOTER_);
  
  return m_xId.nLevels();
}

/*----------------------------------------------------------------------------*/
/* get offset bytes                                                           */
/*----------------------------------------------------------------------------*/
i8 KeyWrk::nOffsetSize(void)
{
  _precondition_(_MAGIC_,_FOOTER_);
  
  i8 nBytes = sizeof(i4) * m_xId.nLevels();
  
  return nBytes;
}

/*----------------------------------------------------------------------------*/
/* set number of levels                                                       */
/*----------------------------------------------------------------------------*/
void KeyWrk::vLevels(i8 nLevels)
{
  _precondition_(_MAGIC_,_FOOTER_);
  
  m_xId.vLevels(nLevels);
}

/*----------------------------------------------------------------------------*/
/* set a random number of levels                                              */
/*----------------------------------------------------------------------------*/
void KeyWrk::vRandomLevels(void)
{
  _precondition_(_MAGIC_,_FOOTER_);
  
  Task* pTask = Task::pExecutingTask();
  
  i8 nLevels = Random::iLogLevel(3,C_FLAT_FREQ + 1,pTask->i_NextRnd()) + 1;

  m_xId.vLevels(nLevels);
}

/*----------------------------------------------------------------------------*/
/* get the entry buffer that this key references                              */
/*----------------------------------------------------------------------------*/
EntBuffer* KeyWrk::pReferencedEntBuffer(void)
{
  _precondition_(_MAGIC_,CN_KEYWRK,_FOOTER_);
  
  EntBuffer* pEntBuffer = EntBuffer::pRefdEntBuffer(xId());
  
  return pEntBuffer;
}

/*----------------------------------------------------------------------------*/
/* reset the key wsp                                                          */
/*----------------------------------------------------------------------------*/
void KeyWrk::vReset(void)
{
  _precondition_(_MAGIC_,CN_KEYWRK,_FOOTER_);
  
  vDataSize(0);
}

/*----------------------------------------------------------------------------*/
/* set entry using given key working entry                                    */
/*----------------------------------------------------------------------------*/
void KeyWrk::vSetEntry(Ent* pEntry)
{
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // set id, transaction and offsets
  
  pEntry->vId(xId());
  pEntry->vLevels(nLevels());
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // copy key into this entry
  
  Key* aSrcKey = aKey();
  Key* aDstKey = _aKey(pEntry->aDataArea());
  i8 nKeySize  = this->nKeySize();
  
  MemLib::vCopy(aDstKey,aSrcKey,nKeySize,CN_ENT);
}

/*----------------------------------------------------------------------------*/
/* set key using given working entry                                          */
/*----------------------------------------------------------------------------*/
void KeyWrk::vSetKey(Ent* pEntry, i8 iBufferNo, i8 iItem, IdxColVec* pIdxColVec)
{
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // set id, transaction and offsets
  
  i8 nOffsets = pEntry->nLevels();
  vLevels(nOffsets);
  
  Id xId(iBufferNo,iItem,nOffsets);
  vId(xId);
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // copy key into this entry
  
  Key* aKeyData = _aKey(pEntry->aDataArea());
  //i8 iKeySize = aKeyData->nKeySize(pIdxColVec);
  i8 iKeySize = pEntry->nSizeData();
  RASSERT(iKeySize % 8 == 0);
  
  vEmbedded((byte*)aKeyData);
  vKeySize(iKeySize);
}

/*----------------------------------------------------------------------------*/
/* show                                                                       */
/*----------------------------------------------------------------------------*/
void KeyWrk::vShow(IdxColVec* pIdxColVec)
{
  _precondition_(_MAGIC_,CN_KEYWRK,_FOOTER_);
  
  Key* aKey = this->aKey();
  aKey->vShow(pIdxColVec);
  vTrace("\n"_Stz);
}

/*----------------------------------------------------------------------------*/
/* take chunk                                                                 */
/*----------------------------------------------------------------------------*/
void KeyWrk::vTake(i8 iSize)
{
  _precondition_(_MAGIC_,_FOOTER_);
  
  m_xWsp.vTake(iSize);
}

/*----------------------------------------------------------------------------*/
/*-- Protected ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* get embedded object size                                                   */
/*----------------------------------------------------------------------------*/
i8 KeyWrk::nDataSize(void)
{
  _precondition_(_MAGIC_,_FOOTER_);
  
  return m_xWsp.nUsed();
}

/*----------------------------------------------------------------------------*/
/* set embedded object size                                                   */
/*----------------------------------------------------------------------------*/
void KeyWrk::vDataSize(i8 nDataBytes)
{
  _precondition_(_MAGIC_,_FOOTER_);
  
  m_xWsp.vUsed(nDataBytes);
}


/*----------------------------------------------------------------------------*/
/*-- Footer ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
__aFooterFunction(KeyWrk)

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
