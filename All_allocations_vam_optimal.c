#include<stdio.h>
#include<stdlib.h>
#define BIGNUM 9999

typedef struct cell{
    int cost;
    int allotment;
    int valid;
    int netEvaluation;
}cell;
cell matrix[10][10]={0};

typedef struct element{
    int rowIndex;
    int colIndex;
    int RC; //1 = row, 0 = column
}element;
element selected, loop[4];

int origin[10],destination[10];
int u[10], v[10];
int o,d;

void input(void);
void print(void);
void getDiff(void);
void allotVAM(void);
void allotNWC(void);
void allotRCMinima(int );
void setValidity(void);
int loopCondition(void);
void findMin(int );
void intro(void);

void vam(void);
void NorthWestCorner(void);
void RowMinima(void);
void ColumnMinima(void);
void matrixMinima(void);

int optimalityTest(void);
void getUandV(void);
int checkUandV(void);
int getEvaluation(void);
void createLoop(void);
void optimalSolution(void);
int findCost(void);

int main()
{
    int choice,choice2;
    int optimal = 0,finalCost = 0;
    intro();
    scanf("%d",&choice);
    system("cls");
    switch(choice)
    {
        case 1:
            printf("\nEnter the number of origins and destinations:\n ");
            scanf("%d%d", &o, &d);  //o = origins and d = destinations

            input_improved();//use input() while not using terminal
            
            printf("\nAvailable Metods are");
            printf("\n1 -> North-West Cornar method");
            printf("\n2 -> Row Minima method");
            printf("\n3 -> Column Minima method");
            printf("\n4 -> Matrix Minima method");
            printf("\n5 -> Vogel's Approximation methd (VAM)\n: ");
            scanf("%d",&choice2);
            system("cls");
            switch(choice2)
            {
                case 1:
                    NorthWestCorner();
                    printf("\nThe initial allocation is\n\n");
                    print();
                    finalCost = findCost();
                    printf("\n\nThe total cost in initial solution is: %d units\n",finalCost);
                    getch();
                    break;
                case 2:
                    RowMinima();
                    printf("\nThe initial allocation is\n\n");
                    print();
                    finalCost = findCost();
                    printf("\n\nThe total cost in initial solution is: %d units\n",finalCost);
                    getch();
                    break;
                case 3:
                    ColumnMinima();
                    printf("\nThe initial allocation is\n\n");
                    print();
                    finalCost = findCost();
                    printf("\n\nThe total cost in initial solution is: %d units\n",finalCost); 
					getch(); 
                    break;
                case 4:
                    matrixMinima();
                    printf("\nThe initial allocation is\n\n");
                    print();
                    finalCost = findCost();
                    printf("\n\nThe total cost in initial solution is: %d units\n",finalCost);
                    getch();
                    break;
                case 5:
                    vam();
                    printf("\nThe initial allocation is\n\n");
                    print();
                    finalCost = findCost();
                    printf("\n\nThe total cost in initial solution is: %d units\n",finalCost);
                    getch();
                    break;
                default:
                    printf("\nWrong Choice");
                    break;
            }
            break;
        case 2:
            printf("\nEnter the number of origins and destinations: ");
            scanf("%d%d", &o, &d);  //o = origins and d = destinations
            input();
            system("cls");
            while(1)
            {
                vam();
                print();
                finalCost = findCost();
                printf("\nCost for current allocaton is: %d",finalCost);
                optimal = optimalityTest();
                if(optimal == 1)
                    break;
                optimalSolution();
            }
            break;
        default:
            printf("\nWrong Choice");
            break;
    }
   
}

