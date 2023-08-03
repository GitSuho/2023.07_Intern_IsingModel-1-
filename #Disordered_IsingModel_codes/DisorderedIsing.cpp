//Disordered IsingModel by Suho Cho
#include <iostream>
#include <random>
#include <ctime>
#include <functional>
#include <fstream>
#include <string>
using namespace std;

int main( int argc, char** argv ){
    //input factors
    long long int eff_mc_step = -1; 
    float T_start = -1; float T_last = -1; float T_interval = -1;
    int sqrtN = -1; float p_factor = -1;
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
        else if (arg == "-p"){
            p_factor = stof(argv[i+1]);i += 1;}
        else if (arg == "-ad"){
            addition_name = argv[i+1];i+=1;
        }
    } if((sqrtN < 0)||(eff_mc_step < 0)||(T_start < 0)||(T_last < 0)||(T_interval < 0 )||(p_factor < 0)){cout<<"ERROR:3823018hd83h899h98hf"<<endl;return 0;}
    //initial position and conditions
    int N = sqrtN*sqrtN; int junk_count = 5000*N; 
    long long int apply_count = N*eff_mc_step;

    //matrix generate
    int SpinMatrix[N];
    fill_n(SpinMatrix, N, 1);

    //random generate
    random_device rd;
    mt19937 engine1(rd());
    uniform_real_distribution<float> distribution1(0, N);
    mt19937 engine2(rd());
    uniform_real_distribution<float> distribution2(0, 1);

    //write file generate
    string filename = "DisorderedIsing_" +  to_string(sqrtN) + "x" + to_string(sqrtN) + "_effmc" + to_string(eff_mc_step) 
    +"_intv" +to_string(T_interval).substr(0, 5) + "_p" + to_string(p_factor).substr(0.4) + addition_name + ".txt"; 
    ofstream writefile(filename);

    //junk walk
    int T = T_start;
    for (int _____ = 0 ; _____ < junk_count ; _____ ++ ){
        //random position
        float prob = distribution1(engine1);
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
    //calculate N*m0
    float S_sum = .0 ;
    for (int i = 0 ; i < N ; i++){
        S_sum += SpinMatrix[i];
    }  


    //main work
    for (float T = T_start ; T >= T_last ; T-=T_interval ){
        //apply to mean m
        string line = to_string(T);
        for(int ___ = 0 ; ___ < 10 ; ___++){
            long double m_sum = .0; int J;
            for (long long int _ = 0 ; _ < apply_count / 10 ; _++){
                //random position
                float prob = distribution1(engine1);
                int position = prob ;

                //calculation
                J = -1;
                if (distribution2(engine2) < p_factor){
                    J = 1;
                }
                float value = exp(-2*J*SpinMatrix[position]*(SpinMatrix[(position/sqrtN)*sqrtN + (position%sqrtN+1)%sqrtN] + SpinMatrix[(position/sqrtN)*sqrtN + (position%sqrtN-1+sqrtN)%sqrtN] 
                        + SpinMatrix[position%sqrtN + ((position/sqrtN+1)%sqrtN)*sqrtN] + SpinMatrix[position%sqrtN + ((position/sqrtN-1+sqrtN)%sqrtN)*sqrtN])/T);

                //change state by condition
                if ((prob-position) < value){//pick random float which between 0 and 1. And compare
                    SpinMatrix[position] = -1*SpinMatrix[position];
                    S_sum += 2*SpinMatrix[position];
                }
                //calculate m sum
                m_sum += S_sum/N;
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
