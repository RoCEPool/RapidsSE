/*============================================================================*/
/* Crw.hpp -- Compact Row                                                    */
/*============================================================================*/

#include "CompactBuffers.h"

/*----------------------------------------------------------------------------*/
/*-- LifeCycle ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
Crw::Crw(void)
{
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
Crw::~Crw(void) noexcept(false)
{
}

/*----------------------------------------------------------------------------*/
/* dump compact row                                                           */
/*----------------------------------------------------------------------------*/
void Crw::vDumpCrw(Stz* szTitle, HeadLine* aHeadLine, StreamDef* pStreamDef)
{

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // word dump of headLine (& overLine)
  
  i8 nHeadBytes = aHeadLine->nBytes();
  
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
           nDifference(aOverLine,aHeadLine),
           nDifference(aOverLine,aHeadLine) / sizeof(SpaceUnit));
    
    MemLib::vWordDump(aOverLine,nOverBytes);
  }
  else {
    vTrace("\n%s HeadDump %p :\n"_Stz,szTitle,aHeadLine);
    
    MemLib::vWordDump(aHeadLine,nHeadBytes);
  }
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  
  // get no of columns
  i8 nColumns = pStreamDef->nColumns(C_ALL);
  
  i8 nFreeBytes = aHeadLine->nFreeSpace(pStreamDef);
  if (nFreeBytes < 0)
    nFreeBytes = 0;
  
  if (aHeadLine->bHasOverflow()) {
    i8 i_Over_rel = aHeadLine->iOverflow2B();
    OverLine* aOverLine = RowNub::aGetOver_rel(aHeadLine,i_Over_rel);
    eMC eOverMeta = (eMC)aOverLine->w8_1B(1);
    i8 i_Col_over;
    for (i_Col_over = 0; i_Col_over < nColumns; i_Col_over++)
      if (MC_OVERFLOW_2T == aHeadLine->eMetaCode(i_Col_over))
        break;
    
    
    vTrace("HL: 0x%012llx <W>=%lld {M}=%lld [OT]=%04x @ col %lld %s FreeBytes %lld\n"
           "OT: 0x%012llx <U>=%lld <M>=%ll02x %s\n"_Stz,
           aHeadLine,
           aHeadLine->nWords(),
           nColumns,
           aHeadLine->iOverflow2B(),
           i_Col_over,
           pStreamDef->pGetColumn(i_Col_over)->szColNm(),
           nFreeBytes,
           
           aOverLine,
           nOverUnits,
           eOverMeta,
           aMC[eOverMeta]
           );
  }
  else {
    vTrace("HL: 0x%012llx <W>=%lld FreeBytes %lld; "
           "OT: none\n"_Stz,
           aHeadLine,
           aHeadLine->nWords(),
           nFreeBytes);
  }
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  
  HV_Rec hvRec(pStreamDef, C_ALL);
  Datum oDatum;
  
  i8 i_Offset = C_UNKNOWN;

  vTrace("Col ColNm         Datum MetaCode"
         "           Offset Address      DataBytes           ColVal\n"_Stz);
  
  
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
    Column* pCol = pStreamDef->pGetColumn(i_Col);
    
    // get the encoded information
    eDatum     = pCol->eDatum();
    szDatum    = aDM[eDatum];
    eMetaCode  = aHeadLine->eMetaCode(i_Col);
    szMetaCode = aMC[eMetaCode];
    
    if (MC_OVERFLOW_2T == eMetaCode) {
      i8 i_Over_rel = aHeadLine->iOverflow2B();
      OverLine* aOverLine = RowNub::aGetOver_rel(aHeadLine,i_Over_rel);
      eMetaCode  = (eMC)aOverLine->w8_1B(1);
      szMetaCode = aMC[eMetaCode];
      i_Offset   = nHeadBytes + 2;
      ayAddr     = aHeadLine->ayAdrLogical(i_Offset);
      nBytes     = RowCoder::nDataSizeOfCode(eMetaCode);
      //vTrace("--- --------------- --- -- ------------------- "
      //       "-- ------------ -- ---------------- -----------\n"_Stz);
      vTrace("*** *************** *** ** ******************* "
             "** ************ ** **************** **********************\n"_Stz);
    }
    else {
      ayAddr = aHeadLine->ayAdrLogical(i_Offset);
      nBytes = RowCoder::nDataSizeOfCode(eMetaCode);
    }
    if (MC_STR_FIT_1L == eMetaCode) {
      RASSERT(1 == nBytes);
      nBytes += aHeadLine->wGetLogical(i_Offset,C_1B);
    }
    if (MC_STR_FIT_2L == eMetaCode) {
      RASSERT(2 == nBytes);
      nBytes += aHeadLine->wGetLogical(i_Offset,C_2B);
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
    ColDecoder::vDecode(&oDatum,aHeadLine,pStreamDef,pCol,i_Col,i_Offset);
    hvRec->vAppendDtm(pCol, &oDatum);
    
    vTrace("%s"_Stz,hvRec->szRecStr());
    
    if (MC_STR_FIT_1L == eMetaCode) {
      i8 i1L = aHeadLine->wGetLogical(i_Offset,C_1B);
      vTrace(" [ =%lld ]"_Stz,i1L);
    }
    else
    if (MC_STR_FIT_2L == eMetaCode) {
      i8 i2L = aHeadLine->wGetLogical(i_Offset,C_2B);
      vTrace(" [ =%lld ]"_Stz,i2L);
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
  }
  
  //vDumpHead_IV("DumpHeap_IV"_Stz,aHeadLine,pColVec);
}