///////////////// VAM /////////////////////////
void vam()
{
    int i,j;
    while(loopCondition())
    {
        getDiff();
        allotVAM();
        setValidity();
    }
}
void setValidity()
{
    int i,j;
    for(i=0;i<o;i++)    //checking origin array
    {
        if(origin[i] == 0)
        {
            for(j=0;j<d;j++)
                matrix[i][j].valid = 0; 
        }
    }
    for(i=0;i<d;i++)
    {
        if(destination[i] == 0)
        {
            for(j=0;j<o;j++)
                matrix[j][i].valid = 0;
        }
    }
}
void allotVAM()
{
    // makes the allotment
    int i,j,minCost = BIGNUM,minCostIndex;
    if(selected.RC == 1)    //for row
    {
        for(j=0;j<d;j++)
        {
            if(matrix[selected.rowIndex][j].cost < minCost && matrix[selected.rowIndex][j].valid == 1)
            {
                minCost = matrix[selected.rowIndex][j].cost;
                minCostIndex = j;
            } 
        }
        matrix[selected.rowIndex][minCostIndex].allotment = 
            (destination[minCostIndex] < origin[selected.rowIndex])?destination[minCostIndex]:origin[selected.rowIndex];
        
        destination[minCostIndex] -= matrix[selected.rowIndex][minCostIndex].allotment;
        origin[selected.rowIndex] -= matrix[selected.rowIndex][minCostIndex].allotment;

        matrix[selected.rowIndex][minCostIndex].valid = 0;
    }
    if(selected.RC == 0)    //for column
    {
        for(i=0;i<o;i++)
        {
            if(matrix[i][selected.colIndex].cost < minCost && matrix[i][selected.colIndex].valid == 1)
            {
                minCost = matrix[i][selected.colIndex].cost;
                minCostIndex = i;
            } 
        }
        matrix[minCostIndex][selected.colIndex].allotment = 
            (destination[selected.colIndex] < origin[minCostIndex])?destination[selected.colIndex]:origin[minCostIndex];

        destination[selected.colIndex] -= matrix[minCostIndex][selected.colIndex].allotment;
        origin[minCostIndex] -= matrix[minCostIndex][selected.colIndex].allotment;

        matrix[minCostIndex][selected.colIndex].valid = 0;
    }
}
void getDiff()
{
    // -> finds the difference between 1st and 2nd min of row and column
    // -> then finds the max difference
    // -> then returns the indexes through selected structure
    int i,j;
    int min1,min2,max=0;

    //for row
    for(i=0;i<o;i++)
    {
        min1=BIGNUM,min2=BIGNUM;
        for(j=0;j<d;j++)
        {
            if(matrix[i][j].valid == 1)
                if(matrix[i][j].cost < min1)
                    min1 = matrix[i][j].cost;
        }
        for(j=0;j<d;j++)
        {
            if(matrix[i][j].valid == 1)
                if(matrix[i][j].cost > min1 && matrix[i][j].cost < min2)
                    min2 = matrix[i][j].cost;
        }
        matrix[i][j].allotment = min2 - min1;
    }

    //for column
    for(j=0;j<d;j++)
    {
        min1=BIGNUM,min2=BIGNUM;
        for(i=0;i<o;i++)
        {
            if(matrix[i][j].valid == 1)
                if(matrix[i][j].cost < min1)
                    min1 = matrix[i][j].cost;
        }
        for(i=0;i<o;i++)
        {
            if(matrix[i][j].valid == 1)
                if(matrix[i][j].cost > min1 && matrix[i][j].cost < min2)
                    min2 = matrix[i][j].cost;
        }
        matrix[i][j].allotment = min2 - min1;
    }

    for(i=0;i<o;i++)
        if(matrix[i][d].allotment > max)
        {
            max = matrix[i][d].allotment;
            selected.rowIndex = i;
            selected.colIndex = d-1;
            selected.RC = 1;
        }
            
    for(i=0;i<d;i++)
        if(matrix[o][i].allotment > max)
        {
            max = matrix[o][i].allotment;
            selected.rowIndex = o-1;
            selected.colIndex = i;
            selected.RC = 0;
        }
}
int loopCondition()
{
    int i;
    for(i=0;i<o;i++)
    {
        if(origin[i] != 0)
            return 1;   //not ended
    }
    return 0;   //ended
}
//////////////////// North West Corner /////////////////////////////////
void NorthWestCorner()
{
    allotNWC();
}
void allotNWC()
{
    int i,j;
    for(i=0;i<o;i++)
    {
        for(j=0;j<d;j++)
        {
            if(matrix[i][j].valid)
            {
                matrix[i][j].allotment = (origin[i] < destination[j])? origin[i] : destination[j];
                matrix[i][j].valid = 0;
                origin[i] -= matrix[i][j].allotment;
                destination[j] -= matrix[i][j].allotment;
                setValidity();
            }
        }
    }
}
//////////////////// Row Minima /////////////////////////////////
void RowMinima()
{
    allotRCMinima(1);
}
//////////////////Column Minima//////////////////////
void ColumnMinima()
{
    allotRCMinima(2);
}
//////////////////Matrix Minima/////////////////////
void matrixMinima()
{
    allotRCMinima(3);
}
void allotRCMinima(int x)
{
    int i,j,minCost = BIGNUM,flag;
    int o1=o,d1=d;
    if(x == 2)
    {
        o1 = d;
        d1 = o;
    }
    while(loopCondition())
    {
        flag = 0;
        minCost = BIGNUM;
        for(i=0;i<o1;i++)
        {
            for(j=0;j<d1;j++)
            {
                if(x==1)
                {
                    if(matrix[i][j].valid)
                    {
                        if(matrix[i][j].cost < minCost)
                        {
                            flag = 1;
                            minCost = matrix[i][j].cost;
                            selected.rowIndex = i;
                            selected.colIndex = j;
                        } 
                    }
                }
                if(x==2)
                {
                    if(matrix[j][i].valid)
                    {
                        if(matrix[j][i].cost < minCost)
                        {
                            flag = 1;
                            minCost = matrix[j][i].cost;
                            selected.rowIndex = j;
                            selected.colIndex = i;
                        } 
                    }
                }
                if(x==3)
                {
                    if(matrix[i][j].valid)
                    {
                        if(matrix[i][j].cost < minCost)
                        {
                            minCost = matrix[i][j].cost;
                            selected.rowIndex = i;
                            selected.colIndex = j;
                        } 
                    }
                }
            }
            if(flag)
                break;
        }
        matrix[selected.rowIndex][selected.colIndex].allotment = (origin[selected.rowIndex] < destination[selected.colIndex])?
            origin[selected.rowIndex] : destination[selected.colIndex];
        origin[selected.rowIndex] -= matrix[selected.rowIndex][selected.colIndex].allotment;
        destination[selected.colIndex] -= matrix[selected.rowIndex][selected.colIndex].allotment;
        setValidity();
    }
    
}

