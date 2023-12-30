#include<iostream>
#include<vector>
#include<string>
#include<string.h>
#include<chrono>
#include<thread>           
#include<unistd.h>    
#include<cstdlib>
#include<unordered_map>
#include<ctime>
#include "timestamp.cpp"
#include "sha256.cpp"
#include "parser.cpp"
#include "QRcode.cpp"
#define BLOCK_SIZE 4
#define NODE 10

using namespace std;
using namespace std::chrono;

class transaction
{
    //string manufactureID;
    
    public:
        string distributorID;
        string transactionID;
        string clientID;
        string productID;
        string timestamp1;
        string timestamp2;
        string timestamp3;
        string transactionHash;

        void viewTransaction()
        {
            cout<<"---------------------------------------------------------------------\n";
            cout<<"Distributer ID: "<<distributorID<<endl;
            cout<<"Transaction ID: "<<transactionID<<endl;
            cout<<"Client ID: "<<clientID<<endl;
            cout<<"Product ID: "<<productID<<endl;
            cout<<"Timestamp when Distributor got selected: "<<timestamp1<<endl;
            cout<<"Timestamp when distributor dispatched product "<<timestamp2<<endl;
            cout<<"Timestamp when client recieved product: "<<timestamp3<<endl<<endl;
            cout<<"---------------------------------------------------------------------\n\n";
        }

};


class block
{
    vector<transaction*> transactions;
    string merkleNodehash;
    string previous_hash;
    string blockHash;
    
    int block_incentive;
    int node;
    int number;//block serial

    friend class blockchain;

    void displayBlock()
    {
            cout<<"----------------START OF BLOCK-----------------------\n\n";
            cout<<"Block Number: "<<number<<"\n";
            cout<<"Merkle Root: "<<merkleNodehash<<"\n";
            cout<<"Block Hash: "<<blockHash<<"\n";
            cout<<"Previous Hash\n"<<previous_hash<<"\n";
            cout<<"Block Added by: Node "<<node<<"\n";
            cout<<"Transactions\n";
            for(int j=0;j<BLOCK_SIZE;j++)
                transactions[j]->viewTransaction();
            cout<<"Block Incentive "<<block_incentive<<"\n";
            cout<<"----------------END OF BLOCK-----------------------\n\n";
    }
};

class blockchain
{
    vector<pair<string,string>> distributors;//distributor ID, name of the distributor
    vector<string> clients; //client id
    vector<pair<string,int>> products;//productID, product count
    block *rearBlock;
    vector<transaction*> allTxn;
    
    vector<block*> chain;
    unordered_map<string,block*> blockMap;
    int chain_size;
    string PoETMaximal;
    public:
        vector<transaction*> transaction_queue;
        int nodeQ;
    
    blockchain()
    {
        nodeQ=-1;
        //default distributors
        distributors.push_back({"DIS001","Piyush and Company"});
        distributors.push_back({"DIS002","Avesh and Sons"});
        distributors.push_back({"DIS003","Cups and Mugs"});
        chain_size=1;
        //default clients
        clients.push_back("CLI001");
        clients.push_back("CLI002");
        clients.push_back("CLI003");

        //defatult products
        products.push_back({"PROD001",10});
        products.push_back({"PROD002",20});
        products.push_back({"PROD003",10});

        block *b=new block();
        transaction *t=new transaction();

        t->clientID="CLI000";
        t->distributorID="DIS000";
        t->transactionID="TXN0001";
        t->timestamp1="0";
        t->timestamp2="0";
        t->timestamp3="0";
        t->productID="PROD000";
        t->transactionHash="00000000000000000000000000000000000000000000000";
        b->transactions.push_back(t);
        
        for(int i=0;i<BLOCK_SIZE-1;i++)
        {
            transaction *t1=new transaction();
            t1->clientID="CLI000";
            t1->distributorID="DIS000";
            t1->transactionID="TXN0001";
            t1->timestamp1="0";
            t1->timestamp2="0";
            t1->timestamp3="0";
            t1->productID="PROD000";
            t1->transactionHash="00000000000000000000000000000000000000000000000";
            b->transactions.push_back(t1);
        }
        b->number=0;
        b->blockHash="000000000000000000000000000000000000000000000000000000000000000000000000";
        chain.push_back(b);
        blockMap.emplace((b->blockHash),b);
        rearBlock=b;
        b->merkleNodehash="THIS IS THE GENESIS BLOCK, IGNORE";
        b->block_incentive=0;
    }
    void viewDis()
    {
        cout<<"Currently we have "<<distributors.size()<<" distributors as:\n\n";
        for(int i=0;i<distributors.size();i++)
        {
            cout<<"Distributor ID: "<<distributors[i].first<<"  ";
            cout<<"Distributor Name: "<<distributors[i].second<<endl<<endl;
        }
    }

