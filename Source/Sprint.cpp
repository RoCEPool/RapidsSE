/*============================================================================*/
/* Sprint.cpp - Sprint Utility                                                */
/*============================================================================*/

#include "ScanSprint.h"

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
Sprint::Sprint(eCN eClass)
: SP(eClass)
{

}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
Sprint::~Sprint(void) noexcept(false)
{
  _precondition_(_MAGIC_,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/*-- Static ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* sprint into target                                                         */
/*----------------------------------------------------------------------------*/
void Sprint::vInto(Stz *szTgt, void *avSrc, Column *pCol)
{
  eDM eDatum = pCol->eDatumType();
  char* az   = (char*)szTgt;
  i8 iValue  = *(i8*)avSrc;
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // null processing
  
  if (C_DATUM_NULL == iValue) {
    StrLib::vCopy(az,(char*)"null"_Stz);
    return;
  }

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  
  switch (eDatum) {
      
    case DM_BCD: {
      SP_Bcd SP_Bcd;
      StrLib::vCopy(az,(char*)SP_Bcd.sz(iValue));
      break;
    }
      
    case DM_BOL: {
      SP_Bol SP_Bol;
      StrLib::vCopy(az,(char*)SP_Bol.sz(iValue));
      break;
    }
      
    case DM_DTE: {
      SP_Dte SP_Dte;
      StrLib::vCopy(az,(char*)SP_Dte.sz(iValue));
      break;
    }
      
    case DM_FLT: {
      SP_Flt SP_Flt;
      StrLib::vCopy(az,(char*)SP_Flt.sz(*(d8*)avSrc));
      break;
    }
      
    case DM_INT: {
      SP_Int spInt;
      StrLib::vCopy(az,(char*)spInt.sz(iValue));
      break;
    }
      
    case DM_IVL: {
      SP_Ivl spIvl;
      StrLib::vCopy(az,(char*)spIvl.sz(iValue));
      break;
    }
      
    case DM_TME: {
      SP_Tme SP_Tme;
      StrLib::vCopy(az,(char*)SP_Tme.sz(iValue));
      break;
    }
      
    case DM_TSP: {
      SP_Tsp SP_Tsp;
      StrLib::vCopy(az,(char*)SP_Tsp.sz(iValue));
      break;
    }
      
    case DM_VCH: {
      SP_Vch SP_Vch;
      StrLib::vCopy(az,(char*)SP_Vch.sz((char*)avSrc));
      break;
    }
      
    default:
      vThrow(FK_FATAL,__func__,__FILE__,__LINE__,
             "Sprint::Into: Invalid eDatum: %lld\n"_Stz,
             eDatum);
  }

}

/*----------------------------------------------------------------------------*/
/*-- Public ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* get result as Stz*                                                         */
/*----------------------------------------------------------------------------*/
Stz* Sprint::sz(void)
{
  _precondition_(_MAGIC_,_FOOTER_);

  return (Stz*) &m_caResult[0];
}

/*----------------------------------------------------------------------------*/
/* get format as Stz*                                                         */
/*----------------------------------------------------------------------------*/
Stz* Sprint::szFormat(void)
{
  _precondition_(_MAGIC_,_FOOTER_);
  
  return (Stz*) &m_caFormat[0];
}

/*----------------------------------------------------------------------------*/
/* set format                                                                 */
/*----------------------------------------------------------------------------*/
void Sprint::vFormat(const Stz* szFormat)
{
  _precondition_(_MAGIC_,_FOOTER_);
  
  StrLib::vCopy(m_caFormat,(char*)szFormat);
}

/*----------------------------------------------------------------------------*/
/* null                                                                        */
/*----------------------------------------------------------------------------*/
Stz* Sprint::szNull(void)
{
  _precondition_(_MAGIC_,_FOOTER_);
  
  StrLib::vCopy(m_caResult,"NULL");
  
  return (Stz*) &m_caResult[0];
}

/*----------------------------------------------------------------------------*/
/* get result as Stz*                                                         */
/*----------------------------------------------------------------------------*/
Stz* Sprint::szResult(void)
{
  _precondition_(_MAGIC_,_FOOTER_);
  
  return (Stz*) &m_caResult[0];
}

/*----------------------------------------------------------------------------*/
/* set result                                                                 */
/*----------------------------------------------------------------------------*/
void Sprint::vResult(const Stz* szResult)
{
  _precondition_(_MAGIC_,_FOOTER_);
  
  StrLib::vCopy(m_caFormat,(char*)szResult);
}


/*----------------------------------------------------------------------------*/
/*-- Footer ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* get location of footer                                                     */
/*----------------------------------------------------------------------------*/
Footer* Sprint::aFooter(void)
{
  _precondition_(_MAGIC_);
  
  return &m_xFooter;
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