/*----------------------------------------------------------------------------*/
/* dump head line                                                             */
/*----------------------------------------------------------------------------*/
void Crw::vDumpCrw_IV(Stz* szTitle, HeadLine* aHeadLine, StreamDef* pStreamDef)
{
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // word dump of headLine (& overLine)
  
  i8 nHeadBytes = aHeadLine->nBytes();
  
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
           nDifference(aOverLine,aHeadLine),
           nDifference(aOverLine,aHeadLine) / sizeof(SpaceUnit));
    
    MemLib::vWordDump(aOverLine,nOverBytes);
  }
  else {
    vTrace("\n%s HeadDump %p :\n"_Stz,szTitle,aHeadLine);
    
    MemLib::vWordDump(aHeadLine,nHeadBytes);
  }
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  
  // get no of columns
  i8 nColumns = pStreamDef->nColumns(C_ALL);
  
  i8 nFreeBytes = aHeadLine->nFreeSpace(pStreamDef);
  if (nFreeBytes < 0)
    nFreeBytes = 0;
  
  if (aHeadLine->bHasOverflow()) {
    i8 i_Over_rel = aHeadLine->iOverflow2B();
    OverLine* aOverLine = RowNub::aGetOver_rel(aHeadLine,i_Over_rel);
    eMC eOverMeta = (eMC)aOverLine->w8_1B(1);
    i8 i_Col_over;
    for (i_Col_over = 0; i_Col_over < nColumns; i_Col_over++)
      if (MC_OVERFLOW_2T == aHeadLine->eMetaCode(i_Col_over))
        break;
    
    
    vTrace("HL: 0x%012llx <W>=%lld {M}=%lld [OT]=%04x @ col %lld %s FreeBytes %lld\n"
           "OT: 0x%012llx <U>=%lld <M>=%ll02x %s\n"_Stz,
           aHeadLine,
           aHeadLine->nWords(),
           nColumns,
           aHeadLine->iOverflow2B(),
           i_Col_over,
           pStreamDef->pGetColumn(i_Col_over)->szColNm(),
           nFreeBytes,
           
           aOverLine,
           nOverUnits,
           eOverMeta,
           aMC[eOverMeta]
           );
  }
  else {
    vTrace("HL: 0x%012llx <W>=%lld FreeBytes %lld; "
           "OT: none\n"_Stz,
           aHeadLine,
           aHeadLine->nWords(),
           nFreeBytes);
  }
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  
  HV_Rec hvRec(pStreamDef, C_ALL);
  Datum oDatum;
  
  i8 nCols = pStreamDef->nColumns(C_ALL);
  
  vTrace("Col ColNm         Datum MetaCode"
         "           Offset Address      DataBytes           ColVal\n"_Stz);
  
  
  eDM   eDatum;
  Stz*  szDatum;
  eMC   eMetaCode;
  Stz*  szMetaCode;
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  
  HV_Row hRow(pStreamDef, C_ALL);
  RowInflator::vInflateRow(hRow,aHeadLine);
  
  // for each column
  for (i8 i_Col = 0; i_Col < nColumns; i_Col++) {
    
    // get the column
    Column* pCol = pStreamDef->pGetColumn(i_Col);
    
    // get the encoded information
    eDatum     = pCol->eDatum();
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
  
  //delete pRow;
}



/*----------------------------------------------------------------------------*/
/*-- Public ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
