#include <stdio.h>
#include <stdlib.h>
#include "sparse.h"

void CreateSparse(Sparse *pl)
{
    pl->down = NULL;
    pl->right = NULL;
    pl->size = 0;
}
int InsertNode(Sparse *pl, SparseEntry *e, int enteredrows, int enteredcolumns)
{
    int cexist, rexist, inserted;
    Column *q = pl->right;
    Node *n;
    Node *new_node = (Node *)malloc(sizeof(Node));
    new_node->down = NULL;
    new_node->right = NULL;
    new_node->col = enteredcolumns;
    new_node->row = enteredrows;
    new_node->entry = *e;
    cexist = 0;
    rexist = 0;
    inserted = 0;
    ///Check if column exists.
    while(q!=NULL)
    {
        if(q->colno == enteredcolumns)
        {
            cexist = 1;
            break;
        }
        q=q->right;
    }
    ///Column exists, check if node with desired row already exists.
    if(cexist == 1)
    {
        n=q->down;
        while(n!=NULL)
        {
            if(n->row == enteredrows)
            {
                rexist = 1;
                break;
            }
            n=n->down;
        }
        if(cexist == 1 && rexist == 1)
        {
            free(new_node);
            return 0; ///Node already exists.
        }
        ///Column exists, inserting node.
        else
        {
            inserted = 0;
            n=q->down;
            ///Check first node.
            if(n->row > enteredrows)
            {
                new_node->down = q->down;
                q->down = new_node;
            }
            ///Check the rest of the nodes.
            else
            {
                n=q->down;
                while(n->down!=NULL)
                {
                    if(n->down->row > enteredrows)
                    {
                        if(inserted == 0)
                        {
                            new_node->down = n->down;
                            n->down = new_node;
                            inserted = 1;
                        }
                    }
                    n=n->down;
                }
                ///Node is biggest, insert at the end of the column.
                if(inserted == 0)
                {
                    new_node->down = n->down;
                    n->down = new_node;
                    inserted = 1;
                }
            }
        }
    }
    ///Column doesn't exist, insert column with node.
    else
    {
        inserted = 0;
        Column *new_column = (Column *)malloc(sizeof(Column));
        new_column->down = NULL;
        new_column->right = NULL;
        new_column->colno = enteredcolumns;
        ///First column insertion in the whole sparse.
        if(pl->right == NULL)
        {
            new_column->right = pl->right;
            pl->right = new_column;
            new_column->down = new_node;
        }
        ///Not first column, start comparing.
        else
        {
            q=pl->right;
            ///Check first column.
            if(q->colno > enteredcolumns)
            {
                new_column->right = pl->right;
                pl->right = new_column;
                new_column->down = new_node;
                inserted = 1;
            }
            ///Check the rest of the columns.
            else
            {
                while(q->right!=NULL)
                {
                    if(q->right->colno > enteredcolumns)
                    {
                        if(inserted == 0)
                        {
                            new_column->right = q->right;
                            q->right = new_column;
                            new_column->down = new_node;
                            inserted = 1;
                        }
                    }
                    q=q->right;
                }
                ///Column is biggest, insert at the end.
                if(inserted == 0)
                {
                    new_column->right = q->right;
                    q->right = new_column;
                    new_column->down = new_node;
                    inserted = 1;
                }
            }
        }
    }
    ///Check if row exists.
    rexist = 0;
    Row *r = pl->down;
    while(r!=NULL)
    {
        if(r->rowno == enteredrows)
        {
            rexist = 1;
            break;
        }
        r=r->down;
    }
    ///Row exists, inserting node.
    if(rexist == 1)
    {
        n=r->right;
        inserted = 0;
        ///Check first node.
        if(n->col > enteredcolumns)
        {
            new_node->right = r->right;
            r->right = new_node;
        }
        ///Check rest of the nodes.
        else
        {
            while(n->right!=NULL)
            {
                if(n->right->col > enteredcolumns)
                {
                    if(inserted == 0)
                    {
                        new_node->right = n->right;
                        n->right = new_node;
                        inserted = 1;
                    }
                }
                n = n->right;
            }
            ///Node is biggest, insert at the end of the row.
            if(inserted == 0)
            {
                new_node->right = n->right;
                n->right = new_node;
                inserted = 1;
            }
        }
    }
    ///Row doesn't exist, insert row with node.
    else
    {
        inserted = 0;
        Row *new_row = (Row *)malloc(sizeof(Row));
        new_row->right = NULL;
        new_row->down = NULL;
        new_row->rowno = enteredrows;
        ///First row insertion in the whole sparse.
        if(pl->down == NULL)
        {
            new_row->down = pl->down;
            pl->down = new_row;
            new_row->right = new_node;
        }
        ///Not first row, start comparing.
        else
        {
            r=pl->down;
            ///Check first row.
            if(r->rowno > enteredrows)
            {
                new_row->down = pl->down;
                pl->down = new_row;
                new_row->right = new_node;
                inserted = 1;
            }
            ///Check rest of the rows.
            else
            {
                while(r->down!=NULL)
                {
                    if(r->down->rowno > enteredrows)
                    {
                        if(inserted == 0)
                        {
                            new_row->down = r->down;
                            r->down = new_row;
                            new_row->right = new_node;
                            inserted = 1;
                        }
                    }
                    r=r->down;
                }
                ///Row is biggest, insert at the end.
                if(inserted == 0)
                {
                    new_row->down = r->down;
                    r->down = new_row;
                    new_row->right = new_node;
                    inserted = 1;
                }
            }
        }
    }
    pl->size++;
    return 1;
}


