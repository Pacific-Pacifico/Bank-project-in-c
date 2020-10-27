#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "customer.c"
#include "date.c"

// function to creates an account
void create_account();
// Checking whether the user is legitimate, check whether user is authorized or not
struct customer *login();
// for the account balance check, make fund transfer,withdraw and update balance
void transaction(struct customer *);
// Details about customer, account_number,name,balance,account_active
void print_details(struct customer);
/* return pointer to the block which contains the required cutomer block by using account no.
    , returns null if it doesn't find the respective block*/
struct customer *search(unsigned int);
// returns the last entered account number in the file "record.dat"
int previous_accno();
// adding money to the account
void deposit(struct customer *,double);
// withdawing money from account
void withdraw(struct customer *,double);
// to transfer fund from cuurent account to the another account
void fund_transfer(struct customer *);
// menu for admin
void admin();
// reads contains from the admin file and display on the console
void admin_file();
// to activate or deactivate an account
void set_active(struct customer *,int);
void clear_screen();
//int first_account();
void mask_pass(char []);


FILE *fp=NULL,*afp=NULL;
struct customer c1;
struct customer *cptr=NULL;
long f_pos;
char s_year[20],s_month[20];
char admin_filename[50];

int main()
{
    int option;
    char admin_pass[]="admin";
    char entered_pass[10];

    int year=get_year();

    sprintf(s_year, "%d", year);
//    to modify comment above line, uncomment below line and add any year in place of 2021
   // sprintf(s_year, "%d", 2021);

    int check=mkdir(s_year);
    if(check)
    {
        printf("\ndirectory already exists");
    }

//    sprintf(s_month, "%d", 11);
//  to modify uncomment above line, comment below line and add any month in place of 11
    sprintf(s_month, "%d", get_month());

    sprintf(admin_filename, "%s/%s.txt", s_year,s_month);

    while(1)
    {
        //clear();
        printf("\n*****Authentication system program*****\n");
        printf("\nSelect the required option:");
        printf("\n1.create account");
        printf("\n2.Login");
        printf("\n3.Admin login");
        printf("\n4.exit\n");
        scanf("%d",&option);
        switch(option)
        {
            case 1:
                create_account();
                clear_screen();
                break;

            case 2:
                cptr=login();
                if(cptr!=NULL)
                {
                    printf("\nLogin Successful!!\n");
                    transaction(cptr);
                }
                else
                {
                    printf("\n\nLogin Unsuccessful!!\n");
                }
                clear_screen();
                break;

            case 3:
//                printf("\nEnter password for admin login=");
                mask_pass(entered_pass);
//                scanf("%s",entered_pass);
                if(strcmp(admin_pass,entered_pass)!=0)
                {
                    printf("\nWrong password..");
                    exit(1);
                }
                else
                {
                    printf("\nWelcome Admin\n");
                    admin();
                }
                clear_screen();
                break;

            case 4:
                printf("\nThank You!\n");
                clear_screen();
                exit(1);
                break;

//            case 4:
//                printf("\n%d",previous_accno());
//                break;

            default:
                printf("\nInvalid input!");
        }
    }
    printf("\nThank You!\n");
    return 0;
}

void admin()
{
    int option,acc_no;
    struct customer *ptr=NULL;
    while(1)
    {
        printf("\nSelect required option:");
        printf("\n1.View unauthorised records file");
        printf("\n2.Activate an account");
        printf("\n3.go back to main menu");
        printf("\n4.exit\n");
        scanf("%d",&option);
        switch(option)
        {
            case 1:
                admin_file();
                break;

            case 2:
                printf("\nEnter account number to activate=");
                scanf("%u",&acc_no);

                ptr=search(acc_no);
                if(ptr==NULL)
                {
                    printf("\nEntered account  number not found\nExiting..");
                    exit(6);
                }
                else
                {
                    set_active(ptr,1);
                    printf("\nAccount number %u successfully activated\n",acc_no);
                }
                break;

            case 3:
                return;
                break;

            case 4:
                exit(6);
                break;

            default:
                printf("\nInvalid input");
        }
    }
}


