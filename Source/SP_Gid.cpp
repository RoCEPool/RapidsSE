/*============================================================================*/
/* SP_Gid.cpp - Gid sprint helper                                (on c stack) */
/*============================================================================*/

#include "Focals.h"

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
SP_Gid::SP_Gid(void)
{
  _precondition_(_MAGIC_,CN_SPSMALL,_FOOTER_);
  
  _checkInitialised_(CN_SPSMALL);
  
  _postcondition_(_MAGIC_,CN_SPSMALL,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
SP_Gid::~SP_Gid(void) noexcept(false)
{
  _precondition_(_MAGIC_,CN_SPSMALL,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/*-- Public ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* sprint Gid and return Stz* to result                                       */
/*----------------------------------------------------------------------------*/
Stz* SP_Gid::sz(i8 iGid)
{
  _precondition_(_MAGIC_,CN_SPSMALL,_FOOTER_);
  
  if (C_UNKNOWN == iGid) {
    StrLib::vSprint(m_caResult,"-.---"_Stz);
    return (Stz*) &m_caResult[0];
 }
  
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  
  Gid wGid;
  wGid.m_iAsI8 = iGid;
  
  StrLib::vSprint(m_caResult,
                  "%lld.%03lld"_Stz,
                  
                  wGid.m_uNodeNo,
                  wGid.m_uIdNo);

  return (Stz*) &m_caResult[0];
}

/*----------------------------------------------------------------------------*/
/* sprint Gid and return Stz* to result                                       */
/*----------------------------------------------------------------------------*/
Stz* SP_Gid::sz(eIK eKind, i8 iGid)
{
  _precondition_(_MAGIC_,CN_SPSMALL,_FOOTER_);
  
  if (C_UNKNOWN == iGid) {
    StrLib::vSprint(m_caResult,"%s-.---"_Stz,aIK_3c[eKind]);
    return (Stz*) &m_caResult[0];
  }
  
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  
  Gid wGid;
  wGid.m_iAsI8 = iGid;

  StrLib::vSprint(m_caResult,
                  "%s%lld.%03lld"_Stz,
                  
                  aIK_3c[eKind],
                  wGid.m_uNodeNo,
                  wGid.m_uIdNo);
  
  return (Stz*) &m_caResult[0];
}

/*----------------------------------------------------------------------------*/
/* sprint Gid and return Stz* to result                                       */
/*----------------------------------------------------------------------------*/
Stz* SP_Gid::sz(eEK eKind, i8 iGid)
{
  _precondition_(_MAGIC_,CN_SPSMALL,_FOOTER_);
  
  if (C_UNKNOWN == iGid) {
    StrLib::vSprint(m_caResult,"%s-.---"_Stz,aEK[eKind]);
    return (Stz*) &m_caResult[0];
  }
  
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  
  Gid wGid;
  wGid.m_iAsI8 = iGid;

  StrLib::vSprint(m_caResult,
                  "%s%lld.%03lld"_Stz,
                  
                  aEK[eKind],
                  wGid.m_uNodeNo,
                  wGid.m_uIdNo);
  
  return (Stz*) &m_caResult[0];
}


/*----------------------------------------------------------------------------*/
/*-- Footer ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
__aFooterFunction(SP_Gid)

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
