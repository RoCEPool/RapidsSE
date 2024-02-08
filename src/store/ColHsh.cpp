/*============================================================================*/
/* ColHsh.cpp -- Columns Required Metadata                               */
/*============================================================================*/

#include "ColHsh.hpp"

#include "Card_Term.hpp"
#include "Kid.hpp"

/*----------------------------------------------------------------------------*/
/*-- LifeCycle ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

__pCastFunction(STORE,ColHsh,CN_COLHSH)
__aNewFunction(ColHsh,CN_COLHSH)
__vDeleteFunction(ColHsh,CN_COLHSH)
__aFooterFunction(ColHsh)


/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
ColHsh::ColHsh(void)
    : Object(CN_COLHSH)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_COLHSH, _FOOTER_);

  for (i8 i_Col = 0; i_Col < C_COLHSH; i_Col++) {
    m_axCols[i_Col].m_i_Col       = C_UNKNOWN;
    m_axCols[i_Col].m_eColumnType = CT_NONE;
    m_axCols[i_Col].m_sHashOf     = "?"_Stz;
  }
  m_nCols = 0;

  _postcondition_(_ADDRESS_,_MAGIC_,CN_COLHSH,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
ColHsh::~ColHsh(void) noexcept(false)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_COLHSH,_FOOTER_);

}


/*----------------------------------------------------------------------------*/
/*-- static ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/


/*----------------------------------------------------------------------------*/
/*-- public ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* add column mapping                                                         */
/*----------------------------------------------------------------------------*/
void ColHsh::vAdd(i8 i_Col, eCT eColumnType, Stz* szHashOf)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_COLHSH, _FOOTER_);

  m_axCols[m_nCols].m_i_Col       = i_Col;
  m_axCols[m_nCols].m_eColumnType = eColumnType;
  m_axCols[m_nCols].m_sHashOf     = szHashOf;

  m_nCols++;

  vValidate();
}

/*----------------------------------------------------------------------------*/
/* dump                                                                       */
/*----------------------------------------------------------------------------*/
void ColHsh::vDump(i8 i_Tab)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_COLHSH, _FOOTER_);
  UNUSED(i_Tab);

  for (i8 i_Col = 0; i_Col < m_nCols; i_Col++) {
    vTrace("               [%lld] %lld %s %s\n"_Stz,
           i_Col,
           m_axCols[i_Col].m_i_Col,
           aCT[m_axCols[i_Col].m_eColumnType],
           m_axCols[i_Col].m_sHashOf.szStz()
           );
  }
}

/*----------------------------------------------------------------------------*/
/* validate                                                                   */
/*----------------------------------------------------------------------------*/
void ColHsh::vUpdate(Card_Hashing* p_hashing)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_COLHSH, _FOOTER_);

  Kid xHashing(p_hashing);
  while (xHashing.bEach()) {
    Card_Term* p_term = xHashing.p_term();

    Symbol* pSymbol = p_term->pSymbol();

    vAdd(p_term->pSymbol()->iTo_ordinal() - 1,
         p_term->eCTVal(),
         pSymbol->Y_szGet(P_HashOf));

  }
}

/*----------------------------------------------------------------------------*/
/* validate                                                                   */
/*----------------------------------------------------------------------------*/
void ColHsh::vValidate(void)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_COLHSH, _FOOTER_);


}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
