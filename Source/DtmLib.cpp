/*============================================================================*/
/* DtmLib.cpp -- Datum Library                                                */
/*============================================================================*/

#include "Datums.h"

#include "StorageEngine.h"

/*----------------------------------------------------------------------------*/
/* copy datum (return the number of bytes copied)                             */
/*----------------------------------------------------------------------------*/
i8 DtmLib::nCopy(eDM eDatum, void* pDst, void* pSrc, DBobjects::Column* pColumn)
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
void DtmLib::vCopy(eDM eDatum, void* aDst, void* aSrc, DBobjects::Column* pColumn)
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
/*-- Extract -----------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* extract                                                                    */
/*----------------------------------------------------------------------------*/
bool DtmLib::bExtract(eDM eDatum, Stz* aDst, char** aaSrc,
                      DBobjects::Column* pColumn, Formatters* pFormatters)
{
  bool b = false;
  
  switch (eDatum) {
    case DM_BCD: b = Bcd::bExtract(aaSrc,(Bcd*)aDst,pColumn,pFormatters); break;
    case DM_BOL: b = Bol::bExtract(aaSrc,(Bol*)aDst,pColumn,pFormatters); break;
    case DM_CHR: b = Vch::bExtract(aaSrc,(Vch*)aDst,pColumn,pFormatters); break;                                
    case DM_DTE: b = Dte::bExtract(aaSrc,(Dte*)aDst,pColumn,pFormatters); break;
    case DM_FLT: b = Flt::bExtract(aaSrc,(Flt*)aDst,pColumn,pFormatters); break;
    case DM_INT: b = Int::bExtract(aaSrc,(Int*)aDst,pColumn,pFormatters); break;
    case DM_IVL: b = Ivl::bExtract(aaSrc,(Ivl*)aDst,pColumn,pFormatters); break;
    case DM_TME: b = Tme::bExtract(aaSrc,(Tme*)aDst,pColumn,pFormatters); break;
    case DM_TSP: b = Tsp::bExtract(aaSrc,(Tsp*)aDst,pColumn,pFormatters); break;
    case DM_VCH: b = Vch::bExtract(aaSrc,(Vch*)aDst,pColumn,pFormatters); break;
    default:
      vFatal("DtmLib::bValid: Invalid aDatum argument: %lld\n"_Stz,
             eDatum);
  }
  
  // move aSrc to after the C_FS but not past the C_NL or C_CR
  if ( *(*aaSrc) == C_FS )
    (*aaSrc)++;
  
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
void DtmLib::vHash(eDM eDatum, i8& iHash, Stz* aScr, DBobjects::Column* pColumn)
{
  switch (pColumn->eDatumType()) {
      
    case DM_BOL: {
      Bol xBol = *((Bol*)aScr);
      iHash += XXH32(&xBol,sizeof(Bol),0);
      break;
    }
      
    case DM_DTE: {
      Dte xDte = *((Dte*)aScr);
      iHash += XXH32(&xDte,sizeof(Dte),0);
      break;
    }
      
    case DM_BCD: {
      Bcd xBcd = *((Bcd*)aScr);;
      iHash += XXH32(&xBcd,sizeof(Bcd),0);
      break;
    }
      
    case DM_FLT: {
      Flt xFlt = *((Flt*)aScr);
      //bool b = Flt::bExtract((char*)aScr,&xFlt,iLength,pColumn);
      //ASSERT(b);
      iHash += XXH32(&xFlt,sizeof(Flt),0);
      break;
    }
      
    case DM_INT: {
      Int xInt = *((Int*)aScr);
      iHash += XXH32(&xInt,sizeof(Int),0);
      break;
    }
      
    case DM_IVL: {
      Ivl xIvl = *((Ivl*)aScr);
      iHash += XXH32(&xIvl,sizeof(Ivl),0);
      break;
    }
      
    case DM_TME: {
      Tme xTme = *((Tme*)aScr);
      iHash += XXH32(&xTme,sizeof(Tme),0);
      break;
    }
      
    case DM_TSP: {
      Tsp xTsp = *((Tsp*)aScr);
      iHash += XXH32(&xTsp,sizeof(Tsp),0);
      break;
    }
      
    case DM_VBN: {
      NEVER_REACH;
      break;
    }
      
    case DM_VCH: {
      iHash += XXH32((char*)aScr,StrLib::iSize((char*)aScr),0);
      break;
    }
      
    default:
      NEVER_REACH;
  }
  
}

/*----------------------------------------------------------------------------*/
/*-- Sprint ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* s-print                                                                    */
/*----------------------------------------------------------------------------*/
void DtmLib::vSprint(eDM eDatum, Stz* aDst, Stz* aSrc,
                     DBobjects::Column* pColumn)
{
  switch (eDatum) {
    case DM_BCD: Bcd::vSprint(aDst,aSrc,pColumn); break;
    case DM_BOL: Bol::vSprint(aDst,aSrc); break;
    case DM_CHR: Vch::vSprint(aDst,aSrc); break;
    case DM_DTE: Dte::vSprint(aDst,aSrc); break;
    case DM_FLT: Flt::vSprint(aDst,aSrc); break;
    case DM_INT: Int::vSprint(aDst,aSrc); break;
    case DM_IVL: Ivl::vSprint(aDst,aSrc); break;
    case DM_TME: Tme::vSprint(aDst,aSrc); break;
    case DM_TSP: Tsp::vSprint(aDst,aSrc); break;
    case DM_VCH: Vch::vSprint(aDst,aSrc); break;
    default:
      vFatal("DtmLib::vSprint: Invalid aDatum argument: %lld\n"_Stz,
             eDatum);
  }
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
    case DM_CHR: bLT = Vch::bLessThan(aArgA,nBytesA,aArgB,nBytesB); break;
    case DM_DTE: bLT = Dte::bLessThan(aArgA,nBytesA,aArgB,nBytesB); break;
    case DM_FLT: bLT = Flt::bLessThan(aArgA,nBytesA,aArgB,nBytesB); break;
    case DM_INT: bLT = Int::bLessThan(aArgA,nBytesA,aArgB,nBytesB); break;
    case DM_IVL: bLT = Ivl::bLessThan(aArgA,nBytesA,aArgB,nBytesB); break;
    case DM_TME: bLT = Tme::bLessThan(aArgA,nBytesA,aArgB,nBytesB); break;
    case DM_TSP: bLT = Tsp::bLessThan(aArgA,nBytesA,aArgB,nBytesB); break;
    case DM_VBN: NEVER_REACH; break;
    case DM_VCH: bLT = Vch::bLessThan(aArgA,nBytesA,aArgB,nBytesB); break;
      
    default:
      THROW("DtmLib::bLessThan: Invalid eDatum %lld"_Stz,eDatum);
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
    case DM_CHR: bLE = Vch::bLessEqual(aArgA,nBytesA,aArgB,nBytesB); break;
    case DM_DTE: bLE = Dte::bLessEqual(aArgA,nBytesA,aArgB,nBytesB); break;
    case DM_FLT: bLE = Flt::bLessEqual(aArgA,nBytesA,aArgB,nBytesB); break;
    case DM_INT: bLE = Int::bLessEqual(aArgA,nBytesA,aArgB,nBytesB); break;
    case DM_IVL: bLE = Ivl::bLessEqual(aArgA,nBytesA,aArgB,nBytesB); break;
    case DM_TME: bLE = Tme::bLessEqual(aArgA,nBytesA,aArgB,nBytesB); break;
    case DM_TSP: bLE = Tsp::bLessEqual(aArgA,nBytesA,aArgB,nBytesB); break;
    case DM_VBN: NEVER_REACH; break;
    case DM_VCH: bLE = Vch::bLessEqual(aArgA,nBytesA,aArgB,nBytesB); break;
      
    default:
      THROW("DtmLib::bLessEqual: Invalid eDatum %lld"_Stz,eDatum);
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
    case DM_CHR: bEQ = Vch::bEqualTo(aArgA,nBytesA,aArgB,nBytesB); break;
    case DM_DTE: bEQ = Dte::bEqualTo(aArgA,nBytesA,aArgB,nBytesB); break;
    case DM_FLT: bEQ = Flt::bEqualTo(aArgA,nBytesA,aArgB,nBytesB); break;
    case DM_INT: bEQ = Int::bEqualTo(aArgA,nBytesA,aArgB,nBytesB); break;
    case DM_IVL: bEQ = Ivl::bEqualTo(aArgA,nBytesA,aArgB,nBytesB); break;
    case DM_TME: bEQ = Tme::bEqualTo(aArgA,nBytesA,aArgB,nBytesB); break;
    case DM_TSP: bEQ = Tsp::bEqualTo(aArgA,nBytesA,aArgB,nBytesB); break;
    case DM_VBN: NEVER_REACH; break;
    case DM_VCH: bEQ = Vch::bEqualTo(aArgA,nBytesA,aArgB,nBytesB); break;
      
    default:
      THROW("DtmLib::bEqualTo: Invalid eDatum %lld"_Stz,eDatum);
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
    case DM_CHR: bNE = Vch::bNotEqual(aArgA,nBytesA,aArgB,nBytesB); break;
    case DM_DTE: bNE = Dte::bNotEqual(aArgA,nBytesA,aArgB,nBytesB); break;
    case DM_FLT: bNE = Flt::bNotEqual(aArgA,nBytesA,aArgB,nBytesB); break;
    case DM_INT: bNE = Int::bNotEqual(aArgA,nBytesA,aArgB,nBytesB); break;
    case DM_IVL: bNE = Ivl::bNotEqual(aArgA,nBytesA,aArgB,nBytesB); break;
    case DM_TME: bNE = Tme::bNotEqual(aArgA,nBytesA,aArgB,nBytesB); break;
    case DM_TSP: bNE = Tsp::bNotEqual(aArgA,nBytesA,aArgB,nBytesB); break;
    case DM_VBN: NEVER_REACH; break;
    case DM_VCH: bNE = Vch::bNotEqual(aArgA,nBytesA,aArgB,nBytesB); break;
      
    default:
      THROW("DtmLib::bNotEqual: Invalid eDatum %lld"_Stz,eDatum);
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
    case DM_CHR: bGE = Vch::bGreaterEqual(aArgA,nBytesA,aArgB,nBytesB); break;
    case DM_DTE: bGE = Dte::bGreaterEqual(aArgA,nBytesA,aArgB,nBytesB); break;
    case DM_FLT: bGE = Flt::bGreaterEqual(aArgA,nBytesA,aArgB,nBytesB); break;
    case DM_INT: bGE = Int::bGreaterEqual(aArgA,nBytesA,aArgB,nBytesB); break;
    case DM_IVL: bGE = Ivl::bGreaterEqual(aArgA,nBytesA,aArgB,nBytesB); break;
    case DM_TME: bGE = Tme::bGreaterEqual(aArgA,nBytesA,aArgB,nBytesB); break;
    case DM_TSP: bGE = Tsp::bGreaterEqual(aArgA,nBytesA,aArgB,nBytesB); break;
    case DM_VBN: NEVER_REACH; break;
    case DM_VCH: bGE = Vch::bGreaterEqual(aArgA,nBytesA,aArgB,nBytesB); break;
      
    default:
      THROW("DtmLib::bGreaterEqual: Invalid eDatum %lld"_Stz,eDatum);
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
    case DM_CHR: bGT = Vch::bGreaterThan(aArgA,nBytesA,aArgB,nBytesB); break;
    case DM_DTE: bGT = Dte::bGreaterThan(aArgA,nBytesA,aArgB,nBytesB); break;
    case DM_FLT: bGT = Flt::bGreaterThan(aArgA,nBytesA,aArgB,nBytesB); break;
    case DM_INT: bGT = Int::bGreaterThan(aArgA,nBytesA,aArgB,nBytesB); break;
    case DM_IVL: bGT = Ivl::bGreaterThan(aArgA,nBytesA,aArgB,nBytesB); break;
    case DM_TME: bGT = Tme::bGreaterThan(aArgA,nBytesA,aArgB,nBytesB); break;
    case DM_TSP: bGT = Tsp::bGreaterThan(aArgA,nBytesA,aArgB,nBytesB); break;
    case DM_VBN: NEVER_REACH; break;
    case DM_VCH: bGT = Vch::bGreaterThan(aArgA,nBytesA,aArgB,nBytesB); default:
      THROW("DtmLib::bGreaterThan: Invalid eDatum %lld"_Stz,eDatum);
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
    case DM_CHR: iCmp = Vch::iCmp(aArgA,nBytesA,aArgB,nBytesB); break;
    case DM_DTE: iCmp = Dte::iCmp(aArgA,nBytesA,aArgB,nBytesB); break;
    case DM_FLT: iCmp = Flt::iCmp(aArgA,nBytesA,aArgB,nBytesB); break;
    case DM_INT: iCmp = Int::iCmp(aArgA,nBytesA,aArgB,nBytesB); break;
    case DM_IVL: iCmp = Ivl::iCmp(aArgA,nBytesA,aArgB,nBytesB); break;
    case DM_TME: iCmp = Tme::iCmp(aArgA,nBytesA,aArgB,nBytesB); break;
    case DM_TSP: iCmp = Tsp::iCmp(aArgA,nBytesA,aArgB,nBytesB); break;
    case DM_VBN: NEVER_REACH; break;
    case DM_VCH: iCmp = Vch::iCmp(aArgA,nBytesA,aArgB,nBytesB); break;
      
    default:
      THROW("DtmLib::iCmp: Invalid eDatum %lld"_Stz,eDatum);
  }
  
  return iCmp;
}

