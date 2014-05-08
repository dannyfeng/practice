#include <cstdlib>
#include <cmath>

#if 1
double gaussrand(void)
{
    static double V1, V2, S;
    static int phase = 0;
    double X;
     
    if ( phase == 0 ) {
        do {
            double U1 = (double)rand() / RAND_MAX;
            double U2 = (double)rand() / RAND_MAX;
             
            V1 = 2 * U1 - 1;
            V2 = 2 * U2 - 1;
            S = V1 * V1 + V2 * V2;
        } while(S >= 1 || S == 0);
         
        X = V1 * sqrt(-2 * log(S) / S);
    } else
        X = V2 * sqrt(-2 * log(S) / S);
         
    phase = 1 - phase;
 
    return X;
}
#else
/* 多余的，跟上面的函数功能一样 */
double gaussrand(void)
{
	double u = ((double) rand() / (RAND_MAX)) * 2 - 1;
	double v = ((double) rand() / (RAND_MAX)) * 2 - 1;
	double r = u * u + v * v;
	if (r == 0 || r > 1) return gaussrand();
	double c = sqrt(-2 * log(r) / r);
	return u * c;
}
#endif

