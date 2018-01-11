static double pre_x,pre_y;
void Filter_Init(void)
{
    pre_x=0;
    pre_y=0;
}
double Filter_UpdateY(double x)
{
    double y;
    y=pre_y+1.6*x-1.598*pre_x;
    pre_y=y;
    pre_x=x;
    return y;
}