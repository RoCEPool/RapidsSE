
/*============================================================================*/
/* Task_Axioms.cpp - functions which add axioms during the boot of system     */
/*============================================================================*/

#include "Tasks.h"

extern Stz* gszaMungedText[];

/*----------------------------------------------------------------------------*/
/* add munged words into main dictionary                                      */
/*----------------------------------------------------------------------------*/
void Task::vAddMunged(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TASK,_FOOTER_);
  
#ifdef WITH_MUNGED_SEN
  if (AM::bMunged()) {

    if (AM::bVerbose())
      vTrace("[[mainThread]] Adding munged routines...\n"_Stz);

    m_pForth->vNewInput();
    m_pForth->pInput()->vFormSenTokens();
    
    vAddMunged(m_pForth);
    
    m_pForth->vDeleteInput();
  }
#endif
}

/*----------------------------------------------------------------------------*/
/* add initial set of axiomatic routines into dictionary (min set to boot)    */
/*----------------------------------------------------------------------------*/
void Task::vAddAxioms(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TASK,_FOOTER_);
  
  if (AM::bVerbose())
    vTrace("[[mainThread]] Adding axiomatic routines...\n"_Stz);
  
  m_pForth->vNewInput();
  
  s48K sDefinition;
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // : interpret -- Sen Interpret & Compile Loop
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  sDefinition =
  
  "axiom: interpret"
    
  
  //" \"((a))\" . "
  // -- -- -- -- -- -- --
  // read next input token
  "  input::read"
  
  
  //" \"((b))\" .put input::chars . "
  // -- -- -- -- -- -- --
  // test for end of input?
  "  input::kind@ TN_NULL == if"
  "    exit_rt"
  "  then"
  
  
  //"  input::chars ."

  
  //" \"((c))\" .put input::chars . "
  // -- -- -- -- -- -- --
  // test for discarding
  "  compiler::isDiscarding if"
  "    input::chars \"?else\" str.same if"
  "      input::chars word::find execute"
  "      reenter"
  "    then"
  "    input::chars \"?]\" str.same if"
  "      input::chars word::find execute"
  "      reenter"
  "    then"
  "    reenter"
  "  then"
  
  
  //" \"((d))\" . "
  // -- -- -- -- -- -- --
  // is token an integer?
  "  input::kind@ TN_INTEGER == if"
  
  "    compiler::isPostponing if"
  "      \": interpret : SHOULD NOT BE POSTPONING AN INT\" ."
  "      process::quit"
  "    then"

  "    input::chars"             // push str token onto stack
  "    >int"                     // convert to int, leave on stack

  "    compiler::isCompiling if"
  "      encode::push"           // compile : encode push int
  "      reenter"
  "    then"
  
  "    reenter"                  // interpret : leave int on stack

  "  then"
  
  
  //" \"((e))\" . "
  // -- -- -- -- -- -- --
  // is token a string?
  "  input::kind@ TN_STRING == if"
  
  "    compiler::isPostponing if"
  "      \": interpret : SHOULD NOT BE POSTPONING A STR\" ."
  "      process::quit"
  "    then"
  
  "    input::chars"             // push str token onto stack
  
  "    compiler::isCompiling if"
  "      encode::push"           // compile : encode push str
  "      reenter"
  "    then"
  
  "    reenter"                  // interpret : leave str on stack

  "  then"
  
  
  //" \"((f))\" . "
  // -- -- -- -- -- -- --
  // is token a tag?
  "  input::kind@ TN_TAG == if"
  
  "    compiler::isPostponing if"
  "      \": interpret : SHOULD NOT BE POSTPONING A TAG\" ."
  "      process::quit"
  "      reenter"
  "    then"

  "    compiler::isCompiling if"
  "      input::chars"           // compile : encode push "<tagNm>"
  "      encode::push"           //         :
  "      \"tag::get\""           //         : encode tag::get
  "        word::find"           //         :
  "        encode::call"         //         :
  "      reenter"
  "    then"
  
  "    input::chars"             // interpret : push <tagNm> onto stack
  "    tag::get"                 //           : translate to <tagValue>
  "    reenter" 

  "  then"

  
  //" \"((g))\" . "
  //
  // -- token should be a name --
  //
  "  input::kind@ TN_NAME != if"
  "    \"strange token\" . input::kind@ ."
  "    reenter"
  "  then"
  
  // push name token onto stack
  "  input::chars"

  // find word using name on stack
  "  word::find"
  "  dup not if "
  "    \"Unable to find: \" . input::chars ."
  "    input::dump"
  "    process::quit "
  "  then "
  
  // pointer to word on stack

  
  //" \"((h))\" . "
  // -- -- -- -- -- -- --
  // is token a known word?
  "  dup not if"
  "    discard"                  // error -- token is an unknown word
  "    input::unknown"
  "    reenter"
  "  then"

  // pointer to word on stack

  
  //" \"((i))\" . "
  // -- -- -- -- -- -- --
  // is it an immediate word?
  "  dup word.isImmediate if"
  "    execute"                  // execute the immediate word on stack
  "    reenter"
  "  then"
  
  // pointer to word on stack
  
  
  //" \"((j))\" . "
  // -- -- -- -- -- -- --
  // postponing mode?
  "  compiler::isPostponing if"
  "    \"POSTPONING\" ."
  "    encode::call "
  "    \"encode::call\" word::find encode::call"
  "    reenter"
  "  then"

  // pointer to word on stack
  
  
  //" \"((k))\" . "
  // -- -- -- -- -- -- --
  // compiling mode?
  "  compiler::isCompiling if "
  "    encode::call"             // compile : encode call to word on stack
  "    reenter"
  "  then"
  
  // pointer to word on stack
  
  
  //" \"((l))\" . "
  // -- -- -- -- -- -- --
  // we are interpreting
  "  execute"                    // interpret: execute word on stack

  
  //" \"((m))\" . "
  // -- -- -- -- -- -- --
  // loop over
  "  reenter"

  "  ;"_Stz;
  
  pCompileAxiomaticColon(sDefinition);
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // : include file
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  sDefinition =
  "axiom: include-file"
  "  input::push"
  
  "  dup"
  "    word::new input::useInclude"
  
  "  args.senDir str.prepend"
  "  input::open"
  "  input::senTokens"
  "  input::useBegin"
  "  interpret"
  "  input::inspect"
  "  input::close"
  "  input::useEnd"
  "  input::pop"
  "  ;"_Stz;
  
  pCompileAxiomaticColon(sDefinition);
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // : include munged file
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  sDefinition =
  "axiom: include-munged"
  "  input::push"
  //"  dup ."
  "  input::useString"
  "  input::senTokens"
  "  input::useBegin"
  "  interpret"
  "  input::inspect"
  "  input::useEnd"
  "  input::pop"
  "  ;"_Stz;
  
  pCompileAxiomaticColon(sDefinition);
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // : include
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  sDefinition =
  "axiom: include"
  //"  traceon"
  "  args.munged if"
  "    dup word::find if"
  "      word::find execute >int mungedText include-munged"
  "      exit_rt"
  "    then"
  "    . debug::trap"
  "    process::quit "
  "  then"
  "  include-file"
  "  ;"_Stz;
  
  pCompileAxiomaticColon(sDefinition);
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // Temporary ifdef till we get sen files embedded in the executable
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

