#include <stdlib.h>
#include <stdio.h>
#include <ctype.h>
#include <sys/stat.h>
#include<time.h>
#include "my402list.h"
#include "cs402.h"
#include <string.h>

typedef struct transNode {
    char tag;
    time_t time;
    unsigned long num;
    char * tag_dsp;
} Node;

void BubbleForward(My402List *pList, My402ListElem **pp_elem1, My402ListElem **pp_elem2){
    My402ListElem *elem1=(*pp_elem1), *elem2=(*pp_elem2);
    void *obj1 = elem1->obj, *obj2 = elem2->obj;
    My402ListElem *elem1prev=My402ListPrev(pList, elem1);
    My402ListElem *elem2next=My402ListNext(pList, elem2);
    My402ListUnlink(pList, elem1);
    My402ListUnlink(pList, elem2);
    if (elem1prev == NULL) {
        (void)My402ListPrepend(pList, obj2);
        *pp_elem1 = My402ListFirst(pList);
//        printf("exchanged!\n");
    } else {
        (void)My402ListInsertAfter(pList, obj2, elem1prev);
        *pp_elem1 = My402ListNext(pList, elem1prev);
//        printf("exchanged!\n");
    }
    if (elem2next == NULL) {
        (void)My402ListAppend(pList, obj1);
        *pp_elem2 = My402ListLast(pList);
//        printf("exchanged!\n");
    } else {
        (void)My402ListInsertBefore(pList, obj1, elem2next);
        *pp_elem2 = My402ListPrev(pList, elem2next);
//        printf("exchanged!\n");
    }
}

void sortList(My402List * list){
    My402ListElem *elem=NULL;
    int i ;
    int len = My402ListLength(list);
    for(i = 0; i < len; i++){
        int j , something_swapped=FALSE;
        My402ListElem *next_elem = NULL;
        for (elem=My402ListFirst(list), j = 0; j < len-i-1; elem=next_elem, j++) {
            unsigned long cur_val=(unsigned long)(((Node*)elem->obj)->time), next_val=0;

            next_elem=My402ListNext(list, elem);
            next_val = (unsigned long)(((Node*)next_elem->obj)->time);

            if (cur_val > next_val) {
                BubbleForward(list, &elem, &next_elem);
                something_swapped = TRUE;
            }else if(cur_val == next_val){
                fprintf(stderr, "Error: Two transaction happens at the same time!");
                exit(0);
            }
        }
        if (!something_swapped) break;
    }
}


void readLine(char *buff, My402List * list){
    if(strlen(buff) > 1024){
        fprintf(stderr, "Error: the line is beyond 1024 bite");
        exit(0);
    }

    Node *node = (Node *)malloc(sizeof(Node));
    if(!node){
        fprintf(stderr, "Error: about initialization of Node");
        exit(0);
    }

    char * piece[] = {NULL, NULL, NULL, NULL, NULL};
    char * curPiece;
    char * de = "\t";
    int p = 0;
    piece[p ++] = strtok(buff, de);
    while((curPiece = strtok(NULL, de))){
        piece[p ++] = curPiece;
    }

    if(piece[4] != NULL){
        fprintf(stderr, "Error: There are more than 4 slices!");
                exit(0);
    }


    // type
    if(strcmp(piece[0], "+") == 0|| strcmp(piece[0], "-") == 0){
        char * tag = piece[0];
        node->tag = *tag;
    }else{
        fprintf(stderr, "Error: Unexpected input information!");
        exit(0);
    }
    //time
    if(strlen(piece[1]) == 0){
        fprintf(stderr, "Error: short timestamp!");
        exit(0);
    }else if(strlen(piece[1]) >= 11){
        fprintf(stderr, "Error: timeStamp is too long!");
        exit(0);
    }else{
        unsigned long oritime = atoi(piece[1]);
        unsigned long cur_time = time(NULL);
        if(cur_time < oritime){
            fprintf(stderr, "Error: The transaction time cannot beyond current time");
            exit(0);
        }
        node->time = oritime;
    }

    // trans num
    if(piece[2] == NULL ){
        fprintf(stderr, "Error: the transaction amount is NULL!");
        exit(0);
    }
    if(strlen(piece[2]) <=2){
        fprintf(stderr, "Error: the transaction amount is less than 1!");
        exit(0);
    }
    int amount = 0;
    char integer;
    char *dot ;
    dot = strchr(piece[2], '.');
    int k = 0;
    integer = (piece[2])[k];
    while(integer != '.'){
        if(isdigit(integer)){
            amount = amount*10 + atoi(&integer);
            integer = (piece[2])[++k];
        }else{
            fprintf(stderr, "Error: the transaction amount should be number!");
            exit(0);
        }
    }
    if(amount >= 10000000){
        fprintf(stderr, "Error: the transaction cannot be beyond 9,999,999!");
        exit(0);
    }
    char *end ;
    end = strchr(piece[2], '\0');
    if((end - dot) != 3){
        fprintf(stderr, "Error: the transaction amount should be with two decimal digits!");
        exit(0);
    }
    k ++;
    integer = (piece[2])[k];
    while(integer != *end){
        if(isdigit(integer)){
            amount = amount * 10 + atoi(&integer);
            integer = (piece[2])[++k];
        }else{
            fprintf(stderr, "Error: the transaction amount should be number!");
            exit(0);
        }
    }
    node->num = amount;

    // trans description

    if(piece[3] == NULL || strlen(piece[3]) == 0){
        fprintf(stderr, "Error: the transaction discription should not be empty!");
        exit(0);
    }
    char * discription = strdup(piece[3]);
    node->tag_dsp = discription;
    My402ListAppend(list,(void*)node);
    return ;
}

