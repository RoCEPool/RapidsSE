/*============================================================================*/
/* DatumToken.cpp -- Datum-Token Helper                                       */
/*============================================================================*/

#include "Datums.h"

/*----------------------------------------------------------------------------*/
/*-- Public Static -----------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* assign datum from token                                                    */
/*----------------------------------------------------------------------------*/
void DatumToken::vAssign(Datum_h* pDatum, Token* pTkn, Column* pCol)
{
  eDM eDatum = pCol->eDatum();
  
  switch (eDatum) {
      
    case DM_BCD: {
      break;
    }
      
    case DM_BOL:{
      break;
    }
      
    case DM_DTE:{
      break;
    }
      
    case DM_FLT:{
      break;
    }
      
    case DM_INT: {
      i8 iInt = pTkn->iToI8();
      pDatum->vInt(iInt);
      break;
    }
      
    case DM_IVL:{
      break;
    }
      
    case DM_TME:{
      break;
    }
      
    case DM_TSP:{
      break;
    }
      
    case DM_VCH:{
      break;
    }
      
    default:
      vThrow(FK_FATAL,VL_FATAL,__func__,__FILE__,__LINE__,
             "DatumToken::vAssign: Invalid eDatum : %lld\n"_Stz,eDatum);
  }
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
