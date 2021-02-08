/*============================================================================*/
/* LogicUnit_GenCode.hpp -- Logic Unit (generate code functions)              */
/*============================================================================*/

#include "BASE.h"

#include "RowOnce.hpp"
#include "IndexStream.hpp"
#include "Node_I.hpp"
#include "Response.hpp"
#include "Result.hpp"
#include "Job.hpp"
#include "LogicUnit.hpp"

#include "TagConstants.h"
#include "Grid.hpp"
#include "Node.hpp"

extern Atomic gixQueryNo;

/*----------------------------------------------------------------------------*/
/* accept token and advance to next token                                     */
/*----------------------------------------------------------------------------*/
void LogicUnit::vAcceptToken(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_LOGICUNIT,_FOOTER_);
  
  ++ m_i_Tkn;
  
  m_pTkn = m_pTknVec->pGet(m_i_Tkn);
  
#ifdef WITH_TRACE_LOGICUNIT
  //vTrace("Next %03lld: %s\n"_Stz,m_i_Tkn,m_pTkn->szChars());
#endif
}

/*----------------------------------------------------------------------------*/
/* set token on which parsing begins                                          */
/*----------------------------------------------------------------------------*/
void LogicUnit::vBeginToken(i8 i_Tkn)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_LOGICUNIT,_FOOTER_);
  
  m_i_Tkn = i_Tkn;
  m_pTkn  = m_pTknVec->pGet(m_i_Tkn);
  
#ifdef WITH_TRACE_LOGICUNIT
  //vTrace("Begin %03lld: %s\n"_Stz,i_Tkn,m_pTkn->szChars());
#endif
}

/*----------------------------------------------------------------------------*/
/* set token on which parsing begins                                          */
/*----------------------------------------------------------------------------*/
void LogicUnit::vCollectOpcode(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_LOGICUNIT,_FOOTER_);
  
  //vTrace("Tkn: %s\n"_Stz, m_pTkn->azChars());
  
  if (m_pTkn->bSame("Label"_Stz)) {
    vAcceptToken();
    vDefineLabel(m_pTkn->szChars());
    return;
  }
  
  TagSet* pJobTags = AM::pPipelineJob()->pJobTags();
  
  for (i8 i_OpCode = 0; i_OpCode < EO_ENUMS; i_OpCode++) {

    //vTrace("Opcode: %s; Arg: %s\n"_Stz, aEO[i_OpCode], aEA[EoArgs[i_OpCode].m_eArgument]);

    if (m_pTkn->bSame(aEO[i_OpCode])) {

      //vTrace("Tkn: %s\n"_Stz, m_pTkn->azChars());

      // debug
      char* azEname;
      
      switch ((i8)(EoArgs[i_OpCode].m_eArgument)) {
          
        case EA_DATE:
          vAcceptToken();
          azEname = m_pTkn->azChars();
          if (0 == StrLib::iCompare(m_pTkn->azChars(),"<param", 6,  C_CASE)) {
            i8 i_Ename = pJobTags->i_Find(m_pTkn->szChars());
            ASSERT(i_Ename != C_UNKNOWN);
            if (pJobTags->eToken(i_Ename) != TN_STRING) {
              pJobTags->vDumpSet(true);
              vTrace("azEname : %s \n"_Stz,azEname);
              THROW_USER("vCollectOpcode"_Stz);
            }
            Stz* szParam = pJobTags->szValue(i_Ename);
            vAddCodeArg((eEO)i_OpCode, szParam);
          }
          else
            vAddCodeArg((eEO)i_OpCode, m_pTkn->szChars());
          return;
          
        case EA_INT: {
          i8 iArg = C_UNKNOWN;
      
          vAcceptToken();
          if ( m_pTkn->bIsInteger() ) {
            iArg = m_pTkn->iToI8();
          }
          else {
            if (0 == StrLib::iCompare(m_pTkn->azChars(),"CT_", 3,  C_CASE)) {
              for(iArg = 0; iArg < CT_ENUMS; iArg++) {
                if (StrLib::bEqualTo(m_pTkn->azChars()+3,(char*)aCT[iArg]))
                  break;
              }
            }
            else if (0 == StrLib::iCompare(m_pTkn->azChars(),"ZK_", 3,  C_CASE)) {
              for(iArg = 0; iArg < ZK_ENUMS; iArg++) {
                if (StrLib::bEqualTo(m_pTkn->azChars(),(char*)aZK[iArg]))
                  break;
              }
            }
            else if (0 == StrLib::iCompare(m_pTkn->azChars(),"<param", 6,  C_CASE)) {
              i8 i_Ename = pJobTags->i_Find(m_pTkn->szChars());
              ASSERT(i_Ename != C_UNKNOWN);
            
              //pJobTags->vContentDump();
              if (pJobTags->eToken(i_Ename) == TN_INTEGER)
                iArg = pJobTags->iValue(i_Ename);
              else if (pJobTags->eToken(i_Ename) == TN_STRING) {
                Stz* szParam = pJobTags->szValue(i_Ename);
                iArg = StrLib::iToI8((char*)szParam);
              }
              else
                THROW_USER("vCollectOpcode"_Stz);

            }
            else {
              vTrace("\n*** Invalid integer argument to opCode %s ***\n"_Stz, m_pTkn->szChars());
            }
          }
          ASSERT(iArg != C_UNKNOWN);
          vAddCodeArg((eEO)i_OpCode, iArg);
          return;
        }
          
        case EA_LABEL:
          vAcceptToken();
          vAddCodeLabel((eEO)i_OpCode, m_pTkn->szChars());
          return;
          
        case EA_STRING:
          vAcceptToken();
          if (0 == StrLib::iCompare(m_pTkn->azChars(),"<param", 6,  C_CASE)) {
            i8 i_Ename = pJobTags->i_Find(m_pTkn->szChars());
            ASSERT(i_Ename != C_UNKNOWN);
            if (pJobTags->eToken(i_Ename) != TN_STRING)
              THROW_USER("vCollectOpcode"_Stz);
            Stz* szParam = pJobTags->szValue(i_Ename);
            vAddCodeString((eEO)i_OpCode, szParam);
          }
          else
            vAddCodeString((eEO)i_OpCode, m_pTkn->szChars());
          return;
          
        case EA_SUBSCR:
          vAcceptToken();
          vAddCodeArg((eEO)i_OpCode, m_pTkn->iToI8());

          return;
          
       case EO_NONE:
         vAddCode((eEO)i_OpCode);
         return;
          
        case EO_ENUMS:
          ASSERT(0);
      }
    }
    
  }
  
  vTrace("\n*** Invalid opCode %s ***\n"_Stz, m_pTkn->szChars());
  
  NEVER_REACH;
}

