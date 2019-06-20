/*============================================================================*/
/* RowCoder.cpp -- Row Coder Mark IV                                          */
/*============================================================================*/

#include "Data.h"


/*----------------------------------------------------------------------------*/
/*-- Static Functions --------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

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
/* get tail line using relative index [FT]                                    */
/*----------------------------------------------------------------------------*/
TailLine* RowCoder::aGetTailRel(HeadLine* aHeadLine, i8 i_Tail_rel)
{
  TailLine* aTail = RowNub::aGetTail(aHeadLine,i_Tail_rel);
  
  return aTail;
}


/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
