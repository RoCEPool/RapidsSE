/*============================================================================*/
/* Datum.cpp -- Datum                                                         */
/*============================================================================*/

#include "Data.h"

#include "StorageEngine.h"

extern AttachMap* gpAttachMap;

/*----------------------------------------------------------------------------*/
/*-- LifeCycle ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* safe cast                                                                  */
/*----------------------------------------------------------------------------*/
Datum* Data::_pDatum(void* aAddr)
{
  Datum* pDatum = reinterpret_cast<Datum*>(aAddr);
  
  return pDatum;
}

/*----------------------------------------------------------------------------*/
/* operator new                                                               */
/*----------------------------------------------------------------------------*/
void* Datum::operator new(size_t)
{
  void* pObject = HeapMem::pAllocate(CN_DATUM);
  
  return pObject;
}

/*----------------------------------------------------------------------------*/
/* operator delete                                                            */
/*----------------------------------------------------------------------------*/
void Datum::operator delete(void* aAddr)
{
  Datum* pDatum = reinterpret_cast<Datum*>(aAddr);
  
  HeapMem::vFree(pDatum);
}


/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
Datum::Datum(void)
: Object(CN_DATUM)
{
  _precondition_(_MAGIC_,CN_DATUM,_FOOTER_);
  
  m_eDatum = DM_NONE;
  m_wWord  = C_UNKNOWN;
#ifdef WITH_CHECK_INITIALISED
  m_xWsp.vZero(CN_DATUM);
#endif
  m_nChars = 0;

  m_eEncoding = MN_NULL;
  m_nEncodedBytes = 0;
  
  _checkInitialised_(CN_DATUM);
  
  _postcondition_(_MAGIC_,CN_DATUM,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
Datum::~Datum(void) noexcept(false)
{
}

/*----------------------------------------------------------------------------*/
/*-- Public ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* test null                                                                   */
/*----------------------------------------------------------------------------*/
bool Datum::bIsNull(void)
{
  _precondition_(_MAGIC_,CN_DATUM,_FOOTER_);

  if (m_iInt == C_DATUM_NULL)
    return true;
  else
    return false;
}

/*----------------------------------------------------------------------------*/
/* set null                                                                   */
/*----------------------------------------------------------------------------*/
void Datum::vSetNull(void)
{
  _precondition_(_MAGIC_,CN_DATUM,_FOOTER_);

  m_iInt = C_DATUM_NULL;
}

/*----------------------------------------------------------------------------*/
/* sprint into szDst                                                          */
/*----------------------------------------------------------------------------*/
void Datum::vSprint(Stz* szDst, Column* pCol)
{
  _precondition_(_MAGIC_,CN_DATUM,_FOOTER_);
  
  switch (m_eDatum) {
    case DM_BCD: vSprintBcd(szDst,pCol); return;
    case DM_BOL: vSprintBol(szDst,pCol); return;
    case DM_DTE: vSprintDte(szDst,pCol); return;
    case DM_FLT: vSprintFlt(szDst,pCol); return;
    case DM_INT: vSprintInt(szDst,pCol); return;
    case DM_IVL: vSprintIvl(szDst,pCol); return;
    case DM_TME: vSprintTme(szDst,pCol); return;
    case DM_TSP: vSprintTsp(szDst,pCol); return;
    case DM_VCH: vSprintVch(szDst,pCol); return;
    default:
      NEVER_REACH;
  }
}

/*----------------------------------------------------------------------------*/
/* sprint into szDst                                                          */
/*----------------------------------------------------------------------------*/
void Datum::vSprintBcd(Stz* szDst, Column* pCol)
{
  _precondition_(_MAGIC_,CN_DATUM,_FOOTER_);
  
  m_wBcd.vSprint(szDst,pCol);
}

/*----------------------------------------------------------------------------*/
/* sprint into szDst                                                          */
/*----------------------------------------------------------------------------*/
void Datum::vSprintBol(Stz* szDst, Column* pCol)
{
  _precondition_(_MAGIC_,CN_DATUM,_FOOTER_);
  
  m_wBol.vSprint(szDst);
}

/*----------------------------------------------------------------------------*/
/* sprint into szDst                                                          */
/*----------------------------------------------------------------------------*/
void Datum::vSprintDte(Stz* szDst, Column* pCol)
{
  _precondition_(_MAGIC_,CN_DATUM,_FOOTER_);
  
  m_wDte.vSprint(szDst);
}

/*----------------------------------------------------------------------------*/
/* sprint into szDst                                                          */
/*----------------------------------------------------------------------------*/
void Datum::vSprintFlt(Stz* szDst, Column* pCol)
{
  _precondition_(_MAGIC_,CN_DATUM,_FOOTER_);
  
  m_wFlt.vSprint(szDst);
}

/*----------------------------------------------------------------------------*/
/* sprint into szDst                                                          */
/*----------------------------------------------------------------------------*/
void Datum::vSprintInt(Stz* szDst, Column* pCol)
{
  _precondition_(_MAGIC_,CN_DATUM,_FOOTER_);
  
  m_wInt.vSprint(szDst);
}

/*----------------------------------------------------------------------------*/
/* sprint into szDst                                                          */
/*----------------------------------------------------------------------------*/
void Datum::vSprintIvl(Stz* szDst, Column* pCol)
{
  _precondition_(_MAGIC_,CN_DATUM,_FOOTER_);
  
  m_wIvl.vSprint(szDst);
}

/*----------------------------------------------------------------------------*/
/* sprint into szDst                                                          */
/*----------------------------------------------------------------------------*/
void Datum::vSprintTme(Stz* szDst, Column* pCol)
{
  _precondition_(_MAGIC_,CN_DATUM,_FOOTER_);
  
  m_wTme.vSprint(szDst);
}

/*----------------------------------------------------------------------------*/
/* sprint into szDst                                                          */
/*----------------------------------------------------------------------------*/
void Datum::vSprintTsp(Stz* szDst, Column* pCol)
{
  _precondition_(_MAGIC_,CN_DATUM,_FOOTER_);
  
  m_wTsp.vSprint(szDst);
}

/*----------------------------------------------------------------------------*/
/* sprint into szDst                                                          */
/*----------------------------------------------------------------------------*/
void Datum::vSprintVch(Stz* szDst, Column* pCol)
{
  _precondition_(_MAGIC_,CN_DATUM,_FOOTER_);
  
  if (bIsNull()) {
    szDst->vSprint("null"_Stz);
    return;
  }
  
  Vch::vSprintQuotes(szDst,m_xWsp.aStz());
}

/*----------------------------------------------------------------------------*/
/*-- Footer ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* get location of footer                                                     */
/*----------------------------------------------------------------------------*/
Footer* Datum::aFooter(void)
{
  _precondition_(_MAGIC_,CN_DATUM);
  
  return &m_xFooter;
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
