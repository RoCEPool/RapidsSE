/*============================================================================*/
/* DtmLib.cpp -- Datum Library                                                */
/*============================================================================*/

#include "Datums.h"

#include "StorageEngine.h"

/*----------------------------------------------------------------------------*/
/* extract an varchar from substr                                             */
/*----------------------------------------------------------------------------*/
bool DtmLib::bExtractVch(char** aaSrc, Vch* aVch, Column* pColumn)
{
  char* aChar = (char*)aVch;
  
  // test for null
  if ( ! **aaSrc || '|' == **aaSrc ) {
    aVch->vSetNull();
    return true;
  }
  
  // collect the csv
  while ('|' != **aaSrc && ',' != **aaSrc)
    *aChar++ = *(*aaSrc)++;
  *aChar++ = '\0';
  
  return true;
}

/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* valid datum?                                                               */
/*----------------------------------------------------------------------------*/
bool DtmLib::bValidateDatum(eDM eDatum, char** aaSrc, Column* pColumn)
{
  bool bValid = false;
  
  switch (eDatum) {
    case DM_BCD: bValid = Bcd::bValid(aaSrc,pColumn); break;
    case DM_BOL: bValid = Bol::bValid(aaSrc,pColumn); break;
    case DM_CHR: bValid = bValidVch(aaSrc,pColumn); break;
    case DM_DTE: bValid = Dte::bValid(aaSrc,pColumn); break;
    case DM_FLT: bValid = Flt::bValid(aaSrc,pColumn); break;
    case DM_INT: bValid = Int::bValid(aaSrc,pColumn); break;
    case DM_IVL: bValid = Ivl::bValid(aaSrc,pColumn); break;
    case DM_TME: bValid = Tme::bValid(aaSrc,pColumn); break;
    case DM_TSP: bValid = Tsp::bValid(aaSrc,pColumn); break;
    case DM_VCH: bValid = bValidVch(aaSrc,pColumn); break;
    default:
      vFatal("DtmLib::bValid: Invalid aDatum argument: %lld\n"_Str,
             eDatum);
  }
  
  if (!bValid)
    return bValid;
  else
    return bValid;
}

/*----------------------------------------------------------------------------*/
/* valid varchar in substr?                                                   */
/*----------------------------------------------------------------------------*/
bool DtmLib::bValidVch(char** aaSrc, Column* pColumn)
{
  s48K sBuffer;
  
  Vch* aVch = (Vch*)sBuffer.aStr();
  bool bValid = bExtractVch(aaSrc,aVch,pColumn);
  
  return bValid;
}

/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* is argA < argB?                                                            */
/*----------------------------------------------------------------------------*/
bool DtmLib::bLessThan(eDM eDatum, char* aArgA, i8 nBytesA, char* aArgB, i8 nBytesB)
{
  bool bLessThan = false;
  
  switch (eDatum) {
    case DM_BCD: bLessThan = Bcd::bLessThan(aArgA,nBytesA,aArgB,nBytesB); break;
    case DM_BOL: bLessThan = Bol::bLessThan(aArgA,nBytesA,aArgB,nBytesB); break;
    case DM_CHR: ASSERT(0); break;
    case DM_DTE: bLessThan = Dte::bLessThan(aArgA,nBytesA,aArgB,nBytesB); break;
    case DM_FLT: bLessThan = Flt::bLessThan(aArgA,nBytesA,aArgB,nBytesB); break;
    case DM_INT: bLessThan = Int::bLessThan(aArgA,nBytesA,aArgB,nBytesB); break;
    case DM_IVL: bLessThan = Ivl::bLessThan(aArgA,nBytesA,aArgB,nBytesB); break;
    case DM_TME: bLessThan = Tme::bLessThan(aArgA,nBytesA,aArgB,nBytesB); break;
    case DM_TSP: bLessThan = Tsp::bLessThan(aArgA,nBytesA,aArgB,nBytesB); break;
    case DM_VBN: ASSERT(0); break;
    case DM_VCH: bLessThan = Vch::bLessThan(aArgA,nBytesA,aArgB,nBytesB); break;
      
    default:
      THROW("DtmLib::bLessThan: Invalid eDatum %lld"_Str,eDatum);
  }
  
  return bLessThan;
}

