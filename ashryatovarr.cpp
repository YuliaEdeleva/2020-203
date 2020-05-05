﻿#include "ashryatovarr.h"

/**
 * Введение в дисциплину
 */
void ashryatovarr::lab1()
{
    cout << "hello world!" << endl;
}


/**
 * Метод Гаусса с выбором главного элемента
 */
void ashryatovarr::lab2()
{

    for (int k = 0; k < N; k++)
    {
        int idmax = 0;
        for (int i=0; i<N; i++)
        {
            if(abs(A[i][k]) > abs(A[idmax][k]))
                idmax = i;
        }

        for (int i = 0; i < N; i++)
        {
            swap(A[idmax][i], A[k][i]);
        }
        swap(b[idmax], b[k]);

        for (int i = k + 1; i < N; i++)
        {
            double tmp = A[i][k]/A[k][k];
            for (int j = k; j < N; j++)
            {
                A[i][j] -= A[k][j]*tmp;
            }
            b[i] -= b[k]*tmp;
        }
    }

    for(int i = 0; i<N; i++)
    {
        x[i]=b[i];
    }

    for (int i = N-1; i >= 0; i--)
    {
        for (int j = i+1; j < N; j++)
        {
            x[i] -= A[i][j]*x[j];
        }
        x[i] /= A[i][i];
    }

}



/**
 * Метод прогонки
 */
void ashryatovarr::lab3()
{
    double *alfa = new double[N];
    double *beta = new double[N];
    alfa[0]=A[0][1]/-A[0][0];
	beta[0]=b[0]/A[0][0];
	for( int i=1; i<N; i++){
		alfa[i]=A[i][i+1]/(-A[i][i]-A[i][i-1]*alfa[i-1]);
		beta[i]=(A[i][i-1]*beta[i-1]-b[i])/(-A[i][i]-A[i][i-1]*alfa[i-1]);
}

  x[N-1]=beta[N-1];
  for (int i=N-2; i>=0; i--){
	  x[i]=alfa[i]*x[i+1]+beta[i];
  }

}



/**
 * Метод Холецкого
 */
void ashryatovarr::lab4()
{
    double **S = new double*[N];
    for (int i=0; i<N; i++)
    {
        S[i]=new double[N];
        for(int j=0; j<N; j++)
            S[i][j]=0;
    }
    double *D = new double[N];
    if (A[0][0]>0)
        D[0]=1;
    else
        D[0]=-1;
    S[0][0]=sqrt(fabs(A[0][0]));

    for (int i=1; i<N; i++)
    {
        S[0][i]=A[0][i]/(D[0]*S[0][0]);
    }

    for (int i=1; i<N; i++)
    {
        double temp =0;
        for (int j=0; j<i; j++)
            temp+=D[j]*S[j][i]*S[j][i];
        if (A[i][i]-temp>=0)
            D[i]=1;
        else
            D[i]=-1;
        S[i][i]=sqrt(D[i]*(A[i][i]-temp));

        for (int j=i+1; j<N; j++)
        {
            double l = 0;
            for (int k=0; k<j; k++)
                l+=D[k]*S[k][i]*S[k][j];

            S[i][j]=(A[i][j]-l)/(D[i]*S[i][i]);
        }
    }
    double *y = new double[N];
    y[0]=b[0]/S[0][0];
    for (int i=1; i<N; i++)
    {
        double temp = 0;
        for (int j=0; j<i; j++)
            temp+=y[j]*S[j][i];
        y[i]=(b[i]-temp)/S[i][i];
    }
    x[N-1]=y[N-1]/(D[N-1]*S[N-1][N-1]);

    for (int i=N-2; i>=0; i--)
    {
        double temp =0;
        for (int j=i+1; j<N; j++)
            temp+=x[j]*D[j]*S[i][j];      
        x[i]=(y[i]-temp)/(D[i]*S[i][i]);
    }

}



/**
 * Метод Якоби или Зейделя
 */
void ashryatovarr::lab5()
{
    int n = N;
    double eps = 1e-69;
    double norma;
    double *y = new double[n];
    do{
        for(int i = 0; i<n; i++)
            y[i]=x[i];
        norma = 0;
        for(int i = 0; i<n; i++)
        {
            double sum1 = 0, sum2 = 0;
            for(int j = i + 1; j < n; j++)
                sum1 += A[i][j]*x[j];
            for(int j = i-1; j>= 0; j--)
                sum2 += A[i][j]*x[j];
            x[i] = (b[i] - sum1 - sum2)/A[i][i];
        }

        for (int i = 0; i < n; i++)
            norma += abs(x[i] - y[i]);
    } while (norma>eps);

}



/**
 * Метод минимальных невязок
 */
void ashryatovarr::lab6()
{
	int n = N;
	double* F = new double[n];
	double* r = new double[n];
	double* alfa = new double[n];
	double a, norma, k = 0;
	double eps = 1e-19;
	do {
		for (int i = 0; i < n; i++) {
			double tmp = 0;
			for (int j = 0; j < n; j++)
				tmp += A[i][j] * x[j];
			r[i] = tmp - b[i];
			F[i] = 2 * r[i];
		}
		double* Ar = new double[n];
		for (int i = 0; i < n; i++) {
			double tmps = 0;
			for (int j = 0; j < n; j++)
				tmps += A[i][j] * r[j];
			Ar[i] = tmps;
		}
		double ts1 = 0, ts2 = 0;
		for (int i = 0; i < n; i++) {
			ts1 += abs(Ar[i] * r[i]);
			ts2 += abs(Ar[i] * Ar[i]);
		}
		a = ts1 / (2 * ts2);

		double*y = new double[n];
		for (int i = 0; i < n; i++)
			y[i] = x[i];
		for (int i = 0; i < n; i++)
			x[i] = x[i] - a * F[i];

		norma = 0;
		for (int i = 0; i < n; i++)
			norma += (y[i] - x[i])*(y[i] - x[i]);

	}while(sqrt(norma)>eps);

}



/**
 * Метод сопряженных градиентов
 */
void ashryatovarr::lab7()
{

}


void ashryatovarr::lab8()
{

}


void ashryatovarr::lab9()
{

}


std::string ashryatovarr::get_name()
{
    return "Ashryatova R.R.";
}
