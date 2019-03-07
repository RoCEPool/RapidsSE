/*============================================================================*/
/* Prompt_floats.cpp -- prompt floats function                                */
/*============================================================================*/

#include "Prompt.h"

PromptNote mPromptNote_floats (
  PF_FLOATS,             // prompt function
  "Prompt_floats"_Stz,   // name of the prompt function
  "comment"_Stz,         // comment about the function
  "floats"_Stz,          // the prompt command
  nullptr,               // the second word
  bPrompt_floats,        // prompt function
  true,                  // enabled

  true,                  // visible
  "floats"_Stz,          // help
  ""_Stz                 // description
);

extern PromptCatalog gxPromptCatalog;

extern PromptNote mPromptNote_actionSequence;

/*----------------------------------------------------------------------------*/
/* prompt function                                                            */
/*----------------------------------------------------------------------------*/
struct xConv {
  union {
    i8 _i;
    d8 _d;
    byte _y[8];
  };
  
} ;

void vHuh(d8 d)
{
  xConv x;
  
  x._d = d;
  
  printf("(%g : %02x%02x%02x%02x%02x%02x%02x%02x %lld) ",
         x._d,
         x._y[7], x._y[6], x._y[5], x._y[4],
         x._y[3], x._y[2], x._y[1], x._y[0],
         x._i);
}

bool Prompt::bPrompt_floats(SePrompt* pPrompt)
{
  struct xConv x;
  
  vTrace("+dbl_min "_Stz); vHuh(DBL_MIN);   vTrace("\n"_Stz);
  vTrace("+dbl_max "_Stz); vHuh(DBL_MAX);   vTrace("\n"_Stz);
  vTrace("-dbl_min "_Stz); vHuh(-DBL_MIN);  vTrace("\n"_Stz);
  vTrace("-dbl_max "_Stz); vHuh(-DBL_MAX);  vTrace("\n"_Stz);

  x._i = C_DATUM_PLUS_INFINITY;
  vTrace("+dbl_inf "_Stz); vHuh(x._d);  vTrace("\n"_Stz);
  
  x._i = C_DATUM_MINUS_INFINITY;
  vTrace("-dbl_inf "_Stz); vHuh(x._d); vTrace("\n"_Stz);
  
  x._i = C_DATUM_NULL;
  vTrace("null     "_Stz); vHuh(x._d);           vTrace("\n"_Stz);

  x._d = DBL_MAX;
  vTrace("+dbl_inf "_Stz); vHuh(x._d);  vTrace("\n"_Stz);
  
  x._d = -DBL_MIN;
  vTrace("-dbl_inf "_Stz); vHuh(x._d); vTrace("\n"_Stz);
  
  x._d = -DBL_MIN;
  x._i += 1;
  vTrace("null     "_Stz); vHuh(x._d);           vTrace("\n"_Stz);
  
  x._i = C_DATUM_PLUS_INFINITY;
  vTrace("+dbl_inf "_Stz); vHuh(x._d);  vTrace("\n"_Stz);

  x._i = C_DATUM_MINUS_INFINITY;
  vTrace("-dbl_inf "_Stz); vHuh(x._d); vTrace("\n"_Stz);
  
  x._i = C_DATUM_NULL;
  vTrace("null     "_Stz); vHuh(x._d); vTrace("\n"_Stz);

  
  printf("%lld: ",0);
  vHuh((d8)0.0);
  vHuh((d8)0.0);
  vHuh((d8)0.0);
  vHuh((d8)0.0);
  printf("\n");

  d8 dMptv = 1.0;
  d8 dDptv = 1.0;
  d8 dMntv = -1.0;
  d8 dDntv = -1.0;
  
  for (i8 n = 1; n <= 312; n++) {
    
//    printf("%lld %llx: %llx %llx %llx %llx : %g %g %g %g\n",
//           n,n,
//           dMptv,dDptv, dMntv,dDntv,
//           dMptv,dDptv, dMntv,dDntv);

    printf("%lld: ",n);
    vHuh(dMptv);
    vHuh(dDptv);
    vHuh(dMntv);
    vHuh(dDntv);
    printf("\n");

    
    dMptv *= 10.0;
    dDptv /= 10.0;
    dMntv *= 10.0;
    dDntv /= 10.0;
  }
  return true;
}

/*============================================================================*/
/* end                                                                        */
/*============================================================================*/
