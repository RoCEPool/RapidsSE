/*============================================================================*/
/* Arrange.cpp -- Arrange things in alphabetic order                          */
/*============================================================================*/

#include "Utilities.h"

/*----------------------------------------------------------------------------*/
/*-- LifeCycle ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

__pCastFunction(Utilities,Arrange,CN_ARRANGE)
__aNewFunction(Arrange,CN_ARRANGE)
__vDeleteFunction(Arrange,CN_ARRANGE)
__aFooterFunction(Arrange)


/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
Arrange::Arrange(CompareFp fpCompare)
: Object(CN_ARRANGE)
, m_okLock(LK_JOBS,this)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ARRANGE,_FOOTER_);
  
  m_pSkipList = new SkipListNL(C_UNQIUE_KEY,fpCompare);
  
  _checkInitialised_(CN_ARRANGE);
  
  _postcondition_(_ADDRESS_,_MAGIC_,CN_ARRANGE,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
Arrange::~Arrange(void) noexcept(false)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ARRANGE,_FOOTER_);
  
  delete m_pSkipList;
}

/*----------------------------------------------------------------------------*/
/*-- Public ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* begin iteration                                                            */
/*----------------------------------------------------------------------------*/
void Arrange::vBeginIter(void** vppCursor)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ARRANGE,_FOOTER_);
  
  *vppCursor = nullptr;
}

/*----------------------------------------------------------------------------*/
/* end iteration                                                              */
/*----------------------------------------------------------------------------*/
void Arrange::vEndIter(void** vppCursor)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ARRANGE,_FOOTER_);

  *vppCursor = nullptr;
}

/*----------------------------------------------------------------------------*/
/* get first thing                                                            */
/*----------------------------------------------------------------------------*/
Thing* Arrange::pFirst(void** vppCursor)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ARRANGE,_FOOTER_);
  
  SkipNode* pNode = m_pSkipList->pFirst(0);
  
  *vppCursor = pNode;

  Thing* pThing = _pThing(pNode->m_pObject);
  
  return pThing;
}

/*----------------------------------------------------------------------------*/
/* get the name of the thing                                                  */
/*----------------------------------------------------------------------------*/
void Arrange::vInsert(Stz* szObjNm, Object* pObject)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ARRANGE,_FOOTER_);
  
  Thing* pThing = new Thing(szObjNm,pObject);
  
  vInsert(pThing);
}

/*----------------------------------------------------------------------------*/
/* get the name of the thing                                                  */
/*----------------------------------------------------------------------------*/
void Arrange::vInsert(Thing* pThing)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ARRANGE,_FOOTER_);
  
  m_pSkipList->bInsert(pThing);
}

/*----------------------------------------------------------------------------*/
/* get next thing                                                             */
/*----------------------------------------------------------------------------*/
Thing* Arrange::pNext(void** vppCursor)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ARRANGE,_FOOTER_);
  
  SkipNode* pNode = _pSkipNode(* vppCursor);

  pNode = m_pSkipList->pNext(pNode,0);

  *vppCursor = pNode;

  Thing* pThing = pNode ? _pThing(pNode->m_pObject) : nullptr;

  return pThing;
}

/*----------------------------------------------------------------------------*/
/* sequence number the things                                                 */
/*----------------------------------------------------------------------------*/
void Arrange::vSequence(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_ARRANGE,_FOOTER_);
  
  void* vpCursor;
  Thing* pThing;
  
  vBeginIter(&vpCursor);
  
  i8 iSeqNo = 0;
  for (pThing = pFirst(&vpCursor); pThing; pThing = pNext(&vpCursor))
    pThing->vSeqNo(iSeqNo++);
  
  vEndIter(&vpCursor);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
