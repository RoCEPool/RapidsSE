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
/*-- Extract -----------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

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
      iHash += HshLib::iHash(&xBol,sizeof(Bol));
      break;
    }
      
    case DM_DTE: {
      Dte xDte = *((Dte*)aScr);
      iHash += HshLib::iHash(&xDte,sizeof(Dte));
      break;
    }
      
    case DM_BCD: {
      Bcd xBcd = *((Bcd*)aScr);;
      iHash += HshLib::iHash(&xBcd,sizeof(Bcd));
      break;
    }
      
    case DM_FLT: {
      Flt xFlt = *((Flt*)aScr);
      //bool b = Flt::bExtract((char*)aScr,&xFlt,iLength,pColumn);
      //ASSERT(b);
      iHash += HshLib::iHash(&xFlt,sizeof(Flt));
      break;
    }
      
    case DM_INT: {
      Int xInt = *((Int*)aScr);
      iHash += HshLib::iHash(&xInt,sizeof(Int));
      break;
    }
      
    case DM_IVL: {
      Ivl xIvl = *((Ivl*)aScr);
      iHash += HshLib::iHash(&xIvl,sizeof(Ivl));
      break;
    }
      
    case DM_TME: {
      Tme xTme = *((Tme*)aScr);
      iHash += HshLib::iHash(&xTme,sizeof(Tme));
      break;
    }
      
    case DM_TSP: {
      Tsp xTsp = *((Tsp*)aScr);
      iHash += HshLib::iHash(&xTsp,sizeof(Tsp));
      break;
    }
      
    case DM_VBN: {
      NEVER_REACH;
      break;
    }
      
    case DM_VCH: {
      iHash += HshLib::iHash((char*)aScr,StrLib::iSize((char*)aScr));
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
void DtmLib::vSprint(MetaData::Column* pColumn, Stz* aDst, Stz* aSrc)
{
  eDM eDatum = pColumn->eDatumType();
  
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
void DtmLib::vSprintQuotes(MetaData::Column* pColumn, Stz* aDst, Stz* aSrc)
{
  eDM eDatum = pColumn->eDatumType();
  
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
/* s-print escaped                                                            */
/*----------------------------------------------------------------------------*/
void DtmLib::vSprintEscaped(MetaData::Column* pColumn, Stz* aDst, Stz* aSrc)
{
  eDM eDatum = pColumn->eDatumType();
  
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

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
