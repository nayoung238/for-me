Java에선 ```Primitive type```과 ```Reference type```으로 나뉜다.

## Primitive type

- Primitive type
  - Boolean type : boolean
  - Numeric type
    - Integral type
      - Integer type : byte, short, int, long
      - Floating type : float, double
    - Character type : char
<br>

- 8가지 primitive type
- 비객체 타입으로 null 값을 가질 수 없음
  - Primitive type에 null 값을 넣고 싶다면 Wrapper class 활용
- stack에 저장
<br>

## Reference type

- Reference type
  - Class type
  - Interface type
  - Array type
  - Enum type
<br>

- Primitive type을 제외한 모든 type
- java.lang.Object 클래스를 상속하는 모든 클래스
- new로 생성해 Heap 에 저장
- Garbage collector가 돌면서 메모리를 해제
- 객체이므로 null 값을 가질 수 있음
- 하지만 객체나 배열을 Null 값으로 받으면 NullPointExcpetion 발생
- Heap에 생성된 Reference type에 접근하기 위해 JVM의 stack 영역에 존재하는 frame에 일종의 포인터인 참조값을 통해 instance를 handling

<br>

## Wrapper class

- Primitive type에 null 값을 넣고 싶다면 Wrapper class 활용한다.
- 즉, Primitive type에 대한 클래스 표현이 Wrapper class이다.
- ```Integer```, ```Long```, ```Float```, ```Boolean```, ```Character``` 등을 말한다.
- ```package java.lang```에 존재
- ```intValue()```으로 Wrapper class의 값을 가져와 비교할 수 있다.

```java
Integer age1 = 10;
Integer age2 = 10;

System.out.println(age1 == age2); // true
System.out.println(age1.equals(age2)); // true
System.out.println(age1.intValue() == age2.intValue()); // true
```