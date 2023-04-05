#!/bin/bash

# Usage: bash script_name.sh [optional: V<n>]
# This script compiles and runs all V*.c files in the current directory with input files of the form "input[A]1.txt" and "input[A]2.txt", where A is a capitalized letter between A and Z. The output files are of the form "V[n]output[A].txt" and are compared to expected output files of the form "expected_output[A].txt". The times and differences are logged to files in the ./times directory.

if [ -z "$1" ]; then
  # Run for all .c files in the current directory
  c_files="V*.c"
else
  # Run only for specified .c file
  c_files="$1.c"
fi

# Determine the name of the times file to use
times_dir="./times"
if [[ ! -d "$times_dir" ]]
then
  mkdir "$times_dir"
fi

if [[ -e "$times_dir/times1.txt" ]]
then
  i=2
  while [[ -e "$times_dir/times$i.txt" ]]
  do
    ((i++))
  done
  times_file="$times_dir/times$i.txt"
else
  times_file="$times_dir/times1.txt"
fi

# Compile all .c files in the current directory
for file in $c_files
do
  echo "Compiling $file..."
  gcc -o "${file%.c}" "$file" >> "$times_file" 2>&1 || { echo "Error compiling $file" ; exit 1 ; }
done

# Run each compiled file with the specified arguments, log the times, and diff the output files
for file in $c_files
do
  n="${file#V}"
  n="${n%.c}"
  small_output_file="V${n}smalloutput.txt"
  for letter in {A..Z}
  do
    input_file="input${letter}1.txt"
    if [[ -e $input_file ]]
    then
      output_file_name="V${n}output${letter}.txt"
      input_file2="input${letter}2.txt"
      echo "Running ${file%.c} with arguments $input_file $input_file2 $output_file_name..." >> "$times_file"
      { echo -n "${file%.c}_${letter}: " ; time "./${file%.c}" "$input_file" "$input_file2" "$output_file_name" ; } 2>> "$times_file" >> "$times_file" || { echo "Error running ${file%.c} with arguments $input_file $input_file2 $output_file_name" ; exit 1 ; }
      expected_output_file="expected_output${letter}.txt"
      if diff -q "$output_file_name" "$expected_output_file" >/dev/null ; then
        echo "${file%.c}_${letter}: Output file matches expected"
        echo "${file%.c}_${letter}: Output file matches expected" >> "$times_file"
      else
        diff_output=$(diff "$output_file_name" "$expected_output_file" | wc -l)
        #echo "${file%.c}_${letter}: Output files are different, Number of different lines: $diff_output"
        #echo "${file%.c}_${letter}: Output files are different, Number of different lines: $diff_output" >> "$times_file"
        # diff -y --suppress-common-lines "$output_file_name" "$expected_output_file" >> "$times_file"
      fi
      echo "" >> "$times_file"
    fi
  done
  echo "-----------------------------------------------------"
done

echo "Script completed successfully. Results written to $times_file."