#include "header.hpp"
using namespace std;

void fill_matrix(double *matrix_msr, int *vector_msr, point *x, Triangle* T, int n)
{
    int num_ver = number_of_vertices(n);
    int trapeze, n_i;
    int counter = 0;
    
    // Заполнение диагонали
    for(int i = 0; i < 3; i++)
    {   
        int first = 2*i;
        int second = 2*i + 1;
        trapeze = i*n*(n + 1);

        double first_integral_direct_1 = T[first].integral(1, 0, 0, 1, 0, 0, x[trapeze], x[trapeze + n + 1], x[trapeze + 1]);
        double first_integral_direct_2 = T[first].integral(0, 1, 0, 0, 1, 0, x[trapeze], x[trapeze + n + 1], x[trapeze + 1]);
        double first_integral_direct_3 = T[first].integral(0, 0, 1, 0, 0, 1, x[trapeze], x[trapeze + n + 1], x[trapeze + 1]);
        double second_integral_direct_1 = T[second].integral(1, 0, 0, 1, 0, 0, x[trapeze + n], x[trapeze + 2*n], x[trapeze + 2*n + 1]);
        double second_integral_direct_2 = T[second].integral(0, 1, 0, 0, 1, 0, x[trapeze + n], x[trapeze + 2*n], x[trapeze + 2*n + 1]);
        double second_integral_direct_3 = T[second].integral(0, 0, 1, 0, 0, 1, x[trapeze + n], x[trapeze + 2*n], x[trapeze + 2*n + 1]);
        double first_integral_reverse_1;
        double first_integral_reverse_2;
        double first_integral_reverse_3; 
        double second_integral_reverse_1;
        double second_integral_reverse_2;
        double second_integral_reverse_3;
        if(n > 1)
        {
            first_integral_reverse_1 = T[first].integral(1, 0, 0, 1, 0, 0, x[trapeze + n + 2], x[trapeze + 1], x[trapeze + n + 1]);
            first_integral_reverse_2 = T[first].integral(0, 1, 0, 0, 1, 0, x[trapeze + n + 2], x[trapeze + 1], x[trapeze + n + 1]);
            first_integral_reverse_3 = T[first].integral(0, 0, 1, 0, 0, 1, x[trapeze + n + 2], x[trapeze + 1], x[trapeze + n + 1]);
            second_integral_reverse_1 = T[second].integral(1, 0, 0, 1, 0, 0, x[trapeze + 3*n + 1], x[trapeze + 2*n + 1], x[trapeze + 2*n]);
            second_integral_reverse_2 = T[second].integral(0, 1, 0, 0, 1, 0, x[trapeze + 3*n + 1], x[trapeze + 2*n + 1], x[trapeze + 2*n]);
            second_integral_reverse_3 = T[second].integral(0, 0, 1, 0, 0, 1, x[trapeze + 3*n + 1], x[trapeze + 2*n + 1], x[trapeze + 2*n]);
        }

        matrix_msr[trapeze] += first_integral_direct_1;
        vector_msr[trapeze] = (i == 0)? num_ver + 2: vector_msr[trapeze - 1] + n_i;
        n_i = (i == 0)? 2: 4;
        for(int j = 1; j < n; j++)
        {
            int tj = trapeze + j;
            matrix_msr[tj] += first_integral_direct_3;
            matrix_msr[tj] += first_integral_reverse_2;
            matrix_msr[tj] += first_integral_direct_1;
            vector_msr[tj] = vector_msr[tj - 1] + n_i;
            n_i = ((i == 0)? 4: 6);
        }
        matrix_msr[trapeze + n] += first_integral_direct_3;
        matrix_msr[trapeze + n] += second_integral_direct_1;
        vector_msr[trapeze + n] = vector_msr[trapeze + n - 1] + n_i;
        n_i = ((i == 0)? 3: 4);
        for(int l = 1; l < n; l++)
        {
            int left = l*(n + 1);
            int lt = left + trapeze;
            matrix_msr[lt] += first_integral_direct_1;
            matrix_msr[lt] += first_integral_reverse_3;
            matrix_msr[lt] += first_integral_direct_2;
            vector_msr[lt] = vector_msr[lt - 1] + n_i;
            n_i = 4;
            for(int j = 1; j < n - l; j++)
            {
                int ltj = left + trapeze + j;
                matrix_msr[ltj] += first_integral_direct_1;
                matrix_msr[ltj] += first_integral_direct_2;
                matrix_msr[ltj] += first_integral_direct_3;
                matrix_msr[ltj] += first_integral_reverse_1;
                matrix_msr[ltj] += first_integral_reverse_2;
                matrix_msr[ltj] += first_integral_reverse_3;
                vector_msr[ltj] = vector_msr[ltj - 1] + n_i;
                n_i = 6;
            }
            matrix_msr[lt + n - l] += first_integral_direct_3;
            matrix_msr[lt + n - l] += first_integral_reverse_1;
            matrix_msr[lt + n - l] += first_integral_direct_2;
            matrix_msr[lt + n - l] += second_integral_direct_2;
            matrix_msr[lt + n - l] += second_integral_reverse_3;
            matrix_msr[lt + n - l] += second_integral_direct_1;
            vector_msr[lt + n - l] = vector_msr[lt + n - l - 1] + n_i;
            n_i = 6;
            for(int j = n - l + 1; j < n; j++)
            {
                int ltj = left + trapeze + j;
                matrix_msr[ltj] += second_integral_direct_1;
                matrix_msr[ltj] += second_integral_direct_2;
                matrix_msr[ltj] += second_integral_direct_3;
                matrix_msr[ltj] += second_integral_reverse_1;
                matrix_msr[ltj] += second_integral_reverse_2;
                matrix_msr[ltj] += second_integral_reverse_3;
                vector_msr[ltj] = vector_msr[ltj - 1] + n_i;
                n_i = 6;
            }
            matrix_msr[lt + n] += second_integral_direct_3;
            matrix_msr[lt + n] += second_integral_reverse_2;
            matrix_msr[lt + n] += second_integral_direct_1;
            vector_msr[lt + n] = vector_msr[lt + n - 1] + n_i;
            n_i = 4;
        }
        trapeze = (i + 1)*n*(n + 1);
        matrix_msr[trapeze] += first_integral_direct_2;
        matrix_msr[trapeze] += second_integral_direct_2;
        for(int j = 1; j < n; j++)
        {
            int tj = trapeze + j;
            matrix_msr[tj] += second_integral_direct_3;
            matrix_msr[tj] += second_integral_reverse_1;
            matrix_msr[tj] += second_integral_direct_2;
        }
        matrix_msr[trapeze + n] += second_integral_direct_3;
    }
    vector_msr[trapeze] = vector_msr[trapeze - 1] + n_i;
    n_i = 3;
    for(int i = 1; i < n; i++)
    {
        vector_msr[trapeze + i] = vector_msr[trapeze + i - 1] + n_i;
        n_i = 4;
    }
    vector_msr[trapeze + n] = vector_msr[trapeze + n - 1]  + n_i;
    n_i = 2;
    vector_msr[trapeze + n + 1] = vector_msr[trapeze + n]  + n_i;

    // Заполнение остальной матрицы
    double *matrix_msr_local = matrix_msr + num_ver + 1;
    int *vector_msr_local = vector_msr + num_ver + 1;

    for(int i = 0; i < 3; i++)
    {
        int first = 2*i;
        int second = 2*i + 1;
        trapeze = i*n*(n + 1);
        double first_integral_direct_1_2 = T[first].integral(1, 0, 0, 0, 1, 0, x[trapeze], x[trapeze + n + 1], x[trapeze + 1]);
        double first_integral_direct_1_3 = T[first].integral(1, 0, 0, 0, 0, 1, x[trapeze], x[trapeze + n + 1], x[trapeze + 1]);
        double first_integral_direct_2_3 = T[first].integral(0, 1, 0, 0, 0, 1, x[trapeze], x[trapeze + n + 1], x[trapeze + 1]);
        double second_integral_direct_1_2 = T[second].integral(1, 0, 0, 0, 1, 0, x[trapeze + n], x[trapeze + 2*n], x[trapeze + 2*n + 1]);
        double second_integral_direct_1_3 = T[second].integral(1, 0, 0, 0, 0, 1, x[trapeze + n], x[trapeze + 2*n], x[trapeze + 2*n + 1]);
        double second_integral_direct_2_3 = T[second].integral(0, 1, 0, 0, 0, 1, x[trapeze + n], x[trapeze + 2*n], x[trapeze + 2*n + 1]);
        double first_integral_reverse_1_2;
        double first_integral_reverse_1_3;
        double first_integral_reverse_2_3;
        double second_integral_reverse_1_2; 
        double second_integral_reverse_1_3;
        double second_integral_reverse_2_3; 
        if(n > 1)
        {
            first_integral_reverse_1_2 = T[first].integral(1, 0, 0, 0, 1, 0, x[trapeze + n + 2], x[trapeze + 1], x[trapeze + n + 1]);
            first_integral_reverse_1_3 = T[first].integral(1, 0, 0, 0, 0, 1, x[trapeze + n + 2], x[trapeze + 1], x[trapeze + n + 1]);
            first_integral_reverse_2_3 = T[first].integral(0, 1, 0, 0, 0, 1, x[trapeze + n + 2], x[trapeze + 1], x[trapeze + n + 1]);
            second_integral_reverse_1_2 = T[second].integral(1, 0, 0, 0, 1, 0, x[trapeze + 3*n + 1], x[trapeze + 2*n + 1], x[trapeze + 2*n]);
            second_integral_reverse_1_3 = T[second].integral(1, 0, 0, 0, 0, 1, x[trapeze + 3*n + 1], x[trapeze + 2*n + 1], x[trapeze + 2*n]);
            second_integral_reverse_2_3 = T[second].integral(0, 1, 0, 0, 0, 1, x[trapeze + 3*n + 1], x[trapeze + 2*n + 1], x[trapeze + 2*n]);
        }
        if(i == 0)
        {
            matrix_msr_local[counter] += first_integral_direct_1_3;
            vector_msr_local[counter] = 1;
            counter++;
            matrix_msr_local[counter] += first_integral_direct_1_2;
            vector_msr_local[counter] = n + 1;
            counter++;
            for(int j = 1; j < n; j++)
            {
                int tj = j;
                matrix_msr_local[counter] += first_integral_direct_1_3;
                vector_msr_local[counter] = tj - 1;
                counter++;
                matrix_msr_local[counter] += first_integral_direct_1_3;
                vector_msr_local[counter] = tj + 1;
                counter++;
                matrix_msr_local[counter] += first_integral_reverse_2_3;
                matrix_msr_local[counter] += first_integral_direct_2_3;
                vector_msr_local[counter] = tj + n;
                counter++;
                matrix_msr_local[counter] += first_integral_reverse_1_2;
                matrix_msr_local[counter] += first_integral_direct_1_2;
                vector_msr_local[counter] = tj + n + 1;
                counter++;
            }
            matrix_msr_local[counter] += first_integral_direct_1_3;
            vector_msr_local[counter] = trapeze + n - 1;
            counter++;
            matrix_msr_local[counter] += second_integral_direct_1_2;
            matrix_msr_local[counter] += first_integral_direct_2_3;
            vector_msr_local[counter] = trapeze + 2*n;
            counter++;
            matrix_msr_local[counter] += second_integral_direct_1_3;
            vector_msr_local[counter] = trapeze + 2*n + 1;
            counter++;
        }
        else
        {
            matrix_msr_local[counter] += first_integral_direct_1_3;
            vector_msr_local[counter] = trapeze + 1;
            counter++;
            matrix_msr_local[counter] += first_integral_direct_1_2;
            vector_msr_local[counter] = trapeze + n + 1;
            counter += 2;
            for(int j = 1; j < n; j++)
            {
                counter++;

                matrix_msr_local[counter] += first_integral_direct_1_3;
                counter++;

                matrix_msr_local[counter] += first_integral_direct_1_3;
                counter++; 

                matrix_msr_local[counter] += first_integral_direct_2_3;
                matrix_msr_local[counter] += first_integral_reverse_2_3;
                counter++;

                matrix_msr_local[counter] += first_integral_direct_1_2;
                matrix_msr_local[counter] += first_integral_reverse_1_2;
                counter += 2;
            }
            matrix_msr_local[counter] += first_integral_direct_1_3;
            vector_msr_local[counter] = trapeze + n - 1;
            counter++;
            matrix_msr_local[counter] += second_integral_direct_1_2;
            matrix_msr_local[counter] += first_integral_direct_2_3;
            vector_msr_local[counter] = trapeze + 2*n;
            counter++;
            matrix_msr_local[counter] += second_integral_direct_1_3;
            vector_msr_local[counter] = trapeze + 2*n + 1;
            counter++;
        }
        for(int l = 1; l < n; l++)
        {
            int left = l*(n + 1);
            int lt = left + trapeze;
            matrix_msr_local[counter] += first_integral_direct_1_2;
            vector_msr_local[counter] = lt - n - 1;
            counter++;
            matrix_msr_local[counter] += first_integral_reverse_2_3;
            matrix_msr_local[counter] += first_integral_direct_2_3;
            vector_msr_local[counter] = lt - n;
            counter++;
            matrix_msr_local[counter] += first_integral_direct_1_3;
            matrix_msr_local[counter] += first_integral_reverse_1_3;
            vector_msr_local[counter] = lt + 1;
            counter++;
            matrix_msr_local[counter] += first_integral_direct_1_2;
            vector_msr_local[counter] = lt + n + 1;
            counter++;
            for(int j = 1; j < n - l; j++)
            {
                int ltj = left + trapeze + j;
                matrix_msr_local[counter] += first_integral_direct_1_2;
                matrix_msr_local[counter] += first_integral_reverse_1_2;
                vector_msr_local[counter] = ltj - n - 1;
                counter++;
                matrix_msr_local[counter] += first_integral_direct_2_3;
                matrix_msr_local[counter] += first_integral_reverse_2_3;
                vector_msr_local[counter] = ltj - n;
                counter++;
                matrix_msr_local[counter] += first_integral_direct_1_3;
                matrix_msr_local[counter] += first_integral_reverse_1_3;
                vector_msr_local[counter] = ltj - 1;
                counter++;
                matrix_msr_local[counter] += first_integral_direct_1_3;
                matrix_msr_local[counter] += first_integral_reverse_1_3;
                vector_msr_local[counter] = ltj + 1;
                counter++;
                matrix_msr_local[counter] += first_integral_direct_2_3;
                matrix_msr_local[counter] += first_integral_reverse_2_3;
                vector_msr_local[counter] = ltj + n;
                counter++;
                matrix_msr_local[counter] += first_integral_direct_1_2;
                matrix_msr_local[counter] += first_integral_reverse_1_2;
                vector_msr_local[counter] = ltj + n + 1;
                counter++;
            }
            matrix_msr_local[counter] += first_integral_direct_1_2;
            matrix_msr_local[counter] += first_integral_reverse_1_2;
            vector_msr_local[counter] = lt - 1 - l;
            counter++;
            matrix_msr_local[counter] += first_integral_direct_2_3;
            matrix_msr_local[counter] += second_integral_direct_1_2;
            vector_msr_local[counter] = lt - l;
            counter++;
            matrix_msr_local[counter] += first_integral_direct_1_3;
            matrix_msr_local[counter] += first_integral_reverse_1_3;
            vector_msr_local[counter] = lt + n - l - 1;
            counter++;
            matrix_msr_local[counter] += second_integral_direct_2_3;
            matrix_msr_local[counter] += second_integral_reverse_2_3;
            vector_msr_local[counter] = lt + n - l + 1;
            counter++;
            matrix_msr_local[counter] += first_integral_direct_2_3;
            matrix_msr_local[counter] += second_integral_direct_1_2;
            vector_msr_local[counter] = lt + 2*n - l;
            counter++;
            matrix_msr_local[counter] += second_integral_direct_1_3;
            matrix_msr_local[counter] += second_integral_direct_1_3;
            vector_msr_local[counter] = lt + 2*n - l + 1;
            counter++; 
            for(int j = n - l + 1; j < n; j++)
            {
                int ltj = left + trapeze + j;
                matrix_msr_local[counter] += second_integral_direct_1_2;
                matrix_msr_local[counter] += second_integral_reverse_1_2;
                vector_msr_local[counter] = ltj - n - 1;
                counter++;
                matrix_msr_local[counter] += second_integral_direct_2_3;
                matrix_msr_local[counter] += second_integral_reverse_2_3;
                vector_msr_local[counter] = ltj - n;
                counter++;
                matrix_msr_local[counter] += second_integral_direct_1_3;
                matrix_msr_local[counter] += second_integral_reverse_1_3;
                vector_msr_local[counter] = ltj - 1;
                counter++;
                matrix_msr_local[counter] += second_integral_direct_1_3;
                matrix_msr_local[counter] += second_integral_reverse_1_3;
                vector_msr_local[counter] = ltj + 1;
                counter++;
                matrix_msr_local[counter] += second_integral_direct_2_3;
                matrix_msr_local[counter] += second_integral_reverse_2_3;
                vector_msr_local[counter] = ltj + n;
                counter++;
                matrix_msr_local[counter] += second_integral_direct_1_2;
                matrix_msr_local[counter] += second_integral_reverse_1_2;
                vector_msr_local[counter] = ltj + n + 1;
                counter++;
            }
            matrix_msr_local[counter] += second_integral_direct_1_3;
            vector_msr_local[counter] = lt - 1;
            counter++;
            matrix_msr_local[counter] += second_integral_direct_2_3;
            matrix_msr_local[counter] += second_integral_reverse_2_3;
            vector_msr_local[counter] = lt + n - 1;
            counter++;
            matrix_msr_local[counter] += second_integral_direct_1_2;
            matrix_msr_local[counter] += second_integral_reverse_1_2;
            vector_msr_local[counter] = lt + 2*n;
            counter++;
            matrix_msr_local[counter] += second_integral_direct_1_3;
            vector_msr_local[counter] = lt + 2*n + 1;
            counter++;
        }
        trapeze = (i + 1)*n*(n + 1);
        matrix_msr_local[counter] += first_integral_direct_1_2;
        vector_msr_local[counter] = trapeze - n - 1;
        counter++;
        matrix_msr_local[counter] += first_integral_direct_2_3;
        matrix_msr_local[counter] += second_integral_direct_1_2;
        vector_msr_local[counter] = trapeze - n;
        counter++;
        matrix_msr_local[counter] += second_integral_direct_2_3;
        vector_msr_local[counter] = trapeze + 1;
        if(i == 2) counter--;
        int local_6_4 = (i == 2)? 4: 6;
        for(int j = 1; j < n; j++)
        {
            int j_6 = (j - 1)*local_6_4;
            matrix_msr_local[counter + 2 + j_6] += second_integral_direct_1_3;
            matrix_msr_local[counter + 2 + j_6] += second_integral_reverse_1_3;
            vector_msr_local[counter + 2 + j_6] = trapeze + j - n - 1;

            matrix_msr_local[counter + 3 + j_6] += second_integral_direct_1_2;
            matrix_msr_local[counter + 3 + j_6] += second_integral_reverse_1_2;
            vector_msr_local[counter + 3 + j_6] = trapeze + j - n;

            matrix_msr_local[counter + 4 + j_6] += second_integral_direct_2_3;
            vector_msr_local[counter + 4 + j_6] = trapeze + j - 1;

            matrix_msr_local[counter + 5 + j_6] += second_integral_direct_2_3;
            vector_msr_local[counter + 5 + j_6] = trapeze + j + 1;
            if(i != 2)
            {
                vector_msr_local[counter + 6 + j_6] = trapeze + j + n;

                vector_msr_local[counter + 7 + j_6] = trapeze + j + n + 1;
            }
        }
        matrix_msr_local[counter + 2 + (n - 1)*local_6_4] += second_integral_direct_1_3;
        vector_msr_local[counter + 2 + (n - 1)*local_6_4] = trapeze - 1;

        matrix_msr_local[counter + 3 + (n - 1)*local_6_4] += second_integral_direct_2_3;
        vector_msr_local[counter + 3 + (n - 1)*local_6_4] = trapeze + n - 1;
    }
}

