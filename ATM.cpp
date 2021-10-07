// long has max value as 2147483637 hence we can not store account_no and phone_no more than this


#include<iostream>
#include<fstream>
#include<string.h>

using namespace std;

// count the digits of a number
short countDigits(long  num)
{
    short count1=0;

    while(num!=0)
    {
        count1++;
        num=num/10;
    }
    return count1;
}



// Details of the bank
class bank
{
    private:
        char bank_name[20];
        char ifsc_code[20];
        char branch[20];

    public:
        void details_of_bank()
        {
            strcpy(bank_name,"Bank of Balrampur");
            strcpy(ifsc_code,"BBU123098");
            strcpy(branch,"Utraula");
            cout<<"                                   Bank : "<<bank_name<<"\n                                   IFSC Code : "<<ifsc_code<<"\n                                   Branch : "<<branch;
        }
};


// Details of account holder and basic operations
class customer_bank : public bank
{
    private:
        char customer_name[30];
        long account_no;
        char address[30];
        long phone_no;
        char account_type[20];
        double balance;

    public:

        // Get the details of account holder
        void getDetails()
        {
            cin.ignore();
            cout<<"Customer Name--->";
            cin.getline(customer_name,20);

            acc:cout<<"Account Number--->";
            cin>>account_no;


            // Check Account number is 10 digits number or not
            if(countDigits(account_no)!=10)
            {
                cout<<"Account number Should be 10 digits\n";
                goto acc;
            }



            cin.ignore();
            cout<<"Address of customer--->";
            cin.getline(address,30);


            acc1:cout<<"Phone Number--->";
            cin>>phone_no;


            // Check phone number is 10 digits number or not
            if(countDigits(phone_no)!=10)
            {
                cout<<"Phone number Should be 10 digits\n";
                goto acc1;
            }

            cin.clear();
            cout<<"Account Type--->";
            cin.ignore();
            cin.getline(account_type,20);

            cout<<"Balance--->";
            cin>>balance;
        }

        // Display the details of account holder
        void putDetails()
        {
            cout<<customer_name<<" "<<account_no<<" "<<address<<" "<<phone_no<<" "<<account_type<<" "<<balance<<endl;
        }
        // Declaration of basic operations
        void account();
        void check_balance(long *);
        void withdraw(long *,double *);
        void deposit(long *,double *);
        void transfer_money(long *,long *,double *);
        void account_holder_list();
        void close_account(long *);
        void modify_account(long *);
};



// Create new account
void customer_bank::account()
{

    ofstream fout;
    fout.open("Bank1_Customer_Details.txt",ios::app|ios::binary);

    fout.write((char*)this,sizeof(*this));
    fout.close();
}

// Check your details
void customer_bank::check_balance(long *cb)
{
    ifstream fin;

    fin.open("Bank1_Customer_Details.txt",ios::in|ios::binary);

    // Check file exist or not
    if(!fin)
        cout<<"File not found\n";
    else
    {
        int count1=0;
        fin.read((char*)this,sizeof(*this));

        while(!fin.eof())
        {
            // Check the account number that we are searching for
            if(account_no==*cb)
            {
                count1++;
                putDetails();
                break;
            }
            fin.read((char*)this,sizeof(*this));
        }

        if(count1==0)
            cout<<*cb<<" is not exits";
    }
    fin.close();
}


// Withdraw money
void customer_bank::withdraw(long *cb,double *bal)
{
    int count1=0;
    fstream file;

    // open file into memory
    file.open("Bank1_Customer_Details.txt",ios::in|ios::out|ios::ate|ios::binary);
    file.seekg(0);    // representing first character of first record

    file.read((char *)this,sizeof(*this));
    while(!file.eof())
    {
        if(account_no==*cb)
        {
            if(balance>=*bal)
            {
                balance=balance-*bal;
                file.seekp(file.tellp()-sizeof(*this));  // pointer pointing whose account number is *cb
                file.write((char *)this,sizeof(*this));  // updated data writing into file
                count1++;                                // if withdraw has been done then count1=1

                cout<<"\nwithdraw successfully has been done";
                break;
            }
            else
            {
                cout<<"\nYou don't have enough balance";
            }
        }
        file.read((char *)this,sizeof(*this));
    }

    if(count1==0)
    {
        cout<<"\nThere is not such an account";
    }
    file.close();
}

