## SRP 단일 책임 원칙

- SRP(Single Responsibility Principle)는 하나의 기능만 가지며 해당 클래스가 제공하는 모든 서비스는 그 하나의 책임을 수행하는데 집중되어야 하는 원칙
- 책임을 한곳에 두지 않고 적절히 분배해 코드의 가독성과 유지 보수를 향상시킴
- 버그가 발생해도 관련없는 동작에는 영향을 미치지 않도록 하는 것이 목표

## OCP 개방-패쇄 원칙

- OCP(Open-Closed Principle)는 **확장에는 열려 있으나 변경에는 닫혀 있어야** 한다는 원칙
- FileService에 DB별로 저장 method가 있다면 새로운 DB를 추가하기위해 FileService를 수정해야므로 OCP 위반
- FileService는 Save 메소드를 갖고 있는 추상 클래스로 만든다면 새로운 DB 추가시 새로운 객체를 만들면되고 FileService를 수정하지 않음
- 즉, 해당 클래스의 기존 동작을 변경하지 않고 클래스의 동작을 확장하는 것이 목표

## LSP 리스코프 치환 원칙

- LSP(Liskov Substitution Principle)는 자식 클래스가 부모 클래스를 대체해도 제대로 동작해야한다는 원칙
- LSP를 지키면 OCP를 확장하고 응용 프로그램을 중단하지 않고 상위 클래스의 객체를 하위 클래스의 객체로 바꿀 수 있음

## ISP 인터페이스 분리 원칙

- ISP(Interface Segregation Principle)는 자신이 사용하지 안흔 메서드에 의존하지 않는 것이 원칙
- 세분화된 인터페이스를 만들고 필요없는 메서드를 구현하지 않도록 하는 것이 목표
- ISP와 SRP는 비슷한 목표지만 ISP는 인터페이스와, SRP는 클래스와 관련있다는 차이

## DIP 의존성 역전 원칙

- DIP(Dependency Inversion Principle)는 변화하기 쉬운 것에 의존하지 말라는 원칙
- 구체 클래스에 의존하는 것보다 추상 클래스에 의존해야 수정 가능성이 낮아짐
- 상위 모듈은 하위 모듈에 의존하며 안됨
    - 상위 모듈: 도구로 작업을 실행하는 클래스
    - 하위 모듈: 작업을 실행하는데 필요한 도구 
- 하위 모듈이 추상클래스라면 세부사항이 바뀌어도 상위 모듈의 수정이 필요없음
- 즉, 상위 모듈을 구체적인 구현이 아닌 추상화(Interface)에 종속하도록 설계
- 하위 모듈에 대한 상위 모듈의 종속성을 줄이는 것이 목표

<br>

> Reference
- https://yoongrammer.tistory.com/98?category=1046505