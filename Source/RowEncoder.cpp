/*============================================================================*/
/* RowEncoder.hpp -- Row Encoder                                              */
/*============================================================================*/

#include "Data.h"

#include "StorageEngine.h"

/*----------------------------------------------------------------------------*/
/* operator new                                                               */
/*----------------------------------------------------------------------------*/
void* RowEncoder::operator new(size_t)
{
  void* pObject = HeapMem::pAllocate(CN_ROWENCODER);
  
  return pObject;
}

/*----------------------------------------------------------------------------*/
/* operator delete                                                            */
/*----------------------------------------------------------------------------*/
void RowEncoder::operator delete(void* pAddr)
{
  RowEncoder* pRowEncoder = reinterpret_cast<RowEncoder*>(pAddr);
  
  HeapMem::vFree(pRowEncoder);
}

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
RowEncoder::RowEncoder(RowParts* pRowParts)
: Object(CN_ROWENCODER)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ROWENCODER,_FOOTER_);
  
  m_i_CacheOffset = 8;
  
  m_pRowParts = pRowParts;
  
  m_pRowParts->vReset();
  
  _checkInitialised_(CN_ROWENCODER);
  
  i8 i_Head = m_pRowParts->iAllotHead();
  RASSERT(0 == i_Head);
  
  CacheLine* aCacheLine = m_pRowParts->aGet(i_Head);
  aCacheLine->vSetAll(0xFF);
  aCacheLine->v8B(0,0);

  _postcondition_(_ADDRESS_,_MAGIC_,CN_ROWENCODER,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
RowEncoder::~RowEncoder(void) noexcept(false)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ROWENCODER,_FOOTER_);
}


/*----------------------------------------------------------------------------*/
/*-- Public ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* encode given datum into cache line                                         */
/*----------------------------------------------------------------------------*/
void RowEncoder::vEncode(Column* pColumn, Datum* pDatum, i8& iHash)
{
  i8 i_Column = pColumn->iColumnNo();
  eDM eDatum  = pColumn->eDatumType();
  
  // don't encode columns 16+
  if (i_Column >= 16)
    return;
  
  switch (eDatum) {
    case DM_BCD: vEncodeBcd(pColumn,pDatum,iHash); return;
    case DM_BOL: vEncodeBol(pColumn,pDatum,iHash); return;
    case DM_DTE: vEncodeDte(pColumn,pDatum,iHash); return;
    case DM_FLT: vEncodeFlt(pColumn,pDatum,iHash); return;
    case DM_INT: vEncodeInt(pColumn,pDatum,iHash); return;
    case DM_IVL: vEncodeIvl(pColumn,pDatum,iHash); return;
    case DM_TME: vEncodeTme(pColumn,pDatum,iHash); return;
    case DM_TSP: vEncodeTsp(pColumn,pDatum,iHash); return;
    case DM_VCH: vEncodeVch(pColumn,pDatum,iHash); return;
    default:
      NEVER_REACH;
  }
}

/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/*-- Private -----------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* encode decimal                                                             */
/*----------------------------------------------------------------------------*/
void RowEncoder::vEncodeBcd(Column* pColumn, Datum* pDatum, i8& iHash)
{
  CacheLine* aCacheLine = m_pRowParts->aGet(0);
  
  i8 i_Column = pColumn->iColumnNo();
  w8 wVal = *(i8*)pDatum->ayDatum();
  
  WordView wEncoded;
  wEncoded.m_wWord = wVal;
  
  // hash the column value if part of partition key
  if (pColumn->bIsPartitionKey()) {
    iHash += HshLib::iHash(&wEncoded.m_wWord,sizeof(w8));
  }
  
  i8 nBytesEncoded;
  i8 iEncoding = RowCoder::iBcdEncoding(wEncoded,nBytesEncoded);
  RASSERT(nBytesEncoded == RowCoder::nEncodedBytes(iEncoding,DM_BCD));

  // set 4bit meta data for column value
  aCacheLine->v1N(i_Column,iEncoding);
  
  // set n-byte data for column value
  if (nBytesEncoded)
    aCacheLine->vPut(m_i_CacheOffset,wVal,iEncoding);
  
  m_i_CacheOffset += nBytesEncoded;
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // cross-check
  
  WordView wDecoded;
  wDecoded.m_wWord = RowCoder::wDecode(pColumn,aCacheLine);
  RASSERT(wVal == wDecoded.m_iI8);
}

