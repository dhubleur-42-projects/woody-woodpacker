#!/bin/python3

################### GLOBALS #############################
TOKEN_MARKER = 137
LEN_TOKEN = 3 # TOKEN_MARKER + BACKLOOK + SIZE
################### END GLOBALS #########################

def find_largest_subbytes(data: list, i_begin: int):
    best_i_subbyte = -1
    best_length_subbyte = 0

    i_search_haystack = max(0, i_begin - 255)

    while i_search_haystack < i_begin:
        i_haystack = i_search_haystack
        i_needle = i_begin
        while i_haystack < i_begin and i_needle < len(data) and data[i_haystack] == data[i_needle]:
            i_haystack += 1
            i_needle += 1

        cur_length_subbyte = i_haystack - i_search_haystack
        if cur_length_subbyte > best_length_subbyte:
            best_length_subbyte = cur_length_subbyte
            best_i_subbyte = i_search_haystack

        i_search_haystack += 1

    return best_i_subbyte, best_length_subbyte

with open('test.bin', 'rb') as file:
    data = file.read()

encoded_data = []

with open('test_bin.lzsw', 'wb') as file_lzsw:
    i = 0
    while i < len(data):
        i_subbyte, length_subbyte = find_largest_subbytes(data, i)
        if length_subbyte <= LEN_TOKEN:
            if data[i] == TOKEN_MARKER:
                file_lzsw.write(bytes([TOKEN_MARKER, 0]))
            else:
                file_lzsw.write(bytes([data[i]]))
            i += 1
        else:
            file_lzsw.write(bytes([TOKEN_MARKER, i - i_subbyte, length_subbyte]))
            i += length_subbyte
