/*============================================================================*/
/* Stencil.cpp -- Op Stencil                                                  */
/*============================================================================*/

#include "BASE.h"

#include "Stencil.hpp"

ShapeRef xaShapeRef[]
{
  { "none"_Stz         }, // LS_NONE
  { "LS_INFLATE_T"_Stz }, // LS_INFLATE_T
  { "LS_FILTER_T"_Stz  }, // LS_FILTER_T
  { "LS_PROJECT_T"_Stz }, // LS_PROJECT_T
  { "LS_JOIN_KEY"_Stz  }, // LS_JOIN_KEY
  { "LS_JOIN"_Stz      }, // LS_JOIN
  { "LS_INFLATE_J"_Stz }, // LS_INFLATE_J
  { "LS_PROJECT_J"_Stz }, // LS_PROJECT_J
  { "LS_AGG_KEY"_Stz   }, // LS_AGG_KEY
  { "LS_AGG"_Stz       }, // LS_AGG
  { "LS_fetch"_Stz     }, // LS_fetch
  { "LS_PROJECT_H"_Stz }, // LS_PROJECT_H
  { "LS_FILTER_H"_Stz  }, // LS_FILTER_H
  { "LS_OUT_H"_Stz     }, // LS_OUT_H
  
  { nullptr            },
};

/*----------------------------------------------------------------------------*/
/*-- LifeCycle ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

__pCastFunction(STORE,Stencil,CN_STENCIL)
__aNewFunction(Stencil,CN_STENCIL)
__vDeleteFunction(Stencil,CN_STENCIL)
__aFooterFunction(Stencil)

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
Stencil::Stencil(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_STENCIL,_FOOTER_);
    
  m_ePattern    = LP_NONE;

  m_pBannerHead = nullptr;
  m_pBannerTail = nullptr;
  m_pOpSource   = nullptr;

  _checkInitialised_(CN_STENCIL);
  
  _postcondition_(_ADDRESS_,_MAGIC_,CN_STENCIL,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
Stencil::~Stencil(void) noexcept(false)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_STENCIL,_FOOTER_);

  delete m_pBannerHead;
  delete m_pBannerTail;
  delete m_pOpSource;
}

/*----------------------------------------------------------------------------*/
/*-- Static ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* repair and delete the stencil                                              */
/*----------------------------------------------------------------------------*/
void Stencil::vRepair(Object* pObject)
{
  _static_

  Stencil* pStencil = _pStencil(pObject);

  // repair the stencil
  // ...
  
  // delete the stencil object
  delete pStencil;
}


/*----------------------------------------------------------------------------*/
/*-- Public ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* combine op                                                                 */
/*----------------------------------------------------------------------------*/
void Stencil::vCombine(Op* pOp)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_STENCIL,_FOOTER_);
  
  UNUSED(pOp);
}

/*----------------------------------------------------------------------------*/
/* return op source code                                                      */
/*----------------------------------------------------------------------------*/
Stz* Stencil::szOpSource(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_STENCIL,_FOOTER_);
  
  char* azCode = m_pOpSource->azCode();
  
  return (Stz*)azCode;
}

/*----------------------------------------------------------------------------*/
/* get stencil pattern                                                        */
/*----------------------------------------------------------------------------*/
eLP Stencil::ePattern(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_STENCIL,_FOOTER_);

  return m_ePattern;
}

/*----------------------------------------------------------------------------*/
/* set stencil pattern                                                        */
/*----------------------------------------------------------------------------*/
void Stencil::vPattern(eLP ePattern)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_STENCIL,_FOOTER_);

  m_ePattern = ePattern;
}

/*----------------------------------------------------------------------------*/
/* sketch source from pattern                                                 */
/*----------------------------------------------------------------------------*/
void Stencil::vSketch(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_STENCIL,_FOOTER_);

}


/*----------------------------------------------------------------------------*/
/*-- Private -----------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

//*============================================================================*/
/* end                                                                        */
/*============================================================================*/
