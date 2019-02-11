/*============================================================================*/
/* PromptHelper.cpp -- a set of prompt helper functions                       */
/*============================================================================*/

#include "Prompt.h"
#include "StorageEngine.h"

extern AttachMap* gpAttachMap;

/*----------------------------------------------------------------------------*/
/* process command flags                                                      */
/*----------------------------------------------------------------------------*/
void PromptHelper::vCommandFlags(ClassFlags& oFlags,
                                 Stz* szEnumGroupNm,
                                 TokenVec* pTknVec)
{
  JobClass* pJobClass = nullptr;
  
  Stz* aTkn1 = nullptr;
  Stz* aTkn2 = nullptr;
  Stz* aTkn3 = nullptr;
  
  i8 nTokens = pTknVec->nTokens();
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // mark<Group>
  
  if (nTokens == 1) {
    
    for (i8 i_Class = 0; i_Class < CN_ENUMS; i_Class++)
      if (oFlags.bEnum((eCN)i_Class))
        vTrace("%s : %s\n"_Stz,
               aCN[i_Class],
               "on"_Stz);
    
    vTrace("Mark%sAll is currently : %s\n"_Stz,
           szEnumGroupNm,
           oFlags.bAll() ? "on"_Stz : "off"_Stz);
    
    return;
  }
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // mark<Group> ?
  // mark<Group> all
  // mark<Group> <command>
  
  if (nTokens == 2) {
    aTkn1 = pTknVec->pGet(1)->aText();
    
    if (aTkn1->bSame("?"_Stz)) {
      for (i8 i_Class = 0; i_Class < CN_ENUMS; i_Class++)
        if ( _bIsValidJob((eCN)i_Class))
          vTrace("  %s\n"_Stz,aCN[i_Class]);
      return;
    }
    
    if (aTkn1->bSame("all"_Stz)) {
      vTrace("Mark%sAll is currently : %s\n"_Stz,
             szEnumGroupNm,
             oFlags.bAll() ? "on"_Stz : "off"_Stz);
      return;
    }
    
    pJobClass = JobClass::pFind(aTkn1, nullptr);
    if (pJobClass)
      vTrace("%s : %s\n"_Stz,
             aCN[pJobClass->m_eClass],
             oFlags.bEnum(pJobClass->m_eClass) ? "on"_Stz : "off"_Stz);
    return;
  }
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // mark<Group> all on
  // mark<Group> all off
  // mark<Group> <command> on
  // mark<Group> <command> off
  
  if (nTokens == 3) {
    aTkn1 = pTknVec->pGet(1)->aText();
    aTkn2 = pTknVec->pGet(2)->aText();
    
    if (aTkn1->bSame("all"_Stz)) {
      if (aTkn2->bIsSomeTrue())
        oFlags.vAll(true);
      else
        if (aTkn2->bIsSomeFalse())
          oFlags.vAll(false);
      return;
    }
    
    if (aTkn2->bIsSomeTrue()) {
      pJobClass = JobClass::pFind(aTkn1, nullptr);
      if (pJobClass)
        oFlags.vEnum(pJobClass->m_eClass,true);
      return;
    }
    
    if (aTkn2->bIsSomeFalse()) {
      pJobClass = JobClass::pFind(aTkn1, nullptr);
      if (pJobClass)
        oFlags.vEnum(pJobClass->m_eClass,false);
      return;
    }
    
    pJobClass = JobClass::pFind(aTkn1, aTkn2);
    if (pJobClass)
      vTrace("%s : %s\n"_Stz,
             aCN[pJobClass->m_eClass],
             oFlags.bEnum(pJobClass->m_eClass) ? "on"_Stz : "off"_Stz);
    return;
  }
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // mark<Group> <command1> <command2> on
  // mark<Group> <command1> <command2> off
  
  if (nTokens == 4) {
    aTkn1 = pTknVec->pGet(1)->aText();
    aTkn2 = pTknVec->pGet(2)->aText();
    aTkn3 = pTknVec->pGet(3)->aText();
    
    pJobClass = JobClass::pFind(aTkn1, aTkn2);
    if (! pJobClass)
      return;
    
    if (aTkn3->bIsSomeTrue()) {
      oFlags.vEnum(pJobClass->m_eClass,true);
      return;
    }
    
    if (aTkn3->bIsSomeFalse()) {
      oFlags.vEnum(pJobClass->m_eClass,false);
      return;
    }
    
    return;
  }
  
}

/*----------------------------------------------------------------------------*/
/* process command flags                                                      */
/*----------------------------------------------------------------------------*/
void PromptHelper::vEnumFlags(ClassFlags& oFlags,
                              Stz* szEnumNm,
                              Stz* szaEnumName[],
                              i8   iEnumLimit,
                              TokenVec* pTknVec)
{
  Stz* aTkn1 = nullptr;
  Stz* aTkn2 = nullptr;
  
  i8 nTokens = pTknVec->nTokens();
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // mark<Group>
  
  if (nTokens == 1) {
    
    for (i8 i_Enum = 0; i_Enum < iEnumLimit; i_Enum++)
      if (oFlags.bEnum((eCN)i_Enum))
        vTrace("%s : %s\n"_Stz,
               szaEnumName[i_Enum],
               "on"_Stz);
    
    vTrace("Mark%sAll is currently : %s\n"_Stz,
           szEnumNm,
           oFlags.bAll() ? "on"_Stz : "off"_Stz);
    
    return;
  }
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // mark<Group> ?
  // mark<Group> all
  // mark<Group> <name>

  if (nTokens == 2) {
    aTkn1 = pTknVec->pGet(1)->aText();
    
    if (aTkn1->bSame("?"_Stz)) {
      for (i8 i_Enum = 0; i_Enum < iEnumLimit; i_Enum++)
          vTrace("  %s\n"_Stz,szaEnumName[i_Enum]);
      return;
    }
    
    if (aTkn1->bSame("all"_Stz)) {
      vTrace("Mark%sAll is currently : %s\n"_Stz,
             szEnumNm,
             oFlags.bAll() ? "on"_Stz : "off"_Stz);
      return;
    }
    
    for (i8 i_Enum = 0; i_Enum < iEnumLimit; i_Enum++)
      if (aTkn1->bSame(szaEnumName[i_Enum]))
        vTrace("%s : %s\n"_Stz,
               szaEnumName[i_Enum],
               "on"_Stz);
  }
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // mark<Group> all on
  // mark<Group> all off
  // mark<Group> <name> on
  // mark<Group> <name> off

  if (nTokens == 3) {
    aTkn1 = pTknVec->pGet(1)->aText();
    aTkn2 = pTknVec->pGet(2)->aText();
    
    if (aTkn1->bSame("all"_Stz)) {
      if (aTkn2->bIsSomeTrue())
        oFlags.vAll(true);
      else
        if (aTkn2->bIsSomeFalse())
          oFlags.vAll(false);
      return;
    }
    
    if (aTkn2->bIsSomeTrue()) {
      for (i8 i_Enum = 0; i_Enum < iEnumLimit; i_Enum++)
        if (aTkn1->bSame(szaEnumName[i_Enum]))
          oFlags.vEnum(i_Enum,true);
      return;
    }
    
    if (aTkn2->bIsSomeFalse()) {
      for (i8 i_Enum = 0; i_Enum < iEnumLimit; i_Enum++)
        if (aTkn1->bSame(szaEnumName[i_Enum]))
          oFlags.vEnum(i_Enum,false);
      return;
    }
    
    return;
  }

}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
