#include <stdio.h>
#include <stdlib.h>
#include <time.h>

// 乘法取模函数，实现高精度乘法取模，模拟类似的计算逻辑
long long mul(long long a, long long b, long long mod) {
    a %= mod;
    b %= mod;
    long long c = (long long)((long double)a * b / mod);
    long long ans = a * b - c * mod;
    return (ans % mod + mod) % mod;
}

// 快速幂取模函数，用于高效计算幂次取模结果
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

// Miller-Rabin素性测试核心函数
int Miller_Rabbin(long long a, long long n) {
    // 把n - 1转化成 (2^r) * d的形式，r记录2的幂次，d为分解后剩下的奇数部分
    long long s = n - 1;
    int r = 0;
    while ((s & 1) == 0) {
        s >>= 1;
        r++;
    }

    // 算出2^d存在k里，即计算a的s次幂对n取模的结果
    long long k = pow_mod(a, s, n);

    // 二次探测，看变化过程中是不是等于1或n - 1
    if (k == 1) return 1;
    for (int i = 0; i < r; i++) {
        k = k * k % n;
        if (k == n - 1) return 1;
    }
    return 0;
}

// 判断是否为素数的函数，通过多次调用Miller_Rabbin进行测试
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
    long long num = 1039;  // 这里示例一个要测试的数，你可以替换成其他的数
    if (isprime(num)) {
        printf("可能是素数\n");
    } else {
        printf("是合数\n");
    }
    return 0;
}