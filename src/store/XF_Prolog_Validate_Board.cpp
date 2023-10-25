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

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void XF_Prolog_Validate_Board::vValidate(CardIter* pCI)
{
  _static_

  XF_Prolog_Validate_Board* pXF_Validate = new XF_Prolog_Validate_Board(pCI);

  pXF_Validate->vValidate();

  delete pXF_Validate;
}


/*----------------------------------------------------------------------------*/
/*-- public ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* ensure each op has a level                                                 */
/*----------------------------------------------------------------------------*/
void XF_Prolog_Validate_Board::vValidate(void)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_XF_PROLOG_VALIDATE_BOARD, _FOOTER_);

  m_pCI->vTransform(__func__, XF_PROLOG_VALIDATE_BOARD, "validate boards"_Stz);

  Card_OP* p_OP = nullptr;
  //Card_Scan* p_scan = nullptr;

  m_pCI->vClearRestart();
  while (m_pCI->G_bEachMain_NONE()) {
    switch (m_pCI->G.e_MainCard) {

      //case CK_SCAN: p_scan = m_pCI->G.p_scan; break;

      case CK_COMBINED:
      case CK_SEND:
        if (! p_OP) {
          Error xError(EC_MISSING_OP,(Stz*)__FILE__,__LINE__);
          throw xError;
        }
        break;

      case CK_STAGE:
        //p_scan = nullptr;
        p_OP   = nullptr;
        break;

      default:
        p_OP = m_pCI->G.p_OP;
        break;
    }
  }

//  if (! p_scan) {
//    Error xError(EC_MISSING_SCAN_OP,(Stz*)__FILE__,__LINE__);
//    throw xError;
//  }

  m_pCI->vEndTransform(__LINE__);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