/*----------------------------------------------------------------------------*/
/* generate asm code for given text                                           */
/*----------------------------------------------------------------------------*/
bool LogicUnit::bGenerateAsm(Stz* aText)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_LOGICUNIT,_FOOTER_);
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // tokenize the code text
  m_pTknVec->vTokenise(aText);
  i8 nTokens = m_pTknVec->nTokens();

#ifdef WITH_TRACE_LOGICUNIT

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // dump the tokens

  for (i8 i_Tkn = 0; i_Tkn < nTokens; ) {
    Token* pTkn = m_pTknVec->pGet(i_Tkn++);

    vTrace("  [%03lld] %s\n"_Stz,i_Tkn,pTkn->szChars());
  }
  vTrace("- - - - - - - - - - - - - - - - - -\n"_Stz);
#endif

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // generate the code
  m_i_Code = 0;
  
  vBeginToken(0);
  
  ASSERT(m_pTkn->bSame("Routine"_Stz) );
  vAcceptToken();
  
  // add label
  vDefineLabel(m_pTkn->szChars());  
  
  while (m_i_Tkn <  nTokens - 1) {
    vAcceptToken();
    try {
      vCollectOpcode();
    }
    catch (Exception xException) {
      m_pTknVec->vSetAllTokens();
      return false;
    }
  }
  
  m_pTknVec->vSetAllTokens();
  //vDumpCode();
  
  return true;
}