/*----------------------------------------------------------------------------*/
/* encode boolean                                                             */
/*----------------------------------------------------------------------------*/
void RowEncoder::vEncodeBol(Column* pColumn, Datum* pDatum, i8& iHash)
{
  CacheLine* aCacheLine = m_pRowParts->aGet(0);
  
  i8 i_Column = pColumn->iColumnNo();
  w8 wVal = *(i8*)pDatum->ayDatum();

  WordView wEncoded;
  wEncoded.m_wWord = wVal;
  
  // hash the column value if part of partition key
  if (pColumn->bIsPartitionKey()) {
    iHash += HshLib::iHash(&wEncoded.m_wWord,sizeof(w8));
  }
  
  i8 nBytesEncoded;
  i8 iEncoding = RowCoder::iBolEncoding(wEncoded,nBytesEncoded);
  RASSERT(nBytesEncoded == RowCoder::nEncodedBytes(iEncoding,DM_BOL));

  // set 4bit meta data for column value
  aCacheLine->v1N(i_Column,iEncoding);
  
  m_i_CacheOffset += nBytesEncoded;

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // cross-check
  
  WordView wDecoded;
  wDecoded.m_wWord = RowCoder::wDecode(pColumn,aCacheLine);
  RASSERT(wVal == wDecoded.m_iI8);
}

/*----------------------------------------------------------------------------*/
/* encode date                                                                */
/*----------------------------------------------------------------------------*/
void RowEncoder::vEncodeDte(Column* pColumn, Datum* pDatum, i8& iHash)
{
  CacheLine* aCacheLine = m_pRowParts->aGet(0);
  
  i8 i_Column = pColumn->iColumnNo();
  w8 wVal = *(i8*)pDatum->ayDatum();
  
  WordView wEncoded;
  wEncoded.m_wWord = wVal;
  
  // hash the column value if part of partition key
  if (pColumn->bIsPartitionKey()) {
    iHash += HshLib::iHash(&wEncoded.m_wWord,sizeof(w8));
  }
  
  i8 nBytesEncoded;
  i8 iEncoding = RowCoder::iDteEncoding(wEncoded,nBytesEncoded);
  RASSERT(nBytesEncoded == RowCoder::nEncodedBytes(iEncoding,DM_DTE));

  // set 4bit meta data for column value
  aCacheLine->v1N(i_Column,iEncoding);
  
  // set n-byte data for column value
  if (nBytesEncoded)
    aCacheLine->vPut(m_i_CacheOffset,wEncoded.m_iI8,iEncoding);
  
  m_i_CacheOffset += nBytesEncoded;

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // cross-check
  
  WordView wDecoded;
  wDecoded.m_wWord = RowCoder::wDecode(pColumn,aCacheLine);
  RASSERT(wVal == wDecoded.m_iI8);
}

/*----------------------------------------------------------------------------*/
/* encode float                                                               */
/*----------------------------------------------------------------------------*/
void RowEncoder::vEncodeFlt(Column* pColumn, Datum* pDatum, i8& iHash)
{
  CacheLine* aCacheLine = m_pRowParts->aGet(0);
  
  i8 i_Column = pColumn->iColumnNo();
  w8 wVal = *(i8*)pDatum->ayDatum();

  WordView wEncoded;
  wEncoded.m_wWord = wVal;
  
  // hash the column value if part of partition key
  if (pColumn->bIsPartitionKey()) {
    iHash += HshLib::iHash(&wEncoded.m_wWord,sizeof(w8));
  }
  
  i8 nBytesEncoded;
  i8 iEncoding = RowCoder::iFltEncoding(wEncoded,nBytesEncoded);
  RASSERT(nBytesEncoded == RowCoder::nEncodedBytes(iEncoding,DM_FLT));

  // set 4bit meta data for column value
  aCacheLine->v1N(i_Column,iEncoding);
  
  // set n-byte data for column value
  if (nBytesEncoded)
    aCacheLine->vPut(m_i_CacheOffset,wVal,iEncoding);
  
  m_i_CacheOffset += nBytesEncoded;

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // cross-check
  
  WordView wDecoded;
  wDecoded.m_wWord = RowCoder::wDecode(pColumn,aCacheLine);
  RASSERT(wVal == wDecoded.m_iI8);
}

