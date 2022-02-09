#include <iostream>
#include <cassert>

using namespace std;

#define _ERR_CODE_NULL_PARAM 110
#define _ERR_CODE_OVERFLOW 111
#define _ERR_CODE_INVALID_SCALE 112

#define _FM_CHECKNULL(arg1, arg2) (((arg1) == C_DATUM_NULL || (arg2) == C_DATUM_NULL) ? 1 : 0)

#define OFLOW_SIGNEDSUM(a,b,c) ( \
(a>0 && b>0) ? ((c<0) ? true : false) : \
(a<0 && b<0) ? ((c>0) ? true : false) : \
false \
)

#define OFLOW_SIGNEDSUB(a,b,c) ( \
(a>0 && b<0) ? ((c<0) ? true : false) : \
(a<0 && b>0) ? ((c>0) ? true : false) : \
false \
)

typedef long long int i8;
#  define LLONG_MAX    __LONG_LONG_MAX__
#  define LLONG_MIN    (-LLONG_MAX-1)
i8 C_DATUM_NULL           = LLONG_MIN + 1;

static long long int pow10[15] = {
    1, 10, 100, 1000, 10000, 100000,
    1000000, 10000000, 100000000, 1000000000,
    10000000000, 100000000000, 1000000000000,
    10000000000000, 100000000000000
};

union Bcd_T {
  struct {
    i8 m_iSpare: 8; // usually 0
    i8 m_iScale: 8; //
    i8 m_iMantissa: 48; // 2's complement
  };
  i8 m_iBcd;
  i8 m_iAsI8;
};
/*
 * //Defined #defs for this instead
bool overflowSignedSum(i8 a, i8 b, i8 c)
{
  if(a>0 && b>0)
  {
    if(c<0)
      return true;
    else
      return false;
  } else if(a<0 && b<0)
  {
    if(c>0)
      return true;
    else
      return false;
  }
  return false;
}
*/

i8 rescale(Bcd_T arg1, i8 scaleDiff, int* err)
{
  if(scaleDiff <0 || scaleDiff > 14)
  {
    *err = _ERR_CODE_INVALID_SCALE;
    return C_DATUM_NULL;
  }
  arg1.m_iScale +=scaleDiff;
  Bcd_T tmp = arg1;
  arg1.m_iMantissa = arg1.m_iMantissa * pow10[scaleDiff];
  if(OFLOW_SIGNEDSUM(tmp.m_iMantissa, tmp.m_iMantissa, arg1.m_iMantissa))
  {
    *err = _ERR_CODE_OVERFLOW;
    return C_DATUM_NULL;
  }
  return arg1.m_iBcd;
}

i8 add(Bcd_T arg1, Bcd_T arg2, int* err)
{
  Bcd_T retVal={0};
  if(*err != 0)
    return C_DATUM_NULL;
  if (_FM_CHECKNULL(arg1.m_iBcd, arg2.m_iBcd))
  {
    *err = _ERR_CODE_NULL_PARAM;
    return C_DATUM_NULL;
  }
  if(arg1.m_iScale < arg2.m_iScale)
  {
    i8 dec_diff = arg2.m_iScale - arg1.m_iScale;
    arg1.m_iBcd = rescale(arg1, dec_diff, err);
    if(*err!=0)
      return C_DATUM_NULL;
  } else if(arg1.m_iScale > arg2.m_iScale)
  {
    i8 dec_diff = arg1.m_iScale - arg2.m_iScale;
    arg2.m_iBcd = rescale(arg2, dec_diff, err);
    if(*err!=0)
      return C_DATUM_NULL;
  }

  retVal.m_iMantissa = arg1.m_iMantissa + arg2.m_iMantissa;
  if(OFLOW_SIGNEDSUM(arg1.m_iMantissa, arg2.m_iMantissa, retVal.m_iMantissa))
  {
    *err = _ERR_CODE_OVERFLOW;
    return C_DATUM_NULL;
  }

  retVal.m_iScale = arg1.m_iScale;
  retVal.m_iSpare = 0;
  return retVal.m_iBcd;
}

