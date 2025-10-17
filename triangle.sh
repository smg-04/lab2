rows=5  # 출력할 줄 수

for (( i=rows; i>=1; i-- ))
do
    for (( j=1; j<=i; j++ ))
    do
        echo -n "*"
    done
    echo
done
