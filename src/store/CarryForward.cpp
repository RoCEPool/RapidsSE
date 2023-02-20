/*============================================================================*/
/* CarryForward.cpp -- CarryForward                                           */
/*============================================================================*/

#include "CarryForward.hpp"

#include "CardIter.hpp"

/*----------------------------------------------------------------------------*/
/*-- LifeCycle ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

__pCastFunction(STORE, CarryForward, CN_CARRYFORWARD)
__aNewFunction(CarryForward, CN_CARRYFORWARD)
__vDeleteFunction(CarryForward, CN_CARRYFORWARD)
__aFooterFunction(CarryForward)

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
CarryForward::CarryForward(void)
: Object(CN_CARRYFORWARD)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_CARRYFORWARD, _FOOTER_);

  m_p_sqCrlKey = nullptr;
  m_bCrlKeyActive = false;

  m_p_sqResult = nullptr;
  m_bResultActive = false;

  m_pCF = nullptr;

  Table.m_bReproject = false;

  _postcondition_(_ADDRESS_, _MAGIC_, CN_CARRYFORWARD, _FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
CarryForward::~CarryForward(void) noexcept(false)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_CARRYFORWARD, _FOOTER_);

  delete m_pCF;
}


/*----------------------------------------------------------------------------*/
/*-- Public ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
bool CarryForward::bCrlKeyActive(void)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_CARRYFORWARD, _FOOTER_);

  return m_bCrlKeyActive;
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void CarryForward::vCrlKeyActive(bool bCrlKeyActive)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_CARRYFORWARD, _FOOTER_);

  m_bCrlKeyActive = bCrlKeyActive;
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
bool CarryForward::bIsCorrelated(void)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_CARRYFORWARD, _FOOTER_);

  return (m_p_sqCrlKey ? true : false);
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
bool CarryForward::bResultActive(void)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_CARRYFORWARD, _FOOTER_);

  return m_bResultActive;
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void CarryForward::vResultActive(bool bResultActive)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_CARRYFORWARD, _FOOTER_);

  m_bResultActive = bResultActive;
}


/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
Card_Expr* CarryForward::p_sqCrlKey(void)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_CARRYFORWARD, _FOOTER_);

  return m_p_sqCrlKey;
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void CarryForward::v_sqCrlKey(Card_Expr* p_sqCrlKey)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_CARRYFORWARD, _FOOTER_);

  m_p_sqResult = p_sqCrlKey;
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
Card_Expr* CarryForward::p_sqResult(void)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_CARRYFORWARD, _FOOTER_);

  return m_p_sqResult;
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void CarryForward::v_sqResult(Card_Expr* p_sqResult)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_CARRYFORWARD, _FOOTER_);

  m_p_sqResult = p_sqResult;
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
bool CarryForward::bWorkToDo(void)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_CARRYFORWARD, _FOOTER_);

  if (m_bResultActive || m_bCrlKeyActive)
    return true;

  return false;
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
