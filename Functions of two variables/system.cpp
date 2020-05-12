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
            // int ln_plus = (l + 1)*(n + 1);
            // int ln_minus = (l - 1)*(n + 1);
            // int lt_one = left + trapeze + 1;
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
            second_integral_reverse_2_3 = T[second].integral(0, 2, 0, 0, 0, 1, x[trapeze + 3*n + 1], x[trapeze + 2*n + 1], x[trapeze + 2*n]);
        }
        if(i == 0)
        {
            matrix_msr_local[counter] = first_integral_direct_1_3;
            vector_msr_local[counter] = trapeze + 1;
            counter++;
            matrix_msr_local[counter] = first_integral_direct_1_2;
            vector_msr_local[counter] = trapeze + n + 1;
            counter++;
            for(int j = 1; j < n; j++)
            {
                int tj = trapeze + j;
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
                matrix_msr_local[counter] += first_integral_reverse_1_3;
                matrix_msr_local[counter] += first_integral_direct_1_3;
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
                counter += 2;
            }
            cout<<"Counter = "<<counter<<endl;
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
            matrix_msr_local[counter] = first_integral_direct_1_2;
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
            matrix_msr_local[counter] = first_integral_direct_1_2;
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
            vector_msr_local[counter] = lt - 2;
            counter++;
            matrix_msr_local[counter] += first_integral_direct_2_3;
            matrix_msr_local[counter] += second_integral_direct_1_2;
            vector_msr_local[counter] = lt - 1;
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
            vector_msr_local[counter] = lt + n - l + n;
            counter++;
            matrix_msr_local[counter] += second_integral_direct_1_3;
            matrix_msr_local[counter] += second_integral_direct_1_3;
            vector_msr_local[counter] = lt + n - l + n + 1;
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
            matrix_msr_local[counter] = second_integral_direct_1_3;
            vector_msr_local[counter] = lt - 1;
            counter++;
            matrix_msr_local[counter] += second_integral_direct_2_3;
            matrix_msr_local[counter] += second_integral_reverse_2_3;
            vector_msr_local[counter] = lt + n - 1;
            counter++;
            matrix_msr_local[counter] += second_integral_direct_1_2;
            matrix_msr_local[counter] += second_integral_reverse_1_3;
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
        cout<<"Counter bottom = "<<counter + num_ver + 1<<endl;
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
    return (f[B + C] + f[B + A] + 6*f[B] + 4*f[B + F] + 10*f[B + E] + 10*f[B + D]);
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

        right[trapeze_real] += j_1*right_side_help_func(trapeze + 2, -2, 2*n_local, -1, n_local - 1, n_local, f);
        for(int j = 1; j < n; j++)
        {
            int tj = trapeze + 2*j;
            right[trapeze_real + j] += j_1*right_side_help_func(tj, -1, n_local, -2, 2*n_local, n_local - 1, f);
            right[trapeze_real + j] += j_1*right_side_help_func(tj, n_local, n_local + 1, 2*n_local, 2*(n_local + 1), 2*n_local + 1, f);
            right[trapeze_real + j] += j_1*right_side_help_func(tj, 1, n_local, 2, 2*(n_local + 1), n_local + 1, f);

        }
        right[trapeze_real + n] += j_1*right_side_help_func(trapeze + n_local, -1, n_local, -2, 2*n_local, n_local - 1, f);
        right[trapeze_real + n] += j_2*right_side_help_func(trapeze + n_local, n_local, n_local + 1, 2*n_local, 2*n_local + 2, 2*n_local + 1, f);
        for(int l = 1; l < n; l++)
        {
            int left = 2*l*(n_local + 1);
            int left_real = l*(n + 1);
            int lt = left + trapeze;
            int lt_real = left_real + trapeze_real;
            right[lt_real] += right_side_help_func(lt, 1, n_local + 1, 2, 2*(n_local + 1), n_local + 2, f);
            right[lt_real] += right_side_help_func(lt, 1, -n_local, 2, -2*(n_local), -n_local + 1, f);
            right[lt_real] += right_side_help_func(lt, -n_local - 1, -n_local, -2*n_local, -2*(n_local + 1), -2*n_local - 1, f);
            right[lt_real] *= j_1;
        // cout<<"Okay"<<endl;
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
            right[lt_real + n - l] += right_side_help_func(k, -n_local, -n_local - 1, -2*(n_local + 1), -2*n_local, -2*n_local - 1, f);
            right[lt_real + n - l] += right_side_help_func(k, -1, -n_local - 1, -2, -2*(n_local + 1), -n_local - 2, f);
            right[lt_real + n - l] += right_side_help_func(k, -1, n_local, -2, 2*n_local, n_local - 1, f);
            right[lt_real + n - l] *= j_1;
            right[lt_real + n - l] += j_2*right_side_help_func(k, n_local, n_local + 1, 2*n_local, 2*(n_local + 1), 2*n_local + 1, f);
            right[lt_real + n - l] += j_2*right_side_help_func(k, n_local + 1, 1, 2*(n_local + 1), 2, n_local + 2, f);
            right[lt_real + n - l] += j_2*right_side_help_func(k, 1, -n_local, 2, -2*n_local, -n_local + 1, f);
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
            right[tj_real] += right_side_help_func(k, 1, -n_local, 2, -2*n_local, -n_local + 1, f);
            right[tj_real] += right_side_help_func(k, -n_local, -n_local - 1, -2*(n_local + 1), -2*n_local, -2*n_local - 1, f);
            right[tj_real] += right_side_help_func(k, -1, -n_local - 1, -2, -2*(n_local + 1), -n_local - 2, f);
            right[tj_real] *= j_2;
        }
        k = trapeze + n_local;
        right[trapeze_real + n] += j_2*right_side_help_func(k, -1, -n_local - 1, -2, -2*(n_local + 1), -n_local - 2, f);
    }
}