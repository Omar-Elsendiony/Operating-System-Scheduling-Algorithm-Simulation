char* switchStr(char* str,int size){
    
    char * savedStr = str;
    char* str2 = malloc((size) * sizeof(char));
    for (int i = 0 ; i < size;i++){
        str2[i] = savedStr[size-1 - i];
        // printf("%c\n",str2[i]);
    }
    str2[size] = 0x00;
    // printf("%s",str2);
    // return switchStr(char* str,int size);
    return str2;
}

int checkString(char* fString, char* sString){
    int i = 0;
    while(1){
        if (fString[i] != sString[i]){
            return 0;
        }
        else{
            if(fString[i] == 0x00 && sString[i] == 0x00){
                break;
            }
        }
        i++;
    }
    return 1;
}
char* intTochar(int num){
    int i = 0;
    int rem;
    int numTemp = num;
    int size = 0;
    while(numTemp%10 || (numTemp/10 != 0)){
        size++;
        numTemp /= 10;
    }
    char* str = malloc((size+1)*sizeof(char));
    while (num % 10 || (num/10 != 0)){
        rem = num % 10;
        num /= 10;
        char c = rem;
        c += '0';
        str[i] = c;
        i++;
    }
    str[i] = 0x00;
    // char * str2 = str;
    // printf("%d\n",i);
    return switchStr(str,i);
    // return str2;
}

