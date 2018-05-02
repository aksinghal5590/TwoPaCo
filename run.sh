oFile="bin/de_bruijn.bin"
kmer=31

./build/graphconstructor/twopaco -f 30 -t 1 -k $kmer -o $oFile $1
./build/graphdump/graphdump $oFile -f gfa1 -k $kmer -s $1 > "bin/output.txt"
