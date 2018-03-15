oFile="bin/de_bruijn.bin"
kmer=31

./build/graphconstructor/twopaco -f 30 -k $kmer -o $oFile $1
# ./build/graphdump/graphdump $oFile -f gfa2 -k $kmer -s $1 > "bin/output.txt" 
