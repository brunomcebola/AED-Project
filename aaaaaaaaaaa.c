/*
*
*           TODO: put flag for high or low season
*
*/


int readLayout() {
    char *tabuleiro = getBoardLayout(), c = '\0', *buffer = getBoardBuffer();
    int sum_tents = 0, trees = 0, i = 0,
    rows = getBoardRows(), columns = getBoardColumns(), j;

    tabuleiro = "";

    for (i = 0; i < rows; i++) {

        fscanf(in_file, " %s", buffer);

        for (j = 0; i < columns; i++) {
            if (buffer[i] == 'A') {
                trees++;
            }
        }
        strcat(tabuleiro, buffer);
    }



    return 1;
}
