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
  i8 iOffset  = nMetaSize(pColVec);
  
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
/* convert a Edt into an Dte (in situ)                                        */
/*----------------------------------------------------------------------------*/
void RowCoder::vDteFromEdt(Dtm* aDte, i8 iEraBaseYear)
{
  i8 iYearInEra = aDte->xEdt.m_uYearInEra;
  i8 iMonth     = aDte->xEdt.m_uMonth;
  i8 iDay       = aDte->xEdt.m_uDay;
  
  aDte->m_iDte = 0;
  
  aDte->xDte.m_uYearDelta = iYearInEra + iEraBaseYear;
  aDte->xDte.m_uMonth     = iMonth;
  aDte->xDte.m_uDay       = iDay;
}

/*----------------------------------------------------------------------------*/
/* convert a Dte into an Edt                                                  */
/*----------------------------------------------------------------------------*/
void RowCoder::vDteIntoEdt(Dtm wDte,
                           Dtm& wEdt_, i8& iEra_)
{
  i8 iYearDelta = wDte.xDte.m_uYearDelta;
  i8 iMonth     = wDte.xDte.m_uMonth;
  i8 iDay       = wDte.xDte.m_uDay;
  
  i8 iYearInEra;
  
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
/* get overflow line using relative index [OT]                                */
/*----------------------------------------------------------------------------*/
OverLine* RowCoder::aGetOverRel(HeadLine* aHeadLine, i8 i_Over_rel)
{
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
/* calc size of the head line for column vector                               */
/*----------------------------------------------------------------------------*/
i8 RowCoder::nHeadSize(ColumnVec* pColVec)
{
  // experiment 1
  //i8 nBytes = sizeof(CacheLine) + (1 * C_BYTES_PER_WORD);
  //i8 nHeadWords = MemLib::nQuantaRequired(nBytes, sizeof(Word));
  
  // experiment 2
  //i8 nBytes = pColVec->nItems() * 4;
  //i8 nHeadBytes = MemLib::nQuantaRequired(nBytes, sizeof(Word));
  
  // experiment 3
  //i8 nBytes = pColVec->nItems() * 5;
  //i8 nHeadBytes = MemLib::nQuantaRequired(nBytes, sizeof(Word));
  
  // experiment 4
  //i8 nBytes = pColVec->nItems() * 6;
  //i8 nHeadBytes = MemLib::nQuantaRequired(nBytes, sizeof(Word));
  
  // seth
  //i8 nBytes = pColVec->nItems() * 10;
  //i8 nHeadBytes = MemLib::nQuantaRequired(nBytes, sizeof(Word));
  
  //nBytes     = 24;
  //nHeadWords = MemLib::nQuantaRequired(nBytes, sizeof(Word));
  
  //i8 nBytesHead = nHeadWords * (i8)C_BYTES_PER_WORD;
  i8 nBytesHead = 256;
  RASSERT(nBytesHead >= C_HEAD_MIN_BYTES);
  
  return nBytesHead;
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
i8 RowCoder::nMetaSize(ColumnVec* pColVec)
{
  i8 nColumns = pColVec->nItems();
  
  i8 nBits = (i8)C_BITS_PER_BYTE + (nColumns * (i8)C_METABITS);
  
  i8 nMetaBytes = nBits / (i8)C_BITS_PER_BYTE
  + (nBits % (i8)C_BITS_PER_BYTE ? 1 : 0);
  
  return nMetaBytes;
}

/*----------------------------------------------------------------------------*/
/* get address of given shoco column from head line                           */
/*----------------------------------------------------------------------------*/
byte* RowCoder::ayShoco(HeadLine* aHeadLine, i8 i_BaseOffset,
                        ColumnVec* pColVec, Column* pCol) //! pColVec not needed
{
  i8 i_LastOffset = i_BaseOffset;
  i8 i_LastColumn = 0;
  
  eMC eEncoding = aHeadLine->eGetEncoding(pCol,i_BaseOffset,
                                          i_LastColumn,i_LastOffset);
  
  if (MC_SHOCO_FIT_1C != eEncoding)
    return nullptr;
  
  // get address of <C><shoco...>
  byte* ayAddress = aHeadLine->ayAdrLogical(i_LastOffset);
  
  return ayAddress;
}
/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
