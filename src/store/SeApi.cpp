/*============================================================================*/
/* SeApi.cpp -- Se Reentrant API                                              */
/*============================================================================*/

#include "BASE.h"

#include "SeApi.hpp"

/*----------------------------------------------------------------------------*/
/*-- LifeCycle ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

__pCastFunction(STORE,SeApi,CN_SEAPI)
__aNewFunction(SeApi,CN_SEAPI)
__vDeleteFunction(SeApi,CN_SEAPI)
__aFooterFunction(SeApi)

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
SeApi::SeApi(void)
: Object(CN_SEAPI)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_SEAPI,_FOOTER_);
  
  m_sSequence = ""_Stz;
  
  _postcondition_(_ADDRESS_,_MAGIC_,CN_SEAPI,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
SeApi::~SeApi(void) noexcept(false)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_SEAPI,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/*-- Public ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* pipe | operator                                                            */
/*----------------------------------------------------------------------------*/
SeApi& SeApi::operator |(SeApi* pArg)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_SEAPI,_FOOTER_);
  
  return *(SeApi*)this;
}

/*----------------------------------------------------------------------------*/
/* add operator argument                                                      */
/*----------------------------------------------------------------------------*/
SeApi* SeApi::pAdd(Stz* szOpNm, Stz* szOpArg)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_SEAPI,_FOOTER_);
  
  m_sSequence.vCatSprint(" %s %s"_Stz,szOpArg,szOpNm);
  
  return this;
}

/*----------------------------------------------------------------------------*/
/* encode                                                                     */
/*----------------------------------------------------------------------------*/
SeApi* SeApi::pEncode(Stz* szAction)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_SEAPI,_FOOTER_);
  
  pEncode(szAction,""_Stz);
  
  return this;
}

/*----------------------------------------------------------------------------*/
/* encode                                                                     */
/*----------------------------------------------------------------------------*/
SeApi* SeApi::pEncode(Stz* szAction, Stz* szArgument)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_SEAPI,_FOOTER_);
  
  if (szAction->bSame("scan"_Stz) && szArgument->bSame("tables"_Stz)) {
    pAdd("metaScan_seapi"_Stz,""_Stz);
  }
  else
  if (szAction->bSame("into"_Stz)) {
    pAdd("rowToList_seapi"_Stz,""_Stz);
  }
  else
  if (szAction->bSame("tbd..."_Stz)) {
    vTrace("pEncode tbd : %s\n"_Stz,szArgument); \
  }
  else {
    NEVER_REACH;
  }
  
  return this;
}

/*----------------------------------------------------------------------------*/
/* get sequence                                                               */
/*----------------------------------------------------------------------------*/
Stz* SeApi::szGet(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_SEAPI,_FOOTER_);
    
  return m_sSequence;
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