/*----------------------------------------------------------------------------*/
/* generate asm text for aggregates                                           */
/*----------------------------------------------------------------------------*/
void LogicUnit::vGenerateAggregate(ExprEval* pExprEval, StreamDef* pStreamDefOut) {

  _precondition_(_ADDRESS_, _MAGIC_, CN_LOGICUNIT, _FOOTER_);

  TagSet oTagSet;
  File *pFile = new File(nullptr);

  s256 sFileName;
  s4K sMacro;

  ++gixQueryNo;
  sFileName.vSprint("../tmp/Query%lld_%s.txt"_Stz, *gixQueryNo,
                    AM::pGrid()->pThisNode()->szNodeNm());

  bool b = pFile->bOpenWrite(sFileName);

  ASSERT(pExprEval->bHasAggregate());

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // AM_QUERY_INIT
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  oTagSet.vAppend(C_TAG_QUERY_NO, (i8) * gixQueryNo);
  oTagSet.vAppend(C_TAG_SUBQUERY_NO, (i8) 0);
  pFile->vPrint(LogicCompiler::szGetMacro(AM_QUERY_INIT, &oTagSet, sMacro));
  oTagSet.vReset();

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // DEFAULT_HANDLERS
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  pFile->vPrint(LogicCompiler::szGetMacro(AM_DEFAULT_HANDLER_STARTMARKER, &oTagSet, sMacro));
  pFile->vPrint(LogicCompiler::szGetMacro(AM_DEFAULT_HANDLER_DATAPULSE, &oTagSet, sMacro));
  pFile->vPrint(LogicCompiler::szGetMacro(AM_DEFAULT_HANDLER_EOSMARKER, &oTagSet, sMacro));
  pFile->vPrint(LogicCompiler::szGetMacro(AM_DEFAULT_HANDLER_STREAMDEF, &oTagSet, sMacro));

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // CRWBUFFER_HANDLER
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  //   to execute PROJECT_COLUMNS for each row
  if (pExprEval->m_iStreamInColumns) {
    oTagSet.vAppend(C_TAG_CRW_PROCESS_ROUTINE, "CRW_PROJECT_COLUMNS_AGG"_Stz);
  }
  else
    oTagSet.vAppend(C_TAG_CRW_PROCESS_ROUTINE, "CRW_PROJECT_NONE"_Stz);

  pFile->vPrint(LogicCompiler::szGetMacro(AM_HANDLER_CRWBUFFER_AGG, &oTagSet, sMacro));
  oTagSet.vReset();

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // ROWBUFFER_HANDLER
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  //   to execute PROJECT_COLUMNS for each row
  if (pExprEval->m_iStreamInColumns) {
    oTagSet.vAppend(C_TAG_ROW_PROCESS_ROUTINE, "ROW_PROJECT_COLUMNS_AGG"_Stz);
  }
  else
    oTagSet.vAppend(C_TAG_ROW_PROCESS_ROUTINE, "ROW_PROJECT_NONE"_Stz);

  pFile->vPrint(LogicCompiler::szGetMacro(AM_HANDLER_ROWBUFFER_AGG, &oTagSet, sMacro));

  oTagSet.vReset();

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // Query Done
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  pFile->vPrint(LogicCompiler::szGetMacro(AM_QUERY_DONE, &oTagSet, sMacro));


  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // CRW_PROJECT_COLUMNS
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // add the columns to be projected to PROJECT_COLUMNS
  if (pExprEval->m_iStreamInColumns) {
    oTagSet.vAppend(C_TAG_PROJECT_ITEMS, pExprEval->m_iStreamInColumns);
    oTagSet.vAppend(C_TAG_PROJECT_SRC_COLUMNS, pExprEval->m_uExprEvalColumns);

    pFile->vPrint(LogicCompiler::szGetMacro(AM_CRW_PROJECT_COLUMNS_AGG, &oTagSet, sMacro));

    oTagSet.vReset();
  }
  else {
    pFile->vPrint(LogicCompiler::szGetMacro(AM_CRW_PROJECT_NONE, &oTagSet, sMacro));
  }

  pFile->vPrint("%s\n"_Stz, pExprEval->m_sAsmText.azChar());
  pFile->vPrint(LogicCompiler::szGetMacro(AM_CRW_PROJECT_END, &oTagSet, sMacro));



  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // ROW_PROJECT_COLUMNS
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // add the columns to be projected to PROJECT_COLUMNS
  if (pExprEval->m_iStreamInColumns) {
    oTagSet.vAppend(C_TAG_PROJECT_ITEMS, pExprEval->m_iStreamInColumns);
    oTagSet.vAppend(C_TAG_PROJECT_SRC_COLUMNS, pExprEval->m_uExprEvalColumns);
    pFile->vPrint(LogicCompiler::szGetMacro(AM_ROW_PROJECT_COLUMNS_AGG, &oTagSet, sMacro));
    oTagSet.vReset();
  }
  else {
    pFile->vPrint(LogicCompiler::szGetMacro(AM_ROW_PROJECT_NONE, &oTagSet, sMacro));
  }

  //for (i8 i_Col = 0; i_Col < pStreamDefOut->nTotalColumns(); i_Col++) {
  pFile->vPrint("%s\n"_Stz, pExprEval->m_sAsmText.azChar());
  //}
  pFile->vPrint(LogicCompiler::szGetMacro(AM_ROW_PROJECT_END, &oTagSet, sMacro));

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // close the file
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  pFile->vClose();

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // Generate asm
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  s16KB sText;
  b = pFile->bOpenRead(sFileName);
  RASSERT(b);
  i8 nSize = pFile->iSize();
  i8 nBytes = pFile->nReadBytes((void*)sText.azChar(), nSize);

  // null terminate the read bytes
  char* sReadStr = sText.azChar();
  sReadStr[nSize] = '\0';

  bool bResult = bGenerateAsm(sText);

  pFile->vClose();
  delete pFile;
}

