#include<stdio.h>
#include<stdlib.h>
#include<stdbool.h>
#define MAXSIZE 128

struct Triple
{
	int row;
	int col;
	int data;
};

struct TSMatrix
{
	struct Triple Elem[MAXSIZE];
	int position[MAXSIZE];
	int length_row, length_col, length_Elem;
};

struct TSMatrix* MatrixInit()
{
	struct TSMatrix* TsMatrix = (struct TSMatrix*)malloc(sizeof(struct TSMatrix));
	printf("Please enter the number of rows and columns of the matrix.\n");
	printf("row: ");
	scanf("%d", &TsMatrix->length_row);
	printf("col: ");
	scanf("%d", &TsMatrix->length_col);
	TsMatrix->length_Elem = 0;
	printf("\n");
	int data_in, count = 0;
	for (int i = 0; i < TsMatrix->length_row; i++)
	{
		printf("row %d: ", i + 1);
		TsMatrix->position[count++] = TsMatrix->length_Elem;
		for (int j = 0; j < TsMatrix->length_col; j++)
		{
			scanf("%d", &data_in);
			if (data_in)
			{
				TsMatrix->Elem[TsMatrix->length_Elem].row = i;
				TsMatrix->Elem[TsMatrix->length_Elem].col = j;
				TsMatrix->Elem[TsMatrix->length_Elem].data = data_in;
				TsMatrix->length_Elem++;
			}
		}
	}
	printf("\n");
	return TsMatrix;
}

int** matrixInit(struct TSMatrix* TsMatrix)
{
	int** matrix = (int**)malloc(sizeof(int) * TsMatrix->length_row);
	for (int i = 0; i < TsMatrix->length_row; i++)
	{
		matrix[i] = (int*)malloc(sizeof(int) * TsMatrix->length_col);
		for (int j = 0; j < TsMatrix->length_col; j++)
			matrix[i][j] = 0;
	}
	return matrix;
}

struct TsMatrix* Copy(struct TSMatrix* TsMatrix, int** matrix)
{
	for (int i = 0; i < TsMatrix->length_row; i++)
	{
		for (int j = 0; j < TsMatrix->length_col; j++)
		{
			if (matrix[i][j])
			{
				TsMatrix->Elem[TsMatrix->length_Elem].row = i;
				TsMatrix->Elem[TsMatrix->length_Elem].col = j;
				TsMatrix->Elem[TsMatrix->length_Elem].data = matrix[i][j];
				TsMatrix->length_Elem++;
			}	
		}
	}
	return TsMatrix;
}

void print(struct TSMatrix* TsMatrix)
{
	int count = 0;
	for (int i = 0; i < TsMatrix->length_row; i++)
	{
		for (int j = 0; j < TsMatrix->length_col; j++)
		{
			if (TsMatrix->Elem[count].row == i && TsMatrix->Elem[count].col == j)
			{
				printf("%4d", TsMatrix->Elem[count].data);
				count++;
			}
			else printf("%4d", 0);
		}
		printf("\n");
	}
	count = 0;
	printf("©°©¤©¤©¤©¤©¤©¤©¤©¤©Ð©¤©¤©¤©¤©¤©¤©¤©¤©Ð©¤©¤©¤©¤©¤©¤©¤©¤©´\n");
	printf("©¦   row  ©¦   col  ©¦  data  ©¦\n");
	for (int i = 0; i < TsMatrix->length_row; i++)
	{
		for (int j = 0; j < TsMatrix->length_col; j++)
			if (TsMatrix->Elem[count].row == i && TsMatrix->Elem[count].col == j)
			{
				printf("©À©¤©¤©¤©¤©¤©¤©¤©¤©à©¤©¤©¤©¤©¤©¤©¤©¤©à©¤©¤©¤©¤©¤©¤©¤©¤©È\n");
				printf("©¦  %3d   ©¦  %3d   ©¦  %3d   ©¦\n", TsMatrix->Elem[count].row + 1, TsMatrix->Elem[count].col + 1, TsMatrix->Elem[count].data);
				count++;
			}
	}
	printf("©¸©¤©¤©¤©¤©¤©¤©¤©¤©Ø©¤©¤©¤©¤©¤©¤©¤©¤©Ø©¤©¤©¤©¤©¤©¤©¤©¤©¼\n");
}