double right_side_help_func(int B, int A, int C, int D, int E, int F, double *f)
{
    return (10*f[B + C] + 10*f[B + A] + 6*f[B] + 4*f[B + F] + f[B + E] + f[B + D])/192;
}

void fill_right_side(double *right, double *f, Triangle* T, int n)
{
    int trapeze, trapeze_real;
    int n_local = 2*n;
    double j_1, j_2; 

    for(int i = 0; i < 3; i++)
    {   
        int first = 2*i;
        int second = 2*i + 1;
        trapeze_real = i*n*(n + 1);
        trapeze = i*n_local*(n_local + 1);

        j_1 = T[first].get_jacobian();
        j_2 = T[second].get_jacobian();

        right[trapeze_real] += j_1*right_side_help_func(trapeze, 1, n_local + 1, 2, 2*(n_local + 1), n_local + 2, f);
        for(int j = 1; j < n; j++)
        {
            int tj = trapeze + 2*j;
            int tj_real = trapeze_real + j;
            right[tj_real] += j_1*right_side_help_func(tj, -1, n_local, -2, 2*n_local, n_local - 1, f);
            right[tj_real] += j_1*right_side_help_func(tj, n_local, n_local + 1, 2*n_local, 2*(n_local + 1), 2*n_local + 1, f);
            right[tj_real] += j_1*right_side_help_func(tj, 1, n_local + 1, 2, 2*(n_local + 1), n_local + 2, f);

        }
        right[trapeze_real + n] += j_1*right_side_help_func(trapeze + n_local, -1, n_local, -2, 2*n_local, n_local - 1, f);
        right[trapeze_real + n] += j_2*right_side_help_func(trapeze + n_local, n_local, n_local + 1, 2*n_local, 2*n_local + 2, 2*n_local + 1, f);
        for(int l = 1; l < n; l++)
        {
            int left = 2*l*(n_local + 1);
            int left_real = l*(n + 1);
            int lt = left + trapeze;
            int lt_real = left_real + trapeze_real;
            right[lt_real] += j_1*right_side_help_func(lt, 1, n_local + 1, 2, 2*(n_local + 1), n_local + 2, f);
            right[lt_real] += j_1*right_side_help_func(lt, 1, -n_local, 2, -2*(n_local), -n_local + 1, f);
            right[lt_real] += j_1*right_side_help_func(lt, -n_local - 1, -n_local, -2*n_local, -2*(n_local + 1), -2*n_local - 1, f);

            for(int j = 1; j < n - l; j++)
            {
                int ltj_real = lt_real + j;
                int ltj = lt + 2*j;
                int k = ltj;
                right[ltj_real] += right_side_help_func(k, -1, n_local, -2, 2*n_local, n_local - 1, f);
                right[ltj_real] += right_side_help_func(k, n_local, n_local + 1, 2*n_local, 2*(n_local + 1), 2*n_local + 1, f);
                right[ltj_real] += right_side_help_func(k, n_local + 1, 1, 2*(n_local + 1), 2, n_local + 2, f);
                right[ltj_real] += right_side_help_func(k, 1, -n_local, 2, -2*n_local, -n_local + 1, f);
                right[ltj_real] += right_side_help_func(k, -n_local, -n_local - 1, -2*(n_local + 1), -2*n_local, -2*n_local - 1, f);
                right[ltj_real] += right_side_help_func(k, -1, -n_local - 1, -2, -2*(n_local + 1), -n_local - 2, f);
                right[ltj_real] *= j_1;
            }
            int k = lt + 2*(n - l);
            int ltj_real = lt_real + n - l;
            right[ltj_real] += right_side_help_func(k, -n_local, -n_local - 1, -2*(n_local + 1), -2*n_local, -2*n_local - 1, f);
            right[ltj_real] += right_side_help_func(k, -1, -n_local - 1, -2, -2*(n_local + 1), -n_local - 2, f);
            right[ltj_real] += right_side_help_func(k, -1, n_local, -2, 2*n_local, n_local - 1, f);
            right[ltj_real] *= j_1;
            right[ltj_real] += j_2*right_side_help_func(k, n_local, n_local + 1, 2*n_local, 2*(n_local + 1), 2*n_local + 1, f);
            right[ltj_real] += j_2*right_side_help_func(k, n_local + 1, 1, 2*(n_local + 1), 2, n_local + 2, f);
            right[ltj_real] += j_2*right_side_help_func(k, 1, -n_local, 2, -2*n_local, -n_local + 1, f);
            for(int j = n - l + 1; j < n; j++)
            {
                int ltj_real = lt_real + j;
                int ltj = lt + 2*j;
                k = ltj;
                right[ltj_real] += right_side_help_func(k, -1, n_local, -2, 2*n_local, n_local - 1, f);
                right[ltj_real] += right_side_help_func(k, n_local, n_local + 1, 2*n_local, 2*(n_local + 1), 2*n_local + 1, f);
                right[ltj_real] += right_side_help_func(k, n_local + 1, 1, 2*(n_local + 1), 2, n_local + 2, f);
                right[ltj_real] += right_side_help_func(k, 1, -n_local, 2, -2*n_local, -n_local + 1, f);
                right[ltj_real] += right_side_help_func(k, -n_local, -n_local - 1, -2*(n_local + 1), -2*n_local, -2*n_local - 1, f);
                right[ltj_real] += right_side_help_func(k, -1, -n_local - 1, -2, -2*(n_local + 1), -n_local - 2, f);
                right[ltj_real] *= j_2;
            }
            k = lt + n_local;
            right[lt_real + n] += right_side_help_func(k, -1, -n_local - 1, -2, -2*(n_local + 1), -n_local - 2, f);
            right[lt_real + n] += right_side_help_func(k, -1, n_local, -2, 2*n_local, n_local - 1, f);
            right[lt_real + n] += right_side_help_func(k, n_local, n_local + 1, 2*n_local, 2*(n_local + 1), 2*n_local + 1, f);
            right[lt_real + n] *= j_2;
        }
        trapeze_real = (i + 1)*n*(n + 1);
        trapeze = (i + 1)*n_local*(n_local + 1);
        int k = trapeze;
        right[trapeze_real] += j_1*right_side_help_func(k, -n_local, -n_local - 1, -2*(n_local + 1), -2*n_local, -2*n_local - 1, f);
        right[trapeze_real] += j_2*right_side_help_func(k, 1, -n_local, 2, -2*n_local, -n_local + 1, f);
        for(int j = 1; j < n; j++)
        {
            int tj_real = trapeze_real + j;
            int tj = trapeze + 2*j;
            k = tj;
            right[tj_real] += j_2*right_side_help_func(k, 1, -n_local, 2, -2*n_local, -n_local + 1, f);
            right[tj_real] += j_2*right_side_help_func(k, -n_local, -n_local - 1, -2*(n_local + 1), -2*n_local, -2*n_local - 1, f);
            right[tj_real] += j_2*right_side_help_func(k, -1, -n_local - 1, -2, -2*(n_local + 1), -n_local - 2, f);
        }
        k = trapeze + n_local;
        right[trapeze_real + n] += j_2*right_side_help_func(k, -1, -n_local - 1, -2, -2*(n_local + 1), -n_local - 2, f);
    }
}

