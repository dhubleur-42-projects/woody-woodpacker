rm -f payload_style
nasm -f bin -o payload payload.s
xxd -i -c 8 < payload > payload_style
rm payload
sed -i 's/0x\([0-9a-f][0-9a-f]\)/\\x\1/g' payload_style
tr -d '\n' < payload_style > payload_style2
mv payload_style2 payload_style
tr -d ' ' < payload_style > payload_style2
mv payload_style2 payload_style
tr -d ',' < payload_style > payload_style2
mv payload_style2 payload_style
