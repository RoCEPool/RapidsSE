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
  v1B((_1B_W + i_Meta),wArg);
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
  i8 i_Over_rel = aHeadLine->iOverflow2B();
  i8 nOverBytes = C_UNKNOWN;
  i8 nOverUnits = C_UNKNOWN;
  
  if (aHeadLine->bHasOverflow()) {
    vTrace("\n%s HeadDump %p :\n"_Stz,szTitle,aHeadLine);
    
    MemLib::vWordDump(aHeadLine,nHeadBytes);
    
    aOverLine  = RowCoder::aGetOverRel(aHeadLine,i_Over_rel);
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
  
  i8 nFreeBytes = RowCoder::nFreeSpaceHead(aHeadLine,pColVec);
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
      ayAddr     = RowCoder::ayAdrLogical(aHeadLine,i_Offset);
      nBytes     = RowCoder::nDataBytes(eMetaCode);
      //vTrace("--- --------------- --- -- ------------------- "
      //       "-- ------------ -- ---------------- -----------\n"_Stz);
      vTrace("*** *************** *** ** ******************* "
             "** ************ ** **************** **********************\n"_Stz);
    }
    else {
      ayAddr = RowCoder::ayAdrLogical(aHeadLine,i_Offset);
      nBytes = RowCoder::nDataBytes(eMetaCode);
    }
    if (MC_STR_FIT_1L == eMetaCode) {
      RASSERT(1 == nBytes);
      nBytes += RowCoder::wGetLogical(aHeadLine,i_Offset,C_1B);
    }
    if (MC_SHOCO_FIT_1C == eMetaCode) {
      RASSERT(1 == nBytes);
      nBytes += RowCoder::wGetLogical(aHeadLine,i_Offset,C_1B);
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
          vTrace("%02x"_Stz,RowCoder::wGetLogical(aHeadLine,(i_Offset+i_Byte),C_1B));
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
      i8 i2T = RowCoder::wGetLogical(aHeadLine,i_Offset,C_2B);
      i8 i1C = RowCoder::wGetLogical(aHeadLine,(i_Offset+2),C_1B);
      vTrace(" [ =%lld.%lld ]"_Stz,i2T,i1C);
    }
    else if (MC_SHOCO_FIT_1C == eMetaCode) {
      i8 i1C = RowCoder::wGetLogical(aHeadLine,i_Offset,C_1B);
      vTrace(" [ =%lld ]"_Stz,i1C);
    }
    else if (MC_STR_FIT_1L == eMetaCode) {
      i8 i1C = RowCoder::wGetLogical(aHeadLine,i_Offset,C_1B);
      vTrace(" [ =%lld ]"_Stz,i1C);
    }
    else {
      i8 iDataValue = RowCoder::wGetLogical(aHeadLine,i_Offset,nBytes);
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
  i8 i_Over_rel = aHeadLine->iOverflow2B();
  i8 nOverBytes = C_UNKNOWN;
  i8 nOverUnits = C_UNKNOWN;
  
  if (aHeadLine->bHasOverflow()) {
    vTrace("\n%s HeadDump %p :\n"_Stz,szTitle,aHeadLine);
    
    MemLib::vWordDump(aHeadLine,nHeadBytes);
    
    aOverLine  = RowCoder::aGetOverRel(aHeadLine,i_Over_rel);
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
  
  i8 nFreeBytes = RowCoder::nFreeSpaceHead(aHeadLine,pColVec);
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