    void viewProd()
    {
        cout<<"Currently we have "<<products.size()<<" products as: \n\n";
        for(int i=0;i<products.size();i++)
        {
            cout<<"Product ID: "<<products[i].first<<endl<<endl;
            cout<<"Product Count: "<<products[i].second<<endl<<endl;
        }
    }

    void viewClients()
    {
        cout<<"Number of Clients we have "<<clients.size()<<" with client ids:"<<"\n";
        for(int i=0;i<clients.size();i++)
        {
            cout<<clients[i]<<endl;
        }
    }

    void viewArr(vector<string> &t)
    {
        for(int i=0;i<t.size();i++)
            cout<<t[i]<<" ";
        cout<<endl;
    }

    string merkle(block *b)
    {
        int i=BLOCK_SIZE;
        vector<string> h;
        

        for(int i=0;i<BLOCK_SIZE;i++)
            h.push_back((b->transactions[i])->transactionHash);

        if(i%2!=0)//odd number of leaf
            h.push_back((b->transactions[BLOCK_SIZE-1])->transactionHash);
        while(i>1)
        {
            vector<string> temp;
            viewArr(temp);
            int j=0;
            while(j<i)
            {
                temp.push_back(getHash(h[j]+h[j+1]));
                j+=2;
            }
            h=temp;
            i-=2;
        }
        return h[0];
    }

    pair<string,string> getDisId()
    {
        srand(time(nullptr));
        int index = std::rand() % distributors.size();
        return distributors[index];
    }

    void manufacturer()
    {
        system("clear");
        cout<<"Choose one of the following (1 or 2)\n\n";
        cout<<"1. Add More Products\n";
        cout<<"2. Go Back\n\n";
        int choice;
        cin>>choice;
        
        if(choice==2)
        {
            system("clear");
            return;
        }
        else if(choice==1)
        {
            viewProd();
            cout<<"Product ID generated now Enter Product count of the new product\n";
            int c;
            cin>>c;
            auto cl=products[products.size()-1];
            string newProd=parseNext(cl.first);
            products.push_back({newProd,c});
            system("clear");
            cout<<"New Product Succesfully added with product id: "<<newProd<<endl<<endl;

        }
        else
        {
            cout<<"Invalid Choice, Choose again\n\n";
            manufacturer();
        }
    }
    void addNewDistributor()
    {
            viewDis();
            cout<<"Enter a distributer name\n";
            string name;
            cin.ignore();
            getline(cin,name);
            cout<<"Generating Distributor ID, please wait....\n";
            auto dis = distributors[distributors.size()-1];
            string prevDisID = dis.first;
            string newDisID = parseNext(prevDisID);
            
            cout<<"Distributor ID generated: "<<newDisID<<endl;
            cout<<"Press 1 and enter to acknowledge and send the security amount or press 2 and enter to abort\n";
            int choice;
            cin>>choice;
            if(choice==1)
            {
                distributors.push_back({newDisID,name});
                system("clear");
                cout<<"Distributor successfully added with:\n\n";
                cout<<"Distributor ID:"<<newDisID<<endl;
                cout<<"Distributor name:"<<name<<endl<<endl;
                cout<<"Please wait..\n\n";
                sleep(2);
                system("clear");
                return;
            }
            cout<<"Aborted\n";
            cout<<"Please wait\n";
            sleep(2);
            return;

    }

