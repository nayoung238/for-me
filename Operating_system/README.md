## issue

producer_consumers_buffer_size_one.c

- Producer: buffer 사이즈가 1로 두고 데이터를 생산한다.
- Consumer: 여러 소비자가 사이즈가 1인 buffer 에서 데이터를 얻는다.

```text
nayoung@nayoungui-MacBookPro concurrency % ./pc 5 2
PUT -> [0x16b01b000]: 0
GET -> [0x16b0a7000]: 0
PUT -> [0x16b01b000]: 1
GET -> [0x16b133000]: 1
PUT -> [0x16b01b000]: 2
GET -> [0x16b133000]: 2
^C

nayoung@nayoungui-MacBookPro concurrency % ./pc 5 2
PUT -> [0x16bae3000]: 0
GET -> [0x16bb6f000]: 0
PUT -> [0x16bae3000]: 1
GET -> [0x16bbfb000]: 1
PUT -> [0x16bae3000]: 2
GET -> [0x16bb6f000]: 2
PUT -> [0x16bae3000]: 3
GET -> [0x16bb6f000]: 3
PUT -> [0x16bae3000]: 4
GET -> [0x16bb6f000]: 4
^C

nayoung@nayoungui-MacBookPro concurrency % ./pc 5 2
PUT -> [0x16fa1f000]: 0
GET -> [0x16faab000]: 0
PUT -> [0x16fa1f000]: 1
GET -> [0x16faab000]: 1
PUT -> [0x16fa1f000]: 2
GET -> [0x16faab000]: 2
PUT -> [0x16fa1f000]: 3
GET -> [0x16faab000]: 3
PUT -> [0x16fa1f000]: 4
GET -> [0x16faab000]: 4
PUT -> [0x16fa1f000]: -1
GET -> [0x16faab000]: -1
PUT -> [0x16fa1f000]: -1
GET -> [0x16fb37000]: -1
```
- 정상적으로 처리될 때도 있고 강제 종료 해야될 때도 생기는 문제가 발생 (해결 못함)
- 문제가 되는 이유를 다음과 같이 생각해봄
  - 2명의 소비자: tc1, tc2
  - 2명의 소비자가 먼저 실행되었지만 자원이 없어 둘다 SLEEP 상태
  - 생산자가 데이터를 1개 생산하고 tc1(소비자)를 꺠우고 생산자는 다시 SLEEP 상태
  - tc1(소비자)가 데이터를 소비하고 tc2(소비자)를 깨우고 tc1(소비자)는 다시 SLEEP 상태
  - tc2(소비자)는 소비할 데이터가 없어 다시 SLEEP 상태가됨
  - 그럼 SLEEP 상태인 생산자를 깨워줄 소비자가 없음
- 이런 원리로 생기는 문제라고 추측
- one_producer_and_multi_consumer.c 도 버퍼 사이즈만 다르기 때문에 위와 같은 상황이 빌셍해 모두가 SLEEP 상태인 상황이 발생할 수 있지 않나라고 생각
- 하지만 아무리 돌려도 모두가 SLEEP 상태인 상황이 발생하지 않음
- 그래서 무엇이 문제인지 파악하지 못함