struct TsMatrix* AddMatrix(struct TSMatrix* A, struct TSMatrix* B)
{
	if (A->length_row != B->length_row || A->length_col != B->length_col)return NULL;
	struct TSMatrix* C = (struct TSMatrix*)malloc(sizeof(struct TSMatrix));
	C->length_row = A->length_row;
	C->length_col = A->length_col;
	C->length_Elem = 0;
	int** matrix = matrixInit(C);
	int row, col;
	for (int i = 0; i < A->length_Elem; i++)
	{
		row = A->Elem[i].row;
		col = A->Elem[i].col;
		matrix[row][col] = matrix[row][col] + A->Elem[i].data;
	}
	for (int i = 0; i < B->length_Elem; i++)
	{
		row = B->Elem[i].row;
		col = B->Elem[i].col;
		matrix[row][col] = matrix[row][col] + B->Elem[i].data;
	}
	return Copy(C, matrix);
}

struct TsMatrix* SubMatrix(struct TSMatrix* A, struct TSMatrix* B)
{
	if (A->length_row != B->length_row || A->length_col != B->length_col)return NULL;
	struct TSMatrix* C = (struct TSMatrix*)malloc(sizeof(struct TSMatrix));
	C->length_row = A->length_row;
	C->length_col = A->length_col;
	C->length_Elem = 0;
	int** matrix = matrixInit(C);
	int row, col;
	for (int i = 0; i < A->length_Elem; i++)
	{
		row = A->Elem[i].row;
		col = A->Elem[i].col;
		matrix[row][col] = matrix[row][col] + A->Elem[i].data;
	}
	for (int i = 0; i < B->length_Elem; i++)
	{
		row = B->Elem[i].row;
		col = B->Elem[i].col;
		matrix[row][col] = matrix[row][col] - B->Elem[i].data;
	}
	return Copy(C, matrix);
}

struct TSMatrix* MultMatrix(struct TSMatrix* A, struct TSMatrix* B)
{
	if (A->length_col != B->length_row)return NULL;
	struct TSMatrix* C = (struct TSMatrix*)malloc(sizeof(struct TSMatrix));
	C->length_row = A->length_row;
	C->length_col = B->length_col;
	C->length_Elem = 0;
	if (A->length_Elem * B->length_Elem != 0)
	{
		for (int A_row = 0; A_row < A->length_row; A_row++)
		{
			int* sum = (int*)malloc(sizeof(int) * C->length_col);
			for (int C_col = 0; C_col < C->length_col; C_col++)
				sum[C_col] = 0;
			C->position[A_row] = C->length_Elem;
			int end_A;
			if (A_row + 1 < A->length_row)end_A = A->position[A_row + 1];
			else end_A = A->length_Elem;
			for (int p = A->position[A_row]; p < end_A; p++)
			{
				int B_row = A->Elem[p].col;
				int end_B;
				if (B_row + 1 < B->length_row)end_B = B->position[B_row + 1];
				else end_B = B->length_Elem;
				for (int q = B->position[B_row]; q < end_B; q++)
				{
					int C_col = B->Elem[q].col;
					sum[C_col] += A->Elem[p].data * B->Elem[q].data;
				}
			}
			for (int C_col = 0; C_col < C->length_col; C_col++)
			{
				if (sum[C_col])
				{
					if (C->length_Elem > MAXSIZE) return NULL;
					C->Elem[C->length_Elem].row = A_row;
					C->Elem[C->length_Elem].col = C_col;
					C->Elem[C->length_Elem].data = sum[C_col];
					C->length_Elem++;
				}
			}
		}
	}
	return C;
}

