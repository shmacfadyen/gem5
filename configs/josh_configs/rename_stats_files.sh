#!/usr/bin/env bash

dir=$1

if [ -d "$dir" ]
then
    mkdir "$dir/combined"
    
    for bp in $dir/*
    do
        if [ -d "$bp" ] &&
           [[ "$bp" != "$dir/combined" ]]
        then
            for ex in $bp/*
            do
                if [ -d "$ex" ]
                then
                    if [ -f "$ex/stats.txt" ]
                    then
                        cp $ex/stats.txt $dir/combined/$(basename "$bp")_$(basename "$ex").txt
                    fi
                fi
            done
        fi
    done

    echo "Done!"
else
    echo "The directory $dir does not exist!"
fi
