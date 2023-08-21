# esp8266-dht11

esp8266에서 MQTT 프로토콜을 사용해 온/습도 데이터를 측정 결과를 MQTT 브로커로 전송하고</br>
MQTT 브로커로부터 입력이 들어오면 적절한 콜백 함수를 실행하는 코드입니다. </br>

---
## Compile
해당 소스코드를 컴파일 후 디바이스에 업로드 하시면 됩니다.

---
## Hardware
### 예제에서 사용된 디바이스
  - Wemos D1 R1
  - DHT11
  - IR Remote Sender

> 해당 예제에서는 DHT11 센서를 사용한 온습도 데이터 측정과, 서버로부터 입력이 들어올 경우 IR 센서를 통해 특정 값을 보내는 콜백 함수를 사용하고 있습니다.
> HTTP Client와 같은 추가적인 모듈을 사용할 경우, 추가되는 메모리 사용에 따른 기기(D1 R1) 변경을 고려해야 할 수 있습니다.

---
## FlowChart
<img width=50% src="https://github.com/youngmoneee/esp8266-dht11/assets/79129960/bad1d22d-84ea-4f4e-ab13-eaea6f597743" />

</br>
