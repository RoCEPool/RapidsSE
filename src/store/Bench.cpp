/*============================================================================*/
/* Bench.cpp -- Bench                                                         */
/*============================================================================*/

#include "Bench.hpp"
#include "Board.hpp"
#include "Optimiser.hpp"
#include "CardIter.hpp"

#include "XC.hpp"
#include "eOH.h"

/*----------------------------------------------------------------------------*/
/*-- LifeCycle ---------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

__pCastFunction(STORE,Bench,CN_BENCH)
__aNewFunction(Bench,CN_BENCH)
__vDeleteFunction(Bench,CN_BENCH)
__aFooterFunction(Bench)

/*----------------------------------------------------------------------------*/
/* constructor                                                                */
/*----------------------------------------------------------------------------*/
Bench::Bench(Optimiser* pOptimiser)
: Object(CN_BENCH)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_BENCH,_FOOTER_);
    
  m_pOptimiser = pOptimiser;

  m_iBenchNo = AM::OPTIMISER::iNextBenchNo();

  m_iXform        = 0;
  m_eXform        = XF_NONE;
  m_iXaction      = 0;
  m_nPrintBoard   = 0;
  m_nTransformPrint = 0;

  for (i8 i_Seq = 0; i_Seq < CK_ENUMS; i_Seq++) {
    m_iaSequence[i_Seq] = 0;
  }

  _checkInitialised_(CN_BENCH);
  
  _postcondition_(_ADDRESS_,_MAGIC_,CN_BENCH,_FOOTER_);
}

/*----------------------------------------------------------------------------*/
/* destructor                                                                 */
/*----------------------------------------------------------------------------*/
Bench::~Bench(void) noexcept(false)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_BENCH,_FOOTER_);
   vDeleteAll();

}

/*----------------------------------------------------------------------------*/
/*-- Static ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/*-- Public ------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* add a new board                                                            */
/*----------------------------------------------------------------------------*/
void Bench::vAddBoard(Board* pBoard)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_BENCH, _FOOTER_);

  //RASSERT(! pFindBoard_By_sqyNm(pBoard->eBoard()));

  vPush(pBoard);
}

/*----------------------------------------------------------------------------*/
/* get number of board                                                        */
/*----------------------------------------------------------------------------*/
i8 Bench::nBoards(void)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_BENCH, _FOOTER_);

  i8 nBoards = nItems();

  return nBoards;
}

/*----------------------------------------------------------------------------*/
/* dump this bench                                                            */
/*----------------------------------------------------------------------------*/
void Bench::vDump(const char* azFunc)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_BENCH, _FOOTER_);

  vTrace("Bench Dump [%s]: %p %lld PipelineNm %s\n"_Stz,
         azFunc,
         this,
         m_iBenchNo,
         m_sPipelineNm.szStz()
  );

  i8 nBoards = this->nBoards();
  for (i8 i_Board = 0; i_Board < nBoards; i_Board++) {
    Board* pBoard = this->pGet(i_Board);

    pBoard->vDump(1);
  }

  vTrace("End Bench Dump\n"_Stz);

}

/*----------------------------------------------------------------------------*/
/* end of phase                                                               */
/*----------------------------------------------------------------------------*/
void Bench::vEndPhase(CardIter* pCI)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_BENCH,_FOOTER_);

  pCI->vBeginBoard();
  while (pCI->bEachBoard())
    pCI->pBoard()->vEndPhase();
}

/*----------------------------------------------------------------------------*/
/* get board of given name                                                    */
/*----------------------------------------------------------------------------*/
Board* Bench::pFindBoard_By_sqyNm(Stz* szSqyNm)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_BENCH, _FOOTER_);

  i8 nBoards = this->nBoards();
  for (i8 i_Board = 0; i_Board < nBoards; i_Board++) {
    Board *pBoard = this->pGet(i_Board);

    if (pBoard->pSubQuery()->szSqyNm()->bSame(szSqyNm))
      return pBoard;
  }

  return nullptr;
}

/*----------------------------------------------------------------------------*/
/* get board of given name                                                    */
/*----------------------------------------------------------------------------*/
Board* Bench::pFindBoard_By_sqyNo(Stz* szSqyNo)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_BENCH, _FOOTER_);

  i8 nBoards = this->nBoards();
  for (i8 i_Board = 0; i_Board < nBoards; i_Board++) {
    Board *pBoard = this->pGet(i_Board);

    if (pBoard->pSubQuery()->szSqyNo()->bSame(szSqyNo))
      return pBoard;
  }

  return nullptr;
}

