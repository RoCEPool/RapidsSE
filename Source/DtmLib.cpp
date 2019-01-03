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
/* copy datum (return the number of bytes copied)                             */
/*----------------------------------------------------------------------------*/
i8 DtmLib::nCopyVLQ(eDM eDatum, void* pDst, void* pSrc)
{
  switch (eDatum) {
      
    case DM_BCD: return Bcd::nCopyVLQ(pDst,pSrc);
    case DM_BOL: return Bol::nCopy(pDst,pSrc);
    case DM_DTE: return Dte::nCopy(pDst,pSrc);
    case DM_FLT: return Flt::nCopy(pDst,pSrc);
    case DM_INT: return Int::nCopyVLQ(pDst,pSrc);
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
/*-- Extract -----------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* extract                                                                    */
/*----------------------------------------------------------------------------*/
bool DtmLib::bExtract(eDM eDatum, Stz* aDst, char** aaSrc,
                      DBobjects::Column* pColumn, bool bLastColumn,
                      bool bAllowNonPrintable, Formatters* pFormatters)
{
//  bool b = false;
//
//  switch (eDatum) {
//    case DM_BCD: b = Bcd::bExtract(aaSrc,aDst,pColumn,pFormatters); break;
//    case DM_BOL: b = Bol::bExtract(aaSrc,aDst,pColumn,pFormatters); break;
//    case DM_CHR: b = Vch::bExtract(aaSrc,aDst,pColumn,pFormatters); break;
//    case DM_DTE: b = Dte::bExtract(aaSrc,aDst,pColumn,pFormatters); break;
//    case DM_FLT: b = Flt::bExtract(aaSrc,aDst,pColumn,pFormatters); break;
//    case DM_INT: b = Int::bExtract(aaSrc,aDst,pColumn,pFormatters); break;
//    case DM_IVL: b = Ivl::bExtract(aaSrc,aDst,pColumn,pFormatters); break;
//    case DM_TME: b = Tme::bExtract(aaSrc,aDst,pColumn,pFormatters); break;
//    case DM_TSP: b = Tsp::bExtract(aaSrc,aDst,pColumn,pFormatters); break;
//    case DM_VCH: b = Vch::bExtract2(aaSrc,aDst,pColumn,pFormatters,bAllowNonPrintable); break;
//    default:
//      vFatal("DtmLib::bValid: Invalid aDatum argument: %lld\n"_Stz,
//             eDatum);
//  }
  
  // invoke the dataum types extract function
  bool b = pColumn->bExtract(aaSrc,aDst,pFormatters);
  
  // move past the field delimiter
  //   (but not past the record terminator or record ends)
  
  if (bLastColumn) {
    // do not expect to see a field delimiter
    if ( pFormatters->m_cFieldDelimiter &&
         pFormatters->m_cRecordTerminator &&
         pFormatters->m_cFieldDelimiter != pFormatters->m_cRecordTerminator &&
         *(*aaSrc) == pFormatters->m_cFieldDelimiter ) {
      // saw a field delimiter after last field (incorrect)
      b = false;
    }
    /*
    // did not see a field delimiter (correct)
    if ( pFormatters->m_cFieldDelimiter &&
        *(*aaSrc) == pFormatters->m_cFieldDelimiter )
      (*aaSrc)++;
    */
  }
  else {
    // expect to see a field delimiter
    if ( pFormatters->m_cFieldDelimiter &&
        *(*aaSrc) == pFormatters->m_cFieldDelimiter )
      // saw a field delimiter after non-last field (correct)
      (*aaSrc)++;
    else
      // did not see a field delimiter after non-last field (incorrect)
      b = false;
  }
  
  return b;
}

/*----------------------------------------------------------------------------*/
/*-- Hash --------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* hash                                                                       */
/*----------------------------------------------------------------------------*/
void DtmLib::vHash(eDM eDatum, i8& iHash, Stz* aScr)
{
  switch (eDatum) {
      
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
/* s-print                                                                    */
/*----------------------------------------------------------------------------*/
void DtmLib::vSprintVLQ(eDM eDatum, Stz* aDst, Stz* aSrc,
                     DBobjects::Column* pColumn)
{
  switch (eDatum) {
    case DM_BCD: Bcd::vSprintVLQ(aDst,aSrc,pColumn); break;
    case DM_BOL: Bol::vSprint(aDst,aSrc); break;
    case DM_CHR: Vch::vSprint(aDst,aSrc); break;
    case DM_DTE: Dte::vSprint(aDst,aSrc); break;
    case DM_FLT: Flt::vSprint(aDst,aSrc); break;
    case DM_INT: Int::vSprintVLQ(aDst,aSrc); break;
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
/* s-print                                                                    */
/*----------------------------------------------------------------------------*/
void DtmLib::vSprintQuotes(eDM eDatum, Stz* aDst, Stz* aSrc,
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
    case DM_VCH: Vch::vSprintQuotes(aDst,aSrc); break;
    default:
      vFatal("DtmLib::vSprint: Invalid aDatum argument: %lld\n"_Stz,
             eDatum);
  }
}

/*----------------------------------------------------------------------------*/
/* s-print                                                                    */
/*----------------------------------------------------------------------------*/
void DtmLib::vSprintQuotesVLQ(eDM eDatum, Stz* aDst, Stz* aSrc,
                     DBobjects::Column* pColumn)
{
  switch (eDatum) {
    case DM_BCD: Bcd::vSprintVLQ(aDst,aSrc,pColumn); break;
    case DM_BOL: Bol::vSprint(aDst,aSrc); break;
    case DM_CHR: Vch::vSprint(aDst,aSrc); break;
    case DM_DTE: Dte::vSprint(aDst,aSrc); break;
    case DM_FLT: Flt::vSprint(aDst,aSrc); break;
    case DM_INT: Int::vSprintVLQ(aDst,aSrc); break;
    case DM_IVL: Ivl::vSprint(aDst,aSrc); break;
    case DM_TME: Tme::vSprint(aDst,aSrc); break;
    case DM_TSP: Tsp::vSprint(aDst,aSrc); break;
    case DM_VCH: Vch::vSprintQuotes(aDst,aSrc); break;
    default:
      vFatal("DtmLib::vSprint: Invalid aDatum argument: %lld\n"_Stz,
             eDatum);
  }
}


/*----------------------------------------------------------------------------*/
/* s-print escaped                                                            */
/*----------------------------------------------------------------------------*/
void DtmLib::vSprintEscaped(eDM eDatum, Stz* aDst, Stz* aSrc,
                     DBobjects::Column* pColumn)
{
  switch (eDatum) {
    case DM_BCD: Bcd::vSprint(aDst,aSrc,pColumn); break;
    case DM_BOL: Bol::vSprint(aDst,aSrc);         break;
    case DM_CHR: Vch::vSprint(aDst,aSrc);         break;
    case DM_DTE: Dte::vSprintEscaped(aDst,aSrc);  break;
    case DM_FLT: Flt::vSprint(aDst,aSrc);         break;
    case DM_INT: Int::vSprint(aDst,aSrc);         break;
    case DM_IVL: Ivl::vSprint(aDst,aSrc);         break;
    case DM_TME: Tme::vSprintEscaped(aDst,aSrc);  break;
    case DM_TSP: Tsp::vSprintEscaped(aDst,aSrc);  break;
    case DM_VCH: Vch::vSprintEscaped(aDst,aSrc);  break;
    default:
      vFatal("DtmLib::vSprint: Invalid aDatum argument: %lld\n"_Stz,
             eDatum);
  }
}

/*----------------------------------------------------------------------------*/
/* s-print escaped                                                            */
/*----------------------------------------------------------------------------*/
void DtmLib::vSprintEscapedVLQ(eDM eDatum, Stz* aDst, Stz* aSrc,
                            DBobjects::Column* pColumn)
{
  switch (eDatum) {
    case DM_BCD: Bcd::vSprintVLQ(aDst,aSrc,pColumn); break;
    case DM_BOL: Bol::vSprint(aDst,aSrc);         break;
    case DM_CHR: Vch::vSprint(aDst,aSrc);         break;
    case DM_DTE: Dte::vSprintEscaped(aDst,aSrc);  break;
    case DM_FLT: Flt::vSprint(aDst,aSrc);         break;
    case DM_INT: Int::vSprintVLQ(aDst,aSrc);         break;
    case DM_IVL: Ivl::vSprint(aDst,aSrc);         break;
    case DM_TME: Tme::vSprintEscaped(aDst,aSrc);  break;
    case DM_TSP: Tsp::vSprintEscaped(aDst,aSrc);  break;
    case DM_VCH: Vch::vSprintEscaped(aDst,aSrc);  break;
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

/*----------------------------------------------------------------------------*/
/* cmp datum argA with argB (-ve: A < B, 0: A == B, +ve: A > B)               */
/*----------------------------------------------------------------------------*/
i8 DtmLib::iCmp(eDM eDatum, char* aArgA, char* aArgB)
{
  i8 iCmp = 0;
  
  switch (eDatum) {
    case DM_BCD: iCmp = Bcd::iCmp(aArgA,aArgB); break;
    case DM_BOL: iCmp = Bol::iCmp(aArgA,aArgB); break;
    case DM_CHR: iCmp = Vch::iCmp(aArgA,aArgB); break;
    case DM_DTE: iCmp = Dte::iCmp(aArgA,aArgB); break;
    case DM_FLT: iCmp = Flt::iCmp(aArgA,aArgB); break;
    case DM_INT: iCmp = Int::iCmp(aArgA,aArgB); break;
    case DM_IVL: iCmp = Ivl::iCmp(aArgA,aArgB); break;
    case DM_TME: iCmp = Tme::iCmp(aArgA,aArgB); break;
    case DM_TSP: iCmp = Tsp::iCmp(aArgA,aArgB); break;
    case DM_VBN: NEVER_REACH; break;
    case DM_VCH: iCmp = Vch::iCmp(aArgA,aArgB); break;
      
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
/* is the string enclosed?                                                    */
/*----------------------------------------------------------------------------*/
bool DtmLib::bIsEnclosed(char** aaSrc, Formatters* pFormatters)
{
  if (! pFormatters->m_cEnclosedBy_Leading)
    return false;
  
  char *aStart = *aaSrc;
  
  // find first non-space character
  while (' ' == **aaSrc)
    (*aaSrc)++;
  
  // is it an enclosed varchar?
  if (**aaSrc == pFormatters->m_cEnclosedBy_Leading) {
    (*aaSrc)++;
    return true;
  }
  else {
    *(aaSrc) = aStart;
    return false;
  }
}

/*----------------------------------------------------------------------------*/
/* is there data?                                                             */
/*----------------------------------------------------------------------------*/
bool DtmLib::bIsThereFieldData(char* aSrc, Formatters* pFormatters)
{
  if ( ! *aSrc
       || *aSrc == pFormatters->m_cFieldDelimiter
       || *aSrc == C_NL
       || *aSrc == C_CR
      )
    return false;
  else
    return true;
}

/*----------------------------------------------------------------------------*/
/* is there data?                                                             */
/*----------------------------------------------------------------------------*/
bool DtmLib::bIsThereRecordData(char* aSrc, Formatters* pFormatters)
{
  if ( ! *aSrc
      || *aSrc == C_NL
      || *aSrc == C_CR
      )
    return false;
  else
    return true;
}

/*----------------------------------------------------------------------------*/
/* test for null token                                                        */
/*----------------------------------------------------------------------------*/
bool DtmLib::bTestForNull(char** aaSrc, DBobjects::Column* pColumn,
                          Formatters* pFormatters, bool& bValidNull,
                          bool bSkipTrailingSpaces)
{
  /*
   * This function scans aaSrc and attempts to identify the text form of a null. 
   * There are three text forms of null: null, \N or '\N'.
   * If a null (and only a null) is detected and 
   *  a) the column is nullable, then bValidNull is set to true and the
   *     function returns true; or
   *  b) the column is not nullable, then bValidNull is set to false and the
   *     function returns true.
   * If no form of null is detected then bValidNull is set to false and the
   * function returns false.
   * Importantly a valid null is one of the three forms of null -- there can be
   * no further characters after it. Eg: "nulla" or "null a" are not
   * correct forms of null.
   */
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // test if there is any field data
 
  if ( ! DtmLib::bIsThereFieldData(*aaSrc, pFormatters)) {
    if ( ! pColumn->bIsNullable() ) {
      // a null was detected in a non-nullable column
      bValidNull = false;
      return true;
    }
    // a null was detected in a nullable column
    bValidNull = true;
    return true;
  }
 
  //if ( ! DtmLib::bIsThereFieldData(*aaSrc, pFormatters))
  //  return false;
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // test for null, \N, '\N' or [\N]
  
  char* aStart = *aaSrc;
  
  if ( (**aaSrc == 'N' || **aaSrc == 'n') && (0 == StrLib::iCompare(*aaSrc+1, "ULL", 3, C_NO_CASE)) ) {
    // skip over token null
    *aaSrc = (*aaSrc) + 4;
  }
  else if (**aaSrc     == pFormatters->m_cEscapeChar &&
           *(*aaSrc+1) == 'N') {
    // skip over \N
    *aaSrc = (*aaSrc) + 2;
  }
  else if (**aaSrc     == pFormatters->m_cEnclosedBy_Leading &&
           *(*aaSrc+1) == pFormatters->m_cEscapeChar &&
           *(*aaSrc+2) == 'N'                        &&
           *(*aaSrc+3) == pFormatters->m_cEnclosedBy_Trailing) {
    // skip over '\N' or [\N]
    *aaSrc = (*aaSrc) + 4;
  }
  else if (**aaSrc    == pFormatters->m_cEnclosedBy_Leading &&
           ((*(*aaSrc+1) == 'N' || *(*aaSrc+1) == 'n') && (0 == StrLib::iCompare(*aaSrc+2, "ULL", 3, C_NO_CASE))) &&
          *(*aaSrc+5) == pFormatters->m_cEnclosedBy_Trailing) {
    // skip over 'NULL'
    *aaSrc = (*aaSrc) + 6;
  }
  else {
    // no null detected
    bValidNull = false; // no null, so neither valid or invalid
    return false;
  }
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // tail processing
  
  // optionally skip trailing spaces
  if (bSkipTrailingSpaces)
    StrLib::vSkipSpaces(aaSrc);
  
  // check it was "null" only
  if (! bValidFieldEnd(*aaSrc,pFormatters)) {
    // no well formed null detected
    *(aaSrc) = aStart;
    bValidNull = false; // no null, so neither valid or invalid
    return false;
  }
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // check the column is nullable
  
  if ( ! pColumn->bIsNullable() ) {
    // a null was detected in a non-nullable column
    bValidNull = false;
    return true;
  }
  
  // a null was detected in a nullable column
  bValidNull = true;
  return true;
}

/*----------------------------------------------------------------------------*/
/* test for null token                                                        */
/*----------------------------------------------------------------------------*/
bool DtmLib::bTestForNullDefault(char** aaSrc, DBobjects::Column* pColumn,
                          Formatters* pFormatters, bool& bValidNull)
{
  /*
   * This function scans aaSrc and attempts to identify the text form of a null.
   * There are three text forms of null: null, \N or '\N'.
   * If a null (and only a null) is detected and
   *  a) the column is nullable, then bValidNull is set to true and the
   *     function returns true; or
   *  b) the column is not nullable, then bValidNull is set to false and the
   *     function returns true.
   * If no form of null is detected then bValidNull is set to false and the
   * function returns false.
   * Importantly a valid null is one of the three forms of null -- there can be
   * no further characters after it. Eg: "nulla" or "null a" are not
   * correct forms of null.
   */
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // test if there is any field data
  
  if ( ! DtmLib::bIsThereFieldData(*aaSrc, pFormatters)) {
    if ( ! pColumn->bIsNullable() ) {
      // a null was detected in a non-nullable column
      bValidNull = false;
      return true;
    }
    // a null was detected in a nullable column
    bValidNull = true;
    return true;
  }
  
  //if ( ! DtmLib::bIsThereFieldData(*aaSrc, pFormatters))
  //  return false;
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // test for null, \N, '\N' or [\N]
  
  char* aStart = *aaSrc;
  
  if ( (**aaSrc == 'N' || **aaSrc == 'n') && (0 == StrLib::iCompare(*aaSrc+1, "ULL", 3, C_NO_CASE)) ) {
    // skip over token null
    *aaSrc = (*aaSrc) + 4;
  }
  else if (**aaSrc     == pFormatters->m_cEscapeChar &&
           *(*aaSrc+1) == 'N') {
    // skip over \N
    *aaSrc = (*aaSrc) + 2;
  }
  else {
    // no null detected
    bValidNull = false; // no null, so neither valid or invalid
    return false;
  }
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // tail processing
  
  // check it was "null" only
  if (! bValidFieldEnd(*aaSrc,pFormatters)) {
    // no well formed null detected
    *(aaSrc) = aStart;
    bValidNull = false; // no null, so neither valid or invalid
    return false;
  }
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // check the column is nullable
  
  if ( ! pColumn->bIsNullable() ) {
    // a null was detected in a non-nullable column
    bValidNull = false;
    return true;
  }
  
  // a null was detected in a nullable column
  bValidNull = true;
  return true;
}

/*----------------------------------------------------------------------------*/
/* test for valid field end                                                   */
/*----------------------------------------------------------------------------*/
bool DtmLib::bValidFieldEnd(char* aSrc, Formatters* pFormatters)
{
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // test for end of data
  
  // valid field end if we've run out of data
  if (! *aSrc)
    return true;
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // test for explicit field separator
  
  // valid field end if we've encounter the field delimiter
  if (*aSrc == pFormatters->m_cFieldDelimiter)
    return true;
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // test for explicit record terminator
  
  if (pFormatters->m_cRecordTerminator) {
    
    // not a valid field end if expected record terminator is not seen
    if (*aSrc != pFormatters->m_cRecordTerminator)
      return false;

    // test we see the record end after the record termintor
    bool bValidEnd = bValidRecordEnd(aSrc+1,pFormatters);
    
    return bValidEnd;
  }
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // test for explicit record ends

  bool bValidEnd = bValidRecordEnd(aSrc,pFormatters);
  
  return bValidEnd;
}

/*----------------------------------------------------------------------------*/
/* test for valid record end                                                  */
/*----------------------------------------------------------------------------*/
bool DtmLib::bValidRecordEnd(char* aSrc, Formatters* pFormatters)
{
  switch (pFormatters->m_eRecordEnd) {
      
    case RE_NL: // test for \n
      if (C_NL == *aSrc)
        return true;
      else
        return false;
      
    case RE_CR: // test for \r
      if (C_CR == *aSrc)
        return true;
      else
        return false;
      
    case RE_CRNL: // test for \r\n
      if (C_CR == *aSrc && C_NL == *(aSrc+1))
        return true;
      else
        return false;
      
    default: // otherwise not a field end
      NEVER_REACH;
      return false;
  }
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
