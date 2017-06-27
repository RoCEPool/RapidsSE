/*============================================================================*/
/* DtmLib.cpp -- Datum Library                                                */
/*============================================================================*/

#include "Datums.h"

#include "StorageEngine.h"

/*----------------------------------------------------------------------------*/
/*-- Validation --------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* valid datum?                                                               */
/*----------------------------------------------------------------------------*/
bool DtmLib::bValidateDatum(eDM eDatum, char** aaSrc, Column* pColumn)
{
  bool b = false;
  
  switch (eDatum) {
    case DM_BCD: b = Bcd::bValid(aaSrc,pColumn); break;
    case DM_BOL: b = Bol::bValid(aaSrc,pColumn); break;
    case DM_CHR: ASSERT(0); break;
    case DM_DTE: b = Dte::bValid(aaSrc,pColumn); break;
    case DM_FLT: b = Flt::bValid(aaSrc,pColumn); break;
    case DM_INT: b = Int::bValid(aaSrc,pColumn); break;
    case DM_IVL: b = Ivl::bValid(aaSrc,pColumn); break;
    case DM_TME: b = Tme::bValid(aaSrc,pColumn); break;
    case DM_TSP: b = Tsp::bValid(aaSrc,pColumn); break;
    case DM_VCH: b = Vch::bValid(aaSrc,pColumn); break;
    default:
      vFatal("DtmLib::bValid: Invalid aDatum argument: %lld\n"_Str,
             eDatum);
  }
  
  if (!b)
    return b;
  else
    return b;
}

/*----------------------------------------------------------------------------*/
/*-- Embedded Comparators ----------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* is datum argA < argB?                                                      */
/*----------------------------------------------------------------------------*/
bool DtmLib::bLessThan(eDM eDatum, char* aArgA, i8 nBytesA, char* aArgB, i8 nBytesB)
{
  bool bLT = false;
  
  switch (eDatum) {
    case DM_BCD: bLT = Bcd::bLessThan(aArgA,nBytesA,aArgB,nBytesB); break;
    case DM_BOL: bLT = Bol::bLessThan(aArgA,nBytesA,aArgB,nBytesB); break;
    case DM_CHR: ASSERT(0); break;
    case DM_DTE: bLT = Dte::bLessThan(aArgA,nBytesA,aArgB,nBytesB); break;
    case DM_FLT: bLT = Flt::bLessThan(aArgA,nBytesA,aArgB,nBytesB); break;
    case DM_INT: bLT = Int::bLessThan(aArgA,nBytesA,aArgB,nBytesB); break;
    case DM_IVL: bLT = Ivl::bLessThan(aArgA,nBytesA,aArgB,nBytesB); break;
    case DM_TME: bLT = Tme::bLessThan(aArgA,nBytesA,aArgB,nBytesB); break;
    case DM_TSP: bLT = Tsp::bLessThan(aArgA,nBytesA,aArgB,nBytesB); break;
    case DM_VBN: ASSERT(0); break;
    case DM_VCH: bLT = Vch::bLessThan(aArgA,nBytesA,aArgB,nBytesB); break;
      
    default:
      THROW("DtmLib::bLessThan: Invalid eDatum %lld"_Str,eDatum);
  }
  
  return bLT;
}

/*----------------------------------------------------------------------------*/
/* is datum argA <= argB?                                                     */
/*----------------------------------------------------------------------------*/
bool DtmLib::bLessEqual(eDM eDatum, char* aArgA, i8 nBytesA, char* aArgB, i8 nBytesB)
{
  bool bLE = false;
  
  switch (eDatum) {
    case DM_BCD: bLE = Bcd::bLessEqual(aArgA,nBytesA,aArgB,nBytesB); break;
    case DM_BOL: bLE = Bol::bLessEqual(aArgA,nBytesA,aArgB,nBytesB); break;
    case DM_CHR: ASSERT(0); break;
    case DM_DTE: bLE = Dte::bLessEqual(aArgA,nBytesA,aArgB,nBytesB); break;
    case DM_FLT: bLE = Flt::bLessEqual(aArgA,nBytesA,aArgB,nBytesB); break;
    case DM_INT: bLE = Int::bLessEqual(aArgA,nBytesA,aArgB,nBytesB); break;
    case DM_IVL: bLE = Ivl::bLessEqual(aArgA,nBytesA,aArgB,nBytesB); break;
    case DM_TME: bLE = Tme::bLessEqual(aArgA,nBytesA,aArgB,nBytesB); break;
    case DM_TSP: bLE = Tsp::bLessEqual(aArgA,nBytesA,aArgB,nBytesB); break;
    case DM_VBN: ASSERT(0); break;
    case DM_VCH: bLE = Vch::bLessEqual(aArgA,nBytesA,aArgB,nBytesB); break;
      
    default:
      THROW("DtmLib::bLessEqual: Invalid eDatum %lld"_Str,eDatum);
  }
  
  return bLE;
}

