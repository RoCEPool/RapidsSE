/*============================================================================*/
/* RowCoder.cpp -- Row Coder Mark IV                                          */
/*============================================================================*/

#include "Data.h"


/*----------------------------------------------------------------------------*/
/*-- Static Functions --------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* calc base offset for column vector                                         */
/*----------------------------------------------------------------------------*/
i8 RowCoder::iBaseOffset(ColumnVec* pColVec)
{
  i8 nColumns = pColVec->nItems();
  
  i8 iOffset  = nMetaData(pColVec);
  
  //! temporary                                                          FIX
  if (iOffset < 8)
    iOffset = 8;
  
  return iOffset;
}
/*----------------------------------------------------------------------------*/
/* how many bytes does given encoding occupy in the data portion of a head?   */
/*----------------------------------------------------------------------------*/
i8 RowCoder::nDataBytes(eMC eMetaCode)
{
  switch (eMetaCode) {
      
    case MC_NULL:
    case MC_1B:
    case MC_2B:
    case MC_3B:
    case MC_4B:
    case MC_5B:
    case MC_6B:
    case MC_7B:
    case MC_8B:
      return eMetaCode;
      
    case MC_MINUS_ONE:
    case MC_ZERO:
    case MC_ONE:
    case MC_TWO:
    case MC_THREE:
    case MC_FOUR:
    case MC_FIVE:
    case MC_SIX:
    case MC_SEVEN:
    case MC_EIGHT:
    case MC_NINE:
    case MC_TEN:
    case MC_SAME:
      return 0;
      
    case MC_ADD_1B:  return 1;
    case MC_ADD_2B:  return 2;
    case MC_SUB_1B:  return 1;
    case MC_SUB_2B:  return 2;
      
    case MC_FALSE:
    case MC_TRUE:
      return 0;
      
    case MC_DATE_E0_2B:
    case MC_DATE_E1_2B:
    case MC_DATE_E2_2B:
      return 2;
      
    case MC_DAYS_ADD_1B:
    case MC_DAYS_SUB_1B:
      return 1;
      
    case MC_STR_EMPTY:        return 0;
      
    case MC_STR_DUP_2H:       return 2;
    case MC_STR_FIT_1L:       return 1;
    case MC_STR_TAIL_2T:      return 2;
    case MC_STR_SPLIT_2T1L:   return 3;
      
    case MC_BSTR_FIT_2L:      return 2;
    case MC_BSTR_TAIL_2T:     return 2;
    case MC_BSTR_SPLIT_2T2L:  return 4;
      
    case MC_SHOCO_DUP_3D:     return 3;
    case MC_SHOCO_FIT_1C:     return 1;
    case MC_SHOCO_TAIL_2T:    return 2;
    case MC_SHOCO_SPLIT_2T1C: return 3;
      
    case MC_TWOS_1B: return 1;
    case MC_TWOS_2B: return 2;
    case MC_TWOS_3B: return 3;
    case MC_TWOS_4B: return 4;
    case MC_TWOS_5B: return 5;
    case MC_TWOS_6B: return 6;
    case MC_TWOS_7B: return 7;
      
    case MC_OVERFLOW_2T:
      return 2;
      
    default:
      NEVER_REACH;
  }
  
}

/*----------------------------------------------------------------------------*/
/* compute an Edt into a Dte                                                  */
/*----------------------------------------------------------------------------*/
void RowCoder::vDteFromEdt(Datum* pDatum, i8 iEraBaseYear)
{
  RowCoder::vDteFromEdt((Word*)&pDatum->m_wWord,iEraBaseYear);
}

/*----------------------------------------------------------------------------*/
/* convert a Edt into an Dte                                                  */
/*----------------------------------------------------------------------------*/
void RowCoder::vDteFromEdt(Word* aWord, i8 iEraBaseYear)
{
  i8 iYearInEra;
  i8 iMonth;
  i8 iDay;
  
  iYearInEra = aWord->xEdt.m_uYearInEra;
  iMonth     = aWord->xEdt.m_uMonth;
  iDay       = aWord->xEdt.m_uDay;
  
  aWord->m_iDte = 0;
  aWord->xDte.m_uYearDelta = iYearInEra + iEraBaseYear;
  aWord->xDte.m_uMonth     = iMonth;
  aWord->xDte.m_uDay       = iDay;
}

