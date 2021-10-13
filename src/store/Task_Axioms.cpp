/*============================================================================*/
/* Task_Axioms.cpp - functions which add axioms during the boot of system     */
/*============================================================================*/

#include "BASE.h"

#include "eWK.h"
#include "TagConstants.h"

extern Stz* gszaMungedText[];

//#define _INTERPRET_V2

/*----------------------------------------------------------------------------*/
/* add munged words into main dictionary                                      */
/*----------------------------------------------------------------------------*/
void Task::vAddMunged(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TASK,_FOOTER_);
  
#ifdef WITH_MUNGED_SEN
  if (AM::bMunged()) {

    if (AM::TRACE::bVerbose(C_VB_BOOT))
      vTrace("[[mainThread]] Adding munged routines...\n"_Stz);

    vNewInput();
    m_pInput->vFormSenTokens();
    
    vAddMunged(nullptr);
    
    vDeleteInput();
  }
#endif
}

/*----------------------------------------------------------------------------*/
/* add initial set of axiomatic routines into dictionary (min set to boot)    */
/*----------------------------------------------------------------------------*/
void Task::vAddAxioms(void)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TASK,_FOOTER_);
  
  if (AM::TRACE::bVerbose(C_VB_BOOT))
    vTrace("[[mainThread]] Adding axiomatic routines...\n"_Stz);
  
  vNewInput();
  
  s48K sDefinition;
  
  
#ifndef _INTERPRET_V2

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // : interpret -- Sen Interpret & Compile Loop
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  sDefinition =
  
  "axiom: interpret"
    
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // read next input token
  //" \"((a))\" . "
  
  "  input::read"
  
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // test for end of input?
  //" \"((b))\" .put input::chars . "
  
  "  input::kind@ TN_NULL == if"
  "    exit_rt"
  "  then"
  
  //"  input::chars ."

  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // test for discarding input
  //" \"((c))\" .put input::chars . "

  "  senCompiler::isDiscarding if"
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
  
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // is input token an integer?
  //" \"((d))\" . "

  "  input::kind@ TN_INTEGER == if"
  
  "    senCompiler::isPostponing if"
  "      \": interpret : SHOULD NOT BE POSTPONING AN INT\" ."
  "      process::quit"
  "    then"

  "    input::chars"             // push str token onto stack
  "    >int"                     // convert to int, leave on stack

  "    senCompiler::isCompiling if"
  "      encode::push"           // compile : encode push int
  "      reenter"
  "    then"
  
  "    reenter"                  // interpret : leave int on stack

  "  then"
  
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // is input token a string?
  //" \"((e))\" . "

  "  input::kind@ TN_STRING == if"
  
  "    senCompiler::isPostponing if"
  "      \": interpret : SHOULD NOT BE POSTPONING A STR\" ."
  "      process::quit"
  "    then"
  
  "    input::chars"             // push str token onto stack
  
  "    senCompiler::isCompiling if"
  "      encode::push"           // compile : encode push str
  "      reenter"
  "    then"
  
  "    reenter"                  // interpret : leave str on stack

  "  then"
  
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // is input token a tag?
  //" \"((f))\" . "

  "  input::kind@ TN_TAG == if"
  
  "    senCompiler::isPostponing if"
  "      \": interpret : SHOULD NOT BE POSTPONING A TAG\" ."
  "      process::quit"
  "      reenter"
  "    then"

  "    senCompiler::isCompiling if"
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

  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // input token should be a name
  //" \"((g))\" . "

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
  
  // top of stack: pointer to word
  

  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // is input token a known word?
  //" \"((h))\" . "
  "  dup not if"
  "    discard"                  // error -- token is an unknown word
  "    input::unknown"
  "    reenter"
  "  then"

  // top of stack: pointer to word

  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // is that word an immediate word?
  //" \"((i))\" . "

  "  dup word.isImmediate if"
  "    execute"                  // execute the immediate word on stack
  "    reenter"
  "  then"
  
  // top of stack: pointer to word

  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // postponing mode?
  //" \"((j))\" . "

  "  senCompiler::isPostponing if"
  "    \"POSTPONING\" ."
  "    encode::call "
  "    \"encode::call\" word::find encode::call"
  "    reenter"
  "  then"

  // top of stack: pointer to word

  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // compiling mode?
  //" \"((k))\" . "

  "  senCompiler::isCompiling if "
  "    encode::call"             // compile : encode call to word on stack
  "    reenter"
  "  then"
  
  // top of stack: pointer to word

  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // in interpreting mode
  //" \"((l))\" . "

  "  execute"                    // interpret: execute word on stack


  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // loop again
  //" \"((m))\" . "
  
  "  reenter"

  "  ;"_Stz;
  
  pCompileAxiomColon(sDefinition);
  
  
 // 🟪🟪🟪🟪🟪🟪🟪🟪🟪🟪🟪🟪🟪🟪🟪🟪🟪🟪🟪🟪🟪🟪🟪🟪🟪🟪🟪🟪🟪🟪🟪🟪🟪🟪🟪
 // 🟪🟪🟪🟪🟪🟪🟪🟪🟪🟪🟪🟪🟪🟪🟪🟪🟪🟪🟪🟪🟪🟪🟪🟪🟪🟪🟪🟪🟪🟪🟪🟪🟪🟪🟪

