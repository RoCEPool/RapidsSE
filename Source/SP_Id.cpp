/*============================================================================*/
/* SP_Id.cpp - Id sprint helper                                  (on c stack) */
/*============================================================================*/

#include "Focals.h"

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
SP_Id::SP_Id(void)
: SP(CN_SPID)
{
  _precondition_(_MAGIC_,CN_SPID,_FOOTER_);
  
  _checkInitialised_(CN_SPID);
  
  _postcondition_(_MAGIC_,CN_SPID,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
SP_Id::~SP_Id(void) noexcept(false)
{
  _precondition_(_MAGIC_,CN_SPID,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/*-- Public ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* sprint Id and return Stz* to result                                        */
/*----------------------------------------------------------------------------*/
Stz* SP_Id::sz(i8 iId)
{
  _precondition_(_MAGIC_,CN_SPID,_FOOTER_);
  
  if (C_UNKNOWN == iId) {
    StrLib::vSprint(m_caResult,"-.---"_Stz);
    return (Stz*) &m_caResult[0];
 }
  
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  
  Gid wGid;
  wGid.m_iAsI8 = iId;
  
  StrLib::vSprint(m_caResult,
                  "%lld.%03lld"_Stz,
                  
                  wGid.m_uNodeNo,
                  wGid.m_uIdNo);

  return (Stz*) &m_caResult[0];
}

/*----------------------------------------------------------------------------*/
/* sprint Id and return Stz* to result                                        */
/*----------------------------------------------------------------------------*/
Stz* SP_Id::sz(eIK eKind, i8 iId)
{
  _precondition_(_MAGIC_,CN_SPID,_FOOTER_);
  
  if (C_UNKNOWN == iId) {
    StrLib::vSprint(m_caResult,"%s-.---"_Stz,aIK_3c[eKind]);
    return (Stz*) &m_caResult[0];
  }
  
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  
  Gid wGid;
  wGid.m_iAsI8 = iId;

  StrLib::vSprint(m_caResult,
                  "%s%lld.%03lld"_Stz,
                  
                  aIK_3c[eKind],
                  wGid.m_uNodeNo,
                  wGid.m_uIdNo);
  
  return (Stz*) &m_caResult[0];
}

/*----------------------------------------------------------------------------*/
/* sprint Id and return Stz* to result                                        */
/*----------------------------------------------------------------------------*/
Stz* SP_Id::sz(eEK eKind, i8 iId)
{
  _precondition_(_MAGIC_,CN_SPID,_FOOTER_);
  
  if (C_UNKNOWN == iId) {
    StrLib::vSprint(m_caResult,"%s-.---"_Stz,aEK[eKind]);
    return (Stz*) &m_caResult[0];
  }
  
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  
  Gid wGid;
  wGid.m_iAsI8 = iId;

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
__aFooterFunction(SP_Id)

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
