## Stream 

## 1 부터 n 까지의 합, 곱 구하기

```java
IntStream.rangeClosed(1, n).sum();
```

```java
IntStream.rangeClosed(1, n).reduce(1, (a, b) -> a * b);
// n == 0이면 초기값을 그대로 반환
```

## 다른 값으로 변경해 새로운 값으로 도출하기

- [BOJ 1546 평균](https://www.acmicpc.net/problem/1546), [정답](https://www.acmicpc.net/source/57595197)
- 입력된 수 중 가장 큰 수로 모든 수를 변경한다.
- ```(입력 수) / (가장 큰 수) * 100``` 로 변경한다.
- 변경된 모든 수의 평균을 구한다.

```java
int n = Integer.parseInt(st.nextToken());

ArrayList<Integer> numbers = new ArrayList<>();
st = new StringTokenizer(br.readLine());
for(int i = 0; i < n; i++) {
    numbers.add(Integer.parseInt(st.nextToken()));
}

Optional<Integer> maximum_number = numbers.stream().reduce(Integer::max);

if(maximum_number.isPresent()) {
    List<Double> numbers2 = numbers.stream().map(i -> (i * 100.0) / maximum_number.get())
                                    .collect(Collectors.toList());

    Optional<Double> sum = numbers2.stream().reduce(Double::sum);
    sum.ifPresent(s -> System.out.print(s / n));
}
```