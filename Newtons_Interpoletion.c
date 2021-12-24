#include<stdio.h>
#include<malloc.h>
#include<stdlib.h>
int FLAG = 0;
/*
    every required functions will have a flag for denoting if it's
    forward or backward interpolation
    This flag will be called "flag" and is an integer
    if flag == 0 then forward
    if flag == 1 then backward
    if flag == 2 then third option
*/
void input(float * ,float *, int );
void reverse(float *,int );
int makeTable(float ** ,float * ,float * ,int ,int );
void Interpoletion(float ** ,float * ,float * ,float ,int ,int ,int );
void formula(float , float , float *, int ,int ,int );
int checkTable(float ** , int ,int );
void printTable(float ** ,int ,int );
int search(float *,float ,int ,int );
int fact(int );
void fimdMethod(float ** ,float * ,float * ,int ,int );

int main()
{
    int flag,choice;
    int n,i,j,val;
    float X;    //this x is in capital and is for f(x) to find
    printf("\nEnter number of elements in x: ");
    scanf("%d",&n);

    //array x
        float *x = (float*)calloc(n ,sizeof(float));
    //array fx
        float *fx = (float*)calloc(n ,sizeof(float));
    //the interpoletion table
        float **table = (float**)calloc(n,sizeof(float*));
            for(i=0;i<n;i++)
                table[i] = (float*)calloc(n,sizeof(float));

    //taking the users choice
    printf("\nThe options are");
    printf("\n1 -> Forward interpoletion\n2 -> Backword interpoletion\n3 -> Let computer decide");
    printf("\nEnter your choice: ");
    scanf("%d",&choice);
    switch(choice)
    {
        case 1:
            flag = 0;   //set for forward interpoletion
            // and call the functions
            input(x, fx, n);    //taking x and f(x) from input
            val = makeTable(table, x, fx, n, flag);    //the funciton for interpoletion table
            printf("\nEnter the value of X to find: ");
            scanf("%f",&X);
            Interpoletion(table, x, fx, X, n, val, flag);
            break;
        case 2:
            flag = 1;   //set for backward interpoletion
            // and call the functions
            input(x, fx, n);    //taking x and f(x) from input
            reverse(fx, n);
            val = makeTable(table, x, fx, n, flag);    //the funciton for interpoletion table
            printf("\nEnter the value of X to find: ");
            scanf("%f",&X);
            Interpoletion(table, x, fx, X, n, val, flag);
            break;
        case 3:
            FLAG = 1;
            fimdMethod(table, x, fx, n, flag);
            break;
        default:
            printf("\n!!!Please enter correct option!!!");
    }
}

void Interpoletion(float **table,float *x,float *fx,float X,int n,int val,int flag)
{   
    int i,j,f=0,f1=0;
    float h = 0.0;
    f = search(x, X, n, flag);//if found f stores the location of X in x[]
    if(f == -1)
    {
        printf("!!! Invalid value of X !!!");
        exit(1);
    }
    if(flag == 1)   //for backward
    {
        f1 = f;
        f = (n-1)-f;
    }

    // dx is the final array which stores the dx0,dx1,dx2...
    // val is the size of this final array 
    float *dx = (float*)calloc(val,sizeof(float));
    for(i=0;i<val;i++)
        *(dx+i) = *(*(table+i)+f);
        //values are stored to final array

    //check if interpoletion is possible
    if( *(*(table+(val-1))+0) != *(dx+(val-1)) )
    {
        if(FLAG == 1)//means called from third option
            fimdMethod(table, x, fx, n, flag);
        else
        {
            printf("\nForward interpoletion is'nt possoble for the given value of x");
            exit(1);
        }
    }
    //else possible and
    //call formula() with this array
    h = *(x+1) - *(x+0);    //calculating h = difference between each value of x;
    if(flag == 0)   //forward interpoletion
        formula(X, *(x+f), dx, val, h, flag);
    else if(flag == 1)  //backward interpoletion
        formula(X, *(x+f1), dx, val, h, flag);

}

void formula(float x, float x0, float *fx, int n, int h, int flag)
{
    int i,j;
    float u=(x-x0)/h,ru=1.0,result = 0.0; //result stores the final calculated formula
    float term[n];  //for storing each terms

    term[0] = *(fx+0);    //hard coding the first term f(x)
    if(flag == 0)
        printf("\nUsing Forward interpoletion formula we get\n");
    else if(flag == 1)
        printf("\nUsing Backward interpoletion formula we get\n");
    printf("%.2f",term[0]);
    for(i=1;i<n;i++)
    {
        ru = 1.0;
        printf(" + ");
        for(j=0;j<i;j++)
        {
            if(flag == 0)   //forward interpoletion
            {
                ru = ru * (u-j);
                printf("(u-%d)",j); //prints the portion of u(u-1)(u-2)(u-3)...
            }
            else if(flag == 1)  //backward interpoletion
            {
                ru = ru * (u+j);
                printf("(u+%d)",j); //prints the portion of u(u+1)(u+2)(u+3)...
            }
        }
        printf("(%.2f)/%d!",*(fx+i),i);
        term[i] = ru * (*(fx+i))/fact(i);
    }
    for(i=0;i<n;i++)
        result = result + term[i];
    printf("\n\nThe required value of f(%.2f) = %f \n",x,result);
    exit(1);
}

