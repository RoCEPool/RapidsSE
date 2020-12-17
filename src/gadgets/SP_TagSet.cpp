/*============================================================================*/
/* SP_TagSet.cpp - TagSet sprint helper                          (on c stack) */
/*============================================================================*/

#include "Resources.h"

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
SP_TagSet::SP_TagSet(void)
{
  _precondition_(_MAGIC_,CN_SPMEDIUM,_FOOTER_);
  
  _checkInitialised_(CN_SPMEDIUM);
  
  _postcondition_(_MAGIC_,CN_SPMEDIUM,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
SP_TagSet::~SP_TagSet(void) noexcept(false)
{
  _precondition_(_MAGIC_,CN_SPMEDIUM,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/*-- Public ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* sprint SP_TagSet and return Stz* to result                                 */
/*----------------------------------------------------------------------------*/
Stz* SP_TagSet::sz(TagSet* pTagSet)
{
  _precondition_(_MAGIC_,CN_SPMEDIUM,_FOOTER_);
  
   StrLib::vSprint(m_caResult,"{ "_Stz);
  
  for (i8 i_Ename = 0; i_Ename < pTagSet->nEnames(); i_Ename++) {
    
    if (i_Ename)
      StrLib::vSprintCat((Stz*)m_caResult,", "_Stz);

    if (TN_INTEGER == pTagSet->eToken(i_Ename)) {
      
      StrLib::vSprintCat((Stz*)m_caResult,"%s=%lld"_Stz,
              
                         pTagSet->szName(i_Ename),
                         pTagSet->iValue(i_Ename),
             
                         C_VARGS_END
                        );
      continue;
    }
    
    StrLib::vSprintCat((Stz*)m_caResult,"%s=%s"_Stz,
           
                       pTagSet->szName(i_Ename),
                       pTagSet->szValue(i_Ename),

                       C_VARGS_END
                      );
  }
  
  StrLib::vSprintCat((Stz*)m_caResult," }"_Stz);
  
  return (Stz*) &m_caResult[0];
}


/*----------------------------------------------------------------------------*/
/*-- Footer ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/
__aFooterFunction(SP_TagSet)

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
