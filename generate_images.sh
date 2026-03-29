if [ "$#" -ne 2 ]; then
    echo "Usage: $0 INPUT_FOLDER OUTPUT_FOLDER"
    exit 1
fi

INPUT_FOLDER=$1
OUTPUT_FOLDER=$2

MAX=400

echo $INPUT_FOLDER
echo $OUTPUT_FOLDER

for ((i = 4 ; i <= MAX ; i += 2 )); do
    echo $i
    python3 create_images.py ${OUTPUT_FOLDER}/input_${i}.eps < ${INPUT_FOLDER}/input_${i}
done
