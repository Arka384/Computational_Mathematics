/*
    this is the program for assignment problem
    flag bit is for marking lines
    boxed denotes the final solution
    balanced_bit == 0 denotes not required
                    1 denotes the last row is dummy row
                    2 denotes the last column is dummy column
	timeLimit_excedeed == 1 denotes that code is stuck in an infinite while() loop
						  execution is terminated in that case

    updates-
    everything is working except for the conditions where we get multiple
    optimal solutions. In that case it will stop the execution after a predefined time limit
    and will show appropriate message.
*/

#include<stdio.h>
#include<malloc.h>
#include<stdlib.h>
#include<time.h>
#define TIMELIMIT 4 //in seconds

typedef struct cell{
    float data;
    int flag;
    int boxed;
}cell;
cell matrix[10][10], copyMat[10][10];

typedef struct element{
    int data;
    int index;
    int valid;
}element;

int balanced_bit = 0;
int timeLimit_excedeed = 0;

void input(int ,int );
void input_improved(int n,int m);
int makeBalanced(int , int );
void createMatrix(int ,int );
void printMatrix(int ,int );
int createLines(int ,int );
void modifyMatrix(int ,int );
int checkMatrix(int ,int );
void printResults(int ,int );
void restrictedValues(int ,int );

int main()
{
    int n,m,i,j;
    int lines = 0;
    char rowName,columnName;
    printf("Program for assignment problems");
    printf("\nThis program works for both balanced and unbalanced problems");
    printf("\nEnter number of row and column: ");
    scanf("%d%d",&n,&m);

    input_improved(n,m);    //while not using terminal use the input() function
//  input(n,m);
    restrictedValues(n,m);
    if(n<m)
        n = makeBalanced(n,m);
    else if(m<n)
        m = makeBalanced(n,m);
    printMatrix(n,m);
    createMatrix(n,m);
    printf("\nThe reduced matrix, which have atleast one 0 in each row and column, is: ");
    printMatrix(n,m);
    while(1)
    {
        lines = createLines(n,m);
        printf("\nno of lines = %d\n\n",lines);
        if(lines == n || timeLimit_excedeed == 1)
            break;
        modifyMatrix(n,m);
    }
    if(timeLimit_excedeed == 1)
    {
        printf("\n!!!! Time limit exceeded !!!!");
        printf("\nExecution took more time than excepted\n");
        printf("\nThis problem might have multiple optimul solutions");
        printf("\nUnable to find solutions");
    }
    else
        printResults(n,m);
    
    return 0;
}

void modifyMatrix(int n,int m)
{
    int i,j;
    float min = 9999; //hardcoding a large value
    //finding minimum among unflagged numbers
    for(i=0;i<n;i++)
    {
        for(j=0;j<m;j++)
        {
            if(matrix[i][j].flag == 0)
            {
                if(matrix[i][j].data < min)
                    min = matrix[i][j].data;
            }
        }
    }
    //subtracting min value from unflagged and adding to junction values
    for(i=0;i<n;i++)
    {
        for(j=0;j<m;j++)
        {
            if(matrix[i][j].flag == 0)
                matrix[i][j].data -= min;
            if(matrix[i][j].flag == 2)
                matrix[i][j].data += min;
        }
    }
    printMatrix(n,m);
}

int createLines(int n,int m)
{
    element row[n],col[n];  //holds number of zeros in row and column
    int i,j,k,f=0,count=0;
    time_t start, end;
    
    // have to make all flag and boxed to 0 of matrix before we proceed
    for(i=0;i<n;i++)
    {
        for(j=0;j<m;j++)
        {
            matrix[i][j].boxed = 0;
            matrix[i][j].flag = 0;
        }
    }

    start = time(NULL);
    while(1)    //while all zeros are not covered
    {   
        //initializing all values of rowMax and colMax
        for(i=0;i<n;i++)
        {
            row[i].data = 0;
            row[i].valid = 1;
            row[i].index = 0;
            col[i].data = 0;
            col[i].valid = 1;
            col[i].index = 0;
        }

        for(i=0;i<n;i++)
        {
            for(j=0;j<m;j++)
            {
                if(matrix[i][j].data == 0 && matrix[i][j].flag == 0)
                {
                    row[i].valid = 1;
                    row[i].data += 1;
                    row[i].index = j;
                }
            }
        }
        for(j=0;j<m;j++)
        {
            for(i=0;i<n;i++)
            {
                if(matrix[i][j].data == 0 && matrix[i][j].flag == 0)
                {
                    col[j].valid = 1;
                    col[j].data += 1;
                    col[j].index = i;
                }
            }
        }

        //find index having value of 1 from row and col
        int RowVal = 0, ColVal = 0;
        int RowIndex = 0, ColIndex = 0;
        
        for(i=0;i<n;i++)
        {
            if(row[i].data == 1 && matrix[i][row[i].index].flag == 0)
            {
                // i contains rowIndex (rowIndex = i)
                //make a line in column indexed ColIndex and mark boxed
                j = row[i].index;
                matrix[i][j].boxed = 1;
                printf("\nBoxed in the location [%d,%d]",i,j);
                printf("\nLine created in column: ");
                for(k=0;k<n;k++)
                {
                    printf("%.0f ",matrix[k][j].data);
                    matrix[k][j].flag = matrix[k][j].flag + 1;  //line created for column
                }
                count++; //for getting number of lines
                f = checkMatrix(n,m);
                if(f==0)
                    break;
            }
        }


        for(j=0;j<n;j++)
        {
            if(col[j].data == 1 && matrix[col[j].index][j].flag == 0)
            {
                // j contains colIndex (colIndex = j)
                //make a line in row indexed rowIndex and mark boxed
                i = col[j].index;
                matrix[i][j].boxed = 1;
                printf("\nLine created in row: ");
                for(k=0;k<n;k++)
                {
                    printf("%.0f ",matrix[i][k].data);
                    matrix[i][k].flag = matrix[i][k].flag + 1;  //line created for column
                }
                count++; //for getting number of lines
                f = checkMatrix(n,m);
                if(f==0)
                    break;
            }
        }

        end = time(NULL);
        if(difftime(end, start)>TIMELIMIT)
        {
            printf("\nExecution terminated");
            timeLimit_excedeed = 1;
            break;
        }

        if(f==0)
            break;
        
    }
    return count;

}

