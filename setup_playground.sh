#!/bin/bash
#
# Sets up a known directory tree, for testing plaidsh

topdir="Plaid Shell Playground"

rm -rf "$topdir"
mkdir "$topdir"
cd "$topdir"

cat > README <<EOF
This directory has a small number of files, for testing plaidsh
EOF

cat > "seven dwarfs.txt" <<EOF
Doc
Grumpy
Happy
Sleepy
Bashful
Sneezy
Dopey
EOF

cat > "best sitcoms.txt" <<EOF
Cheers
How I Met Your Mother
Friends
Happy Days
The Big Bang Theory
The Mary Tyler Moore Show
M*A*S*H
All In The Family
Seinfeld
The Simpsons
EOF

cat > shells.txt <<EOF
bash
csh
tcsh
plaidsh
sh
EOF

echo "Plaid Shell playground created..."
