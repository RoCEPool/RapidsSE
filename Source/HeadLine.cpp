/*============================================================================*/
/* HeadLine.hpp -- a head line                                                */
/*============================================================================*/

#include "Data.h"

/*----------------------------------------------------------------------------*/
/*-- LifeCycle ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
HeadLine::HeadLine(void)
{
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
HeadLine::~HeadLine(void) noexcept(false)
{
}

/*----------------------------------------------------------------------------*/
/*-- Public ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* store meta code                                                            */
/*----------------------------------------------------------------------------*/
void HeadLine::vMetaCode(i8 i_Meta, w8 wArg)
{
  v1B((C_W_1B + i_Meta),wArg);
}

/*----------------------------------------------------------------------------*/
/* return size of head in bytes                                               */
/*----------------------------------------------------------------------------*/
i8 HeadLine::nBytes(void)
{
  i8 nSizeBytes = nWords() * C_BYTES_PER_WORD;
  
  return nSizeBytes;
}

/*----------------------------------------------------------------------------*/
/* return size of head in words                                               */
/*----------------------------------------------------------------------------*/
i8 HeadLine::nUnits(void)
{
  i8 nSizeUnits = w8_1B(0);
  
  if (C_W_largehead == nSizeUnits)
    return C_LARGEHEAD_UNITS;
  
  if (nSizeUnits >= 128)
    nSizeUnits -= 128;
  
  return nSizeUnits;
}

/*----------------------------------------------------------------------------*/
/* return size of head in words                                               */
/*----------------------------------------------------------------------------*/
i8 HeadLine::nWords(void)
{
  i8 nSizeWords = nUnits();
  
  return nSizeWords;
}

/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* get address of a byte from logicalRow : head[+over]                        */
/*----------------------------------------------------------------------------*/
byte* HeadLine::ayAdrLogical(i8 i_Offset)
{
  // scenario 1: simple case (no overflow), getting address from head
  if (! bHasOverflow()) {
    byte* ayAdr = (byte*)this + i_Offset;
    return ayAdr;
  }
  
  // scenario 2: there is an overflow, but getting address from head
  i8 nHeadBytes = nBytes();
  if (i_Offset < nHeadBytes - 2) {
    byte* ayAdr = (byte*)this + i_Offset;
    return ayAdr;
  }
  
  // scenario 3: there is an overflow, and getting address from overflow
  OverLine* aOverLine = aGetOver();
  i8 i_Offset_over = i_Offset - nHeadBytes;
  byte* ayAdr = (byte*)aOverLine + i_Offset_over;
  return ayAdr;
}

/*----------------------------------------------------------------------------*/
/* calculate free space of a head line                                        */
/*----------------------------------------------------------------------------*/
i8 HeadLine::nFreeSpace(ColumnVec* pColVec)
{
  //MemLib::vWordDump(aHeadLine,m_nHeadBytes);
  
  Column* pLastCol = pColVec->pGet(pColVec->nItems() - 1);
  ASSERT(pLastCol->bIsLastColumn());
  
  i8 i_BaseOffset = RowCoder::iBaseOffset(pColVec);
  i8 i_Offset = i_BaseOffset;
  i8 i_LastColumn = 0;
  
  eMC eLastEncoding = eGetEncoding(pLastCol,i_BaseOffset,
                                   i_LastColumn,i_Offset);
  
  if (pLastCol->eDatumType() == DM_VCH && MC_SHOCO_SPLIT_2T1C == eLastEncoding)
    return 0;
  
  if (pLastCol->eDatumType() == DM_VCH && MC_SHOCO_FIT_1C == eLastEncoding) {
    i8 nCompressed = wGetLogical(i_Offset,C_1B);
    //RASSERT(nCompressed <= 56);
    i_Offset += nCompressed;
  }
  
  i8 nDataBytes = RowCoder::nDataBytes(eLastEncoding);
  
  i8 nFreeBytes = nBytes() - i_Offset - nDataBytes;
  
  if (nFreeBytes < 0)
    vDebugPoint();
  
  return nFreeBytes;
}

