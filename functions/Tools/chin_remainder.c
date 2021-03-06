#include "typedef.h"
#include "tools.h"

/**************************************************************************\
@---------------------------------------------------------------------------
@---------------------------------------------------------------------------
@ FILE: chin_remainder.c
@---------------------------------------------------------------------------
@---------------------------------------------------------------------------
@
\**************************************************************************/
/**************************************************************************\
@-------------------------------------------------------------------------
@ chinrest(x1, x2, p1, p2) calculates for positive primes p1,p2
@ and integers x1, x2 an integer x with
@   -(p1*p2)/2 < x < (p1*p2)/2 and
@ x kongruent x1 modulo p1 and x konguent x2 modulo p2
@-------------------------------------------------------------------------
\**************************************************************************/
int 
chin_remainder (int x1, int x2, int p1, int p2)
{

  int a1, a2;
  double q, q1, q2, res, waste;
  int y1, y2;

  a1 = p_inv(p2, p1);
  a2 = p_inv(p1, p2);
  q = ((double) p1) * ((double) p2);
  q1 = ((double) p2) * ((double) a1);
  q2 = ((double) p1) * ((double) a2);

    y1 = x1%p1;
    if(2*y1 <= -p1)
      y1 +=p1;
    if(2*y1 > p1)
      y1 -=p1;
    y2 = x2%p2;
    if(2*y2 <= -p2)
      y2 +=p2;
    if(2*y2 > p2)
      y2 -=p2;
    if(y1 == y2)
      return(y1);
    res = ((double) y1) * q1 + ((double) y2) * q2;
    modf(res/q, &waste);
    res = res - waste * q;
    while( (res/q) > 0.5)
       res = res -q;
    while( (res/q) < -0.5)
       res = res + q;
    return(((int) res));
}
