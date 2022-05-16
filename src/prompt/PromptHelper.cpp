/*============================================================================*/
/* PromptHelper.cpp -- a set of prompt helper functions                       */
/*============================================================================*/

#include "BASE.h"

#include "PromptHelper.hpp"

/*----------------------------------------------------------------------------*/
/* process command flags                                                      */
/*----------------------------------------------------------------------------*/
void PromptHelper::vClassFlags(ClassFlags* pFlags,
                              Stz* szGroupNm,
                              Stz* szaEnumNm[],
                              i8   iEnumLimit,
                              TokenVec* pTknVec)
{
  Stz* aTkn1 = nullptr;
  Stz* aTkn2 = nullptr;
  
  i8 nTokens = pTknVec->nTokens();
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // mark<Group>
  
  if (nTokens == 1) {
    
    for (i8 i_Enum = 1; i_Enum < iEnumLimit; i_Enum++)
      if (pFlags->bEnum((eCN)i_Enum))
        vTrace("%s : %s\n"_Stz,
               szaEnumNm[i_Enum],
               "on"_Stz);
    
    vTrace("Mark%sAll is currently : %s\n"_Stz,
           szGroupNm,
           pFlags->bAll() ? "on"_Stz : "off"_Stz);
    
    return;
  }
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // mark<Group> ?
  // mark<Group> all
  // mark<Group> <name>

  if (nTokens == 2) {
    aTkn1 = pTknVec->pGet(1)->szChars();
    
    if (aTkn1->bSame("?"_Stz)) {
      for (i8 i_Enum = 1; i_Enum < iEnumLimit; i_Enum++)
          vTrace("  %s\n"_Stz,szaEnumNm[i_Enum]);
      return;
    }
    
    if (aTkn1->bSame("all"_Stz)) {
      vTrace("Mark%sAll is currently : %s\n"_Stz,
             szGroupNm,
             pFlags->bAll() ? "on"_Stz : "off"_Stz);
      return;
    }
    
    for (i8 i_Enum = 1; i_Enum < iEnumLimit; i_Enum++)
      if (aTkn1->bSame(szaEnumNm[i_Enum]))
        vTrace("%s : %s\n"_Stz,
               szaEnumNm[i_Enum],
               pFlags->bEnum(i_Enum) ? "on"_Stz : "off"_Stz);
  }
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // mark<Group> all on
  // mark<Group> all off
  // mark<Group> <name> on
  // mark<Group> <name> off

  if (nTokens == 3) {
    aTkn1 = pTknVec->pGet(1)->szChars();
    aTkn2 = pTknVec->pGet(2)->szChars();
    
    if (aTkn1->bSame("all"_Stz)) {
      if (aTkn2->bIsSomeTrue())
        pFlags->vAll(true);
      else
        if (aTkn2->bIsSomeFalse())
          pFlags->vAll(false);
      return;
    }
    
    if (aTkn2->bIsSomeTrue()) {
      for (i8 i_Enum = 1; i_Enum < iEnumLimit; i_Enum++)
        if (aTkn1->bSame(szaEnumNm[i_Enum]))
          pFlags->vEnum(i_Enum,true);
      return;
    }
    
    if (aTkn2->bIsSomeFalse()) {
      for (i8 i_Enum = 1; i_Enum < iEnumLimit; i_Enum++)
        if (aTkn1->bSame(szaEnumNm[i_Enum]))
          pFlags->vEnum(i_Enum,false);
      return;
    }
    
    return;
  }

}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