//////////////////////////////////////////
int optimalityTest()
{
    int i,j,solved=1;
    getUandV();
    solved = getEvaluation();
    print();
    if(solved == 1)
    {
        printf("\nThe optimality condition has been realised");
        printf("\nThis solution is optimal");
        getch();
        return 1;
    }
    else if(solved == 0)
    {
        printf("\nThis solution is NOT optimal");
        printf("\nOptimal solution can be generated");
        createLoop();
        return 0;
    }
    else
    {
        printf("\nThis solution is optimal");
        printf("\nBut other Optimal solution can be generated too");
        getch();
        return 1;
    }
}

void getUandV()
{
    int i,j;
    //Initialize origin and destination with bignum
    for(i=0;i<o;i++)
        u[i] = BIGNUM;
    u[0] = 0;  //making the u1 0;
    for(j=0;j<d;j++)
        v[j] = BIGNUM;

    while(checkUandV())
    {
        for(i=0;i<o;i++)
        {
            for(j=0;j<d;j++)
            {
                if(matrix[i][j].allotment != 0)
                {
                    if(u[i] != BIGNUM)
                        v[j] = matrix[i][j].cost - u[i];
                    else if(v[j] != BIGNUM)
                        u[i] = matrix[i][j].cost - v[j];
                }
            }
        }
    }
}

int getEvaluation()
{
    int i,j,flag = 1;
    for(i=0;i<o;i++)
    {
        for(j=0;j<d;j++)
        {
            if(matrix[i][j].allotment == 0)
                matrix[i][j].netEvaluation = u[i] + v[j] - matrix[i][j].cost;
            else
                matrix[i][j].netEvaluation = -BIGNUM;
        }
    }

    for(i=0;i<o;i++)
    {
        for(j=0;j<d;j++)
        {
            if(matrix[i][j].netEvaluation > 0)
            {
                selected.rowIndex = i;
                selected.colIndex = j;
                flag = 0;
                break;
            }
            else if(matrix[i][j].netEvaluation == 0)
                flag = 2;
        }
    }
    return flag;
}