/*----------------------------------------------------------------------------*/
/* get the encoding (and offset) of a column datum                            */
/*----------------------------------------------------------------------------*/
eMC HeadLine::eGetEncoding(Column* pCol, i8 i_BaseOffset,
                           i8& i_PriorColumn_, i8& i_Offset_)
{
  i8 i_ForCol = pCol->iColumnNo();
  
  if (i_ForCol < i_PriorColumn_) {
    i_PriorColumn_ = 0;
    i_Offset_ = i_BaseOffset;
  }
  
  for (i8 i_Col = i_PriorColumn_; i_Col < i_ForCol; i_Col++) {
    
    eMC eEncoding = eMetaCode(i_Col);
    
    if (MC_OVERFLOW_2T == eEncoding) {
      i_Offset_ = nBytes() + C_U_1B;
      eEncoding = (eMC)wGetLogical(i_Offset_++,C_1B);
      i_Offset_ += RowCoder::nDataBytes(eEncoding);
    }
    else {
      i_Offset_ += RowCoder::nDataBytes(eEncoding);
    }
    
    if (MC_STR_FIT_1L == eEncoding) {
      i_Offset_ += wGetLogical((i_Offset_ - 1),C_1B);
    }
    else if (MC_SHOCO_FIT_1C == eEncoding) {
      i_Offset_ += wGetLogical((i_Offset_ - 1),C_1B);
    }
  }
  
  eMC eEncoding = eMetaCode(i_ForCol);
  if (MC_OVERFLOW_2T == eEncoding) {
    i_Offset_ = nBytes() + C_U_1B + _1B_M;
    OverLine* aOverLine = aGetOver();
    eEncoding = (eMC)aOverLine->w8_1B(1);
  }
  i_PriorColumn_ = i_ForCol;
  
  return eEncoding;
}

/*----------------------------------------------------------------------------*/
/* get data code from logical row : head[+over]                               */
/*----------------------------------------------------------------------------*/
w8 HeadLine::wGetLogical(i8 i_Offset, i8 nDataSize)
{
  w8 wDataCode = 0;
  
  // scenario 1: simple case (no overflow), getting bytes from head
  if (! bHasOverflow()) {
    wDataCode = wGet(i_Offset,nDataSize);
    return wDataCode;
  }
  
  // scenario 2: there is an overflow, but getting bytes from head
  i8 nHeadBytes = nBytes();
  if (i_Offset < nHeadBytes - 2) {
    wDataCode = wGet(i_Offset,nDataSize);
    return wDataCode;
  }
  
  // scenario 3: there is an overflow, and getting bytes from overflow
  OverLine* aOverLine = aGetOver();
  i8 i_Offset_over = i_Offset - nHeadBytes;
  wDataCode = aOverLine->wGet(i_Offset_over,nDataSize);
  return wDataCode;
}

/*----------------------------------------------------------------------------*/
/* get overflow line using relative index [OT]                                */
/*----------------------------------------------------------------------------*/
OverLine* HeadLine::aGetOver(void)
{
  i8 i_Over_rel = iOverflow2B();
  OverLine* aOver = RowNub::aGetOver(this,i_Over_rel);
  
  return aOver;
}

/*----------------------------------------------------------------------------*/
/* does this head have an overflow?                                           */
/*----------------------------------------------------------------------------*/
bool HeadLine::bHasOverflow(void)
{
  i8 nSizeWords = iW();
  
  if (nSizeWords >= 128)
    return true;
  else
    return false;
}

/*----------------------------------------------------------------------------*/
/* mask this head to have an overflow                                         */
/*----------------------------------------------------------------------------*/
void HeadLine::vHasOverflow(void)
{
  i8 nSizeWords = iW();
  
  if (nSizeWords < 128) {
    nSizeWords += 128;
    vW(nSizeWords);
  }
}

/*----------------------------------------------------------------------------*/
/* is this a large head line?                                                 */
/*----------------------------------------------------------------------------*/
bool HeadLine::bIsLargeHead(void)
{
  i8 nSizeWords = w8_1B(0);
  
  if (C_LARGEHEAD == nSizeWords)
    return true;
  else
    return false;
}