/*----------------------------------------------------------------------------*/
/* convert a Dte into an Edt                                                  */
/*----------------------------------------------------------------------------*/
void RowCoder::vDteIntoEdt(Dtm wDte,
                           Dtm& wEdt_, i8& iEra_)
{
  i8 iYearDelta;
  i8 iYearInEra;
  i8 iMonth;
  i8 iDay;
  
  iYearDelta = wDte.xDte.m_uYearDelta;
  iMonth     = wDte.xDte.m_uMonth;
  iDay       = wDte.xDte.m_uDay;
  
  if (iYearDelta < C_ERA_1) {
    iEra_ = C_ERA_0;
    iYearInEra = iYearDelta - C_ERA_0;
  }
  else if (iYearDelta < C_ERA_2) {
    iEra_ = C_ERA_1;
    iYearInEra = iYearDelta - C_ERA_1;
  }
  else {
    iEra_ = C_ERA_2;
    iYearInEra = iYearDelta - C_ERA_2;
  }
  
  wEdt_.m_iEdt = 0;
  wEdt_.xEdt.m_uYearInEra = iYearInEra;
  wEdt_.xEdt.m_uMonth     = iMonth;
  wEdt_.xEdt.m_uDay       = iDay;
}

/*----------------------------------------------------------------------------*/
/* get address of a byte from logicalRow : head[+over]                        */
/*----------------------------------------------------------------------------*/
byte* RowCoder::ayAdrLogical(HeadLine* aLogicalRow, i8 i_Offset)
{
  HeadLine* aHeadLine = aLogicalRow;
  
  // scenario 1: simple case (no overflow), getting address from head
  if (! aHeadLine->bHasOverflow()) {
    byte* ayAdr = (byte*)aHeadLine + i_Offset;
    return ayAdr;
  }
  
  // scenario 2: there is an overflow, but getting address from head
  i8 nHeadBytes = aLogicalRow->nBytes();
  if (i_Offset < nHeadBytes - 2) {
    byte* ayAdr = (byte*)aHeadLine + i_Offset;
    return ayAdr;
  }
  
  // scenario 3: there is an overflow, and getting address from overflow
  OverLine* aOverLine = aGetOver(aHeadLine);
  i8 i_Offset_over = i_Offset - nHeadBytes;
  byte* ayAdr = (byte*)aOverLine + i_Offset_over;
  return ayAdr;
}

/*----------------------------------------------------------------------------*/
/* get some bytes from logicalRow : head[+over]                               */
/*----------------------------------------------------------------------------*/
w8 RowCoder::wGetLogical(HeadLine* aLogicalRow, i8 i_Offset, i8 nBytes)
{
  HeadLine* aHeadLine = aLogicalRow;
  w8 wSomeBytes = 0;
  
  // scenario 1: simple case (no overflow), getting bytes from head
  if (! aHeadLine->bHasOverflow()) {
    wSomeBytes = aHeadLine->wGet(i_Offset,nBytes);
    return wSomeBytes;
  }
  
  // scenario 2: there is an overflow, but getting bytes from head
  i8 nHeadBytes = aLogicalRow->nBytes();
  if (i_Offset < nHeadBytes - 2) {
    wSomeBytes = aHeadLine->wGet(i_Offset,nBytes);
    return wSomeBytes;
  }
  
  // scenario 3: there is an overflow, and getting bytes from overflow
  OverLine* aOverLine = aGetOver(aHeadLine);
  i8 i_Offset_over = i_Offset - nHeadBytes;
  wSomeBytes = aOverLine->wGet(i_Offset_over,nBytes);
  return wSomeBytes;
}

/*----------------------------------------------------------------------------*/
/* put some bytes into logicalRow : head[+over]                               */
/*----------------------------------------------------------------------------*/
void RowCoder::vPutLogical(HeadLine* aLogicalRow, i8 i_Offset, i8 nBytes, w8 wBytes)
{
  HeadLine* aHeadLine = aLogicalRow;
  
  // scenario 1: simple case (no overflow), putting bytes into head
  if (! aHeadLine->bHasOverflow()) {
    aHeadLine->vPut(i_Offset,wBytes,nBytes);
    return;
  }
  
  // scenario 2: there is an overflow, but putting bytes into head
  i8 nHeadBytes = aHeadLine->nBytes();
  if (i_Offset < nHeadBytes - 2) {
    aHeadLine->vPut(i_Offset,wBytes,nBytes);
    return;
  }
  
  // scenario 3: there is an overflow, and putting bytes into overflow
  
  OverLine* aOverLine = aGetOver(aHeadLine);
  i8 i_Offset_over = i_Offset - nHeadBytes;
  aOverLine->vPut(i_Offset_over,wBytes,nBytes);
}

