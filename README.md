# computation offloading ROS1         
ROS 기반 자율주행 application의 로드밸런싱을 위한 computation offloading 기술 개발  
 
# Sofware Spec
 
### 프로그램종류	
대분류 : 시스템 S/W

중분류 : 미들웨어

소분류 : 기타 미들웨어

### 적용분야	
ROS(Robot Operating System) 기반의 자율주행 application

### 사용방법	
ROS melodic이 빌드된 ubuntu 18.04 환경 필요, 구체적인 내용은 첨부파일 __[사용방법.pdf](https://github.com/IITP-Load-Balancing/computation_offloading_ROS1/blob/main/%EC%82%AC%EC%9A%A9%EB%B0%A9%EB%B2%95.pdf)__ 참고

# details

### 프로그램 overview
본 프로그램, "ROS 기반 자율주행 application의 로드밸런싱을 위한 computation offloading 기술 개발"은 ROS 기반의 자율주행 객체 인식 application(YOLOv3)의 computing task를 local embedded board(OBU)에서 server pc(RSU)로 적응적으로 offloading하여 종단 간 지연시간(end-to-end latency)을 최소화함을 목표로 함.
 
### 기존 방식의 한계
기존의 ROS 기반 자율주행 프로그램들은 local embedded board(OBU) 와 server (RSU)간의 통신을 사용하더라도, 자율주행 application 특성 상 정해진 순서대로 실행되어야 하기에, 특정 task는 정해진 기기(로컬 또는 서버)에서 고정적으로 실행될 수 밖에 없었음.
 
### motivation
하지만, 네트워크 상태, 로컬과 서버의 리소스 유휴상태 등 실시간으로 바뀌는 시스템의 여러 상태 parameter에 따라 특정 computing task의 최적의 실행 위치는 dynamic하게 달라질 수 있음.
 
### 프로그램 상세 설명
이 사실에 입각하여, 본 프로그램은 유명 객체 인식 모듈, YOLOv3를 target computing task로 잡고 해당 task를 network 상황과 로컬 및 서버의 리소스 유휴상태 등에 따라 로컬과 서버 중 더 작은 end-to-end latency를 갖는 기기를 선택하여 수행하도록 하였음.
 
### 프로그램 도입 결과 분석
구체적으로, data_arbitrator node를 사용하여 같은 task 연산 결과 중 더 빠르게 수행되는 결과를 선택하였고, LGSVL simulator와 연동하여 실험한 결과, 동일 조건에서 기존 로컬 또는 서버만 사용하였을 때 평균 deadline miss rate가 13.8%였는데, 해당 프로그램을 도입하여 deadline miss가 발생하지 않는 것을 확인하였음.
 
### 확장 가능성
끝으로, 해당 프로그램은 YOLOv3에만 국한되는 것이 아닌, 모든 ros 기반 자율주행 application에 data_arbitrator 메커니즘을 적용할 수 있음. 해당 메커니즘의 적용으로, 기존의 자율주행 application들이 보다 system의 상태에 적응적으로 대응하여 전체 실행시간을 줄이는 효과를 기대할 수 있음.