void createLoop()
{
    int i,j,f = 0;
    loop[0].rowIndex = selected.rowIndex;
    loop[0].colIndex = selected.colIndex;

    for(j=0;j<d;j++)
    {
        f = 0;
        if(matrix[selected.rowIndex][j].allotment != 0 && j != selected.colIndex)
        {
            loop[1].rowIndex = selected.rowIndex;
            loop[1].colIndex = j;

            for(i=0;i<o;i++)
            {
                if(matrix[i][selected.colIndex].allotment != 0 && i != selected.rowIndex)
                {
                    f = 1;
                    loop[3].rowIndex = i;
                    loop[3].colIndex = selected.colIndex;
                }
                if(f == 1)
                {
                    if(matrix[loop[3].rowIndex][loop[1].colIndex].allotment != 0)
                    {
                        loop[2].rowIndex = loop[3].rowIndex;
                        loop[2].colIndex = loop[1].colIndex;
                        return;
                    }
                }
            }
        }
    }
    printf("\n No loop found");
}

int checkUandV()
{
    int i,j;
    for(i=0;i<o;i++)
        if(u[i] == BIGNUM)
            return 1;
    for(j=0;j<d;j++)
        if(v[j] == BIGNUM)
            return 1;
    
    return 0;
}

void optimalSolution()
{
    int i,j,x=0,y=0;
    int theta = 0;

    x = matrix[loop[1].rowIndex][loop[1].colIndex].allotment;
    y = matrix[loop[3].rowIndex][loop[3].colIndex].allotment;
    theta = (x<y)?x:y;

    for(i=0;i<4;i++)
    {
        if(i%2==0)
            matrix[loop[i].rowIndex][loop[i].colIndex].allotment += theta;
        else
            matrix[loop[i].rowIndex][loop[i].colIndex].allotment -= theta;
    }
}
///////////////////////////////////////////////
void input_improved()
{
    int i,j,x,k;

    for(i=0;i<d;i++)
        printf("\tD(%d)",i+1);
        printf("   Available");
        printf("\n");
    for(i=0;i<=o;i++)
    {
        if(i==o)
        {
            printf("Demand  ");
            for(j=0;j<d;j++)
            {
                scanf("%d",&destination[j]);
                printf("\033[1A");  //one line up
                for(k=0;k<j+2;k++)
                    printf("\t");
            }
        }
        else
        {
            printf("O(%d)\t",i+1);
            for(j=0;j<d;j++)
            {
                scanf("%d", &x);
                matrix[i][j].cost = x;
                matrix[i][j].allotment = 0;
                matrix[i][j].valid = 1;
                printf("\033[1A");  //one line up
                for(k=0;k<j+2;k++)
                    printf("\t");
            }
            scanf("%d",&origin[i]);
            printf("\n"); 
        }
    }
}
void input()
{
    int i,j,x;
    for(i=0;i<o;i++)
    {
        printf("\nEnter available items in Origin_(%d): ",i+1);
        scanf("%d",&origin[i]);
    }
    for(i=0;i<d;i++)
    {
        printf("\nEnter requirments in Destinstoion_(%d): ",i+1);
        scanf("%d",&destination[i]);
    }
    printf("\nEnter the cost in following cells");
    for(i=0;i<o;i++)
    {
        for(j=0;j<d;j++)
        {
            printf("\nEnter cost for [%d][%d]: ", i+1,j+1);
            scanf("%d", &x);
            matrix[i][j].cost = x;
            matrix[i][j].allotment = 0;
            matrix[i][j].valid = 1;
        }
    }
}
void print()
{
    int i,j;
    printf("\n");
    for(i=0;i<o;i++)
    {
        for(j=0;j<d;j++)
        {
            printf(" [%d|%d|%d] ", matrix[i][j].allotment, matrix[i][j].cost, matrix[i][j].valid);
            if(j == d-1)
                printf(" %d",origin[i]);
        }
        printf("\n");
    }
    for(j=0;j<d;j++)
        printf("     %d   ",destination[j]);
}
int findCost()
{
    int i,j,finalcost = 0;
    for(i=0;i<o;i++)
    {
        for(j=0;j<d;j++)
        {
            if(matrix[i][j].allotment != 0)
                finalcost += matrix[i][j].allotment * matrix[i][j].cost;
        }
    }
    return finalcost;
}
void intro()
{
    printf("\nThis program solvs the initial allocation for a given transportation problem");
    printf("\nAnd finds the cost in initial allocation");
    printf("\nYou can get the optimal solution but this will use VAM method only");
    printf("\nAll given problems MUST be BALANCED ");
    printf("\nPress any key to continue.... ");
    getch();
    printf("\n\n1-> Get the initial feasible solution only");
    printf("\n2-> Get optimal solution:\n ");
}

