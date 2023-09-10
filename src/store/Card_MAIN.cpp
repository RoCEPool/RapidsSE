/*============================================================================*/
/* Card_MAIN.cpp -- Abstract OP                                                 */
/*============================================================================*/

#include "Card_MAIN.hpp"

#include "Card_Level.hpp"
#include "Card_Table.hpp"
#include "CardIter.hpp"
#include "Table.hpp"
#include "Index.hpp"
#include "ColsReq.hpp"

/*----------------------------------------------------------------------------*/
/*-- LifeCycle ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
Card_MAIN::Card_MAIN(eCK eCard, i8 iFixedDepth)
: Card_symbol(eCard, iFixedDepth)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);

  _postcondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
Card_MAIN::~Card_MAIN(void) noexcept(false)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);
}


/*----------------------------------------------------------------------------*/
/*-- Static ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* test if eCard is a main                                                    */
/*----------------------------------------------------------------------------*/
bool Card::bIsMain(eCK eCard)
{
  _static_

  switch (eCard) {

    case CK_ABSJOIN:
    case CK_AGGREG:
    case CK_CALL:
    case CK_CAST:
    case CK_COMBINED:
    case CK_CORRELATION:
    case CK_CROSSJOIN:
    case CK_FILTER:
    case CK_FLAT:
    case CK_FOLD:
    case CK_FROM:
    case CK_GBYAGG:
    case CK_HASH:
    case CK_HAVING:
    case CK_INDEXSCAN:
    case CK_INDEXJOIN:
    case CK_KEYEXISTS:
    case CK_KEYNOTEXISTS:
    case CK_LIMIT:
    case CK_PROJECT:
    case CK_SEND:
    case CK_SCAN:
    case CK_SORT:
    case CK_STAGE:
    case CK_TOP:
    case CK_WHERE:
      return true;

    default:
      return false;
  }
}


/*----------------------------------------------------------------------------*/
/*-- Public ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/


/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Card_MAIN::vValidate(CardIter* pCI, i8 iDepth, Card* pParent)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_CARD, _FOOTER_);
  UNUSED(pCI);
  UNUSED(iDepth);
  UNUSED(pParent);

  RASSERT(1 == iDepth);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
