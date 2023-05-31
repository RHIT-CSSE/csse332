#!/usr/bin/bash

echo "[Cleaning up...]"
rm -rf data.csv make_plot.gp

echo "[Compiling....]"
make clean && make


# for size in 64 128 256 512 1024 2048 4096 8192
for size in $(seq 32 32 1024)
do
    echo "Running simulation for $size..."
    ./matmult.bin $size > /tmp/res.txt
    naive_time=$(cat /tmp/res.txt | grep naive | cut -d' ' -f 8)
    weird_time=$(cat /tmp/res.txt | grep weird | cut -d' ' -f 8)

    echo "$size,$naive_time,$weird_time" >> data.csv
done

cat <<EOF > make_plot.gp
# set terminal to pdf
set terminal pdf

# Set name of output file.  The quotes are required
set output "matmult.pdf"

# Set style of plot.  We want lines
set style data lines

# Set how the data is separated in the file
set datafile separator ","

# Set the title of the plot
set title "Naive vs weird matrix multiplication"

# Do the actual plotting
plot "data.csv" using 1:2 with lines t "naive", \
    "" using 1:3 with lines t "weird"

EOF

gnuplot make_plot.gp
