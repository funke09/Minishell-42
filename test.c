int main()
{
    char c = 'a'; // @1

    // printf("c value => %c\n", c);
    // printf("c adress => %p\n", &c);

    char *p; 

    p = &c; // p hold @1 | &p @2
    // printf("p value => %c\n", *p);
    // printf("c address => %p\n", p);
    // printf("p adress => %p\n", &p);


    char **pp;

    pp = &p; // pp hold @2 | &pp @3
    printf("pp value => %c\n", **pp); // @ pp = @2 || *pp = @1 || **pp = 'a'
    // printf("p adress => %p\n", pp);
    // printf("pp address => %p\n", &pp);
    // printf("pp value => %c\n", **pp);
    return(0);
}