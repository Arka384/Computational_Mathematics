//Simplex algorithm

#include<stdio.h>
#include<stdlib.h>
#define SIZE 10
#define BIGNUM 9999

float table[SIZE][SIZE]={0};
float cj[SIZE]={0}, zj_cj[SIZE]={0}, min_ratio[SIZE]={0};
int N,C;    //n = number of variables, C = number of constraints
int n;  //actual number of columns in the table

typedef struct element{
    int row;
    int col;
}element;
element keyVal; //for key value

void input(void);
void print_table(void);
void find_zjcj(void);   //function which calculates zj-cj row
int check_zjcj(void);
void get_min_ratio(void);
void get_new_table(void);   //this makes the new table
void get_final_solution(void);

int main()
{
    input();    //makes the initial table too
                //  find_zjcj() is called within it
    print_table();

    while(check_zjcj())  //this will be false when optimal solution has been obtained
    {
        get_min_ratio();
        printf("\nkey element = %f\n", table[keyVal.row][keyVal.col]);
        // function for changing the table
        get_new_table();
    }
    get_final_solution();
}

void get_new_table()
{
    int i,j;
    float factor;

    // for the particular row with key value 
    // have to make it to 1 
        // hard coding for first three columns
        table[keyVal.row][1] = cj[keyVal.col-4];
        table[keyVal.row][2] = keyVal.col - 4;
        table[keyVal.row][3] = keyVal.col - 4; 

        float key = table[keyVal.row][keyVal.col];  //this should be taken first cause in the for 
                                                //below the keyvalue was changing to 1
        for(i=4;i<=n;i++)
            table[keyVal.row][i] = table[keyVal.row][i] / key;

    //now for the rest of the rows where 
    // we have to make it to 0
        for(i=1;i<=C;i++)
        {
            if(i == keyVal.row)
                continue;

            factor = table[i][keyVal.col];  //different for each row
            for(j=4;j<=n;j++)
                table[i][j] = table[i][j] - table[keyVal.row][j]*factor;
        }

    // now a new table is created and we will calculate zj-cj
    find_zjcj();

    print_table();
}

int check_zjcj()
{
    int i,flag=0;
    float min = BIGNUM;
    
    for(i=1;i<=N;i++)
    {
        if(zj_cj[i] < 0 )
            flag = 1;   //means contains negetive element
        if(zj_cj[i] < min)
        {
            min = zj_cj[i];
            keyVal.col = i+4; //column index of the key element
        }
    }
    return flag;   //means the problem is solved
}

void get_min_ratio()
{
    int i,j;
    float min = BIGNUM;

    for(i=1;i<=C;i++)
    {
        min_ratio[i] = table[i][4] / table[i][keyVal.col];
    }
    i--; //now i contains the size of min_ration[]

    for(j=1;j<=i;j++)
    {
        if(min_ratio[j] < min && min_ratio[j]!=0 && min_ratio[j] > 0)
        {
            min = min_ratio[j];
            keyVal.row = j;
        }
    }
}

void find_zjcj()
{
    int i,j,k=1;    //k is zj-cj array size
    float zj=0.0;

    for(j=5;j<=n;j++)
    {
        zj = 0.0;
        for(i=1;i<=C;i++)
            zj = zj + table[i][1]*table[i][j];    //calculating zj
        zj_cj[k] = zj - cj[k];
        k++;
    }
}

void get_final_solution()
{
    int i,j;
    printf("\n\nSince all the value of (zj-cj)>=0 ");
    printf("\nWe have obtained the optimal solution");
    printf("\nThe solution is: ");
    for(i=1;i<=C;i++)
    {
        printf("\n X%.0f = %.4f", table[i][2], table[i][4]);
    }
}

void input()
{   //makes the initial table too (table 1)
    int i,j,x;
    printf("\nEnter the number of variables: ");
    scanf("%d",&N);
    printf("\nEnter the number of constraints: ");
    scanf("%d", &C);

    n = N+4;    //actual number of columns

    printf("\nEnter the Maximization problem:\n");  //directly into cj
    for(i=1;i<=N;i++)
    {
        printf("\nCoefficient for variable x%d: ",i);
        scanf("%f", &cj[i]);
    }
    
    printf("\nNow enter the constraints: ");    //directly into table
    for(i=1;i<=C;i++)   
    {
        printf("\nEnter constraint %d",i);  
        for(j=5;j<=n;j++)   //first 4 columns are preserved
        {                   //variables start from column 5
            printf("\nCoefficient of x%d: ",j-4);
            fflush(stdin);
            scanf("%f", &table[i][j]);
        }
        printf("\nWhich equals to = ");
        scanf("%f", &table[i][4]);
    }
    
    for(i=1,j=1;i<=C;i++)
    {
        x = N-C+i;
        table[i][1] = cj[x];
        table[i][2] = x;
        table[i][3] = x;
    }

    find_zjcj();    //calling this to complete initial table
}

void print_table()
{
    int i,j;
    printf("\n============================================================");
    printf("\n\t\tCj = ");
    for(i=1;i<=N;i++)
        printf(" %.4f ",cj[i]);

    printf("\nCB\tB\txb\tb");
    for(i=1;i<=N;i++)
        printf("\ta%d",i);
    printf("\n");
    for(i=1;i<=C;i++)   
    {
        for(j=1;j<=n;j++)
            printf("%.4f\t", table[i][j]);
        printf("\n");
    }
    printf("\t\tzj-cj\t\t");
    for(i=1;i<=N;i++)
        printf("%.4f\t",zj_cj[i]);
    
    printf("\n==============================================");
}