#line 1 "g:\\내 드라이브\\projects\\NanoGen\\ed\\1.nanoGen\\nanogen_1.0\\nanogen.V.ESP\\index.h"
const char INDEX_HEAD[] PROGMEM = R"=====(
  <!DOCTYPE html>
  <html lang='ko'>
  <head>
  <meta charset='UTF-8'/>
  <meta content="no-cache"/>
  <meta name='viewport' content='width=device-width,initial-scale=1.0,user-scalable=no' />

  <style type='text/css'>
    body{
      text-align: center;
      background-color: #383847;
      }
    body::after{
      position: absolute; 
      width: 0; 
      height: 0; 
      overflow: hidden; 
      z-index:-1;
      content:url('/toggle_air.png') url('/toggle_gas.png') url('/cirbtn_off.png') url('/cirbtn_on.png') url('/cirbtn_check.png') url('/cirbtn_warm.png');
    }
    h2 {color: #F8F4F4; margin: 5.0rem auto 3.0rem; text-decoration: underline;}
    h3 {color: #E6D00A;}
    h4 {color: #e8e6e6;}
    .modBtn{
      width: 70px;
      height: 35px;
      border: none;
      border-radius:0%;
      font-size: 0rem;
      background: url('/toggle_air.png') no-repeat;
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
      background: url('/cirbtn_off.png') no-repeat;
      cursor: pointer;}
    .formBox{
      margin: 0 auto;
      width: 19rem;
      padding: 0.2rem;
      border: 1px solid #a29797;
      border-radius: 0.25rem;
    }
    .inputW label, .inputW input{
      float:none;
      display: inline-block;
      vertical-align: right;
    }
    .inputW label{
      vertical-align: left;
    }
    .inputW input{
      vertical-align: right;
    }
    input, select, .submit{
      height: 1.5rem;
      -moz-box-sizing: border-box;
      box-sizing: border-box;
      border: 1px solid #a29797;
      border-color: #a29797;
      font: 1rem verdana;
      color: #726a6a;
      margin:0.5rem;
      text-align:left;
      resize: vertical;
    }    
    input, select{
      width: 10rem;
    }
    .submit{
      display: inline-block;
      width: auto;
      border-radius: 3px;
      background: #4472c7;
      color: #e8e6e6;
      text-align:center;
      font-weight: 400;
      cursor: pointer;
      transition: 0.5s;
      box-shadow: rgba(0, 0, 0, 0.15) 0px 5px 10px, rgba(0, 0, 0, 0.05) 0px 10px 15px;
    }
    .submit:active,
    .submit:hover,
    .submit:focus {
      background: #0d6efd;
      outline: 0;
    }
  </style>
  <title>IEUM IoT</title>
  </head>
  <body>
  <h2 align: center>NANO BUBBLE GENERATOR</h2>
)=====";
const char BODY_CTRL[] PROGMEM = R"=====(
  <br>
  <div>
    <button id='modBTN' class='modBtn' onclick='modClick()'>GAS</button>
  </div>
  <div>
    <button id='thingsBTN' class='thingsBtn' onclick='thingsClick()'>ON</button> 
  </div>
  <br>
)=====";
const char BODY_CONFIG1[] PROGMEM = R"=====(
  <div>
    <input type='button' class='submit' value='CONFIGURATION' onclick='configShow()'>
  </div>
  <div id='config' style='display:none'>
    <form name='apForm' class='formBox'>
    <h3 align: center>Access Point mode</h3>
      <div class='inputW'>
        <label><h4>Password</h4></label>
        <input type='password' name='ApPass' minlength='8' maxlength='8' value='00000000'>
      </div>
      <div>
        <input type='submit' class='submit' value='SUBMIT'>
      </div>
    </form>
    <form name='netForm' class='formBox'>
    <h3 align: center>Station mode</h3>
      <div class='inputW'>
        <label><h4>Network</h4></label>
        <select type='text' name='NetId' required>
          <option value=''>Select IoT Network</option>
)=====";
const char BODY_CONFIG2[] PROGMEM = R"=====(
        </select>
      </div>
      <div class='inputW'>
        <label><h4>Static IP</h4></label>
        <input type='text' name='NetIp' value='192.168.0.107'>
      </div>
      <div class='inputW'>
        <label><h4>Gateway</h4></label>
        <input type='text' name='NetGateWay' value='192.168.0.1'>
      </div>
      <div class='inputW'>
        <label><h4>Subnet</h4></label>
        <input type='text' name='NetSubNet' value='255.255.255.0'>
      </div>
      <div class='inputW'>
        <label><h4>password</h4></label>
        <input type='password' name='NetPass' maxlength='20' required>
      </div>
      <div>
        <input type='submit' class='submit' value='SUBMIT' onclick="ValidateInput(document.netForm.NetIp, document.netForm.NetGateWay, document.netForm.NetSubNet)">
        <input type='submit' class='submit' style='background-color:red;' value='DELETE' onclick='delInput()'">
      </div>
    </form>
  </div>

  <script type='text/javascript'>  
    let websocket;
    let wsUri = 'ws://' + window.location.hostname + ':81/';
    let mbtnID = document.getElementById('modBTN');
    let tbtnID = document.getElementById('thingsBTN');
    let sendTXT = '52C';
    let selectMod = '0';
    let selectReq = '0';

    function init(){
      websocket = new WebSocket(wsUri);
      websocket.onopen = function(evt) { onOpen(evt) };
      websocket.onclose = function(evt) { onClose(evt) };
      websocket.onmessage = function(evt) { onMessage(evt) };
      websocket.onerror = function(evt) { onError(evt) };
    }
    function onOpen(evt){
      //console.log("set initialization on socket open");
    }    
    function onClose(evt){
      if (event.wasClean) {
      } else {
        console.log('disconnection!')
      }
    }
    function selectGas(){
      mbtnID.style.backgroundImage = "url('/toggle_gas.png')";
      mbtnID.innerHTML = 'AIR';
    }
    function selectAir(){
      mbtnID.style.backgroundImage = "url('/toggle_air.png')";
      mbtnID.innerHTML = 'GAS';
    }
    function selectOn(){
      mbtnID.disabled = true;
      tbtnID.style.backgroundImage = "url('/cirbtn_on.png')";
      tbtnID.innerHTML = 'OFF';
    }
    function selectOff(){
      mbtnID.disabled = false;
      tbtnID.style.backgroundImage = "url('/cirbtn_off.png')";
      tbtnID.innerHTML = 'ON';
    }
    function modClick(){
      let mbtnText = mbtnID.textContent || mbtnID.innerText;
      if(mbtnText ==='GAS'){
        selectMod = '1';
        selectGas();
      }else{
        selectMod = '0';
        selectAir();
      }
    }
    function thingsClick(){
      let tbtnText = tbtnID.textContent || tbtnID.innerText;
      if(tbtnText ==='ON'){
        selectReq = '1';
        selectOn();        
      }else{
        selectReq = '0';
        selectOff();        
      }
      sendMsg();
    }
    function sendMsg(){
      sendTXT += selectMod;
      sendTXT += selectReq;
      sendTXT += ';';
      websocket.send(sendTXT);
      //console.log('web send msg: ', sendTXT);
      sendTXT = '52C';
    }
    function onMessage(evt){
      let btnObj = JSON.parse(evt.data);
      if(btnObj.MOD == 0){
        selectMod = '0';
        selectAir();
      }else if(btnObj.MOD == 1){
        selectMod = '1';
        selectGas();
      }
      if(btnObj.REQ == 0){
        selectReq = '0';
        selectOff();
      }else if(btnObj.REQ == 1){
        selectReq = '1';
        selectOn();
      }
      //console.log('web rcvd msg: ', btnObj.MOD, btnObj.REQ);
    }
    function onError(evt){
      console.log('error message!')
    }
    function configShow(){
      let config = document.getElementById('config')
      if(config.style.display=='none'){
        config.style.display = 'block';
      }else{
        config.style.display = 'none';
      }
    }
    function ValidateInput(input0, input1, input2){
      let ipformat = /^(25[0-5]|2[0-4][0-9]|[01]?[0-9][0-9]?)\.(25[0-5]|2[0-4][0-9]|[01]?[0-9][0-9]?)\.(25[0-5]|2[0-4][0-9]|[01]?[0-9][0-9]?)\.(25[0-5]|2[0-4][0-9]|[01]?[0-9][0-9]?)$/;
      let passw =  /^[A-Za-z]\w{7,14}$/;
      const ipArr = [input0, input1, input2];
      for(var i=0; i<3; i++){
        if(ipArr[i].value.match(ipformat)){
          ipArr[i].focus();
          return true;
        }else{
          alert("You have entered an invalid IP address!");
          ipArr[i].focus();
          return false;
        }
      }      
    }
    function delInput(){      
      alert("Delete all network information inputted. This machine'll be rebooted. Re-connect with AP mode(192.168.4.1), after rebooted.");
      window.location = '/netClear';
    }
    document.apForm.onsubmit = function(){
      let ApInfo = {};
      ApInfo['ApPass'] = this.ApPass.value;
      websocket.send(JSON.stringify(ApInfo));
      setTimeout(function(){
        alert('After reboot, Your AP Password was changed.');
      }, 500);
      return false;
    }    
    document.netForm.onsubmit = function(){
      let NetInfo = {};
      NetInfo['NetId'] = this.NetId.value;
      NetInfo['NetIp'] = this.NetIp.value;
      NetInfo['NetGateWay'] = this.NetGateWay.value;
      NetInfo['NetSubNet'] = this.NetSubNet.value;
      NetInfo['NetPass'] = this.NetPass.value;
      websocket.send(JSON.stringify(NetInfo));
      alert('After reboot, Connect the IP: ' + NetInfo['NetIp'] + 'on network \'' + NetInfo['NetId'] + '\'.');
      return false;
    } 
    window.addEventListener('load', init, false);
  </script>
  </html>
)=====";
