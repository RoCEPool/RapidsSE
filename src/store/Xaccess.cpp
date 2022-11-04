/*============================================================================*/
/* Xaccess.cpp --                                                       */
/*============================================================================*/

#include "Xaccess.hpp"

#include "Table.hpp"
#include "PtnDef.hpp"

/*----------------------------------------------------------------------------*/
/*-- LifeCycle ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

__aFooterFunction(Xaccess)

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
Xaccess::Xaccess(eCN eClass)
: Xbase(eClass)
{
  //_precondition_(_ADDRESS_,_MAGIC_,CN_TRANSFORMS,_FOOTER_);

}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
Xaccess::~Xaccess(void) noexcept(false)
{
  //_precondition_(_ADDRESS_,_MAGIC_,CN_TRANSFORMS,_FOOTER_);

}


/*----------------------------------------------------------------------------*/
/*-- Public ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/*A A A A A A A A A A A A A A A A A A A A A A A A A A A A A A A A A A A A A A */
/*----------------------------------------------------------------------------*/



/*----------------------------------------------------------------------------*/
/* access table                                                               */
/*----------------------------------------------------------------------------*/
Table* Xaccess::A_pAccess_Table(Stz* szTableNm)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TRANSFORMS,_FOOTER_);

  Table* pTable = AM::pTableFind(szTableNm);

  return pTable;
}

/*----------------------------------------------------------------------------*/
/* visit all sub-cards of pred and apply lambda function                      */
/*----------------------------------------------------------------------------*/
bool Xaccess::M_bPredRecursive(Card_PRED* p_pred,
                               std::function<bool(Card_PRED* p_pred, bool bUpwards)> bfApply)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TRANSFORMS,_FOOTER_);

  bool bContinue;

  eCK eCard = p_pred->eCard();

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // only call term once

  if (CK_TERM == eCard) {
    bContinue = bfApply(p_pred,false);
    return bContinue;
  }

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

  // apply pred function for downwards
  bContinue = bfApply(p_pred,false);
  if (! bContinue)
    return false;

  // descend into children
  i8 nSubCards = p_pred->nChildCards();
  for (i8 i_Child = 0; i_Child < nSubCards; i_Child++) {
    Card *pSubCard = p_pred->pGetChild(i_Child);
    bContinue = M_bPredRecursive(static_cast<Card_PRED *>(pSubCard), bfApply);
    if (! bContinue)
      return false;
  }

  // apply pred function for upwards
  bContinue = bfApply(p_pred,true);
  return bContinue;
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