// depositing Money
void customer_bank::deposit(long *cb,double *bal)
{
    int count1=0;
    fstream file;

    // open file into memory
    file.open("Bank1_Customer_Details.txt",ios::in|ios::out|ios::ate|ios::binary);
    file.seekg(0);    // representing first character of first record

    file.read((char *)this,sizeof(*this));
    while(!file.eof())
    {
        // Account number matched
        if(account_no==*cb)
        {
            balance=balance+*bal;   // withdraw ammount
            file.seekp(file.tellp()-sizeof(*this));  // pointer pointing whose account number is *cb
            file.write((char *)this,sizeof(*this));  // updated data writing into file
            count1++;
            cout<<"\nDeposit successfully has been done";
            break;
        }
        file.read((char *)this,sizeof(*this));
    }

    if(count1==0)
    {
        cout<<"\nThere is not such an account";
    }
    file.close();
}

// Transfer money
void customer_bank::transfer_money(long *cb,long *cb2,double *bal)
{
    int count1=0;

    fstream file;

    file.open("Bank1_Customer_Details.txt",ios::in|ios::out|ios::ate|ios::binary);
    file.seekg(0);


    // Check whether both account exits or not
    file.read((char *)this,sizeof(*this));
    while(!file.eof())
    {
        if(account_no==*cb || account_no==*cb2)
            count1++;
        if(count1==2)
            break;
        file.read((char *)this,sizeof(*this));
    }


    // both account exit
    if(count1==2)
    {

        file.seekg(0);  // pointer in file pointing first character of first record

        // Tranfer process are happening
        file.read((char *)this,sizeof(*this));
        while(!file.eof())
        {
            // first account from where transfer is being happend
            if(account_no==*cb)
            {
                if(balance>=*bal)
                {
                    balance=balance-*bal;
                    file.seekp(file.tellp()-sizeof(*this));
                    file.write((char *)this,sizeof(*this));
                }
            }
            // second account in which transfer is being happend
            if(account_no==*cb2)
            {
                    balance=balance+*bal;
                    file.seekp(file.tellp()-sizeof(*this));
                    file.write((char *)this,sizeof(*this));
            }

            file.read((char *)this,sizeof(*this));
        }

    }

    if(count1==0)
    {
        cout<<"\nBoth file aren't exit";
    }

    file.close();
}


// List of all account holder

void customer_bank::account_holder_list()
{
    int count1=0;
    ifstream fin;

    fin.open("Bank1_Customer_Details.txt",ios::in|ios::binary);

    if(!fin)
    {
        cout<<"File not found\n";
    }

    else
    {
        cout<<"******List of all Account Holder*******\n\n";

        fin.read((char *)this,sizeof(*this));
        while(!fin.eof())
        {
            count1++;
            putDetails();
            fin.read((char *)this,sizeof(*this));
        }
        fin.close();

        if(count1==0)
        {
            cout<<"No account Found\n\n";
        }
    }
}

void customer_bank::close_account(long *cb)
{
    int count1=0;
    ifstream fin;
    ofstream fout;
    fin.open("Bank1_Customer_Details.txt",ios::in|ios::binary);

    if(!fin)
        cout<<"\nFile is not found";
    else
    {
        fout.open("temp.txt",ios::out|ios::binary);
        fin.read((char *)this,sizeof(*this));
        while(!fin.eof())
        {

            if(account_no!=*cb)
            {
                fout.write((char *)this,sizeof(*this));
                count1++;
            }
            count1--;
            fin.read((char *)this,sizeof(*this));
        }
    }
    fout.close();
    fin.close();

    remove("Bank1_Customer_Details.txt");
    rename("temp.txt","Bank1_Customer_Details.txt");


    // Account not exist
    if(count1==0)
    {
        cout<<"\n Account not exits";
    }
    //Account exist
    else
    {
        cout<<"\nAccount successfully closed";
    }
}

