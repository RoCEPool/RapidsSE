/*============================================================================*/
/* Card_Comment.cpp -- As                                                          */
/*============================================================================*/

#include "Card_Comment.hpp"

/*----------------------------------------------------------------------------*/
/*-- LifeCycle ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

__pCastFunction_Card(STORE,Card_Comment,CK_COMMENT)
__aNewFunction(Card_Comment,CN_CARD)
__vDeleteFunction(Card_Comment,CN_CARD)

__bIsCard(STORE,Card_Comment,CK_COMMENT)
__pGetSubFunction_Card(STORE,Comment,CK_COMMENT)

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
Card_Comment::Card_Comment(void)
    : Card(CK_COMMENT)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);

  _checkInitialised_(CN_CARD);

  _postcondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
Card_Comment::Card_Comment(Stz* szComment)
: Card(CK_COMMENT,szComment)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);

  //vValue(szComment);
    
  _checkInitialised_(CN_CARD);
  
  _postcondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
Card_Comment::~Card_Comment(void) noexcept(false)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_CARD,_FOOTER_);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