void admin_file()
{
    int option,m,y;
    char data[100],a_filename[50];
    printf("\nSelect the required option");
    printf("\n1.see latest report");
    printf("\n2.see another previous record");
    printf("\npress any key to go back");
    scanf("%d",&option);
    if(option==1)
    {
        strcpy(a_filename,admin_filename);
    }
    else if(option==2)
    {
        printf("\nEnter year=");
        scanf("%d",&y);
        printf("\nEnter month number(1 to 12)=");
        scanf("%d",&m);
        sprintf(a_filename, "%d/%d.txt", y, m);
    }
    else
    {
        return;
    }
    printf("\n\n filename-->%s",a_filename);
    afp=fopen(a_filename,"r");
    if(afp==NULL)
    {
        printf("\nError opening admin file..");
        return;
    }
    printf("\n------------------------------\n");
    while(fgets(data,100,afp)!=NULL)
    {
        puts(data);
    }
    fclose(afp);
    printf("\n------------------------------\n");
}

int previous_accno()
{
    int pacc_no=-1;
    struct customer c1;
    fp=fopen("record.dat","rb");
    if(fp==NULL)
    {
        return -1;
    }
    fseek(fp,-(sizeof(c1)),SEEK_END);
    if(fread(&c1,sizeof(c1),1,fp)==1)
    {
        pacc_no=c1.acc_no;
    }
    fclose(fp);
    return pacc_no;
}

void create_account()
{
    struct customer c1;
    char name[50],pass[15];
    int acc_no=previous_accno()+1;
    if(acc_no==0)
    {
        acc_no=1001;
    }
   /* if(first_account())
    {
        int year=get_year();
        sprintf(s_year, "%d", year);
        int check=mkdir(s_year);
        if(!check)
        {
            printf("\ndirectory created");
        }
    }*/
    fp=fopen("record.dat","a+b");
    if(fp==NULL)
    {
        printf("\nfile open error!");
        exit(2);
    }
    c1.acc_no=acc_no;
//    c1.acc_bal=1000;
    printf("\nEnter name=");
    while(getchar()!='\n');
    gets(name);
    strcpy(&c1.acc_name,name);
    printf("\nEnter password=");
    gets(pass);
    strcpy(&c1.acc_pass,pass);

    printf("\nEnter amount deposited=");
    scanf("%lf",&c1.acc_bal);

    c1.acc_active=1;
    fwrite(&c1,sizeof(c1),1,fp);
    printf("\nAccount successfully created!");
    fclose(fp);
    print_details(c1);
}

struct customer *login()
{
    static int attempts=3;
    struct customer *cptr=NULL;
    int acc_no;
    char pass[15];
    printf("\nEnter account number=");
    scanf("%u",&acc_no);
//    printf("\nEnter password=");
//    while(getchar()!='\n');
//    gets(pass);
    mask_pass(pass);
    cptr=search(acc_no);
    if(cptr==NULL)
    {
//        attempts=3;
        return NULL;
    }
    if(cptr->acc_active==0)
    {
        printf("\n\nAccount inactive!! Please contact admin to activate");
        return NULL;
    }
    else
    {
        if((strcmp(cptr->acc_pass,pass)==0))
        {
            attempts=3;
            return cptr;
        }
        else
        {
            attempts--;
             if(attempts==0)
            {
                set_active(cptr,0);
                return NULL;
            }
            printf("\n remaining attempts=%d",attempts);

            printf("\n\nFilename=%s",admin_filename);
            afp=fopen(admin_filename,"a");
            char *str=get_date();
            fprintf(afp,"%s -> wrong login attempt for account number=%u\n",str,acc_no);
            fclose(afp);
            return NULL;
        }
    }
    return NULL;
}

