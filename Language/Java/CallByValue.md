> https://github.com/evelyn82ny/Computer-science/blob/master/Language/Java/code/CallByValue.java

## Primitive type

```java
private static void checkPrimitiveType() {
    int a = 10;
    int b = a;

    System.out.println("a = " + System.identityHashCode(a)); // 1057941451
    System.out.println("b = " + System.identityHashCode(b)); // 1057941451

    b = 10;
    System.out.println("b = " + System.identityHashCode(b)); // 1057941451

    b = 11;
    System.out.println("b = " + System.identityHashCode(b)); // 434091818

    b = 10;
    System.out.println("b = " + System.identityHashCode(b)); // 1057941451

    int c = 10;
    System.out.println("c = " + System.identityHashCode(c)); // 1057941451
}
```
- b 가 a 와 같은 ```10```인 값을 참조하면 identityHashCode 값이 동일
- b가 ```11```인 값을 참조하면 identityHashCode 이 변경
- b가 다시 a 와 같은 ```10```인 값을 참조하면 identityHashCode 값이 동일
<br>

## Object

```java
private static void checkObject() {
    User a = new User("apple");
    User b = a;

    System.out.println("a = " + a); // CallByValue$User@75bd9247
    System.out.println("b = " + b); // CallByValue$User@75bd9247
    System.out.println("a == b ? " + (a == b)); // true

    b.printName(); // apple
    b.name = "kiwi";
    b.printName(); // kiwi
    a.printName(); // kiwi

    System.out.println("a = " + a); // CallByValue$User@75bd9247
    System.out.println("b = " + b); // CallByValue$User@75bd9247
}
```
- a 와 b 는 Heap 영역에 있는 같은 객체를 참조
<br>

```java
private static void checkObjectNew() {
    User a = new User("apple");
    User b = new User("apple");

    System.out.println("a = " + a); // CallByValue$User@3d24753a
    System.out.println("b = " + b); // CallByValue$User@59a6e353
    System.out.println("a == b ? " + (a == b)); // false
}
```
- 속성 값만 같고 서로 다른 객체
<br>

## String

Java에는 문자열을 **리터럴 생성** 또는 **new 연산자로 생성**하는 방식으로 나뉜다.

```java
private static void checkString() {
    String a = "apple"; // String Constant Pool 에 할당
    String b = "apple";
    System.out.println("a == b ? " + (a == b)); // true

    String c = new String("apple"); // Heap 에 할당
    String d = new String("apple");
    System.out.println("a == c ? " + (a == c)); // false
    System.out.println("a.equal(c) ? " + a.equals(c)); // true
        
    System.out.println("c == d ? " + (c == d)); // false
    System.out.println("c.equal(d) ? " + c.equals(d)); // true
}
```

![png](/Language/_img/java_string_literal_vs_string_object.png)

### 문자열 리터럴 생성 방식 -> a, b

- 리터럴를 이용하면 ```String Constant Pool```에 할당
- ```Constant Pool```에 생성되면 같은 문자열일 경우 1개만 생성
- 위 코드에서 a 와 b 는 "apple" 이라는 같은 값을 가지고 있으므로 ```Constant Pool```의 같은 곳을 참조


### new 연산자를 이용한 문자열 생성 방식 -> c

- new 연산자로 문자열 생성시 ```Heap``` 영역에 할당
- 각각의 instance가 생성되는 것이므로 같은 값이라도 다른 객체
- ```==``` 연산은 주소 값을 비교
- ```equals()```는 값을 비교
<br>

## intern()

```java
private static void checkIntern() {
    String a = "apple";
    String b = a.intern();

    System.out.println("a == b ? " + (a == b));  // true
    System.out.println("a.equals(b) ? " + a.equals(b)); // true
}
```

```java
package java.lang;

public final class String implements java.io.Serializable, Comparable<String>, CharSequence {

    public native String intern();
}
```

같은 문자열 값을 리터럴 방식으로 생성하면 같은 참조를 하게 되는 이유는 ```intern()``` 때문이다.

- ```equal()``` 로 비교해 ```Constant pool```에 있는 값과 같다면 해당 String 을 return
- 그렇지 않을 경우 ``Constant pool```에 String 객체 생성 후 해당 객체의 reference 를 return