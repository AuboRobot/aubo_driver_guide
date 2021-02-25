#ifndef _MATRIX_H
#define _MATRIX_H

//ͷ�ļ�
#include <stdio.h>
#include <stdlib.h>
//#include "stdafx.h"

//�������ݽṹ
//��ά����
struct _Matrix
{

	int m;
	int n;
	double *arr;
};

		//��ʼ��
    //_Matrix(int mm = 0, int nn = 0);
	//����m
	void Matrix_set_m(struct _Matrix *m,int mm);
	//����n
	void Matrix_set_n(struct _Matrix *n,int nn);
	//��ʼ��
	void Matrix_init_matrix(struct _Matrix *m);
	//�ͷ�
	void Matrix_free_matrix(struct _Matrix *m);
	//��ȡi,j���������
	//ʧ�ܷ���-31415,�ɹ�����ֵ
	double Matrix_read(struct _Matrix *m,int i, int j);
	//д��i,j���������
	//ʧ�ܷ���-1,�ɹ�����1
	int Matrix_write(struct _Matrix *m,int i, int j, double val);

	//_Matrix_Calc();
	//C = A + B
	//�ɹ�����1,ʧ�ܷ���-1
	int Matrix_add(struct _Matrix *A, struct _Matrix *B, struct _Matrix *C);
	//C = A - B
	//�ɹ�����1,ʧ�ܷ���-1
	int Matrix_subtract(struct _Matrix *A, struct _Matrix *B, struct _Matrix *C);
	//C = A * B
	//�ɹ�����1,ʧ�ܷ���-1
	int Matrix_multiply(struct _Matrix *A, struct _Matrix *B, struct _Matrix *C);

	int Matrix_Cross(struct _Matrix *A, struct _Matrix *B, struct _Matrix *C);
	//����ʽ��ֵ,ֻ�ܼ���2 * 2,3 * 3
	//ʧ�ܷ���-31415,�ɹ�����ֵ
	double Matrix_det(struct _Matrix *A);
	//��ת�þ���,B = AT
	//�ɹ�����1,ʧ�ܷ���-1
	int Matrix_transpos(struct _Matrix *A, struct _Matrix *B);
	//�������,B = A^(-1)
	//�ɹ�����1,ʧ�ܷ���-1
	int Matrix_inverse(struct _Matrix *A, struct _Matrix *B);

	void printff_matrix(struct _Matrix *A);
#endif

