rm -f payload_c
nasm -f bin -o payload payload.s
size=$(wc -c < payload)
echo "Payload uses $size bytes"
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
echo 'Converted payload'