/*----------------------------------------------------------------------------*/
/* is argA <= argB?                                                           */
/*----------------------------------------------------------------------------*/
bool DtmLib::bLessEqual(eDM eDatum, char* aArgA, i8 nBytesA, char* aArgB, i8 nBytesB)
{
  bool bLessEqual = false;
  
  switch (eDatum) {
    case DM_BCD: bLessEqual = Bcd::bLessEqual(aArgA,nBytesA,aArgB,nBytesB); break;
    case DM_BOL: bLessEqual = Bol::bLessEqual(aArgA,nBytesA,aArgB,nBytesB); break;
    case DM_CHR: ASSERT(0); break;
    case DM_DTE: bLessEqual = Dte::bLessEqual(aArgA,nBytesA,aArgB,nBytesB); break;
    case DM_FLT: bLessEqual = Flt::bLessEqual(aArgA,nBytesA,aArgB,nBytesB); break;
    case DM_INT: bLessEqual = Int::bLessEqual(aArgA,nBytesA,aArgB,nBytesB); break;
    case DM_IVL: bLessEqual = Ivl::bLessEqual(aArgA,nBytesA,aArgB,nBytesB); break;
    case DM_TME: bLessEqual = Tme::bLessEqual(aArgA,nBytesA,aArgB,nBytesB); break;
    case DM_TSP: bLessEqual = Tsp::bLessEqual(aArgA,nBytesA,aArgB,nBytesB); break;
    case DM_VBN: ASSERT(0); break;
    case DM_VCH: bLessEqual = Vch::bLessEqual(aArgA,nBytesA,aArgB,nBytesB); break;
      
    default:
      THROW("DtmLib::bLessEqual: Invalid eDatum %lld"_Str,eDatum);
  }
  
  return bLessEqual;
}

/*----------------------------------------------------------------------------*/
/* is argA == argB?                                                           */
/*----------------------------------------------------------------------------*/
bool DtmLib::bEqualTo(eDM eDatum, char* aArgA, i8 nBytesA, char* aArgB, i8 nBytesB)
{
  bool bEqualTo = false;
  
  switch (eDatum) {
    case DM_BCD: bEqualTo = Bcd::bEqualTo(aArgA,nBytesA,aArgB,nBytesB); break;
    case DM_BOL: bEqualTo = Bol::bEqualTo(aArgA,nBytesA,aArgB,nBytesB); break;
    case DM_CHR: ASSERT(0); break;
    case DM_DTE: bEqualTo = Dte::bEqualTo(aArgA,nBytesA,aArgB,nBytesB); break;
    case DM_FLT: bEqualTo = Flt::bEqualTo(aArgA,nBytesA,aArgB,nBytesB); break;
    case DM_INT: bEqualTo = Int::bEqualTo(aArgA,nBytesA,aArgB,nBytesB); break;
    case DM_IVL: bEqualTo = Ivl::bEqualTo(aArgA,nBytesA,aArgB,nBytesB); break;
    case DM_TME: bEqualTo = Tme::bEqualTo(aArgA,nBytesA,aArgB,nBytesB); break;
    case DM_TSP: bEqualTo = Tsp::bEqualTo(aArgA,nBytesA,aArgB,nBytesB); break;
    case DM_VBN: ASSERT(0); break;
    case DM_VCH: bEqualTo = Vch::bEqualTo(aArgA,nBytesA,aArgB,nBytesB); break;
      
    default:
      THROW("DtmLib::bEqualTo: Invalid eDatum %lld"_Str,eDatum);
  }
  
  return bEqualTo;
}

/*----------------------------------------------------------------------------*/
/* is argA != argB?                                                           */
/*----------------------------------------------------------------------------*/
bool DtmLib::bNotEqual(eDM eDatum, char* aArgA, i8 nBytesA, char* aArgB, i8 nBytesB)
{
  bool bNotEqual = false;
  
  switch (eDatum) {
    case DM_BCD: bNotEqual = Bcd::bNotEqual(aArgA,nBytesA,aArgB,nBytesB); break;
    case DM_BOL: bNotEqual = Bol::bNotEqual(aArgA,nBytesA,aArgB,nBytesB); break;
    case DM_CHR: ASSERT(0); break;
    case DM_DTE: bNotEqual = Dte::bNotEqual(aArgA,nBytesA,aArgB,nBytesB); break;
    case DM_FLT: bNotEqual = Flt::bNotEqual(aArgA,nBytesA,aArgB,nBytesB); break;
    case DM_INT: bNotEqual = Int::bNotEqual(aArgA,nBytesA,aArgB,nBytesB); break;
    case DM_IVL: bNotEqual = Ivl::bNotEqual(aArgA,nBytesA,aArgB,nBytesB); break;
    case DM_TME: bNotEqual = Tme::bNotEqual(aArgA,nBytesA,aArgB,nBytesB); break;
    case DM_TSP: bNotEqual = Tsp::bNotEqual(aArgA,nBytesA,aArgB,nBytesB); break;
    case DM_VBN: ASSERT(0); break;
    case DM_VCH: bNotEqual = Vch::bNotEqual(aArgA,nBytesA,aArgB,nBytesB); break;
      
    default:
      THROW("DtmLib::bNotEqual: Invalid eDatum %lld"_Str,eDatum);
  }
  
  return bNotEqual;
}