/*----------------------------------------------------------------------------*/
/* encode integer                                                             */
/*----------------------------------------------------------------------------*/
void RowEncoder::vEncodeInt(Column* pColumn, Datum* pDatum, i8& iHash)
{
  CacheLine* aCacheLine = m_pRowParts->aGet(0);
  
  i8 i_Column = pColumn->iColumnNo();
  w8 wVal = *(i8*)pDatum->ayDatum();
  
  WordView wEncoded;
  wEncoded.m_wWord = wVal;
  
  // hash the column value if part of partition key
  if (pColumn->bIsPartitionKey()) {
    iHash += HshLib::iHash(&wEncoded.m_wWord,sizeof(w8));
  }
  
  i8 nBytesEncoded;
  i8 iEncoding = RowCoder::iIntEncoding(wEncoded,nBytesEncoded);
  RASSERT(nBytesEncoded == RowCoder::nEncodedBytes(iEncoding,DM_INT));

  // set 4bit meta data for column value
  aCacheLine->v1N(i_Column,iEncoding);
  
  // set n-byte data for column value
  if (nBytesEncoded)
    aCacheLine->vPut(m_i_CacheOffset,wVal,iEncoding);
  
  m_i_CacheOffset += nBytesEncoded;

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // cross-check
  
  WordView wDecoded;
  wDecoded.m_wWord = RowCoder::wDecode(pColumn,aCacheLine);
  RASSERT(wVal == wDecoded.m_iI8);
}

/*----------------------------------------------------------------------------*/
/* encode interval                                                            */
/*----------------------------------------------------------------------------*/
void RowEncoder::vEncodeIvl(Column* pColumn, Datum* pDatum, i8& iHash)
{
  CacheLine* aCacheLine = m_pRowParts->aGet(0);
  
  i8 i_Column = pColumn->iColumnNo();
  w8 wVal = *(i8*)pDatum->ayDatum();
  
  WordView wEncoded;
  wEncoded.m_wWord = wVal;
  
  // hash the column value if part of partition key
  if (pColumn->bIsPartitionKey()) {
    iHash += HshLib::iHash(&wEncoded.m_wWord,sizeof(w8));
  }
  
  i8 nBytesEncoded;
  i8 iEncoding = RowCoder::iIvlEncoding(wEncoded,nBytesEncoded);
  RASSERT(nBytesEncoded == RowCoder::nEncodedBytes(iEncoding,DM_IVL));

  // set 4bit meta data for column value
  aCacheLine->v1N(i_Column,iEncoding);
  
  // set n-byte data for column value
  if (nBytesEncoded)
    aCacheLine->vPut(m_i_CacheOffset,wVal,iEncoding);
  
  m_i_CacheOffset += nBytesEncoded;

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // cross-check
  
  WordView wDecoded;
  wDecoded.m_wWord = RowCoder::wDecode(pColumn,aCacheLine);
  RASSERT(wVal == wDecoded.m_iI8);
}

/*----------------------------------------------------------------------------*/
/* encode time                                                                */
/*----------------------------------------------------------------------------*/
void RowEncoder::vEncodeTme(Column* pColumn, Datum* pDatum, i8& iHash)
{
  CacheLine* aCacheLine = m_pRowParts->aGet(0);
  
  i8 i_Column = pColumn->iColumnNo();
  w8 wVal = *(i8*)pDatum->ayDatum();
  
  WordView wEncoded;
  wEncoded.m_wWord = wVal;
  
  // hash the column value if part of partition key
  if (pColumn->bIsPartitionKey()) {
    iHash += HshLib::iHash(&wEncoded.m_wWord,sizeof(w8));
  }
  
  i8 nBytesEncoded;
  i8 iEncoding = RowCoder::iTmeEncoding(wEncoded,nBytesEncoded);
  RASSERT(nBytesEncoded == RowCoder::nEncodedBytes(iEncoding,DM_TME));

  // set 4bit meta data for column value
  aCacheLine->v1N(i_Column,iEncoding);
  
  // set n-byte data for column value
  if (nBytesEncoded)
    aCacheLine->vPut(m_i_CacheOffset,wEncoded.m_iI8,iEncoding);
  
  m_i_CacheOffset += nBytesEncoded;

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // cross-check
  
  WordView wDecoded;
  wDecoded.m_wWord = RowCoder::wDecode(pColumn,aCacheLine);
  RASSERT(wVal == wDecoded.m_iI8);
}

