#include "SL.h"
mt19937 gen(steady_clock::now().time_since_epoch().count());

void SportsLayout::set_iterations(){
    // if(l<=20) this->it = 10000;
    // else if(l<=50) this->it = 7500;
    // else if(l<=100) this->it = 4000;
    // else if(l<=400) this->it = 2500;
    // else if(l<=600) this->it = 1000;
    // else this->it=1000;
    int num = ((this->time*60-1)*(10*10*10*10))/(this->l+this->z);
    this->it=num;
}

SportsLayout::SportsLayout(string inputfilename)
{
    readInInputFile(inputfilename);
    mapping = new int[z];
}

// Make a destructor to empty the memory allocated in the constructor.

SportsLayout::~SportsLayout()
{
    for (int i = 0; i < l; ++i)
        delete[] T[i];
    delete[] T;
    for (int i = 0; i < z; ++i)
        delete[] N[i];
    delete[] N;
    delete[] mapping;
}

bool SportsLayout::check_output_format()
{
    vector<bool> visited(l, false);
    for (int i = 0; i < z; i++)
    {
        if ((mapping[i] >= 1 && mapping[i] <= l))
        {
            if (!visited[mapping[i] - 1])
                visited[mapping[i] - 1] = true;
            else
            {
                cout << "Repeated locations, check format\n";
                return false;
            }
        }
        else
        {
            cout << "Invalid location, check format\n";
            return false;
        }
    }
    return true;
}

long long SportsLayout::cost_fn(int *mp)
{
    long long cost = 0;
    for (int i = 0; i < z; i++)
    {
        for (int j = i + 1; j < z; j++)
        {
            cost += ((long long)N[i][j] + (long long)N[j][i]) * (long long)T[mp[i] - 1][mp[j] - 1];
        }
    }
    return cost;
}

void SportsLayout::readInInputFile(string inputfilename)
{
    fstream ipfile;
    ipfile.open(inputfilename, ios::in);
    if (!ipfile)
    {

        cout << "No such file\n";
        exit(0);
    }
    else
    {

        ipfile >> time;
        ipfile >> z;
        ipfile >> l;

        if (z > l)
        {
            cout << "Number of zones more than locations, check format of input file\n";
            exit(0);
        }

        int **tempT;
        int **tempN;

        tempT = new int *[l];
        for (int i = 0; i < l; ++i)
            tempT[i] = new int[l];

        tempN = new int *[z];
        for (int i = 0; i < z; ++i)
            tempN[i] = new int[z];

        for (int i = 0; i < z; i++)
        {
            for (int j = 0; j < z; j++)
                ipfile >> tempN[i][j];
        }

        for (int i = 0; i < l; i++)
        {
            for (int j = 0; j < l; j++)
                ipfile >> tempT[i][j];
        }

        ipfile.close();

        T = tempT;
        N = tempN;
    }
}

void SportsLayout::write_to_file(string outputfilename)
{

    // Open the file for writing
    ofstream outputFile(outputfilename);

    // Check if the file is opened successfully
    if (!outputFile.is_open())
    {
        cerr << "Failed to open the file for writing." << std::endl;
        exit(0);
    }

    for (int i = 0; i < z; i++)
        outputFile << mapping[i] << " ";

    // Close the file
    outputFile.close();

    cout << "Allocation written to the file successfully." << endl;
}

int *SportsLayout::generate_random_mapping()
{
    int *locations = new int[l];
    for (int i = 0; i < l; i++)
        locations[i] = i + 1;
    shuffle(locations, locations + l, gen);
    int *init_mp = new int[z];
    for (int i = 0; i < z; i++)
        init_mp[i] = locations[i];
    return init_mp;
}

void SportsLayout::compute_allocation(int *mp)
{
    for (int i = 0; i < z; i++)
        mapping[i] = mp[i];
}

long long SportsLayout::find_contribution(int *mp, int idx)
{
    long long ans = 0;
    for (int i = 0; i < z; i++)
    {
        ans += ((long long)N[i][idx] + (long long)N[idx][i]) * (long long)T[mp[i] - 1][mp[idx] - 1];
    }
    return ans;
}

bool SportsLayout::exit_indicator(){ //returns true when it is time to exit
    auto stop = high_resolution_clock::now();
    auto duration = duration_cast<milliseconds>(stop - start);
    return duration.count() >= (double)((time * 1.0 * 60 - 0.3) * 1000);
}