/*----------------------------------------------------------------------------*/
/* generate asm text for filter                                              */
/*----------------------------------------------------------------------------*/
void LogicUnit::vGenerateFilter(ExprEval* pExprEval, StreamDef* pStreamDefOut) {

  _precondition_(_ADDRESS_, _MAGIC_, CN_LOGICUNIT, _FOOTER_);

  TagSet oTagSet;
  File *pFile = new File(nullptr);

  s256 sFileName;
  s4K  sMacro;

  ++gixQueryNo;
  sFileName.vSprint("../tmp/Query%lld_%s.txt"_Stz, *gixQueryNo,
                    AM::pGrid()->pThisNode()->szNodeNm());

  bool b = pFile->bOpenWrite(sFileName);

  ASSERT( ! pExprEval->bHasAggregate() );

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // AM_QUERY_INIT
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  oTagSet.vAppend(C_TAG_QUERY_NO, (i8) * gixQueryNo);
  oTagSet.vAppend(C_TAG_SUBQUERY_NO, (i8) 0);
  pFile->vPrint(LogicCompiler::szGetMacro(AM_QUERY_INIT, &oTagSet, sMacro));
  oTagSet.vReset();

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // DEFAULT_HANDLERS
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  pFile->vPrint(LogicCompiler::szGetMacro(AM_DEFAULT_HANDLER_STARTMARKER, &oTagSet, sMacro));
  pFile->vPrint(LogicCompiler::szGetMacro(AM_DEFAULT_HANDLER_DATAPULSE, &oTagSet, sMacro));
  pFile->vPrint(LogicCompiler::szGetMacro(AM_DEFAULT_HANDLER_EOSMARKER, &oTagSet, sMacro));
  pFile->vPrint(LogicCompiler::szGetMacro(AM_DEFAULT_HANDLER_STREAMDEF, &oTagSet, sMacro));

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // CRWBUFFER_HANDLER
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  //   to execute FILTER for each row
  oTagSet.vAppend(C_TAG_CRW_PROCESS_ROUTINE, "CRW_FILTER_ROWS"_Stz);
  pFile->vPrint(LogicCompiler::szGetMacro(AM_HANDLER_CRWBUFFER, &oTagSet, sMacro));
  oTagSet.vReset();

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // ROWBUFFER_HANDLER
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  //   to execute FILTER_ROWS for each row
  oTagSet.vAppend(C_TAG_ROW_PROCESS_ROUTINE, "ROW_FILTER_ROWS"_Stz);
  pFile->vPrint(LogicCompiler::szGetMacro(AM_HANDLER_ROWBUFFER, &oTagSet, sMacro));
  oTagSet.vReset();

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // Query Done
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  pFile->vPrint(LogicCompiler::szGetMacro(AM_QUERY_DONE, &oTagSet, sMacro));

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // CRW_PROJECT_COLUMNS
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  ASSERT(pExprEval->m_iStreamInColumns);
  oTagSet.vAppend(C_TAG_PROJECT_ITEMS, pExprEval->m_iStreamInColumns);
  oTagSet.vAppend(C_TAG_PROJECT_SRC_COLUMNS, pExprEval->m_uExprEvalColumns);
  pFile->vPrint(LogicCompiler::szGetMacro(AM_CRW_FILTER_ROWS, &oTagSet, sMacro));
  oTagSet.vReset();

  pFile->vPrint(pExprEval->m_sAsmText.szStz());

  pFile->vPrint(LogicCompiler::szGetMacro(AM_CRW_FILTER_END, &oTagSet, sMacro));

  pFile->vPrint(LogicCompiler::szGetMacro(AM_CRW_PROJECT_STAR, &oTagSet, sMacro));


  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // ROW_FILTER_ROWS
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // add the filter
  oTagSet.vAppend(C_TAG_PROJECT_ITEMS, pExprEval->m_iStreamInColumns);
  oTagSet.vAppend(C_TAG_PROJECT_SRC_COLUMNS, pExprEval->m_uExprEvalColumns);
  pFile->vPrint(LogicCompiler::szGetMacro(AM_ROW_FILTER_ROWS, &oTagSet, sMacro));
  oTagSet.vReset();

  pFile->vPrint("%s\n"_Stz, pExprEval->m_sAsmText.azChar());

  pFile->vPrint(LogicCompiler::szGetMacro(AM_ROW_FILTER_END, &oTagSet, sMacro));

  u8 uProjectColumns = 0;
  for (i8 i = 0; i < pStreamDefOut->nColumns(); i++)
    BitLib::vSetOn(uProjectColumns, i);
  oTagSet.vAppend(C_TAG_PROJECT_ITEMS, pStreamDefOut->nColumns());
  oTagSet.vAppend(C_TAG_PROJECT_SRC_COLUMNS, uProjectColumns);
  pFile->vPrint(LogicCompiler::szGetMacro(AM_ROW_PROJECT_STAR, &oTagSet, sMacro));
  oTagSet.vReset();

  ExprEval* pExprEvalStar = new ExprEval;
  eEC eError = pExprEvalStar->eExprEvalAll(pStreamDefOut);
  RASSERT(eError == EC_NONE);

  // add the columns to be projected to PROJECT_COLUMNS
  pFile->vPrint("%s\n"_Stz, pExprEvalStar->m_sAsmText.azChar());

  pFile->vPrint(LogicCompiler::szGetMacro(AM_ROW_PROJECT_END, &oTagSet, sMacro));
  delete pExprEvalStar;

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // close the file
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  pFile->vClose();

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // Generate asm
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  s16KB sText;
  b = pFile->bOpenRead(sFileName);
  RASSERT(b);
  i8 nSize = pFile->iSize();
  i8 nBytes = pFile->nReadBytes((void*)sText.azChar(), nSize);

  // null terminate the read bytes
  char* sReadStr = sText.azChar();
  sReadStr[nSize] = '\0';

  bool bResult = bGenerateAsm(sText);

  pFile->vClose();
  delete pFile;
}

