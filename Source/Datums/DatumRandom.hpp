/*============================================================================*/
/* DatumRandom.hpp -- Datum-Random Helper                                     */
/*============================================================================*/
#pragma once

namespace Datums
{
  class DatumRandom
  {
  public:
    static void vRndDtm(Datum_h* pDatum, Column* pCol);
  
    static void vRndBcd(Datum_h* pDatum, Column* pCol);
    static void vRndBol(Datum_h* pDatum, Column* pCol);
    static void vRndDte(Datum_h* pDatum, Column* pCol);
    static void vRndFlt(Datum_h* pDatum, Column* pCol);
    static void vRndInt(Datum_h* pDatum, Column* pCol);
    static void vRndIvl(Datum_h* pDatum, Column* pCol);
    static void vRndTme(Datum_h* pDatum, Column* pCol);
    static void vRndTsp(Datum_h* pDatum, Column* pCol);
    static void vRndVch(Datum_h* pDatum, Column* pCol);
  
    static void vRndBcd(Datum_h* pDatum, Datum_h* pMin, Datum_h* pMax);
    static void vRndBol(Datum_h* pDatum, Datum_h* pMin, Datum_h* pMax);
    static void vRndDte(Datum_h* pDatum, Datum_h* pMin, Datum_h* pMax);
    static void vRndFlt(Datum_h* pDatum, Datum_h* pMin, Datum_h* pMax);
    static void vRndInt(Datum_h* pDatum, Datum_h* pMin, Datum_h* pMax);
    static void vRndIvl(Datum_h* pDatum, Datum_h* pMin, Datum_h* pMax);
    static void vRndTme(Datum_h* pDatum, Datum_h* pMin, Datum_h* pMax);
    static void vRndTsp(Datum_h* pDatum, Datum_h* pMin, Datum_h* pMax);
    static void vRndVch(Datum_h* pDatum, Datum_h* pMin, Datum_h* pMax);
  private:
  
  };
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