    void updateProductCount()
    {
        viewProd();
        cout<<"Enter the product ID whose count you want to update (case sensitive)\n";
        string id;
        cin>>id;

        for(int i=0;i<products.size();i++)
        {
            if(products[i].first==id)
            {
                cout<<"Enter the count of the product\n";
                int cnt;
                cin>>cnt;
                products[i].second=cnt;

                cout<<"Count updated successfully\n";
                cout<<"Please wait...\n";
                sleep(2);
                system("clear");
                return ;
            }
        }
        system("clear");
        cout<<"No such products found, please try again\n";
        cout<<"Please wait...\n\n";
        sleep(2);
    }

    void addNewClient()
    {
        system("clear");
        cout<<"Please wait, generating client ID\n";
        string newclID=parseNext(clients[clients.size()-1]);
        sleep(2);
        cout<<"Client ID generated: "<<newclID<<endl;
        cout<<"Press 1 and enter to acknowledge and send the security amount or press 2 and enter to abort\n";
        int choice;
        cin>>choice;
        
        if(choice==2)
        {
            cout<<"Aborted\n";
            cout<<"Please wait\n";
            sleep(2);
            system("clear");
            return;
        }

        clients.push_back(newclID);
        cout<<"Client successfully added\n\n";
        cout<<"Please wait...\n\n";
        sleep(2);
        return ;
    }

    void performTransaction()
    {
        cout<<"Enter your client ID\n";
        cin.ignore();
        string id;
        getline(cin,id);

        for(int i=0;i<clients.size();i++)
        {
            if(clients[i]==id)
            {
                system("clear");
                cout<<"Welcome "<<id<<"\n\n";
                cout<<"Your Previous Transaction(s) (Blank means no transaction has been done by you)\n\n";
                for(int m=0;m<allTxn.size();m++)
                {
                    if((allTxn[m]->clientID)==id)
                        allTxn[m]->viewTransaction();

                }


                viewProd();
                cout<<"Enter a product ID to begin the transaction\n";
                string pid;
                cin>>pid;
                for(int j=0;j<products.size();j++)
                {
                    if(products[j].first==pid)
                    {
                        if(products[j].second==0)
                        {
                            cout<<"Currently No stock of the product, returning...\n";
                            sleep(2);
                            system("clear");
                            return;
                        }
                        else
                        {
                            string t1=getTimestamp();
                            cout<<"Please wait while we select your distributor...\n\n";
                            sleep(2);
                            auto disID=getDisId();
                            cout<<"Distributor "<<disID.second<<" Will be your distributor for this transaction\n";
                            cout<<"Distributor ID: "<<disID.first<<endl<<endl;
                            cout<<"Please wait while the distributor dispatches your product\n\n";
                            sleep(2);
                            products[j].second--;
                            string t2=getTimestamp();
                            cout<<"Product delivered successfully \n\n";
                            cout<<"Please wait, while the transaction is being finalised...\n\n";
                            sleep(1);
                            string t3=getTimestamp();

                            transaction *txn=new transaction();
                            string txnID;

                            if(transaction_queue.size()==0)
                                txnID=parseNext(chain[chain.size()-1]->transactions[BLOCK_SIZE-1]->transactionID);
                            else
                                txnID=parseNext(transaction_queue[transaction_queue.size()-1]->transactionID);
                            
                            string data=t1+t2+t3+disID.first+txnID+id+pid;
                            txn->transactionHash=getHash(data);
                            txn->distributorID=disID.first;
                            txn->clientID=id;
                            txn->productID=pid;
                            txn->timestamp1=t1;
                            txn->timestamp2=t2;
                            txn->timestamp3=t3;
                            txn->transactionID=txnID;
                            allTxn.push_back(txn);
                            transaction_queue.push_back(txn);
                            cout<<"Transaction Complete\n\n";
                            txn->viewTransaction();
                            string gen="Transaction ID: "+txnID+"\n"+"Timestamp when Distributor got selected: "+t1+"\n";
                            gen+="Timestamp when distributor dispatched product: "+t2+"\n";
                            gen+="Timestamp when client recieved product: "+t3+"\n";
                            gen+="Product ID: "+pid+"\n";
                            gen+="Distributor ID: "+disID.first+"\n";
                            gen+="Client ID: "+id;

                            cout<<"Transaction QR Code:\n";
                            generateQR(gen);
                            cout<<"Please wait....\n";
                            sleep(2);
                            return;
                        }
                    }

                }
                cout<<"Invalid Product ID, Aborting....\n";
                sleep(2);
                system("clear");
                return;
            }
        }
        cout<<"Invalid Client ID\n";
        sleep(2);
        system("clear");
        return;
    }

