/*============================================================================*/
/* MetaCell.cpp - Meta Block Cell                                             */
/*============================================================================*/

#include "Reflection.h"

/*----------------------------------------------------------------------------*/
/*-- LifeCycle ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

__pCastFunction(Reflection,MetaCell,CN_METACELL)
__aNewFunction(MetaCell,CN_METACELL)
__vDeleteFunction(MetaCell,CN_METACELL)
__aFooterFunction(MetaCell)


/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
MetaCell::MetaCell(Stz* szCellNm, i8 i_Cell, i8 i_Offset, i8 nSize,
                   MetaBlock* p_WithinBlock, MetaMember* p_MetaMember)
: Object(CN_METACELL)
{
  _precondition_(_MAGIC_,CN_METACELL,_FOOTER_);
  
  m_p_WithinBlock  = p_WithinBlock;
  m_p_MetaMember   = p_MetaMember;

  m_sCellNm  = szCellNm;
  m_i_Cell   = i_Cell;
  m_i_Offset = i_Offset;
  m_nSize    = nSize;

  _checkInitialised_(CN_METACELL);
  
  _postcondition_(_MAGIC_,CN_METACELL,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
MetaCell::~MetaCell(void) noexcept(false)
{
  _precondition_(_MAGIC_,CN_METACELL,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/*-- Static ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/*-- Public ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* get which member set this member var is within                             */
/*----------------------------------------------------------------------------*/
 Stz* MetaCell::szCellNm(void)
{
  _precondition_(_MAGIC_,CN_METACELL,_FOOTER_);
    
  return m_sCellNm;
}

/*----------------------------------------------------------------------------*/
/* get which member set this member var is within                             */
/*----------------------------------------------------------------------------*/
MetaBlock* MetaCell::p_WithinBlock(void)
{
  _precondition_(_MAGIC_,CN_METACELL,_FOOTER_);
    
  return m_p_WithinBlock;
}

/*----------------------------------------------------------------------------*/
/* get which member set this member var is within                             */
/*----------------------------------------------------------------------------*/
MetaMember* MetaCell::p_MetaMember(void)
{
  _precondition_(_MAGIC_,CN_METACELL,_FOOTER_);
    
  return m_p_MetaMember;
}

/*----------------------------------------------------------------------------*/
/* print                                                                      */
/*----------------------------------------------------------------------------*/
void MetaCell::vPrint(i8 i_Tab)
{
  _precondition_(_MAGIC_,CN_METACELL,_FOOTER_);
     
  vTrace("%sCell %-16s %lldB\n"_Stz,
         
         Stz::szTab(i_Tab)->azChar(),
         m_sCellNm.azChar(),
         m_nSize,
         
         C_VARGS_END);
}

/*----------------------------------------------------------------------------*/
/* get rolled-up size of this member                                          */
/*----------------------------------------------------------------------------*/
i8 MetaCell::nSize(void)
{
  _precondition_(_MAGIC_,CN_METACELL,_FOOTER_);
    
  return m_nSize;
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