/*----------------------------------------------------------------------------*/
/* is datum argA == argB?                                                     */
/*----------------------------------------------------------------------------*/
bool DtmLib::bEqualTo(eDM eDatum, char* aArgA, i8 nBytesA, char* aArgB, i8 nBytesB)
{
  bool bEQ = false;
  
  switch (eDatum) {
    case DM_BCD: bEQ = Bcd::bEqualTo(aArgA,nBytesA,aArgB,nBytesB); break;
    case DM_BOL: bEQ = Bol::bEqualTo(aArgA,nBytesA,aArgB,nBytesB); break;
    case DM_CHR: ASSERT(0); break;
    case DM_DTE: bEQ = Dte::bEqualTo(aArgA,nBytesA,aArgB,nBytesB); break;
    case DM_FLT: bEQ = Flt::bEqualTo(aArgA,nBytesA,aArgB,nBytesB); break;
    case DM_INT: bEQ = Int::bEqualTo(aArgA,nBytesA,aArgB,nBytesB); break;
    case DM_IVL: bEQ = Ivl::bEqualTo(aArgA,nBytesA,aArgB,nBytesB); break;
    case DM_TME: bEQ = Tme::bEqualTo(aArgA,nBytesA,aArgB,nBytesB); break;
    case DM_TSP: bEQ = Tsp::bEqualTo(aArgA,nBytesA,aArgB,nBytesB); break;
    case DM_VBN: ASSERT(0); break;
    case DM_VCH: bEQ = Vch::bEqualTo(aArgA,nBytesA,aArgB,nBytesB); break;
      
    default:
      THROW("DtmLib::bEqualTo: Invalid eDatum %lld"_Str,eDatum);
  }
  
  return bEQ;
}

/*----------------------------------------------------------------------------*/
/* is datum argA != argB?                                                     */
/*----------------------------------------------------------------------------*/
bool DtmLib::bNotEqual(eDM eDatum, char* aArgA, i8 nBytesA, char* aArgB, i8 nBytesB)
{
  bool bNE = false;
  
  switch (eDatum) {
    case DM_BCD: bNE = Bcd::bNotEqual(aArgA,nBytesA,aArgB,nBytesB); break;
    case DM_BOL: bNE = Bol::bNotEqual(aArgA,nBytesA,aArgB,nBytesB); break;
    case DM_CHR: ASSERT(0); break;
    case DM_DTE: bNE = Dte::bNotEqual(aArgA,nBytesA,aArgB,nBytesB); break;
    case DM_FLT: bNE = Flt::bNotEqual(aArgA,nBytesA,aArgB,nBytesB); break;
    case DM_INT: bNE = Int::bNotEqual(aArgA,nBytesA,aArgB,nBytesB); break;
    case DM_IVL: bNE = Ivl::bNotEqual(aArgA,nBytesA,aArgB,nBytesB); break;
    case DM_TME: bNE = Tme::bNotEqual(aArgA,nBytesA,aArgB,nBytesB); break;
    case DM_TSP: bNE = Tsp::bNotEqual(aArgA,nBytesA,aArgB,nBytesB); break;
    case DM_VBN: ASSERT(0); break;
    case DM_VCH: bNE = Vch::bNotEqual(aArgA,nBytesA,aArgB,nBytesB); break;
      
    default:
      THROW("DtmLib::bNotEqual: Invalid eDatum %lld"_Str,eDatum);
  }
  
  return bNE;
}

