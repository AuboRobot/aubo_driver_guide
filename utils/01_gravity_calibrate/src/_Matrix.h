#ifndef _MATRIX_H
#define _MATRIX_H

//头文件
#include <stdio.h>
#include <stdlib.h>
//#include "stdafx.h"

//矩阵数据结构
//二维矩阵
struct _Matrix
{

	int m;
	int n;
	double *arr;
};

		//初始化
    //_Matrix(int mm = 0, int nn = 0);
	//设置m
	void Matrix_set_m(struct _Matrix *m,int mm);
	//设置n
	void Matrix_set_n(struct _Matrix *n,int nn);
	//初始化
	void Matrix_init_matrix(struct _Matrix *m);
	//释放
	void Matrix_free_matrix(struct _Matrix *m);
	//读取i,j坐标的数据
	//失败返回-31415,成功返回值
	double Matrix_read(struct _Matrix *m,int i, int j);
	//写入i,j坐标的数据
	//失败返回-1,成功返回1
	int Matrix_write(struct _Matrix *m,int i, int j, double val);

	//_Matrix_Calc();
	//C = A + B
	//成功返回1,失败返回-1
	int Matrix_add(struct _Matrix *A, struct _Matrix *B, struct _Matrix *C);
	//C = A - B
	//成功返回1,失败返回-1
	int Matrix_subtract(struct _Matrix *A, struct _Matrix *B, struct _Matrix *C);
	//C = A * B
	//成功返回1,失败返回-1
	int Matrix_multiply(struct _Matrix *A, struct _Matrix *B, struct _Matrix *C);

	int Matrix_Cross(struct _Matrix *A, struct _Matrix *B, struct _Matrix *C);
	//行列式的值,只能计算2 * 2,3 * 3
	//失败返回-31415,成功返回值
	double Matrix_det(struct _Matrix *A);
	//求转置矩阵,B = AT
	//成功返回1,失败返回-1
	int Matrix_transpos(struct _Matrix *A, struct _Matrix *B);
	//求逆矩阵,B = A^(-1)
	//成功返回1,失败返回-1
	int Matrix_inverse(struct _Matrix *A, struct _Matrix *B);

	void printff_matrix(struct _Matrix *A);
#endif