/*----------------------------------------------------------------------------*/
/* find board matching given symbol                                           */
/*----------------------------------------------------------------------------*/
Board* Bench::pFindBoard(Symbol* pSymbol_arg)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_BENCH, _FOOTER_);

  i8 nBoards = this->nBoards();
  for (i8 i_Board = 0; i_Board < nBoards; i_Board++) {
    Board *pBoard = this->pGet(i_Board);

    if (pBoard->pSubQuery()->pSymbol()->bMatches(pSymbol_arg))
      return pBoard;
  }

  return nullptr;
}

/*----------------------------------------------------------------------------*/
/* find subquery given symbol                                                 */
/*----------------------------------------------------------------------------*/
SubQuery* Bench::pFindSubQuery(Symbol* pSymbol_arg)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_BENCH, _FOOTER_);

  if (Board* pBoard = pFindBoard(pSymbol_arg)) {
    SubQuery *pSubQuery = pBoard->pSubQuery();
    return pSubQuery;
  }

  return nullptr;
}

/*----------------------------------------------------------------------------*/
/* get board                                                                  */
/*----------------------------------------------------------------------------*/
Board* Bench::pGetBoard(i8 i_Board)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_BENCH, _FOOTER_);

  if (i_Board >= nBoards())
    return nullptr;

  Board *pBoard = this->pGet(i_Board);
  return pBoard;
}

/*----------------------------------------------------------------------------*/
/* is given name a known subquery?                                            */
/*----------------------------------------------------------------------------*/
bool Bench::bKnown_sqy(Symbol* pSymbol_arg)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_BENCH, _FOOTER_);

  if (Board* pBoard = pFindBoard(pSymbol_arg)) {
    UNUSED(pBoard);
    return true;
  }

  return false;
}

/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
Optimiser* Bench::pOptimiser(void)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_BENCH, _FOOTER_);

  return m_pOptimiser;
}

/*----------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* past given phase?                                                          */
/*----------------------------------------------------------------------------*/
bool Bench::bPastPhase(CardIter* pCI, eOH ePhase)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_BENCH,_FOOTER_);

  bool bPast = true;
  pCI->vPushEach();

  pCI->vBeginBoard();
  while (pCI->bEachBoard())
    if (! pCI->pBoard()->bPastPhase(ePhase))
      bPast = false;

  pCI->vPopEach();

  return bPast;
}

/*----------------------------------------------------------------------------*/
/* begin phase                                                                */
/*----------------------------------------------------------------------------*/
void Bench::vPhase(CardIter* pCI, eOH ePhase)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_BENCH,_FOOTER_);

  pCI->vPushEach();

  pCI->vBeginBoard();
  while (pCI->bEachBoard())
    pCI->pBoard()->vPhase(ePhase);

  pCI->vPopEach();
}

/*----------------------------------------------------------------------------*/
/* get pipeline name                                                          */
/*----------------------------------------------------------------------------*/
Stz* Bench::szPipelineNm(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_BENCH,_FOOTER_);

  return m_sPipelineNm;
}

/*----------------------------------------------------------------------------*/
/* set pipeline name                                                          */
/*----------------------------------------------------------------------------*/
void Bench::vPipelineNm(Stz* szPipelineNm)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_BENCH,_FOOTER_);

  m_sPipelineNm = szPipelineNm;
}

/*----------------------------------------------------------------------------*/
/* get next sequence for card kind                                            */
/*----------------------------------------------------------------------------*/
i8 Bench::iSequence_next(eCK eCard)
{
  _precondition_(_ADDRESS_, _MAGIC_, CN_BENCH, _FOOTER_);

  return m_iaSequence[eCard]++;
}

/*----------------------------------------------------------------------------*/
/* begin and record transformation information                                */
/*----------------------------------------------------------------------------*/
void Bench::vTransform(const char* azXfile, const char* azXfunc, eXF eXform)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_BENCH,_FOOTER_);

  vXform_Inc();

  vXfile((Stz *) azXfile);
  vXfunc((Stz *) azXfunc);
  vXform(eXform);

  m_nTransformPrint = 0;
}

