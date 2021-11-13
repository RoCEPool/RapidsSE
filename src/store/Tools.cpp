/*============================================================================*/
/* Tools.cpp -- Drafting Tools                                                */
/*============================================================================*/

#include "Tools.hpp"

/*----------------------------------------------------------------------------*/
/*-- LifeCycle ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

__pCastFunction(STORE,Tools,CN_TOOLS)
__aNewFunction(Tools,CN_TOOLS)
__vDeleteFunction(Tools,CN_TOOLS)
__aFooterFunction(Tools)

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
Tools::Tools(void)
: Object(CN_TOOLS)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TOOLS,_FOOTER_);
    
  _checkInitialised_(CN_TOOLS);
  
  _postcondition_(_ADDRESS_,_MAGIC_,CN_TOOLS,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
Tools::~Tools(void) noexcept(false)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TOOLS,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/*-- Public ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* expand stream definitions                                                  */
/*----------------------------------------------------------------------------*/
void Tools::vExpandStreamDefs(Board* pBoard)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TOOLS,_FOOTER_);
  
//  auto fClosure = [](Object* pObject, i8 iDepth) {
//
//    eCN  eClassNo  = pObject->eGetClassNo();
//    Stz* szClassNm = aCN[eClassNo];
//
//    vTrace("%sClosure: %p %s:%03lld\n"_Stz
//           ,Stz::szTab(iDepth)
//           ,pObject
//           ,szClassNm
//           ,eClassNo);
//  };
  
//  ListExoIter2_t<Card>* pBoard_I = pBoard->pNewIter();
//  while (pBoard_I->bHasNext()) {
//    Card* pCard = pBoard_I->pNext();
//
//    if (CN_CARD_SCAN != pCard->eGetClassNo())
//      continue;
//
//    Card_Scan* pCard_Scan = _pCard_Scan(pCard);
//  }
//  delete pBoard_I;

  auto fClosure = [](Card* pCard)
  {
    vTrace("%s\n"_Stz,pCard->szCardNm());
  };

  pBoard->vWalk(fClosure);
  
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
