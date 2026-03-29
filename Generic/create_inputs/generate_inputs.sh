g++-15 gen.cpp -O3 -o gen
g++-15 draw.cpp -O3 -o draw

MAX=400

for ((i = 4 ; i <= MAX ; i += 2 )); do
    ./gen $i > input_raw/input_${i}
    ./draw < input_raw/input_${i} > ../input/input_$i
done