int factorial(int a) {
    if(a == 1) {
        return a;
    }
    return (a*factorial(a-1));
    
}

int main() {
    int x = 5;
    return factorial(x);
}
