/*============================================================================*/
/* PdsCursor.cpp -- Cursor for Primary Data Structure (PDS)                   */
/*============================================================================*/

#include "PdsCursor.hpp"

/*----------------------------------------------------------------------------*/
/*-- LifeCycle ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

__pCastFunction(BASE,PdsCursor,CN_PDSCURSOR)
__aNewFunction(PdsCursor,CN_PDSCURSOR)
__vDeleteFunction(PdsCursor,CN_PDSCURSOR)
__aFooterFunction(PdsCursor)

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
PdsCursor::PdsCursor(Pds* pPds)
    : Object(CN_PDSCURSOR)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_PDSCURSOR,_FOOTER_);

  m_pPds = pPds;

  m_pCurrentBlock = pPds->pFirstBlock();

  _postcondition_(_ADDRESS_,_MAGIC_,CN_PDSCURSOR,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* constructor (set cursor to start at a specific point in the skip list      */
/*----------------------------------------------------------------------------*/
PdsCursor::PdsCursor(Pds* pPds, SkipListBlock* pCurr)
    : Object(CN_PDSCURSOR)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_PDSCURSOR,_FOOTER_);

  RASSERT(pPds);
  RASSERT(pCurr);

  m_pPds = pPds;

  vCurr(pCurr);

  _postcondition_(_ADDRESS_,_MAGIC_,CN_PDSCURSOR,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
PdsCursor::~PdsCursor(void) noexcept(false)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_PDSCURSOR,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* repair the cursor                                                          */
/*----------------------------------------------------------------------------*/
void PdsCursor::vRepair(Object *pObject)
{
  _static_

  PdsCursor *pPdsCursor = _pPdsCursor(pObject);

  // delete the cursor
  delete pPdsCursor;
}

/*----------------------------------------------------------------------------*/
/* is the current block null?                                                 */
/*----------------------------------------------------------------------------*/
bool PdsCursor::bIsCurrNull(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_PDSCURSOR,_FOOTER_);

  if(!m_pCurrentBlock)
    return true;
  else
    return false;
}

/*----------------------------------------------------------------------------*/
/* set current block                                                          */
/*----------------------------------------------------------------------------*/
void PdsCursor::vCurr(SkipListBlock *pCurr)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_PDSCURSOR,_FOOTER_);

  m_pCurrentBlock = pCurr;
}

/*----------------------------------------------------------------------------*/
/* get current row                                                            */
/*----------------------------------------------------------------------------*/
Row* PdsCursor::pCurrRow(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_PDSCURSOR,_FOOTER_);

  if( ! m_pCurrentBlock)
    return nullptr;

  return m_pCurrentBlock->aRow();
}

/*----------------------------------------------------------------------------*/
/* get current row's key                                                      */
/*----------------------------------------------------------------------------*/
void* PdsCursor::pCurrKey(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_PDSCURSOR,_FOOTER_);

  if( ! m_pCurrentBlock)
    return nullptr;

  return m_pCurrentBlock->aItem();
}

/*----------------------------------------------------------------------------*/
/* is the current block null?                                                 */
/*----------------------------------------------------------------------------*/
void PdsCursor::vNext(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_PDSCURSOR,_FOOTER_);

  if(!m_pCurrentBlock)
    return;

  m_pCurrentBlock = m_pPds->pNextSLBlock(m_pCurrentBlock, 0);
}

/*----------------------------------------------------------------------------*/
/* reset the cursor to the first block (can be null)                          */
/*----------------------------------------------------------------------------*/
void PdsCursor::vReset(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_PDSCURSOR,_FOOTER_);

  m_pCurrentBlock = m_pPds->pFirstBlock();
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/