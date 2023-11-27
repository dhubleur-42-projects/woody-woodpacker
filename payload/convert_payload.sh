rm -f payload64_C
nasm -f bin -o payload payload64.s
size=$(wc -c < payload)
echo "Payload 64bits uses $size bytes"
xxd -i -c 8 < payload > payload_c
rm payload
sed -i 's/0x\([0-9a-f][0-9a-f]\)/\\x\1/g' payload_c
tr -d '\n' < payload_c > payload_c2
mv payload_c2 payload_c
tr -d ' ' < payload_c > payload_c2
mv payload_c2 payload_c
tr -d ',' < payload_c > payload_c2
mv payload_c2 payload_c
sed -i 's/\\xe9\\x[0-9a-f][0-9a-f]\\x[0-9a-f][0-9a-f]\\x[0-9a-f][0-9a-f]\\x[0-9a-f][0-9a-f]/\n\n&\n\n/g' payload_c
mv payload_c payload64_C

nasm -f bin -o payload payload32.s
size=$(wc -c < payload)
echo "Payload 32bits uses $size bytes"
xxd -i -c 8 < payload > payload_c
rm payload
sed -i 's/0x\([0-9a-f][0-9a-f]\)/\\x\1/g' payload_c
tr -d '\n' < payload_c > payload_c2
mv payload_c2 payload_c
tr -d ' ' < payload_c > payload_c2
mv payload_c2 payload_c
tr -d ',' < payload_c > payload_c2
mv payload_c2 payload_c
sed -i 's/\\xe9\\x[0-9a-f][0-9a-f]\\x[0-9a-f][0-9a-f]\\x[0-9a-f][0-9a-f]\\xff/\n\n&\n\n/g' payload_c
mv payload_c payload32_C