void multiplication(double *matrix_msr, int *vector_msr, double *vector, double *res, int num, int p)
{

    int n = vector_msr[0] - 2;
    for(int i = num; i < n; i += p)
    {
        res[i] = matrix_msr[i]*vector[i];
        for(int j = vector_msr[i] - 1; j < vector_msr[i + 1] - 1; j++)
        {
            res[i] += matrix_msr[j]*vector[vector_msr[j]]; 
        }
    }
}

void scalar_product(double *vector_1, double *vector_2, int n, double *scalar_product, int num, int p)
{
    static pthread_mutex_t mut = PTHREAD_MUTEX_INITIALIZER;
    static pthread_cond_t c_in = PTHREAD_COND_INITIALIZER;
    static pthread_cond_t c_out = PTHREAD_COND_INITIALIZER;
    static int t_in = 0;
    static int t_out = 0;
    double local_scalar_product = 0;
    pthread_mutex_lock(&mut);
    for(int i = num; i < n; i += p) local_scalar_product += vector_1[i]*vector_2[i];
    t_in++;
    if(t_in >= p)
    {
        t_out = 0;
        pthread_cond_broadcast(&c_in);
    }
    else
    {
        while(t_in < p)
        {
            pthread_cond_wait(&c_in, &mut);
        }
    }
    *scalar_product += local_scalar_product;
    t_out++;
    if(t_out >= p)
    {
        t_in = 0;
        pthread_cond_broadcast(&c_out);
    }
    else
    {
        while(t_out < p)
        {
            pthread_cond_wait(&c_out, &mut);
        }
    }
    pthread_mutex_unlock(&mut);
}