void set_active(struct customer *cptr,int value)
{
    struct customer c1,c2;
    strcpy(c1.acc_name,cptr->acc_name);
    c1.acc_no=cptr->acc_no;
    c1.acc_bal=cptr->acc_bal;
    strcpy(c1.acc_pass,cptr->acc_pass);
    c1.acc_active=value;
    printf("\nacc_no=%u",cptr->acc_no);

    FILE *fr=fopen("record.dat","rb");
    FILE *fw=fopen("temp.dat","wb");
    if(fr==NULL)
    {
        if(fw==NULL)
        {
            printf("\nerror opening file!");
            exit(3);
        }
    }
    while(ftell(fr)<(f_pos-sizeof(c1)))
    {
        fread(&c2,sizeof(c2),1,fr);
        fwrite(&c2,sizeof(c2),1,fw);
    }
     printf("\n#ftell@%ld",ftell(fr));
    fwrite(&c1,sizeof(c1),1,fw);
    fseek(fr,sizeof(c1),1);
    printf("\n##ftell@%ld",ftell(fr));
    while(fread(&c2,sizeof(c2),1,fr)==1)
    {
        fwrite(&c2,sizeof(c2),1,fw);
    }
    fclose(fr);
    fclose(fw);
    remove("record.dat");
    rename("temp.dat","record.dat");
}

void transaction(struct customer *cptr)
{
    int option;
    double amount;
    while(1)
    {
        printf("\n\n---fpos@ %ld",f_pos);
        printf("\n1.check account balance");
        printf("\n2.make fund transfer");
        printf("\n3.deposit");
        printf("\n4.withdraw");
        printf("\n5.go to main menu");
        printf("\n6.exit\n");
        scanf("%d",&option);
        switch(option)
        {
            case 1:
                print_details(*cptr);
                break;

            case 2:
                fund_transfer(cptr);
                return;
                break;

            case 3:
                printf("\nEnter amount to deposit=");
                scanf("%lf",&amount);
                deposit(cptr,amount);
                break;

            case 4:
                printf("\nEnter amount to withdraw=");
                scanf("%lf",&amount);
                withdraw(cptr,amount);
                break;

            case 5:
                return;
                break;

            case 6:
                exit(1);
                break;

            default:
                printf("\nInvalid entry");
        }
    }
}


struct customer *search(unsigned int acc_no)
{
    struct customer *cptr=NULL;
    fp=fopen("record.dat","rb");
    if(fread(&c1,sizeof(c1),1,fp)!=1)
    {
        return cptr;
    }
    f_pos=ftell(fp);
    printf("\nftell at beg=%ld",f_pos);
    while(c1.acc_no!=acc_no)
    {
        if(fread(&c1,sizeof(c1),1,fp)!=1)
        {
            return cptr;
        }
        f_pos=ftell(fp);
        printf("\nftell at=%ld",f_pos);
    }
    fclose(fp);
    cptr=&c1;
    return cptr;
}

void print_details(struct customer c1)
{
    struct customer *cptr=NULL;
    cptr=search(c1.acc_no);
    printf("\n********************\n");
    printf("\naccount number= %u",cptr->acc_no);
    printf("\nName=");
    puts(cptr->acc_name);
    printf("\nBalance= %lf ",cptr->acc_bal);
    printf("\n********************\n");
//    printf("\n");
}