/*----------------------------------------------------------------------------*/
/* generate asm text for groupby                                              */
/*----------------------------------------------------------------------------*/
void LogicUnit::vGenerateGroupBy(ExprEval* pExprEval, StreamDef* pStreamDefOut) {

  _precondition_(_ADDRESS_, _MAGIC_, CN_LOGICUNIT, _FOOTER_);

  TagSet oTagSet;
  File *pFile = new File(nullptr);

  s256 sFileName;
  s4K sMacro;

  ++gixQueryNo;
  sFileName.vSprint("../tmp/Query%lld_%s.txt"_Stz, *gixQueryNo,
                    AM::pGrid()->pThisNode()->szNodeNm());

  bool b = pFile->bOpenWrite(sFileName);

  ASSERT(pExprEval->bHasGroupBy());

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // AM_QUERY_INIT
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  oTagSet.vAppend(C_TAG_QUERY_NO, (i8) * gixQueryNo);
  oTagSet.vAppend(C_TAG_SUBQUERY_NO, (i8) 0);
  pFile->vPrint(LogicCompiler::szGetMacro(AM_QUERY_INIT, &oTagSet, sMacro));
  oTagSet.vReset();

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // DEFAULT_HANDLERS
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  pFile->vPrint(LogicCompiler::szGetMacro(AM_DEFAULT_HANDLER_STARTMARKER, &oTagSet, sMacro));
  pFile->vPrint(LogicCompiler::szGetMacro(AM_DEFAULT_HANDLER_DATAPULSE, &oTagSet, sMacro));

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // STREAMDEF_HANDLER for groupBy
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  pFile->vPrint(LogicCompiler::szGetMacro(AM_HANDLER_STREAMDEF_GBY, &oTagSet, sMacro));
  pFile->vPrint("%s\n"_Stz, pExprEval->m_sAsmGbyIdx.azChar());
  pFile->vPrint(LogicCompiler::szGetMacro(AM_HANDLER_STREAMDEF_GBY_END, &oTagSet, sMacro));

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // EOSMARKER_HANDLER for groupBy
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  oTagSet.vAppend(C_TAG_EOS_PROCESS_ROUTINE, "INDEX_TRAVERSAL"_Stz);
  pFile->vPrint(LogicCompiler::szGetMacro(AM_HANDLER_EOSMARKER_GBY, &oTagSet, sMacro));
  oTagSet.vReset();


  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // CRWBUFFER_HANDLER
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  //   to execute PROJECT_COLUMNS for each row
  ASSERT(pExprEval->m_iStreamInColumns);
  oTagSet.vAppend(C_TAG_CRW_PROCESS_ROUTINE, "CRW_PROJECT_COLUMNS_AGG"_Stz);
  oTagSet.vAppend(C_TAG_PAYLOAD_ROUTINE, "PAYLOAD_GBY"_Stz);

  pFile->vPrint(LogicCompiler::szGetMacro(AM_HANDLER_CRWBUFFER_AGG_GBY, &oTagSet, sMacro));
  oTagSet.vReset();

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // ROWBUFFER_HANDLER
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  //   to execute PROJECT_COLUMNS for each row
  ASSERT(pExprEval->m_iStreamInColumns);
  oTagSet.vAppend(C_TAG_ROW_PROCESS_ROUTINE, "ROW_PROJECT_COLUMNS_AGG"_Stz);
  oTagSet.vAppend(C_TAG_PAYLOAD_ROUTINE, "PAYLOAD_GBY"_Stz);
  pFile->vPrint(LogicCompiler::szGetMacro(AM_HANDLER_ROWBUFFER_AGG_GBY, &oTagSet, sMacro));
  oTagSet.vReset();

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // Query Done
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  pFile->vPrint(LogicCompiler::szGetMacro(AM_QUERY_DONE, &oTagSet, sMacro));





  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // Index Traversal routine
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  oTagSet.vAppend(C_TAG_INDEX_PROCESS_ROUTINE, "PAYLOAD_TO_BUFFER"_Stz);
  pFile->vPrint(LogicCompiler::szGetMacro(AM_INDEX_TRAVERSAL, &oTagSet, sMacro));
  oTagSet.vReset();


  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // PAYLOAD_TO_BUFFER
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  pFile->vPrint(LogicCompiler::szGetMacro(AM_PAYLOAD_TO_BUFFER, &oTagSet, sMacro));
  pFile->vPrint("%s\n"_Stz, pExprEval->m_sPayLoadToBuffer.azChar());
  pFile->vPrint(LogicCompiler::szGetMacro(AM_PAYLOAD_TO_BUFFER_END, &oTagSet, sMacro));


  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // PAYLOAD_GBY
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  oTagSet.vAppend(C_TAG_PAYLOAD_KEY, "PAYLOAD_KEY"_Stz);
  pFile->vPrint(LogicCompiler::szGetMacro(AM_PAYLOAD_GBY, &oTagSet, sMacro));
  oTagSet.vReset();

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // PAYLOAD_GBYKEY
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  pFile->vPrint(LogicCompiler::szGetMacro(AM_PAYLOAD_KEY, &oTagSet, sMacro));
  pFile->vPrint("%s\n"_Stz, pExprEval->m_sPayLoadKey.azChar());
  oTagSet.vReset();

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // PAYLOAD_STORE
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  oTagSet.vAppend(C_TAG_PAYLOAD_BYTES, pExprEval->m_nPayLoadBytes);
  pFile->vPrint(LogicCompiler::szGetMacro(AM_PAYLOAD_STORE, &oTagSet, sMacro));
  pFile->vPrint("%s\n"_Stz, pExprEval->m_sPayLoadStore.azChar());
  oTagSet.vReset();

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // PAYLOAD_END
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  pFile->vPrint(LogicCompiler::szGetMacro(AM_PAYLOAD_END, &oTagSet, sMacro));
  oTagSet.vReset();

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // PAYLOAD_ADD
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  pFile->vPrint(LogicCompiler::szGetMacro(AM_PAYLOAD_ADD, &oTagSet, sMacro));
  pFile->vPrint("%s\n"_Stz, pExprEval->m_sAsmText.azChar());
  oTagSet.vReset();

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // PAYLOAD_END
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  pFile->vPrint(LogicCompiler::szGetMacro(AM_PAYLOAD_END, &oTagSet, sMacro));
  oTagSet.vReset();


  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // CRW_PROJECT_COLUMNS
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // add the columns to be projected to PROJECT_COLUMNS
  if (pExprEval->m_iStreamInColumns) {
    oTagSet.vAppend(C_TAG_PROJECT_ITEMS, pExprEval->m_iStreamInColumns);
    oTagSet.vAppend(C_TAG_PROJECT_SRC_COLUMNS, pExprEval->m_uExprEvalColumns);

    pFile->vPrint(LogicCompiler::szGetMacro(AM_CRW_PROJECT_COLUMNS_AGG, &oTagSet, sMacro));

    oTagSet.vReset();
  }
  else {
    pFile->vPrint(LogicCompiler::szGetMacro(AM_CRW_PROJECT_NONE, &oTagSet, sMacro));
  }

  //pFile->vPrint("%s\n"_Stz, pExprEval->m_sAsmText.azChar());
  pFile->vPrint(LogicCompiler::szGetMacro(AM_CRW_PROJECT_END, &oTagSet, sMacro));

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // ROW_PROJECT_COLUMNS
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // add the columns to be projected to PROJECT_COLUMNS
  if (pExprEval->m_iStreamInColumns) {
    oTagSet.vAppend(C_TAG_PROJECT_ITEMS, pExprEval->m_iStreamInColumns);
    oTagSet.vAppend(C_TAG_PROJECT_SRC_COLUMNS, pExprEval->m_uExprEvalColumns);
    pFile->vPrint(LogicCompiler::szGetMacro(AM_ROW_PROJECT_COLUMNS_AGG, &oTagSet, sMacro));
    oTagSet.vReset();
  }
  else {
    pFile->vPrint(LogicCompiler::szGetMacro(AM_ROW_PROJECT_NONE, &oTagSet, sMacro));
  }

  pFile->vPrint(LogicCompiler::szGetMacro(AM_ROW_PROJECT_END, &oTagSet, sMacro));

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // close the file
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  pFile->vClose();

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // Generate asm
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  s16KB sText;
  b = pFile->bOpenRead(sFileName);
  RASSERT(b);
  i8 nSize = pFile->iSize();
  i8 nBytes = pFile->nReadBytes((void*)sText.azChar(), nSize);

  // null terminate the read bytes
  char* sReadStr = sText.azChar();
  sReadStr[nSize] = '\0';

  bool bResult = bGenerateAsm(sText);

  pFile->vClose();
  delete pFile;
}

