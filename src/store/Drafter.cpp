/*============================================================================*/
/* Drafter.cpp --                                                               */
/*============================================================================*/

#include "Drafter.hpp"
#include "Optimiser.hpp"

/*----------------------------------------------------------------------------*/
/*-- LifeCycle ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

__pCastFunction(STORE,Drafter,CN_DRAFTER)
__aNewFunction(Drafter,CN_DRAFTER)
__vDeleteFunction(Drafter,CN_DRAFTER)
__aFooterFunction(Drafter)

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
Drafter::Drafter(Optimiser* pOptimiser)
: Object(CN_DRAFTER)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_DRAFTER,_FOOTER_);
    
  m_pOptimiser = pOptimiser;
  m_pCardCtl   = m_pOptimiser->pCardCtl();

  _checkInitialised_(CN_DRAFTER);
  
  _postcondition_(_ADDRESS_,_MAGIC_,CN_DRAFTER,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
Drafter::~Drafter(void) noexcept(false)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_DRAFTER,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/*-- static ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* repair the ruler                                                           */
/*----------------------------------------------------------------------------*/
void Drafter::vRepair(Object* pObject)
{
  _static_

  Drafter* pDrafter = _pDrafter(pObject);

  delete pDrafter;
}


/*----------------------------------------------------------------------------*/
/*-- Public ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* add                                                                        */
/*----------------------------------------------------------------------------*/
bool Drafter::bAdd(TokenVec* pTknVec, i8& riAction)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_DRAFTER,_FOOTER_);

  return false;
}

/*----------------------------------------------------------------------------*/
/* below                                                                      */
/*----------------------------------------------------------------------------*/
bool Drafter::bBelow(TokenVec* pTknVec, i8& riAction)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_DRAFTER,_FOOTER_);

  return false;
}

/*----------------------------------------------------------------------------*/
/* clear                                                                      */
/*----------------------------------------------------------------------------*/
bool Drafter::bClear(TokenVec* pTknVec, i8& riAction)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_DRAFTER,_FOOTER_);

  return false;
}

/*----------------------------------------------------------------------------*/
/* compare                                                                    */
/*----------------------------------------------------------------------------*/
bool Drafter::bCmp(TokenVec* pTknVec, i8& riAction)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_DRAFTER,_FOOTER_);

  return false;
}

/*----------------------------------------------------------------------------*/
/* cut                                                                        */
/*----------------------------------------------------------------------------*/
bool Drafter::bCut(TokenVec* pTknVec, i8& riAction)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_DRAFTER,_FOOTER_);

  return false;
}

/*----------------------------------------------------------------------------*/
/* depth                                                                      */
/*----------------------------------------------------------------------------*/
bool Drafter::bDepth(TokenVec* pTknVec, i8& riAction)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_DRAFTER,_FOOTER_);

  return false;
}

/*----------------------------------------------------------------------------*/
/* form                                                                       */
/*----------------------------------------------------------------------------*/
bool Drafter::bForm(TokenVec* pTknVec, i8& riAction)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_DRAFTER,_FOOTER_);

  return false;
}

/*----------------------------------------------------------------------------*/
/* add                                                                        */
/*----------------------------------------------------------------------------*/
bool Drafter::bHide(TokenVec* pTknVec, i8& riAction)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_DRAFTER,_FOOTER_);

  return false;
}

/*----------------------------------------------------------------------------*/
/* load                                                                       */
/*----------------------------------------------------------------------------*/
bool Drafter::bLoad(TokenVec* pTknVec, i8& riAction)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_DRAFTER,_FOOTER_);

  return false;
}

/*----------------------------------------------------------------------------*/
/* name                                                                       */
/*----------------------------------------------------------------------------*/
bool Drafter::bName(TokenVec* pTknVec, i8& riAction)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_DRAFTER,_FOOTER_);

  return false;
}

/*----------------------------------------------------------------------------*/
/* reprint                                                                    */
/*----------------------------------------------------------------------------*/
bool Drafter::bReprint(TokenVec* pTknVec, i8& riAction)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_DRAFTER,_FOOTER_);

  return false;
}

/*----------------------------------------------------------------------------*/
/* set                                                                        */
/*----------------------------------------------------------------------------*/
bool Drafter::bSet(TokenVec* pTknVec, i8& riAction)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_DRAFTER,_FOOTER_);

  return false;
}

/*----------------------------------------------------------------------------*/
/* sub                                                                        */
/*----------------------------------------------------------------------------*/
bool Drafter::bSub(TokenVec* pTknVec, i8& riAction)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_DRAFTER,_FOOTER_);

  return false;
}

/*----------------------------------------------------------------------------*/
/* weave                                                                      */
/*----------------------------------------------------------------------------*/
bool Drafter::bWeave(TokenVec* pTknVec, i8& riAction)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_DRAFTER,_FOOTER_);

  vTrace("%s\n"_Stz
         ,m_pOptimiser->szWeave()
         );
  
  riAction = 1;
  
  return true;
}

/*----------------------------------------------------------------------------*/
/* write                                                                      */
/*----------------------------------------------------------------------------*/
bool Drafter::bWrite(TokenVec* pTknVec, i8& riAction)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_DRAFTER,_FOOTER_);

  return false;
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
