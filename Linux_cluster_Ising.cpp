//IsingModel by Suho Cho
#include <iostream>
#include <random>
#include <ctime>
#include <functional>
#include <fstream>
#include <string>
#include <algorithm>
using namespace std;

//Sum of element of array
int Sum(int* arr, int size){
    int result = 0;
    for(int i = 0 ; i < size ; i ++){
        result += arr[i];
    }
    return result;
}

//find the random valid position by random input.
int Index_position(int* arr, int random_input_num, int size){
    int result = 0;
    int count = random_input_num;
    for (int i = 0; i < size; i++){
        if(arr[i]){
            result = i;
            count -= 1;
            if (!count){
                break;
            }
        }
    }
    return result;
}

int main( int argc, char** argv ){
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
        // else if (arg == "-ctit") {
        //     T_critical_intv = stof(argv[i+1]);i += 1;}
        else if (arg == "-ad"){
            addition_name = argv[i+1];i+=1;
        }
    } if((sqrtN < 0)||(eff_mc_step < 0)||(T_start < 0)||(T_last < 0)||(T_interval < 0 )){cout<<"ERROR:38026asdf89h98hf"<<endl;return 0;}
    //initial position and conditions
    int N = sqrtN*sqrtN; int junk_count = 5000*N; 
    long long int apply_count = N*eff_mc_step;

    //matrix generate
    int SpinMatrix[N];
    fill_n(SpinMatrix, N, 1);

    //random generate
    mt19937 engine2((unsigned int)time(NULL));
    uniform_real_distribution<float> distribution2(0, N);
    auto generator2 = bind(distribution2, engine2);

    //write file generate
    string filename = "Ising_" +  to_string(sqrtN) + "x" + to_string(sqrtN) + "_effmc" + to_string(eff_mc_step) 
    +"_intv" +to_string(T_interval).substr(0, 5);
    if (T_critical_intv < 0){
        T_critical_intv = T_interval;}
    else{
        filename += "_ctit" + to_string(T_critical_intv).substr(0, 6);}
    filename += addition_name + ".txt"; 
    ofstream writefile(filename);

    //junk walk
    int T = T_start;
    for (int _____ = 0 ; _____ < junk_count ; _____ ++ ){
        //random position
        float prob = generator2();
        int position = prob ;

        //calculation
        int J = 1;
        float value = exp(-2*J*SpinMatrix[position]*(SpinMatrix[(position/sqrtN)*sqrtN + (position%sqrtN+1)%sqrtN] + SpinMatrix[(position/sqrtN)*sqrtN + (position%sqrtN-1+sqrtN)%sqrtN] 
                        + SpinMatrix[position%sqrtN + ((position/sqrtN+1)%sqrtN)*sqrtN] + SpinMatrix[position%sqrtN + ((position/sqrtN-1+sqrtN)%sqrtN)*sqrtN])/T);
        //change state by condition
        if ( (prob-position) < value){//pick random float which between 0 and 1. And compare
            SpinMatrix[position] = -1*SpinMatrix[position];
        }
    }

    //main work
    for (float T = T_start ; T >= T_last ; (2<=T&&T<=2.5)?T-=T_critical_intv:T-=T_interval ){
        //apply to mean m
        string line = to_string(T);
        float p = 1.0 - exp(-2.0/T);
        for(int ___ = 0 ; ___ < 10 ; ___++){
            long double m_sum = .0;
            for (long long int _ = 0 ; _ < apply_count / 10 ; _++){
                float prob = generator2();
                int j = prob ;
                int Pocket[N]; fill_n(Pocket, N, 0);
                int Cluster[N]; fill_n(Cluster, N, 0);
                Pocket[j] = 1; Cluster[j] = 1;
                while (Sum(Pocket, N)){
                    mt19937 engine3((unsigned int)time(NULL));
                    uniform_int_distribution<int> distribution3(0, Sum(Pocket, N));
                    auto generator3 = bind(distribution3, engine3);
                    int k = Index_position(Pocket, generator3(), N);
                    for (int l : {sqrtN*((k/sqrtN - 1) % sqrtN) + (k % sqrtN), sqrtN*((k/sqrtN + 1) % sqrtN) + (k % sqrtN), 
                                  sqrtN*(k/sqrtN) + ((k % sqrtN - 1) % sqrtN), sqrtN*(k/sqrtN) + ((k % sqrtN + 1) % sqrtN)} ){
                        float prob2 = generator2(); int prob2_int = prob2;
                        if ((SpinMatrix[l] == SpinMatrix[k])&&(!Cluster[l])&&(prob2-prob2_int < p)){
                            Pocket[l] = 1; Cluster[l] = 1;
                        }
                    }
                    Pocket[k] = 0;
                }
                for (int i = 0 ; i < N ; i ++){
                    if (Cluster[i]){
                        SpinMatrix[i] *= -1;
                    }
                }
                float S_sum = 0;
                for (int i = 0; i < N ; i ++){
                    S_sum += SpinMatrix[i];
                }
                m_sum += S_sum / N;
            }
            line += "," + to_string(m_sum / (apply_count/10));
        }
        //write at writefile
        writefile << line << endl;
    }
    writefile.close();  

    cout << "\n-------------------------------" << endl;
    cout << "End the excuction!!!" <<endl;
    cout << "-------------------------------" << endl;  
    return 0;
}
