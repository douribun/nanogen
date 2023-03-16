let ws;
let gateWay = 'ws://' + window.location.hostname + '/ws';
let divPass = document.getElementById('passDiv');    
let divNet = document.getElementById('netDiv');
let selWiFi = document.getElementById('selNet');
let pbtnID = document.getElementById('passBTN');
let nbtnID = document.getElementById('netBTN');
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

function onMessage(evt){
  let rcvObj = JSON.parse(evt.data);  //the text format with websocket is UTF-8.
  console.log('rcvd msg from MCU: ', rcvObj);
  if(rcvObj.SSID){
    let len = rcvObj.SSID.length;
    //console.log('rcvObj length: (' + len + ')');
    if(selWiFi.children.length == 1){      
      for (var i = 0; i < len; i++) {
        let opt = document.createElement('option');
        //console.log(rcvObj.SSID[i]);
        opt.setAttribute('value', rcvObj.SSID[i]);
        opt.text = rcvObj.SSID[i];
        selWiFi.options.add(opt);
      }
    }
  }
}
function onError(evt){
  console.log('error message!')
}

function passShow(){
  divNet.style.display = 'none';
  if(divPass.style.display=='none'){
    divPass.style.display = 'block';
  }else{
    divPass.style.display = 'none';
  }
}

function netShow(){ //async 
  ws.send('WiFiscan');
  divPass.style.display = 'none';
  if(divNet.style.display=='none'){  
    divNet.style.display = 'block';
  }else{
    divNet.style.display = 'none';
  }
}

function validatePASS(pass_txt, id_input){
  var pwformat = /^[0-9]{8,11}$/;
  if(pwformat.test(pass_txt)==true){
    return false;
  } else {
    alert('**keep the password format.\nEnter only number, no more than 4~11 digits.');
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
    document.getElementById(iput_id).value = '';
    document.getElementById(iput_id).focus();
    return true;
  }
}

document.forms.apform.onsubmit = function(){
  let ApInfo = {};
  ApInfo['ApPass'] = this.ApPass.value;
  if(!confirm('새로운 비밀번호를 입력했습니다. 계속하시겠습니까?')){
    alert('취소하셨습니다.');
  }else{
    alert('기계의 전원을 껐다 켠 후 바뀐 비밀번호가 적용됩니다.');
    ws.send(JSON.stringify(ApInfo));
  }
  divPass.style.display = 'none';
  divNet.style.display = 'none';
  return false;
}

document.forms.staForm.onsubmit = function(){
  let NetInfo = {};
  NetInfo['NetId'] = this.NetId.value;
  NetInfo['NetIp'] = this.NetIp.value;
  NetInfo['NetGateWay'] = this.NetGateWay.value;
  NetInfo['NetSubNet'] = this.NetSubNet.value;
  NetInfo['NetPass'] = this.NetPass.value;
  //console.log(JSON.stringify(NetInfo));
  if(!confirm('새로운 네크워크 정보를 입력했습니다. 계속하시겠습니까?')){
    alert('취소하셨습니다.');
  }else{
    alert('전원을 껐다 켠 후 바뀐 네트웤크정보가 적용됩니다. \'네트워크: \'' + NetInfo['NetId'] + '\', IP: \'' + NetInfo['NetIp'] +'\'');
    ws.send(JSON.stringify(NetInfo));
  }
  divPass.style.display = 'none';
  divNet.style.display = 'none';
  return false;
}

function delAPInput(){
  if(!confirm('기기의 비밀번호를 \'공장초기화\'합니다. 계속하시겠습니까?')){
    alert('취소하셨습니다.');
  }else{
    alert('전원을 껐다 켠 후 설정하셨던 기기의 비밀번호는 삭제되고 \'00000000\'으로 초기화됩니다.');
    ws.send('apClear');
  }  
}
function delSTAInput(){
  if(!confirm('기계를 껐다 켜면 네트워크 정보를 \'공장초기화\'합니다. 계속하시겠습니까?')){
    alert('취소하셨습니다.');
  }else{
    alert('전원을 껐다 켜면 기존의 WiFi네트워크 정보를 모두 삭제합니다. 이후엔 AP mode로 연결하세요.');
    ws.send('staClear');
  }  
}

window.addEventListener('load', init, false);
