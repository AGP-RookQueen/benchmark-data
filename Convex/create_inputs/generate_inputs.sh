g++-15 convex.cpp -O3 -o gen
g++-15 helper.cpp -O3 -o helper

MAX=400

for ((i = 4 ; i <= MAX ; i += 2 )); do
    ./gen $i > ./input_raw/input_${i}
    return_value=$?
    if [ $return_value -ne 0 ]; then
        echo "ERROR"
    fi
    ./helper < ./input_raw/input_${i} > ../input/input_${i}
done