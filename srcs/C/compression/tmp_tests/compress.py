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

with open('alice', 'rb') as file:
    data = file.read()

print("\tOriginal text...\n")
for i in range(len(data)):
    print(end=chr(data[i]))

encoded_data = []

print("\n\tEncoding...\n")
i = 0
while i < len(data):
    i_subbyte, length_subbyte = find_largest_subbytes(data, i)
    if length_subbyte <= LEN_TOKEN:
        if data[i] == TOKEN_MARKER:
            encoded_data.append(TOKEN_MARKER)
            encoded_data.append(0)
        else:
            encoded_data.append(data[i])
        print(end=chr(data[i]))
        i += 1
    else:
        encoded_data.append(TOKEN_MARKER)
        encoded_data.append(i - i_subbyte)
        encoded_data.append(length_subbyte)
        print("<" + str(i - i_subbyte) + ", " + str(length_subbyte) + ">", end="")
        i += length_subbyte

decoded_data = []
print("\n\tDecoding...\n")
i = 0;

while i < len(encoded_data):
    if encoded_data[i] != TOKEN_MARKER:
        decoded_data.append(encoded_data[i])
        i += 1
    else:
        i += 1
        if encoded_data[i] == 0:
            decoded_data.append(TOKEN_MARKER)
            i += 1
        else:
            decoded_data += decoded_data[len(decoded_data) - encoded_data[i]:len(decoded_data)-encoded_data[i]+encoded_data[i+1]]
            i += 2

for i in range(len(decoded_data)):
    print(end=chr(decoded_data[i]))

print("\n################## INFO #####################")
print("Length data:", len(data))
print("Length encoded:", len(encoded_data))
