//
// Created by 13561 on 2024/11/21.
//
#include<stdio.h>
#include<stdlib.h>

#define ERROR   0
#define TRUE    1
#define OK      1

typedef int Status;

typedef struct {
    char *ch;
    int  length;
} HString;



Status StrAssign(HString *S, const char *chars);
Status StrCompare(HString S, HString T);
Status StrLength(HString S);
Status Concat(HString *S, HString T1, HString T2);
Status SubString(HString *sub, HString S, int pos, int len);
Status StrTraverse(HString S);
Status StrCopy(HString *S, HString T);
Status Index(HString S, HString T, int pos);
Status Replace(HString *S, HString T1, HString T2);
Status StrInsert(HString *S, int pos, HString T);
Status StrDelete(HString *S, int pos, int len);


Status StrAssign(HString *S, const char *chars) {

    /*

    if(S->ch != NULL) {
        printf("About to free memory.\n");
        free(S->ch);
        printf("Memory freed.\n");
        S->ch = NULL;
    }
    */


    int len = 0;
    while(chars[len]!='\0') {
        len++;
    }

    // ?????????
    S->ch = (char*)malloc(sizeof(char) * len);
    if(S->ch == NULL) {
        return ERROR;
    }


    for(int i = 0;i<len;i++) {
        S->ch[i] = chars[i];
    }
    // ??????????????
    S->length = len;

    return OK;
}


Status StrCompare(HString S,HString T) {


    int minlen = (S.length < T.length) ? S.length:T.length;
    int i = 0;

    while(i<minlen) {
        if(S.ch[i]>T.ch[i]) {
            return 1;
        }else if(S.ch[i]<T.ch[i]) {
            return -1;
        }else {
            i++;
        }
    }


    if(S.length>T.length) {
        return 1;
    }else if(S.length < T.length) {
        return -1;
    }else {
        return 0;
    }
}


Status StrLength(HString S) {
    return S.length;
}


Status Concat(HString *S,HString T1,HString T2) {

    /*
    if (S->ch!= NULL) {
        free(S->ch);
        S->ch = NULL;
    }
    */

    // ???????????????
    int newLength = T1.length + T2.length;

    // ?S????????????
    S->ch = (char *)malloc(newLength * sizeof(char));
    if (S->ch == NULL) {
        return ERROR;
    }

    // ??T1??????????S??
    for (int i = 0; i < T1.length; i++) {
        S->ch[i] = T1.ch[i];
    }

    // ??T2??????????S?????¶À??
    for (int j = 0; j < T2.length; j++) {
        S->ch[T1.length + j] = T2.ch[j];
    }

    // ????S???????
    S->length = newLength;

    return OK;
}


Status SubString(HString *sub,HString S,int pos , int len) {

    /*
    if (sub->ch!= NULL) {
        free(sub->ch);
        sub->ch = NULL;
    }
    */

    // ???pos??len??????
    printf("pos = %d,len = %d \n",pos,len);
    /* ?????
    if (pos < 1 || pos > S.length || len < 0 || len > S.length - pos + 1) {
        printf("ERROR: illegal pos or len\n");
        return ERROR;
    }
    */

    // ?sub?????????
    sub->ch = (char *)malloc(len * sizeof(char));
    if (sub->ch == NULL) {
        return ERROR;
    }
    //printf("sss\n");

    if(len==0) {
        sub->length=0;
        return OK;
    }
    // ??????????????????sub??
    for (int i = 0; i < len; i++) {
        sub->ch[i] = S.ch[pos - 1 + i];
    }
    //printf("wss\n");

    // ????sub?????
    sub->length = len;

    //printf("kess\n");

    return OK;
}



Status StrTraverse(HString S) {
    for (int i = 0; i < S.length; i++) {
        printf("%c", S.ch[i]);
    }
    printf("\n");

    return OK;
}


Status StrCopy(HString *S, HString T) {

    /*
     if (S->ch!= NULL) {
        free(S->ch);
        S->ch = NULL;
    }
    */

    // ????Concat??????T????S
    HString empty;
    empty.ch = NULL;
    empty.length = 0;

    Status result = Concat(S, empty, T);

    return result;
}


// ?????T???S?ß÷?pos??????????????¶À??????T?????S??????????0??
// 1?? pos ??StrLength(S)??????SubString()??StrCompare()????
Status Index(HString S, HString T, int pos) {
    int i = pos - 1;
    int S_length = S.length;
    int T_length = T.length;

    while (i <= S_length - T_length) {
        HString sub;
        // ???S?ß’??i + 1?????????T_length?????
        Status result = SubString(&sub, S, i + 1, T_length);
        if (result == OK) {
            // ????????????T
            Status compare_result = StrCompare(sub, T);
            if (compare_result == 0) {
                free(sub.ch);
                return i + 1;
            }
            free(sub.ch);
        }
        i++;
    }

    return 0;
}


