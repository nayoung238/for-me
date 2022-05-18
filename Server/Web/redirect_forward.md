## Redirect

- Client가 Server에게 특정 URL 요청
- 만약 특정 URL이 다른 URL로 변경되었다면 Server는 변경된 URL을 Client에게 전달
- Client는 Server에게 받은 URL을 다시 요청
<br>

- Client는 받은 URL을 다시 요청하므로 URL 변화를 파악할 수 있다.
- 다시 새로운 요청을 하는 것이므로 이전 요청에 사용했던 Request, Response 를 사용하지 않고 새로 생성
- POST 요청을 처리하는 경우 Forward가 아닌 Redirect를 사용해 POST 중복 요청을 막아야함: PRG pattern
<br>

## Forward

- Client가 Server에게 특정 URL 요청
- 만약 특정 URL이 다른 URL로 변경되었다면 변경된 URL로 바로 이동
<br>

- Client는 URL의 변경을 파악할 수 없고 결과만 받는다.
- 변경된 URL 정보를 알 수 없음
- 같은 Web container 내에서만 페이지 이동 가능
- 기존 Request, Response 를 연결이 끊어질 때까지 사용