/*----------------------------------------------------------------------------*/
/* inc action number                                                         */
/*----------------------------------------------------------------------------*/
i8 Bench::iXaction_Inc(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_BENCH,_FOOTER_);

  return m_iXaction++;
}

/*----------------------------------------------------------------------------*/
/* get transformation function                                                */
/*----------------------------------------------------------------------------*/
Stz* Bench::szXfile(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_BENCH,_FOOTER_);

  return m_sXfile;
}

/*----------------------------------------------------------------------------*/
/* set transformation function                                                */
/*----------------------------------------------------------------------------*/
void Bench::vXfile(Stz* szXfile)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_BENCH,_FOOTER_);

  m_sXfile = szXfile;
}

/*----------------------------------------------------------------------------*/
/* get transformation                                                         */
/*----------------------------------------------------------------------------*/
eXF Bench::eXform(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_BENCH,_FOOTER_);

  return m_eXform;
}

/*----------------------------------------------------------------------------*/
/* inc transform number                                                       */
/*----------------------------------------------------------------------------*/
void Bench::vXform(eXF eXform)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_BENCH,_FOOTER_);

  m_eXform = eXform;
}

/*----------------------------------------------------------------------------*/
/* get transformation number                                                  */
/*----------------------------------------------------------------------------*/
i8 Bench::iXform(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_BENCH,_FOOTER_);

  return m_iXform;
}

/*----------------------------------------------------------------------------*/
/* inc transform number                                                       */
/*----------------------------------------------------------------------------*/
void Bench::vXform_Inc(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_BENCH,_FOOTER_);

  m_iXform++;
}

/*----------------------------------------------------------------------------*/
/* get transformation function                                                */
/*----------------------------------------------------------------------------*/
Stz* Bench::szXfunc(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_BENCH,_FOOTER_);

  return m_sXfunc;
}

/*----------------------------------------------------------------------------*/
/* set transformation function                                                */
/*----------------------------------------------------------------------------*/
void Bench::vXfunc(Stz* szXfunc)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_BENCH,_FOOTER_);

  m_sXfunc = szXfunc;
}

/*----------------------------------------------------------------------------*/
/* get print number                                                           */
/*----------------------------------------------------------------------------*/
i8 Bench::nPrintBoard(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_BENCH,_FOOTER_);

  return m_nPrintBoard;
}

/*----------------------------------------------------------------------------*/
/* increment and get print number                                             */
/*----------------------------------------------------------------------------*/
i8 Bench::nPrintBoard_inc(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_BENCH,_FOOTER_);

  return m_nPrintBoard++;
}

/*----------------------------------------------------------------------------*/
/* get transformation number                                                  */
/*----------------------------------------------------------------------------*/
i8 Bench::iBenchNo(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_BENCH,_FOOTER_);

  return m_iBenchNo;
}

/*----------------------------------------------------------------------------*/
/* increment and get transform print number                                   */
/*----------------------------------------------------------------------------*/
i8 Bench::nTransformPrint_inc(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_BENCH,_FOOTER_);

  return m_nTransformPrint++;
}


/*----------------------------------------------------------------------------*/
/*                                                                            */
/*----------------------------------------------------------------------------*/
void Bench::vValidate(CardIter* pCI)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_BENCH,_FOOTER_);

//  if (! AM::JOB::bHasTag(C_OPTIMISER_TRACE))
//    return;

  pCI->vPushEach();

  i8 nBoards = this->nBoards();
  for (i8 i_Board = 0; i_Board < nBoards; i_Board++) {
    Board *pBoard = this->pGet(i_Board);
    pCI->vUseBoard(pBoard);
    RASSERT(pCI->E.pBoard);

    //pBoard->pState()->vDumpSet();

    RASSERT(pBoard->i_Board() == i_Board);

    //vTrace("Validate board [%lld] : %p\n"_Stz,i_Board,pBoard);

    if (! AM::JOB::bHasTag(C_OPTIMISER_TRACE))
      pBoard->vClearCardMarks();

    pBoard->vValidate(pCI);

    if (pBoard->pState()->bHasName(aOH[OH_FLATTEN]))
      break;
  }

  pCI->vPopEach();


  XC::vXC(pCI);
}


/*----------------------------------------------------------------------------*/
/*-- Private -----------------------------------------------------------------*/
/*----------------------------------------------------------------------------*/


/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
