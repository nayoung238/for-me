# Node.js App 만들기

Node.js로 App을 만들기 위해 ```package.json``` 와 ```server.js```가 필요하다.

- ```package.json```: 프로젝트의 정보와 프로젝트에서 사용중인 Package의 의존성 관리
- ```server.js```: Node.js에서 진입점이 되는 파일

## package.json 생성

![png](/Docker/_img/create_package_json.png)

- ```npm init```: customizing 가능
- ```npm init -y```: 모든 것을 default로 설정후 생성

<br>

### express 의존성 추가

```json
{
  "name": "nodejs-app",
  "version": "1.0.0",
  "description": "",
  "main": "server.js",  // Node.js에서 진입점이 되는 파일
  "scripts": {
    "test": "echo \"Error: no test specified\" && exit 1"
  },
  "dependencies": {
    "express": "4.18.1" // Node.js의 API를 단순화하고 새로운 기능을 추가해
  },                    // Node.js를 더 쉽고 유용하게 사용할 수 있게 함
  "author": "",
  "license": "ISC"
}

```
- ```npm i express```: express 모듈 설치
- Express: 웹 및 모바일 애플리케이션을 위한 편리 기능을 제공하는 Node.js 웹 애플리케이션 프레임웤,

```js
// Express 모듈 불러오기
const express = require('express');

const PORT = 8080;       // Express server를 위한 PORT 설정
const HOST = '0.0.0.0';  // HOST 지정

const app = express();   // 새로운 Express application 생성
app.get('/', (req, res) => {   // '/' 이 경로로 요청이 오면
    res.send('Hello world');   // Hello world 를 결과값 전달
});

app.listen(PORT, HOST);  // 해당 PORT와 HOST에서 HTTP 서버 시작
console.log("Server is running");
```

<br>

## Dockerfile 생성

![png](/Docker/_img/create_nodejs_app_dockerfile_structure.png)

> 출처: 따라하며 배우는 도커와 CI환경(John Ahn)

<br>

- Docker 환경에서 Node.js App을 실행하려면 Image를 생성해야함
- Image를 통해 Container를 실행하고 해당 Container 안에서 Node.js App 실행
- Image를 생성하기 위해 Dockerfile 필요

<br>

### RUN npm install

```dockerfile
FROM node:10

RUN npm install

CMD ["node", "server.js"]
```

- ```npm```: Node.js로 만들어진 module을 web에서 받아서 설치하고 관리해주는 프로그램
- ```npm inatll```: package.json의 종속성들을 web에서 자동으로 다운받아 설치해주는 명령어 
<br>

- ```CMD ["node", server.js]```: Node web server를 작동시키려면 ```node + entry 파일 이름``` 입력

<br>

### COPY package.json ./

![png](/Docker/_img/error_no_such_file_of_directory.png)

- 위 Dockerfile을 build 했지만 **no such file or directory** 오류가 발생
- ```package.json```이 없다는 오류
<br>

![png](/Docker/_img/error_no_such_file_package_json.png)

> 출처: 따라하며 배우는 도커와 CI환경(John Ahn)

<br>

- ```npm install``` 부분에서 Application에 필요한 종속성을 내려받음
- ```package.json```을 보고 ```package.json```에 명시된 종속성들을 설치
- 하지만 Container 안에 ```package.json```이 없기 때문에 에러 발생
- 이를 해결하기 위해 ```COPY``` 사용해 ```package.json```을 Container 안에 넣어줌

```dockerfile
FROM node:10

# 로컬에 있는 파일(package.json)을 Docker container의 지정된 곳(./)에 복사
COPY package.json ./  

RUN npm install

CMD ["node", "server.js"]
```

- Dockfile에 ```COPY package.json ./``` 을 추가하면 ```package.json```이 Container의 ```./``` 위치에 COPY
- 다시 Build 했지만 아래와 같은 오류가 발생

<br>

![png](/Docker/_img/error_cannot_find_module_server_js.png)

- 로컬에 있는 ```server.js```도 Docker container로 Copy 해야됨

<br>

### COPY ./ ./

```dockerfile
FROM node:10

COPY ./ ./

RUN npm install

CMD ["node", "server.js"]
```

![png](/Docker/_img/solution_cannot_find_module_server_js.png)

- 모든 파일을 COPY 하기 위해서 ```COPY ./ ./``` 를 추가하면 성공적으로 Build 됨
