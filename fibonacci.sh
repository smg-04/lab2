read -p "출력할 항의 개수를 입력하세요: " n

a=0
b=1

echo "피보나치 수열 ($n 항):"

for (( i=1; i<=n; i++ ))
do
    echo -n "$a "
    let next=a+b
    a=$b
    b=$next
done
echo
