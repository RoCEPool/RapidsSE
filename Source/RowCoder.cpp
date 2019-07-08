/*============================================================================*/
/* RowCoder.cpp -- Row Coder Mark V                                           */
/*============================================================================*/

#include "Rows.h"


/*----------------------------------------------------------------------------*/
/*-- Static Functions --------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* get auxilliary length length fow this meta code (those MC_XYZ_nL)          */
/*----------------------------------------------------------------------------*/
i8 RowCoder::iAuxLen(eMC eMetaCode)
{
  if (MC_STR_FIT_1L   == eMetaCode) return 1;
  if (MC_STR_FIT_2L  == eMetaCode) return 2;

  return 0;
}

/*----------------------------------------------------------------------------*/
/* calc base offset for column vector                                         */
/*----------------------------------------------------------------------------*/
i8 RowCoder::iBaseOffset(ColumnVec* pColVec)
{
  i8 iOffset  = C_W_1B + nMetaSize(pColVec);
  
  return iOffset;
}

/*----------------------------------------------------------------------------*/
/* how many bytes does given encoding occupy in the data portion of a head?   */
/*----------------------------------------------------------------------------*/
i8 RowCoder::nDataSize(eMC eMetaCode)
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
      return 0;
      
    case MC_FALSE:
    case MC_TRUE:
      return 0;
      
    case MC_DATE_E0_2B:
    case MC_DATE_E1_2B:
    case MC_DATE_E2_2B:
      return 2;
      
      
    case MC_STR_EMPTY:        return 0;
      
    case MC_STR_FIT_1L:       return 1;
    case MC_STR_TAIL_2T:      return 2;
      
    case MC_STR_FIT_2L:      return 2;
      
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
/* does this meta code have an auxilliary length (those MC_XYZ_nL)            */
/*----------------------------------------------------------------------------*/
bool RowCoder::bHasAuxLen(eMC eMetaCode)
{
  if (   MC_STR_FIT_1L   == eMetaCode // <L><Short>
      || MC_STR_FIT_2L  == eMetaCode // <C><Shoco>
      )
    return true;
  
  return false;
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
  i8 nBytesHead = 32; //256;
  
  RASSERT(nBytesHead >= C_HEAD_MIN_BYTES);
  return nBytesHead;
}

/*----------------------------------------------------------------------------*/
/* calc size of meta tape for given column vector (does not incl <W>)         */
/*----------------------------------------------------------------------------*/
i8 RowCoder::nMetaSize(ColumnVec* pColVec)
{
//  i8 nColumns = pColVec->nItems();
//
//  i8 nBits = (nColumns * (i8)C_METABITS);
//
//  i8 nMetaBytes = nBits / (i8)C_BITS_PER_BYTE
//                   + (nBits % (i8)C_BITS_PER_BYTE ? 1 : 0);

  i8 nMetaBytes = pColVec->nItems();

  return nMetaBytes;
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