struct TSMatrix* FastTransMatrix(struct TSMatrix* A)
{
	int number[MAXSIZE];
	int position[MAXSIZE];
	struct TSMatrix* B = (struct TSMatrix*)malloc(sizeof(struct TSMatrix));
	B->length_row = A->length_col;
	B->length_col = A->length_row;
	B->length_Elem = A->length_Elem;
	if (B->length_Elem)
	{
		for (int j = 0; j < A->length_col; j++)
			number[j] = 0;
		for (int t = 0; t < A->length_Elem; t++)
			number[A->Elem[t].col]++;
		position[0] = 0;
		for (int j = 1; j < A->length_col; j++)
			position[j] = position[j - 1] + number[j - 1];
		for (int p = 0; p < A->length_Elem; p++)
		{
			int j = A->Elem[p].col;
			int q = position[j];
			B->Elem[q].row = A->Elem[p].col;
			B->Elem[q].col = A->Elem[p].row;
			B->Elem[q].data = A->Elem[p].data;
			position[j]++;
		}
	}
	return B;
}

void Involution()
{
	struct TSMatrix* TsMatrix = MatrixInit();
	TsMatrix = MultMatrix(TsMatrix, TsMatrix);
	if (TsMatrix != NULL)
	{
		printf("The result is: \n");
		print(TsMatrix);
	}
	else printf("The matrix cannot be Involuted!\n");
}

void Addition()
{
	struct TSMatrix* TsMatrix_A = MatrixInit();
	struct TSMatrix* TsMatrix_B = MatrixInit();
	struct TSMatrix* TsMatrix_C = AddMatrix(TsMatrix_A, TsMatrix_B);
	if (TsMatrix_C != NULL)
	{
		printf("The result is: \n");
		print(TsMatrix_C);
	}
	else printf("These two matrices cannot be added!\n");
}

void Subtraction()
{
	struct TSMatrix* TsMatrix_A = MatrixInit();
	struct TSMatrix* TsMatrix_B = MatrixInit();
	struct TSMatrix* TsMatrix_C = SubMatrix(TsMatrix_A, TsMatrix_B);
	if (TsMatrix_C != NULL)
	{
		printf("The result is: \n");
		print(TsMatrix_C);
	}
	else printf("These two matrices cannot be subtracted!\n");
}

void Multiplication()
{
	struct TSMatrix* TsMatrix_A = MatrixInit();
	struct TSMatrix* TsMatrix_B = MatrixInit();
	struct TSMatrix* TsMatrix_C = MultMatrix(TsMatrix_A, TsMatrix_B);
	if (TsMatrix_C != NULL)
	{
		printf("The result is: \n");
		print(TsMatrix_C);
	}
	else printf("These two matrices cannot be multiplied!\n");
}

void Transpose()
{
	struct TSMatrix* TsMatrix = MatrixInit();
	printf("Before transpose: \n");
	print(TsMatrix);
	TsMatrix = FastTransMatrix(TsMatrix);
	printf("After transpose: \n");
	print(TsMatrix);
}

void menu()
{
	char ch;
	while (true)
	{
		printf("-----functions-----\n");
		printf("[1]Involution\n[2]Addition\n[3]Subtraction\n[4]Multiplication\n[5]Transpose\n[0]Exit\n");
		printf("-------------------\n");
		printf("Please select a function: ");
		switch (ch = getchar())
		{
		case'1': system("cls"); Involution(); break;
		case'2': system("cls"); Addition(); break;
		case'3': system("cls"); Subtraction(); break;
		case'4': system("cls"); Multiplication(); break;
		case'5': system("cls"); Transpose(); break;
		case'0': return;
		default: printf("Wrong input!\n");
		}
		while (getchar() != '\n');
		system("pause");
		system("cls");
	}
}

int main()
{
	menu();
	return 0;
}