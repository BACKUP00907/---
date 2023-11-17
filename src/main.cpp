#include <iomanip>
#include <iostream>
#include <fstream>
#include <pybind11/pybind11.h>
#include <thread>
#include "rx-slow-hash.h"
#include <sstream>
#include <ctime>
#include <chrono>

namespace py = pybind11;


using namespace std::chrono;


py::bytes get_rx_hash( std::string fblob ,std::string lblob , std::string seed_hash, const uint64_t height, uint64_t turget , unsigned long int  hohu , int fasnt)
{
    if (fasnt==0){
            unsigned long long int tash = 0;
            

            
            char* spine [6] ;
            spine[0] ="|";
            spine[1] ="/";
            spine[2] = "-";
            spine[3] = "\\";
            spine[5] ="/";
            
            int xn =0;
            
            unsigned int nonce = 1 ;
            nonce += hohu;
            unsigned int lnnonce = 1 ;
            unsigned int* enonce = &lnnonce;
            
            
            
            char input[250];
            uint64_t seed_height = rx_seedheight(height);
            std::string output;
            output.resize(32);
            
            uint64_t r64;
            
            std::stringstream lks;
            
            std::ofstream nom;
            nom.open("non.txt");
        
            char or64[8];
        
            size_t fbl = fblob.size();
            size_t lbl = lblob.size();
            size_t nbl = (fbl+sizeof(nonce));
            size_t xbl = nbl + lbl;
            auto start = std::chrono::high_resolution_clock::now();
            while (1 == 1){
            
                //lnnonce = __builtin_bswap64(nonce);
                lnnonce = nonce;
                //input.resize((sizeof(fblob))+ (sizeof(lblob)) + (sizeof(nonce))) ;
                
                memcpy((&input[0]),fblob.data(),fbl);
                memcpy((&input[fbl]),&nonce,sizeof(nonce));
                memcpy((&input[nbl]),lblob.data(),lbl);
                
                
            
                rx_slow_hashL(height, seed_height, seed_hash.data(), &input[0], xbl, &output[0], 1, 0);
                
                tash++;
                
                memcpy(or64,output.substr(24).data(),8);
                memcpy(&r64,or64,8);
            
                
                
                
                //r64 = __builtin_bswap64(r64);
                if( r64 < turget){
                    
                    
                    auto end = std::chrono::high_resolution_clock::now();
                    auto ctim =std::chrono::duration_cast<std::chrono::seconds>(end-start);
                    unsigned long long int kdj = (tash /ctim.count() );
                    std::cout << "\n" << "HASHRATE: " <<  kdj << "\n";
                    nom << nonce ;
                    nom.close();
                    
                    return output;
                
                }
                memset(&input[0], 0, 250);
                nonce++;
            }
    }

    if (fasnt==1){
            unsigned long long int tash = 0;
            

            
            char* spine [6] ;
            spine[0] ="|";
            spine[1] ="/";
            spine[2] = "-";
            spine[3] = "\\";
            spine[5] ="/";
            
            int xn =0;
            
            unsigned int nonce = 1 ;
            nonce += hohu;
            unsigned int lnnonce = 1 ;
            unsigned int* enonce = &lnnonce;
            
            
            
            char input[250];
            uint64_t seed_height = rx_seedheight(height);
            std::string output;
            output.resize(32);
            
            uint64_t r64;
            
            std::stringstream lks;
            
            std::ofstream nom;
            nom.open("non.txt");
        
            char or64[8];
        
            size_t fbl = fblob.size();
            size_t lbl = lblob.size();
            size_t nbl = (fbl+sizeof(nonce));
            size_t xbl = nbl + lbl;
            auto start = std::chrono::high_resolution_clock::now();
            while (1 == 1){
            
                //lnnonce = __builtin_bswap64(nonce);
                lnnonce = nonce;
                //input.resize((sizeof(fblob))+ (sizeof(lblob)) + (sizeof(nonce))) ;
                
                memcpy((&input[0]),fblob.data(),fbl);
                memcpy((&input[fbl]),&nonce,sizeof(nonce));
                memcpy((&input[nbl]),lblob.data(),lbl);
                
                
            
                rx_slow_hash(height, seed_height, seed_hash.data(), &input[0], xbl, &output[0], 1, 0);
                
                tash++;
                
                memcpy(or64,output.substr(24).data(),8);
                memcpy(&r64,or64,8);
            
                
                
                
                //r64 = __builtin_bswap64(r64);
                if( r64 < turget){
                    
                    
                    auto end = std::chrono::high_resolution_clock::now();
                    auto ctim =std::chrono::duration_cast<std::chrono::seconds>(end-start);
                    unsigned long long int kdj = (tash /ctim.count() );
                    std::cout << "\n" << "HASHRATE: " <<  kdj << "\n";
                    nom << nonce ;
                    nom.close();
                    
                    return output;
                
                }
                memset(&input[0], 0, 250);
                nonce++;
            }
    }
            
}



PYBIND11_MODULE(pyrx, m)
{
    m.def("get_rx_hash", &get_rx_hash, R"pbdoc(Get a RandomX hash which is a valid share)pbdoc");

#ifdef VERSION_INFO
    m.attr("__version__") = VERSION_INFO;
#else
    m.attr("__version__") = "dev";
#endif
}