void* solve_system(void* args)
{
    arg* a = (arg*)args;
    double *matrix_msr = a->matrix_msr;
    double *right = a->right;
    double *jacobi_matrix_msr = a->jacobi_matrix_msr;
    double *u = a->u;
    double *v = a->v;
    double *r = a->r; 
    double *x = a->x;
    double *scalar_product_1 = a->scalar_product_1; 
    double *scalar_product_2 = a->scalar_product_2;
    double EPS = a->eps;
    int *jacobi_vector_msr = a->jacobi_vector_msr;
    int *vector_msr = a->vector_msr;
    int n = a->n;
    int p = a->p;
    int num = a->num;
    int cpu_num = get_nprocs();
    int num_ver = number_of_vertices(n);
    int okay = 0;
    double eps_norm = pow(vector_norm(right, num_ver), 2)*EPS*EPS;
    double tau;

    cpu_set_t cpu;
	CPU_ZERO(&cpu);
	CPU_SET(cpu_num - a->num - 1, &cpu);
	pthread_setaffinity_np(pthread_self(), sizeof(cpu), &cpu);

    if(num == 0)
    {
        for(int i = 0; i < num_ver; i++)
        {
            r[i] = -right[i];
            jacobi_matrix_msr[i] = 1/matrix_msr[i];
            jacobi_vector_msr[i] = num_ver + 2;
        }
        jacobi_vector_msr[num_ver] = num_ver + 2;
    }
    while(1)
    {
        for(int i = 0; i < NUMBER_OF_ITERATIONS; i++)
        {
            pthread_barrier_wait(a->barrier);
            
            multiplication(jacobi_matrix_msr, jacobi_vector_msr, r, v, num, p);
            pthread_barrier_wait(a->barrier);
            
            multiplication(matrix_msr, vector_msr, v, u, num, p);
            pthread_barrier_wait(a->barrier);

            if(num == 0) *scalar_product_1 = *scalar_product_2 = 0;
            pthread_barrier_wait(a->barrier);

            scalar_product(u, r, num_ver, scalar_product_1, num, p);
            scalar_product(u, u, num_ver, scalar_product_2, num, p);
            pthread_barrier_wait(a->barrier);

            if(*scalar_product_1 < eps_norm || *scalar_product_2 < eps_norm)
            {
                okay = 1;
                break;
            }
            tau = *scalar_product_1/(*scalar_product_2);
            for(int i = num; i < num_ver; i += p)
            {
                x[i] -= tau*v[i];
                r[i] -= tau*u[i];
            }
        }
        if(okay == 1) 
        {
            pthread_barrier_wait(a->barrier);
            return 0;
        }
        else
        {
            multiplication(matrix_msr, vector_msr, x, r, num, p);
            pthread_barrier_wait(a->barrier);
            if(num == 0)
            {
                for(int i = 0; i < num_ver; i++)
                {
                    r[i] -= right[i];
                }
            }
            pthread_barrier_wait(a->barrier);
        }
    }
    return 0;
}

