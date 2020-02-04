/*============================================================================*/
/* MetaStructure.cpp - Meta Structure (definition of class, struct, union)    */
/*============================================================================*/

#include "Reflection.h"

/*----------------------------------------------------------------------------*/
/*-- LifeCycle ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

__pCastFunction(Reflection,MetaStructure,CN_METATYPE)
__aNewFunction(MetaStructure,CN_METATYPE)
__vDeleteFunction(MetaStructure,CN_METATYPE)
__aFooterFunction(MetaStructure)


/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
MetaStructure::MetaStructure(Stz* szStructureNm,
                             eMS eStructure,
                             eCP eCppPart,
                             MetaStructure* p_ParentStructure)
: Object(CN_METATYPE)
{
  _precondition_(_MAGIC_,CN_METATYPE,_FOOTER_);
  
  m_p_ParentStructure = p_ParentStructure;

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
MetaStructure::~MetaStructure(void) noexcept(false)
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
MetaMember* MetaStructure::pAddMember(Stz* szMemberNm)
{
  _precondition_(_MAGIC_,CN_METATYPE,_FOOTER_);
  
  MetaMember* pMember = nullptr; //new MetaMember(szMemberNm,this);
    
  vAppend(pMember);
  
  return pMember;
}

/*----------------------------------------------------------------------------*/
/* add a new member with given name                                           */
/*----------------------------------------------------------------------------*/
MetaInherit* MetaStructure::pAddInherit(Stz* szBaseClassNm)
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
MetaInherits* MetaStructure::pInherits(void)
{
  _precondition_(_MAGIC_,CN_METATYPE,_FOOTER_);
  
  return m_pMetaInherits;
}

/*----------------------------------------------------------------------------*/
/* get parent structure                                                       */
/*----------------------------------------------------------------------------*/
MetaStructure* MetaStructure::p_ParentStructure(void)
{
  _precondition_(_MAGIC_,CN_METATYPE,_FOOTER_);
  
  return m_p_ParentStructure;
}

/*----------------------------------------------------------------------------*/
/* get structure kind                                                         */
/*----------------------------------------------------------------------------*/
eMS MetaStructure::eStructure(void)
{
  _precondition_(_MAGIC_,CN_METATYPE,_FOOTER_);
    
  return m_eStructure;
}

/*----------------------------------------------------------------------------*/
/* set structure kind                                                         */
/*----------------------------------------------------------------------------*/
void MetaStructure::vStructure(eMS eStructure)
{
  _precondition_(_MAGIC_,CN_METATYPE,_FOOTER_);
    
  m_eStructure = eStructure;
}

/*----------------------------------------------------------------------------*/
/* print                                                                      */
/*----------------------------------------------------------------------------*/
void MetaStructure::vPrint(i8 i_Tab)
{
  _precondition_(_MAGIC_,CN_METATYPE,_FOOTER_);
    
  // -- -- -- -- -- --
  vTrace("%sMetaStructure %6s %6s %lldB %16s "_Stz,
         
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

      vTrace("%s"_Stz,pInherit->sMetaStructure()->azChar());
      
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
void MetaStructure::vRecursivePrint(i8 i_Depth)
{
  _precondition_(_MAGIC_,CN_METATYPE,_FOOTER_);
  
  vPrint(i_Depth);
  
  for (MetaMember* pMember = pHead(); pMember; pMember = pMember->pNextItem())
    pMember->vRecursivePrint(i_Depth + 1);
}

/*----------------------------------------------------------------------------*/
/* get rolled up size of this structure                                       */
/*----------------------------------------------------------------------------*/
i8 MetaStructure::nSize(void)
{
  _precondition_(_MAGIC_,CN_METATYPE,_FOOTER_);
    
  return m_nSize;
}

/*----------------------------------------------------------------------------*/
/* get structure name                                                         */
/*----------------------------------------------------------------------------*/
Stz* MetaStructure::szStructureNm(void)
{
  _precondition_(_MAGIC_,CN_METATYPE,_FOOTER_);
    
  return m_sStructureNm.szStz();
}

/*----------------------------------------------------------------------------*/
/* set structure name                                                         */
/*----------------------------------------------------------------------------*/
void MetaStructure::vStructureNm(Stz* szStructureNm)
{
  _precondition_(_MAGIC_,CN_METATYPE,_FOOTER_);
    
  m_sStructureNm = szStructureNm;
}

/*----------------------------------------------------------------------------*/
/* get template arguments                                                     */
/*----------------------------------------------------------------------------*/
EnameSet* MetaStructure::pTplParams(void)
{
  _precondition_(_MAGIC_,CN_METATYPE,_FOOTER_);
    
  return m_pTplParams;
}

/*----------------------------------------------------------------------------*/
/* set template arguments                                                     */
/*----------------------------------------------------------------------------*/
void MetaStructure::vTplParams(EnameSet* pTplParams)
{
  _precondition_(_MAGIC_,CN_METATYPE,_FOOTER_);
  
  if (! m_pTplParams)
    m_pTplParams = new EnameSet;
    
  m_pTplParams->vCopy(pTplParams);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
