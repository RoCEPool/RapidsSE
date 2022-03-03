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
  
  Stencil* pStencil;
  pStencil = _pStencil(m_pDataStack->pPopPtr(__func__));

  bool bClean = pTinyC->bCompile(pStencil->szOpSource()->azChar());
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

  if (pTinyC->bRelocate(m_p_TaskLogicUnit->m_aTinyFunc))
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
  { "vEnt_payloadCreate"_Stz,          (const void *)Ent::vPayloadCreate         },

  { "aEntBuffer_nextEntry"_Stz,        (const void *)EntBuffer::aGetNextEntry    },

  { "vEntWrk_appendElementEntInt"_Stz, (const void *)EntWrk::vAppendElementEntInt},
  { "vEntWrk_appendElementEntStz"_Stz, (const void *)EntWrk::vAppendElementEntStz},
  { "vEntWrk_entKind"_Stz,             (const void *)EntWrk::vEntKind            },
  { "vEntWrk_payloadCreate"_Stz,       (const void *)EntWrk::vPayloadCreate      },
  { "vEntWrk_print"_Stz,               (const void *)EntWrk::vPrint              },

  { "aIndexTree_begin"_Stz,            (const void *)IndexTree::aIndexBegin      },
  { "aIndexTree_findEntry"_Stz,        (const void *)IndexTree::aFindEntry       },
  { "aIndexTree_insertEnt"_Stz,        (const void *)IndexTree::aInsertEnt       },
  { "aIndexTree_insertEntTop"_Stz,     (const void *)IndexTree::aInsertEntTop    },
  { "aIndexTree_positionGE"_Stz,       (const void *)IndexTree::aPositionGE      },
  { "aIndexTree_positionGT"_Stz,       (const void *)IndexTree::aPositionGT      },
  { "aIndexTree_positionLE"_Stz,       (const void *)IndexTree::aPositionLE      },
  { "aIndexTree_positionLT"_Stz,       (const void *)IndexTree::aPositionLT      },

  { "aRowBuffer_GetCellArray"_Stz,     (const void *)RowBuffer::aGetCellArray    },
  { "vRowSingle_Print"_Stz,            (const void *)RowSingle::vPrint           },

  { "aPot_AllotRow"_Stz,               (const void *)Pot::aAllotRow              },
  { "vPot_RowStrSize"_Stz,             (const void *)Pot::vRowStrSize            },

  { "aTub_AllotRow"_Stz,               (const void *)Tub::aAllotRow              },
  { "vTub_RowStrSize"_Stz,             (const void *)Tub::vRowStrSize            },

  { "StrLib_iLike2"_Stz,                (const void *)StrLib::iLike2             },
  { "StrLib_iLike3"_Stz,                (const void *)StrLib::iLike3             },
  { "StrLib_iWordFlipBack"_Stz,         (const void *)StrLib::iWordFlipBack      },

  { "vNeverReach"_Stz,                  (const void *)vNeverReach                },

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
