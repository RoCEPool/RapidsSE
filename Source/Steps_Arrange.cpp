/*============================================================================*/
/* Steps_Arrange.cpp - arrange step group                                     */
/*============================================================================*/

#include "Steps.h"

/*----------------------------------------------------------------------------*/
/* add primitives                                                             */
/*----------------------------------------------------------------------------*/
void Step::vAddSteps_Arrange(void)
{
  vAddStep("arrange::new"_Stz,     &Step::vArrangeNew);
  
  vAddStep("arrange.delete"_Stz,   &Step::vArrangeDelete);
  vAddStep("arrange.end"_Stz,      &Step::vArrangeEnd);
  vAddStep("arrange.first"_Stz,    &Step::vArrangeFirst);
  vAddStep("arrange.insert"_Stz,   &Step::vArrangeInsert);
  vAddStep("arrange.name"_Stz,     &Step::vArrangeName);
  vAddStep("arrange.next"_Stz,     &Step::vArrangeNext);
  vAddStep("arrange.object"_Stz,   &Step::vArrangeObject);
  vAddStep("arrange.seqNo"_Stz,    &Step::vArrangeSeqNo);
  vAddStep("arrange.sequence"_Stz, &Step::vArrangeSequence);
}

/*----------------------------------------------------------------------------*/
/* arrange::new -- new arrange                                                */
/*----------------------------------------------------------------------------*/
void Step::vArrangeNew(void)
{
  ENTRY();
  
  Arrange* pArrange = new Arrange;
     
  m_pDataStack->vPushPtr(pArrange);

  EXIT();
}

/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* arrange.begin -- begin iteration of arrange                                */
/*----------------------------------------------------------------------------*/
void Step::vArrangeBegin(void)
{
  ENTRY();

  Arrange* pArrange = _pArrange(m_pDataStack->pPopPtr(__func__));
  
  SkipNode* pSkipNode;

  pArrange->vBeginIter((void**)&pSkipNode);
  
  m_pDataStack->vPushPtr(pSkipNode);

  EXIT();
}

/*----------------------------------------------------------------------------*/
/* arrange.delete -- delete arrange                                           */
/*----------------------------------------------------------------------------*/
void Step::vArrangeDelete(void)
{
  ENTRY();
  
  Arrange* pArrange = _pArrange(m_pDataStack->pPopPtr(__func__));

  delete pArrange;
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* arrange.end -- end teration of arrange                                     */
/*----------------------------------------------------------------------------*/
void Step::vArrangeEnd(void)
{
  ENTRY();
  
  Arrange* pArrange = _pArrange(m_pDataStack->pPopPtr(__func__));
  
  SkipNode* pSkipNode = _pSkipNode(m_pDataStack->pPopPtr(__func__));

  pArrange->vEndIter((void**)&pSkipNode);

  EXIT();
}

/*----------------------------------------------------------------------------*/
/* arrange.first -- move to first thing in arrange                            */
/*----------------------------------------------------------------------------*/
void Step::vArrangeFirst(void)
{
  ENTRY();

  Arrange* pArrange = _pArrange(m_pDataStack->pPopPtr(__func__));
  
  SkipNode* pSkipNode = _pSkipNode(m_pDataStack->pPopPtr(__func__));

  bool bFirst = pArrange->bFirst((void**)&pSkipNode);
  
  if (bFirst) {
    m_pDataStack->vPushPtr(nullptr);
  }
  else {
    m_pDataStack->vPushPtr(pSkipNode);
  }
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* arrange.insert -- insert thing into arrange                                */
/*----------------------------------------------------------------------------*/
void Step::vArrangeInsert(void)
{
  ENTRY();

  Arrange* pArrange = _pArrange(m_pDataStack->pPopPtr(__func__));

  Object* pObject = m_pDataStack->pPopPtr(__func__);

  s256 sObjNm;
  m_pDataStack->vPopStr(sObjNm,__func__);
  
  pArrange->vInsert(sObjNm,pObject);

  EXIT();
}

/*----------------------------------------------------------------------------*/
/* arrange.name -- get name of current thing                                  */
/*----------------------------------------------------------------------------*/
void Step::vArrangeName(void)
{
  ENTRY();

  Arrange* pArrange = _pArrange(m_pDataStack->pPopPtr(__func__));
    
  SkipNode* pSkipNode = _pSkipNode(m_pDataStack->pPopPtr(__func__));

  Stz* szObjNm = pArrange->szObjNm((void**)&pSkipNode);
  
  m_pDataStack->vPushStz(szObjNm);

  EXIT();
}

/*----------------------------------------------------------------------------*/
/* arrange.next -- move to next thing in arrange                              */
/*----------------------------------------------------------------------------*/
void Step::vArrangeNext(void)
{
  ENTRY();

  Arrange* pArrange = _pArrange(m_pDataStack->pPopPtr(__func__));

  SkipNode* pSkipNode = _pSkipNode(m_pDataStack->pPopPtr(__func__));
  
  bool bNext = pArrange->bNext((void**)&pSkipNode);
  
  if (bNext) {
    m_pDataStack->vPushPtr(nullptr);
  }
  else {
    m_pDataStack->vPushPtr(pSkipNode);
  }
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* arrange.object -- get object of current thing                              */
/*----------------------------------------------------------------------------*/
void Step::vArrangeObject(void)
{
  ENTRY();

  Arrange* pArrange = _pArrange(m_pDataStack->pPopPtr(__func__));

  SkipNode* pSkipNode = _pSkipNode(m_pDataStack->pPopPtr(__func__));

  Object* pObject = pArrange->pObject((void**)&pSkipNode);
  
  m_pDataStack->vPushPtr(pObject);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* arrange.seqNo -- get sequence number of current thing                      */
/*----------------------------------------------------------------------------*/
void Step::vArrangeSeqNo(void)
{
  ENTRY();

  Arrange* pArrange = _pArrange(m_pDataStack->pPopPtr(__func__));
  
  SkipNode* pSkipNode = _pSkipNode(m_pDataStack->pPopPtr(__func__));

  i8 iSeqNo = pArrange->iSeqNo((void**)&pSkipNode);
  
  m_pDataStack->vPushInt(iSeqNo);

  EXIT();
}

/*----------------------------------------------------------------------------*/
/* arrange.sequence -- sequence number the things in arrange                  */
/*----------------------------------------------------------------------------*/
void Step::vArrangeSequence(void)
{
  ENTRY();
  
  Arrange* pArrange = _pArrange(m_pDataStack->pPopPtr(__func__));
  
  pArrange->vSequence();

  EXIT();
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