/*----------------------------------------------------------------------------*/
/* generate asm text for projection                                           */
/*----------------------------------------------------------------------------*/
void LogicUnit::vGenerateProjection(ExprEval* pExprEval, StreamDef* pStreamDefOut) {

  _precondition_(_ADDRESS_, _MAGIC_, CN_LOGICUNIT, _FOOTER_);

  TagSet oTagSet;
  File *pFile = new File(nullptr);

  s256 sFileName;
  s4K sMacro;

  ++gixQueryNo;
  sFileName.vSprint("../tmp/Query%lld_%s.txt"_Stz, *gixQueryNo,
                    AM::pGrid()->pThisNode()->szNodeNm());

  bool b = pFile->bOpenWrite(sFileName);

  ASSERT( ! pExprEval->bHasAggregate() );

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // AM_QUERY_INIT
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  oTagSet.vAppend(C_TAG_QUERY_NO, (i8) * gixQueryNo);
  oTagSet.vAppend(C_TAG_SUBQUERY_NO, (i8) 0);
  pFile->vPrint(LogicCompiler::szGetMacro(AM_QUERY_INIT, &oTagSet, sMacro));
  oTagSet.vReset();

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // DEFAULT_HANDLERS
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  pFile->vPrint(LogicCompiler::szGetMacro(AM_DEFAULT_HANDLER_STARTMARKER, &oTagSet, sMacro));
  pFile->vPrint(LogicCompiler::szGetMacro(AM_DEFAULT_HANDLER_DATAPULSE, &oTagSet, sMacro));
  pFile->vPrint(LogicCompiler::szGetMacro(AM_DEFAULT_HANDLER_EOSMARKER, &oTagSet, sMacro));
  pFile->vPrint(LogicCompiler::szGetMacro(AM_DEFAULT_HANDLER_STREAMDEF, &oTagSet, sMacro));

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // CRWBUFFER_HANDLER
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  //   to execute PROJECT_COLUMNS for each row
  if (pExprEval->m_iStreamInColumns) {
    oTagSet.vAppend(C_TAG_CRW_PROCESS_ROUTINE, "CRW_PROJECT_COLUMNS"_Stz);
  }
  else
    oTagSet.vAppend(C_TAG_CRW_PROCESS_ROUTINE, "CRW_PROJECT_NONE"_Stz);

  pFile->vPrint(LogicCompiler::szGetMacro(AM_HANDLER_CRWBUFFER, &oTagSet, sMacro));
  oTagSet.vReset();

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // ROWBUFFER_HANDLER
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  //   to execute PROJECT_COLUMNS for each row
  if (pExprEval->m_iStreamInColumns) {
    oTagSet.vAppend(C_TAG_ROW_PROCESS_ROUTINE, "ROW_PROJECT_COLUMNS"_Stz);
  }
  else
    oTagSet.vAppend(C_TAG_ROW_PROCESS_ROUTINE, "ROW_PROJECT_NONE"_Stz);

  pFile->vPrint(LogicCompiler::szGetMacro(AM_HANDLER_ROWBUFFER, &oTagSet, sMacro));

  oTagSet.vReset();

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // Query Done
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  pFile->vPrint(LogicCompiler::szGetMacro(AM_QUERY_DONE, &oTagSet, sMacro));


  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // CRW_PROJECT_COLUMNS
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // add the columns to be projected to PROJECT_COLUMNS
  if (pExprEval->m_iStreamInColumns) {
    oTagSet.vAppend(C_TAG_PROJECT_ITEMS, pExprEval->m_iStreamInColumns);
    oTagSet.vAppend(C_TAG_PROJECT_SRC_COLUMNS, pExprEval->m_uExprEvalColumns);
    pFile->vPrint(LogicCompiler::szGetMacro(AM_CRW_PROJECT_COLUMNS, &oTagSet, sMacro));
    oTagSet.vReset();
  }
  else {
    pFile->vPrint(LogicCompiler::szGetMacro(AM_CRW_PROJECT_NONE, &oTagSet, sMacro));
  }

  pFile->vPrint("%s\n"_Stz, pExprEval->m_sAsmText.azChar());
  pFile->vPrint(LogicCompiler::szGetMacro(AM_CRW_PROJECT_END, &oTagSet, sMacro));



  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // ROW_PROJECT_COLUMNS
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // add the columns to be projected to PROJECT_COLUMNS
  if (pExprEval->m_iStreamInColumns) {
    oTagSet.vAppend(C_TAG_PROJECT_ITEMS, pExprEval->m_iStreamInColumns);
    oTagSet.vAppend(C_TAG_PROJECT_SRC_COLUMNS, pExprEval->m_uExprEvalColumns);
    pFile->vPrint(LogicCompiler::szGetMacro(AM_ROW_PROJECT_COLUMNS, &oTagSet, sMacro));
    oTagSet.vReset();
  }
  else {
    pFile->vPrint(LogicCompiler::szGetMacro(AM_ROW_PROJECT_NONE, &oTagSet, sMacro));
  }

  //for (i8 i_Col = 0; i_Col < pStreamDefOut->nTotalColumns(); i_Col++) {
    pFile->vPrint("%s\n"_Stz, pExprEval->m_sAsmText.azChar());
  //}
  pFile->vPrint(LogicCompiler::szGetMacro(AM_ROW_PROJECT_END, &oTagSet, sMacro));

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // close the file
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  pFile->vClose();

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // Generate asm
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  s16KB sText;
  b = pFile->bOpenRead(sFileName);
  RASSERT(b);
  i8 nSize = pFile->iSize();
  i8 nBytes = pFile->nReadBytes((void*)sText.azChar(), nSize);

  // null terminate the read bytes
  char* sReadStr = sText.azChar();
  sReadStr[nSize] = '\0';

  bool bResult = bGenerateAsm(sText);

  pFile->vClose();
  delete pFile;
}