i8 subtract(Bcd_T arg1, Bcd_T arg2, int* err)
{
  Bcd_T retVal={0};
  if(*err != 0)
    return C_DATUM_NULL;
  if (_FM_CHECKNULL(arg1.m_iBcd, arg2.m_iBcd))
  {
    *err = _ERR_CODE_NULL_PARAM;
    return C_DATUM_NULL;
  }
  if(arg1.m_iScale < arg2.m_iScale)
  {
    i8 dec_diff = arg2.m_iScale - arg1.m_iScale;
    arg1.m_iBcd = rescale(arg1, dec_diff, err);
    if(*err!=0)
      return C_DATUM_NULL;
  } else if(arg1.m_iScale > arg2.m_iScale)
  {
    i8 dec_diff = arg1.m_iScale - arg2.m_iScale;
    arg2.m_iBcd = rescale(arg2, dec_diff, err);
    if(*err!=0)
      return C_DATUM_NULL;
  }

  retVal.m_iMantissa = arg1.m_iMantissa - arg2.m_iMantissa;
  if(OFLOW_SIGNEDSUB(arg1.m_iMantissa, arg2.m_iMantissa, retVal.m_iMantissa))
  {
    *err = _ERR_CODE_OVERFLOW;
    return C_DATUM_NULL;
  }

  retVal.m_iScale = arg1.m_iScale;
  retVal.m_iSpare = 0;
  return retVal.m_iBcd;
}