void printLine(){
    fprintf(stdout, "+-----------------+--------------------------+----------------+----------------+\n");
}
void printTitle(){
    fprintf(stdout, "|       Date      | Description              |         Amount |        Balance |\n");
}
long printNode(My402ListElem * elem, int balance){
    Node * curNode = elem->obj;
    time_t * data = &(curNode->time);
    char *time = ctime(data);
    char subtime[17];
    strncpy(subtime, time, 11);

    int i = 20;
    while(i < 23){
        subtime[i - 9] = time[i];
        i ++;
    }
    char last = atoi(&time[23]) + '0';
//    printf("%c", last);
    subtime[14] = last;
    subtime[15] = '\0';


    char subdsp[26];
    char * dsp = curNode->tag_dsp;
    int p =0;
    int len = strlen(dsp);
    if(len > 24) len = 24;
    while(p < len ){
        subdsp[p] = dsp[p];
           p ++;
    }
    while(p < 24){
        subdsp[p] = ' ';
        p ++;
    }
    subdsp[25] ='\0';


    char curNum[15];
    long num = curNode->num;
//    printf("%ld \n", num);
    int excessive = 0;
    if(num > 1000000000){
        excessive = 1;
    }
    long subnum;
    if(curNode->tag == '-'){
        balance = balance - num;
        subnum = 0 - num;
    }else{
        balance = balance + num;
        subnum = num;
    }

//    printf("%ld \n", num);
//    printf("%d \n", balance);
    if(excessive){
        if(subnum > 0){
            char *str1 = " ?,???,?";
            char *str2 = "??.?? ";
            strcpy(curNum, str1);
            strcat(curNum, str2);
        }

        else{
            char *str1 = "(?,???,?";
            char *str2 = "??.??";
            char *str3 = ")";
            strcpy(curNum, str1);
            strcat(curNum, str2);
            strcat(curNum, str3);
        }
    }else{
        if(subnum > 0){
            curNum[0] = ' ';
            curNum[13] = ' ';
        }else{
            curNum[0] = '(';
            curNum[13] = ')';
        }
        int p ;
        for(p = 12; p > 0; p --){
            if(num == 0){
                if(p < 9) break;
                else if(p == 10) curNum[p] = '.';
                else curNum[p] = '0';
            }
            if(p == 6 || p == 2){
                curNum[p] = ',';
            }else if(p == 10){
                curNum[p] = '.';
            }else{
                curNum[p] = '0'+(num % 10);
//                printf("%d %c \n", p, curNum[p]);
                num = num / 10;
            }
        }
        while (p > 0){
            curNum[p--] = ' ';
        }
    }

    curNum[14] = '\0';

    char curBal[15];
    long cpBal = balance;
    if(balance < 0) cpBal = (-1) * balance;
    int excessiveBalance = 0;
    if(cpBal > 1000000000){
        excessiveBalance = 1;
    }
    if(excessiveBalance){
        if(balance > 0){
            char *str1 = " ?,???,?";
            char *str2 = "??.?? ";
            strcpy(curBal, str1);
            strcat(curBal, str2);
        }

        else{
            char *str1 = "(?,???,?";
            char *str2 = "??.??";
            char *str3 = ")";
            strcpy(curBal, str1);
            strcat(curBal, str2);
            strcat(curBal, str3);
        }
    }else{
        if(balance > 0){
            curBal[0] = ' ';
            curBal[13] = ' ';
        }else{
            curBal[0] = '(';
            curBal[13] = ')';
        }
        int p ;
        for(p = 12; p > 0; p --){
            if(cpBal == 0){
                if(p < 9) break;
                else if(p == 10) curBal[p] = '.';
                else curBal[p] = '0';
            }
            if(p == 6 || p == 2){
                curBal[p] = ',';
            }else if(p == 10){
                curBal[p] = '.';
            }else{
                curBal[p] = '0'+cpBal%10;
                cpBal = cpBal / 10;
            }
        }
        while (p > 0){
            curBal[p--] = ' ';
        }
    }
    curBal[14] = '\0';

    fprintf(stdout, "| %s | %s | %s | %s |\n", subtime, subdsp, curNum, curBal);

    return balance;
}


