/*============================================================================*/
/* DtmLib.cpp -- Datum Library                                                */
/*============================================================================*/

#include "Datums.h"

#include "StorageEngine.h"

/*----------------------------------------------------------------------------*/
/* copy datum (return the number of bytes copied)                             */
/*----------------------------------------------------------------------------*/
i8 DtmLib::nCopy(eDM eDatum, void* pDst, void* pSrc)
{
  switch (eDatum) {
      
    case DM_BCD: return Bcd::nCopy(pDst,pSrc);
    case DM_BOL: return Bol::nCopy(pDst,pSrc);
    case DM_DTE: return Dte::nCopy(pDst,pSrc);
    case DM_FLT: return Flt::nCopy(pDst,pSrc);
    case DM_INT: return Int::nCopy(pDst,pSrc);
    case DM_IVL: return Ivl::nCopy(pDst,pSrc);
    case DM_TME: return Tme::nCopy(pDst,pSrc);
    case DM_TSP: return Tsp::nCopy(pDst,pSrc);
    case DM_VCH: return Vch::nCopy(pDst,pSrc);
      
    default:
      vFatal("ValLib::vCopyDatum: Invalid copy: eDatum %lld\n"_Stz,
             eDatum);
      return -1;
  }
}

/*----------------------------------------------------------------------------*/
/* s-print                                                                    */
/*----------------------------------------------------------------------------*/
void DtmLib::vCopy(eDM eDatum, void* aDst, void* aSrc)
{
  switch(eDatum) {
    case DM_BCD: Bcd::vCopy((char*)aDst,(char*)aSrc); break;
    case DM_BOL: Bol::vCopy((char*)aDst,(char*)aSrc); break;
    case DM_DTE: Dte::vCopy((char*)aDst,(char*)aSrc); break;
    case DM_FLT: Flt::vCopy((char*)aDst,(char*)aSrc); break;
    case DM_INT: Int::vCopy((char*)aDst,(char*)aSrc); break;
    case DM_IVL: Ivl::vCopy((char*)aDst,(char*)aSrc); break;
    case DM_TME: Tme::vCopy((char*)aDst,(char*)aSrc); break;
    case DM_TSP: Tsp::vCopy((char*)aDst,(char*)aSrc); break;
    case DM_VCH: Vch::vCopy((char*)aDst,(char*)aSrc); break;
    default:
      NEVER_REACH;
  }
}

/*----------------------------------------------------------------------------*/
/*-- Embedded Comparators ----------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* is datum argA < argB?                                                      */
/*----------------------------------------------------------------------------*/
bool DtmLib::bLessThan(eDM eDatum, char* aArgA, char* aArgB)
{
  bool bLT = false;
  
  switch (eDatum) {
    case DM_BCD: bLT = Bcd::bLessThan(aArgA,aArgB); break;
    case DM_BOL: bLT = Bol::bLessThan(aArgA,aArgB); break;
    case DM_CHR: bLT = Vch::bLessThan(aArgA,aArgB); break;
    case DM_DTE: bLT = Dte::bLessThan(aArgA,aArgB); break;
    case DM_FLT: bLT = Flt::bLessThan(aArgA,aArgB); break;
    case DM_INT: bLT = Int::bLessThan(aArgA,aArgB); break;
    case DM_IVL: bLT = Ivl::bLessThan(aArgA,aArgB); break;
    case DM_TME: bLT = Tme::bLessThan(aArgA,aArgB); break;
    case DM_TSP: bLT = Tsp::bLessThan(aArgA,aArgB); break;
    case DM_VBN: NEVER_REACH; break;
    case DM_VCH: bLT = Vch::bLessThan(aArgA,aArgB); break;
      
    default:
      THROW("DtmLib::bLessThan: Invalid eDatum %lld"_Stz,eDatum);
  }
  
  return bLT;
}

/*----------------------------------------------------------------------------*/
/* is datum argA <= argB?                                                     */
/*----------------------------------------------------------------------------*/
bool DtmLib::bLessEqual(eDM eDatum, char* aArgA, char* aArgB)
{
  bool bLE = false;
  
  switch (eDatum) {
    case DM_BCD: bLE = Bcd::bLessEqual(aArgA,aArgB); break;
    case DM_BOL: bLE = Bol::bLessEqual(aArgA,aArgB); break;
    case DM_CHR: bLE = Vch::bLessEqual(aArgA,aArgB); break;
    case DM_DTE: bLE = Dte::bLessEqual(aArgA,aArgB); break;
    case DM_FLT: bLE = Flt::bLessEqual(aArgA,aArgB); break;
    case DM_INT: bLE = Int::bLessEqual(aArgA,aArgB); break;
    case DM_IVL: bLE = Ivl::bLessEqual(aArgA,aArgB); break;
    case DM_TME: bLE = Tme::bLessEqual(aArgA,aArgB); break;
    case DM_TSP: bLE = Tsp::bLessEqual(aArgA,aArgB); break;
    case DM_VBN: NEVER_REACH; break;
    case DM_VCH: bLE = Vch::bLessEqual(aArgA,aArgB); break;
      
    default:
      THROW("DtmLib::bLessEqual: Invalid eDatum %lld"_Stz,eDatum);
  }
  
  return bLE;
}