void* system_processing(void* args)
{
    thr* th = (thr*)args;
    double (*f) (point);
    f = th->f;
    double *x = th->x;;
    point *vertices = th->vertices;
    point *cutout_vertices = th->cutout_vertices;
    point *points = th->points;
    double *func = th->func; 
    int n = th->n;
    int p = th->p;
    int p_error = th->p_error;
    double *max = th->max;
    double *min = th->min;
    double eps = th->eps;
    pthread_t *threads = th->threads;
    pthread_barrier_t *barrier = th->barrier;
    int *is_ready = th->is_ready;

    int n_double = n*2;
    int num_ver = number_of_vertices(n);
    int num_ver_double = number_of_vertices(n_double);
    int *jacobi_vector_msr;
    double *u, *v, *r, *jacobi_matrix_msr, *matrix_msr, *right;
    int *vector_msr;
    double scalar_product_1, scalar_product_2;
    Triangle *T = static_cast<Triangle*>(operator new[] (6*sizeof(Triangle)));
    for(int i = 0; i < 3; i++)
    {
        new(T + 2*i) Triangle(vertices[i], vertices[i + 1], cutout_vertices[i], n);
        new(T + 2*i + 1) Triangle(cutout_vertices[i], vertices[i + 1], cutout_vertices[i + 1], n);
    }

    matrix_msr = new double[7*num_ver];
    vector_msr = new int[7*num_ver];
    right = new double[num_ver];

    memset(matrix_msr, 0, 7*num_ver*sizeof(double));
    memset(vector_msr, 0, 7*num_ver*sizeof(int));
    memset(right, 0, num_ver*sizeof(double));

    fill_f_and_x(f, vertices, cutout_vertices, func, points, n);
    fill_matrix(matrix_msr, vector_msr, points, T, n);
    fill_f_and_x(f, vertices, cutout_vertices, func, points, n_double);
    min_max(func, n_double, min, max);
    func[num_ver_double/2] += 0.1*p_error*(*max);
    fill_right_side(right, func, T, n);

    // cout<<"\n\x1B[1;36mAA\x1B[0m: ";
    // for(int i = 0; i < num_ver; i++)
    // {
    //     cout<<matrix_msr[i]<<"  ";
    // }
    // cout<<"\x1B[1m| * |\x1B[0m"<<"  ";
    // for(int i = num_ver + 1; i < 7*num_ver; i++)
    // {
    //     cout<<matrix_msr[i]<<"  ";
    // }
    // cout<<"\n\n\x1B[1;36mIJ\x1B[0m: ";
    // for(int i = 0; i < 7*num_ver; i++)
    // {
    //     cout<<vector_msr[i]<<"  ";
    // }
    // cout<<"\n\n\x1B[1;36mRight side\x1B[0m: ";
    // for(int i = 0; i < num_ver; i++)
    // {
    //     cout<<right[i]<<"  ";
    // }
    // cout<<"\n"<<endl;

    fill_f_and_x(f, vertices, cutout_vertices, func, points, n);

    jacobi_matrix_msr = new double[num_ver + 1];
    jacobi_vector_msr = new int[num_ver + 1];
    u = new double[num_ver];
    v = new double[num_ver];
    r = new double[num_ver];

    arg* a;
    a = new arg[p];
	if(!a)
    {
        cout<<"Not enough memory!"<<endl;
        delete[] u;
        delete[] v;
        delete[] r;
        delete[] jacobi_matrix_msr;
        delete[] jacobi_vector_msr;
        delete[] a;
        throw -1;
    }
    for (int k = 0; k < p; k++)
    {
            a[k].matrix_msr = matrix_msr;
            a[k].vector_msr = vector_msr;
            a[k].right = right;
            a[k].u = u;
            a[k].v = v;
            a[k].x = x;
            a[k].r = r;
            a[k].barrier = barrier;
            a[k].n = n;
            a[k].p = p;
            a[k].num = k;
            a[k].eps = eps;
            a[k].scalar_product_1 = &scalar_product_1;
            a[k].scalar_product_2 = &scalar_product_2;
            a[k].jacobi_matrix_msr = jacobi_matrix_msr;
            a[k].jacobi_vector_msr = jacobi_vector_msr;
    }

    for(int k = 1; k < p; k++)
    {
        if(pthread_create(&threads[k + 1], NULL, solve_system, a + k) != 0)
        {
            cout<<"Cannot create thread"<<k<<endl;
            delete[] u;
            delete[] v;
            delete[] r;
            delete[] jacobi_matrix_msr;
            delete[] jacobi_vector_msr;
            delete[] a;
            throw -1;
        }
    }
    solve_system(a + 0);

    *is_ready = 1;
    delete[] th->th;
    for(int i = 0; i < 6; i++)
    {
        T[i].~Triangle();
    }
    operator delete[] (T);
    delete[] u;
    delete[] v;
    delete[] r;
    delete[] jacobi_matrix_msr;
    delete[] jacobi_vector_msr;
    delete[] a;
    delete[] matrix_msr;
    delete[] vector_msr;
    delete[] right;
    return 0;
}