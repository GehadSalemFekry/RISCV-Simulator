int strlen (char str[]) {
    int len = 0, i = 0;
    while(str[i] != '\0'){
        i++;
        len++;
    }
    return len;
}