/*----------------------------------------------------------------------------*/
/* is datum argA >= argB?                                                     */
/*----------------------------------------------------------------------------*/
bool DtmLib::bGreaterEqual(eDM eDatum, char* aArgA, i8 nBytesA, char* aArgB, i8 nBytesB)
{
  bool bGE = false;
  
  switch (eDatum) {
    case DM_BCD: bGE = Bcd::bGreaterEqual(aArgA,nBytesA,aArgB,nBytesB); break;
    case DM_BOL: bGE = Bol::bGreaterEqual(aArgA,nBytesA,aArgB,nBytesB); break;
    case DM_CHR: ASSERT(0); break;
    case DM_DTE: bGE = Dte::bGreaterEqual(aArgA,nBytesA,aArgB,nBytesB); break;
    case DM_FLT: bGE = Flt::bGreaterEqual(aArgA,nBytesA,aArgB,nBytesB); break;
    case DM_INT: bGE = Int::bGreaterEqual(aArgA,nBytesA,aArgB,nBytesB); break;
    case DM_IVL: bGE = Ivl::bGreaterEqual(aArgA,nBytesA,aArgB,nBytesB); break;
    case DM_TME: bGE = Tme::bGreaterEqual(aArgA,nBytesA,aArgB,nBytesB); break;
    case DM_TSP: bGE = Tsp::bGreaterEqual(aArgA,nBytesA,aArgB,nBytesB); break;
    case DM_VBN: ASSERT(0); break;
    case DM_VCH: bGE = Vch::bGreaterEqual(aArgA,nBytesA,aArgB,nBytesB); break;
      
    default:
      THROW("DtmLib::bGreaterEqual: Invalid eDatum %lld"_Str,eDatum);
  }
  
  return bGE;
}

/*----------------------------------------------------------------------------*/
/* is datum argA > argB?                                                      */
/*----------------------------------------------------------------------------*/
bool DtmLib::bGreaterThan(eDM eDatum, char* aArgA, i8 nBytesA, char* aArgB, i8 nBytesB)
{
  bool bGT = false;
  
  switch (eDatum) {
    case DM_BCD: bGT = Bcd::bGreaterThan(aArgA,nBytesA,aArgB,nBytesB); break;
    case DM_BOL: bGT = Bol::bGreaterThan(aArgA,nBytesA,aArgB,nBytesB); break;
    case DM_CHR: ASSERT(0); break;
    case DM_DTE: bGT = Dte::bGreaterThan(aArgA,nBytesA,aArgB,nBytesB); break;
    case DM_FLT: bGT = Flt::bGreaterThan(aArgA,nBytesA,aArgB,nBytesB); break;
    case DM_INT: bGT = Int::bGreaterThan(aArgA,nBytesA,aArgB,nBytesB); break;
    case DM_IVL: bGT = Ivl::bGreaterThan(aArgA,nBytesA,aArgB,nBytesB); break;
    case DM_TME: bGT = Tme::bGreaterThan(aArgA,nBytesA,aArgB,nBytesB); break;
    case DM_TSP: bGT = Tsp::bGreaterThan(aArgA,nBytesA,aArgB,nBytesB); break;
    case DM_VBN: ASSERT(0); break;
    case DM_VCH: bGT = Vch::bGreaterThan(aArgA,nBytesA,aArgB,nBytesB); default:
      THROW("DtmLib::bGreaterThan: Invalid eDatum %lld"_Str,eDatum);
  }
  
  return bGT;
}

/*----------------------------------------------------------------------------*/
/* cmp datum argA with argB (-ve: A < B, 0: A == B, +ve: A > B)               */
/*----------------------------------------------------------------------------*/
i8 DtmLib::iCmp(eDM eDatum, char* aArgA, i8 nBytesA, char* aArgB, i8 nBytesB)
{
  i8 iCmp = 0;
  
  switch (eDatum) {
    case DM_BCD: iCmp = Bcd::iCmp(aArgA,nBytesA,aArgB,nBytesB); break;
    case DM_BOL: iCmp = Bol::iCmp(aArgA,nBytesA,aArgB,nBytesB); break;
    case DM_CHR: ASSERT(0); break;
    case DM_DTE: iCmp = Dte::iCmp(aArgA,nBytesA,aArgB,nBytesB); break;
    case DM_FLT: iCmp = Flt::iCmp(aArgA,nBytesA,aArgB,nBytesB); break;
    case DM_INT: iCmp = Int::iCmp(aArgA,nBytesA,aArgB,nBytesB); break;
    case DM_IVL: iCmp = Ivl::iCmp(aArgA,nBytesA,aArgB,nBytesB); break;
    case DM_TME: iCmp = Tme::iCmp(aArgA,nBytesA,aArgB,nBytesB); break;
    case DM_TSP: iCmp = Tsp::iCmp(aArgA,nBytesA,aArgB,nBytesB); break;
    case DM_VBN: ASSERT(0); break;
    case DM_VCH: iCmp = Vch::iCmp(aArgA,nBytesA,aArgB,nBytesB); break;
      
    default:
      THROW("DtmLib::iCmp: Invalid eDatum %lld"_Str,eDatum);
  }
  
  return iCmp;
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
