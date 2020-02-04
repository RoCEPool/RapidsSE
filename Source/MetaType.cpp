/*============================================================================*/
/* MetaType.cpp - Meta Structure (definition of class, struct, union)    */
/*============================================================================*/

#include "Reflection.h"

/*----------------------------------------------------------------------------*/
/*-- LifeCycle ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

__pCastFunction(Reflection,MetaType,CN_METATYPE)
__aNewFunction(MetaType,CN_METATYPE)
__vDeleteFunction(MetaType,CN_METATYPE)
__aFooterFunction(MetaType)


/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
MetaType::MetaType(Stz* szStructureNm,
                   eMS eStructure,
                   eCP eCppPart,
                   MetaType* p_ParentType)
: Object(CN_METATYPE)
{
  _precondition_(_MAGIC_,CN_METATYPE,_FOOTER_);
  
  m_p_ParentType  = p_ParentType;

  m_sStructureNm  = szStructureNm;
  m_eStructure    = eStructure;
  m_eCppPart      = eCppPart;
  
  m_pTplParams    = nullptr;
  m_pMetaInherits = nullptr;
  
  m_nSize         = 0;

  _checkInitialised_(CN_METATYPE);
  
  _postcondition_(_MAGIC_,CN_METATYPE,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
MetaType::~MetaType(void) noexcept(false)
{
  _precondition_(_MAGIC_,CN_METATYPE,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/*-- Static ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/*-- Public ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* add a new member with given name                                           */
/*----------------------------------------------------------------------------*/
MetaMember* MetaType::pAddMember(Stz* szMemberNm)
{
  _precondition_(_MAGIC_,CN_METATYPE,_FOOTER_);
  
  MetaMember* pMember = nullptr; //new MetaMember(szMemberNm,this);
    
  vAppend(pMember);
  
  return pMember;
}

/*----------------------------------------------------------------------------*/
/* add a new member with given name                                           */
/*----------------------------------------------------------------------------*/
MetaInherit* MetaType::pAddInherit(Stz* szBaseClassNm)
{
  _precondition_(_MAGIC_,CN_METATYPE,_FOOTER_);
  
  if (! m_pMetaInherits)
    m_pMetaInherits = new MetaInherits(this);

  MetaInherit* pMetaInherit = m_pMetaInherits->pAddInherit(szBaseClassNm);
  
  return pMetaInherit;
}

/*----------------------------------------------------------------------------*/
/* get meta inheritance list                                                  */
/*----------------------------------------------------------------------------*/
MetaInherits* MetaType::pInherits(void)
{
  _precondition_(_MAGIC_,CN_METATYPE,_FOOTER_);
  
  return m_pMetaInherits;
}

/*----------------------------------------------------------------------------*/
/* get parent structure                                                       */
/*----------------------------------------------------------------------------*/
MetaType* MetaType::p_ParentType(void)
{
  _precondition_(_MAGIC_,CN_METATYPE,_FOOTER_);
  
  return m_p_ParentType;
}

/*----------------------------------------------------------------------------*/
/* get structure kind                                                         */
/*----------------------------------------------------------------------------*/
eMS MetaType::eStructure(void)
{
  _precondition_(_MAGIC_,CN_METATYPE,_FOOTER_);
    
  return m_eStructure;
}

/*----------------------------------------------------------------------------*/
/* set structure kind                                                         */
/*----------------------------------------------------------------------------*/
void MetaType::vStructure(eMS eStructure)
{
  _precondition_(_MAGIC_,CN_METATYPE,_FOOTER_);
    
  m_eStructure = eStructure;
}

/*----------------------------------------------------------------------------*/
/* print                                                                      */
/*----------------------------------------------------------------------------*/
void MetaType::vPrint(i8 i_Tab)
{
  _precondition_(_MAGIC_,CN_METATYPE,_FOOTER_);
    
  // -- -- -- -- -- --
  vTrace("%sMetaType %6s %6s %lldB %16s "_Stz,
         
         Stz::szTab(i_Tab)->azChar(),
          aMS[m_eStructure]->azChar(),
         aCP[m_eCppPart]->azChar(),
         m_nSize,
         
         m_sStructureNm.azChar()
         );
  
  // -- -- -- -- -- --
  if (m_pTplParams) {
    vTrace(" < "_Stz);
          
    i8 nArgs = m_pTplParams->nItems();
    for (i8 i_Arg = 0; i_Arg < nArgs; i_Arg++) {
      if (i_Arg)
        vTrace(" "_Stz);
      vTrace("%s"_Stz,m_pTplParams->szName(i_Arg));
    }
    
    vTrace(" >"_Stz);
  }
  
  // -- -- -- -- -- --
  if (m_pMetaInherits) {
    vTrace(" : "_Stz);
    for (MetaInherit* pInherit = m_pMetaInherits->pHead();
         pInherit;
         pInherit = pInherit->pNextItem()) {
     
      if (pInherit != m_pMetaInherits->pHead())
          vTrace(", "_Stz);

      vTrace("%s"_Stz,pInherit->sMetaType()->azChar());
      
      EnameSet* pTplArgs = pInherit->pTplArgs();
      if (pTplArgs) {
        vTrace("<"_Stz);
        i8 nArgs = pTplArgs->nItems();
        for (i8 i_Arg = 0; i_Arg < nArgs; i_Arg++)
          vTrace("%s"_Stz,pTplArgs->szName(i_Arg));
        vTrace(">"_Stz);
      }
    }
  }
  
  // -- -- -- -- -- --
  vTrace("\n"_Stz);
}

/*----------------------------------------------------------------------------*/
/* recursive print                                                            */
/*----------------------------------------------------------------------------*/
void MetaType::vRecursivePrint(i8 i_Depth)
{
  _precondition_(_MAGIC_,CN_METATYPE,_FOOTER_);
  
  vPrint(i_Depth);
  
  for (MetaMember* pMember = pHead(); pMember; pMember = pMember->pNextItem())
    pMember->vRecursivePrint(i_Depth + 1);
}

/*----------------------------------------------------------------------------*/
/* get rolled up size of this structure                                       */
/*----------------------------------------------------------------------------*/
i8 MetaType::nSize(void)
{
  _precondition_(_MAGIC_,CN_METATYPE,_FOOTER_);
    
  return m_nSize;
}

/*----------------------------------------------------------------------------*/
/* get structure name                                                         */
/*----------------------------------------------------------------------------*/
Stz* MetaType::szStructureNm(void)
{
  _precondition_(_MAGIC_,CN_METATYPE,_FOOTER_);
    
  return m_sStructureNm.szStz();
}

/*----------------------------------------------------------------------------*/
/* set structure name                                                         */
/*----------------------------------------------------------------------------*/
void MetaType::vStructureNm(Stz* szStructureNm)
{
  _precondition_(_MAGIC_,CN_METATYPE,_FOOTER_);
    
  m_sStructureNm = szStructureNm;
}

/*----------------------------------------------------------------------------*/
/* get template arguments                                                     */
/*----------------------------------------------------------------------------*/
EnameSet* MetaType::pTplParams(void)
{
  _precondition_(_MAGIC_,CN_METATYPE,_FOOTER_);
    
  return m_pTplParams;
}

/*----------------------------------------------------------------------------*/
/* set template arguments                                                     */
/*----------------------------------------------------------------------------*/
void MetaType::vTplParams(EnameSet* pTplParams)
{
  _precondition_(_MAGIC_,CN_METATYPE,_FOOTER_);
  
  if (! m_pTplParams)
    m_pTplParams = new EnameSet;
    
  m_pTplParams->vCopy(pTplParams);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