void printList(My402List * list){
    printLine();
    printTitle();
    printLine();
    My402ListElem * curElem = NULL;
    curElem = My402ListFirst(list);

    long balance = 0;

    while(curElem != NULL){
        balance = printNode(curElem, balance);
        curElem = My402ListNext(list, curElem);
    }
    printLine();
    return;
}

void readFile(char * name){
    FILE *file = NULL;
    // check whether a given file name and file exist
    if(name == NULL){
        file = stdin;
        if(file == NULL){
            fprintf(stderr, "Error: 2. this file cannot be opened! Check and enter a correct file name");
            exit(0);
        }
    }
    else{
        struct stat fileInfo;
        stat(name, &fileInfo);
        if(S_ISDIR(fileInfo.st_mode)){
            fprintf(stderr, "Error: this is a directory! Check and enter a correct file name");
            exit(0);
        }
        if((file = fopen(name, "r"))== NULL){
            fprintf(stderr, "Error: this file cannot be opened! Check and enter a correct file name");
            exit(0);
        }
    }
    // read file line by line
    char buff[1026];

    My402List *list ;
    list = (My402List *) malloc(sizeof(My402List));
    (void) My402ListInit(list);
    int times = 0;
    while(fgets(buff, 1026, file) != NULL){
        times = times + 1;
        int bl = strlen(buff);
        if(bl <= 1){
            fprintf(stderr, "Error: one line of input file is unexpected!");
            exit(0);
        }
        if(buff[bl - 1] == '\n'){
            buff[bl - 1] = '\0';
        }
        readLine(buff, list);

    }

    if(times == 0){
        fprintf(stderr, "Error: no content in the file!");
        exit(0);
    }

        fclose(file);
        sortList(list);
        printList(list);
        return;


}

int  commandCheck(int argc, char * argv[]){
    if(argc < 2 || argc > 3){
        fprintf(stderr, "Error: wrong number commands please input : ./warmup1 sort [filename]!");
        exit(1);
    }else {
        if(strcmp(argv[1], "sort")){
            fprintf(stderr, "Error: <sort> wanted but this command cannot be recognized! ");
            exit(1);
        }
//        else if( (argc > 2 ) &&(strchr(argv[2], '.') != NULL || strchr(argv[2], '-') != NULL)){
//            fprintf(stderr, "Error: file name format is fault! ");
//            exit(1);
//        }
        readFile(argv[2]);
    }
    return 0;
}

int main(int argc, char ** argv){
    commandCheck(argc, argv);
    return 0;

}