    int PoETWaitTime()
    {
        cout<<"Initiating PoET Timer for "<<NODE<<" nodes. Maximum waiting time "<<(NODE*2)<<" seconds\n\n";
        // set seed for PoET wait time by sleeping for a random duration
        srand(time(0));
        int waitTimeSeconds = rand() % (NODE*2) + 1;  //  1 to no.of users*2
        sleep(waitTimeSeconds);
        return waitTimeSeconds/2;
    }

    void mine()
    {
        
        if(nodeQ!=-1 && transaction_queue.size()<BLOCK_SIZE)
        {
            cout<<"A node is already waiting in the queue\n";
            cout<<"It will begin mining once enough transactions are performed\n\n";
            return;
        }
        int n=-1;
        if(nodeQ==-1)//queue is empty
        {
            cout<<"New Nodes have joined and are willing to mine a block\n";
            cout<<"Mining will initiate if enough transactions are available\n";
            cout<<"Starting Consensus amongst the nodes\n";
            sleep(1);
            n=PoETWaitTime();
            cout<<"Node "<<n<<"'s wake timer has expired, Node "<<n<<" will attempt to mine a block\n";
        }
        if(transaction_queue.size()>=BLOCK_SIZE)
        {   
            if(nodeQ!=-1)//a node is waiting
            {
                cout<<"Node "<<nodeQ<<" will now start mining, Please wait...\n";
                sleep(2);
                block *b=new block();
                b->node=nodeQ;
                nodeQ=-1;
                b->previous_hash=chain[chain.size()-1]->blockHash;
                b->number=chain[chain.size()-1]->number+1;

                for(int i=0;i<BLOCK_SIZE;i++)
                {
                    (b->transactions).push_back(transaction_queue[0]);
                    transaction_queue.pop_back();
                }

                b->merkleNodehash=merkle(b);
                string data=to_string(b->number)+(b->merkleNodehash)+to_string((b->node));
                b->blockHash=getHash(data);
                chain.push_back(b);
                cout<<b->previous_hash;
                blockMap.emplace((b->blockHash),b);
                b->block_incentive=(b->node)*chain_size*BLOCK_SIZE;
                chain_size++;
                cout<<"Block Mined Successfully\n";
                b->displayBlock();
                PoETMaximal=b->transactions[BLOCK_SIZE-1]->timestamp3;
                rearBlock=b;
            }
            else
            {
                cout<<"Node "<<n<<" will now start mining, Please wait...\n";
                sleep(2);
                block *b=new block();
                b->node=n;

                b->previous_hash=chain[chain.size()-1]->blockHash;
                b->number=chain[chain.size()-1]->number+1;
                for(int i=0;i<BLOCK_SIZE;i++)
                {
                    (b->transactions).push_back(transaction_queue[0]);
                    transaction_queue.pop_back();
                }
                b->merkleNodehash=merkle(b);
                string data=to_string(b->number)+(b->merkleNodehash)+to_string((b->node));
                b->blockHash=getHash(data);
                cout<<"Block Mined Successfully\n";
                chain.push_back(b);
                cout<<b->previous_hash;
                blockMap.emplace((b->blockHash),b);
                chain_size++;
                PoETMaximal=b->transactions[BLOCK_SIZE-1]->timestamp3;
                b->block_incentive=(b->node)*chain_size*BLOCK_SIZE;
                b->displayBlock();
                rearBlock=b;
            }
        }
        else
        {
            cout<<"Not enough transactions\n";
            nodeQ=n;
            cout<<"Node will be added to the wait queue and will start mining when enough transactions are ready";
            return;
        }
    }

