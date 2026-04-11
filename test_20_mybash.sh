#!/bin/bash
set -e

cd "$(dirname "$0")/exercises/20_mybash"

cat > /tmp/mybash_cmd_20.txt <<'EOF'
myfile ./bin/mybash
myfile ./obj/mybash/main.o
mysed s/unix/linux/ "unix is opensource. unix is free os."
mytrans ./src/mytrans/text.txt
mywc ./src/mytrans/text.txt
EOF

./20_mybash /tmp/mybash_cmd_20.txt