int DeleteNode(Sparse *pl, SparseEntry *e, int enteredrows, int enteredcolumns)
{
    int cexist = 0, rexist = 0;
    Column *q, *tmpq;
    Row *r, *tmpr;
    Node *n, *tmpn;
    ///Check if column exists.
    for(q=pl->right;q!=NULL;q=q->right)
    {
        if(q->colno == enteredcolumns)
        {
            cexist = 1;
            ///Column exists, check if a node with the desired row number exists.
            for(n=q->down;n!=NULL;n=n->down)
            {
                if(n->row == enteredrows)
                {
                    rexist = 1;
                    break;
                }
            }
            break;
        }
    }
    ///Node exists, start deleting.
    if(cexist == 1 && rexist == 1)
    {
        ///Search for the row.
        for(r=pl->down;r!=NULL;r=r->down)
        {
            if(r->rowno == enteredrows)
                break;
        }
        ///Check first node in the column.
        if(q->down->row == enteredrows)
        {
            tmpn=q->down;
            q->down=q->down->down;
        }
        ///Check the rest of the nodes in the same column.
        else
        {
            for(n=q->down;n->down!=NULL;n=n->down)
            {
                if(n->down->row == enteredrows)
                {
                    tmpn = n->down;
                    n->down = n->down->down;
                    break;
                }
            }
        }
        ///Check first node in the row.
        if(r->right->col == enteredcolumns)
        {
            tmpn=r->right;
            r->right=r->right->right;
        }
        ///Check the rest of the nodes in the same row.
        else
        {
            for(n=r->right;n->right!=NULL;n=n->right)
            {
                if(n->right->col == enteredcolumns)
                {
                    tmpn = n->right;
                    n->right = n->right->right;
                    break;
                }
            }
        }
        ///Column is NULL after deletion.
        if(q->down == NULL)
        {
            ///Check if NULL column is the first column.
            if(pl->right->down == NULL)
            {
                tmpq = pl->right;
                pl->right = pl->right->right;
                free(tmpq);
            }
            ///Check if NULL column is in the rest of the columns.
            else
            {
                for(q=pl->right;q->right!=NULL;q=q->right)
                {
                    if(q->right->down == NULL)
                    {
                        tmpq = q->right;
                        q->right = q->right->right;
                        free(tmpq);
                        break;
                    }
                }
            }
        }
        ///Row is NULL after deletion.
        if(r->right == NULL)
        {
            {
                ///Check if NULL row is the first row.
                if(pl->down->right == NULL)
                {
                    tmpr = pl->down;
                    pl->down = pl->down->down;
                    free(tmpr);
                }
                ///Check if NULL row is in the rest of the rows.
                else
                {
                    for(r=pl->down;r->down!=NULL;r=r->down)
                    {
                        if(r->down->right == NULL)
                        {
                            tmpr = r->down;
                            r->down = r->down->down;
                            free(tmpr);
                            break;
                        }
                    }
                }
            }
        }
        *e = tmpn->entry;
        free(tmpn);
        pl->size--;
        return 1; //Node deleted.
    }
    else
        return 0; //Node doesn't exist.
}

