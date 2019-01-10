/*============================================================================*/
/* Task_boot.cpp - parts of task which boot the system                        */
/*============================================================================*/

//#define MUNGED

#include "StorageEngine.h"

extern AttachMap* gpAttachMap;
extern Arguments  gxArguments;

/*----------------------------------------------------------------------------*/
/* add initial set of words in main dictionary                                */
/*----------------------------------------------------------------------------*/
void Task::vAddBaseWords(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TASK,_FOOTER_);
  
  vAddRoutines();

  MP_Task mpTask(this);
  
#ifdef MUNGED
  vAddMunged(mpTask);
#endif
}

/*----------------------------------------------------------------------------*/
/* add initial set of axiomatic routines into dictionary (min set to boot)    */
/*----------------------------------------------------------------------------*/
void Task::vAddRoutines(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TASK,_FOOTER_);
  
  if (gpAttachMap->m_bVerbose)
    vTrace("[[mainThread]] Adding routines...\n"_Stz);
  
  s48K sDefinition;

  sDefinition =
  ": __nil ;"_Stz;
  
  pCompileColon(sDefinition);
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  sDefinition =
  ": docol"
  
  "  retryPoint"
  "  begin_"
  
  // get next input token
  "    input.next"
  
  // test for end of input?
  "    input.type TN_NONE == if"
  "      exit"
  "    then"
  
  // is token an integer?
  "    input.type TN_INTEGER == if"
  
  "      input.token >int"        // convert str to int, leave on stack
  
  "      docol.isInterpreting if" // interpret leaves int on stack
  "        jumpBegin_"
  "      then"
  
  "      docol.isCompiling if"    // compile encodes push intval
  "        encodePushValue"
  "        jumpBegin_"
  "      then"
  
  "    then"
  
  // is token a string?
  "    input.type TN_STRING == if"
  
  "      input.token"             // put str on stack
  
  "      docol.isInterpreting if" // interpret leaves str on stack
  "        jumpBegin_"
  "      then"
  
  "      docol.isCompiling if"    // compile encodes push StrVal
  "        encodePushValue"
  "        jumpBegin_"
  "      then"
  
  "    then"
  
  // should be a name
  "    input.type TN_NAME != if"  // double check we have a name
  "      \"strange token\" . input.type ."
  "      jumpBegin_"
  "    then"
  
  // is token a known word?
  "    input.token find"
  "    dup not if"                // we have an unknown word
  "      drop"
//"      \"unknown\" ."
//"      input.token ."
  "      input.unknown"
//"      dic.isCompiled not if process::quit then "
  "      jumpBegin_"
  "    then"
  
  // known word on stack
  
  // is an immediate word?
  "    dup word.isImmediate if"   // execute an immediate word
  "      execute"
  "      jumpBegin_"
  "    then"
  
  // are we compiling?
  "    docol.isCompiling if "     // compile encodes call to word
  "      dup word.isPrimitive if"
  "        encodeCallPrimitive"   // primitive
  "        jumpBegin_"
  "      then"
  "      encodeCallRoutine"       // routine
  "      jumpBegin_"
  "    then"
  
  // we are interpreting
  "    execute"                   // execute word
  
  "  jumpBegin_"
  
  " ;"_Stz;
  
  pCompileColon(sDefinition);
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  sDefinition =
  ": ::task_exit"
  "  TS_EXITED task::this task.state!"
  "  ;"_Stz;
  
  pCompileColon(sDefinition);
    
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  sDefinition =
  ": ::enter"
  "  input.stdin"
  "  docol"
  "  ::task_exit"
  "  ;"_Stz;
  
  pCompileColon(sDefinition);
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  sDefinition =
  ": include-file"
  "  input.push"
  "  args.senDir str.prepend"
  "  input.open"
  "  docol"
  "  input.close"
  "  input.pop"
  "  ;"_Stz;
  
  pCompileColon(sDefinition);
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  sDefinition =
  ": include-string"
  "  input.push"
  //"  dup ."
  "  input.text"
  "  docol"
  "  input.pop"
  "  ;"_Stz;
  
  pCompileColon(sDefinition);
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  sDefinition =
  ": include"
//  "  traceon"
  "  args.munged if"
  "    dup find if"
  "      find execute include-string"
  "      exit"
  "    then"
  "    . debug::trap"
  "  then"
  "  include-file"
  "  ;"_Stz;
  
  pCompileColon(sDefinition);
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // Temporary ifdef till we get sen files embedded in the executable
#ifdef __APPLE__
  sDefinition =
  ": boot_run"
  "  \"boot_run\" .verbose"
  "  args.munged if \"compiling munged dictionary...\" . then"
  "  \"job_Boot.sen\" include"
  "  \"boot_run exit\" .verbose"
  "  ::task_exit"
  "  ;"_Stz;
