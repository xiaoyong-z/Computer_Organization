#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

// include SSE intrinsics
#if defined(_MSC_VER)
#include <intrin.h>
#elif defined(__GNUC__) && (defined(__x86_64__) || defined(__i386__))
#include <x86intrin.h>
#endif


#define CLOCK_RATE_GHZ 2.26e9

static __inline__ uint64_t RDTSC()
{
    uint32_t hi, lo;
    __asm__ volatile
	(
		"rdtsc"
		: "=a"(lo), "=d"(hi)
	);
	return (((uint64_t)hi) << 32) | ((uint64_t)lo);
}

static int sum_naive(int n, int *a)
{
    int sum = 0;
	for (int i = 0; i < n; i++)
	{
		sum += a[i];
	}
	return sum;
}

static int sum_unrolled(int n, int *a)
{
    int sum = 0;

    // unrolled loop
	for (int i = 0; i < n / 4 * 4; i += 4)
    {
        sum += a[i+0];
        sum += a[i+1];
        sum += a[i+2];
        sum += a[i+3];
    }

    // tail case
	for (int i = n / 4 * 4; i < n; i++)
	{
		sum += a[i];
	}

    return sum;
}
/*
 *
__m128i _mm_setzero_si128( )	returns 128-bit zero vector
__m128i _mm_loadu_si128( __m128i *p )	returns 128-bit vector stored at pointer p
__m128i _mm_add_epi32( __m128i a, __m128i b )	returns vector (a0+b0, a1+b1, a2+b2, a3+b3)
   void _mm_storeu_si128( __m128i *p, __m128i a )	stores 128-bit vector a at pointer p
 */
static int sum_vectorized_unrolled(int n, int *a)
{
    int result = 0, i;
    int sum_array[4] = {0, 0, 0, 0};
    __m128i sum1 = _mm_setzero_si128();
    __m128i sum2 = _mm_setzero_si128();
    __m128i sum3 = _mm_setzero_si128();
    __m128i sum4 = _mm_setzero_si128();
    __m128i sum5 = _mm_setzero_si128();
    for (int i = 0; i < n / 4 * 4; i += 16){
        sum1 = _mm_add_epi32(sum1, _mm_loadu_si128((__m128i *)(a + i)));
        sum2 = _mm_add_epi32(sum2, _mm_loadu_si128((__m128i *)(a + i + 4)));
        sum3 = _mm_add_epi32(sum3, _mm_loadu_si128((__m128i *)(a + i + 8)));
        sum4 = _mm_add_epi32(sum4, _mm_loadu_si128((__m128i *)(a + i + 12)));
    }
    sum5 = _mm_add_epi32(sum1, sum2);
    sum5 = _mm_add_epi32(sum5, sum3);
    sum5 = _mm_add_epi32(sum5, sum4);
    _mm_storeu_si128((__m128i *)sum_array, sum5);
    for (int j = 0; j < 4; j++){
        result = result + sum_array[j];
    }
    for (int i = n / 4 * 4; i < n; i++){
        result += a[i];
    }
    return result;
}

static int sum_vectorized(int n, int *a)
{
    int result = 0, i;
    int sum_array[4] = {0, 0, 0, 0};
    __m128i sum = _mm_setzero_si128( );
    for (i = 0; i + 4 < n; i += 4){
        sum = _mm_add_epi32(sum, _mm_loadu_si128((__m128i *)(a + i)));
    }
    _mm_storeu_si128((__m128i *)sum_array, sum);
    for (int j = 0; j < 4; j++){
        result = result + sum_array[j];
    }
    for (; i < n; i++){
        result += a[i];
    }
    return result;
}

void benchmark(int n, int *a, int(*computeSum)(int, int*), char *name)
{
    // warm up cache
    int sum = computeSum(n, a);

    printf("sum1: %d", sum);
    int sum_temp = sum;

    // measure
    uint64_t beginCycle = RDTSC();
    sum += computeSum(n, a);
	uint64_t cycles = RDTSC() - beginCycle;

    printf("  sum2: %d", sum - sum_temp);
    double microseconds = cycles/CLOCK_RATE_GHZ*1e6;
    
    // print results
	printf("%20s: ", name);
	if (sum == 2 * sum_naive(n, a))
	{
		printf("%.2f microseconds\n", microseconds);
	}
	else
	{
		printf("ERROR!\n");
	}
}

int main(int argc, char **argv)
{
    const int n = 7777;
    
    // initialize the array with random values
	srand48(time(NULL));
	int a[n] __attribute__((aligned(16)));
	for (int i = 0; i < n; i++)
	{
		a[i] = lrand48();
	}
    
    // benchmark series of codes
	benchmark(n, a, sum_naive, "naive");
	benchmark(n, a, sum_unrolled, "unrolled");
	benchmark(n, a, sum_vectorized, "vectorized");
	benchmark(n, a, sum_vectorized_unrolled, "vectorized unrolled");

    return 0;
}

