//IsingModel by Suho Cho
#include <iostream>
#include <random>
#include <ctime>
#include <functional>
#include <fstream>
#include <string>
#include <algorithm>
#include <vector>
using namespace std;

int main( int argc, char** argv ){
    // clock_t start, end;
    // start = clock();

    //input factors
    long long int eff_mc_step = -1; 
    float T_start = -1; float T_last = -1; float T_interval = -1; float T_critical_intv = -1;
    int sqrtN = -1;
    string addition_name = "";
    for (int i = 1; i < argc; i++){
        string arg = argv[i];
        if (arg == "-s"){
            sqrtN = stoi(argv[i+1]);i += 1;}
        else if (arg == "-mc"){
            eff_mc_step = stoll(argv[i+1]);i += 1;} 
        else if (arg == "-ts"){
            T_start = stof(argv[i+1]);i += 1;}
        else if (arg == "-tl"){
            T_last = stof(argv[i+1]);i += 1;}
        else if (arg == "-it") {
            T_interval = stof(argv[i+1]);i += 1;}
        else if (arg == "-ad"){
            addition_name = argv[i+1];i+=1;}
    } if((sqrtN < 0)||(eff_mc_step < 0)||(T_start < 0)||(T_last < 0)||(T_interval < 0 )){cout<<"ERROR:38026asdf89h98hf"<<endl;return 0;}
    
    //initial conditions
    int N = sqrtN*sqrtN; int junk_count = 5000*N; 
    long long int apply_count = N*eff_mc_step;

    //matrix generate
    int SpinMatrix[N];
    fill_n(SpinMatrix, N, 1);

    //Random generate
    random_device rd;
    static mt19937 engine2(rd());
    static uniform_int_distribution<int> distribution2(0, N-1);
    static mt19937 engine3(rd());
    static uniform_real_distribution<float> distribution3(0, 1);

    //write file generate
    string filename = "Ising_"+to_string(sqrtN)+"x"+to_string(sqrtN)+"_MCeff"+to_string(eff_mc_step)+"_intv"+to_string(T_interval).substr(0, 5)+"mean(abs(m))_arr"+addition_name+".txt"; 
    ofstream writefile(filename);

    //junk MC
    for (int _____ = 0 ; _____ < junk_count ; _____ ++ ){
        int position = distribution2(engine2);
        int J = 1;
        float value = exp(-2*J*SpinMatrix[position]
                    *(SpinMatrix[(position/sqrtN)*sqrtN + (position%sqrtN+1)%sqrtN] + SpinMatrix[(position/sqrtN)*sqrtN + (position%sqrtN-1+sqrtN)%sqrtN] 
                    + SpinMatrix[position%sqrtN + ((position/sqrtN+1)%sqrtN)*sqrtN] + SpinMatrix[position%sqrtN + ((position/sqrtN-1+sqrtN)%sqrtN)*sqrtN])
                    /T_start);
        if ( distribution3(engine3) < value){
            SpinMatrix[position] = -1*SpinMatrix[position];
        }
    }

    //Sum of the initial Spins
    float S_sum = 0;
    for (int i = 0; i < N ; i ++){
        S_sum += SpinMatrix[i];
    }

    //main work
    int Pocket[N]; int Cluster[N];
    int Pocket_size; int Cluster_size; 
    int j; 
    for (float T = T_start ; T >= T_last ; T -= T_interval ){
        string write_line = to_string(T);
        float p = 1.0 - exp(-2.0/T);
        for(int ___ = 0 ; ___ < 10 ; ___++){
            long double m_sum = .0;
            for (long long int _ = 0 ; _ < apply_count / 10 ; _++){
                j = distribution2(engine2);
                fill_n(Pocket, N, 1); fill_n(Cluster, N, 1);
                Pocket[j] = -1 ; Cluster[j] = -1;
                Pocket_size = 1; Cluster_size = 1;

                while (Pocket_size > 0){
                    //Choice random element of the Pocket
                    int foo = distribution3(engine3) * Pocket_size + 1; 
                    int ind = 0;
                    for ( ; ind < N ; ind++){
                        foo += (Pocket[ind] - 1)/2;
                        if (foo == 0){
                            break;
                        }
                    }
                    int k = ind;
                    for (int l : {sqrtN*((k/sqrtN - 1 + sqrtN) % sqrtN) + (k % sqrtN), sqrtN*((k/sqrtN + 1) % sqrtN) + (k % sqrtN), 
                                  sqrtN*(k/sqrtN) + ((k % sqrtN - 1 + sqrtN) % sqrtN), sqrtN*(k/sqrtN) + ((k % sqrtN + 1) % sqrtN)} ){ //l is one of close index of k position of SpinMatrix



                        //Add the element l at Pocket and Cluster when the below conditions are satisfied
                        if ((SpinMatrix[l] == SpinMatrix[k])&&(Cluster[l] == 1)&&(distribution3(engine3) < p)){
                            Pocket[l] = -1; Pocket_size += 1;
                            Cluster[l] = -1; Cluster_size += 1;
                        }
                    }
                    Pocket[k] = 1; Pocket_size -= 1;//Eliminate the k element of the Pocket
                }
                for (int i = 0; i < N; i++){
                    SpinMatrix[i] *= Cluster[i];//Update the spins' direction
                }
                // S_sum += 2.0*SpinMatrix[j]*Cluster_size;//Update the sum of spins
                S_sum = 0;
                for (int i = 0; i < N ; i ++){
                    S_sum += SpinMatrix[i];
                }

                m_sum += fabs(S_sum / N);//Add the m value
            }
            write_line += "," + to_string(m_sum / (apply_count/10));//Wirte the <m>
        }
        writefile << write_line << endl;
    }
    // end = clock();
    // writefile << "#" << to_string(end - start) << endl;

    writefile.close();

    cout << "\n-------------------------------" << endl;
    cout << "End the excuction!!!" <<endl;
    cout << "-------------------------------" << endl;  
    return 0;
}