int main() {
  Bcd_T tmp1={0}, tmp2={0}, tmp3={0};
  int err = 0;
  //Largest number possible is: 140737488355327 = 2^47 - 1
  //Smallest number possible is: -140737488355328 = -(2^47)
  //-------Test-Case-1----------
  //BCD Addition
  //+ve + +ve BCD add no overflow
  err=0;
  tmp1.m_iMantissa = 100;
  tmp1.m_iScale = 0;

  tmp2.m_iMantissa = 200;
  tmp2.m_iScale = 0;

  tmp3.m_iBcd = add(tmp1, tmp2, &err);
  assert (err == 0);
  assert (tmp3.m_iMantissa == 300);
  assert (tmp3.m_iScale == 0);

  //-------Test-Case-2----------
  //+ve + +ve BCD add no overflow, scale difference 1
  err=0;
  tmp1.m_iMantissa = 100;
  tmp1.m_iScale = 1;

  tmp2.m_iMantissa = 200;
  tmp2.m_iScale = 0;

  tmp3.m_iBcd = add(tmp1, tmp2, &err);
  assert (err == 0);
  assert (tmp3.m_iMantissa == 2100);
  assert (tmp3.m_iScale == 1);

  //-------Test-Case-3----------
  //+ve + +ve BCD add no overflow, scale difference 1
  err=0;
  tmp1.m_iMantissa = 100;
  tmp1.m_iScale = 0;

  tmp2.m_iMantissa = 200;
  tmp2.m_iScale = 1;

  tmp3.m_iBcd = add(tmp1, tmp2, &err);
  assert (err == 0);
  assert (tmp3.m_iMantissa == 1200);
  assert (tmp3.m_iScale == 1);

  //-------Test-Case-4----------
  //-ve + -ve BCD add no overflow, scale difference 1
  err=0;
  tmp1.m_iMantissa = -100;
  tmp1.m_iScale = 1;

  tmp2.m_iMantissa = -200;
  tmp2.m_iScale = 0;

  tmp3.m_iBcd = add(tmp1, tmp2, &err);
  assert (err == 0);
  assert (tmp3.m_iMantissa == -2100);
  assert (tmp3.m_iScale == 1);

  //-------Test-Case-5----------
  //-ve + -ve BCD add no overflow, scale difference 1
  err=0;
  tmp1.m_iMantissa = -200;
  tmp1.m_iScale = 1;

  tmp2.m_iMantissa = -100;
  tmp2.m_iScale = 0;

  tmp3.m_iBcd = add(tmp1, tmp2, &err);
  assert (err == 0);
  assert (tmp3.m_iMantissa == -1200);
  assert (tmp3.m_iScale == 1);

  //-------Test-Case-6----------
  //+ve + -ve BCD add no overflow
  err=0;
  tmp1.m_iMantissa = 100;
  tmp1.m_iScale = 0;

  tmp2.m_iMantissa = -200;
  tmp2.m_iScale = 0;

  tmp3.m_iBcd = add(tmp1, tmp2, &err);
  assert (err == 0);
  assert (tmp3.m_iMantissa == -100);
  assert (tmp3.m_iScale == 0);

  //-------Test-Case-7----------
  //-ve + +ve BCD add no overflow
  err=0;
  tmp1.m_iMantissa = -100;
  tmp1.m_iScale = 0;

  tmp2.m_iMantissa = 200;
  tmp2.m_iScale = 0;

  tmp3.m_iBcd = add(tmp1, tmp2, &err);
  assert (err == 0);
  assert (tmp3.m_iMantissa == 100);
  assert (tmp3.m_iScale == 0);

  //-------Test-Case-8----------
  //-ve + +ve BCD add no overflow, scale difference 1
  err=0;
  tmp1.m_iMantissa = -100;
  tmp1.m_iScale = 1;

  tmp2.m_iMantissa = 200;
  tmp2.m_iScale = 0;

  tmp3.m_iBcd = add(tmp1, tmp2, &err);
  assert (err == 0);
  assert (tmp3.m_iMantissa == 1900);
  assert (tmp3.m_iScale == 1);

  //-------Test-Case-9----------
  //-ve + +ve BCD add no overflow, scale difference 1
  err=0;
  tmp1.m_iMantissa = -100;
  tmp1.m_iScale = 0;

  tmp2.m_iMantissa = 200;
  tmp2.m_iScale = 1;

  tmp3.m_iBcd = add(tmp1, tmp2, &err);
  assert (err == 0);
  assert (tmp3.m_iMantissa == -800);
  assert (tmp3.m_iScale == 1);

  //-------Test-Case-10----------
  //+ve + +ve BCD add overflow
  err=0;
  tmp1.m_iMantissa = 140737488355327;
  tmp1.m_iScale = 0;

  tmp2.m_iMantissa = 2;
  tmp2.m_iScale = 0;

  tmp3.m_iBcd = add(tmp1, tmp2, &err);
  assert (err == _ERR_CODE_OVERFLOW);
  assert (tmp3.m_iAsI8 == C_DATUM_NULL);

  //-------Test-Case-11----------
  //+ve + +ve BCD add overflow
  err=0;
  tmp1.m_iMantissa = 2;
  tmp1.m_iScale = 0;

  tmp2.m_iMantissa = 140737488355327;
  tmp2.m_iScale = 0;

  tmp3.m_iBcd = add(tmp1, tmp2, &err);
  assert (err == _ERR_CODE_OVERFLOW);
  assert (tmp3.m_iAsI8 == C_DATUM_NULL);

  //-------Test-Case-12----------
  //-ve + -ve BCD add overflow
  err=0;
  tmp1.m_iMantissa = -2;
  tmp1.m_iScale = 0;

  tmp2.m_iMantissa = -140737488355328;
  tmp2.m_iScale = 0;

  tmp3.m_iBcd = add(tmp1, tmp2, &err);
  assert (err == _ERR_CODE_OVERFLOW);
  assert (tmp3.m_iAsI8 == C_DATUM_NULL);

  //-------Test-Case-13----------
  //-ve + -ve BCD add overflow
  err=0;
  tmp1.m_iMantissa = -140737488355328;
  tmp1.m_iScale = 0;

  tmp2.m_iMantissa = -2;
  tmp2.m_iScale = 0;

  tmp3.m_iBcd = add(tmp1, tmp2, &err);
  assert (err == _ERR_CODE_OVERFLOW);
  assert (tmp3.m_iAsI8 == C_DATUM_NULL);

  //-------Test-Case-14----------
  //+ve + +ve BCD add overflow during rescale
  err=0;
  tmp1.m_iMantissa = 140737488355325;
  tmp1.m_iScale = 0;

  tmp2.m_iMantissa = 2;
  tmp2.m_iScale = 1;

  tmp3.m_iBcd = add(tmp1, tmp2, &err);
  assert (err == _ERR_CODE_OVERFLOW);
  assert (tmp3.m_iAsI8 == C_DATUM_NULL);

  //-------Test-Case-15----------
  //-ve + -ve BCD add overflow during rescale
  err=0;
  tmp1.m_iMantissa = -140737488355326;
  tmp1.m_iScale = 0;

  tmp2.m_iMantissa = -2;
  tmp2.m_iScale = 1;

  tmp3.m_iBcd = add(tmp1, tmp2, &err);
  assert (err == _ERR_CODE_OVERFLOW);
  assert (tmp3.m_iAsI8 == C_DATUM_NULL);

  //-------Test-Case-16----------
  //+ve + +ve BCD add no overflow, rescale greater than 1
  err=0;
  tmp1.m_iMantissa = 100;
  tmp1.m_iScale = 2;

  tmp2.m_iMantissa = 200;
  tmp2.m_iScale = 0;

  tmp3.m_iBcd = add(tmp1, tmp2, &err);
  assert (err == 0);
  assert (tmp3.m_iMantissa == 20100);
  assert (tmp3.m_iScale == 2);

  //-------Test-Case-17----------
  //+ve + +ve BCD add no overflow, rescale greater than 1
  err=0;
  tmp1.m_iMantissa = 100;
  tmp1.m_iScale = 3;

  tmp2.m_iMantissa = 200;
  tmp2.m_iScale = 0;

  tmp3.m_iBcd = add(tmp1, tmp2, &err);
  assert (err == 0);
  assert (tmp3.m_iMantissa == 200100);
  assert (tmp3.m_iScale == 3);

  //-------Test-Case-17----------
  //+ve + +ve BCD add no overflow, rescale greater than 1
  err=0;
  tmp1.m_iMantissa = 100;
  tmp1.m_iScale = 4;

  tmp2.m_iMantissa = 200;
  tmp2.m_iScale = 0;

  tmp3.m_iBcd = add(tmp1, tmp2, &err);
  assert (err == 0);
  assert (tmp3.m_iMantissa == 2000100);
  assert (tmp3.m_iScale == 4);

  //-------Test-Case-18----------
  //+ve + +ve BCD add no overflow, rescale greater than 1
  err=0;
  tmp1.m_iMantissa = 100;
  tmp1.m_iScale = 5;

  tmp2.m_iMantissa = 200;
  tmp2.m_iScale = 0;

  tmp3.m_iBcd = add(tmp1, tmp2, &err);
  assert (err == 0);
  assert (tmp3.m_iMantissa == 20000100);
  assert (tmp3.m_iScale == 5);

  //-------Test-Case-19----------
  //BCD Subtraction
  //+ve - +ve BCD sub no overflow, rescale 0
  err=0;
  tmp1.m_iMantissa = 100;
  tmp1.m_iScale = 0;

  tmp2.m_iMantissa = 200;
  tmp2.m_iScale = 0;

  tmp3.m_iBcd = subtract(tmp1, tmp2, &err);
  assert (err == 0);
  assert (tmp3.m_iMantissa == -100);
  assert (tmp3.m_iScale == 0);

  //-------Test-Case-20----------
  //+ve - +ve BCD sub no overflow, rescale 0
  err=0;
  tmp1.m_iMantissa = 200;
  tmp1.m_iScale = 0;

  tmp2.m_iMantissa = 100;
  tmp2.m_iScale = 0;

  tmp3.m_iBcd = subtract(tmp1, tmp2, &err);
  assert (err == 0);
  assert (tmp3.m_iMantissa == 100);
  assert (tmp3.m_iScale == 0);

  //-------Test-Case-21----------
  //+ve - -ve BCD sub overflow, rescale 0
  err=0;
  tmp1.m_iMantissa = 140737488355327;
  tmp1.m_iScale = 0;

  tmp2.m_iMantissa = -1;
  tmp2.m_iScale = 0;

  tmp3.m_iBcd = subtract(tmp1, tmp2, &err);
  assert (err == _ERR_CODE_OVERFLOW);
  assert (tmp3.m_iBcd == C_DATUM_NULL);

  //-------Test-Case-22----------
  //-ve - +ve BCD sub overflow, rescale 0
  err=0;
  tmp1.m_iMantissa = -140737488355328;
  tmp1.m_iScale = 0;

  tmp2.m_iMantissa = 1;
  tmp2.m_iScale = 0;

  tmp3.m_iBcd = subtract(tmp1, tmp2, &err);
  assert (err == _ERR_CODE_OVERFLOW);
  assert (tmp3.m_iBcd == C_DATUM_NULL);

  //-------Test-Case-23----------
  //-ve - +ve BCD sub overflow, rescale 0
  err=0;
  tmp1.m_iMantissa = -2;
  tmp1.m_iScale = 0;

  tmp2.m_iMantissa = 140737488355327;
  tmp2.m_iScale = 0;

  tmp3.m_iBcd = subtract(tmp1, tmp2, &err);
  assert (err == _ERR_CODE_OVERFLOW);
  assert (tmp3.m_iBcd == C_DATUM_NULL);

  //-------Test-Case-24----------
  //+ve - -ve BCD sub overflow, rescale 0
  err=0;
  tmp1.m_iMantissa = 140737488355327;
  tmp1.m_iScale = 0;

  tmp2.m_iMantissa = -140737488355328;
  tmp2.m_iScale = 0;

  tmp3.m_iBcd = subtract(tmp1, tmp2, &err);
  assert (err == _ERR_CODE_OVERFLOW);
  assert (tmp3.m_iBcd == C_DATUM_NULL);

  //-------Test-Case-25----------
  //+ve - +ve BCD sub no overflow
  err=0;
  tmp1.m_iMantissa = 100;
  tmp1.m_iScale = 0;

  tmp2.m_iMantissa = 200;
  tmp2.m_iScale = 0;

  tmp3.m_iBcd = subtract(tmp1, tmp2, &err);
  assert (err == 0);
  assert (tmp3.m_iMantissa == -100);
  assert (tmp3.m_iScale == 0);

  //-------Test-Case-26----------
  //+ve - +ve BCD add no overflow, scale difference 1
  err=0;
  tmp1.m_iMantissa = 100;
  tmp1.m_iScale = 1;

  tmp2.m_iMantissa = 200;
  tmp2.m_iScale = 0;

  tmp3.m_iBcd = subtract(tmp1, tmp2, &err);
  assert (err == 0);
  assert (tmp3.m_iMantissa == -1900);
  assert (tmp3.m_iScale == 1);

  //-------Test-Case-3----------
  //+ve - +ve BCD add no overflow, scale difference 1
  err=0;
  tmp1.m_iMantissa = 100;
  tmp1.m_iScale = 0;

  tmp2.m_iMantissa = 200;
  tmp2.m_iScale = 1;

  tmp3.m_iBcd = subtract(tmp1, tmp2, &err);
  assert (err == 0);
  assert (tmp3.m_iMantissa == 800);
  assert (tmp3.m_iScale == 1);

  //-------Test-Case-4----------
  //-ve - -ve BCD add no overflow, scale difference 1
  err=0;
  tmp1.m_iMantissa = -100;
  tmp1.m_iScale = 1;

  tmp2.m_iMantissa = -200;
  tmp2.m_iScale = 0;

  tmp3.m_iBcd = subtract(tmp1, tmp2, &err);
  assert (err == 0);
  assert (tmp3.m_iMantissa == 1900);
  assert (tmp3.m_iScale == 1);

  //-------Test-Case-5----------
  //-ve - -ve BCD add no overflow, scale difference 1
  err=0;
  tmp1.m_iMantissa = -200;
  tmp1.m_iScale = 1;

  tmp2.m_iMantissa = -100;
  tmp2.m_iScale = 0;

  tmp3.m_iBcd = subtract(tmp1, tmp2, &err);
  assert (err == 0);
  assert (tmp3.m_iMantissa == 800);
  assert (tmp3.m_iScale == 1);

  //-------Test-Case-6----------
  //+ve - -ve BCD add no overflow
  err=0;
  tmp1.m_iMantissa = 100;
  tmp1.m_iScale = 0;

  tmp2.m_iMantissa = -200;
  tmp2.m_iScale = 0;

  tmp3.m_iBcd = subtract(tmp1, tmp2, &err);
  assert (err == 0);
  assert (tmp3.m_iMantissa == 300);
  assert (tmp3.m_iScale == 0);

  //-------Test-Case-7----------
  //-ve - +ve BCD add no overflow
  err=0;
  tmp1.m_iMantissa = -100;
  tmp1.m_iScale = 0;

  tmp2.m_iMantissa = 200;
  tmp2.m_iScale = 0;

  tmp3.m_iBcd = subtract(tmp1, tmp2, &err);
  assert (err == 0);
  assert (tmp3.m_iMantissa == -300);
  assert (tmp3.m_iScale == 0);

  //-------Test-Case-8----------
  //-ve - +ve BCD add no overflow, scale difference 1
  err=0;
  tmp1.m_iMantissa = -100;
  tmp1.m_iScale = 1;

  tmp2.m_iMantissa = 200;
  tmp2.m_iScale = 0;

  tmp3.m_iBcd = subtract(tmp1, tmp2, &err);
  assert (err == 0);
  assert (tmp3.m_iMantissa == -2100);
  assert (tmp3.m_iScale == 1);

  //-------Test-Case-9----------
  //-ve - +ve BCD add no overflow, scale difference 1
  err=0;
  tmp1.m_iMantissa = -100;
  tmp1.m_iScale = 0;

  tmp2.m_iMantissa = 200;
  tmp2.m_iScale = 1;

  tmp3.m_iBcd = subtract(tmp1, tmp2, &err);
  assert (err == 0);
  assert (tmp3.m_iMantissa == -1200);
  assert (tmp3.m_iScale == 1);

  //-------Test-Case-10----------
  //-ve - +ve BCD add overflow
  err=0;
  tmp1.m_iMantissa = -140737488355327;
  tmp1.m_iScale = 0;

  tmp2.m_iMantissa = 2;
  tmp2.m_iScale = 0;

  tmp3.m_iBcd = subtract(tmp1, tmp2, &err);
  assert (err == _ERR_CODE_OVERFLOW);
  assert (tmp3.m_iAsI8 == C_DATUM_NULL);

  //-------Test-Case-11----------
  //-ve - -ve BCD add no overflow
  err=0;
  tmp1.m_iMantissa = -2;
  tmp1.m_iScale = 0;

  tmp2.m_iMantissa = -140737488355327;
  tmp2.m_iScale = 0;

  tmp3.m_iBcd = subtract(tmp1, tmp2, &err);
  assert (err == 0);
  assert (tmp3.m_iMantissa == 140737488355325);
  assert (tmp3.m_iScale == 0);

  //-------Test-Case-12----------
  //-ve - -ve BCD add overflow
  err=0;
  tmp1.m_iMantissa = 2;
  tmp1.m_iScale = 0;

  tmp2.m_iMantissa = -140737488355328;
  tmp2.m_iScale = 0;

  tmp3.m_iBcd = subtract(tmp1, tmp2, &err);
  assert (err == _ERR_CODE_OVERFLOW);
  assert (tmp3.m_iAsI8 == C_DATUM_NULL);

  //-------Test-Case-14----------
  //-ve - +ve BCD add overflow during rescale
  err=0;
  tmp1.m_iMantissa = -140737488355325;
  tmp1.m_iScale = 0;

  tmp2.m_iMantissa = 2;
  tmp2.m_iScale = 1;

  tmp3.m_iBcd = subtract(tmp1, tmp2, &err);
  assert (err == _ERR_CODE_OVERFLOW);
  assert (tmp3.m_iAsI8 == C_DATUM_NULL);

  //-------Test-Case-15----------
  //-ve - -ve BCD add overflow during rescale
  err=0;
  tmp1.m_iMantissa = -140737488355326;
  tmp1.m_iScale = 0;

  tmp2.m_iMantissa = -2;
  tmp2.m_iScale = 1;

  tmp3.m_iBcd = subtract(tmp1, tmp2, &err);
  assert (err == _ERR_CODE_OVERFLOW);
  assert (tmp3.m_iAsI8 == C_DATUM_NULL);

  //-------Test-Case-16----------
  //+ve - +ve BCD add no overflow, rescale greater than 1
  err=0;
  tmp1.m_iMantissa = 100;
  tmp1.m_iScale = 2;

  tmp2.m_iMantissa = 200;
  tmp2.m_iScale = 0;

  tmp3.m_iBcd = subtract(tmp1, tmp2, &err);
  assert (err == 0);
  assert (tmp3.m_iMantissa == -19900);
  assert (tmp3.m_iScale == 2);

  //-------Test-Case-17----------
  //+ve - +ve BCD add no overflow, rescale greater than 1
  err=0;
  tmp1.m_iMantissa = 100;
  tmp1.m_iScale = 3;

  tmp2.m_iMantissa = 200;
  tmp2.m_iScale = 0;

  tmp3.m_iBcd = subtract(tmp1, tmp2, &err);
  assert (err == 0);
  assert (tmp3.m_iMantissa == -199900);
  assert (tmp3.m_iScale == 3);

  cout << "All tests passed\n";
  return 0;
}

