/*============================================================================*/
/* Steps_Pds.cpp -- Pds                                                       */
/*============================================================================*/

#include "BASE.h"
#include "Step.hpp"
#include "EnterExit.h"
#include "StepAccum.hpp"
#include "Steps.hpp"
#include "LogicUnit.hpp"
#include "Table.hpp"

/*----------------------------------------------------------------------------*/
/* add primitives                                                             */
/*----------------------------------------------------------------------------*/
void Step::vAddSteps_Pds(void)
{
  vAddStep("Pds[]"_Stz,                     &Step::vPdsGet);

  vAddStep("Pds[].addProgressUnits"_Stz, &Step::vPdsAddProgressUnits);
  vAddStep("Pds[].addRowVargs"_Stz,      &Step::vPdsAddRowVargs);
  vAddStep("Pds[].bufferId"_Stz  ,          &Step::vPdsGetBufferIdAsAddr);
  vAddStep("Pds[].bufferType@"_Stz,         &Step::vPdsBufferTypeFetch);
  vAddStep("Pds[].bufferType!"_Stz,         &Step::vPdsBufferTypeStore);
  vAddStep("Pds[].delete"_Stz  ,            &Step::vPdsDelete);
  vAddStep("Pds[].isNull"_Stz  ,            &Step::vPdsIsNull);
  vAddStep("Pds[].new"_Stz,                 &Step::vPdsNew);
  vAddStep("Pds[].set"_Stz  ,               &Step::vPdsSet);
}

/*----------------------------------------------------------------------------*/
/* Pds[].addProgressUnits -- add row to Pds with the progressUnits            */
/*----------------------------------------------------------------------------*/
void Step::vPdsAddProgressUnits(void)
{
  ENTRY();

  // get no of progress units
  i8 iProgressUnits = m_pDataStack->iPopInt(__func__);

  // get the Pds
  Pds* pPdsRowBuffer = _pPds(*m_pLogicUnit->m_xpPds);

  Row* pRow = pPdsRowBuffer->pAllotRow(C_VARGS_BEGIN,
                                    iProgressUnits,
                                    C_VARGS_END);
  //ASSERT(pRow);
  //pRow->vDump(pStreamDef);

  m_pDataStack->vPushInt(pRow ? true : false);

  EXIT();
}

/*----------------------------------------------------------------------------*/
/* Pds[].addRowVargs -- add a row by variable arguments                       */
/*----------------------------------------------------------------------------*/
void Step::vPdsAddRowVargs(void)
{
  ENTRY();

  i8 nArgs = m_pDataStack->iPopInt(__func__);

  Pds* pPdsRowBuffer = _pPds(*m_pLogicUnit->m_xpPds);
  RowPix* pRowPix = pPdsRowBuffer->pRowPix();
  i8 nCols = pRowPix->nColumns();

  Row* pRow = pPdsRowBuffer->pAllotRow();

  if( ! pRow) {
    m_pDataStack->vPushInt(false);
    return;
  }

  i8   nSize;
  i8 iInt;
  s256 sStr;

  for (i8 i_Arg = 0; i_Arg < nArgs && i_Arg < nCols; i_Arg++) {

    eDM eDatum = (eDM) pRowPix->m_xaColPix[i_Arg].m_uDatum;

    switch (eDatum) {

      case DM_INT:
        iInt = m_pDataStack->iPopInt(__func__);
        pPdsRowBuffer->vDepositColVal(iInt);
        break;

      case DM_VCH:

        // STRING_DEPOSIT
        m_pDataStack->vPopStr(sStr,__func__);
        nSize = sStr.nSize();
        pPdsRowBuffer->vDepositColVal(sStr, nSize);
        // END STRING_DEPOSIT

        break;

      default:
        RASSERT(0);
    }

  }

  m_pDataStack->vPushInt(true);

  EXIT();
}

/*----------------------------------------------------------------------------*/
/* Pds[] -- get pds                                                           */
/*----------------------------------------------------------------------------*/
void Step::vPdsGet(void)
{
  ENTRY();

  m_pDataStack->vPushPtr(*m_pLogicUnit->m_xpPds);

  EXIT();
}

/*----------------------------------------------------------------------------*/
/* Pds[].bufferId -- fetch buffer id as address                               */
/*----------------------------------------------------------------------------*/
void Step::vPdsGetBufferIdAsAddr(void)
{
  ENTRY();

  Pds* pPds = *m_pLogicUnit->m_xpPds;

  m_pDataStack->vPushPtr((Object*)pPds->oGetBufferId().m_pvAsAddress);

  EXIT();
}

/*----------------------------------------------------------------------------*/
/* Pds[].bufferType@ -- fetch the bufferType                                  */
/*----------------------------------------------------------------------------*/
void Step::vPdsBufferTypeFetch(void)
{
  ENTRY();

  m_pDataStack->vPushInt(m_pLogicUnit->m_xpPds->eBufferType());

  EXIT();
}

/*----------------------------------------------------------------------------*/
/* Pds[].bufferType! -- store buffer type                                     */
/*----------------------------------------------------------------------------*/
void Step::vPdsBufferTypeStore(void)
{
  ENTRY();

  eBT eBufferType = (eBT)m_pDataStack->iPopInt(__func__);

  m_pLogicUnit->m_xpPds->vBufferType(eBufferType);

  EXIT();
}

/*----------------------------------------------------------------------------*/
/* Pds[].delete -- operand delete                                             */
/*----------------------------------------------------------------------------*/
void Step::vPdsDelete(void)
{
  ENTRY();

  AM::vDeleteOperand(*m_pLogicUnit->m_xpPds,nullptr);

  m_pLogicUnit->m_xpPds = nullptr;

  EXIT();
}

/*----------------------------------------------------------------------------*/
/* Pds[].isNull --                                                            */
/*----------------------------------------------------------------------------*/
void Step::vPdsIsNull(void)
{
  ENTRY();

  m_pDataStack->vPushInt(m_pLogicUnit->m_xpPds.bIsNull() ? 1 : 0);

  EXIT();
}

/*----------------------------------------------------------------------------*/
/* Pds[].new -- new an Pds operand                                            */
/*----------------------------------------------------------------------------*/
void Step::vPdsNew(void)
{
  ENTRY();

  ASSERT(m_pLogicUnit->m_xpPds.bIsNull());

  Table* pTable = _pTable(m_pDataStack->pPopPtr(__func__));

  eBT eBufferType = (eBT)m_pDataStack->iPopInt(__func__);

  StreamDef* pStreamDef = pTable->pStreamDef();

  m_pLogicUnit->m_xpPds = new Pds(pStreamDef, false, eBufferType);

  EXIT();
}

/*----------------------------------------------------------------------------*/
/* Pds[].set -- set the pds                                                   */
/*----------------------------------------------------------------------------*/
void Step::vPdsSet(void)
{
  ENTRY();

  m_pLogicUnit->m_xpPds = _pPds(m_pDataStack->pPopPtr(__func__));

  EXIT();
}
