/*============================================================================*/
/* DatumRandom.cpp -- Datum-Random Helper                                      */
/*============================================================================*/

#include "Datums.h"

/*----------------------------------------------------------------------------*/
/*-- Public Static -----------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* generate random datum value using column definition                        */
/*----------------------------------------------------------------------------*/
void DatumRandom::vRndDtm(Datum_h* pDatum, Column* pCol)
{
  eDM eDatum = pCol->eDatum();
  
  switch (eDatum) {
    case DM_BCD: vRndBcd(pDatum,pCol->pMinVal(),pCol->pMaxVal()); break;
    case DM_BOL: vRndBol(pDatum,pCol->pMinVal(),pCol->pMaxVal()); break;
    case DM_DTE: vRndDte(pDatum,pCol->pMinVal(),pCol->pMaxVal()); break;
    case DM_FLT: vRndFlt(pDatum,pCol->pMinVal(),pCol->pMaxVal()); break;
    case DM_INT: vRndInt(pDatum,pCol->pMinVal(),pCol->pMaxVal()); break;
    case DM_IVL: vRndIvl(pDatum,pCol->pMinVal(),pCol->pMaxVal()); break;
    case DM_TME: vRndTme(pDatum,pCol->pMinVal(),pCol->pMaxVal()); break;
    case DM_TSP: vRndTsp(pDatum,pCol->pMinVal(),pCol->pMaxVal()); break;
    case DM_VCH: vRndVch(pDatum,pCol->pMinVal(),pCol->pMaxVal()); break;
    default:
      vThrow(FK_FATAL,VL_FATAL,__func__,__FILE__,__LINE__,
             "DatumRandom::vRnd: Invalid eDatum : %lld\n"_Stz,eDatum);
  }
  
}

/*----------------------------------------------------------------------------*/
/* generate random bcd datum                                                  */
/*----------------------------------------------------------------------------*/
void DatumRandom::vRndBcd(Datum_h* pDatum, Datum_h* pMin, Datum_h* pMax)
{
}

/*----------------------------------------------------------------------------*/
/* generate random bol datum                                                  */
/*----------------------------------------------------------------------------*/
void DatumRandom::vRndBol(Datum_h* pDatum, Datum_h* pMin, Datum_h* pMax)
{
}

/*----------------------------------------------------------------------------*/
/* generate random dte datum                                                  */
/*----------------------------------------------------------------------------*/
void DatumRandom::vRndDte(Datum_h* pDatum, Datum_h* pMin, Datum_h* pMax)
{
}

/*----------------------------------------------------------------------------*/
/* generate random Flt datum                                                  */
/*----------------------------------------------------------------------------*/
void DatumRandom::vRndFlt(Datum_h* pDatum, Datum_h* pMin, Datum_h* pMax)
{
}

/*----------------------------------------------------------------------------*/
/* generate random Int datum                                                  */
/*----------------------------------------------------------------------------*/
void DatumRandom::vRndInt(Datum_h* pDatum, Datum_h* pMin, Datum_h* pMax)
{
}

/*----------------------------------------------------------------------------*/
/* generate random Ivl datum                                                  */
/*----------------------------------------------------------------------------*/
void DatumRandom::vRndIvl(Datum_h* pDatum, Datum_h* pMin, Datum_h* pMax)
{
}

/*----------------------------------------------------------------------------*/
/* generate random tme datum                                                  */
/*----------------------------------------------------------------------------*/
void DatumRandom::vRndTme(Datum_h* pDatum, Datum_h* pMin, Datum_h* pMax)
{
}

/*----------------------------------------------------------------------------*/
/* generate random tsp datum                                                  */
/*----------------------------------------------------------------------------*/
void DatumRandom::vRndTsp(Datum_h* pDatum, Datum_h* pMin, Datum_h* pMax)
{
}

/*----------------------------------------------------------------------------*/
/* generate random vch datum                                                  */
/*----------------------------------------------------------------------------*/
void DatumRandom::vRndVch(Datum_h* pDatum, Datum_h* pMin, Datum_h* pMax)
{
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
