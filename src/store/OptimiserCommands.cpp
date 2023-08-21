/*============================================================================*/
/* OptimiserCommands.cpp -- the optimiser commands                            */
/*============================================================================*/

#include "OptimiserCommands.hpp"

#include "OptimiserCatalog.hpp"
#include "Dynamo.hpp"
#include "PipelineNotation.hpp"
#include "CardIter.hpp"

extern OptimiserCatalog gxOptimiserCatalog;

/*----------------------------------------------------------------------------*/
/* default                                                                    */
/*----------------------------------------------------------------------------*/
bool STORE::bOpt_default(Optimiser* pOptimiser, TokenVec* pTknVec, eOA& reOptAct)
{
  UNUSED(pTknVec);

  reOptAct = OA_REPRINT_BOARD;

  pOptimiser->pCardCtl()->vUseDefault();

  return true;
}

OptimiserFun __Opt_d
    (
        "default"_Stz,      // name of the opt function
        "d"_Stz,            // command first word
        nullptr,            // command second word
        bOpt_default,       // function to call
        0,0,                // spare
        "set default printing"_Stz,
        "usage???"_Stz
    );

/*----------------------------------------------------------------------------*/
/* expanded                                                                   */
/*----------------------------------------------------------------------------*/
bool STORE::bOpt_expanded(Optimiser* pOptimiser, TokenVec* pTknVec, eOA& reOptAct)
{
  UNUSED(pTknVec);

  reOptAct = OA_REPRINT_BOARD;

  //pOptimiser->pCardCtl()->vUseExpanded();
  pOptimiser->pCardCtl()->vSetSubsPrintEverything();

  return true;
}

OptimiserFun __Opt_e
    (
        "expanded"_Stz,     // name of the opt function
        "e"_Stz,            // command first word
        nullptr,            // command second word
        bOpt_expanded,      // function to call
        0,0,                // spare
        "set expanded printing"_Stz,
        "usage???"_Stz
    );

/*----------------------------------------------------------------------------*/
/* help                                                                       */
/*----------------------------------------------------------------------------*/
bool STORE::bOpt_help(Optimiser* pOptimiser, TokenVec* pTknVec, eOA& reOptAct)
{
  UNUSED(pOptimiser);
  UNUSED(pTknVec);

  gxOptimiserCatalog.vHelp();
  
  reOptAct = OA_INPUT_AGAIN;
  
  return true;
}

OptimiserFun __Opt_h_0
(
  "help"_Stz,         // name of the opt function
  "?"_Stz,            // command first word
  nullptr,            // command second word
  bOpt_help,          // function to call
  0,0,                // spare
  "help"_Stz,
 "usage???"_Stz
);

OptimiserFun __Opt_h_1
(
  "help"_Stz,         // name of the opt function
  "h"_Stz,            // command first word
  nullptr,            // command second word
  bOpt_help,          // function to call
  0,0,                // spare
  "help"_Stz,
  "usage???"_Stz
);

/*----------------------------------------------------------------------------*/
/* longer                                                                     */
/*----------------------------------------------------------------------------*/
bool STORE::bOpt_longer(Optimiser* pOptimiser, TokenVec* pTknVec, eOA& reOptAct)
{
  UNUSED(pTknVec);

  reOptAct = OA_REPRINT_BOARD;

  pOptimiser->pCardCtl()->vUseLonger();

  return true;
}

OptimiserFun __Opt_l
    (
        "longer"_Stz,       // name of the opt function
        "l"_Stz,            // command first word
        nullptr,            // command second word
        bOpt_longer,        // function to call
        0,0,                // spare
        "set longer printing"_Stz,
        "usage???"_Stz
    );

/*----------------------------------------------------------------------------*/
/* notation                                                                   */
/*----------------------------------------------------------------------------*/
bool STORE::bOpt_notation(Optimiser* pOptimiser, TokenVec* pTknVec, eOA& reOptAct)
{
  UNUSED(pTknVec);
  UNUSED(reOptAct);

  Bench* pBench = pOptimiser->pBench();
  Card_Deck* pDeck = pBench->pGetBoard((i8)0)->pDeck();

  Kid xCI(pDeck);

  while (xCI.bEach()) {
    s256 sShort = ""_Stz;
    StrLib::vShortHand(sShort.azChar(), xCI.pChild()->szCardNm()->azChar());
    vTrace("%s "_Stz, sShort.azChar());

    Card_TableNm* pCard_TableNm = xCI.pChild()->pSub_tableNm();
    if(pCard_TableNm != nullptr)
    {
      Card* pTableCard = _pCard(pCard_TableNm);
      vTrace("%s "_Stz, pTableCard->szSymbol(SF_NONE)->azChar());
    }
  }

  vTrace("\n"_Stz);

  return true;
}

