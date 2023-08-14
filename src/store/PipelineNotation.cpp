/*============================================================================*/
/* PipelineNotation.cpp -- Pipeline Notation Helper                           */
/*============================================================================*/

#include "PipelineNotation.hpp"

struct NotationMapping
{
  Stz* sFullForm;
  Stz* sShortForm;
};

struct NotationMapping aNM[]
{
    {"scan"_Stz,                   "S"_Stz},
    {"filter"_Stz,                 "F"_Stz},
    {"aggreg"_Stz,                 "Agg"_Stz},
    {"groupBy"_Stz,                "GBy"_Stz},
    {"project"_Stz,                "Pj"_Stz},
    {"type"_Stz,                   "Ty"_Stz},
    {"as"_Stz,                     "as"_Stz},
    {"combined"_Stz,               "C"_Stz},
    {"indexJoin"_Stz,              "iJ"_Stz},
    {"hash"_Stz,                   "H"_Stz},
    {"VARCHAR"_Stz,               "V"_Stz},
    {"DECIMAL"_Stz,               "D"_Stz},
    {"INTEGER"_Stz,               "I"_Stz},
    {"BOOLEAN"_Stz,               "B"_Stz},


   {"END"_Stz,                   ""_Stz},
};

/*----------------------------------------------------------------------------*/
/*  Take a pipeline statement and convert it to a short form                  */
/*----------------------------------------------------------------------------*/
void PipelineNotation::vPipelineNotation(Stz* szPipelineStatement)
{
  _static_

  TokenVec* pTokenVec = AM::TOKENISE::pTokeniseSen(szPipelineStatement);

  // Collect "stage xx on all yy :" as a single token each
  vStages(pTokenVec);

  // Replace operators with short hand notation
  vShorten(pTokenVec);

  vPrintPipeline(pTokenVec);

  pTokenVec->vDumpVec();
}

/*----------------------------------------------------------------------------*/
/* Extract the pipeline from a file                                           */
/*----------------------------------------------------------------------------*/
void PipelineNotation::vPipelineFromFile(Stz* szFilename, String* psDstStr)
{
  _static_

  File* pFile = new(C_VIA_MALLOC) File(nullptr);

  if (! pFile->bOpenRead(szFilename)) {
    vTrace("No such file\n"_Stz);
    return;
  }

  s256 sLine;
  while (pFile->bReadLine(sLine)) {
    if (StrLib::bEqualTo(sLine.azChar(), "// pipeline begin"))
    {
      while (pFile->bReadLine(sLine)) {
        if (StrLib::bEqualTo(sLine.azChar(), "// pipeline end"))
        {
          break;
        }
        sLine.vAppend('\n');
        psDstStr->vConcat(sLine);
      }
      break;
    }
  }

  pFile->vClose();
  delete pFile;
}

/*----------------------------------------------------------------------------*/
/* Write the pipeline to a file                                               */
/*----------------------------------------------------------------------------*/
void PipelineNotation::vPipelineToFile(Stz* pFilename, Stz* sSrcStr)
{
  _static_

  File* pFile = new(C_VIA_MALLOC) File(nullptr);

  if (! pFile->bOpenAppend(pFilename)) {
    vTrace("Failed to open file for append\n"_Stz);
    return;
  }

  s256 sBegin = "\n// pipeline begin\n"_Stz;
  s256 sEnd = "\n// pipeline end\n"_Stz;

  pFile->bWriteBytes(sBegin.szStz(), sBegin.nLength());
  pFile->bWriteBytes(sSrcStr, sSrcStr->nLength());
  pFile->bWriteBytes(sEnd.szStz(), sEnd.nLength());

  pFile->vClose();
  delete pFile;
}

/*----------------------------------------------------------------------------*/
/* Print the pipeline                                                         */
/*----------------------------------------------------------------------------*/
void PipelineNotation::vPrintPipeline(TokenVec *pTknVec)
{
  for (i8 i_Token = 0; i_Token < pTknVec->nTokens(); i_Token++) {
    Token *pToken = pTknVec->pGet(i_Token);
    if(pToken->bSame("|"_Stz)) {
      vTrace("\n\t%s "_Stz, pToken->szChars());
    } else if (pToken->szChars()->bContains("stage"_Stz)) {
      vTrace("\n%s\n\t"_Stz, pToken->szChars());
    } else {
        vTrace("%s "_Stz, pToken->szChars());
    }
  }

  vTrace("\n\n"_Stz);
}

/*----------------------------------------------------------------------------*/
/* Replace full form with short form                                          */
/*----------------------------------------------------------------------------*/
void PipelineNotation::vShorten(TokenVec* pTknVec)
{
  for (i8 i_Token = 0; i_Token < pTknVec->nTokens(); i_Token++) {
    Token *pToken = pTknVec->pGet(i_Token);

    for(i8 i = 0; ! aNM[i].sFullForm->bSame("END"_Stz); i++)
    {
      if(pToken->bSame(aNM[i].sFullForm))
      {
        pToken->vUpdateChars(aNM[i].sShortForm,pTknVec);
      }
    }
  }
}
/*----------------------------------------------------------------------------*/
/* Collect stage 'xx' on all 'yy' tokens as single tokens                     */
/*----------------------------------------------------------------------------*/
void PipelineNotation::vStages(TokenVec* pTknVec)
{
  _static_

  for (i8 i_Token = 0; i_Token < pTknVec->nTokens(); i_Token++) {
    Token* pToken = pTknVec->pGet(i_Token);
    if(pToken->bSame("stage"_Stz))
    {
      s4K sStage = ""_Stz;
      sStage.vConcat("stage "_Stz);
      pTknVec->vRemoveToken(i_Token);

      Token* pStageTkn = pTknVec->pGet(i_Token);
      while( ! pStageTkn->bSame(":"_Stz))
      {
        sStage.vConcat(pStageTkn->szChars());
        sStage.vAppend(' ');
        pTknVec->vRemoveToken(i_Token);
        pStageTkn = pTknVec->pGet(i_Token);
      }
      sStage.vConcat(pStageTkn->szChars());
      pTknVec->vRemoveToken(i_Token);
      pTknVec->vInsertToken(i_Token, sStage, TN_NAME);
      sStage="";
    }
  }

}