/*----------------------------------------------------------------------------*/
/* get the relative distance to the overflow line [OT] which is 2B            */
/*----------------------------------------------------------------------------*/
i8 HeadLine::iOverflow2B(void)
{
  i8 nHeadBytes = nBytes();
  
  i8 iOver2B = w8_2B(nHeadBytes - C_2B);

  return iOver2B;
}

/*----------------------------------------------------------------------------*/
/* set the relative distance to the overflow line [OT] which is 2B            */
/*----------------------------------------------------------------------------*/
void HeadLine::vOverflow2B(i8 iOver2B)
{
  i8 nHeadBytes = nBytes();
  
  v2B(nHeadBytes - C_2B,iOver2B);
}

/*----------------------------------------------------------------------------*/
/* put data code into logical row : head[+over]                               */
/*----------------------------------------------------------------------------*/
void HeadLine::vPutLogical(i8 i_Offset, w8 wDataCode, i8 nDataSize)
{
  // scenario 1: simple case (no overflow), putting bytes into head
  if (! bHasOverflow()) {
    vPut(i_Offset,wDataCode,nDataSize);
    return;
  }
  
  // scenario 2: there is an overflow, but putting bytes into head
  i8 nHeadBytes = nBytes();
  if (i_Offset < nHeadBytes - 2) {
    vPut(i_Offset,wDataCode,nDataSize);
    return;
  }
  
  // scenario 3: there is an overflow, and putting bytes into overflow
  
  OverLine* aOverLine = aGetOver();
  i8 i_Offset_over = i_Offset - nHeadBytes;
  aOverLine->vPut(i_Offset_over,wDataCode,nDataSize);
}

/*----------------------------------------------------------------------------*/
/* get the size <W> of this headline in words                                 */
/*----------------------------------------------------------------------------*/
i8 HeadLine::iW(void)
{
  i8 nWords = w8_1B(0);
  
  return nWords;
}

/*----------------------------------------------------------------------------*/
/* set the size <W> of this headline in words                                 */
/*----------------------------------------------------------------------------*/
void HeadLine::vW(i8 nWords)
{
  v1B(0,nWords);
}