//#ifdef WITH_APPLE
  sDefinition =
  "axiom: boot_run"
  "  \"boot_run\" .verbose"
  "  args.munged if \"compiling munged dictionary...\" . then"
  "  \"se_boot.sen\" include"
  "  \"boot_run exit_rt\" .verbose"
  "  task::scheduleDelete"
  "  ;"_Stz;
//#else
//  sDefinition =
//  "axiom: boot_run"
//  "  \"boot_run\" .verbose"
//  "  \"se_boot.sen\" include"
//  "  \"boot_run exit_rt\" .verbose"
//  "  task::scheduleDelete"
//  "  ;"_Stz;
//#endif
  
  pCompileAxiomaticColon(sDefinition);
  
  m_pForth->vDeleteInput();
}

/*----------------------------------------------------------------------------*/
/* compile given axiomatic : routine                                          */
/*----------------------------------------------------------------------------*/
Word* Task::pCompileAxiomaticColon(s48K& sDefinition)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TASK,_FOOTER_);
  
  //vTrace("Compile: %s\n"_Stz,sDefinition.szStz());
  
  Word*     pNewWord;
  StepFp    fpStep;
  Word*     pCalledWord;
  Datum     xVariable;
  TokenVec* pTokenVec;
  
  Compiler*  pCompiler = m_pForth->pCompiler();
  Input* pInput = m_pForth->pInput();
  
  pInput->vUseString(sDefinition);
  pInput->vFormSenTokens();
  pInput->vUseBegin();

  // get axiom:
  pInput->vReadToken();
  Stz* szChars = pInput->szChars();
  ASSERT(szChars->bSame("axiom:"_Stz));
  //vTrace("CompileAxiomaticColon: %s\n"_Stz,szChars);

  // get name of word
  pInput->vReadToken();
  szChars = pInput->szChars();
  //vTrace("CompileAxiomaticColon: %s\n"_Stz,szChars);

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

  Dic* pDic;
  if (m_pForth->m_oDicStack.iCount())
    pDic = m_pForth->m_oDicStack.pTop();
  else
    pDic = AM::pRootDic();

  pNewWord = new(pDic) Word(szChars);
  
  pCompiler->vCurrentWord(pNewWord);

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  
  pInput->vReadToken();
  
  while (TN_NULL != pInput->eToken()) {
    
    szChars = pInput->szChars();
    //vTrace("CompileAxiomaticColon: %s\n"_Stz,szChars);
    
    if (szChars->bEqual(";"_Stz))
      break;
    
    eTN eToken = pInput->eToken();
    switch (eToken) {
        
      // -- -- -- -- -- -- -- -- -- --
      case TN_INTEGER:
        xVariable = szChars->iToI8();
        pNewWord->vEncodePush(xVariable);
        break;
        
      // -- -- -- -- -- -- -- -- -- --
      case TN_NAME:
        pCalledWord = m_pForth->m_oDicStack.pFindWord(szChars);
        if (WK_INTRINSIC == pCalledWord->eWord()) {
          fpStep = pCalledWord->fpEntryStep();
          if (pCalledWord->bImmediate())
            ((Step*)(*m_pForth)->*fpStep)();
          else {
            pNewWord->vEncodeCall(pCalledWord->afpEntryStep());
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
        
      // -- -- -- -- -- -- -- -- -- --
      case TN_STRING: {
        xVariable = szChars;
        pNewWord->vEncodePush(xVariable);
        break;
      }
        
      // -- -- -- -- -- -- -- -- -- --
      case TN_TAG: {
        xVariable = szChars;
        pNewWord->vEncodePush(xVariable);
        
        pCalledWord = m_pForth->m_oDicStack.pFindWord("tag::get"_Stz);
        pNewWord->vEncodeCall(pCalledWord->afpEntryStep());
        break;
      }
              
        
      // -- -- -- -- -- -- -- -- -- --
      default:
        vThrow(FK_CHECK,VL_FATAL,__func__,__FILE__,__LINE__,
               "Invalid eToken %lld %s"_Stz,
               eToken,
               szChars
               );
    }
    
    pInput->vReadToken();
  }
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  
  pNewWord->vEncodeCall("exit_rt"_Stz);
  
  // reset charser
  pInput->vUseEnd();
  
  return pNewWord;
}

/*----------------------------------------------------------------------------*/
/* compile a munged definition                                                */
/*----------------------------------------------------------------------------*/
void Task::vCompileMungedColon(Stz* szDefinition)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TASK,_FOOTER_);
  
  //vTrace("vCompileMungedColon :\n%s\n"_Stz,sDefinition.szStz());
  
  s48K sValue;
  
  Compiler* pCompiler = m_pForth->pCompiler();
  Input* pInput = m_pForth->pInput();

  pInput->vUseString(szDefinition);
  pInput->vUseBegin();
  
  // get string
  pInput->vReadToken();

  //if (AM::bVerbose())
  //  vTrace("CompileConstant: %s\n"_Stz,
  //         pInput->szChars());
  
  sValue = (Stz*)pInput->szChars()->azChar();
  
  // get CONSTANT
  pInput->vReadToken();
  //vTrace("CompileConstant: %s\n"_Stz,m_pCompiler->szToken()->azChar());
  
  // get name of word
  pInput->vReadToken();
  //vTrace("CompileConstant: %s\n"_Stz,m_pCompiler->szToken()->azChar());

  Stz* szName = (Stz*)pInput->szChars();

  //vTrace("CompileConstant: %s %s\n"_Stz,aName,sConstantValue.azChar());
  
  AM::vAddConstant(szName,sValue.szStz());
  
  pInput->vReadToken();
  
  pInput->vUseEnd();
}

/*----------------------------------------------------------------------------*/
/* compile a munged definition                                                */
/*----------------------------------------------------------------------------*/
void Task::vCompileMungedColon(i8 i_Def)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TASK,_FOOTER_);
  
  Stz* szText = gszaMungedText[i_Def];
  
  vCompileMungedColon(szText);
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