// ???S???pos???????????T??
//  1?? pos ??StrLength(S)+1??????SubString()??Concat()????
Status StrInsert(HString *S, int pos, HString T) {
    HString left;
    // ???S?ß’??1?????????pos - 1?????
    Status result = SubString(&left, *S, 1, pos - 1);
    printf("result1 = %d\n",result);

    if (result == OK) {
        HString right;
        // ???S?ß’??pos?????????S->length - pos + 1?????
        result = SubString(&right, *S, pos, (S->length) - pos + 1);
        printf("result2 = %d\n",result);

        if (result == OK) {
            // ??left??T??right????????????S
            //printf("ww77\n");
            Status concat_result = Concat(S, left, T);
            //printf("wwww\n");
            if (concat_result == OK) {
                //printf("wwww\n");
                concat_result = Concat(S, *S, right);
            }
            free(left.ch);
            free(right.ch);
            //printf("wwww\n");
        }

    }

    return OK;
}

// ??S???????ß÷????T1???T2???µµ
// ????SubString()??StrCompare()??Concat()????
Status Replace(HString *S, HString T1, HString T2) {
    int pos = 1;
    int S_length = S->length;

    while (pos <= S_length) {
        HString sub;
        // ???S?ß’??pos?????????T1.length?????
        Status result = SubString(&sub, *S, pos, T1.length);
        if (result == OK) {
            // ????????????T1
            Status compare_result = StrCompare(sub, T1);
            if (compare_result == 0) {
                // ???????ß÷?T1???
                HString left;
                Status left_result = SubString(&left, *S, 1, pos - 1);
                HString right;
                Status right_result = SubString(&right, *S, pos + T1.length, S_length - pos - T1.length + 1);
                if (left_result == OK && right_result == OK) {
                    // ??left??T2??right????????????S
                    Status concat_result = Concat(S, left, T2);
                    if (concat_result == OK) {
                        concat_result = Concat(S, *S, right);
                    }
                }
                free(sub.ch);
                free(left.ch);
                free(right.ch);
                // ????S??????pos???
                S_length = S->length;
                pos = pos + T2.length;
            } else {
                free(sub.ch);
                pos++;
            }
        } else {
            break;
        }
    }

    return OK;
}

// ?????S?ß’??pos??????????????len???????
//  1?? pos ??StrLength(S)-len+1??????SubString()??Concat()????
Status StrDelete(HString *S, int pos, int len) {
    HString left;
    // ???S?ß’??1?????????pos - 1?????
    Status result = SubString(&left, *S, 1, pos - 1);
    if (result == OK) {
        HString right;
        // ???S?ß’??pos + len?????????S->length - pos - len + 1?????
        result = SubString(&right, *S, pos + len, S->length - pos - len + 1);
        if (result == OK) {
            // ??left??right????????????S
            Status concat_result = Concat(S, left, right);
            free(left.ch);
            free(right.ch);
        }
    }

    return OK;
}




int main() {
    HString T1, T2, T3, S;
    // (1) ???????
    const char *str1 = "abc";
    const char *str2 = "def";
    const char *str3 = "ghi";
    
    printf("StrAssign\n");

    StrAssign(&T1, str1);
    StrAssign(&T2, str2);
    StrAssign(&T3, str3);

    printf("StrTraverse\n");

    printf("T1: ");
    StrTraverse(T1);
    printf("T2: ");
    StrTraverse(T2);
    printf("T3: ");
    StrTraverse(T3);

    printf("StrCopy\n");
    // (2) ????StrCopy()????T1??????S???????S?????
    StrCopy(&S, T1);
    printf("S after StrCopy: ");
    StrTraverse(S);

    printf("StrInsert\n");
    // (3) ????StrInsert()????T2????S??¶¬?????????S?????
    StrInsert(&S, StrLength(S) + 1, T2);
    printf("S after StrInsert: ");
    StrTraverse(S);

    printf("StrReplace\n");
    // (4) ????StrReplace()????S?ß÷?T1?ùI??T3???????S?????
    Replace(&S, T1, T3);
    printf("S after Replace: ");
    StrTraverse(S);

    printf("Index\n");
    // (5) ????Index()?????T2??S?ß÷?¶À????????¶À????
    int index = Index(S, T2, 1);
    printf("Index of T2 in S: %d\n", index);

    printf("StrDelete\n");
    // (6) ????StrDelete()???S?????T2????????????S????
    StrDelete(&S, index, StrLength(T2));
    printf("S after StrDelete: ");
    StrTraverse(S);

    return 0;
}