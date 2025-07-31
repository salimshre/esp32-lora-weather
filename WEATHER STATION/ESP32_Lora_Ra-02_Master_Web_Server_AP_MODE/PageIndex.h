const char MAIN_page[] PROGMEM = R"=====(
<!DOCTYPE HTML>
<html>
  <head>
    <title>REMOTE AREA WEATHER STATION WEB SERVER</title>
    <meta name="viewport" content="width=device-width, initial-scale=1">
    <style>
      html {font-family: Arial; display: inline-block; text-align: center;}
      p {font-size: 1.2rem;}
      h4 {font-size: 0.8rem;}
      body {margin: 0;}
      .topnav {overflow: hidden; background-color: #db0fb3; color: white; font-size: 1.2rem;}
      .content {padding: 5px; }
      .card {background-color: white; box-shadow: 0px 0px 10px 1px rgba(140,140,140,.5); border: 1px solid #db0fb3; border-radius: 15px;}
      .card.header {background-color: #db0fb3; color: white; border-bottom-right-radius: 0px; border-bottom-left-radius: 0px; border-top-right-radius: 12px; border-top-left-radius: 12px;}
      .cards {max-width: 700px; margin: 0 auto; display: grid; grid-gap: 2rem; grid-template-columns: repeat(auto-fit, minmax(300px, 1fr));}
      .reading {font-size: 1.3rem;}
      .packet {color: #bebebe;}
      .temperatureColor {color: #fd7e14;}
      .humidityColor {color: #1b78e2;}
      .statusreadColor {color: #702963; font-size:12px;}
      .LEDColor {color: #183153;}
    </style>
  </head>
  <body>
    <div class="topnav">
      <h3>KANTIPUR CITY COLLEGE EXHIBITION 2025<br>PUTALISADAK, KATHMANDU</h3>
    </div>
    <br>
    <div>Project Designed by Santosh, Saraswoti, Surya and Salim</div>
    <div class="content">
      <div class="cards">
        <div class="card">
          <div class="card header">
            <h3 style="font-size: 1.2rem;">ESP32 SLAVE 1</h3>
          </div>
          <h4 class="temperatureColor">TEMPERATURE</h4>
          <p class="temperatureColor"><span class="reading"><span id="temp_Slave_1"></span> &deg;C</span></p>
          <h4 class="humidityColor">HUMIDITY</h4>
          <p class="humidityColor"><span class="reading"><span id="humd_Slave_1"></span> &percnt;</span></p>
          <div style="position:relative;">
            <div style="position:relative; float: left;top: 3px;width: 47%;left: 0px;text-align: right;">
              <span class="LEDColor" style="font-weight:bold;">LED 1 : </span>
            </div>
            <div style="position:relative; float: right;width: 51%; right: 0px;">
              <label class="switch">
                <input type="checkbox" id="togLED_1_Slave_1" onclick="send_LED_State('togLED_1_Slave_1','S1','1')">
                <div class="sliderTS"></div>
              </label>
            </div>
            <div style="position:relative; float: left;top: 18px;width: 47%;left: 0px; text-align: right;">
              <span class="LEDColor" style="font-weight:bold;">LED 2 : </span>
            </div>
            <div style="position:relative;float: right; width: 51%;right: 0px; top: 5px;">
              <label class="switch">
                <input type="checkbox" id="togLED_2_Slave_1" onclick="send_LED_State('togLED_2_Slave_1','S1','2')">
                <div class="sliderTS"></div>
              </label>
            </div>
          </div>
          <br><br><br><br>
          <p class="statusreadColor"><b><span>Last time to receive data : </span><span id="LTRD_Slave_1"></span></b></p>
          <p class="statusreadColor"><span>Status Read Sensor DHT11 : </span><span id="status_read_DHT11_Slave_1"></span></p>
          <p class="statusreadColor"><span>LED 1 State : </span><span id="LED_1_State_Slave_1"></span></p>
          <p class="statusreadColor"><span>LED 2 State : </span><span id="LED_2_State_Slave_1"></span></p>
        </div>
      </div>
    </div>
    <script>
      document.getElementById("temp_Slave_1").innerHTML = "NN"; 
      document.getElementById("humd_Slave_1").innerHTML = "NN";
      document.getElementById("status_read_DHT11_Slave_1").innerHTML = "NN";
      document.getElementById("LTRD_Slave_1").innerHTML = "NN";
      document.getElementById("LED_1_State_Slave_1").innerHTML = "NN";
      document.getElementById("LED_2_State_Slave_1").innerHTML = "NN";

      if (!!window.EventSource) {
        var source = new EventSource('/events');
        source.addEventListener('allDataJSON', function(e) {
          var obj = JSON.parse(e.data);
          if (obj.ID_Slave == "S1") {
            document.getElementById("status_read_DHT11_Slave_1").innerHTML = obj.StatusReadDHT11;
            document.getElementById("humd_Slave_1").innerHTML = obj.Humd;
            document.getElementById("temp_Slave_1").innerHTML = obj.Temp.toFixed(2);
            document.getElementById("LED_1_State_Slave_1").innerHTML = obj.LED1 ? "ON" : "OFF";
            document.getElementById("LED_2_State_Slave_1").innerHTML = obj.LED2 ? "ON" : "OFF";
            var now = new Date();
            document.getElementById("LTRD_Slave_1").innerHTML = now.getHours() + ":" + now.getMinutes() + ":" + now.getSeconds();
          }
        });
      }

      function send_LED_State(id,slave,led_num) {
        var tgState = document.getElementById(id).checked ? "t" : "f";
        var xhr = new XMLHttpRequest();
        xhr.open("GET", "set_LED?Slave_Num="+slave+"&LED_Num="+led_num+"&LED_Val="+tgState, true);
        xhr.send();
      }
    </script>
  </body>
</html>
)=====";