#else
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // : interpret -- Sen Interpret & Compile Loop
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  sDefinition =
  
  "axiom: interpret"
    
  "  innerInterpreter"  // call the inner interpreter
  
  "  dup 1 == if"       // execute?
  "    execute"
  "    reenter"
  "  then"

  "  dup 0 == if"       // exit?
  "    discard"
  "    exit_rt"
  "  then"

  "  \": interpret : UNKNOWN RETURN FROM innerInterpreter\" ."
  "  \": interpret : TOP OF STACK IS:\" ."
  "  ."
  "  \": interpret : QUITTING...\" ."
  "  process::quit"

  "  ;"_Stz;
  
  pCompileAxiomColon(sDefinition);
  
#endif
  
  // 🟪🟪🟪🟪🟪🟪🟪🟪🟪🟪🟪🟪🟪🟪🟪🟪🟪🟪🟪🟪🟪🟪🟪🟪🟪🟪🟪🟪🟪🟪🟪🟪🟪🟪🟪
  // 🟪🟪🟪🟪🟪🟪🟪🟪🟪🟪🟪🟪🟪🟪🟪🟪🟪🟪🟪🟪🟪🟪🟪🟪🟪🟪🟪🟪🟪🟪🟪🟪🟪🟪🟪

  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // : include file
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  sDefinition =
  "axiom: include-file"
  "  input::push"
    
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
  
  pCompileAxiomColon(sDefinition);
  
  
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
  
  pCompileAxiomColon(sDefinition);
  
  
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
  "    . debug::breakpoint"
  "    process::quit "
  "  then"
  "  include-file"
  "  ;"_Stz;
  
  pCompileAxiomColon(sDefinition);
  
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // Temporary ifdef till we get sen files embedded in the executable
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

  sDefinition =
  
    "axiom: bootSystem::bootSystem              \
                                                \
      \"bootSystem::bootSystem\" .verbose       \
                                                \
      Job[] assert                              \
                                                \
      Job[].addLocal                            \
                                                \
      args.munged if                            \
        \"compiling munged dictionary...\" .    \
      then                                      \
                                                \
      \"se_include.sen\" include                \
                                                \
      Job[].removeLocal                         \
      Job[].addRetired                          \
                                                \
      Task[].grabLock                           \
        true Job[].reapable!                    \
        TR_RETIRE_THIS_TASK Task[].transition!  \
                                                \
    ;"_Stz;
  
  pCompileAxiomColon(sDefinition);
  
  vDeleteInput();
}

/*----------------------------------------------------------------------------*/
/* compile given axiomatic : routine                                          */
/*----------------------------------------------------------------------------*/
Word* Task::pCompileAxiomColon(s48K& sDefinition)
{
  _precondition_(_ADDRESS_,_MAGIC_,CN_TASK,_FOOTER_);
  
  //vTrace("Compile: %s\n"_Stz,sDefinition.szStz());
  
  Word*     pNewWord;
  fpSTEPFN    fpStepFn;
  Word*     pCalledWord;
  CellSingle xVariable;
  
  SenCompiler* pSenCompiler = m_pSenCompiler;
  Input* pInput = m_pInput;
  
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
  if (m_oDicStack.nItems())
    pDic = m_oDicStack.pTop();
  else
    pDic = AM::DIC::pRoot();

  pNewWord = new(pDic) Word(szChars);
  
  pSenCompiler->vCurrentWord(pNewWord);

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
        pCalledWord = m_oDicStack.pFindWord(szChars);
        if (WK_INTRINSIC == pCalledWord->eWord()) {
          fpStepFn = pCalledWord->fpEntryStep();
          if (pCalledWord->bImmediate())
            //((Step*)( * m_pForth  )->*fpStepFn)();
            ( ((Step*)this)->*fpStepFn )();
          else {
            pNewWord->vEncodeCall(pCalledWord->afpEntryStep());
          }
        }
        else if (WK_COLON == pCalledWord->eWord()) {
          fpSTEPFN* afpCalledEntryStep = pCalledWord->afpEntryStep();
          pNewWord->vEncodeCall(afpCalledEntryStep);
        }
        else {
          vThrow(FK_CHECK,__func__,__FILE__,__LINE__,
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
        
        pCalledWord = m_oDicStack.pFindWord("tag::get"_Stz);
        pNewWord->vEncodeCall(pCalledWord->afpEntryStep());
        break;
      }
              
        
      // -- -- -- -- -- -- -- -- -- --
      default:
        vThrow(FK_CHECK,__func__,__FILE__,__LINE__,
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
  
  Input* pInput = m_pInput;

  pInput->vUseString(szDefinition);
  pInput->vUseBegin();
  
  // get string
  pInput->vReadToken();
  
  sValue = (Stz*)pInput->szChars()->azChar();
  
  // get CONSTANT
  pInput->vReadToken();
  //vTrace("CompileConstant: %s\n"_Stz,m_pSenCompiler->szToken()->azChar());
  
  // get name of word
  pInput->vReadToken();
  //vTrace("CompileConstant: %s\n"_Stz,m_pSenCompiler->szToken()->azChar());

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
