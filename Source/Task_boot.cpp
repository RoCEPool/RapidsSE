/*============================================================================*/
/* Task_boot.cpp - parts of task which boot the system                        */
/*============================================================================*/

#include "Tasks.h"
#include "AM.hpp"

/*----------------------------------------------------------------------------*/
/* add initial set of words in main dictionary                                */
/*----------------------------------------------------------------------------*/
void Task::vAddBaseWords(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TASK,_FOOTER_);
  
  vAddRoutines();
  
#ifdef WITH_MUNGED_SEN
  if (AM::bMunged())
    vAddMunged(m_pForth);
#endif
}

/*----------------------------------------------------------------------------*/
/* add initial set of axiomatic routines into dictionary (min set to boot)    */
/*----------------------------------------------------------------------------*/
void Task::vAddRoutines(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TASK,_FOOTER_);
  
  if (AM::bVerbose())
    vTrace("[[mainThread]] Adding routines...\n"_Stz);
  
  s48K sDefinition;

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  sDefinition =
  ": __nil ;"_Stz;
  
  pCompileColon(sDefinition);
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  sDefinition =
  ": interpret"
    
  // -- -- -- -- -- -- --
  // get next input token
  "    input::next"
  
  // -- -- -- -- -- -- --
  // test for end of input?
  "    input::type TN_NONE == if"
  "      exit_rt"
  "    then"
  
  // -- -- -- -- -- -- --
  // is token an integer?
  "    input::type TN_INTEGER == if"
  
  "      input::token >int"        // convert str to int, leave on stack
  
  "      compiler::isCompiling if"
  "        ,"                      // compile encodes push intval
  "        reenter"
  "      then"
  
  "      reenter"                  // interpret leaves int on stack

  "    then"
  
  // -- -- -- -- -- -- --
  // is token a string?
  "    input::type TN_STRING == if"
  
  "      input::token"             // put str on stack
  
  "      compiler::isCompiling if"
  "        ,"                      // compile encodes push stzVal
  "        reenter"
  "      then"
  
  "      reenter"                  // interpret leaves str on stack

  "    then"
  
  // -- -- -- -- -- -- --
  // should be a name
  "    input::type TN_NAME != if"  // double check we have a name
  "      \"strange token\" . input::type ."
  "      reenter"
  "    then"
  
  // -- -- -- -- -- -- --
  // is token a known word?
  "    input::token word::find"
  "    dup not if"                 // we have an unknown word
  "      drop"
  "      input::unknown"
  "      reenter"
  "    then"
  
  // -- -- -- -- -- -- --
  // pWord now on stack
  
  // -- -- -- -- -- -- --
  // is an immediate word?
  "    dup word.isImmediate if"
  "      execute"                  // execute an immediate word
  "      reenter"
  "    then"
  
  // -- -- -- -- -- -- --
  // are we compiling?
  "    compiler::isCompiling if "
  "      dup word.isIntrinsic if"
  "        compile,"               // compile encodes call to primitive
  "        reenter"
  "      then"
  "      compile,"                 // compile encodes call to routine
  "      reenter"
  "    then"
  
  // -- -- -- -- -- -- --
  // we are interpreting
  "    execute"                    // execute word
  
  "  reenter"

  "  ;"_Stz;
  
  pCompileColon(sDefinition);
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  sDefinition =
  ": include-file"
  "  input::push"
  "  args.senDir str.prepend"
  "  input::open"
  "  interpret"
  "  input::close"
  "  input::pop"
  "  ;"_Stz;
  
  pCompileColon(sDefinition);
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  sDefinition =
  ": include-string"
  "  input::push"
  //"  dup ."
  "  input::text"
  "  interpret"
  "  input::pop"
  "  ;"_Stz;
  
  pCompileColon(sDefinition);
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  sDefinition =
  ": include"
//  "  traceon"
  "  args.munged if"
  "    dup word::find if"
  "      word::find execute include-string"
  "      exit_rt"
  "    then"
  "    . debug::trap"
  "  then"
  "  include-file"
  "  ;"_Stz;
  
  pCompileColon(sDefinition);
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // Temporary ifdef till we get sen files embedded in the executable

#ifdef WITH_APPLE
  sDefinition =
  ": boot_run"
  "  \"boot_run\" .verbose"
  "  args.munged if \"compiling munged dictionary...\" . then"
  "  \"job_Boot.sen\" include"
  "  \"boot_run exit_rt\" .verbose"
  "  task::delete"
  "  ;"_Stz;
#else
  sDefinition =
  ": boot_run"
  "  \"boot_run\" .verbose"
  "  \"job_Boot.sen\" include"
  "  \"boot_run exit_rt\" .verbose"
  "  task::delete"
  "  ;"_Stz;
#endif
  
  pCompileColon(sDefinition);
}