int SparseEmpty(Sparse *pl)
{
    return(pl->down==NULL && pl->right==NULL);
}

int SparseSize(Sparse *pl)
{
    return pl->size;
}

int SparseFull(Sparse *pl)
{
    return 0;
}

int TraverseSparse(Sparse *pl, void(*pf)(int, int, SparseEntry))
{
    Row *r = pl->down;
    Node *n;
    if(pl->down == NULL)
        return 0;
    while(r!=NULL)
    {
        n=r->right;
        while(n!=NULL)
        {
            (*pf)(n->row, n->col, n->entry);
            n=n->right;
        }
        r=r->down;
    }
    return 1;
}

int TraverseColumn(Sparse *pl, int colnumber, void(*pf)(int, int, SparseEntry))
{
    Column *q;
    Node *n;
    if(pl->right == NULL)
        return 0; ///Sparse is empty.
    else
    {
        q = pl->right;
        while(q!=NULL)
        {
            if(q->colno == colnumber)
            {
                n = q->down;
                while(n!=NULL)
                {
                    (*pf)(n->row, n->col, n->entry);
                    n=n->down;
                }
                return 1; ///Column was traversed successfully.
            }
            q = q->right;
        }
        return -1; ///Column doesn't exist.
    }
}

int TraverseRow(Sparse *pl, int rownumber, void(*pf)(int, int, SparseEntry))
{
    Row *r;
    Node *n;
    if(pl->down == NULL)
        return 0; ///Sparse is empty.
    else
    {
        r = pl->down;
        while(r!=NULL)
        {
            if(r->rowno == rownumber)
            {
                n = r->right;
                while(n!=NULL)
                {
                    (*pf)(n->row, n->col, n->entry);
                    n=n->right;
                }
                return 1; ///Row was traversed successfully.
            }
            r = r->down;
        }
        return -1; ///Row doesn't exist.
    }
}

int ClearSparse(Sparse *pl)
{
    Row *r; //Row *r = pl->down;
    Column *q;
    Node *n;
    ///Sparse is already empty.
    if(pl->down == NULL)
        return 0;
    ///Remove rows and nodes.
    while(pl->down != NULL) //while(r != NULL)
    {
        while(pl->down->right != NULL) //while(r->right != NULL)
        {
            n=pl->down->right; //n = r->right;
            pl->down->right=n->right; //r->right = n->right;
            free(n);
            pl->size--;
        }
        r=pl->down;
        pl->down=r->down; //pl->down = pl->down->down;
        free(r);
    }
    ///Removing columns.
    while(pl->right != NULL)
    {
        q=pl->right;
        pl->right=q->right; //pl->right = pl->right->right;
        free(q);
    }
    return 1;
}

int ClearSparse2(Sparse *pl)
{
    Row *r;
    Column *q;
    Node *n;
    if(pl->down == NULL)
        return 0;
    else
    {
        r=pl->down;
        while(r!=NULL)
        {
            while(r->right!=NULL)
            {
                n = r->right->right;
                free(r->right);
                r->right = n;
                pl->size--;
            }
            r=r->down;
        }
        while(pl->right!=NULL)
        {
            q = pl->right->right;
            free(pl->right);
            pl->right=q;
        }
        while(pl->down!=NULL)
        {
            r = pl->down->down;
            free(pl->down);
            pl->down = r;
        }
        return 1;
    }
}

int RetrieveNode(Sparse *pl, SparseEntry *e, int row, int col)
{
    Row *r=pl->down;
    while(r != NULL)
    {
        if(r->rowno==row)
        {
            Node *n=r->right;
            while(n != NULL)
            {
                if (n->col==col)
                {
                    *e=n->entry;
                    return 1;
                }
                n=n->right;
            }
            return 0;
        }
        r=r->down;
    }
    return 0;
}