    void displayTxnQueue()
    {
        if(transaction_queue.size()==0)
        {
            cout<<"No Transactions in queue\n";
            return;
        }
        
        cout<<"Transactions Yet to be added in the blockchain\n";
        for(int i=0;i<transaction_queue.size();i++)
        {
            transaction_queue[i]->viewTransaction();
        }
    }
    bool trial()
    {
        int n=(NODE-transaction_queue.size()-2)*10;

        if(n<=0)
            return true;

        srand(time(nullptr));
        int check=rand()%n;
        if(check>=0 && check<=NODE-1)
            return true;
        return false;
    }

    void liar()
    {
        int liar;
        cout<<"Check the claim of:\n";
        cout<<"1. Distributor claims it has dispatched the product\n";
        cout<<"2. Client is claiming distributor has not dispatched the product\n";
        cin>>liar;

        if(liar==1)
        {
            string disID;
            string cliID;
            system("clear");
            cout<<"Enter the Distributor ID who raised the claim\n";
            cin>>disID;
            for(int i=0;i<distributors.size();i++)
            {
                if(distributors[i].first==disID)
                {
                    cout<<"Enter the client ID for which the transaction was made\n";
                    cin>>cliID;
                    for(int j=0;j<clients.size();j++)
                    {
                        if(clients[j]==cliID)
                        {
                            cout<<"Client and Distributor verified, they exists in the blockchain\n";
                            string tID;
                            cout<<"Enter the transaction ID \n";
                            cin>>tID;
                            cout<<"Please wait while transaction's validity is being checked\n";
                            sleep(1);

                            for(int b=0;b<transaction_queue.size();b++)
                            {
                                if(tID==(transaction_queue[b]->transactionID))
                                {
                                    cout<<"This transaction is has not been addded to the blockchain, validity cannot be confirmed\n";
                                    cout<<"The transaction will be added in the blockchain if "<<BLOCK_SIZE<<" transactions are in the queue. \n";
                                    cout<<"If enough transactions are available in the queue you can force the nodes to mine a block by selecting the 7th option in the main menu\n";
                                    cout<<"Currently there are "<<transaction_queue.size()<<" transactions\n";
                                    return;
                                }
                            }

                            for(int x=1;x<chain.size();x++)
                            {
                                for(int y=0;y<BLOCK_SIZE;y++)
                                {
                                    if(chain[x]->transactions[y]->clientID==cliID && chain[x]->transactions[y]->distributorID==disID)
                                    {
                                        if(chain[x]->transactions[y]->transactionID==tID)
                                        {
                                            cout<<"Distributor's claims are correct, The transaction was successfully completed\n";
                                            cout<<"Transaction Details\n";
                                            chain[x]->transactions[y]->viewTransaction();
                                            return;
                                        }
                                    }
                                }
                            }
                            cout<<"Distributor is lying\n";
                            cout<<"The transaction was never made/completed \n";
                            return;
                        }
                    }
                    cout<<"No such clients exists in the blockchain\n";
                    cout<<"Distributor is lying\n";
                    return;
                }
            }
            cout<<"No such distributor exists in the blockchain\n";
            cout<<"The person who claims a distributor is a fraud\n";
            return;
        }
        else if(liar==2)
        {
            string disID;
            string cliID;
            system("clear");
            cout<<"Enter the Client ID who raised the claim\n";
            cin>>cliID;
            for(int i=0;i<clients.size();i++)
            {
                if(clients[i]==cliID)
                {
                    cout<<"Enter the distributor from which the client was supposed to get the product\n";
                    cin>>disID;
                    for(int j=0;j<distributors.size();j++)
                    {
                        if(distributors[j].first==disID)
                        {
                            cout<<"Client and Distributor verified, Please enter the transaction ID\n";
                            string tid;
                            cin>>tid;

                            for(int b=0;b<transaction_queue.size();b++)
                            {
                                if(tid==(transaction_queue[b]->transactionID))
                                {
                                    cout<<"This transaction is has not been addded to the blockchain, validity cannot be confirmed\n";
                                    cout<<"The transaction will be added in the blockchain if "<<BLOCK_SIZE<<" transactions are in the queue. \n";
                                    cout<<"If enough transactions are available in the queue you can force the nodes to mine a block by selecting the 7th option in the main menu\n";
                                    cout<<"Currently there are "<<BLOCK_SIZE<<" transactions\n";
                                    return;
                                }
                            }

                            for(int k=1;k<chain.size();k++)
                            {
                                for(int l=0;l<BLOCK_SIZE;l++)
                                {
                                    if(chain[k]->transactions[l]->clientID==cliID && chain[k]->transactions[l]->distributorID==disID)
                                    {
                                        if(chain[k]->transactions[l]->transactionID==tid)
                                        {
                                            cout<<"Transaction found, the transaction was completed successfully\n";
                                            cout<<"Client is lying\n";
                                            cout<<"Transaction Details:\n;";
                                            chain[k]->transactions[l]->viewTransaction();
                                            return;
                                        }
                                    }
                                    
                                }
                            }
                            cout<<"Transaction was never made/completed successfully, Distributor is lying\n";
                            return;
                        }
                        
                    }   
                    cout<<"No such distributor found\n";
                    cout<<"Transaction was never made, client's claims are true\n";
                    return;
                }
            }
            cout<<"No such client ID found\n";
            cout<<"Client is an imposter\n";
            return;
        }
        else
        {
            cout<<"Invalid choice, try again\n";
            return;
        }

    }

