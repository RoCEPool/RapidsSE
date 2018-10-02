/*============================================================================*/
/* Prompt_classes.cpp -- classes report                                       */
/*============================================================================*/

#include "Prompt.h"
#include "StorageEngine.h"

PromptNote mPromptNote_classes (
  "Prompt_classes"_Stz, // name of the prompt function
  "comment"_Stz,        // comment about the functopm
  "classes"_Stz,        // the prompt command
  nullptr,              // the second word
  bPrompt_classes,      // prompt function
  true,                 // enabled

  true,                 // visible
  "fun help"_Stz        // help
);

extern Arguments   gxArguments;
extern AttachMap*  gpAttachMap;

/*----------------------------------------------------------------------------*/
/* prompt function                                                            */
/*----------------------------------------------------------------------------*/
bool Prompt::bPrompt_classes(SePrompt* pPrompt)
{  
  char cGroup;
  s256 sName;
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // enum eCN
  
  cGroup = '~';
  
  for (i8 i_Class = 0; i_Class <= CN_ENUMS; i_Class++) {

    char cFirst = *aCN[i_Class]->aChar();

    if ('~' == cGroup) {
      if ('A' == cFirst)
        vTrace("\n"_Stz);
    }
    else  {
      if (cGroup != cFirst)
        vTrace("\n"_Stz);
    }
    
    vTrace("    /* %3lld */ CN_%s,\n"_Stz,
           i_Class,aCN[i_Class]);
    
    if ('~' != cGroup)
      cGroup = cFirst;
    else if ('A' == cFirst)
      cGroup = cFirst;
  }
  
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // enum aCN
  
  vTrace("\n\n"_Stz);

  cGroup = '~';
  
  for (i8 i_Class = 0; i_Class <= CN_ENUMS; i_Class++) {
    
    char cFirst = *aCN[i_Class]->aChar();
    
    if ('~' == cGroup) {
      if ('A' == cFirst)
        vTrace("\n"_Stz);
    }
    else  {
      if (cGroup != cFirst)
        vTrace("\n"_Stz);
    }
    
    
    sName = "\""_Stz;
    sName.vConcat(aCN[i_Class]);
    sName.vConcat("\"_Stz,"_Stz);
    
    vTrace("    /* %3lld */ %-26s // CN_%s\n"_Stz,
           i_Class,sName.aChar(),aCN[i_Class]);
    
    if ('~' != cGroup)
      cGroup = cFirst;
    else if ('A' == cFirst)
      cGroup = cFirst;
  }

  return true;
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
