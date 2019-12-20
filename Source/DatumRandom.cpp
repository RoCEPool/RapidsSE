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
    case DM_BCD: vRndBcd(pDatum,pCol); break;
    case DM_BOL: vRndBol(pDatum,pCol); break;
    case DM_DTE: vRndDte(pDatum,pCol); break;
    case DM_FLT: vRndFlt(pDatum,pCol); break;
    case DM_INT: vRndInt(pDatum,pCol); break;
    case DM_IVL: vRndIvl(pDatum,pCol); break;
    case DM_TME: vRndTme(pDatum,pCol); break;
    case DM_TSP: vRndTsp(pDatum,pCol); break;
    case DM_VCH: vRndVch(pDatum,pCol); break;
    default:
      vThrow(FK_FATAL,VL_FATAL,__func__,__FILE__,__LINE__,
             "DatumRandom::vRnd: Invalid eDatum : %lld\n"_Stz,eDatum);
  }
}

/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* generate random bcd datum                                                  */
/*----------------------------------------------------------------------------*/
void DatumRandom::vRndBcd(Datum_h* pDatum, Column* pCol)
{
  Datum_h* pMin = pCol->pMinVal();
  Datum_h* pMax = pCol->pMaxVal();
  
  vRndBcd(pDatum,pMin,pMax);
}

/*----------------------------------------------------------------------------*/
/* generate random bol datum                                                  */
/*----------------------------------------------------------------------------*/
void DatumRandom::vRndBol(Datum_h* pDatum, Column* pCol)
{
  Datum_h* pMin = pCol->pMinVal();
  Datum_h* pMax = pCol->pMaxVal();
  
  vRndBol(pDatum,pMin,pMax);
}

/*----------------------------------------------------------------------------*/
/* generate random dte datum                                                  */
/*----------------------------------------------------------------------------*/
void DatumRandom::vRndDte(Datum_h* pDatum, Column* pCol)
{
  Datum_h* pMin = pCol->pMinVal();
  Datum_h* pMax = pCol->pMaxVal();
  
  vRndDte(pDatum,pMin,pMax);
}

/*----------------------------------------------------------------------------*/
/* generate random flt datum                                                  */
/*----------------------------------------------------------------------------*/
void DatumRandom::vRndFlt(Datum_h* pDatum, Column* pCol)
{
  Datum_h* pMin = pCol->pMinVal();
  Datum_h* pMax = pCol->pMaxVal();
  
  vRndFlt(pDatum,pMin,pMax);
}

/*----------------------------------------------------------------------------*/
/* generate random int datum                                                  */
/*----------------------------------------------------------------------------*/
void DatumRandom::vRndInt(Datum_h* pDatum, Column* pCol)
{
  Datum_h* pMin = pCol->pMinVal();
  Datum_h* pMax = pCol->pMaxVal();

  vRndInt(pDatum,pMin,pMax);
}

/*----------------------------------------------------------------------------*/
/* generate random ivl datum                                                  */
/*----------------------------------------------------------------------------*/
void DatumRandom::vRndIvl(Datum_h* pDatum, Column* pCol)
{
  Datum_h* pMin = pCol->pMinVal();
  Datum_h* pMax = pCol->pMaxVal();
  
  vRndIvl(pDatum,pMin,pMax);
}

/*----------------------------------------------------------------------------*/
/* generate random tme datum                                                  */
/*----------------------------------------------------------------------------*/
void DatumRandom::vRndTme(Datum_h* pDatum, Column* pCol)
{
  Datum_h* pMin = pCol->pMinVal();
  Datum_h* pMax = pCol->pMaxVal();
  
  vRndTme(pDatum,pMin,pMax);
}

/*----------------------------------------------------------------------------*/
/* generate random tsp datum                                                  */
/*----------------------------------------------------------------------------*/
void DatumRandom::vRndTsp(Datum_h* pDatum, Column* pCol)
{
  Datum_h* pMin = pCol->pMinVal();
  Datum_h* pMax = pCol->pMaxVal();
  
  vRndTsp(pDatum,pMin,pMax);
}

/*----------------------------------------------------------------------------*/
/* generate random vch datum                                                  */
/*----------------------------------------------------------------------------*/
void DatumRandom::vRndVch(Datum_h* pDatum, Column* pCol)
{
  Datum_h* pMin = pCol->pMinVal();
  Datum_h* pMax = pCol->pMaxVal();
  
 vRndVch(pDatum,pMin,pMax);
}

/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* generate random bcd datum                                                  */
/*----------------------------------------------------------------------------*/
void DatumRandom::vRndBcd(Datum_h* pDatum, Datum_h* pMin, Datum_h* pMax)
{
  RN_Bcd rnBcd(pDatum,pMin,pMax);
}

/*----------------------------------------------------------------------------*/
/* generate random bol datum                                                  */
/*----------------------------------------------------------------------------*/
void DatumRandom::vRndBol(Datum_h* pDatum, Datum_h* pMin, Datum_h* pMax)
{
  RN_Bol rnBol(pDatum,pMin,pMax);
}

/*----------------------------------------------------------------------------*/
/* generate random dte datum                                                  */
/*----------------------------------------------------------------------------*/
void DatumRandom::vRndDte(Datum_h* pDatum, Datum_h* pMin, Datum_h* pMax)
{
  RN_Dte rnDte(pDatum,pMin,pMax);
}

/*----------------------------------------------------------------------------*/
/* generate random Flt datum                                                  */
/*----------------------------------------------------------------------------*/
void DatumRandom::vRndFlt(Datum_h* pDatum, Datum_h* pMin, Datum_h* pMax)
{
  RN_Flt rnFlt(pDatum,pMin,pMax);
}

/*----------------------------------------------------------------------------*/
/* generate random Int datum                                                  */
/*----------------------------------------------------------------------------*/
void DatumRandom::vRndInt(Datum_h* pDatum, Datum_h* pMin, Datum_h* pMax)
{
  RN_Int rnInt(pDatum,pMin,pMax);
}

/*----------------------------------------------------------------------------*/
/* generate random Ivl datum                                                  */
/*----------------------------------------------------------------------------*/
void DatumRandom::vRndIvl(Datum_h* pDatum, Datum_h* pMin, Datum_h* pMax)
{
  RN_Ivl rnIvl(pDatum,pMin,pMax);
}

/*----------------------------------------------------------------------------*/
/* generate random tme datum                                                  */
/*----------------------------------------------------------------------------*/
void DatumRandom::vRndTme(Datum_h* pDatum, Datum_h* pMin, Datum_h* pMax)
{
  RN_Tme rnTme(pDatum,pMin,pMax);
}

/*----------------------------------------------------------------------------*/
/* generate random tsp datum                                                  */
/*----------------------------------------------------------------------------*/
void DatumRandom::vRndTsp(Datum_h* pDatum, Datum_h* pMin, Datum_h* pMax)
{
  RN_Tsp rnTsp(pDatum,pMin,pMax);
}

/*----------------------------------------------------------------------------*/
/* generate random vch datum                                                  */
/*----------------------------------------------------------------------------*/
void DatumRandom::vRndVch(Datum_h* pDatum, Datum_h* pMin, Datum_h* pMax)
{
  RN_Vch rnVch(pDatum,pMin,pMax);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