/*----------------------------------------------------------------------------*/
/* dump head line                                                             */
/*----------------------------------------------------------------------------*/
void HeadLine::vDumpHead(Stz* szTitle,
                         HeadLine* aHeadLine,
                         ColumnVec* pColVec)
{
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // word dump of headLine (& overLine)
  
  i8 nHeadBytes = aHeadLine->nBytes();
  i8 nHeadWords = aHeadLine->nWords();
  
  OverLine* aOverLine = nullptr;
  i8 nOverBytes = C_UNKNOWN;
  i8 nOverUnits = C_UNKNOWN;
  
  if (aHeadLine->bHasOverflow()) {
    vTrace("\n%s HeadDump %p :\n"_Stz,szTitle,aHeadLine);
    
    MemLib::vWordDump(aHeadLine,nHeadBytes);
    
    aOverLine  = aHeadLine->aGetOver();
    nOverBytes = aOverLine->nBytes();
    nOverUnits = aOverLine->nUnits();
    
    vTrace("\n%s OverLine %p (diff %lldB %lldU):\n"_Stz,
           szTitle,aOverLine,
           nSub(aOverLine,aHeadLine),
           nSub(aOverLine,aHeadLine) / sizeof(SpaceUnit));
    
    MemLib::vWordDump(aOverLine,nOverBytes);
  }
  else if (aHeadLine->bIsLargeHead()) {
    vTrace("\n%s HeadDump %p :\n"_Stz,szTitle,aHeadLine);
    
    w8* awLastWord  = MemLib::awFindNonZeroEnd(aHeadLine,nHeadWords);
    i8 nWords = (awLastWord - (w8*)aHeadLine) + 1;
    i8 nBytes_clipped = nWords * C_BYTES_PER_WORD;
    
    MemLib::vWordDump(aHeadLine,nBytes_clipped);
  }
  else {
    vTrace("\n%s HeadDump %p :\n"_Stz,szTitle,aHeadLine);
    
    MemLib::vWordDump(aHeadLine,nHeadBytes);
  }
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  
  // get no of columns
  i8 nColumns = pColVec->nItems();
  
  i8 nFreeBytes = aHeadLine->nFreeSpace(pColVec);
  if (nFreeBytes < 0)
    nFreeBytes = 0;
  
  if (aHeadLine->bHasOverflow()) {
    i8 i_Over_rel = aHeadLine->iOverflow2B();
    OverLine* aOverLine = RowNub::aGetOver(aHeadLine,i_Over_rel);
    eMC eOverMeta = (eMC)aOverLine->w8_1B(1);
    i8 i_Col_over;
    for (i_Col_over = 0; i_Col_over < nColumns; i_Col_over++)
      if (MC_OVERFLOW_2T == aHeadLine->eMetaCode(i_Col_over))
        break;
    
    
    vTrace("HL: 0x%012llx <W>=%lld%s {M}=%lld [OT]=%04x @ col %lld %s FreeBytes %lld\n"
           "OT: 0x%012llx <U>=%lld <M>=%ll02x %s\n"_Stz,
           aHeadLine,
           aHeadLine->nWords(),
           aHeadLine->bIsLargeHead() ? " LARGEHEAD"_Stz : ""_Stz,
           nColumns,
           aHeadLine->iOverflow2B(),
           i_Col_over,
           pColVec->pGet(i_Col_over)->szColNm(),
           nFreeBytes,
           
           aOverLine,
           nOverUnits,
           eOverMeta,
           aMC[eOverMeta]
           );
  }
  else {
    vTrace("HL: 0x%012llx <W>=%lld%s FreeBytes %lld; "
           "OT: none\n"_Stz,
           aHeadLine,
           aHeadLine->nWords(),
           aHeadLine->bIsLargeHead() ? " LARGEHEAD"_Stz : ""_Stz,
           nFreeBytes);
  }
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  
  HV_Rec hvRec(pColVec);
  DtmVal oDatum;
  
  i8 i_Offset = RowCoder::iBaseOffset(pColVec);
  
  vTrace("Col ColNm         DtmVal MetaCode"
         "           Offset Address      DataBytes           ForthValue\n"_Stz);
  
  
  eDM   eDatum;
  Stz*  szDatum;
  eMC   eMetaCode;
  Stz*  szMetaCode;
  byte* ayAddr;
  i8    nBytes;
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  
  
  // for each column
  for (i8 i_Col = 0; i_Col < nColumns; i_Col++) {
    
    // get the column at index
    Column* pCol = pColVec->pGet(i_Col);
    
    // get the encoded information
    eDatum     = pCol->eDatumType();
    szDatum    = aDM[eDatum];
    eMetaCode  = aHeadLine->eMetaCode(i_Col);
    szMetaCode = aMC[eMetaCode];
    
    if (MC_OVERFLOW_2T == eMetaCode) {
      i8 i_Over_rel = aHeadLine->iOverflow2B();
      OverLine* aOverLine = RowNub::aGetOver(aHeadLine,i_Over_rel);
      eMetaCode  = (eMC)aOverLine->w8_1B(1);
      szMetaCode = aMC[eMetaCode];
      i_Offset   = nHeadBytes + 2;
      ayAddr     = aHeadLine->ayAdrLogical(i_Offset);
      nBytes     = RowCoder::nDataBytes(eMetaCode);
      //vTrace("--- --------------- --- -- ------------------- "
      //       "-- ------------ -- ---------------- -----------\n"_Stz);
      vTrace("*** *************** *** ** ******************* "
             "** ************ ** **************** **********************\n"_Stz);
    }
    else {
      ayAddr = aHeadLine->ayAdrLogical(i_Offset);
      nBytes = RowCoder::nDataBytes(eMetaCode);
    }
    if (MC_STR_FIT_1L == eMetaCode) {
      RASSERT(1 == nBytes);
      nBytes += aHeadLine->wGetLogical(i_Offset,C_1B);
    }
    if (MC_SHOCO_FIT_1C == eMetaCode) {
      RASSERT(1 == nBytes);
      nBytes += aHeadLine->wGetLogical(i_Offset,C_1B);
    }
    
    //      colNo colNm Dtm  MC   aMC   Ofset Addr   Bytes
    vTrace("%3lld %-15s %-3s %02x %-19s %2lld %012llx %2lld "_Stz,
           i_Col,
           pCol->szColNm(),
           szDatum,
           eMetaCode,
           szMetaCode,
           i_Offset,
           ayAddr,
           nBytes);
    
    if (nBytes) {
      if (MC_OVERFLOW_2T == eMetaCode) {
        for (i8 i_Byte = 0; i_Byte < nBytes; i_Byte++)
          vTrace("%02x"_Stz,*(ayAddr + i_Byte));
        //vTrace("\n"_Stz); //! temp
        //break;
      }
      else
        for (i8 i_Byte = 0; i_Byte < nBytes; i_Byte++)
          vTrace("%02x"_Stz,aHeadLine->wGetLogical((i_Offset+i_Byte),C_1B));
    }
    
    for (i8 i_Byte = nBytes; i_Byte < 8; i_Byte++)
      vTrace("__"_Stz);
    vTrace(" "_Stz);
    
    hvRec->vReset();
    
    // first get the encoded value into datum
    RowInflator::vDecode(&oDatum,aHeadLine,pColVec,pCol,i_Col,i_Offset);
    hvRec->vAppend(pCol, &oDatum);
    
    vTrace("%s"_Stz,hvRec->szRecStr());
    
    if (MC_SHOCO_SPLIT_2T1C == eMetaCode) {
      i8 i2T = aHeadLine->wGetLogical(i_Offset,C_2B);
      i8 i1C = aHeadLine->wGetLogical((i_Offset+2),C_1B);
      vTrace(" [ =%lld.%lld ]"_Stz,i2T,i1C);
    }
    else if (MC_SHOCO_FIT_1C == eMetaCode) {
      i8 i1C = aHeadLine->wGetLogical(i_Offset,C_1B);
      vTrace(" [ =%lld ]"_Stz,i1C);
    }
    else if (MC_STR_FIT_1L == eMetaCode) {
      i8 i1C = aHeadLine->wGetLogical(i_Offset,C_1B);
      vTrace(" [ =%lld ]"_Stz,i1C);
    }
    else {
      i8 iDataValue = aHeadLine->wGetLogical(i_Offset,nBytes);
      if (nBytes)
        vTrace(" [ =%lld ]"_Stz,iDataValue);
      else
        vTrace(" [ =nil ]"_Stz);
    }
    
    vTrace("\n"_Stz);
    
    i_Offset += nBytes;
    
    if (eMetaCode == MC_STR_SPLIT_2T1L)
      vDebugPoint();
    if (eMetaCode == MC_STR_DUP_2H)
      vDebugPoint();
    if (eMetaCode == MC_SHOCO_DUP_3D)
      vDebugPoint();
  }
  
  
  //vDumpHead_IV("DumpHeap_IV"_Stz,aHeadLine,pColVec);
}

