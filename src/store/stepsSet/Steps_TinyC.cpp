/*============================================================================*/
/* Steps_TinyC.cpp -- TinyC step group                                        */
/*============================================================================*/

#include "BASE.h"
#include "Step.hpp"
#include "EnterExit.h"
#include "StepAccum.hpp"
#include "Steps.hpp"
#include "SkipRoots.hpp"
#include "EntBuffer.hpp"
#include "RowCoder.hpp"
#include "HeadLine.hpp"
#include "TailLine.hpp"
#include "CrwBuffer.hpp"
#include "RowInflator.hpp"

#include "LogicUnit.hpp"


/*----------------------------------------------------------------------------*/
/* add primitives                                                             */
/*----------------------------------------------------------------------------*/
void Step::vAddSteps_TinyC(void)
{
  vAddStep("TinyC[].compile"_Stz,      &Step::vTinyCCompile);
  vAddStep("TinyC[].delete"_Stz,       &Step::vTinyCDelete);
  vAddStep("TinyC[].find"_Stz,         &Step::vTinyCFind);
  vAddStep("TinyC[].new"_Stz,          &Step::vTinyCNew);
  vAddStep("TinyC[].relocate"_Stz,     &Step::vTinyCRelocate);
  vAddStep("TinyC[].symbols"_Stz,      &Step::vTinyCSymbols);
}

/*----------------------------------------------------------------------------*/
/* TinyC[].compile -- compile given source using TinyC                        */
/*----------------------------------------------------------------------------*/
void Step::vTinyCCompile(void)
{
  ENTRY();

  TinyC* pTinyC = * m_p_TaskLogicUnit->m_xpTinyC;
  
  s48K sSource;
  m_pDataStack->vPopStr(sSource,__func__);

  bool bClean = pTinyC->bCompile(sSource.azChar());
  if (bClean)
    m_pDataStack->vPushInt(true);
  else
    m_pDataStack->vPushInt(false);
  
  EXIT();
}

/*----------------------------------------------------------------------------*/
/* TinyC[].delete -- delete TinyC                                             */
/*----------------------------------------------------------------------------*/
void Step::vTinyCDelete(void)
{
  ENTRY();
    
  TinyC* pTinyC = * m_p_TaskLogicUnit->m_xpTinyC;
  m_p_TaskLogicUnit->m_xpTinyC = nullptr;
  
  pTinyC->vEnd();

  delete pTinyC;

  EXIT();
}

/*----------------------------------------------------------------------------*/
/* TinyC[].find -- find symbol                                                */
/*----------------------------------------------------------------------------*/
void Step::vTinyCFind(void)
{
  ENTRY();

  TinyC* pTinyC = * m_p_TaskLogicUnit->m_xpTinyC;

  s32 sFunNm;
  m_pDataStack->vPopStr(sFunNm,__func__);

  bool (*afFun)(void*);
  afFun = (bool (*)(void*)) pTinyC->pFindSymbol(sFunNm.azChar());

  m_pDataStack->vPushInt((i8)afFun);

  EXIT();
}

/*----------------------------------------------------------------------------*/
/* TinyC[].new -- new TinyC                                                   */
/*----------------------------------------------------------------------------*/
void Step::vTinyCNew(void)
{
  ENTRY();
  
  TinyC* pTinyC = new TinyC;
  m_p_TaskLogicUnit->m_xpTinyC = pTinyC;

  pTinyC->vBegin();

  pTinyC->vAcquire(pTinyC,TinyC::vTinyError);

  EXIT();
}

/*----------------------------------------------------------------------------*/
/* TinyC[].relocate -- relocate                                               */
/*----------------------------------------------------------------------------*/
void Step::vTinyCRelocate(void)
{
  ENTRY();
   
  TinyC* pTinyC = * m_p_TaskLogicUnit->m_xpTinyC;

  if (pTinyC->bRelocate())
    m_pDataStack->vPushInt(true);
  else
    m_pDataStack->vPushInt(false);

  EXIT();
}

