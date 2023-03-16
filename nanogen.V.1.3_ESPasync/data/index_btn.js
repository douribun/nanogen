let ws;
let gateWay = 'ws://' + window.location.hostname + '/ws';
let mbtnID = document.getElementById('modBTN');
let tbtnID = document.getElementById('thingsBTN');
let divConf = document.getElementById('confDiv');
let sendTXT = '52C';
let selectMod = '0';
let selectReq = '0';
let mcuPASS;

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
  divConf.style.display = 'none';
  tbtnID.style.backgroundImage = 'url(\'cirbtn_on.png\')';
  tbtnID.value = 'OFF';
}
function selectOff(){
  mbtnID.disabled = false;
  divConf.style.display = 'block';
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
  //console.log('rcvd msg from MCU: ', rcvObj);
  if(rcvObj.mcuPASS){
    mcuPASS = rcvObj.mcuPASS;
  }
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
function goConfig() {
  let goodURL = '/conf'  //이곳에 인증이 되었을때 이동할 페이지  입력
      alert("패스워드를 입력하셔야 합니다.")
  let password = prompt("PASSWD 입력","")
    if (password == null)  {
      alert("패스워드가 다릅니다.")
      location = '/'         // 실패시 이동 주소       history.back();를 넣어도 됨
    }else{
      let combo = password
      let total = combo.toLowerCase()
    if  (total == mcuPASS)  {                // 비밀번호
      //alert("Configuration Page입니다.")
      location = goodURL
    }else{
      alert("관리자에게 문의하세요.")
      location = '/'    // 실패시 이동 주소      history.back();를 넣어도 됨
    }
  }
} 

window.addEventListener('load', init, false);
