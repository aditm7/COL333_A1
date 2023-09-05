#include <bits/stdc++.h>
using namespace std;
using namespace std::chrono;
mt19937 gen(steady_clock::now().time_since_epoch().count()); 

int main()
{
    srand(time(0));
    int z,l,time,maxval;
    cout<<"enter z:";
    cin>>z;
    cout<<"enter l:";
    cin>>l;
    cout<<"enter time:";
    cin>>time;
    cout<<"enter maxval:";
    cin>>maxval;
    uniform_int_distribution<int> rnd(1,maxval);
    string outputfilename = "./io_files/in_"+to_string(z)+"z_"+to_string(l)+"l.txt";
    ofstream outFile(outputfilename);
    outFile<<time<<endl;
    outFile<<z<<endl;
    outFile<<l<<endl;
    auto generateRandomArray= [&](int* array,int l,int idx)
    {
        for(int x=0;x<l;x++)
        {
            if(x!=idx)
            array[x]=rnd(gen);
            else array[x]=0;
        }
    };
    int array1[z];
    for(int i=0;i<z;i++)
    {
        generateRandomArray(array1,z,i);
        for(auto x:array1) outFile<<x<<" ";
        outFile<<endl;
    }
    int array2[l][l];
    for(int i=0;i<l;i++)
    {
        for(int j=i;j<l;j++){
            if(i==j) array2[i][j]=0;
            else{
                array2[i][j]=rnd(gen);
                array2[j][i]=array2[i][j];
            }
        }
    }
    for(int i=0;i<l;i++){
        for(int j=0;j<l;j++) outFile<<array2[i][j]<<" ";
        outFile<<endl;
    }
    outFile.close();
    return 0;
}