/*----------------------------------------------------------------------------*/
/* compile given : routine                                                    */
/*----------------------------------------------------------------------------*/
Word* Task::pCompileColon(s48K& sDefinition)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TASK,_FOOTER_);
  
  Word* pNewWord;
  StepFp  fpStep;
  Word* pCalledWord;
  GenVar xVariable;
  
  m_pForth->m_pCompiler->pTokeniser()->vUseString(sDefinition);
  
  // get :
  m_pForth->m_pCompiler->vNextToken();
  //vTrace("CompileColon: %s\n"_Stz,m_sToken.azChar());
  
  // get name of word
  m_pForth->m_pCompiler->vNextToken();
  //vTrace("CompileColon: %s\n"_Stz,m_sToken.azChar());
  
  Dic* pDic;
  if (m_pForth->m_oDicStack.iCount())
    pDic = m_pForth->m_oDicStack.pTop();
  else
    pDic = AM::pRootDic();


  pNewWord = new(pDic) Word(m_pForth->m_pCompiler->szToken());
  
  m_pForth->m_pCompiler->vCurrentWord(pNewWord);

  m_pForth->m_pCompiler->vNextToken();
  
  while (TN_NONE != m_pForth->m_pCompiler->pTokeniser()->eToken()) {
    
    //vTrace("CompileColon: %s\n"_Stz,m_sToken.azChar());
    
//  if (StrLib::bEqualTo(";",m_pCompiler->szToken()->azChar()))
    if (m_pForth->m_pCompiler->szToken()->bEqual(";"_Stz))
      break;
    
    switch (m_pForth->m_pCompiler->pTokeniser()->eToken()) {
        
      case TN_INTEGER:
        xVariable = m_pForth->m_pCompiler->szToken()->iToI8();
        pNewWord->vEncodePush(xVariable);
        break;
        
      case TN_NAME:
        pCalledWord = m_pForth->m_oDicStack.pFindWord(m_pForth->m_pCompiler->szToken());
        if (WK_INTRINSIC == pCalledWord->eKind()) {
          fpStep = pCalledWord->fpEntryStep();
          if (pCalledWord->bImmediate())
            ((Step*)(*m_pForth)->*fpStep)();
          else {
            pNewWord->vAppend(pCalledWord->afpEntryStep());
          }
        }
        else if (WK_ROUTINE == pCalledWord->eKind()) {
          StepFp* afpCalledEntryStep = pCalledWord->afpEntryStep();
          pNewWord->vEncodeCall(afpCalledEntryStep);
        }
        else {
          vThrow(FK_CHECK,VL_FATAL,__func__,__FILE__,__LINE__,
                 "Invalid eWordType %lld"_Stz,pCalledWord->eKind());
        }
        break;
        
      case TN_STRING: {
        xVariable = m_pForth->m_pCompiler->szToken();
        pNewWord->vEncodePush(xVariable);
        break;
      }
        
      default:
        vThrow(FK_CHECK,VL_FATAL,__func__,__FILE__,__LINE__,
               "Invalid m_eToken %lld"_Stz,
               m_pForth->m_pCompiler->pTokeniser()->eToken());
    }
    
    m_pForth->m_pCompiler->vNextToken();
  }
  
  pNewWord->vEncodeCall("exit_rt"_Stz);
  

  // reset scanning
  m_pForth->m_pCompiler->pTokeniser()->vUseEnd();
  
  return pNewWord;
}

/*----------------------------------------------------------------------------*/
/* compile a constant definition                                              */
/*----------------------------------------------------------------------------*/
void Task::vCompileConstant(s48K& sDefinition)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TASK,_FOOTER_);
  
  s48K sValue;
  
  m_pForth->m_pCompiler->pTokeniser()->vUseString(sDefinition);
  
  // get string
  m_pForth->m_pCompiler->vNextToken();
  
  if (AM::bVerbose())
    vTrace("CompileConstant: %s\n"_Stz,
           m_pForth->m_pCompiler->szToken()->azChar());
  
  sValue = (Stz*)m_pForth->m_pCompiler->szToken()->azChar();
  
  // get CONSTANT
  m_pForth->m_pCompiler->vNextToken();
  //vTrace("CompileConstant: %s\n"_Stz,m_pCompiler->szToken()->azChar());
  
  // get name of word
  m_pForth->m_pCompiler->vNextToken();
  //vTrace("CompileConstant: %s\n"_Stz,m_pCompiler->szToken()->azChar());

  Stz* szName = (Stz*)m_pForth->m_pCompiler->szToken()->azChar();

  //vTrace("CompileConstant: %s %s\n"_Stz,aName,sConstantValue.azChar());
  
  AM::vAddConstant(szName,sValue.szStz());
  
  m_pForth->m_pCompiler->vNextToken();
  
  m_pForth->m_pCompiler->pTokeniser()->vUseEnd();
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