    void diplayChain()
    {
        if(chain_size==1)
        {
            cout<<"No blocks added yet\n";
            return;
        }
        
        block *currBlock=rearBlock;
        
        for(int i=0;i<chain_size-1;i++)//skip genesis block
        {
            currBlock->displayBlock();
            currBlock=blockMap[currBlock->previous_hash];
        }
        
    }
    bool verifyTransaction(transaction *t)
    {
        long long t3 = stoi(t->timestamp3);
        long long maxi = stoi(PoETMaximal);

        return t3<maxi;
    }
};

int main()
{
    string inp="a";

    blockchain *bc =  new blockchain();
    while(true)
    {

        cout<<"\n\nChoose an option\n";
        cout<<"1. Add More Products as a Manufacterer\n";
        cout<<"2. Update Count of existing products as a Manufacterer\n";
        cout<<"3. Reigster yourself as a new client\n";
        cout<<"4. Register yourself as a Distributor\n";
        cout<<"5. Login as Client and Perform a transaction\n";
        cout<<"6. View transactions which are yet to be added in the blokchain\n";
        cout<<"7. Force a block to be Mined\n";
        cout<<"8. Display the blockchain\n";
        cout<<"9. Liar Checker\n";
        cout<<"10. Clear Screen\n";
        cout<<"11. Exit\n\n";
        cin>>inp;

        if(inp=="1")
        {
            system("clear");
            bc->manufacturer();
        }
        else if(inp=="2")
        {
            system("clear");
            bc->updateProductCount();
        }
        else if(inp=="3")
        {
            system("clear");
            bc->addNewClient();
        }
        else if(inp=="4")
        {
            system("clear");
            bc->addNewDistributor();
        }
        else if(inp=="5")
        {
            system("clear");
            bc->performTransaction();
            if((bc->trial() && (bc->nodeQ)==-1) || (bc->nodeQ!=-1 && bc->transaction_queue.size()==BLOCK_SIZE))
                bc->mine();
        }
        else if(inp=="6")
        {
            system("clear");
            bc->displayTxnQueue();
        }
        else if(inp=="7")
        {
            system("clear");
            bc->mine();
        }
        else if(inp=="8")
        {
            system("clear");
            bc->diplayChain();
        }
        else if(inp=="9")
        {
            system("clear");
            bc->liar();
        }
        else if(inp=="10")
        {
            system("clear");
            continue;
        }
        else if(inp=="11")
        {
            exit(EXIT_SUCCESS);
        }
        else
        {
            cout<<"Invalid option try again\n";
            continue;
        }
    }

}