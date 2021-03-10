#include "my_bsq.h"

int my_min(int num, ...)
{
    va_list ap;
    int i = 0;
    int min = 0;
    va_start(ap, num);
    if (num >= 1) {
        min = va_arg(ap, int);
        i++;
    }
    while (i < num) {
        int tmp = va_arg(ap, int);
        if (tmp < min) {
            min = tmp;
        }
        i++;
    }
    va_end(ap);
    return min;
}

int check_around(int* prev, int* current, int pos_counter, char value) {
    int x1 = 0, x2 = 0, x3 = 0;

    if (value == 0) {
        return 0;
    }

    if (pos_counter - 1 >= 0) {
        if (prev[pos_counter - 1]) {
            x1 = prev[pos_counter - 1];
        }
        if (current[pos_counter - 1]) {
            x2 = current[pos_counter - 1];
        }
    }

    if (prev[pos_counter]) {
        x3 = prev[pos_counter];
    }

    return my_min(3, x1, x2, x3) + 1;
}



t_pos* method(t_map_char* input, t_pos* best, int fd) {
    
    // read first row
    input->field = malloc(sizeof(char*) * 2);
    char* tmp = my_readline(fd);
    input->field[1] = fill_bit_line(input->field[1], tmp);
    input->x = my_strlen(tmp);
    free(tmp);
    //end of first row

    int* prev = malloc(sizeof(int) * input->x);
    for (int i = 0; i < input->y; i++) {
        prev[i] = 0;
    }
    best->value = 0;
    best->x = 0;
    best->y = 0;

    // printf("\nmethod!\n");
    for (int row_counter = 0; row_counter < input->y; row_counter++) {
        int* current;
        if (row_counter != 0) {
            current = malloc(sizeof(int) * input->x);
            char* tmp = my_readline(fd);
            input->field[1] = fill_bit_line(input->field[1], tmp);
            free(tmp);
        }
        else {
            current = prev;
        }

        for (int pos_counter = 0; pos_counter < input->x; pos_counter++) {
            int pos_byte = pos_counter / 4;
            int pos_bit = pos_counter % 4;
            char value = bit_field_read(2*pos_bit, 2, input->field[1][pos_byte]);
            current[pos_counter] = check_around(prev, current, pos_counter, value);
            if (current[pos_counter] > best->value) {
                best->y = row_counter;
                best->x = pos_counter;
                best->value = current[pos_counter];
            }
            // printf("%d ", current[pos_counter]);
        }
        if (row_counter != 0) {
            free(prev);
            free(input->field[0]);
            prev = current;
        }
        input->field[0] = input->field[1];
        // tmp = my_readline(fd);
        // input->field[1] = fill_bit_line(input->field[1], tmp);
        // free(tmp);
        // printf("\n");
    }
    free(input->field[0]);
    free(prev);
    return best;
}