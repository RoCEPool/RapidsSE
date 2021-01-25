/*============================================================================*/
/* Hack.hpp -- A hack code-mark                                               */
/*============================================================================*/
#pragma once

namespace BASE
{
  class Hack
  {
  public:
    Hack(const char* azText,const char* azFilePath, i8 iLineNo);
  };
}

#define PASTE3(a,b,c) a##b##c
#define GLOBALNM(_fun,_class,_line) PASTE3(_fun,_class,_line)
#define HACK(_class,_text) Hack GLOBALNM(Hack,_class,__LINE__)(_text,__FILE__,__LINE__)


/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
