const char INDEX_HEAD[] PROGMEM = R"=====(
  <!DOCTYPE html>
  <html lang='ko'>
  <head>
  <meta charset='UTF-8'/>
  <meta content='no-cache'/>
  <meta name='viewport' content='width=device-width,initial-scale=1.0,user-scalable=no' />
  <link rel='stylesheet' href='https://cdn.jsdelivr.net/npm/reset-css@5.0.1/reset.min.css?'>
  <style type='text/css'>
  body{
    margin-left: auto; margin-right: auto;
    line-height: 2.5rem;
    text-align: center;
    background-color: #383847;
  }
  body::after{
    position: absolute; 
    width: 0; 
    height: 0; 
    overflow: hidden; 
    z-index:-1;
    content:url('toggle_air.png') url('toggle_gas.png') url('cirbtn_off.png') url('cirbtn_on.png');
  }
  .modBtn{
    width: 70px;
    height: 35px;
    border: none;
    border-radius:0%;
    font-size: 0rem;
    background: url('toggle_air.png') no-repeat;
    cursor: pointer;
  }
  .thingsBtn{
    width: 120px;
    height: 120px;
    border: none;
    border-radius:70%;
    box-shadow: 0.0rem 0.0rem 0.3rem 0.3rem #063BDF;
    font-size: 0rem;
    padding: 2.0rem 2.0rem;
    margin: 1.0rem auto;
    background: url('cirbtn_off.png') no-repeat;
    cursor: pointer;
  }

  .transit{
    height: 2.0rem;
    width: 15rem;
    border: 0;  
    font-size: 0.9rem;
    text-align: center;
    color: #0e96f0;
    background-color: #19446865;
  }
  .transit:active,
  .transit:hover{
    color: #e0e7e5;
    cursor: pointer;
  }

  input, select, .inputW{
    float:none;
    display: inline-block;
    width: 15rem;
    height: 1.7rem;    
    vertical-align: middle;
    text-align: left;  
    font: 0.8rem verdana;
    color: #3f3131;
    background-color: #c4bbbb;
    resize: vertical;
  }
  select{
    width: 15.5rem;
    height: 2.0rem;
  }
  input::placeholder{
    color: #3f3131;
  }

  button{
    display: inline-block;
    height: 2.5rem;
    width: 7rem;
    border: 0;
    line-height: 2.5rem;
    padding: 0 20px;
    font-size: 0.8rem;
    text-align: center;
    color: #867676;
    text-shadow: 1px 1px 1px #000;
    border-radius: 7%;
    background-image: linear-gradient(to top left, rgba(0, 0, 0, 0.2), rgba(0, 0, 0, 0.2) 30%, rgba(0, 0, 0, 0));
    box-shadow: inset 0.5px 0.5px 1.0px rgba(255, 255, 255, 0.6), inset -0.5px -0.5px 1.0px rgba(0, 0, 0, 0.6);
  }
  button:active,
  button:hover{
    color: #e0e7e5;
    cursor: pointer;
  }
  </style>
  <title>IEUM IoT</title>
  <link rel='icon' type='image/x-icon' href='favicon-ieum.ico'>
  </head>
  <body>
  <h1 style='color:#cbf1b1' align: center><b>NANO BUBBLE GENERATOR</b></h1>
)=====";
const char BODY_CTRL[] PROGMEM = R"=====(
  <br>
  <div id='operDiv' style='display:block'>
    <div>
      <input type='button' id='modBTN' class='modBtn' value='GAS' onclick='modClick();'>
    </div>
    <div>
      <input type='button' id='thingsBTN' class='thingsBtn' value='ON' onclick='thingsClick();'> 
    </div>
  </div>
  <br>
)=====";
const char BODY_CONFIG1[] PROGMEM = R"=====(
  
