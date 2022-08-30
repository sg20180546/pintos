#ifndef FIXED_POINT_H_
#define FIXED_POINT_H_
#define F (1<<14)
#define INT_MAX ((1<<31)-1)
#define INT_MIN (-(1<<31))

#define int_to_fp(n) (n*F)
#define fp_to_int_round(x) x>=0 ? x+(F/2) :\
                                    x-(F/2)
#define fp_to_int(x) x/F
#define add_fp(x,y) (x+y)
#define add_mixed(x,n) (x+(n*F))
#define sub_fp(x,y) (x-y)
#define sub_mixed(x,n) (x-(n*F))
#define mult_fp(x,y) (x*y)/F
#define mult_mixed(x,n) (x*n)
#define div_fp(x,y) ((x/y)*F)
#define div_mixed(x,n) (x/n)

#endif
