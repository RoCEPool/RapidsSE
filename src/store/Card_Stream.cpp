/*============================================================================*/
/* Card_Stream.cpp -- Stream                                                  */
/*============================================================================*/

#include "Card_Stream.hpp"
#include "Card_Expr.hpp"
#include "Card_ObyElist.hpp"

/*----------------------------------------------------------------------------*/
/*-- LifeCycle ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

__pCastFunction_Card(STORE,Card_Stream,CK_STREAM)
__aNewFunction(Card_Stream,CN_CARD)
__vDeleteFunction(Card_Stream,CN_CARD)



/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
Card_Stream::Card_Stream(void)
: Card(CK_STREAM)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);
          
  _checkInitialised_(CN_CARD);
  
  _postcondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
Card_Stream::~Card_Stream(void) noexcept(false)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/*-- Public ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/


/*----------------------------------------------------------------------------*/
/* get number of stream columns                                               */
/*----------------------------------------------------------------------------*/
void Card_Stream::vStreamPrint(Ribbon* pRibbon, i8 iDepth)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_CARD, _FOOTER_);

  s32 sName;
  sName.vSprint("%s%s"_Stz,szCardNm(),nProperties()?" +":"");

  if (bHasProperty(P_OrdLo))
    sName.vCatSprint(" $%lld,%lld"_Stz,
                     Y_iVal(P_OrdLo), Y_iVal(P_OrdHi));

  vPrintCard_byCustomNm(pRibbon, iDepth, sName);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
