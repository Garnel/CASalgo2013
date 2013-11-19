/*
 * 3 Divide and Conquer
 * Recall the problem of finding the number of inversions. As in the course,
 * we are given a sequence of n numbers a_1 ,...,a_n , which we assume are all
 * distinct, and we difine an inversion to be a pair i < j such that a_i > a_j.
 * We motivated the problem of counting inversions as a good measure of
 * how different two orderings are. However, one might feel that this measure
 * is too sensitive. Let’s call a pair a significant inversion if i < j and 
 * a_i > 3*a_j .
 * Given an O(nlogn) algorithm to count the number of significant inversions
 * between two orderings.
 */
#include <iostream>
#include <fstream>
#include <cstdlib>
using namespace std;

//int N=4;
//int data[4]= {3,4,1,2};
int N = 10;
int data[10] = {9,8,7,6,5,4,3,2,1,0};

long long merge_count(int l, int m, int r)
{
    int count=0;
    int* L=new int[m - l + 1];
    int* R=(int*)malloc((r-m)*sizeof(int));
    cout<<"left sequence:";
    for(int i=l; i<=m; i++)
    {
        L[i-l]=data[i];
        cout<<L[i-l]<<" ";
    }
    cout<<endl;
    cout<<"right sequence:";
    for(int i=m+1; i<=r; i++)
    {
        R[i-m-1]=data[i];
        cout<<R[i-m-1]<<" ";
    }
    cout<<endl;
    int i=0,j=0,k=l;
    cout<<"data turns to be:";
    while(i<=m&&j<=r-m-1)
    {
        if(L[i]>R[j])
        {
//cout<<L[i]<<"is big than"<<R[j]<<endl;
            data[k]=R[j];
            j++;
            count+=m-l-i+1;
        }
        else
        {
            data[k]=L[i];
            i++;
//cout<<L[i]<<"is less than"<<R[j]<<endl;
        }
        cout<<data[k]<<" ";
        k++;
    }
    while(i<=m-l)
    {
        data[k++]=L[i++];
    }
    while(j<=r-m-1)
    {
        data[k++]=R[j++];
    }
//free(L);
//free(R);
    cout<<"inversion_count:"<<count<<endl;
    return count;
}

long long inversion_Counting(int l,int r)
{
    if(l<r)
    {
        int m=(l+r)>>1;
//cout<<"m:"<<m<<endl;
        long long L=inversion_Counting(l,m);
        long long R=inversion_Counting(m+1,r);
        long long LR=merge_count(l,m,r);
        cout<<L+R+LR<<endl;
        return L+R+LR;
    }
    else
        return 0;
}
int main()
{
    int n, *data;

    cout << inversion_Counting(0, n);
    system("pause");
    return 0;
}