/*----------------------------------------------------------------------------*/
/* cmp datum argA with argB (-ve: A < B, 0: A == B, +ve: A > B)               */
/*----------------------------------------------------------------------------*/
i8 DtmLib::iSize(eDM eDatum, char* aSrc)
{
  i8 iCmp = 0;
  
  switch (eDatum) {
    case DM_BCD: return sizeof(Bcd); break;
    case DM_BOL: return sizeof(Bol); break;
    case DM_CHR: return StrLib::iSize(aSrc); break;
    case DM_DTE: return sizeof(Dte); break;
    case DM_FLT: return sizeof(Flt); break;
    case DM_INT: return sizeof(Int); break;
    case DM_IVL: return sizeof(Ivl); break;
    case DM_TME: return sizeof(Tme); break;
    case DM_TSP: return sizeof(Tsp); break;
    case DM_VBN: NEVER_REACH; break;
    case DM_VCH: return Vch::iSize(aSrc); break;
      
    default:
      THROW("DtmLib::iCmp: Invalid eDatum %lld"_Stz,eDatum);
  }
  
  return iCmp;
}

/*----------------------------------------------------------------------------*/
/* is there data?                                                             */
/*----------------------------------------------------------------------------*/
bool DtmLib::bIsThereData(char cSrc, Formatters* pFormatters)
{
  if ( ! cSrc
       || cSrc == pFormatters->m_cFieldSeparator
       || cSrc == C_NL
       || cSrc == C_CR
      )
    return false;
  else
    return true;
}

