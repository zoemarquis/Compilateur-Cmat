#!/bin/bash

input_dir="input"
output_dir="output"

rm -rf "$output_dir"
mkdir -p "$output_dir/right"
mkdir -p "$output_dir/wrong"

find "$input_dir" -type f -name "*.expr" | while read -r input_file; do
    filename=$(basename "$input_file" .expr)
    parent_dir=$(dirname "$input_file")

    if [[ "$parent_dir" == */right ]]; then
        # Si le dossier parent se termine par "/right", placez le résultat dans "output/right"
        output_file="$output_dir/right/$filename.txt"
        error_file="$output_dir/right/$filename.err"
        ./cmat < "$input_file" > "$output_file"
        echo "Test effectué pour $input_file, résultat dans $output_file"
    elif [[ "$parent_dir" == */wrong ]]; then
        # Si le dossier parent se termine par "/wrong", placez le résultat dans "output/wrong"
        output_file="$output_dir/wrong/$filename.txt"
        error_file="$output_dir/wrong/$filename.err"
        ./cmat < "$input_file" 2>"$error_file"
        echo "Test effectué pour $input_file, erreurs dans $error_file"
    fi
done

# pour tout ceux dans right les placer dans un dossier right
# pour tout ceux dans wrong les placer fans un dossier output/wrong

# les output/wrong doivent lever des erreurs : comparer ecs erreurs aux résultats obtenus
# générer les fichiers de sorties pour les right (pour ensuite comparer ...)

# TODO