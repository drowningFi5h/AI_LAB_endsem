def multiplication(q1, q2):
    count = 0
    ans = 0
    while q2:
        lsb = q2 & 1
        if lsb:
            ans ^= q1 << count
        q2 >>= 1
        count += 1
    return ans
ans = multiplication(5, 4)
fin_ans = multiplication(ans , 10)
print(fin_ans)