<div id='passBTN' style='display:block'><input class='transit' value='CHANGE YOUR PASSWORD' onclick='passShow();' /></div> 
  <div id='passDiv' style='display:none'>
    <form name='apform'>
      <p class='inputW'><input type='password' name='ApPass' id='ApPass' placeholder='Password(8~11 only numbers)' onchange='validatePASS(this.value, this.id)' required/></p>
      <br>
      <p>
        <button type='submit' style='background-color:rgba(32, 87, 32, 0.5)'>SEND</button>
        <button type='reset' style='background-color:rgba(255, 150, 0, 0.50)'>RESET</button>
      </p><p>  
        <button type='button' style='background-color:rgba(214, 18, 18, 0.5); width:14.2rem' onclick='delAPInput();'>AP FACTORY RESET</button>
      </p>
    </form>
  </div>
  <br>

  <div id='netBTN' style='display:block'><input class='transit' value='CONNECT NETWORK' onclick='netShow();' /></div>     
  <div id='netDiv' style='display:none'> 
    <form name='staForm'>
      <p class='inputW'>
        <select type='text' name='NetId' required>
          <option value=''>Select IoT Network</option>
)=====";
const char BODY_CONFIG2[] PROGMEM = R"=====(
        </select>
      </p>
      <p class='inputW'><input type='text' name='NetIp' id='NetIp' placeholder='Static IP(ex:192.168.0.107)' onchange='validateIP(this.value, this.id)' required/></p>
      <p class='inputW'><input type='text' name='NetGateWay' id='NetGateWay' placeholder='Gateway(ex:192.168.0.1)' onchange='validateIP(this.value, this.id)' required/></p>
      <p class='inputW'><input type='text' name='NetSubNet' id='NetSubNet' placeholder='Subnetmask(ex:255.255.255.0)' onchange='validateIP(this.value, this.id)' required/></p>
      <p class='inputW'><input type='password' name='NetPass' maxlength='20' placeholder='network password(1~20 characters)' required/></p>
      <br>
      <p>
        <button type='submit' style='background-color:rgba(32, 87, 32, 0.5)'>SEND</button>
        <button type='reset' style='background-color:rgba(255, 150, 0, 0.50)'>RESET</button>
      </p><p>  
        <button type='button' style='background-color:rgba(214, 18, 18, 0.5); width:14.2rem' onclick='delSTAInput();'>NETWORK FACTORY RESET</button>
      </p>
    </form>
  </div>
  
  <script type='text/javascript'>  
    let ws;
    let gateWay = 'ws://' + window.location.hostname + ':81/'; 
    let mbtnID = document.getElementById('modBTN');
    let tbtnID = document.getElementById('thingsBTN');
    let divOper = document.getElementById('operDiv');
    let divPass = document.getElementById('passDiv');   
    let divNet = document.getElementById('netDiv');
    let sendTXT = '52C';
    let selectMod = '0';
    let selectReq = '0';

    function init(){
      ws = new WebSocket(gateWay);
      ws.onopen = onOpen;
      ws.onclose = onClose;
      ws.onmessage = onMessage;
      ws.onerror = onError;
    }
    function onOpen(evt){
      //console.log('set initialization on socket open');
    }    
    function onClose(evt){
      if (evt.wasClean) {
      } else {
        console.log('disconnection!');
        setTimeout(init, 2000);
      }
    }
    function selectGas(){
      mbtnID.style.backgroundImage = 'url(\'toggle_gas.png\')';
      mbtnID.value = 'AIR';
    }
    function selectAir(){
      mbtnID.style.backgroundImage = 'url(\'toggle_air.png\')';
      mbtnID.value = 'GAS';
    }
    function selectOn(){
      mbtnID.disabled = true;
      tbtnID.style.backgroundImage = 'url(\'cirbtn_on.png\')';
      tbtnID.value = 'OFF';
    }
    function selectOff(){
      mbtnID.disabled = false;
      tbtnID.style.backgroundImage = 'url(\'cirbtn_off.png\')';
      tbtnID.value = 'ON';
    }
    function modClick(){
      let mbtnText = mbtnID.value;
      if(mbtnText ==='GAS'){
        selectMod = '1';
        selectGas();
      }else{
        selectMod = '0';
        selectAir();
      }
    }
    function thingsClick(){
      let pbtnID = document.getElementById('passBTN');
      let nbtnID = document.getElementById('netBTN');
      let tbtnText = tbtnID.value;
      if(tbtnText ==='ON'){
        selectReq = '1';
        selectOn();
        pbtnID.style.display = 'none';
        nbtnID.style.display = 'none';
      }else{
        selectReq = '0';
        selectOff();
        pbtnID.style.display = 'block';
        nbtnID.style.display = 'block';
      }
      sendBtnMsg();
    }
    function sendBtnMsg(){
      sendTXT += selectMod;
      sendTXT += selectReq;
      sendTXT += ';';
      ws.send(sendTXT);
      console.log('web send msg: ', sendTXT);
      sendTXT = '52C';
    }
    function onMessage(evt){
      let rcvObj = JSON.parse(evt.data);
      console.log('rcvd msg from MCU: ', rcvObj.MOD, rcvObj.REQ);
      if(rcvObj.MOD == 0){
        selectMod = '0';
        selectAir();
      }else if(rcvObj.MOD == 1){
        selectMod = '1';
        selectGas();
      }
      if(rcvObj.REQ == 0){
        selectReq = '0';
        selectOff();
      }else if(rcvObj.REQ == 1){
        selectReq = '1';
        selectOn();
      }
    }
    function onError(evt){
      console.log('error message!')
    }
    
    function passShow(){
      divNet.style.display = 'none';
      if(divPass.style.display=='none'){
        divOper.style.display = 'none';
        divPass.style.display = 'block';
      }else{
        divOper.style.display = 'block';
        divPass.style.display = 'none';
      }
    }
    function netShow(){
      divPass.style.display = 'none';
      if(divNet.style.display=='none'){
        divOper.style.display = 'none';    
        divNet.style.display = 'block';
      }else{
        divOper.style.display = 'block';
        divNet.style.display = 'none';
      }
    }

    function validatePASS(pass_txt, id_input){
      var pwformat = /^[0-9]{8,11}$/;
      if(pwformat.test(pass_txt)==true){
        return false;
      } else {
        alert('**keep the password format.\nEnter only number, no more than 4~11 digits.');
        event.returnValue=false;
        document.getElementById(id_input).value = '';
        document.getElementById(id_input).focus();
        return true;
      }
    }
    
    function validateIP(ip_addr, iput_id){
      let ipformat = /^(([0-9]|[1-9][0-9]|1[0-9]{2}|2[0-4][0-9]|25[0-5])\.){3}([0-9]|[1-9][0-9]|1[0-9]{2}|2[0-4][0-9]|25[0-5])$/;
      if(ipformat.test(ip_addr)==true){
        return false;
      }else{
        alert('**You have entered an invalid IP address!');
        event.returnValue=false;
        document.getElementById(iput_id).value = '';
        document.getElementById(iput_id).focus();
        return true;
      }
    }
    document.forms.apform.onsubmit = function(){
      let ApInfo = {};
      ApInfo['ApPass'] = this.ApPass.value;
      console.log(JSON.stringify(ApInfo));

      ws.send(JSON.stringify(ApInfo));
      setTimeout(function(){
        alert('After reboot, Your AP Password was changed.');
      }, 500);
      divOper.style.display = 'block';
      divPass.style.display = 'none';
      return false;
    }
    document.forms.staForm.onsubmit = function(){
      let NetInfo = {};
      NetInfo['NetId'] = this.NetId.value;
      NetInfo['NetIp'] = this.NetIp.value;
      NetInfo['NetGateWay'] = this.NetGateWay.value;
      NetInfo['NetSubNet'] = this.NetSubNet.value;
      NetInfo['NetPass'] = this.NetPass.value;
      console.log(JSON.stringify(NetInfo));

      ws.send(JSON.stringify(NetInfo));
      alert('After reboot, Connect the IP: ' + NetInfo['NetIp'] + ' on network \'' + NetInfo['NetId'] + '\'.');
      divOper.style.display = 'block';
      divNet.style.display = 'none';
      return false;
    }
    function delAPInput(){
      alert('Delete all AP information inputted. This machine\'ll be rebooted. Please re-connect on AP mode(\'192.168.4.1\') and password(\'00000000\'), after rebooted!');
      window.location = '/apClear';
    }
    function delSTAInput(){
      alert('Delete all network information inputted. This machine\'ll be rebooted. Please re-connect on AP mode(\'192.168.4.1\') and your AP password, after rebooted!');
      window.location = '/staClear';
    }
    
    window.addEventListener('load', init, false);
  </script>
  </html>
)=====";
