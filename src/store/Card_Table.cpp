/*============================================================================*/
/* Card_Table.cpp -- Table                                                    */
/*============================================================================*/

#include "Card_Table.hpp"

#include "Card_IndexPtr.hpp"
#include "Card_IdxColList.hpp"
#include "Card_IndexList.hpp"
#include "Card_TablePtr.hpp"

#include "PtnDef.hpp"

/*----------------------------------------------------------------------------*/
/*-- LifeCycle ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

__pCastFunction_Card(STORE, Card_Table, CK_TABLE)
__aNewFunction(Card_Table, CN_CARD)
__vDeleteFunction(Card_Table, CN_CARD)

__bIsCard(STORE, Card_Table, CK_TABLE)


/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
Card_Table::Card_Table(void)
: Card(CK_TABLE)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);
  
  _checkInitialised_(CN_CARD);
  
  _postcondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
Card_Table::Card_Table(Stz* szTableNm)
    : Card(CK_TABLE,szTableNm)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);

  _checkInitialised_(CN_CARD);

  _postcondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
Card_Table::~Card_Table(void) noexcept(false)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);
}


/*----------------------------------------------------------------------------*/
/*-- Public ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
Card_TablePtr* Card_Table::pGet_tablePtr(void)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_CARD, _FOOTER_);

  Card_TablePtr* p_tablePtr = _pCard_TablePtr(this->pFindSubCard(CK_TABLEPTR));

  return p_tablePtr;
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Card_Table::vTablePrint(Ribbon* pRibbon, i8 iDepth)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_CARD, _FOOTER_);

  s32 sName;
  sName = aCK_short[m_eCard];
  if (0 == iDepth)
    sName.vToUpper();

  if (bHasProperty(PPY_TableSyn))
    sName.vCatSprint(" %c"_Stz, Y_cVal(PPY_TableSyn));
  else
    sName.vCatSprint(" %s"_Stz, szValue());

  vPrintCard_byCustomNm(pRibbon, iDepth, sName);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
