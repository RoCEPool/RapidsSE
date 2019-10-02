/*============================================================================*/
/* SP_EnmSet.cpp - EnmSet sprint helper                          (on c stack) */
/*============================================================================*/

#include "Gadgets.h"

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
SP_EnmSet::SP_EnmSet(void)
: SP(CN_SPELAPSED)
{
  _precondition_(_MAGIC_,CN_SPELAPSED,_FOOTER_);
  
  _checkInitialised_(CN_SPELAPSED);
  
  _postcondition_(_MAGIC_,CN_SPELAPSED,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
SP_EnmSet::~SP_EnmSet(void) noexcept(false)
{
  _precondition_(_MAGIC_,CN_SPELAPSED,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/*-- Public ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* sprint SP_EnmSet and return Stz* to result                                 */
/*----------------------------------------------------------------------------*/
Stz* SP_EnmSet::sz(EnmSet* pEnmSet)
{
  _precondition_(_MAGIC_,CN_SPELAPSED,_FOOTER_);
  
   StrLib::vSprint(m_caResult,"{ "_Stz);
  
  for (i8 i_Enm = 0; i_Enm < pEnmSet->nEnm(); i_Enm++) {
    
    if (i_Enm)
      StrLib::vSprintCat(m_caResult,", ");

    if (TN_INTEGER == pEnmSet->eToken(i_Enm)) {
      
      StrLib::vSprintCat(m_caResult,"%s=%lld",
              
                         pEnmSet->szName(i_Enm),
                         pEnmSet->iValue(i_Enm),
             
                         C_ENDARGS
                        );
      continue;
    }
    
    StrLib::vSprintCat(m_caResult,"%s=%s",
           
                       pEnmSet->szName(i_Enm),
                       pEnmSet->szValue(i_Enm),

                       C_ENDARGS
                      );
  }
  
  StrLib::vSprintCat(m_caResult," }");
  
  return (Stz*) &m_caResult[0];
}


/*----------------------------------------------------------------------------*/
/*-- Footer ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
__aFooterFunction(SP_EnmSet)

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