/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

void vNeverReach(i8 iLine) {
  RASSERT(0);
}

DynamicSymbol xaSymbolSet0[]
{
  { nullptr, nullptr },
};

DynamicSymbol xaSymbolSet1[]
{
  { "aEntBuffer_nextEntry"_Stz,  (const void *)EntBuffer::aGetNextEntry },

  { "vEntWrk_entKind"_Stz,       (const void *)EntWrk::vEntKind       },
  { "vEntWrk_payloadCreate"_Stz, (const void *)EntWrk::vPayloadCreate },
  { "vEntWrk_print"_Stz,         (const void *)EntWrk::vPrint         },

  { "aIndexDef_new"_Stz,       (const void *)IndexDef::aNewIndexDef   },
  { "vIndexDef_addColumn"_Stz, (const void *)IndexDef::vAddCol        },

  { "aIndexTree_begin"_Stz,       (const void *)IndexTree::aIndexBegin },
  { "aIndexTree_findEntry"_Stz,   (const void *)IndexTree::aFindEntry },
  { "aIndexTree_insertEnt"_Stz,   (const void *)IndexTree::aInsertEnt },
  { "aIndexTree_new"_Stz,         (const void *)IndexTree::aNewIndexTree },
  { "aIndexTree_positionGE"_Stz,   (const void *)IndexTree::aPositionGE },
  { "aIndexTree_positionLE"_Stz,   (const void *)IndexTree::aPositionLE },

  { "vInflate_into_RowSingle"_Stz, (const void *)RowInflator::vInflate_AllColumns_into_RowSingle },

  { "aRowBuffer_GetCellArray"_Stz, (const void *)RowBuffer::aGetCellArray },
  { "aRowBuffer_GetStrArea"_Stz,   (const void *)RowBuffer::szGetStrArea  },

  { "aRowSingle_GetCellArray"_Stz, (const void *)RowSingle::aGetCellArray },
  { "vRowSingle_DiscardStr"_Stz,   (const void *)RowSingle::vDiscardStr   },
  { "vRowSingle_Print"_Stz,        (const void *)RowSingle::vPrint        },
  { "vRowSingle_RowStrSize"_Stz,   (const void *)RowSingle::vRowStrSize   },

  { "aTub_AllotRow"_Stz,   (const void *)Tub::aAllotRow   },
  { "vTub_DiscardRow"_Stz, (const void *)Tub::vDiscardRow },
  { "vTub_RowStrSize"_Stz, (const void *)Tub::vRowStrSize },

  { "vNeverReach"_Stz,     (const void *)vNeverReach      },

  { nullptr, nullptr },
};

DynamicSymbol xaSymbolSet2[]
{
  { nullptr, nullptr },
};


/*----------------------------------------------------------------------------*/
/* TinyC[].symbols -- symbol set                                              */
/*----------------------------------------------------------------------------*/
void Step::vTinyCSymbols(void)
{
  ENTRY();
    
  TinyC* pTinyC = * m_p_TaskLogicUnit->m_xpTinyC;

  i8 iSymbolSet = m_pDataStack->iPopInt(__func__);

  DynamicSymbol* axaSymbolSet;
  
  switch (iSymbolSet) {
    case 0: axaSymbolSet = xaSymbolSet0; break;
    case 1: axaSymbolSet = xaSymbolSet1; break;
    case 2: axaSymbolSet = xaSymbolSet2; break;
    default:
      RASSERT(0);
  }
  
  for (i8 i_Symbol = 0; axaSymbolSet[i_Symbol].m_afSymbol; i_Symbol++)
    pTinyC->vAddSymbol((char*)axaSymbolSet[i_Symbol].m_szSymbolNm,
                       (void*)axaSymbolSet[i_Symbol].m_afSymbol);

  EXIT();
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
