#include <stdio.h>
#include <stdlib.h>
#include <windows.h>
#include "sparse.h"

void Display(int row, int col, SparseEntry e)
{
    printf("Row : %d\nColumn : %d\nEntry : %c\n======================\n",row,col,e);
}

int main()
{
    SparseEntry data;
    Sparse s;
    CreateSparse(&s);
    int x, found, checker = 1, col, row;
    while(checker == 1)
    {
        printf("\n\nSparse Array\n==============\n\n");
        printf ("[1] Insert\n[2] Delete\n[3] Retrieve\n[4] Traverse\n[5] Traverse Column\n[6] Traverse Row\n[7] Clear Sparse\n[8] Check Size\n[9] Exit\n\n");
        scanf ("%d",&x);
        fflush(stdin);
        switch(x)
        {
            case 1:
                printf ("Input row number : ");
                scanf ("%d",&row);
                fflush(stdin);

                printf ("Input columns number : ");
                scanf("%d",&col);
                fflush(stdin);

                printf("Input the data : ");
                scanf("%c",&data);
                fflush(stdin);
                if(InsertNode(&s, &data, row, col) == 1)
                    printf("Done. Node added successfully.\n");
                else
                    printf ("Row number : %d, Column number : %d already exists.\n", row, col);
                break;
            case 2:
                if(!SparseEmpty(&s))
                {
                    printf("Input row number : ");
                    scanf ("%d",&row);
                    fflush(stdin);

                    printf("Input column number : ");
                    scanf("%d",&col);
                    fflush(stdin);

                    if(DeleteNode(&s, &data, row, col) == 1)
                        printf("Done. Node deleted successfully.\n\nRow number : %d\nColumn number : %d\nData : %c\n\n======================\n", row, col, data);
                    else
                        printf("Row number : %d, Column number : %d doesn't exist.\n", row, col);
                }
                else
                    printf("Sparse is already empty.\n");
                break;
            case 3:
                printf("Input row number : ");
                scanf ("%d", &row);
                fflush(stdin);

                printf("Input column number : ");
                scanf("%d", &col);
                fflush(stdin);

                if(RetrieveNode(&s, &data, row, col) == 1)
                    printf("Data : %c\n",data);
                else
                    printf ("Column Number %d, Row Number %d doesn't Exist\n",col,row);
                break;
            case 4:
                if (!SparseEmpty(&s))
                {
                    system("cls");
                    TraverseSparse(&s, &Display);
                }
                else
                    printf("Sparse is empty.\n");
                break;
            case 5:
                if(!SparseEmpty(&s))
                {
                    printf("Input column number : ");
                    scanf("%d", &col);
                    fflush(stdin);

                    system("cls");
                    if(TraverseColumn(&s, col, &Display) == -1)
                        printf("Column doesn't exist.\n");
                }
                else
                    printf("Sparse is empty.\n");
                break;
            case 6:
                if(!SparseEmpty(&s))
                {
                    printf("Input row number : ");
                    scanf("%d", &row);
                    fflush(stdin);

                    system("cls");
                    if(TraverseRow(&s, row, &Display) == -1)
                        printf("Row doesn't exist.\n");
                }
                else
                    printf("Sparse is empty.\n");
                break;
            case 7:
                if(ClearSparse(&s) ==1 )
                    printf("Sparse has been cleared successfully.\n");
                else
                    printf ("Sparse is already empty.\n");
                break;
            case 8:
                printf("Size is : %d\n", SparseSize(&s));
                break;
            case 9:
                checker++;
                break;
        }
        printf ("\nPress any key to continue.\n");
        getch();
        system("cls");
    }
    printf ("\n\nProgram has been terminated\n\n======================================\n");
    return 0;
}