/*----------------------------------------------------------------------------*/
/* is datum argA == argB?                                                     */
/*----------------------------------------------------------------------------*/
bool DtmLib::bEqualTo(eDM eDatum, char* aArgA, char* aArgB)
{
  bool bEQ = false;
  
  switch (eDatum) {
    case DM_BCD: bEQ = Bcd::bEqualTo(aArgA,aArgB); break;
    case DM_BOL: bEQ = Bol::bEqualTo(aArgA,aArgB); break;
    case DM_CHR: bEQ = Vch::bEqualTo(aArgA,aArgB); break;
    case DM_DTE: bEQ = Dte::bEqualTo(aArgA,aArgB); break;
    case DM_FLT: bEQ = Flt::bEqualTo(aArgA,aArgB); break;
    case DM_INT: bEQ = Int::bEqualTo(aArgA,aArgB); break;
    case DM_IVL: bEQ = Ivl::bEqualTo(aArgA,aArgB); break;
    case DM_TME: bEQ = Tme::bEqualTo(aArgA,aArgB); break;
    case DM_TSP: bEQ = Tsp::bEqualTo(aArgA,aArgB); break;
    case DM_VBN: NEVER_REACH; break;
    case DM_VCH: bEQ = Vch::bEqualTo(aArgA,aArgB); break;
      
    default:
      THROW("DtmLib::bEqualTo: Invalid eDatum %lld"_Stz,eDatum);
  }
  
  return bEQ;
}

/*----------------------------------------------------------------------------*/
/* is datum argA != argB?                                                     */
/*----------------------------------------------------------------------------*/
bool DtmLib::bNotEqual(eDM eDatum, char* aArgA, char* aArgB)
{
  bool bNE = false;
  
  switch (eDatum) {
    case DM_BCD: bNE = Bcd::bNotEqual(aArgA,aArgB); break;
    case DM_BOL: bNE = Bol::bNotEqual(aArgA,aArgB); break;
    case DM_CHR: bNE = Vch::bNotEqual(aArgA,aArgB); break;
    case DM_DTE: bNE = Dte::bNotEqual(aArgA,aArgB); break;
    case DM_FLT: bNE = Flt::bNotEqual(aArgA,aArgB); break;
    case DM_INT: bNE = Int::bNotEqual(aArgA,aArgB); break;
    case DM_IVL: bNE = Ivl::bNotEqual(aArgA,aArgB); break;
    case DM_TME: bNE = Tme::bNotEqual(aArgA,aArgB); break;
    case DM_TSP: bNE = Tsp::bNotEqual(aArgA,aArgB); break;
    case DM_VBN: NEVER_REACH; break;
    case DM_VCH: bNE = Vch::bNotEqual(aArgA,aArgB); break;
      
    default:
      THROW("DtmLib::bNotEqual: Invalid eDatum %lld"_Stz,eDatum);
  }
  
  return bNE;
}

/*----------------------------------------------------------------------------*/
/* is datum argA >= argB?                                                     */
/*----------------------------------------------------------------------------*/
bool DtmLib::bGreaterEqual(eDM eDatum, char* aArgA, char* aArgB)
{
  bool bGE = false;
  
  switch (eDatum) {
    case DM_BCD: bGE = Bcd::bGreaterEqual(aArgA,aArgB); break;
    case DM_BOL: bGE = Bol::bGreaterEqual(aArgA,aArgB); break;
    case DM_CHR: bGE = Vch::bGreaterEqual(aArgA,aArgB); break;
    case DM_DTE: bGE = Dte::bGreaterEqual(aArgA,aArgB); break;
    case DM_FLT: bGE = Flt::bGreaterEqual(aArgA,aArgB); break;
    case DM_INT: bGE = Int::bGreaterEqual(aArgA,aArgB); break;
    case DM_IVL: bGE = Ivl::bGreaterEqual(aArgA,aArgB); break;
    case DM_TME: bGE = Tme::bGreaterEqual(aArgA,aArgB); break;
    case DM_TSP: bGE = Tsp::bGreaterEqual(aArgA,aArgB); break;
    case DM_VBN: NEVER_REACH; break;
    case DM_VCH: bGE = Vch::bGreaterEqual(aArgA,aArgB); break;
      
    default:
      THROW("DtmLib::bGreaterEqual: Invalid eDatum %lld"_Stz,eDatum);
  }
  
  return bGE;
}

/*----------------------------------------------------------------------------*/
/* is datum argA > argB?                                                      */
/*----------------------------------------------------------------------------*/
bool DtmLib::bGreaterThan(eDM eDatum, char* aArgA, char* aArgB)
{
  bool bGT = false;
  
  switch (eDatum) {
    case DM_BCD: bGT = Bcd::bGreaterThan(aArgA,aArgB); break;
    case DM_BOL: bGT = Bol::bGreaterThan(aArgA,aArgB); break;
    case DM_CHR: bGT = Vch::bGreaterThan(aArgA,aArgB); break;
    case DM_DTE: bGT = Dte::bGreaterThan(aArgA,aArgB); break;
    case DM_FLT: bGT = Flt::bGreaterThan(aArgA,aArgB); break;
    case DM_INT: bGT = Int::bGreaterThan(aArgA,aArgB); break;
    case DM_IVL: bGT = Ivl::bGreaterThan(aArgA,aArgB); break;
    case DM_TME: bGT = Tme::bGreaterThan(aArgA,aArgB); break;
    case DM_TSP: bGT = Tsp::bGreaterThan(aArgA,aArgB); break;
    case DM_VBN: NEVER_REACH; break;
    case DM_VCH: bGT = Vch::bGreaterThan(aArgA,aArgB); break;
    
    default:
      THROW("DtmLib::bGreaterThan: Invalid eDatum %lld"_Stz,eDatum);
  }
  
  return bGT;
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