/*----------------------------------------------------------------------------*/
/* get the encoding (and offset) of a column datum                            */
/*----------------------------------------------------------------------------*/
eMC RowCoder::eGetEncoding(HeadLine* aHeadLine, Column* pCol,
                           ColumnVec* pColVec,
                           i8& i_PriorColumn_, i8& i_Offset_)
{
  i8 i_ForCol = pCol->iColumnNo();
  
  if (i_ForCol < i_PriorColumn_) {
    i_PriorColumn_ = 0;
    i_Offset_ = iBaseOffset(pColVec);
  }
  
  for (i8 i_Col = i_PriorColumn_; i_Col < i_ForCol; i_Col++) {
    
    eMC eEncoding = aHeadLine->eMetaCode(i_Col);
    
    if (MC_OVERFLOW_2T == eEncoding) {
      i_Offset_ = aHeadLine->nBytes() + _1B_U;
      eEncoding = (eMC)wGetLogical(aHeadLine,i_Offset_++,C_1B);
      i_Offset_ += nDataBytes(eEncoding);
    }
    else {
      i_Offset_ += nDataBytes(eEncoding);
    }
    
    if (MC_STR_FIT_1L == eEncoding) {
      i_Offset_ += wGetLogical(aHeadLine,(i_Offset_ - 1),C_1B);
    }
    else if (MC_SHOCO_FIT_1C == eEncoding) {
      i_Offset_ += wGetLogical(aHeadLine,(i_Offset_ - 1),C_1B);
    }
  }
  
  eMC eEncoding = aHeadLine->eMetaCode(i_ForCol);
  if (MC_OVERFLOW_2T == eEncoding) {
    i_Offset_ = aHeadLine->nBytes() + _1B_U + _1B_M;
    OverLine* aOverLine = aGetOver(aHeadLine);
    eEncoding = (eMC)aOverLine->w8_1B(1);
  }
  i_PriorColumn_ = i_ForCol;
  
  return eEncoding;
}

/*----------------------------------------------------------------------------*/
/* get overflow line using relative index [OT]                                */
/*----------------------------------------------------------------------------*/
OverLine* RowCoder::aGetOver(HeadLine* aHeadLine)
{
  i8 i_Over_rel = aHeadLine->iOverflow2B();
  OverLine* aOver = RowNub::aGetOver(aHeadLine,i_Over_rel);
  
  return aOver;
}

/*----------------------------------------------------------------------------*/
/* get tail line using relative index [FT]                                    */
/*----------------------------------------------------------------------------*/
TailLine* RowCoder::aGetTailRel(HeadLine* aHeadLine, i8 i_Tail_rel)
{
  TailLine* aTail = RowNub::aGetTail(aHeadLine,i_Tail_rel);
  
  return aTail;
}

/*----------------------------------------------------------------------------*/
/* is meta code a MC_Xyz_FIT_ ?                                               */
/*----------------------------------------------------------------------------*/
bool RowCoder::bIsFit(eMC eMetaCode)
{
  if ( MC_STR_FIT_1L   == eMetaCode ||
       MC_BSTR_FIT_2L  == eMetaCode ||
       MC_SHOCO_FIT_1C == eMetaCode   )
    return true;
  
  return false;
}

/*----------------------------------------------------------------------------*/
/* calc size of the meta tape needed for given column vector                  */
/*----------------------------------------------------------------------------*/
i8 RowCoder::nMetaData(ColumnVec* pColVec)
{
  i8 nColumns = pColVec->nItems();
  
  i8 nBits = (i8)C_BITS_PER_BYTE + (nColumns * (i8)C_METABITS);
  
  i8 nMetaBytes = nBits / (i8)C_BITS_PER_BYTE
  + (nBits % (i8)C_BITS_PER_BYTE ? 1 : 0);
  
  return nMetaBytes;
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
