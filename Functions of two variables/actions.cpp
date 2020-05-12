#include "header.hpp"
using namespace std;
// void choose_function(double (*f) (point), int k)
// {
//     switch(k)
//     {
//         case 0:
//             f = f_0;
//             break;
//         default:
//             f = f_0;
//             break;
//     }
// }

void print_system_msr(double *matrix_msr, int *vector_msr, double *right, int n)
{
    cout<<"(Scalar products matrix | Right side of system):\n"<<endl;
    int num_ver = number_of_vertices(n);
    int j, num_elem, search, counter = num_ver + 1;
    for(int i = 0; i < num_ver; i++)
    {
        if(i == 0) cout<<"/";
        else if(i == num_ver - 1) cout<<"\\";
        else cout<<"|";
        
        num_elem = vector_msr[i + 1] - vector_msr[i];
        j = 0;
        search = vector_msr[counter];
        while(num_elem != 0)
        {
            while(j != search && j != i)
            {
                cout<<setw(SETW_CONSTANT)<<" - ";
                if(j > num_ver) exit(0);
                j++;
            }
            if(j == search)
            {
                cout<<setw(SETW_CONSTANT)<<matrix_msr[counter];
                counter++;
                search = vector_msr[counter];
                num_elem--;
                j++;
                continue;
            }
            else
            {
                cout<<setw(SETW_CONSTANT)<<matrix_msr[i];
                j++;
                continue;
            }
        }
        for(int k = j; k < num_ver; k++)
        {
            if(k == i) cout<<setw(SETW_CONSTANT)<<matrix_msr[i]; 
            else cout<<setw(SETW_CONSTANT)<<" - ";
        }
        cout<<setw(SETW_CONSTANT)<<" | ";
        cout<<setw(SETW_CONSTANT)<<right[i];
        if(i == 0) cout<<"\\";
        else if(i == num_ver - 1) cout<<"/";
        else cout<<"|";
        cout<<endl;
    }
    cout<<"\nLast element = "<<matrix_msr[num_ver - 1]<<endl;
}