/*============================================================================*/
/* LocalFrame.cpp -- Locals Frame Chain                                       */
/*============================================================================*/

#include "BASE.h"


/*----------------------------------------------------------------------------*/
/*-- LifeCycle ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

__pCastFunction(BASE,LocalFrame,CN_LOCALCHAIN)
__aNewFunction(LocalFrame,CN_LOCALCHAIN)
__vDeleteFunction(LocalFrame,CN_LOCALCHAIN)
__aFooterFunction(LocalFrame)


/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
LocalFrame::LocalFrame(void)
: Object(CN_LOCALCHAIN)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_LOCALCHAIN,_FOOTER_);

  m_pNext         = nullptr;
  m_aDiscardPoint = nullptr;
  //m_xLocal
  
  _checkInitialised_(CN_LOCALCHAIN);
  
  _postcondition_(_ADDRESS_,_MAGIC_,CN_LOCALCHAIN,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
LocalFrame::~LocalFrame(void) noexcept(false)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_LOCALCHAIN,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* dump register frame                                                        */
/*----------------------------------------------------------------------------*/
void LocalFrame::vDump(eCN eClass, Object* pObject, Stz* aTab)
{
  UNUSED(eClass);
  UNUSED(aTab);
  
  LocalFrame* pLocalFrame = _pLocalFrame(pObject);
  UNUSED(pLocalFrame);

  vTrace("\n"_Stz);
}

/*----------------------------------------------------------------------------*/
/*-- Public ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* get return point where this local set should be discarded                  */
/*----------------------------------------------------------------------------*/
TypeValueArea* LocalFrame::aDiscardPoint(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_LOCALCHAIN,_FOOTER_);
  
  return m_aDiscardPoint;
}

/*----------------------------------------------------------------------------*/
/* set return point where this local set should be discarded                  */
/*----------------------------------------------------------------------------*/
void LocalFrame::vDiscardPoint(TypeValueArea* aDiscardPoint)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_LOCALCHAIN,_FOOTER_);
  
  m_aDiscardPoint = aDiscardPoint;
}

/*----------------------------------------------------------------------------*/
/* dump the local chain                                                       */
/*----------------------------------------------------------------------------*/
void LocalFrame::vDump(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_LOCALCHAIN,_FOOTER_);
  
  vTrace("LocalChain: %p discardPoint %p"_Stz,
         this,
         m_aDiscardPoint);
  
  if (m_pNext)
    m_pNext->vDump();
}

/*----------------------------------------------------------------------------*/
/* get given local                                                            */
/*----------------------------------------------------------------------------*/
SenCell* LocalFrame::pSenCell(i8 i_Local)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_LOCALCHAIN,_FOOTER_);
  
  ASSERT(i_Local >= 0 && i_Local < C_LOCALS_PER_FRAME);
  
  return &m_xaSenCell[i_Local];
}

/*----------------------------------------------------------------------------*/
/* get next set of locals                                                     */
/*----------------------------------------------------------------------------*/
LocalFrame* LocalFrame::pNext(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_LOCALCHAIN,_FOOTER_);
  
  return m_pNext;
}

/*----------------------------------------------------------------------------*/
/* set next set of locals                                                     */
/*----------------------------------------------------------------------------*/
void LocalFrame::vNext(LocalFrame* pNext)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_LOCALCHAIN,_FOOTER_);
  
  m_pNext = pNext;
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
