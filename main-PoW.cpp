#include<iostream>
#include<string>
#include<cstring>
#include<vector>
#include<chrono>
#include "sha256.cpp"

using namespace std;
using namespace std::chrono;


class block{
    string data="";
    string hash="";
    int nonce=0;
    string previous_hash="0000000000000000000000000000000000000000000000000000000000000000";
    int number;
    double duration=0.0;
    public:
    
        block(string data,int number=0)
        {
            this->data=data;
            this->number=number;
            computeHash();
        }

        void computeHash()
        {
            string fin=previous_hash+to_string(number)+data+to_string(nonce);
            this->hash=SHA256(fin.c_str());
        }

        void display()
        {
            cout<<"Block# "<<number<<endl;
            cout<<"Hash: "<<hash<<endl;
            cout<<"Previous: "<<previous_hash<<endl;
            cout<<"Data: "<<data<<endl;
            cout<<"Nonce: "<<nonce<<endl;
            cout<<"Time taken to mine(in seconds): "<<duration<<endl<<endl;
        }
        friend class Blockchain;
};
class Blockchain{
    int difficulty=4;
    vector<block> chain;
    public:
        Blockchain(vector<block> chain={})
        {
            this->chain=chain;
        }

        void add(block b)
        {
            chain.push_back(b);
        }
        void mine(block b)
        {
            if(chain.size()!=0)
                b.previous_hash=chain[chain.size()-1].hash;
            //to check elapsed time to mine the block
            auto startTime = high_resolution_clock::now();

            while(true)
            {
                //snip the first difficulty (here 4) characters of the hash and store in checker
                string checker=b.hash.substr(0,difficulty);
                //cout<<b.hash<<endl;
                int flag=1;
                //flag will be 1 if first difficulty (here 4) characters are zero
                for(int i=0;i<difficulty;i++)
                {
                    if(checker[i]!='0')
                        flag=0;
                    //if atleast one character is not zero set flag to zero, meaning this not the final hash
                }
                //we have the correct final hash
                if(flag==1)
                {
                    this->add(b);
                    break;
                }
                else
                {
                    b.nonce++;
                    b.computeHash();
                }
            }

            auto endTime = high_resolution_clock::now();
            auto elapsedTime = duration_cast<microseconds>(endTime - startTime).count();
            double duration=static_cast<double>(elapsedTime)/1e9;
            chain[chain.size()-1].duration=duration;
        }
        void displayChain()
        {
            for(int i=0;i<chain.size();i++)
                chain[i].display();
        }
};


int main()
{
    Blockchain blockchain;
    vector<string> input={"hello world","what's up","hello","bye"};
    for(int i=0;i<input.size();i++)
    {
        blockchain.mine(block(input[i],i+1));
    }
    blockchain.displayChain();
}