/*----------------------------------------------------------------------------*/
/* encode timestamp                                                           */
/*----------------------------------------------------------------------------*/
void RowEncoder::vEncodeTsp(Column* pColumn, Datum* pDatum, i8& iHash)
{
  CacheLine* aCacheLine = m_pRowParts->aGet(0);
  
  i8 i_Column = pColumn->iColumnNo();
  w8 wVal = *(i8*)pDatum->ayDatum();

  WordView wEncoded;
  wEncoded.m_wWord = wVal;
  
  // hash the column value if part of partition key
  if (pColumn->bIsPartitionKey()) {
    iHash += HshLib::iHash(&wEncoded.m_wWord,sizeof(w8));
  }
  
  i8 nBytesEncoded;
  i8 iEncoding = RowCoder::iTspEncoding(wEncoded,nBytesEncoded);
  RASSERT(nBytesEncoded == RowCoder::nEncodedBytes(iEncoding,DM_TSP));

  // set 4bit meta data for column value
  aCacheLine->v1N(i_Column,iEncoding);
  
  // set n-byte data for column value
  if (nBytesEncoded)
    aCacheLine->vPut(m_i_CacheOffset,wEncoded.m_iI8,iEncoding);
  
  m_i_CacheOffset += nBytesEncoded;

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // cross-check
  
  WordView wDecoded;
  wDecoded.m_wWord = RowCoder::wDecode(pColumn,aCacheLine);
  RASSERT(wVal == wDecoded.m_iI8);
}

/*----------------------------------------------------------------------------*/
/* encode varchar                                                             */
/*----------------------------------------------------------------------------*/
void RowEncoder::vEncodeVch(Column* pColumn, Datum* pDatum, i8& iHash)
{
  CacheLine* aCacheLine = m_pRowParts->aGet(0);
  
  i8 i_Column = pColumn->iColumnNo();
  w8 wVal = (w8)pDatum->ayDatum();

  WordView wEncoded;
  wEncoded.m_wWord = wVal;
  
  // hash the column value if part of partition key
  if (pColumn->bIsPartitionKey()) {
    iHash += HshLib::iHash(&wEncoded.m_wWord,sizeof(w8));
  }
  
  i8 nBytesEncoded;
  i8 iEncoding = RowCoder::iVchEncoding(wEncoded,nBytesEncoded);
  //RASSERT(nBytesEncoded == nEncodedBytes(iEncoding,DM_TSP));
  
  // set 4bit meta data for column value
  aCacheLine->v1N(i_Column,iEncoding);
  
  // set n-byte data for column value
  if (iEncoding == MN_SHOCO_2B) {
    // we would encode the shoco version of the string into a cache line
    
    // allot a tail cache line
    i8 i_Tail = m_pRowParts->iAllotTail();
    CacheLine* aShocoLine = m_pRowParts->aGet(i_Tail);

    // set data for column value as the index of the shoco cache line
    aCacheLine->vPut(m_i_CacheOffset,i_Tail,MN_2B);
    
    // compress varchar into tail shoco cache line
    i8 iLenCompressed =
      Compress::iIntoShoco(((byte*)aShocoLine)+8, C_BYTES_PER_CACHE_LINE-8,
                           (char*)wVal, nBytesEncoded);

    // set length of the compressed string encoded into tail shoco cache line
    aShocoLine->v1N(0,iLenCompressed);
    
    MemLib::vWordDump(aShocoLine,sizeof(CacheLine));
    m_i_CacheOffset += 2;

//    s256 sDecomp;
//    i8 iLenDecompressed =
//      Compress::iFromShoco(sDecomp.aChar(), 255,
//                           ((byte*)aShocoLine)+8, C_BYTES_PER_CACHE_LINE-8);
//    sDecomp.aChar()[iLenDecompressed] = '\0';
//    vTrace("Decomp: %s\n"_Stz,sDecomp.aChar());
  }
  else {
    aCacheLine->vPut(m_i_CacheOffset,wVal,nBytesEncoded);
    m_i_CacheOffset += nBytesEncoded;
  }


  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // cross-check
  
}

/*----------------------------------------------------------------------------*/
/*-- Footer ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* get location of footer                                                     */
/*----------------------------------------------------------------------------*/
Footer* RowEncoder::aFooter(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ROWENCODER);
  
  return &m_xFooter;
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