int checkMatrix(int n,int m)
{
    int i,j,f=0;
    for(i=0;i<n;i++)
    {
        for(j=0;j<m;j++)
        {
            if(matrix[i][j].data == 0)
            {
                if(matrix[i][j].flag == 0)
                {
                    f=1;
                    break;
                }
            }
        }
    }
    return f;
}

int makeBalanced(int n, int m)
{
    //this will return the corrected value of the row or column
    int i,j;
    if(n < m)   //means row is less than column
    {
        for(i=0;i<m;i++)
        {
            matrix[n][i].data = 0;
            copyMat[n][i].data = 0;
            matrix[n][i].flag = 0;
        }
        balanced_bit = 1;
        return n+1;
    }
    if(m < n)   //means column is less than row
    {
        for(i=0;i<n;i++)
        {
            matrix[i][m].data = 0;
            copyMat[i][m].data = 0;
            matrix[i][m].flag = 0;
        }
        balanced_bit = 2;
        return m+1;
    }
}

void createMatrix(int n,int m)
{
    int i,j,k;
    float rowMin,colMin;

    for(i=0;i<n;i++)
    {
        rowMin = 9999;
        for(j=0;j<m;j++)
        {
            if(matrix[i][j].data < rowMin)
                rowMin = matrix[i][j].data;
            if(j == m-1)
            {
                for(k=0;k<m;k++)
                    matrix[i][k].data -= rowMin;
            }
        }
    }
    // now we have atleast one zero in every row 
    // but not in column
    printMatrix(n,m);
    for(j=0;j<m;j++)
    {
        colMin = 9999;
        for(i=0;i<n;i++)
        {
            if(matrix[i][j].data == 0)
                break;
            if(matrix[i][j].data < colMin)
                colMin = matrix[i][j].data;
            if(i == n-1)
            {
                for(k=0;k<n;k++)
                    matrix[k][j].data -= colMin;
            }
        }
    }
    printMatrix(n,m);
    
}

void restrictedValues(int n,int m)
{
    int i,j;
    float max_val = 0;
    for(i=0;i<n;i++)
    {
        for(j=0;j<m;j++)
        {
            if(matrix[i][j].data > max_val)
                max_val = matrix[i][j].data;
        }
    }

    for(i=0;i<n;i++)
    {
        for(j=0;j<m;j++)
        {
            if(matrix[i][j].data == -999)
            {
                matrix[i][j].data = max_val;
            }
                
        }
    }
}

void printResults(int n,int m)
{
    int i,j;
    float res=0;
    printf("\nThe optimal assignments are: ");
    if(balanced_bit == 1)
        n--;
    if(balanced_bit == 2)
        m--;
    for(i=0;i<n;i++)
    {
        for(j=0;j<m;j++)
        {
            if(matrix[i][j].boxed == 1)
            {
                res += copyMat[i][j].data;
                printf("\n J%d -> M%d ", i+1, j+1);
            }
        }
    }
    printf("\nMinimum cost is: %.1f",res);
}

void input(int n,int m)
{
    int i,j;
    float x;
    printf("\nTo denote blank or prohabited value use -999 \n");
    for(i=0;i<n;i++)
    {
        for(j=0;j<m;j++)
        {
            printf("\nEnter data for matrix[%d][%d]: ",i,j);
            scanf("%f",&x);
            matrix[i][j].data = x;
            copyMat[i][j].data = x;
            matrix[i][j].flag = 0;
        }
    }
}

void input_improved(int n,int m)
{
    int i,j,k;
    float x;
    printf("Enter the values in the matrix below");
    printf("\nTo denote blank or prohabited value use -999 \n");
    for(j=0;j<m;j++)
        printf("\tM(%d)",j+1);
        printf("\n");
    for(i=0;i<n;i++)
    {
        printf("J(%d)\t",i+1);
        for(j=0;j<m;j++)
        {
            scanf("%f",&x);
            matrix[i][j].data = x;
            copyMat[i][j].data = x;
            matrix[i][j].flag = 0;
            printf("\033[1A");  //one line up
            for(k=0;k<j+2;k++)
                printf("\t");
        }
        printf("\n");
    }
}

void printMatrix(int n,int m)
{
    int i,j;
    printf("\n");   
    for(i=0;i<n;i++)
    {
        for(j=0;j<m;j++)
            printf("%.0f ",matrix[i][j].data);
        printf("\n");
    }
}