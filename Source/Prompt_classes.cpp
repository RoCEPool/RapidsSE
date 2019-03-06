/*============================================================================*/
/* Prompt_classes.cpp -- classes report                                       */
/*============================================================================*/

#include "Prompt.h"
#include "StorageEngine.h"

PromptNote mPromptNote_classes (
  PF_CLASSES,           // prompt function
  "Prompt_classes"_Stz, // name of the prompt function
  "comment"_Stz,        // comment about the functopm
  "classes"_Stz,        // the prompt command
  nullptr,              // the second word
  bPrompt_classes,      // prompt function
  true,                 // enabled

  true,                 // visible
  "classes"_Stz,         // help
                                ""_Stz                       // description
);

extern AttachMap*  gpAttachMap;

extern eHT geWhichHeap[];
extern i8  giClassSize[];

/*----------------------------------------------------------------------------*/
/* prompt function                                                            */
/*----------------------------------------------------------------------------*/
bool Prompt::bPrompt_classes(SePrompt* pPrompt)
{  
  char cGroup;
  s256 sName;
  
  // - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  // eCN enum
  
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
    
    eHT eHeap = geWhichHeap[i_Class];
    
    char cHeap = '?';
         if (HT_INVALID          == eHeap) cHeap = 'I';
    else if (HT_TRANSIENT_JUMBO  == eHeap) cHeap = 'J';
    else if (HT_TRANSIENT_LARGE  == eHeap) cHeap = 'L';
    else if (HT_TRANSIENT_MEDIUM == eHeap) cHeap = 'M';
    else if (HT_TRANSIENT_SMALL  == eHeap) cHeap = 'S';

    vTrace("    /* %3lld 0x%02X %8lldB Heap:%c */ CN_%s,\n"_Stz,
           i_Class,i_Class,giClassSize[i_Class],
           cHeap,
           aCN[i_Class]);
    
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
    
    vTrace("    /* %3lld 0x%02X */ %-26s // CN_%s\n"_Stz,
           i_Class,i_Class,sName.aChar(),aCN[i_Class]);
    
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