#else
  sDefinition =
  ": boot_run"
  "  \"boot_run\" .verbose"
  "  \"job_Boot.sen\" include"
  "  \"boot_run exit\" .verbose"
  "  ::task_exit"
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
  TaskStep  pStep;
  Word* pCalledWord;
  GenVar xVariable;
  
  m_pCompiler->m_pTokeniser->vUseString(sDefinition);
  
  // get :
  m_pCompiler->m_pTokeniser->vNextToken();
  //vTrace("CompileColon: %s\n"_Stz,m_sToken.aChar());
  
  // get name of word
  m_pCompiler->m_pTokeniser->vNextToken();
  //vTrace("CompileColon: %s\n"_Stz,m_sToken.aChar());
  
  Dic* pDic;
  if (m_oPathStack.iCount())
    pDic = m_oPathStack.pTop();
  else
    pDic = &gpAttachMap->m_xMainDic;

  pNewWord = new(pDic) Word(m_pCompiler->m_pTokeniser->sToken());
  
  m_pCompiler->m_pCurrentWord = pNewWord;

  m_pCompiler->m_pTokeniser->vNextToken();
  
  while (TN_NONE != m_pCompiler->m_pTokeniser->eToken()) {
    
    //vTrace("CompileColon: %s\n"_Stz,m_sToken.aChar());
    
//  if (StrLib::bEqualTo(";",m_pCompiler->m_pTokeniser->sToken().aChar()))
    if (m_pCompiler->m_pTokeniser->sToken().bEqual(";"_Stz))
      break;
    
    switch (m_pCompiler->m_pTokeniser->eToken()) {
        
      case TN_INTEGER:
        xVariable = m_pCompiler->m_pTokeniser->sToken().iToI8();
        pNewWord->vEncode("pushValue"_Stz,&xVariable);
        break;
        
      case TN_NAME:
        pCalledWord = m_oPathStack.pFindWord(m_pCompiler->m_pTokeniser->sToken());
        if (WT_PRIMITIVE == pCalledWord->eWordType()) {
          pStep = pCalledWord->pEntryStep();
          if (pCalledWord->bImmediate())
            (*pStep)(this);
          else {
            pNewWord->vAppend(pCalledWord->ppEntryStep());
          }
        }
        else if (WT_ROUTINE == pCalledWord->eWordType()) {
          TaskStep* ppCalledEntryStep = pCalledWord->ppEntryStep();
          pNewWord->vEncode(ppCalledEntryStep);
        }
        else {
          THROW("Invalid eWordType %lld"_Stz,pCalledWord->eWordType());
        }
        break;
        
      case TN_STRING: {
        xVariable = m_pCompiler->m_pTokeniser->sToken();
        pNewWord->vEncode("pushValue"_Stz,&xVariable);
        break;
      }
        
      default:
        THROW("Invalid m_eToken %lld"_Stz,m_pCompiler->m_pTokeniser->eToken());
    }
    
    m_pCompiler->m_pTokeniser->vNextToken();
  }
  
  pNewWord->vEncode("exit"_Stz);
  

  // reset scanning
  m_pCompiler->m_pTokeniser->vUseEnd();
  
  return pNewWord;
}

/*----------------------------------------------------------------------------*/
/* compile a constant definition                                              */
/*----------------------------------------------------------------------------*/
void Task::vCompileConstant(s48K& sDefinition)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TASK,_FOOTER_);
  
  s48K sConstantValue;
  
  m_pCompiler->m_pTokeniser->vUseString(sDefinition);
  
  // get string
  m_pCompiler->m_pTokeniser->vNextToken();
  //vTrace("CompileConstant: %s\n"_Stz,m_pCompiler->m_pTokeniser->sToken().aChar());
  sConstantValue = (Stz*)m_pCompiler->m_pTokeniser->sToken().aChar();
  
  // get CONSTANT
  m_pCompiler->m_pTokeniser->vNextToken();
  //vTrace("CompileConstant: %s\n"_Stz,m_pCompiler->m_pTokeniser->sToken().aChar());
  
  // get name of word
  m_pCompiler->m_pTokeniser->vNextToken();
  //vTrace("CompileConstant: %s\n"_Stz,m_pCompiler->m_pTokeniser->sToken().aChar());

  Stz* aName = (Stz*)m_pCompiler->m_pTokeniser->sToken().aChar();

  //vTrace("CompileConstant: %s %s\n"_Stz,aName,sConstantValue.aChar());
  
  vAddConstant(aName,(Stz*)sConstantValue.aChar());
  
  m_pCompiler->m_pTokeniser->vNextToken();
  
  m_pCompiler->m_pTokeniser->vUseEnd();
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
