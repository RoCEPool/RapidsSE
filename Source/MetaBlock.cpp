/*============================================================================*/
/* MetaBlock.cpp - MetaBlock                                                  */
/*============================================================================*/

#include "Reflection.h"

/*----------------------------------------------------------------------------*/
/*-- LifeCycle ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

__pCastFunction(Reflection,MetaBlock,CN_METABLOCK)
__aNewFunction(MetaBlock,CN_METABLOCK)
__vDeleteFunction(MetaBlock,CN_METABLOCK)
__aFooterFunction(MetaBlock)


/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
MetaBlock::MetaBlock(Stz* szBlockNm, eMT eBlock)
: Object(CN_METABLOCK)
{
  _precondition_(_MAGIC_,CN_METABLOCK,_FOOTER_);
  
  m_sBlockNm = szBlockNm;
  m_nSize    = 0;
  m_eBlock   = eBlock;

  _checkInitialised_(CN_METABLOCK);
  
  _postcondition_(_MAGIC_,CN_METABLOCK,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
MetaBlock::~MetaBlock(void) noexcept(false)
{
  _precondition_(_MAGIC_,CN_METABLOCK,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/*-- Static ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/*-- Public ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* get member set kind                                                        */
/*----------------------------------------------------------------------------*/
eMT MetaBlock::eBlock(void)
{
  _precondition_(_MAGIC_,CN_METABLOCK,_FOOTER_);
    
  return m_eBlock;
}

/*----------------------------------------------------------------------------*/
/* set member set kind                                                        */
/*----------------------------------------------------------------------------*/
void MetaBlock::vBlock(eMT eBlock)
{
  _precondition_(_MAGIC_,CN_METABLOCK,_FOOTER_);
    
  m_eBlock = eBlock;
}

/*----------------------------------------------------------------------------*/
/* print                                                                      */
/*----------------------------------------------------------------------------*/
void MetaBlock::vPrint(i8 i_Tab)
{
  _precondition_(_MAGIC_,CN_METABLOCK,_FOOTER_);
    
  vTrace("%sMemberSet %s %s %lldB\n"_Stz,
         
         Stz::szTab(i_Tab)->azChar(),
         m_sBlockNm.azChar(),
         aMT[m_eBlock]->azChar(),
         m_nSize
         );
  
  for (MetaCell* pCell = pHead(); pCell; pCell = pCell->pNextItem())
    pCell->vPrint(i_Tab + 1);

}

/*----------------------------------------------------------------------------*/
/* get member set name                                                        */
/*----------------------------------------------------------------------------*/
Stz* MetaBlock::szBlockNm(void)
{
  _precondition_(_MAGIC_,CN_METABLOCK,_FOOTER_);
    
  return m_sBlockNm.szStz();
}

/*----------------------------------------------------------------------------*/
/* set member set name                                                        */
/*----------------------------------------------------------------------------*/
void MetaBlock::vBlockNm(Stz* szBlockNm)
{
  _precondition_(_MAGIC_,CN_METABLOCK,_FOOTER_);
    
  m_sBlockNm = szBlockNm;
}

/*----------------------------------------------------------------------------*/
/* get rolled up size of this member set                                      */
/*----------------------------------------------------------------------------*/
i8 MetaBlock::nSize(void)
{
  _precondition_(_MAGIC_,CN_METABLOCK,_FOOTER_);
    
  return m_nSize;
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