/*----------------------------------------------------------------------------*/
/* test for null token                                                        */
/*----------------------------------------------------------------------------*/
//bool DtmLib::bTestForNull(char** aaSrc, DBobjects::Column* pColumn,
//                          Formatters* pFormatters, bool& bValid,
//                          bool bSkipTrailingSpaces)
//{
//  // do we see the token null
//  if (0 != StrLib::iCompare(*aaSrc, "NULL", 4, C_NO_CASE)) {
//    return false;
//  }
//  
//  // skip over token null
//  *aaSrc = (*aaSrc) + 4;
//  
//  // skip trailing spaces
//  if (bSkipTrailingSpaces)
//    StrLib::vSkipSpaces(aaSrc);
//  
//  // check it was "null" only
//  if (**aaSrc
//      && **aaSrc != pFormatters->m_cFieldSeparator
//      && **aaSrc != C_NL
//      && **aaSrc != C_CR) {
//    bValid = false;
//    
//    if ( bSkipTrailingSpaces )
//      return true;
//    else
//      return false;
//  }
//  
//  // check the column is nullable
//  if ( ! pColumn->bIsNullable() ) {
//    bValid = false;
//    return true;
//  }
//
//  bValid = true;
//  return true;
//}

bool DtmLib::bTestForNull(char** aaSrc, DBobjects::Column* pColumn,
                          Formatters* pFormatters, bool& bValid,
                          bool bSkipTrailingSpaces)
{
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // test for null, \N or '\N'
  
  if ( 0 == StrLib::iCompare(*aaSrc, "NULL", 4, C_NO_CASE) ) {
    // skip over token null
    *aaSrc = (*aaSrc) + 4;
  }
  else if (**aaSrc     == pFormatters->m_cEscapeChar &&
           *(*aaSrc+1) == 'N') {
    // skip over \N
    *aaSrc = (*aaSrc) + 2;
  }
  else if (**aaSrc     == pFormatters->m_cEnclosedBy &&
           *(*aaSrc+1) == pFormatters->m_cEscapeChar &&
           *(*aaSrc+2) == 'N'                        &&
           *(*aaSrc+3) == pFormatters->m_cEnclosedBy) {
    // skip over '\N'
    *aaSrc = (*aaSrc) + 4;
  }
  else {
    // no form of null
    return false;
  }
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // check the column is nullable
  
  if ( ! pColumn->bIsNullable() ) {
    bValid = false;
    return true;
  }
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // tail processing
  
  // skip trailing spaces
  if (bSkipTrailingSpaces)
    StrLib::vSkipSpaces(aaSrc);
  
  // check it was "null" only
  if (**aaSrc
      && **aaSrc != pFormatters->m_cFieldSeparator
      && **aaSrc != C_NL
      && **aaSrc != C_CR) {
    bValid = false;
    
    if ( bSkipTrailingSpaces )
      return true;
    else
      return false;
  }
  
  bValid = true;
  return true;
}

/*----------------------------------------------------------------------------*/
/* test for valid field end                                                   */
/*----------------------------------------------------------------------------*/
bool DtmLib::bValidFieldEnd(char** aaSrc, Formatters* pFormatters)
{
  // should see a FS, NL or CR
  if (**aaSrc
      && **aaSrc != pFormatters->m_cFieldSeparator
      && **aaSrc != C_NL
      && **aaSrc != C_CR)
    return false;
  else
    return true;
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
