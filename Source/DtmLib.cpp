/*============================================================================*/
/* DtmLib.cpp -- Datum Library                                                */
/*============================================================================*/

#include "Datums.h"

#include "StorageEngine.h"

/*----------------------------------------------------------------------------*/
/* copy datum (return the number of bytes copied)                             */
/*----------------------------------------------------------------------------*/
i8 DtmLib::nCopy(eCN eClass, void* pDst, void* pSrc)
{
  switch (eClass) {
      
    case CN_INTVAL: return Int::nCopy(pDst,pSrc);
      
    case CN_STRVAL: return Vch::nCopy(pDst,pSrc);
      
    default:
      vFatal("ValLib::vCopyDatum: Invalid copy: eClass %lld\n"_Str,
             eClass);
      return -1;
  }
}

/*----------------------------------------------------------------------------*/
/* copy datum                                                                 */
/*----------------------------------------------------------------------------*/
void DtmLib::vCopy(eCN eClass, void* pDst, void* pSrc)
{
  switch (eClass) {
      
    case CN_INTVAL: Int::vCopy(pDst,pSrc); return;
      
    case CN_STRVAL: Vch::vCopy(pDst,pSrc); return;
      
    default:
      vFatal("ValLib::vCopyDatum: Invalid copy: eClass %lld\n"_Str,
             eClass);
  }
}

/*----------------------------------------------------------------------------*/
/*-- Extract -----------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* extract                                                                    */
/*----------------------------------------------------------------------------*/
bool DtmLib::bExtract(eDM eDatum, Str* aDst, char** aaSrc, DBobjects::Column* pColumn)
{
  bool b = false;
  
  switch (eDatum) {
    case DM_BCD: b = Bcd::bExtract(aaSrc,(Bcd*)aDst,pColumn); break;
    case DM_BOL: b = Bol::bExtract(aaSrc,(Bol*)aDst,pColumn); break;
    case DM_CHR: ASSERT(0);                                   break;
    case DM_DTE: b = Dte::bExtract(aaSrc,(Dte*)aDst,pColumn); break;
    case DM_FLT: b = Flt::bExtract(aaSrc,(Flt*)aDst,pColumn); break;
    case DM_INT: b = Int::bExtract(aaSrc,(Int*)aDst,pColumn); break;
    case DM_IVL: b = Ivl::bExtract(aaSrc,(Ivl*)aDst,pColumn); break;
    case DM_TME: b = Tme::bExtract(aaSrc,(Tme*)aDst,pColumn); break;
    case DM_TSP: b = Tsp::bExtract(aaSrc,(Tsp*)aDst,pColumn); break;
    case DM_VCH: b = Vch::bExtract(aaSrc,(Vch*)aDst,pColumn); break;
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
/*-- Hash --------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* hash                                                                       */
/*----------------------------------------------------------------------------*/
void DtmLib::vHash(eDM eDatum, i8& iHash, Str* aScr, DBobjects::Column* pColumn)
{
  switch (pColumn->eColumnType()) {
      
    case CT_BOOLEAN: {
      Bol xBol;
      bool b = Bol::bExtract((char**)&aScr,&xBol,pColumn);
      ASSERT(b);
      iHash += XXH32(&xBol,sizeof(Bol),0);
      break;
    }
      
    case CT_CHAR: {
      ASSERT(0);
      break;
    }
      
    case CT_DATE: {
      Dte xDte;
      bool b = Dte::bExtract((char**)&aScr,&xDte,pColumn);
      ASSERT(b);
      iHash += XXH32(&xDte,sizeof(Dte),0);
      break;
    }
      
    case CT_DECIMAL: {
      ASSERT(0);
      break;
    }
      
    case CT_FLOAT: {
      Flt xFlt;
      bool b = Flt::bExtract((char**)&aScr,&xFlt,pColumn);
      ASSERT(b);
      iHash += XXH32(&xFlt,sizeof(Flt),0);
      break;
    }
      
    case CT_INTEGER: {
      Int xInt;
      bool b = Int::bExtract((char**)&aScr,&xInt,pColumn);
      ASSERT(b);
      iHash += XXH32(&xInt,sizeof(Int),0);
      break;
    }
      
    case CT_INTERVAL: {
      Ivl xIvl;
      bool b = Ivl::bExtract((char**)&aScr,&xIvl,pColumn);
      ASSERT(b);
      iHash += XXH32(&xIvl,sizeof(Ivl),0);
      break;
    }
      
    case CT_TIME: {
      Tme xTme;
      bool b = Tme::bExtract((char**)&aScr,&xTme,pColumn);
      ASSERT(b);
      iHash += XXH32(&xTme,sizeof(Tme),0);
      break;
    }
      
    case CT_TIMESTAMP: {
      Tsp xTsp;
      bool b = Tsp::bExtract((char**)&aScr,&xTsp,pColumn);
      ASSERT(b);
      iHash += XXH32(&xTsp,sizeof(Tsp),0);
      break;
    }
      
    case CT_VARBINARY: {
      ASSERT(0);
      break;
    }
      
    case CT_VARCHAR: {
      iHash += XXH32((char*)aScr,StrLib::iSize((char*)aScr),0);
      break;
    }
      
    default:
      ASSERT(0);
  }
  
}

/*----------------------------------------------------------------------------*/
/*-- Sprint ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* s-print                                                                    */
/*----------------------------------------------------------------------------*/
void DtmLib::vSprint(eDM eDatum, Str* aDst, Str* aSrc, i8& nBytesTaken,
                     DBobjects::Column* pColumn)
{
  switch (eDatum) {
    case DM_BCD: Bcd::vSprint(aDst,aSrc,nBytesTaken); break;
    case DM_BOL: Bol::vSprint(aDst,aSrc,nBytesTaken); break;
    case DM_CHR: ASSERT(0);                           break;
    case DM_DTE: Dte::vSprint(aDst,aSrc,nBytesTaken); break;
    case DM_FLT: Flt::vSprint(aDst,aSrc,nBytesTaken); break;
    case DM_INT: Int::vSprint(aDst,aSrc,nBytesTaken); break;
    case DM_IVL: Ivl::vSprint(aDst,aSrc,nBytesTaken); break;
    case DM_TME: Tme::vSprint(aDst,aSrc,nBytesTaken); break;
    case DM_TSP: Tsp::vSprint(aDst,aSrc,nBytesTaken); break;
    case DM_VCH: Vch::vSprint(aDst,aSrc,nBytesTaken); break;
    default:
      vFatal("DtmLib::vSprint: Invalid aDatum argument: %lld\n"_Str,
             eDatum);
  }
}

/*----------------------------------------------------------------------------*/
/*-- Validation --------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* valid datum?                                                               */
/*----------------------------------------------------------------------------*/
bool DtmLib::bValidate(eDM eDatum, char** aaSrc, Column* pColumn)
{
  bool b = false;
  
  switch (eDatum) {
    case DM_BCD: b = Bcd::bValid(aaSrc,pColumn); break;
    case DM_BOL: b = Bol::bValid(aaSrc,pColumn); break;
    case DM_CHR: b = Vch::bValid(aaSrc,pColumn); break;
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