void customer_bank::modify_account(long *cb)
{
    int choice;
    fstream file;

    // open file into RAM
    file.open("Bank1_Customer_Details.txt",ios::in|ios::out|ios::ate|ios::binary);
    file.seekg(0);

    file.read((char *)this,sizeof(*this));
    while(!file.eof())
    {
        if(account_no==*cb)
        {
            cout<<"\n1.Customer Name";
            cout<<"\n2.Customer's Address";
            cout<<"\n3.Phone Number";

            cout<<"\nwhat do you want to update";
            cin>>choice;

            switch(choice)
            {
                case 1:
                    cin.ignore();
                    cin.getline(customer_name,20);
                    break;

                case 2:
                    cin.ignore();
                    cin.getline(address,20);
                    break;

                case 3:
                    cin>>phone_no;
                    break;

                default:
                    exit(1);

            }

            file.seekp(file.tellp()-sizeof(*this));
            file.write((char *)this,sizeof(*this));
        }
        file.read((char *)this,sizeof(*this));
    }
    file.close();
}

int main()
{
    customer_bank c1;
    bank b1;
    int choice;
    char ch='y';


    while(ch=='y' || ch=='Y')
    {
        system("cls");
        b1.details_of_bank();
        cout<<"\n\n\n         Main Menu\n\n";
        cout<<"1.Create an account\n";
        cout<<"2.Check your balance\n";
        cout<<"3.Withdraw money\n";
        cout<<"4.Deposit money\n";
        cout<<"5.Transfer money in same bank\n";
        cout<<"6.All Account holder lists\n";
        cout<<"7.Close an Account\n";
        cout<<"8.Modify an account\n";
        cout<<"9.Exit\n\n";


        // Choose a task
        switch1:cout<<"Choose a task-->";
        cin>>choice;
        cout<<"\n";

        switch(choice)
        {
            case 1:
                cout<<"********Create new account********\n";
                c1.getDetails();
                c1.account();
                cout<<"Account successfully created";
                break;

            case 2:
                long cb;
                cout<<"********Check a particular Account Holder ********\n";
                cout<<"Enter an account number--->";
                cin>>cb;
                c1.check_balance(&cb);
                break;

            case 3:
                double bal;
                cout<<"********Withdraw an amount********\n\n";
                cout<<"Enter an account number--->";
                cin>>cb;
                cout<<"Enter amount to withdraw--->";
                cin>>bal;
                c1.withdraw(&cb,&bal);
                break;

            case 4:
                cout<<"********Deposit an amount********\n\n";
                cout<<"Enter an account number--->";
                cin>>cb;
                cout<<"Enter amount to deposit--->";
                cin>>bal;
                c1.deposit(&cb,&bal);
                break;

            case 5:
                long cb2;
                cout<<"********Transfer an amount********\n\n";
                cout<<"Enter an account number from which tranfer has to be--->";
                cin>>cb;
                cout<<"Enter an account number in which tranfer has to be--->";
                cin>>cb2;
                cout<<"Enter tranfer amount--->";
                cin>>bal;
                c1.transfer_money(&cb,&cb2,&bal);
                break;

            case 6:
                c1.account_holder_list();
                break;

            case 7:
                cout<<"*****Close an account******\n\n";
                cout<<"Enter account number that you want to close--->";
                cin>>cb;
                c1.close_account(&cb);
                break;

            case 8:
                cout<<"*****Modify an account******\n\n";
                cout<<"Enter account number that you want to modify--->";
                cin>>cb;
                c1.modify_account(&cb);
                break;

            case 9:
                exit(0);


            default:
                cout<<"Your choiced is wrong\n Try Again\n";
                goto switch1;
                break;
        }

        cout<<"\n\nDo you want to perform another operation press 'y' or 'Y' key--->";
        cin>>ch;
    }

    return 0;
}



