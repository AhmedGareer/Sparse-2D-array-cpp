#ifndef SPARSE_H_INCLUDED
#define SPARSE_H_INCLUDED

typedef char SparseEntry;
typedef struct sparsenode
{
    SparseEntry entry;
    int row;
    int col;
    struct sparsenode *right;
    struct sparsenode *down;
}Node;

typedef struct row{
    int rowno;
    Node *right;
    struct row *down;
}Row;

typedef struct column{
    int colno;
    Node *down;
    struct column *right;
}Column;

typedef struct sparse{
    Row *down;
    Column *right;
    int size;
}Sparse;

void CreateSparse(Sparse *pl);
int InsertNode(Sparse *pl, SparseEntry *e, int enteredrows, int enteredcolumns);
int DeleteNode(Sparse *pl, SparseEntry *e, int enteredrows, int enteredcolumns);
int SparseEmpty(Sparse *pl);
int SparseSize(Sparse *pl);
int SparseFull(Sparse *pl);
int TraverseSparse(Sparse *pl, void(*pf)(int, int, SparseEntry));
int TraverseColumn(Sparse *pl, int colnumber, void(*pf)(int, int, SparseEntry));
int TraverseRow(Sparse *pl, int rownumber, void(*pf)(int, int, SparseEntry));
int ClearSparse(Sparse *pl);
int RetrieveNode(Sparse *pl, SparseEntry *e, int row, int col);

#endif // SPARSE_H_INCLUDED
