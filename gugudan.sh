read -p "출력할 단을 입력하세요 (2~9): " dan

# 입력값 확인
if [[ ! $dan =~ ^[2-9]$ ]]; then
    echo "2에서 9 사이의 숫자를 입력해야 합니다."
    exit 1
fi

echo "=== $dan 단 ==="

# for 반복문 사용
for i in {1..9}
do
    echo "$dan x $i = $((dan*i))"
done

echo "=== while 반복문 ==="
i=1
while [ $i -le 9 ]
do
    echo "$dan x $i = $((dan*i))"
    ((i++))
done

echo "=== until 반복문 ==="
i=1
until [ $i -gt 9 ]
do
    echo "$dan x $i = $((dan*i))"
    ((i++))
done
