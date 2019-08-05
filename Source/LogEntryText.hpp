/*============================================================================*/
/* LogEntryText.hpp - Log Entry Text                                          */
/*============================================================================*/
#pragma once

namespace Logging
{
  static const i8 C_LOG_TEXT = 128;
  
  class _LogEntryText
  {
  public:
    i8   m_iEnumGroup;  // enum group
    i8   m_iEnum;       // enum/jobid
    i8   m_iForeign;    // foreign job id
  
    char m_caText[C_LOG_TEXT]; // comment
  };
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
