/*============================================================================*/
/* SP_EnmSet.cpp - EnameSet sprint helper                          (on c stack) */
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
Stz* SP_EnmSet::sz(EnameSet* pEnameSet)
{
  _precondition_(_MAGIC_,CN_SPELAPSED,_FOOTER_);
  
   StrLib::vSprint(m_caResult,"{ "_Stz);
  
  for (i8 i_Ename = 0; i_Ename < pEnameSet->nEnames(); i_Ename++) {
    
    if (i_Ename)
      StrLib::vSprintCat(m_caResult," , ");

    if (TN_INTEGER == pEnameSet->eToken(i_Ename)) {
      
      StrLib::vSprintCat(m_caResult,"%s = %lld",
              
                         pEnameSet->szName(i_Ename),
                         pEnameSet->iValue(i_Ename),
             
                         C_ENDARGS
                        );
      continue;
    }
    
    StrLib::vSprintCat(m_caResult,"%s = %s",
           
                       pEnameSet->szName(i_Ename),
                       pEnameSet->szValue(i_Ename),

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
