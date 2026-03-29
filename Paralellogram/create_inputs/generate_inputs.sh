g++-15 gen.cpp -O3 -o gen

MAX=400

for ((i = 4 ; i <= MAX ; i += 2 )); do
    ./gen $i > ../input/input_${i}
done