/*============================================================================*/
/* XF_Prolog_Validate_Board.cpp -- Add Level                                              */
/*============================================================================*/

#include "XF_Prolog_Validate_Board.hpp"

#include "Error.hpp"

/*----------------------------------------------------------------------------*/
/*-- LifeCycle ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

__pCastFunction(STORE, XF_Prolog_Validate_Board, CN_XF_PROLOG_VALIDATE_BOARD)
__aNewFunction(XF_Prolog_Validate_Board, CN_XF_PROLOG_VALIDATE_BOARD)
__vDeleteFunction(XF_Prolog_Validate_Board, CN_XF_PROLOG_VALIDATE_BOARD)
__aFooterFunction(XF_Prolog_Validate_Board)


/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
XF_Prolog_Validate_Board::XF_Prolog_Validate_Board(CardIter* pCI)
: XF_PROLOG(CN_XF_PROLOG_VALIDATE_BOARD, pCI)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_XF_PROLOG_VALIDATE_BOARD, _FOOTER_);

  _checkInitialised_(CN_XF_PROLOG_VALIDATE_BOARD);
  
  _postcondition_(_ADDRESS_, _MAGIC_, CN_XF_PROLOG_VALIDATE_BOARD, _FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
XF_Prolog_Validate_Board::~XF_Prolog_Validate_Board(void) noexcept(false)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_XF_PROLOG_VALIDATE_BOARD, _FOOTER_);
}


/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