OptimiserFun __Opt_n
    (
        "notation"_Stz,       // name of the opt function
        "n"_Stz,            // command first word
        nullptr,            // command second word
        bOpt_notation,        // function to call
        0,0,                // spare
        "short hand notation of operators"_Stz,
        "usage???"_Stz
    );

/*----------------------------------------------------------------------------*/
/* read pipeline from file                                                    */
/*----------------------------------------------------------------------------*/
bool STORE::bOpt_readPipeline(Optimiser* pOptimiser, TokenVec* pTknVec, eOA& reOptAct)
{
  UNUSED(reOptAct);

  s256 sFile;
  s16KB szPipeLineStatement = ""_Stz;

  sFile = pTknVec->pGet(1)->szChars();

  PipelineNotation::vPipelineFromFile(sFile, szPipeLineStatement);

//  vTrace("%s\n"_Stz, sPipeLineStatement.szStz());

  VertexSpec* pVertexSpec = Dynamo::pToVertexSpec(szPipeLineStatement, pOptimiser->pJobTags());

  pOptimiser->vToPipeSpec(pVertexSpec);

  return true;
}

OptimiserFun __Opt_r
    (
        "read"_Stz,       // name of the opt function
        "r"_Stz,            // command first word
        nullptr,            // command second word
        bOpt_readPipeline,        // function to call
        0,0,                // spare
        "read pipeline from file"_Stz,
        "usage???"_Stz
    );

/*----------------------------------------------------------------------------*/
/* shorter                                                                    */
/*----------------------------------------------------------------------------*/
bool STORE::bOpt_shorter(Optimiser* pOptimiser, TokenVec* pTknVec, eOA& reOptAct)
{
  UNUSED(pTknVec);

  reOptAct = OA_REPRINT_BOARD;

  pOptimiser->pCardCtl()->vUseShorter();

  return true;
}

OptimiserFun __Opt_s
    (
        "shorter"_Stz,      // name of the opt function
        "s"_Stz,            // command first word
        nullptr,            // command second word
        bOpt_shorter,       // function to call
        0,0,                // spare
        "set shorter printing"_Stz,
        "usage???"_Stz
    );

/*----------------------------------------------------------------------------*/
/* weave                                                                      */
/*----------------------------------------------------------------------------*/
bool STORE::bOpt_weave(Optimiser* pOptimiser, TokenVec* pTknVec, eOA& reOptAct)
{
  vTrace("bOpt_weave\n"_Stz);

  UNUSED(pTknVec);
  UNUSED(reOptAct);

  vTrace("%s\n"_Stz,pOptimiser->szWeave());

  //reOptAct = OA_INPUT_AGAIN;

  return true;
}

OptimiserFun __Opt_w
(
  "weave"_Stz,        // name of the opt function
  "w"_Stz,            // command first word
  nullptr,            // command second word
  bOpt_weave,         // function to call
  0,0,                // spare
  "weave pipeline statement"_Stz,
  "usage???"_Stz
);

/*----------------------------------------------------------------------------*/
/* write pipeline to file                                                     */
/*----------------------------------------------------------------------------*/
bool STORE::bOpt_writePipeline(Optimiser* pOptimiser, TokenVec* pTknVec, eOA& reOptAct)
{
  UNUSED(pOptimiser);
  UNUSED(reOptAct);

//  reOptAct = OA_REPRINT_BOARD;
//
//  pOptimiser->pCardCtl()->vUseLonger();

  s256 sFile;
  s16KB sPipeLineStatement = ""_Stz;

  sFile = pTknVec->pGet(1)->szChars();

  PipelineNotation::vPipelineToFile(sFile, sPipeLineStatement);

//  vTrace("%s\n"_Stz, sPipeLineStatement.szStz());
  return true;
}

OptimiserFun __Opt_wr
    (
        "write"_Stz,       // name of the opt function
        "wr"_Stz,            // command first word
        nullptr,            // command second word
        bOpt_writePipeline,        // function to call
        0,0,                // spare
        "write pipeline from file"_Stz,
        "usage???"_Stz
    );

/*----------------------------------------------------------------------------*/
/* disable                                                                    */
/*----------------------------------------------------------------------------*/
bool STORE::bOpt_toEnd(Optimiser* pOptimiser, TokenVec* pTknVec, eOA& reOptAct)
{
  UNUSED(pOptimiser);

  vTrace("bOpt_weave\n"_Stz);

  UNUSED(pTknVec);

  reOptAct = OA_DISABLE_CUEING;

  return true;
}

OptimiserFun __Opt_z
    (
        "toEnd"_Stz,        // name of the opt function
        "z"_Stz,            // command first word
        nullptr,            // command second word
        bOpt_toEnd,         // function to call
        0,0,                // spare
        "run to end"_Stz,
        "usage???"_Stz
    );


/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