/*----------------------------------------------------------------------------*/
/* dump head line                                                             */
/*----------------------------------------------------------------------------*/
void HeadLine::vDumpHead_IV(Stz* szTitle,
                                HeadLine* aHeadLine,
                                ColumnVec* pColVec)
{
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // word dump of headLine (& overLine)
  
  i8 nHeadBytes = aHeadLine->nBytes();
  i8 nHeadWords = aHeadLine->nWords();
  
  OverLine* aOverLine = nullptr;
  i8 nOverBytes = C_UNKNOWN;
  i8 nOverUnits = C_UNKNOWN;
  
  if (aHeadLine->bHasOverflow()) {
    vTrace("\n%s HeadDump %p :\n"_Stz,szTitle,aHeadLine);
    
    MemLib::vWordDump(aHeadLine,nHeadBytes);
    
    aOverLine  = aHeadLine->aGetOver();
    nOverBytes = aOverLine->nBytes();
    nOverUnits = aOverLine->nUnits();
    
    vTrace("\n%s OverLine %p (diff %lldB %lldU):\n"_Stz,
           szTitle,aOverLine,
           nSub(aOverLine,aHeadLine),
           nSub(aOverLine,aHeadLine) / sizeof(SpaceUnit));
    
    MemLib::vWordDump(aOverLine,nOverBytes);
  }
  else if (aHeadLine->bIsLargeHead()) {
    vTrace("\n%s HeadDump %p :\n"_Stz,szTitle,aHeadLine);
    
    w8* awLastWord  = MemLib::awFindNonZeroEnd(aHeadLine,nHeadWords);
    i8 nWords = (awLastWord - (w8*)aHeadLine) + 1;
    i8 nBytes_clipped = nWords * C_BYTES_PER_WORD;
    
    MemLib::vWordDump(aHeadLine,nBytes_clipped);
  }
  else {
    vTrace("\n%s HeadDump %p :\n"_Stz,szTitle,aHeadLine);
    
    MemLib::vWordDump(aHeadLine,nHeadBytes);
  }
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  
  // get no of columns
  i8 nColumns = pColVec->nItems();
  
  i8 nFreeBytes = aHeadLine->nFreeSpace(pColVec);
  if (nFreeBytes < 0)
    nFreeBytes = 0;
  
  if (aHeadLine->bHasOverflow()) {
    i8 i_Over_rel = aHeadLine->iOverflow2B();
    OverLine* aOverLine = RowNub::aGetOver(aHeadLine,i_Over_rel);
    eMC eOverMeta = (eMC)aOverLine->w8_1B(1);
    i8 i_Col_over;
    for (i_Col_over = 0; i_Col_over < nColumns; i_Col_over++)
      if (MC_OVERFLOW_2T == aHeadLine->eMetaCode(i_Col_over))
        break;
    
    
    vTrace("HL: 0x%012llx <W>=%lld%s {M}=%lld [OT]=%04x @ col %lld %s FreeBytes %lld\n"
           "OT: 0x%012llx <U>=%lld <M>=%ll02x %s\n"_Stz,
           aHeadLine,
           aHeadLine->nWords(),
           aHeadLine->bIsLargeHead() ? " LARGEHEAD"_Stz : ""_Stz,
           nColumns,
           aHeadLine->iOverflow2B(),
           i_Col_over,
           pColVec->pGet(i_Col_over)->szColNm(),
           nFreeBytes,
           
           aOverLine,
           nOverUnits,
           eOverMeta,
           aMC[eOverMeta]
           );
  }
  else {
    vTrace("HL: 0x%012llx <W>=%lld%s FreeBytes %lld; "
           "OT: none\n"_Stz,
           aHeadLine,
           aHeadLine->nWords(),
           aHeadLine->bIsLargeHead() ? " LARGEHEAD"_Stz : ""_Stz,
           nFreeBytes);
  }
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  
  HV_Rec hvRec(pColVec);
  DtmVal oDatum;
  
  i8 i_Offset = RowCoder::iBaseOffset(pColVec);
  
  vTrace("Col ColNm         DtmVal MetaCode"
         "           Offset Address      DataBytes           ForthValue\n"_Stz);
  
  
  eDM   eDatum;
  Stz*  szDatum;
  eMC   eMetaCode;
  Stz*  szMetaCode;
  byte* ayAddr;
  i8    nBytes;
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  
  Row* pRow = new Row(pColVec);
  
  RowInflator::vInflate(pRow,aHeadLine);
  
  // for each column
  for (i8 i_Col = 0; i_Col < nColumns; i_Col++) {
    
    // get the column
    Column* pCol = pColVec->pGet(i_Col);
    
    // get the encoded information
    eDatum     = pCol->eDatumType();
    szDatum    = aDM[eDatum];
    eMetaCode  = aHeadLine->eMetaCode(i_Col);
    szMetaCode = aMC[eMetaCode];
    
    vTrace("%3lld %-15s %-3s %02x\n"_Stz,
           i_Col,
           pCol->szColNm(),
           szDatum,
           eMetaCode,
           szMetaCode
           );
    
  } // for (i_Col)
  
  delete pRow;
}


/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