int makeTable(float **table,float *x, float *fx, int n ,int flag)
{
    int i,j;

    for(i=0;i<n;i++)
        *(*(table+0)+i) = *(fx+i);  

    for(i=0;i<n;i++)
    {
        for(j=0;j<n-i-1;j++)
        {
            if(flag == 0)   //forward interpoletion
                *(*(table+(i+1))+j) = *(*(table+i)+(j+1)) - *(*(table+i)+j);
            else if(flag == 1) //backward interpoletion
                *(*(table+(i+1))+j) = *(*(table+i)+j) - *(*(table+i)+(j+1));
        }
            
        if(!checkTable(table, i, n))
            break;
    }
    printf("\nThe required difference table is\n");
    printTable(table, n, i);
    return i;
}

int checkTable(float **table, int i, int n)
{
    //this function will check where to end the table
    int j,f=0;
    // 1) when all values are same
    for(j=0;j<n-i;j++)
    {
        if( *(*(table+i)+j) != *(*(table+i)+(j+1)) )
        {
            f = 1;
            break;
        }
    }
    /* 
        2) when there is only one value left in the row
        but this conditon is checked automatically when i = n-1 in the makeTable()
        fucntion
    */

    return f;
}

void input(float *x,float *fx, int n)
{
    int i;
    float temp=0.0;
    
    printf("\nNow enter the values of x and corresponding f(x)\n");
    for(i=0;i<n;i++)
    {
        printf("Enter x: ");
            scanf("%f",& *(x+i));
        printf("Enter f(x) for %.2f: ",*(x+i));
            scanf("%f",& *(fx+i));
    }
}

void fimdMethod(float **table,float *x,float *fx,int n,int flag)
{
    static int c=0;
    static float X;
    int val=0;
    c++;
    //here will find for the appropriate method
    if(c==1)
    {
        printf("\nEnter the value of X to find: ");
        scanf("%f",&X);
    }
    //ckecking for forward method
    if(c==1)
    {
        flag = 0;   //set for forward interpoletion
        // and call the functions
        input(x, fx, n);    //taking x and f(x) from input
        val = makeTable(table, x, fx, n, flag);    //the funciton for interpoletion table
        Interpoletion(table, x, fx, X, n, val, flag);
    }
    printf("\nX = %f\n",X);
    //checking for backward method
    if(c==2)
    {
        flag = 1;   //set for backward interpoletion
        // and call the functions
        //input(x, fx, n, flag);    //taking x and f(x) from input
        reverse(fx, n);
        val = makeTable(table, x, fx, n, flag);    //the funciton for interpoletion table
        Interpoletion(table, x, fx, X, n, val, flag);
    }
}

void reverse(float *arr,int n)
{
    int i=0;
    float t;
    n = n-1;
    while(i<n)
    {
        t = *(arr+i);
        *(arr+i) = *(arr+n);
        *(arr+n) = t;
        i++;
        n--;
    }
}

int search(float *arr,float num,int n,int flag)
{
    int i,f=0;
    for(i=0;i<n;i++)
	{
		if(*(arr+i)>num)
		{
			f=1;
			break;
		}
	}
	if(f==1)
    {
        if(flag == 0)
            return i-1;
        else if(flag == 1)
            return i;
    }
	else
		return -1;
}

void printTable(float **table,int n,int max)
{
    static int f=0;
    int i,j,k=0,l=0;
    int cpoy = n;
    if(f==1)
        return;
    f++;
    float **arr = (float**)calloc(n*2,sizeof(float*));
        for(i=0;i<n*2;i++)
            arr[i] = (float*)calloc(max,sizeof(float));

    for(i=0;i<max;i++,n--)
    {
        l = k = i;
        for(j=0;j<n;j++,k+=2)
            *(*(arr+k)+l) = *(*(table+i)+j);
    }

    for(i=0;i<cpoy*2;i++)
    {
        for(j=0;j<max;j++)
        {
            if(*(*(arr+i)+j) == 0.000)
                printf("   ");
            else
                printf("  %.0f  ",*(*(arr+i)+j));
        }
        printf("\n");
    }  
}

int fact(int n)
{
    int i,res=1;
    for(i=1;i<=n;i++)
        res = res * i;
    return res;
}
