int readLayout() {
    char *tabuleiro = getBoardLayout(), c = '\0';
    int sum_tents = 0, trees = 0, linha_atual = 0, coluna_atual = 0,
    rows = getBoardRows(), columns = getBoardColumns(), tents_row = 0, *tents_column = NULL, j;


    sum_tents = getBoardSum();

    //save the actual number of tents in each column
    tents_column = (int *) calloc((unsigned int)columns , sizeof(int));
    checkNull(tents_column);


    free(tents_column);


    return 1;
}
