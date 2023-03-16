let ws;
let gateWay = 'ws://' + window.location.hostname + '/ws'; 
let mbtnID = document.getElementById('modBTN');
let tbtnID = document.getElementById('thingsBTN');
let divOper = document.getElementById('operDiv');
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
  //pbtnID.style.display = 'none';
  //nbtnID.style.display = 'none';
}
function selectOff(){
  mbtnID.disabled = false;
  tbtnID.style.backgroundImage = 'url(\'cirbtn_off.png\')';
  tbtnID.value = 'ON';
  //pbtnID.style.display = 'block';
  //nbtnID.style.display = 'block';
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
  let tbtnText = tbtnID.value;
  if(tbtnText ==='ON'){
    selectReq = '1';
    selectOn();    
  }else{
    selectReq = '0';
    selectOff();    
  }
  sendBtnMsg();
}
function sendBtnMsg(){
  sendTXT += selectMod;
  sendTXT += selectReq;
  sendTXT += ';';
  ws.send(sendTXT);
  //console.log('web send msg: ', sendTXT);
  sendTXT = '52C';
}

function onMessage(evt){
  let rcvObj = JSON.parse(evt.data);  //the text format with websocket is UTF-8.
  console.log('rcvd msg from MCU: ', rcvObj);
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
function goConfig() {
  let goodURL  = '/conf'  //이곳에 인증이 되었을때 이동할 페이지  입력
      alert("패스워드를 입력하셔야 합니다.")
  let password =  prompt("PASSWD 입력","")
    if (password == null)  {
      alert("패스워드가 다릅니다.")
      location  = '/'         // 실패시 이동 주소       history.back();를 넣어도 됨
    }else{
      let  combo =  password
      let  total =  combo.toLowerCase()
    if  (total == "1234")  {                // 비밀번호
      //alert("Configuration Page입니다.")
      location  =  goodURL
    }else{
      alert("관리자에게 문의하세요.")
      location  = '/'    // 실패시 이동 주소      history.back();를 넣어도 됨
    }
  }
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

async function netShow(){
  ws.send('WiFiscan');
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
  //console.log(JSON.stringify(ApInfo));
  //setTimeout(function(){
  //  alert('When you turn this machine off and on, your AP Password will be changed.');
  //}, 500);
  if(!confirm('새로운 비밀번호를 입력했습니다. 계속하시겠습니까?')){
    alert('취소하셨습니다.');
  }else{
    alert('기계의 전원을 껐다 켠 후 바뀐 비밀번호가 적용됩니다.');
    ws.send(JSON.stringify(ApInfo));
  }
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
  //console.log(JSON.stringify(NetInfo));
  
  //alert('Connect the IP: ' + NetInfo['NetIp'] + ' on network\'' + NetInfo['NetId'] + '\', when you turn this machine off and on.\n');
  if(!confirm('새로운 네크워크 정보를 입력했습니다. 계속하시겠습니까?')){
    alert('취소하셨습니다.');
  }else{
    alert('전원을 껐다 켠 후 바뀐 네트웤크정보가 적용됩니다. \'네트워크: \'' + NetInfo['NetId'] + '\', IP: \'' + NetInfo['NetIp'] +'\'');
    ws.send(JSON.stringify(NetInfo));
  }
  divOper.style.display = 'block';
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