/*----------------------------------------------------------------------------*/
/* is argA >= argB?                                                           */
/*----------------------------------------------------------------------------*/
bool DtmLib::bGreaterEqual(eDM eDatum, char* aArgA, i8 nBytesA, char* aArgB, i8 nBytesB)
{
  bool bGreaterEqual = false;
  
  switch (eDatum) {
    case DM_BCD: bGreaterEqual = Bcd::bGreaterEqual(aArgA,nBytesA,aArgB,nBytesB); break;
    case DM_BOL: bGreaterEqual = Bol::bGreaterEqual(aArgA,nBytesA,aArgB,nBytesB); break;
    case DM_CHR: ASSERT(0); break;
    case DM_DTE: bGreaterEqual = Dte::bGreaterEqual(aArgA,nBytesA,aArgB,nBytesB); break;
    case DM_FLT: bGreaterEqual = Flt::bGreaterEqual(aArgA,nBytesA,aArgB,nBytesB); break;
    case DM_INT: bGreaterEqual = Int::bGreaterEqual(aArgA,nBytesA,aArgB,nBytesB); break;
    case DM_IVL: bGreaterEqual = Ivl::bGreaterEqual(aArgA,nBytesA,aArgB,nBytesB); break;
    case DM_TME: bGreaterEqual = Tme::bGreaterEqual(aArgA,nBytesA,aArgB,nBytesB); break;
    case DM_TSP: bGreaterEqual = Tsp::bGreaterEqual(aArgA,nBytesA,aArgB,nBytesB); break;
    case DM_VBN: ASSERT(0); break;
    case DM_VCH: bGreaterEqual = Vch::bGreaterEqual(aArgA,nBytesA,aArgB,nBytesB); break;
      
    default:
      THROW("DtmLib::bGreaterEqual: Invalid eDatum %lld"_Str,eDatum);
  }
  
  return bGreaterEqual;
}

/*----------------------------------------------------------------------------*/
/* is argA > argB?                                                            */
/*----------------------------------------------------------------------------*/
bool DtmLib::bGreaterThan(eDM eDatum, char* aArgA, i8 nBytesA, char* aArgB, i8 nBytesB)
{
  bool bGreaterThan = false;
  
  switch (eDatum) {
    case DM_BCD: bGreaterThan = Bcd::bGreaterThan(aArgA,nBytesA,aArgB,nBytesB); break;
    case DM_BOL: bGreaterThan = Bol::bGreaterThan(aArgA,nBytesA,aArgB,nBytesB); break;
    case DM_CHR: ASSERT(0); break;
    case DM_DTE: bGreaterThan = Dte::bGreaterThan(aArgA,nBytesA,aArgB,nBytesB); break;
    case DM_FLT: bGreaterThan = Flt::bGreaterThan(aArgA,nBytesA,aArgB,nBytesB); break;
    case DM_INT: bGreaterThan = Int::bGreaterThan(aArgA,nBytesA,aArgB,nBytesB); break;
    case DM_IVL: bGreaterThan = Ivl::bGreaterThan(aArgA,nBytesA,aArgB,nBytesB); break;
    case DM_TME: bGreaterThan = Tme::bGreaterThan(aArgA,nBytesA,aArgB,nBytesB); break;
    case DM_TSP: bGreaterThan = Tsp::bGreaterThan(aArgA,nBytesA,aArgB,nBytesB); break;
    case DM_VBN: ASSERT(0); break;
    case DM_VCH: bGreaterThan = Vch::bGreaterThan(aArgA,nBytesA,aArgB,nBytesB); default:
      THROW("DtmLib::bGreaterThan: Invalid eDatum %lld"_Str,eDatum);
  }
  
  return bGreaterThan;
}

/*----------------------------------------------------------------------------*/
/* cmp argA with argB (-ve: argA < argB, 0: argA == argB, +ve: argA > argB)   */
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
