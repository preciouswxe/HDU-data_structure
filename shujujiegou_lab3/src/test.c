#include <stdio.h>
#include <stdlib.h>
#include <time.h>

// �˷�ȡģ������ʵ�ָ߾��ȳ˷�ȡģ��ģ�����Ƶļ����߼�
long long mul(long long a, long long b, long long mod) {
    a %= mod;
    b %= mod;
    long long c = (long long)((long double)a * b / mod);
    long long ans = a * b - c * mod;
    return (ans % mod + mod) % mod;
}

// ������ȡģ���������ڸ�Ч�����ݴ�ȡģ���
long long pow_mod(long long x, long long n, long long mod) {
    long long res = 1;
    while (n) {
        if (n & 1) {
            res = mul(res, x, mod);
        }
        x = mul(x, x, mod);
        n >>= 1;
    }
    return (res + mod) % mod;
}

// Miller-Rabin���Բ��Ժ��ĺ���
int Miller_Rabbin(long long a, long long n) {
    // ��n - 1ת���� (2^r) * d����ʽ��r��¼2���ݴΣ�dΪ�ֽ��ʣ�µ���������
    long long s = n - 1;
    int r = 0;
    while ((s & 1) == 0) {
        s >>= 1;
        r++;
    }

    // ���2^d����k�������a��s���ݶ�nȡģ�Ľ��
    long long k = pow_mod(a, s, n);

    // ����̽�⣬���仯�������ǲ��ǵ���1��n - 1
    if (k == 1) return 1;
    for (int i = 0; i < r; i++) {
        k = k * k % n;
        if (k == n - 1) return 1;
    }
    return 0;
}

// �ж��Ƿ�Ϊ�����ĺ�����ͨ����ε���Miller_Rabbin���в���
int isprime(long long n) {
    int times = 7;
    long long prime[7] = {2, 3, 5, 7, 11, 233, 331};
    for (int i = 0; i < times; i++) {
        if (n == prime[i]) return 1;
        if (!Miller_Rabbin(prime[i], n)) return 0;
    }
    return 1;
}


int main() {
    long long num = 1039;  // ����ʾ��һ��Ҫ���Ե�����������滻����������
    if (isprime(num)) {
        printf("����������\n");
    } else {
        printf("�Ǻ���\n");
    }
    return 0;
}