void insertSort(int a[], int length) {    
    int i, j;    
    for(i = 1; i < length; i++) {      
        int value = a[i];       
        for (j = i-1; j >= 0 && a[j] > value; j--) { 
            a[j+1] = a[j];       
        }       
        a[j+1] = value;   
    } 
}  