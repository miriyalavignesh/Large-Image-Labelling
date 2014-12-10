#!/bin/bash
i=1
while read lines 
do
echo "$i   $lines"  
((i++))
done < $1
