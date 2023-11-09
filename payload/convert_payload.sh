rm -f payload_c
nasm -f bin -o payload payload.s
xxd -i -c 8 < payload > payload_c
rm payload
sed -i 's/0x\([0-9a-f][0-9a-f]\)/\\x\1/g' payload_c
tr -d '\n' < payload_c > payload_c2
mv payload_c2 payload_c
tr -d ' ' < payload_c > payload_c2
mv payload_c2 payload_c
tr -d ',' < payload_c > payload_c2
mv payload_c2 payload_c