/*
int CheckIfExist(Sparse *pl, int enteredrows, int enteredcolumns)
{
    int cexist, rexist;
    Column *q = pl->right;
    Node *n;
    cexist = 0;
    rexist = 0;
    ///Checking if column exist.
    while(q!=NULL)
    {
        if(q->colno == enteredcolumns)
        {
            cexist = 1; ///Column does exist.
            break;
        }
        q=q->right;
    }
    if(cexist == 1) ///Checking if item with similar row already exist.
    {
        n=q->down;
        while(n!=NULL)
        {
            if(n->row == enteredrows)
            {
                rexist = 1;
                break;
            }
            n=n->down;
        }
        if(cexist == 1 && rexist == 1) ///User inputs item in a position that is already occupied.
        {
            printf("Row : %d Column : %d Item : %c", n->row, n->col, n->entry);
            return 1; ///Retrun true. Item exists.
        }
        else
            return 0; ///Return false. Item doesn't exist.
    }
    else
        return 0;
}

int DeleteColumn(Sparse *pl, int enteredcolumns)
{
    int cexist = 0;
    Column *q = pl->right;
    while(q!=NULL)
    {
        if(q->colno == enteredcolumns)
        {
            cexist = 1;
            break;
        }
        q=q->right;
    }
    if(cexist == 1)
    {
        Row *r = pl->down;
        Node *n, *tmpn;
        while(r!=NULL)
        {
            n = r->right;
            if(n->col == enteredcolumns)
            {
                r->right = r->right->right;
            }
            else
            {
                n = r->right;
                while(n->right!=NULL)
                {
                    if(n->right->col == enteredcolumns)
                    {
                        n->right = n->right->right;
                        break;
                    }
                    n=n->right;
                }
            }
            r=r->down;
        }
        while(q->down!=NULL)
        {
            tmpn = q->down->down;
            free(q->down);
            q->down = tmpn;
            pl->size--;
        }
        q = pl->right;
        if(q->down == NULL)
        {
            q = pl->right;
            pl->right = pl->right->right;
            free(q);
        }
        else
        {
            Column *tmpq;
            q = pl->right;
            while(q->right!=NULL)
            {
                if(q->right->down == NULL)
                {
                    tmpq = q->right;
                    q->right = q->right->right;
                    free(tmpq);
                    break;
                }
                q=q->right;
            }
        }
        Row *tmpr;
        while(pl->down!=NULL)
        {
            if(pl->down->right == NULL)
            {
                tmpr = pl->down;
                pl->down = pl->down->down;
                free(tmpr);
            }
            else
                break;
        }
        if(pl->down!=NULL)
        {
            r = pl->down;
            while(r->down!=NULL)
            {
                if(r->down->right == NULL)
                {
                    tmpr = r->down;
                    r->down = r->down->down;
                    free(tmpr);
                }
                else
                    r=r->down;
                if(r->down == NULL) ///only for precise checking, not really necessary.
                    break;
            }
        }
        return 1;
    }
    else
        return 0;
}

int DeleteRow(Sparse *pl, int enteredrows)
{
    int rexist = 0;
    Row *r = pl->down;
    while(r!=NULL)
    {
        if(r->rowno == enteredrows)
        {
            rexist = 1;
            break;
        }
        r=r->down;
    }
    if(rexist == 1)
    {
        Column *q = pl->right;
        Node *n, *tmpn;
        while(q!=NULL)
        {
            n = q->down;
            if(n->row == enteredrows)
            {
                q->down = q->down->down;
            }
            else
            {
                n = q->down;
                while(n->down!=NULL)
                {
                    if(n->down->row == enteredrows)
                    {
                        n->down = n->down->down;
                        break;
                    }
                    n=n->down;
                }
            }
            q=q->right;
        }
        while(r->right!=NULL)
        {
            tmpn = r->right->right;
            free(r->right);
            r->right = tmpn;
            pl->size--;
        }
        r = pl->down;
        if(r->right == NULL)
        {
            r = pl->down;
            pl->down = pl->down->down;
            free(r);
        }
        else
        {
            Row *tmpr;
            r = pl->down;
            while(r->down!=NULL)
            {
                if(r->down->right == NULL)
                {
                    tmpr = r->down;
                    r->down = r->down->down;
                    free(tmpr);
                    break;
                }
                r=r->down;
            }
        }
        Column *tmpq;
        while(pl->right!=NULL)
        {
            if(pl->right->down == NULL)
            {
                tmpq = pl->right;
                pl->right = pl->right->right;
                free(tmpq);
            }
            else
                break;
        }
        if(pl->right!=NULL)
        {
            q = pl->right;
            while(q->right!=NULL)
            {
                if(q->right->down == NULL)
                {
                    tmpq = q->right;
                    q->right = q->right->right;
                    free(tmpq);
                }
                else
                    q=q->right;
                if(q->right == NULL) ///only for precise checking, not really necessary.
                    break;
            }
        }
        return 1;
    }
    else
        return 0;
}
*/