/*----------------------------------------------------------------------------*/
/* generate asm text for range (index)                                        */
/*----------------------------------------------------------------------------*/
void LogicUnit::vGenerateRange(Range* pRange, StreamDef* pStreamDefOut) {

  _precondition_(_ADDRESS_, _MAGIC_, CN_LOGICUNIT, _FOOTER_);

  TagSet oTagSet;
  File *pFile = new File(nullptr);

  s256 sFileName;
  s4K sMacro;

  ++gixQueryNo;
  sFileName.vSprint("../tmp/Query%lld_%s.txt"_Stz, *gixQueryNo,
                    AM::pGrid()->pThisNode()->szNodeNm());

  bool b = pFile->bOpenWrite(sFileName);

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // AM_QUERY_INIT
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  oTagSet.vAppend(C_TAG_QUERY_NO, (i8) * gixQueryNo);
  oTagSet.vAppend(C_TAG_SUBQUERY_NO, (i8) 0);
  pFile->vPrint(LogicCompiler::szGetMacro(AM_QUERY_INIT, &oTagSet, sMacro));
  oTagSet.vReset();

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // DEFAULT_HANDLERS
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  pFile->vPrint(LogicCompiler::szGetMacro(AM_DEFAULT_HANDLER_STARTMARKER, &oTagSet, sMacro));
  pFile->vPrint(LogicCompiler::szGetMacro(AM_DEFAULT_HANDLER_STREAMDEF, &oTagSet, sMacro));
  pFile->vPrint(LogicCompiler::szGetMacro(AM_DEFAULT_HANDLER_CRWBUFFER, &oTagSet, sMacro));
  pFile->vPrint(LogicCompiler::szGetMacro(AM_DEFAULT_HANDLER_ROWBUFFER, &oTagSet, sMacro));
  pFile->vPrint(LogicCompiler::szGetMacro(AM_DEFAULT_HANDLER_EOSMARKER, &oTagSet, sMacro));

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // DATAPULSE HANDLER
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  //   handler initiates index iteration
  oTagSet.vAppend(C_TAG_DATAPULSE_PROCESS_ROUTINE, aAM[pRange->m_eMacro]);
  pFile->vPrint(LogicCompiler::szGetMacro(AM_HANDLER_DATAPULSE, &oTagSet, sMacro));

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // DATAPULSE_INDEXRANGE
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  //   handler intiates index iteration
  oTagSet.vAppend(C_TAG_CRW_PROCESS_ROUTINE, "CRW_PROJECT_STAR"_Stz);
  pFile->vPrint(LogicCompiler::szGetMacro(pRange->m_eMacro, &pRange->m_oTagSet, sMacro));
  oTagSet.vReset();

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // CRW_PROJECT_STAR
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  //   to execute PROJECT_ALL for each row
  pFile->vPrint(LogicCompiler::szGetMacro(AM_CRW_PROJECT_STAR, &oTagSet, sMacro));
  oTagSet.vReset();

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // Query Done
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  pFile->vPrint(LogicCompiler::szGetMacro(AM_QUERY_DONE, &oTagSet, sMacro));
  pFile->vClose();

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // Generate asm
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  s16KB sText;
  b = pFile->bOpenRead(sFileName);
  RASSERT(b);
  i8 nSize = pFile->iSize();
  i8 nBytes = pFile->nReadBytes((void*)sText.azChar(), nSize);

  // null terminate the read bytes
  char* sReadStr = sText.azChar();
  sReadStr[nSize] = '\0';

  bool bResult = bGenerateAsm(sText);

  pFile->vClose();
  delete pFile;
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
