int func(){
    return 1;
}

int main(){
    volatile unsigned int i = 0;
    while(i < 4000){
        i+= func();

    }
    return i;
}