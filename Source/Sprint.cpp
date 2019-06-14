/*============================================================================*/
/* Sprint.cpp - Sprint Utility                                                */
/*============================================================================*/

#include "Transform.h"

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
Sprint::Sprint(eCN eClass)
: Object(eClass)
{
#ifdef WITH_CHECK_INITIALISED
  MemLib::vZero(m_caFormat,sizeof(m_caFormat),CN_SPDTE);
  MemLib::vZero(m_caResult,sizeof(m_caResult),CN_SPDTE);
#else
  m_caFormat[0] = '\0';
  m_caResult[0] = '\0';
#endif

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
      spBCD spBcd;
      StrLib::vCopy(az,(char*)spBcd.sz(iValue));
      break;
    }
      
    case DM_BOL: {
      spBOL spBol;
      StrLib::vCopy(az,(char*)spBol.sz(iValue));
      break;
    }
      
    case DM_DTE: {
      spDTE spDte;
      StrLib::vCopy(az,(char*)spDte.sz(iValue));
      break;
    }
      
    case DM_FLT: {
      spFLT spFlt;
      StrLib::vCopy(az,(char*)spFlt.sz(*(d8*)avSrc));
      break;
    }
      
    case DM_INT: {
      spINT spInt;
      StrLib::vCopy(az,(char*)spInt.sz(iValue));
      break;
    }
      
    case DM_IVL: {
      spIVL spIvl;
      StrLib::vCopy(az,(char*)spIvl.sz(iValue));
      break;
    }
      
    case DM_TME: {
      spTME spTme;
      StrLib::vCopy(az,(char*)spTme.sz(iValue));
      break;
    }
      
    case DM_TSP: {
      spTSP spTsp;
      StrLib::vCopy(az,(char*)spTsp.sz(iValue));
      break;
    }
      
    case DM_VCH: {
      spVCH spVch;
      StrLib::vCopy(az,(char*)spVch.sz((char*)avSrc));
      break;
    }
      
    default:
      vFatal("Sprint::Into: Invalid eDatum: %lld\n"_Stz,
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