void deposit(struct customer *cptr,double amount)
{
    struct customer c1,c2;
    strcpy(c1.acc_name,cptr->acc_name);
    c1.acc_no=cptr->acc_no;
    c1.acc_bal=cptr->acc_bal;
    strcpy(c1.acc_pass,cptr->acc_pass);
    c1.acc_active=cptr->acc_active;
    printf("\nacc_no=%u",cptr->acc_no);
    c1.acc_bal+=amount;

    FILE *fr=fopen("record.dat","rb");
    FILE *fw=fopen("temp.dat","wb");
    if(fr==NULL)
    {
        if(fw==NULL)
        {
            printf("\nerror opening file!");
            exit(3);
        }
    }
    while(ftell(fr)<(f_pos-sizeof(c1)))
    {
        fread(&c2,sizeof(c2),1,fr);
        fwrite(&c2,sizeof(c2),1,fw);
    }
     printf("\n#ftell@%ld",ftell(fr));
    fwrite(&c1,sizeof(c1),1,fw);
    fseek(fr,sizeof(c1),1);
    printf("\n##ftell@%ld",ftell(fr));
    while(fread(&c2,sizeof(c2),1,fr)==1)
    {
        fwrite(&c2,sizeof(c2),1,fw);
    }
    fclose(fr);
    fclose(fw);
    remove("record.dat");
    rename("temp.dat","record.dat");
}

void withdraw(struct customer *cptr,double amount)
{
    struct customer c1,c2;
    strcpy(c1.acc_name,cptr->acc_name);
    c1.acc_no=cptr->acc_no;
    c1.acc_bal=cptr->acc_bal;
    strcpy(c1.acc_pass,cptr->acc_pass);
    c1.acc_active=cptr->acc_active;
    printf("\nacc_no=%u",cptr->acc_no);

    if(c1.acc_bal>=amount)
    {
        c1.acc_bal-=amount;
    }
    else
    {
        printf("\nNot enough balance");
        return;
    }

    FILE *fr=fopen("record.dat","rb");
    FILE *fw=fopen("temp.dat","wb");
    if(fr==NULL)
    {
        if(fw==NULL)
        {
            printf("\nerror opening file!");
            exit(3);
        }
    }
    while(ftell(fr)<(f_pos-sizeof(c1)))
    {
        fread(&c2,sizeof(c2),1,fr);
        fwrite(&c2,sizeof(c2),1,fw);
    }
     printf("\n#ftell@%ld",ftell(fr));
    fwrite(&c1,sizeof(c1),1,fw);
    fseek(fr,sizeof(c1),1);
    printf("\n##ftell@%ld",ftell(fr));
    while(fread(&c2,sizeof(c2),1,fr)==1)
    {
        fwrite(&c2,sizeof(c2),1,fw);
    }
    fclose(fr);
    fclose(fw);
    remove("record.dat");
    rename("temp.dat","record.dat");
}

void fund_transfer(struct customer *cptr)
{
    double amount;
    unsigned int tacc_no;
    struct customer *tcptr=NULL;
    printf("\nEnter amount to transfer=");
    scanf("%lf",&amount);
    printf("\n\nEnter account number to which you want to transfer money=");
    scanf("%ud",&tacc_no);
    withdraw(cptr,amount);
    tcptr=search(tacc_no);
    if(tcptr==NULL)
    {
        printf("\nThe destination account does not exists!!");
        deposit(cptr,amount);
        return;
    }
    deposit(tcptr,amount);
    printf("\nFund transfer successful..exiting");
}

void clear()
{
    #if defined(__linux__) || defined(__unix__) || defined(__APPLE__)
    system("clear");
    #endif
    #if defined(_WIN32) || defined(_WIN64)
    system("cls");
     #endif
 }


 int first_account()
 {
     FILE *fr=fopen("record.dat","rb");
     if(fr==NULL)
     {
         return 1;
     }
     return 0;
 }

 void clear_screen()
 {

      char flag;

    printf("Do you want to clear the screen Y/y or N");
    flag = getche(); // It waits for keyboard input.
    if (flag == 'Y' || flag == 'y' )
    {
       clear();
    }
 }

 void mask_pass(char password[])
 {
	printf("password:\n");
	int p=0;
	do{
		password[p]=getch();
		if(password[p]!='\r'){
			printf("*");
		}
		p++;
	}while(password[p-1]!='\r');
	password[p-1]='\0';
 }
