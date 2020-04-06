/*============================================================================*/
/* Task_boot.cpp - parts of task which boot the system                        */
/*============================================================================*/

#include "Tasks.h"


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
  // : interpret -- Sen Interpret & Compile Loop
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  sDefinition =
  ": interpret"
    
  //" \"((a))\" . "
  // -- -- -- -- -- -- --
  // get next input token
  "  input::next"
  
  
  //" \"((b))\" .put input::token . "
  // -- -- -- -- -- -- --
  // test for end of input?
  "  input::type TN_EOF == if"
  "    exit_rt"
  "  then"
  
  
  //" \"((c))\" . "
  // -- -- -- -- -- -- --
  // is token an integer?
  "  input::type TN_INTEGER == if"
  
  "    compiler::isPostponing if"
  "      \": interpret : SHOULD NOT BE POSTPONING AN INT\" ."
  "      process::quit"
  "    then"

  "    input::token"             // push str token onto stack
  "    >int"                     // convert to int, leave on stack

  "    compiler::isCompiling if"
  "      encode::push"           // compile : encode push int
  "      reenter"
  "    then"
  
  "    reenter"                  // interpret : leave int on stack

  "  then"
  
  
  //" \"((d))\" . "
  // -- -- -- -- -- -- --
  // is token a string?
  "  input::type TN_STRING == if"
  
  "    compiler::isPostponing if"
  "      \": interpret : SHOULD NOT BE POSTPONING A STR\" ."
  "      process::quit"
  "    then"
  
  "    input::token"             // push str token onto stack
  
  "    compiler::isCompiling if"
  "      encode::push"           // compile : encode push str
  "      reenter"
  "    then"
  
  "    reenter"                  // interpret : leave str on stack

  "  then"
  
  
  //" \"((e))\" . "
  // -- -- -- -- -- -- --
  // is token a tag?
  "  input::type TN_TAG == if"
  
  "    compiler::isPostponing if"
  "      \": interpret : SHOULD NOT BE POSTPONING A TAG\" ."
  "      process::quit"
  "      reenter"
  "    then"

  "    compiler::isCompiling if"
  "      input::token"           // compile : encode push "<tagNm>"
  "      encode::push"           //         :
  "      \"tag::get\""           //         : encode tag::get
  "        word::find"           //         :
  "        encode::call"         //         :
  "      reenter"
  "    then"
  
  "    input::token"             // interpret : push <tagNm> onto stack
  "    tag::get"                 //           : translate to <tagValue>
  "    reenter" 

  "  then"

  
  //" \"((f))\" . "
  //
  // -- token should be a name --
  //
  "  input::type TN_NAME != if"
  "    \"strange token\" . input::type ."
  "    reenter"
  "  then"
  
  // push name token onto stack
  "  input::token"

  // find word using name on stack
  "  word::find"
  "  dup not if "
  "    \"Unable to find: \" . input::token ."
  "    input::dump"
  "    process::quit "
  "  then "
  
  // pointer to word on stack

  
  //" \"((g))\" . "
  // -- -- -- -- -- -- --
  // is token a known word?
  "  dup not if"
  "    drop"                     // error -- token is an unknown word
  "    input::unknown"
  "    reenter"
  "  then"

  // pointer to word on stack

  
  //" \"((h))\" . "
  // -- -- -- -- -- -- --
  // is it an immediate word?
  "  dup word.isImmediate if"
  "    execute"                  // execute the immediate word on stack
  "    reenter"
  "  then"
  
  // pointer to word on stack
  
  
  //" \"((i))\" . "
  // -- -- -- -- -- -- --
  // postponing mode?
  "  compiler::isPostponing if"
  "    \"POSTPONING\" ."
  "    encode::call "
  "    \"encode::call\" word::find encode::call"
  "    reenter"
  "  then"

  // pointer to word on stack
  
  
  //" \"((j))\" . "
  // -- -- -- -- -- -- --
  // compiling mode?
  "  compiler::isCompiling if "
  "    encode::call"                 // compile : encode call to word on stack
  "    reenter"
  "  then"
  
  // pointer to word on stack

  
  //" \"((k))\" . "
  // -- -- -- -- -- -- --
  // we are interpreting
  "  execute"                    // interpret: execute word on stack

  
  //" \"((l))\" . "
  // -- -- -- -- -- -- --
  // loop over
  "  reenter"

  "  ;"_Stz;
  
  pCompileColon(sDefinition);
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // : include file
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
  // : include-string
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
  // : include
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  sDefinition =
  ": include"
  //"  traceon"
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
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

#ifdef WITH_APPLE
  sDefinition =
  ": boot_run"
  "  \"boot_run\" .verbose"
  "  args.munged if \"compiling munged dictionary...\" . then"
  "  \"se_boot.sen\" include"
  "  \"boot_run exit_rt\" .verbose"
  "  task::delete"
  "  ;"_Stz;
#else
  sDefinition =
  ": boot_run"
  "  \"boot_run\" .verbose"
  "  \"se_boot.sen\" include"
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
  Datum xVariable;
  
  vTrace("Compile: %s\n"_Stz,sDefinition.szStz());
  
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
  
  while (TN_EOF != m_pForth->m_pCompiler->pTokeniser()->eToken()) {
    
    //vTrace("CompileColon: %s\n"_Stz,m_sToken.azChar());
    
//  if (StrLib::bEqualTo(";",m_pCompiler->szToken()->azChar()))
    if (m_pForth->m_pCompiler->szToken()->bEqual(";"_Stz))
      break;
    
    eTN eToken = m_pForth->m_pCompiler->pTokeniser()->eToken();
    switch (eToken) {
        
      case TN_INTEGER:
        xVariable = m_pForth->m_pCompiler->szToken()->iToI8();
        pNewWord->vEncodePush(xVariable);
        break;
        
      case TN_NAME:
        pCalledWord = m_pForth->m_oDicStack.pFindWord(m_pForth->m_pCompiler->szToken());
        if (WK_INTRINSIC == pCalledWord->eWord()) {
          fpStep = pCalledWord->fpEntryStep();
          if (pCalledWord->bImmediate())
            ((Step*)(*m_pForth)->*fpStep)();
          else {
            pNewWord->vAppend(pCalledWord->afpEntryStep());
          }
        }
        else if (WK_ROUTINE == pCalledWord->eWord()) {
          StepFp* afpCalledEntryStep = pCalledWord->afpEntryStep();
          pNewWord->vEncodeCall(afpCalledEntryStep);
        }
        else {
          vThrow(FK_CHECK,VL_FATAL,__func__,__FILE__,__LINE__,
                 "Invalid eWordType %lld"_Stz,pCalledWord->eWord());
        }
        break;
        
      case TN_STRING: {
        xVariable = m_pForth->m_pCompiler->szToken();
        pNewWord->vEncodePush(xVariable);
        break;
      }
        
      case TN_TAG: {
        xVariable = m_pForth->m_pCompiler->szToken();
        pNewWord->vEncodePush(xVariable);
        
        pCalledWord = m_pForth->m_oDicStack.pFindWord("tag::get"_Stz);
        pNewWord->vAppend(pCalledWord->afpEntryStep());
        break;
      }
        
      default:
        vThrow(FK_CHECK,VL_FATAL,__func__,__FILE__,__LINE__,
               "Invalid m_eToken %lld %s"_Stz,
               m_pForth->m_pCompiler->pTokeniser()->eToken(),
               m_pForth->m_pCompiler->szToken()
               );
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
  
  vTrace("vCompileConstant :\n%s\n"_Stz,sDefinition.szStz());
  
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
